#include "runerror.h"
#include <string>

RunError::RunError()
    : code(0), message("error") // 使用成员初始化列表
{
}

RunError::RunError(int code, const std::string &message)
    : code(code), message(message) // 使用成员初始化列表
{
}

std::string RunError::ToString() const
{
    // 使用字符串拼接代替 stringstream
    return "RunError. Code:" + std::to_string(code) + ", Message:" + message + "\n";
}

std::string RunError::Error() const
{
    return message;
}

int RunError::Code() const
{
    return code;
}
