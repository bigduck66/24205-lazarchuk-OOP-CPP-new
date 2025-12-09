#include "BitArray.h"
#include <iostream>

int main() {
    //Создание и базовые операции
    BitArray ba1(8, 0);
    ba1[0] = true;
    ba1[2] = true;
    ba1[4] = true;
    ba1[6] = true;
    
    std::cout << "BitArray 1: " << ba1.to_string() << std::endl;
    
    BitArray ba2 = ba1;
    ba2[1] = true;
    ba2[3] = ba2[0];
    
    std::cout << "BitArray 2: " << ba2.to_string() << std::endl;
    
    return 0;
}