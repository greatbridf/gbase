#include <gbase/gbase.hpp>

#include <iostream>

class data : public gb::serializable {
public:
    data(int a, const char* b)
        : _a(a)
        , _b(b) {};

    std::string toString() const override
    {
        return "the data consists of " + std::to_string(_a) + " and \"" + _b + '\"';
    }

private:
    int _a;
    const char* _b;
};

int main()
{
    std::cout << data(1, "fuckyou") << std::endl;
    return 0;
}
