#include "BitArray.h"
#include <iostream>
#include <string>

BitArray createBitArray() {
    BitArray local(4, 0b1010);  // Локальный объект
    return local;  // ← Может быть неявное копирование
}

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
    BitArray result = createBitArray();  // ← Неявное копирование
}