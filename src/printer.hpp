#ifndef COLORUTILS_H
#define COLORUTILS_H

#include <cstring>
#include <string>
#include <map>
#include "shell.hpp"
#include <iostream>
#include <unordered_map>

#define ESCAPE "\u001b["
#define DEFAULT '9'
#define BG_STD_ '4'
#define FG_STD_ '3'
#define BG_256_ "48;5;"
#define FG_256_ "38;5;"


struct printer
{
    printer() = delete;

    using replace_map = std::pair<std::string, std::map<char, std::string>>;

    static inline std::string init;
    static inline std::string stop;
    static inline std::string wrap;
    static inline std::string unwrap;
    static inline std::string endl;
    static inline replace_map escapes;
    
    static void mode(shell_t id)
    {
        switch (id)
        {
        case shell_t::bash:
            wrap   = "\\[";
            unwrap = "\\]";
            endl   = "\n";
            escapes = {"$`\\", {
                {'`', "\\`"},
                {'$', "\\$"},
                {'\\', "\\\\"}
            }};
            break;

        case shell_t::csh:
            wrap   = "%{";
            unwrap = "%}";
            endl   = " \\n";
            escapes = {"%!", {
                {'%', "%%" },
                {'!', "\\!"}
            }};
            break;

        case shell_t::zsh:
            wrap   = "%{";
            unwrap = "%}";
            endl   = "\n";
            escapes = {"%", {
                {'%', "%%" }
            }};
            break;

        case shell_t::ksh:
            escapes = {"!", {
                {'!', "!!" }
            }};
            endl   = "\n";
            break;

        case shell_t::fish:
        case shell_t::ps:
        default:
            endl   = "\n";
            break;
        }
        wrap_mode(true);
    };

    static void wrap_mode(bool yes)
    {
        if (yes) {
            init = wrap;
            init += ESCAPE;
            stop = unwrap;
        } else {
            init = ESCAPE;
            stop = "";
        }
    }

    static inline std::string escape(const std::string& what)
    {
        std::string output;
        output.reserve(what.length());

        std::string::size_type cursor = 0;
        std::string::size_type item;

        while((item = what.find_first_of(escapes.first, cursor))
               != std::string::npos)
        {
            output.append(what, cursor, item - cursor);
            output += escapes.second.at(what[item]);
            cursor = item + 1;
        }

        output += what.substr(cursor);
        return output;
    }

    static inline std::string escape(char c)
    {
        if (escapes.first.find(c) != std::string::npos)
        {
            return escapes.second[c];
        }
        return std::string(1, c);
    }

    static inline std::string bg(int value)
    {
        std::string s(init);
        if (value < 0) {
            s += BG_STD_;
            s += '9';
        } else {
            s += BG_256_;
            s += std::to_string(value);
        }
        s += 'm';
        s += stop;
        return s;
    }

    static inline std::string fg(int value)
    {
        std::string s(init);
        if (value < 0) {
            s += FG_STD_;
            s += '9';
        } else {
            s += FG_256_;
            s += std::to_string(value);
        }
        s += 'm';
        s += stop;
        return s;
    }

    static inline std::string reset()
    {
        return init + "0m" + stop;
    }

    static inline std::string cup(size_t i)
    {
        if (i <= 0)
            return init + 'G' + stop;
        else
        return init + std::to_string(i) + 'G' + stop;
    }

    static inline std::string font(const char* str)
    {
        static
        std::unordered_map<std::string, int> y =
        {
            {"bold",      1},
            {"faint",     2},
            {"italic",    3},
            {"underline", 4},
            {"blink",     5},
            {"rblink",    6},
            {"reversed",  7},
            {"conceal",   8},
            {"crossed",   9},
        };
        if (auto code = y.find(str); code != y.end())
            return init + std::to_string(code->second) + 'm' + stop;
        return "";
    }
    // static inline std::string disable(const char* str)
    // {
    //     static
    //     std::unordered_map<std::string, int> y =
    //     {
    //         {"bold",      22},
    //         {"faint",     22},
    //         {"italic",    23},
    //         {"underline", 4},
    //         {"blink",     5},
    //         {"rblink",    6},
    //         {"reversed",  7},
    //         {"conceal",   8},
    //         {"crossed",   9},
    //     };
    //     if (auto code = y.find(str); code != y.end())
    //         return init + std::to_string(code->second) + 'm' + stop;
    //     return "";
    // }
};





#endif
