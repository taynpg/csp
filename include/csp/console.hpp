// csp - 奇门遁甲排盘库
// 终端输出：Windows 控制台按本地代码页解释字节，直接写 UTF-8 会乱码，
// 所以给 std::cout / std::cerr 换一个 u8ConsoleBuf；类 Unix 系统本来就认 UTF-8，
// 这里不做任何处理，直接用标准流。
#ifndef CSP_CONSOLE_HPP
#define CSP_CONSOLE_HPP

#include <string_view>

namespace csp::console {

/// 初始化终端：Windows 下开启 ANSI 转义支持，并把 std::cout / std::cerr / std::clog
/// 换成 UTF-8 适配的 streambuf；其他系统什么都不做。可重复调用。
///
/// 只对「标准流」生效。若进程里还有 printf 之类的 C 调用，它们仍按字节写出。
/// 作为库被嵌入（如 Python 绑定）时不要调用，改用 enable_ansi()。
void init();

/// 只开启 Windows 控制台的 ANSI 转义支持，不接管标准流。可重复调用。
void enable_ansi();

/// 标准输出当前是否连着终端（用于决定要不要输出 ANSI 颜色）
bool stdout_is_console();

/// 输出到标准输出并刷新
void write(std::string_view text);

/// 输出到标准错误并刷新（提示与错误信息，重定向时不会混进结果）
void write_error(std::string_view text);

}   // namespace csp::console

#endif   // CSP_CONSOLE_HPP
