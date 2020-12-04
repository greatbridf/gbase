#pragma once

typedef unsigned char byte;

class non_copyable {
 public:
    non_copyable() = default;
    non_copyable(const non_copyable&) = delete;
    non_copyable& operator=(const non_copyable&) = delete;
};

class interface : private non_copyable {
 public:
    virtual ~interface() = default;
};
