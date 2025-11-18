#include "BitArray.h"
#include <iostream>
#include <string>

int main(){
    BitArray bits(15);
    bits.set(0, true);
    bits.set(5, true);
    bits.set(7, true);
    BitArray bits1(15);
    bits1.set(5, true);
    bits1.set(6, true);
    bits &= bits1;
    std::string str = bits.to_string();
    std::cout << str <<  std::endl;
    std::string str1 = bits1.to_string();
    std::cout << str1;
}