#include <gbase/gbase.hpp>

#include <iostream>

class A : public interface {
 public:
    void fuck();
};

void A::fuck() {
    std::cout << "fuckyou" << std::endl;
}

int main() {
    gb::greetings();

    A fuck;

    fuck.fuck();

    return 0;
}
