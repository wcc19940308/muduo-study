#pragma once
#include <string>
class noncopyable
{
protected:
    noncopyable(){}
private:
    noncopyable(const noncopyable&) = delete;
    void operator=(const noncopyable&) = delete;
};

// 利用默认构造函数、拷贝构造和拷贝赋值的类
struct copyable
{};

class StringArg : copyable
{
public:
    StringArg(const char* str)
    : str_(str)
    {}
    StringArg(const std::string& str)
    : str_(str.c_str())
    {}

    const char* c_str() const {return str_;}

private:
    const char* str_;
};

// 隐式类型转换
template <typename To, typename From>
inline To implicit_cast(const From& f)
{
    return f;
}

