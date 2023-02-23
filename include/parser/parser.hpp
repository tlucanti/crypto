
#ifndef PARSER_HPP
# define PARSER_HPP

# include <crypto/crypto.h>
# include <color/color.hpp>
# include <iostream>
# include <string>
# include <vector>
# include <stdexcept>
# include <fstream>
# include <sstream>

# define array_size(array) (sizeof(array) / sizeof((array)[0]))

struct HashType {
    const char *name;
    const unsigned char *(*hash_func)(const char *, size_t);
    const char *(*hexdigest)(const unsigned char *);
};

struct ArgUnit {
    std::string source;
    std::string str;
    std::string hash;
};

struct Args {
    const HashType *hash_type;
    bool ignore_stdin;
    bool p_flag;
    bool q_flag;
    bool r_flag;
};

inline constexpr HashType hash_types[] = {
    {"MD5", md5, md5_hexdigest},
    {"SHA3_224", sha3_224, sha3_224_hexdigest},
    {"SHA3_256", sha3_256, sha3_256_hexdigest},
    {"SHA3_384", sha3_384, sha3_384_hexdigest},
    {"SHA3_512", sha3_512, sha3_512_hexdigest}
};

namespace parser {
    std::string read_file_text(const std::string &fname);
    std::string read_stdin_text();
    void print_help();
    void argparse(int argc, char **argv, Args &args, std::vector<ArgUnit> &units);
}

#endif /* PARSER_HPP */

