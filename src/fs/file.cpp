#include <gbase/fs/file.hpp>

template <>
bool gb::input_file::writable() const
{
    return false;
}
template <>
bool gb::output_file::readable() const
{
    return false;
}
