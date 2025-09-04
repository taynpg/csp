// Based on https://gitee.com/whatitis/ColorfulConsoleIO/ modification

#ifndef COLOR_CONSOLE_HPP
#define COLOR_CONSOLE_HPP

#include <iostream>
#include <string>

#if defined(_WIN32)
#include <windows.h>
#endif   // OS_TYPE_WINDOWS_CC

// Enum class for console text color
enum class ConsoleColor {
    Black = 30,
    Red = 31,
    Green = 32,
    Yellow = 33,
    Blue = 34,
    Magenta = 35,
    Cyan = 36,
    White = 37,
    Default = 39,

    BrightBlack = 90,
    BrightRed = 91,
    BrightGreen = 92,
    BrightYellow = 93,
    BrightBlue = 94,
    BrightMagenta = 95,
    BrightCyan = 96,
    BrightWhite = 97
};

// Enum class for console background color
enum class ConsoleBackgroundColor {
    Black = 40,
    Red = 41,
    Green = 42,
    Yellow = 43,
    Blue = 44,
    Magenta = 45,
    Cyan = 46,
    White = 47,
    Default = 49,

    BrightBlack = 100,
    BrightRed = 101,
    BrightGreen = 102,
    BrightYellow = 103,
    BrightBlue = 104,
    BrightMagenta = 105,
    BrightCyan = 106,
    BrightWhite = 107
};
#if defined(_WIN32)
// Get the Windows color code for a given ConsoleColor
inline WORD GetColorCode(ConsoleColor color)
{
    switch (color) {
    case ConsoleColor::Black:
        return 0;
    case ConsoleColor::Red:
        return FOREGROUND_RED;
    case ConsoleColor::Green:
        return FOREGROUND_GREEN;
    case ConsoleColor::Yellow:
        return FOREGROUND_RED | FOREGROUND_GREEN;
    case ConsoleColor::Blue:
        return FOREGROUND_BLUE;
    case ConsoleColor::Magenta:
        return FOREGROUND_BLUE | FOREGROUND_RED;
    case ConsoleColor::Cyan:
        return FOREGROUND_BLUE | FOREGROUND_GREEN;
    case ConsoleColor::White:
        return FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN;
    case ConsoleColor::Default:
        return FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN;
    case ConsoleColor::BrightBlack:
        return 0 | FOREGROUND_INTENSITY;
    case ConsoleColor::BrightRed:
        return FOREGROUND_RED | FOREGROUND_INTENSITY;
    case ConsoleColor::BrightGreen:
        return FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    case ConsoleColor::BrightYellow:
        return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    case ConsoleColor::BrightBlue:
        return FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    case ConsoleColor::BrightMagenta:
        return FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY;
    case ConsoleColor::BrightCyan:
        return FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    case ConsoleColor::BrightWhite:
        return FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    default:
        return FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN;
    }
}
#else
// Get the ANSI escape code for a given ConsoleColor
inline std::string GetColorCode(ConsoleColor color)
{
    switch (color) {
    case ConsoleColor::Black:
        return "\033[30m";
    case ConsoleColor::Red:
        return "\033[31m";
    case ConsoleColor::Green:
        return "\033[32m";
    case ConsoleColor::Yellow:
        return "\033[33m";
    case ConsoleColor::Blue:
        return "\033[34m";
    case ConsoleColor::Magenta:
        return "\033[35m";
    case ConsoleColor::Cyan:
        return "\033[36m";
    case ConsoleColor::White:
        return "\033[37m";
    case ConsoleColor::Default:
        return "\033[39m";
    case ConsoleColor::BrightBlack:
        return "\033[90m";
    case ConsoleColor::BrightRed:
        return "\033[91m";
    case ConsoleColor::BrightGreen:
        return "\033[92m";
    case ConsoleColor::BrightYellow:
        return "\033[93m";
    case ConsoleColor::BrightBlue:
        return "\033[94m";
    case ConsoleColor::BrightMagenta:
        return "\033[95m";
    case ConsoleColor::BrightCyan:
        return "\033[96m";
    case ConsoleColor::BrightWhite:
        return "\033[97m";
    default:
        return "\033[39m";
    }
}
#endif

