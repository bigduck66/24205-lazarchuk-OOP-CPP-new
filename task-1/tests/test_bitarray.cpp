#include "../src/BitArray.h"
#include <cassert>
#include <iostream>

void test_constructor() {
    std::cout << "Testing constructors..." << std::endl;
    
    // Конструктор по умолчанию
    BitArray empty;
    assert(empty.empty());
    assert(empty.size() == 0);
    
    // Конструктор с параметрами
    BitArray arr1(8, 0b10101010);
    assert(arr1.size() == 8);
    assert(arr1.count() == 4); // 4 единицы
    
    // Конструктор копирования
    BitArray arr2 = arr1;
    assert(arr2.size() == 8);
    assert(arr2.count() == 4);
    
    // Конструктор с нулевым размером
    BitArray zeroSize(0);
    assert(zeroSize.empty());
    assert(zeroSize.size() == 0);
    
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
    
    // Test AND
    BitArray c = a & b;
    assert(c[0] == false);
    assert(c[1] == false);
    assert(c[2] == false);
    assert(c[3] == true);
    
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
    
    // Test compound assignment operators
    BitArray g = a;
    g &= b;
    assert(g[3] == true);
    
    BitArray h = a;
    h |= b;
    assert(h[1] == true);
    assert(h[2] == true);
    assert(h[3] == true);
    
    BitArray i = a;
    i ^= b;
    assert(i[1] == true);
    assert(i[2] == true);
    assert(i[3] == false);
    
    std::cout << "✓ Logical operations test passed" << std::endl;
}

void test_shift_operations() {
    std::cout << "Testing shift operations..." << std::endl;
    
    BitArray arr(8, 0b10011001);
    
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
    
    // Shift by 0
    BitArray zero_shift = arr << 0;
    assert(zero_shift.to_string() == arr.to_string());
    
    // Shift by size or more
    BitArray big_shift = arr << 10;
    for (int i = 0; i < big_shift.size(); ++i) {
        assert(big_shift[i] == false);
    }
    
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
    
    BitArray original(6);
    original.set(0, true);   // Бит 0 = 1
    original.set(1, false);  // Бит 1 = 0  
    original.set(2, true);   // Бит 2 = 1
    original.set(3, false);  // Бит 3 = 0
    original.set(4, true);   // Бит 4 = 1
    original.set(5, false);  // Бит 5 = 0
    
    // Теперь должно быть: бит5(0) бит4(1) бит3(0) бит2(1) бит1(0) бит0(1) = "010101"
    std::cout << "Original after manual setup: " << original.to_string() 
              << " (bits: 5(0) 4(1) 3(0) 2(1) 1(0) 0(1))" << std::endl;
    
    BitArray copy1 = original;
    std::cout << "Copy: " << copy1.to_string() << std::endl;
    
    // Проверяем конкретные биты
    std::cout << "Verifying bits:" << std::endl;
    std::cout << "  copy1[0] = " << copy1[0] << " (expected: 1)" << std::endl;
    std::cout << "  copy1[1] = " << copy1[1] << " (expected: 0)" << std::endl;
    std::cout << "  copy1[2] = " << copy1[2] << " (expected: 1)" << std::endl;
    std::cout << "  copy1[3] = " << copy1[3] << " (expected: 0)" << std::endl;
    std::cout << "  copy1[4] = " << copy1[4] << " (expected: 1)" << std::endl;
    std::cout << "  copy1[5] = " << copy1[5] << " (expected: 0)" << std::endl;
    
    // Меняем оригинал
    original.set(0, false);
    std::cout << "\nAfter changing original[0] to false:" << std::endl;
    std::cout << "Original: " << original.to_string() << std::endl;
    std::cout << "Copy: " << copy1.to_string() << " (should not change)" << std::endl;
    
    // Проверяем, что копия не изменилась
    assert(copy1[0] == true);   // Должно остаться true
    assert(copy1[1] == false);
    assert(copy1[2] == true);
    assert(copy1[3] == false);
    assert(copy1[4] == true);
    assert(copy1[5] == false);
    
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
    
    // Multiple push_backs
    for (int i = 0; i < 10; ++i) {
        arr.push_back(i % 2 == 0);
    }
    assert(arr.size() == 13);
    
    std::cout << "✓ Push_back test passed" << std::endl;
}


void test_index_assignment() {
    std::cout << "Testing index assignment..." << std::endl;
    
    BitArray ba(8, 0);
    
    // Простое присваивание
    ba[0] = true;
    ba[2] = true;
    assert(ba.to_string() == "10100000");
    
    // Копирование между битами
    ba[4] = ba[0]; // ba[0] = true
    assert(ba[4] == true);
    
    // Изменение через индекс
    ba[0] = false;
    assert(ba[0] == false);
    
    // Каскадное присваивание
    bool value = true;
    ba[1] = value;
    ba[5] = ba[1];
    assert(ba[5] == true);
    
    // Чтение и запись в цикле
    for (int i = 0; i < ba.size(); ++i) {
        ba[i] = (i % 3 == 0);
    }
    
    std::cout << "✓ Index assignment test passed" << std::endl;
}


void test_comparison_operators() {
    std::cout << "Testing comparison operators..." << std::endl;
    
    BitArray a(4, 0b1010);
    BitArray b(4, 0b1010);
    BitArray c(4, 0b0101);
    BitArray d(5, 0b1010);
    
    assert(a == b);
    assert(a != c);
    assert(a != d); // Разный размер
    
    BitArray empty1, empty2;
    assert(empty1 == empty2);
    
    std::cout << "✓ Comparison operators test passed" << std::endl;
}

void test_swap() {
    std::cout << "Testing swap..." << std::endl;
    
    BitArray a(4, 0b1010);
    BitArray b(4, 0b0101);
    
    std::string a_str = a.to_string();
    std::string b_str = b.to_string();
    
    a.swap(b);
    
    assert(a.to_string() == b_str);
    assert(b.to_string() == a_str);
    
    std::cout << "✓ Swap test passed" << std::endl;
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
    test_index_assignment();
    test_comparison_operators();
    test_swap();
    
    std::cout << "==========================" << std::endl;
    std::cout << "All tests passed! ✓" << std::endl;
    
    return 0;
}