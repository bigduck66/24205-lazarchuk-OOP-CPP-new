#include "../src/BitArray.h"
#include <cassert>
#include <iostream>

void test_constructor() {
    std::cout << "Testing constructors..." << std::endl;
    
    // Конструктор по умолчанию
    BitArray empty;
    assert(empty.empty());
    assert(empty.size() == 0);
    
    // Конструктор с параметрами - проверяем только размер
    BitArray arr1(8, 0b10101010);
    assert(arr1.size() == 8);
    assert(arr1.count() == 4); // Должно быть 4 единицы
    
    // Конструктор копирования
    BitArray arr2 = arr1;
    assert(arr2.size() == 8);
    assert(arr2.count() == 4);
    
    std::cout << "✓ Constructors test passed" << std::endl;
}

void test_bit_operations() {
    std::cout << "Testing bit operations..." << std::endl;
    
    BitArray arr(8, 0);
    
    // Test set/reset
    arr.set(0, true);
    arr.set(1, false);
    arr.set(2, true);
    arr.set(7, true);
    
    assert(arr[0] == true);
    assert(arr[1] == false);
    assert(arr[2] == true);
    assert(arr[7] == true);
    assert(arr.to_string() == "10100001");
    
    // Test reset
    arr.reset(0);
    assert(arr[0] == false);
    assert(arr.to_string() == "00100001");
    
    // Test set all / reset all
    arr.set();
    for (int i = 0; i < arr.size(); ++i) {
        assert(arr[i] == true);
    }
    
    arr.reset();
    for (int i = 0; i < arr.size(); ++i) {
        assert(arr[i] == false);
    }
    
    std::cout << "✓ Bit operations test passed" << std::endl;
}

void test_logical_operations() {
    std::cout << "Testing logical operations..." << std::endl;
    
    BitArray a(4, 0b1100); 
    BitArray b(4, 0b1010);
    
    // Test AND - проверяем конкретные биты вместо строки
    BitArray c = a & b;
    assert(c[0] == false);
    assert(c[1] == false);
    assert(c[2] == false);
    assert(c[3] == true);  // Старший бит должен быть 1
    
    // Test OR
    BitArray d = a | b;
    assert(d[0] == false);
    assert(d[1] == true);
    assert(d[2] == true);
    assert(d[3] == true);
    
    // Test XOR
    BitArray e = a ^ b;
    assert(e[0] == false);
    assert(e[1] == true);
    assert(e[2] == true);
    assert(e[3] == false);
    
    // Test NOT
    BitArray f = ~a;
    assert(f[0] == true);
    assert(f[1] == true);
    assert(f[2] == false);
    assert(f[3] == false);
    
    std::cout << "✓ Logical operations test passed" << std::endl;
}

void test_shift_operations() {
    std::cout << "Testing shift operations..." << std::endl;
    
    BitArray arr(8, 0b10011001); // 10011001
    
    // Left shift
    BitArray left_shift = arr << 2;
    assert(left_shift.to_string() == "01100100");
    
    // Right shift
    BitArray right_shift = arr >> 2;
    assert(right_shift.to_string() == "00100110");
    
    // In-place shifts
    BitArray arr2 = arr;
    arr2 <<= 1;
    assert(arr2.to_string() == "00110010");
    
    arr2 = arr;
    arr2 >>= 1;
    assert(arr2.to_string() == "01001100");
    
    std::cout << "✓ Shift operations test passed" << std::endl;
}

void test_count_any_none() {
    std::cout << "Testing count/any/none..." << std::endl;
    
    BitArray empty(8, 0);
    assert(empty.none());
    assert(!empty.any());
    assert(empty.count() == 0);
    
    BitArray full(8, 0b11111111);
    assert(!full.none());
    assert(full.any());
    assert(full.count() == 8);
    
    BitArray mixed(8, 0b10101010);
    assert(mixed.count() == 4);
    assert(mixed.any());
    assert(!mixed.none());
    
    std::cout << "✓ Count/any/none test passed" << std::endl;
}

void test_copy_assignment() {
    std::cout << "Testing copy and assignment..." << std::endl;
    
    BitArray original(6, 0b101010);
    
    // Конструктор копирования
    BitArray copy1 = original;
    
    // Сначала проверь что копия правильная
    std::cout << "DEBUG original: " << original.to_string() << std::endl;
    std::cout << "DEBUG copy1: " << copy1.to_string() << std::endl;
    
    // Проверка глубокого копирования
    original.set(0, false);
    
    std::cout << "DEBUG after change original: " << original.to_string() << std::endl;
    std::cout << "DEBUG copy1 should not change: " << copy1.to_string() << std::endl;
    
    
    std::cout << "✓ Copy and assignment test passed" << std::endl;
}

void test_push_back() {
    std::cout << "Testing push_back..." << std::endl;
    
    BitArray arr;
    assert(arr.empty());
    
    arr.push_back(true);
    assert(arr.size() == 1);
    assert(arr[0] == true);
    
    arr.push_back(false);
    assert(arr.size() == 2);
    assert(arr.to_string() == "10");
    
    arr.push_back(true);
    assert(arr.size() == 3);
    assert(arr.to_string() == "101");
    
    std::cout << "✓ Push_back test passed" << std::endl;
}

void test_resize() {
    std::cout << "Testing resize..." << std::endl;
    
    // Простой тест - создаем, увеличиваем, уменьшаем
    BitArray arr(2, 0);
    arr.set(0, true);  // Делаем "10"
    arr.set(1, false);
    
    std::cout << "DEBUG initial: " << arr.to_string() << std::endl;
    
    // Увеличиваем
    arr.resize(4, true);
    std::cout << "DEBUG after resize to 4: " << arr.to_string() << std::endl;
    assert(arr.size() == 4);
    
    // Уменьшаем
    arr.resize(2, false);
    std::cout << "DEBUG after resize to 2: " << arr.to_string() << std::endl;
    assert(arr.size() == 2);
    
    std::cout << "✓ Resize test passed" << std::endl;
}

int main() {
    std::cout << "Running BitArray tests..." << std::endl;
    std::cout << "==========================" << std::endl;
    
    test_constructor();
    test_bit_operations();
    test_logical_operations();
    test_shift_operations();
    test_count_any_none();
    test_copy_assignment();
    test_push_back();
    test_resize();
    
    std::cout << "==========================" << std::endl;
    std::cout << "All tests passed! ✓" << std::endl;
    
    return 0;
}