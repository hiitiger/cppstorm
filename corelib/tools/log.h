#pragma once

namespace Storm {


class LogStream
{
public:
    LogStream(const char* mod, const char* file, const char* function, unsigned int line)
    : mod_(mod)
    , file_(file)
    , function_(function)
    , line_(line)
    {

    }
    ~LogStream();

    LogStream& operator<<(bool d) { textstream_ << d; return *this; }
    LogStream& operator<<(char d) { textstream_ << d; return *this; }
    LogStream& operator<<(short d) { textstream_ << d; return *this; }
    LogStream& operator<<(unsigned short d) { textstream_ << d; return *this; }
    LogStream& operator<<(int d) { textstream_ << d; return *this; }
    LogStream& operator<<(unsigned int d) { textstream_ << d; return *this; }
    LogStream& operator<<(long d) { textstream_ << d; return *this; }
    LogStream& operator<<(unsigned long d) { textstream_ << d; return *this; }
    LogStream& operator<<(int64_t d) { textstream_ << d; return *this; }
    LogStream& operator<<(uint64_t d) { textstream_ << d; return *this; }
    LogStream& operator<<(float d) { textstream_ << d; return *this; }
    LogStream& operator<<(double d) { textstream_ << d; return *this; }
    LogStream& operator<<(const char* d) { textstream_ << d; return *this; }
    LogStream& operator<<(const std::string& d) { textstream_ << d; return *this; }
    LogStream& operator<<(const wchar_t* d) { textstream_ << d; return *this; }
    LogStream& operator<<(const std::wstring& d) { textstream_ << d; return *this; }
    LogStream& operator<<(const void* d) { textstream_ << d; return *this; }

    template<class T, typename = std::enable_if_t<!std::is_pointer<T>::value, T&>>
    LogStream& operator << (const T& t) {
        std::stringstream ss;
        ss << t;
        return (*this) << ss.str();
    }
private:
    TextStream textstream_;
    const char* mod_;
    const char* file_;
    const char* function_;
    unsigned int line_;
};

}

template <typename T, size_t S>
inline constexpr size_t get_file_name_offset(const T(&str)[S], size_t i = S - 1)
{
    return (str[i] == '/' || str[i] == '\\') ? i + 1 : (i > 0 ? get_file_name_offset(str, i - 1) : 0);
}

template <typename T>
inline constexpr size_t get_file_name_offset(T(&str)[1])
{
    return 0;
}


#define __FILENAME__ &__FILE__[get_file_name_offset(__FILE__)]

#define LOGGER(mod)  Storm::LogStream(mod, __FILENAME__, __FUNCTION__, __LINE__)
