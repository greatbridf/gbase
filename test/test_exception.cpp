#include <gbase/error.hpp>

#include <iostream>

void func_throw()
{
    throw gb::exception("oh wow");
}

int main()
{
    try {
        func_throw();
    } catch (const std::exception& e) {
        std::cerr << "exception caught: " << e.what() << '\n';
    }

    return 0;
}