#if defined(_WIN32)
// Get the Windows color code for a given ConsoleBackgroundColor
inline WORD GetBackgroundColorCode(ConsoleBackgroundColor color)
{
    switch (color) {
    case ConsoleBackgroundColor::Black:
        return 0;
    case ConsoleBackgroundColor::Red:
        return BACKGROUND_RED;
    case ConsoleBackgroundColor::Green:
        return BACKGROUND_GREEN;
    case ConsoleBackgroundColor::Yellow:
        return BACKGROUND_RED | BACKGROUND_GREEN;
    case ConsoleBackgroundColor::Blue:
        return BACKGROUND_BLUE;
    case ConsoleBackgroundColor::Magenta:
        return BACKGROUND_BLUE | BACKGROUND_RED;
    case ConsoleBackgroundColor::Cyan:
        return BACKGROUND_BLUE | BACKGROUND_GREEN;
    case ConsoleBackgroundColor::White:
        return BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN;
    case ConsoleBackgroundColor::Default:
        return 0;
    case ConsoleBackgroundColor::BrightBlack:
        return 0 | BACKGROUND_INTENSITY;
    case ConsoleBackgroundColor::BrightRed:
        return BACKGROUND_RED | BACKGROUND_INTENSITY;
    case ConsoleBackgroundColor::BrightGreen:
        return BACKGROUND_GREEN | BACKGROUND_INTENSITY;
    case ConsoleBackgroundColor::BrightYellow:
        return BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
    case ConsoleBackgroundColor::BrightBlue:
        return BACKGROUND_BLUE | BACKGROUND_INTENSITY;
    case ConsoleBackgroundColor::BrightMagenta:
        return BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY;
    case ConsoleBackgroundColor::BrightCyan:
        return BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
    case ConsoleBackgroundColor::BrightWhite:
        return BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
    default:
        return 0;
    }
}
#else
// Get the ANSI escape code for a given ConsoleBackgroundColor
inline std::string GetBackgroundColorCode(ConsoleBackgroundColor color)
{
    switch (color) {
    case ConsoleBackgroundColor::Black:
        return "\033[40m";
    case ConsoleBackgroundColor::Red:
        return "\033[41m";
    case ConsoleBackgroundColor::Green:
        return "\033[42m";
    case ConsoleBackgroundColor::Yellow:
        return "\033[43m";
    case ConsoleBackgroundColor::Blue:
        return "\033[44m";
    case ConsoleBackgroundColor::Magenta:
        return "\033[45m";
    case ConsoleBackgroundColor::Cyan:
        return "\033[46m";
    case ConsoleBackgroundColor::White:
        return "\033[47m";
    case ConsoleBackgroundColor::Default:
        return "\033[49m";
    case ConsoleBackgroundColor::BrightBlack:
        return "\033[100m";
    case ConsoleBackgroundColor::BrightRed:
        return "\033[101m";
    case ConsoleBackgroundColor::BrightGreen:
        return "\033[102m";
    case ConsoleBackgroundColor::BrightYellow:
        return "\033[103m";
    case ConsoleBackgroundColor::BrightBlue:
        return "\033[104m";
    case ConsoleBackgroundColor::BrightMagenta:
        return "\033[105m";
    case ConsoleBackgroundColor::BrightCyan:
        return "\033[106m";
    case ConsoleBackgroundColor::BrightWhite:
        return "\033[107m";
    default:
        return "\033[49m";
    }
}
#endif

// Operator overloading for console text color
inline std::ostream& operator<<(std::ostream& os, ConsoleColor data)
{
#if defined(_WIN32)
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, GetColorCode(data));
#else
    std::cout << GetColorCode(data);
#endif
    return os;
}

// Operator overloading for console background color
inline std::ostream& operator<<(std::ostream& os, ConsoleBackgroundColor data)
{
#if defined(_WIN32)
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, GetBackgroundColorCode(data));
#else
    std::cout << GetBackgroundColorCode(data);
#endif
    return os;
}

#endif