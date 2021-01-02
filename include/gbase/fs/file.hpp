#pragma once

#include <filesystem>
#include <fstream>
#include <string>
#include <unistd.h>

namespace gb {
// definitions
template <typename T>
class basic_file : public T {
public:
    explicit basic_file(std::string name);
    size_t size();

    // std::string getMimeType() const;
    bool readable() const;
    bool writable() const;
    bool exist() const;

    bool remove();

private:
    std::string _filename;
    size_t _size;
};
typedef basic_file<std::fstream> file;
typedef basic_file<std::ifstream> input_file;
typedef basic_file<std::ofstream> output_file;

template <>
bool input_file::writable() const;
template <>
bool output_file::readable() const;

// realizations
template <typename T>
basic_file<T>::basic_file(std::string _name)
    : _filename(std::move(_name))
    , T(_name)
    , _size(-1)
{
}
template <typename T>
size_t basic_file<T>::size()
{
    if (_size == -1) {
        this->seekg(0, std::ios::end);
        _size = this->tellg();
        this->seekg(0, std::ios::beg);
    }
    return _size;
}
/*
template<typename T>
std::string basic_file<T>::getMimeType() const
{
    auto const& mimeTypes = Static::mimeTypes();
    auto pos = _filename.rfind('.');
    if (pos == std::string::npos)
    {
        return mimeTypes.at("");
    }
    std::string ext = _filename.substr(pos + 1);
    if (mimeTypes.find(ext) == mimeTypes.end())
    {
        return mimeTypes.at("");
    }
    else
    {
        return mimeTypes.at(ext);
    }
}
*/
template <typename T>
bool basic_file<T>::readable() const
{
    return access(_filename.c_str(), 4) == 0;
}
template <typename T>
bool basic_file<T>::writable() const
{
    return access(_filename.c_str(), 2) == 0;
}
template <typename T>
bool basic_file<T>::exist() const
{
    return access(_filename.c_str(), 0) == 0;
}
template <typename T>
bool basic_file<T>::remove()
{
    if (!this->exist())
        return true;
    if (this->is_open())
        this->close();
    return std::filesystem::remove(this->_filename);
}
}
