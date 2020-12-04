#include <gbase/fs/file.hpp>
#include <iostream>

int main()
{
    gb::output_file file("/dev/stdout");

    file << "fuckyou from file module" << std::endl;

    file.flush();

    file.close();

    std::cout << file.readable() << ' ' << file.writable() << ' ' << file.is_open() << std::endl;
    return 0;
}
