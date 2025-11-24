#include "BitArray.h"
#include <iostream>
#include <string>

void printBitArray(BitArray ba) {  // Неявно вызывается конструктор копирования
    std::cout << ba.to_string() << std::endl;
    // При выходе из функции вызывается деструктор для копии
}

void example1() {
    BitArray original(8, 0b10101010);  // original: 10101010
    printBitArray(original);           // Создается копия для функции
    // original остается неизменным
}

int main(){
    BitArray original(15);
    BitArray copy1 = original;//неявное копирование
    example1();
}