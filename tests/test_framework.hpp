// csp - 极简测试框架：不引入第三方依赖，够用即可。
#ifndef CSP_TESTS_FRAMEWORK_HPP
#define CSP_TESTS_FRAMEWORK_HPP

#include <cstdio>
#include <string>
#include <string_view>
#include <vector>

namespace testing {

struct Case {
    const char* name;
    void (*body)();
};

inline std::vector<Case>& cases()
{
    static std::vector<Case> registry;
    return registry;
}

inline int& failure_count()
{
    static int count = 0;
    return count;
}

inline std::vector<std::string>& failure_messages()
{
    static std::vector<std::string> messages;
    return messages;
}

struct Registrar {
    Registrar(const char* name, void (*body)())
    {
        cases().push_back({name, body});
    }
};

inline void report_failure(const char* file, int line, const std::string& message)
{
    ++failure_count();
    failure_messages().push_back(std::string(file) + ":" + std::to_string(line) + ": " + message);
}

inline std::string to_text(int value)
{
    return std::to_string(value);
}

inline std::string to_text(bool value)
{
    return value ? "true" : "false";
}

inline std::string to_text(const std::string& value)
{
    return value;
}

inline std::string to_text(std::string_view value)
{
    return std::string(value);
}

inline std::string to_text(const char* value)
{
    return value;
}

inline int run_all()
{
    for (const auto& item : cases()) {
        const int before = failure_count();
        item.body();
        std::printf("%s %s\n", failure_count() == before ? "[ OK ]" : "[FAIL]", item.name);
    }
    if (!failure_messages().empty()) {
        std::printf("\n");
        for (const auto& message : failure_messages()) {
            std::printf("  %s\n", message.c_str());
        }
    }
    std::printf("\n%zu 个用例，%d 个断言失败\n", cases().size(), failure_count());
    return failure_count() == 0 ? 0 : 1;
}

}   // namespace testing

#define CSP_TEST(name)                                                                                                 \
    static void name();                                                                                                \
    static ::testing::Registrar name##_registrar(#name, &name);                                                        \
    static void name()

#define CHECK(cond)                                                                                                    \
    do {                                                                                                               \
        if (!(cond)) {                                                                                                 \
            ::testing::report_failure(__FILE__, __LINE__, "CHECK(" #cond ") 失败");                                    \
        }                                                                                                              \
    } while (false)

#define CHECK_EQ(actual, expected)                                                                                     \
    do {                                                                                                               \
        const auto actual_value = (actual);                                                                            \
        const auto expected_value = (expected);                                                                        \
        if (!(actual_value == expected_value)) {                                                                       \
            ::testing::report_failure(__FILE__, __LINE__,                                                              \
                                      std::string("CHECK_EQ(" #actual ", " #expected ") 失败：实际 ") +                \
                                          ::testing::to_text(actual_value) + "，期望 " +                               \
                                          ::testing::to_text(expected_value));                                         \
        }                                                                                                              \
    } while (false)

#endif   // CSP_TESTS_FRAMEWORK_HPP
