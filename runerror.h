#ifndef RUNERROR_H
#define RUNERROR_H

#include <iostream>

class RunError
{
private:
    int code;
    std::string message;

public:
    RunError();
    RunError(int code, const std::string &message);
    std::string ToString() const;
    std::string Error() const;
    int Code() const;
};

#endif // RUNERROR_H
