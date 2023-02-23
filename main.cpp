
#include <parser/parser.hpp>

using namespace std::string_literals;

namespace parser {

const HashType *find_hash_type(const std::string &name)
{
    for (int i = 0; i < array_size(hash_types); ++i) {
        if (hash_types[i].name == name) {
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

void argparse(int argc, char **argv, Args &args, std::vector<ArgUnit> &units)
{
    args.p_flag = false;
    args.q_flag = false;
    args.r_flag = false;
    args.ignore_stdin = false;

    if (argc == 1) {
        throw std::invalid_argument("run with --help option");
    }
    if (argv[1] == "--help"s) {
        parser::print_help();
        exit(0);
    } else if (argv[1][0] == '-') {
        throw std::invalid_argument(
            "expected hash type for first argument, run with --help option");
    } else if ((args.hash_type = parser::find_hash_type(argv[1]))) {
        /* nothing */
    } else {
        throw std::invalid_argument("unknown hash type: ("s + argv[1]
            + ") run with --help option");
    }
    for (int i = 2; i < argc; ++i) {
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
    const char *hash_name;

    if (args.p_flag) {
        std::cout << units.front().str << '\n';
    }
    for (const auto &unit : units) {
        if (not args.q_flag and not args.r_flag) {
            std::cout << hash_name << '(' << unit.source << "): ";
        }
        std::cout << unit.str;
        if (not args.q_flag and args.r_flag) {
            std::cout << ' ' << unit.source;
        }
        std::cout << '\n';
    }
}

} /* parser */

int main(int argc, char **argv)
{
    Args args;
    std::vector<ArgUnit> units;

    try {
        parser::argparse(argc, argv, args, units);
    } catch (const std::invalid_argument &exc) {
        std::cout << color::R("Error: ") << exc.what() << '\n';
    }
    parser::hash_units(args, units);
}
