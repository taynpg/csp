#include "csp/console.hpp"

#include <algorithm>
#include <cstdio>
#include <iostream>
#include <streambuf>
#include <string>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace csp::console {

namespace {

#ifdef _WIN32

bool is_console(HANDLE handle)
{
    DWORD mode = 0;
    return handle != nullptr && handle != INVALID_HANDLE_VALUE && GetConsoleMode(handle, &mode) != 0;
}

bool is_high_surrogate(wchar_t ch)
{
    return ch >= 0xD800 && ch <= 0xDBFF;
}

/// 写控制台。单次 WriteConsoleW 不宜给太大缓冲（部分系统上限 64K 字符），
/// 这里分段写，并且不把代理对拆到两次调用之间。
void write_console(HANDLE handle, std::wstring_view text)
{
    constexpr std::size_t kChunk = 4096;
    std::size_t offset = 0;
    while (offset < text.size()) {
        std::size_t count = std::min(kChunk, text.size() - offset);
        if (offset + count < text.size() && is_high_surrogate(text[offset + count - 1])) {
            --count;
        }
        DWORD written = 0;
        if (WriteConsoleW(handle, text.data() + offset, static_cast<DWORD>(count), &written, nullptr) == FALSE ||
            written == 0) {
            return;   // 句柄失效（例如运行中被重定向），交给调用方兜底
        }
        offset += written;
    }
}

/// 把 UTF-8 字节转成宽字符写控制台；'\n' 补成 CRLF，避免控制台吃掉行首。
/// last_cr 记录上一次写出的字符是不是 '\r'，避免跨调用时补出 "\r\r\n"。
bool write_utf8(HANDLE handle, std::string_view text, bool& last_cr)
{
    if (text.empty()) {
        return true;
    }
    const int size = static_cast<int>(text.size());
    const int length = MultiByteToWideChar(CP_UTF8, 0, text.data(), size, nullptr, 0);
    if (length <= 0) {
        return false;
    }
    std::wstring buffer(static_cast<std::size_t>(length), L'\0');
    MultiByteToWideChar(CP_UTF8, 0, text.data(), size, buffer.data(), length);

    std::wstring normalized;
    normalized.reserve(buffer.size() + buffer.size() / 8 + 1);
    for (const wchar_t ch : buffer) {
        if (ch == L'\n' && !last_cr) {
            normalized.push_back(L'\r');
        }
        normalized.push_back(ch);
        last_cr = ch == L'\r';
    }
    write_console(handle, normalized);
    return true;
}

/// 末尾若是一个没写完的 UTF-8 字符，返回它的起始位置，先攒着不写，
/// 等下一个数据块补齐再转换——否则一个汉字会被拆成两次转换，变成乱码。
std::size_t complete_length(const std::string& text)
{
    std::size_t index = text.size();
    std::size_t continuation = 0;
    while (index > 0 && continuation < 3 && (static_cast<unsigned char>(text[index - 1]) & 0xC0) == 0x80) {
        --index;
        ++continuation;
    }
    if (index == 0) {
        return text.size();   // 整段都是续字节，没法判断，原样交给转换
    }
    const unsigned char lead = static_cast<unsigned char>(text[index - 1]);
    std::size_t needed = 1;
    if ((lead & 0xE0) == 0xC0) {
        needed = 2;
    } else if ((lead & 0xF0) == 0xE0) {
        needed = 3;
    } else if ((lead & 0xF8) == 0xF0) {
        needed = 4;
    }
    const std::size_t available = text.size() - (index - 1);
    return available < needed ? index - 1 : text.size();
}

/// std::cout / std::cerr / std::clog 的 UTF-8 适配层。
///
/// - 连着控制台：按行/块把 UTF-8 转成宽字符写出去（Windows 控制台不认 UTF-8 字节）；
/// - 被重定向到文件或管道：**原样**转发给原来的 rdbuf，一个字节都不改，
///   也就不会出现旧版 `--help > help.txt` 得到空文件的问题。
class U8ConsoleBuf : public std::streambuf
{
public:
    U8ConsoleBuf(std::streambuf* fallback, HANDLE handle) : fallback_(fallback), handle_(handle)
    {
    }

protected:
    int_type overflow(int_type ch) override
    {
        if (traits_type::eq_int_type(ch, traits_type::eof())) {
            return sync() == 0 ? traits_type::not_eof(ch) : traits_type::eof();
        }
        const char byte = traits_type::to_char_type(ch);
        return xsputn(&byte, 1) == 1 ? ch : traits_type::eof();
    }

    std::streamsize xsputn(const char* text, std::streamsize size) override
    {
        if (size <= 0) {
            return 0;
        }
        if (!is_console(handle_)) {
            return fallback_->sputn(text, size);
        }
        pending_.append(text, static_cast<std::size_t>(size));
        const std::size_t length = complete_length(pending_);
        if (length > 0) {
            write_utf8(handle_, std::string_view(pending_).substr(0, length), last_cr_);
            pending_.erase(0, length);
        }
        return size;
    }

    int sync() override
    {
        if (!is_console(handle_)) {
            return fallback_->pubsync();
        }
        if (!pending_.empty()) {
            write_utf8(handle_, pending_, last_cr_);
            pending_.clear();
        }
        return 0;
    }

private:
    std::streambuf* fallback_;
    HANDLE handle_;
    std::string pending_;
    bool last_cr_{false};
};

#endif   // _WIN32

}   // namespace

void init()
{
#ifdef _WIN32
    // 故意不释放：标准流可能在静态析构阶段（甚至更晚）才最后一次刷新，
    // 用堆上的对象可以保证那时 buf 仍然有效。函数内静态量保证只初始化一次。
    static U8ConsoleBuf* const console_buf = new U8ConsoleBuf(std::cout.rdbuf(), GetStdHandle(STD_OUTPUT_HANDLE));
    static U8ConsoleBuf* const error_buf = new U8ConsoleBuf(std::cerr.rdbuf(), GetStdHandle(STD_ERROR_HANDLE));

    std::cout.rdbuf(console_buf);
    std::cerr.rdbuf(error_buf);
    std::clog.rdbuf(error_buf);
#endif
    enable_ansi();
}

void enable_ansi()
{
#ifdef _WIN32
    DWORD mode = 0;
    const HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (is_console(handle) && GetConsoleMode(handle, &mode) != 0) {
        SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
#endif
}

bool stdout_is_console()
{
#ifdef _WIN32
    return is_console(GetStdHandle(STD_OUTPUT_HANDLE));
#else
    return ::isatty(STDOUT_FILENO) != 0;
#endif
}

void write(std::string_view text)
{
    std::cout.write(text.data(), static_cast<std::streamsize>(text.size()));
    std::cout.flush();
}

void write_error(std::string_view text)
{
    std::cerr.write(text.data(), static_cast<std::streamsize>(text.size()));
    std::cerr.flush();
}

}   // namespace csp::console
