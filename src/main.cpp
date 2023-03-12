
#include <parser/parser.hpp>

using namespace std::string_literals;

namespace parser {

bool strcaseeq(const std::string &s1, const std::string &s2)
{
    if (s1.size() != s2.size()) {
        return false;
    }
    for (size_t i = 0; i < s1.size(); ++i) {
        if (std::tolower(s1[i]) != std::tolower(s2[i])) {
            return false;
        }
    }
    return true;
}

const HashType *find_hash_type(const std::string &name)
{
    for (size_t i = 0; i < array_size(hash_types); ++i) {
        if (parser::strcaseeq(hash_types[i].name, name)) {
            return hash_types + i;
        }
    }
    return nullptr;
}

std::string read_file_text(const std::string &fname)
{
    std::ifstream stream(fname);
    std::ostringstream sstr;

    if (not stream) {
        throw std::invalid_argument("file (" + fname + ") not found");
    }
    sstr << stream.rdbuf();
    return sstr.str();
}

std::string read_stdin_text()
{
    std::ostringstream sstr;

    sstr << std::cin.rdbuf();
    return sstr.str();
}

void print_help()
{
    std::cout << "usage:\n";
    std::cout << "./crypto (hash_algo) [[flags]] [[files]]\n";
    std::cout << "  (hash_algo):\n";
    std::cout << "    md5/sha3_224/sha3_256/sha3_384/sha3_512\n";
    std::cout << "  [[flags]]:\n";
    std::cout << "    -p: print stdin to stdout\n";
    std::cout << "    -q: print only result hash\n";
    std::cout << "    -r: reverse output format\n";
    std::cout << "    -s (str):\n";
    std::cout << "      -s: print hash of given string\n";
    std::cout << "      (str): source str\n";
    std::cout << "  [[files]]:\n";
    std::cout << "    names of files whos contents will be hashed\n";
}

void argparse(size_t argc, char **argv)
{
    Args args;
    std::vector<ArgUnit> units;

    if (argc == 0) {
        parser::interactive();
    } else {
        parser::cli(argc, argv, args, units);
    }
    parser::hash_units(args, units);
}

void interactive()
{
    while (std::cin) {
        std::string line;
        std::getline(std::cin, line, '\n');
        std::stringstream ss(line);
        std::vector<std::string> argv(1);
        while (true) {
            std::string token;
            ss >> token;
            if (token.empty()) {
                break ;
            }
            argv.push_back(token);
        }

        Args args;
        std::vector<ArgUnit> units;

        parser::cli(argv.size(), argv, args, units);
        parser::hash_units(args, units);
    }
}

template <class args_type>
void cli(size_t argc, args_type argv, Args &args, std::vector<ArgUnit> &units)
{
    args.p_flag = false;
    args.q_flag = false;
    args.r_flag = false;
    args.ignore_stdin = false;

    if (argc == 1) {
        parser::interactive();
        return ;
    }
    if (argv[1] == "--help"s) {
        parser::print_help();
        return ;
    } else if (argv[1][0] == '-') {
        throw std::invalid_argument(
            "expected hash type for first argument, run with --help option");
    } else if ((args.hash_type = parser::find_hash_type(argv[1]))) {
        /* nothing */
    } else {
        throw std::invalid_argument("unknown hash type: ("s + argv[1]
            + ") run with --help option");
    }
    for (size_t i = 2; i < argc; ++i) {
        if (argv[i][0] != '-') {
            units.push_back({argv[i], parser::read_file_text(argv[i]), ""});
            args.ignore_stdin = true;
        } else if (argv[i] == "-p"s) {
            args.p_flag = true;
        } else if (argv[i] == "-q"s) {
            args.q_flag = true;
        } else if (argv[i] == "-r"s) {
            args.r_flag = true;
        } else if (argv[i] == "-s"s) {
            if (i == argc - 1) {
                throw std::invalid_argument(
                    "expected (str) value after -s argument");
            }
            ++i;
            units.push_back({argv[i], argv[i], ""});
            args.ignore_stdin = true;
        } else {
            throw std::invalid_argument("unknown flag: ("s + argv[i]
                + ") run with --help option");
        }
    }
    if (args.p_flag) {
        args.ignore_stdin = false;
    }
    if (not args.ignore_stdin) {
        units.insert(units.begin(), {"stdin", parser::read_stdin_text(), ""});
    }
}

void hash_units(const Args &args, const std::vector<ArgUnit> &units)
{
    const unsigned char *hash;
    const char *digest;


    if (args.p_flag) {
        std::cout << units.front().str << '\n';
    }
    for (const auto &unit : units) {
        if (not args.q_flag and not args.r_flag) {
            std::cout << args.hash_type->name << '(' << unit.source << "): ";
        }
        hash = args.hash_type->hash_func(unit.str.c_str(), unit.str.size());
        digest = args.hash_type->hexdigest(hash);
        std::cout << digest;
        if (not args.q_flag and args.r_flag) {
            std::cout << ' ' << unit.source;
        }
        std::cout << '\n';
    }
}

} /* parser */

int main(int argc, char **argv)
{
    try {
        parser::argparse(static_cast<size_t>(argc), argv);
    } catch (const std::invalid_argument &exc) {
        std::cout << color::R("Error: ") << exc.what() << '\n';
    }
}
