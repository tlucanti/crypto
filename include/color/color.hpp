
#ifndef COLOR_HPP
# define COLOR_HPP

# include <string>
# include <sstream>

namespace color {
    constexpr const char *Red = "\033[1;31m";
    constexpr const char *Green = "\033[1;32m";
    constexpr const char *Yellow = "\033[1;33m";
    constexpr const char *Blue = "\033[1;34m";
    constexpr const char *Purple = "\033[1;35m";
    constexpr const char *Cyan = "\033[1;36m";
    constexpr const char *White = "\033[1;37m";
    constexpr const char *Reset = "\033[0m";

    template <class value_type, class color_type>
    std::string Color(const value_type &val, const color_type &col)
    {
        return (std::stringstream() << col << val << Reset).str();
    }

    template <class value_type>
    std::string R(const value_type &val)
    {
        return Color(val, Red);
    }

    template <class value_type>
    std::string G(const value_type &val)
    {
        return Color(val, Green);
    }

    template <class value_type>
    std::string Y(const value_type &val)
    {
        return Color(val, Yellow);
    }

    template <class value_type>
    std::string B(const value_type &val)
    {
        return Color(val, Blue);
    }

    template <class value_type>
    std::string P(const value_type &val)
    {
        return Color(val, Purple);
    }

    template <class value_type>
    std::string C(const value_type &val)
    {
        return Color(val, Cyan);
    }

    template <class value_type>
    std::string W(const value_type &val)
    {
        return Color(val, White);
    }
}

#endif /* COLOR_HPP */
