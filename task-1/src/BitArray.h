#ifndef BITARRAY_H
#define BITARRAY_H

#include <cstddef>
#include <stdexcept>
#include <algorithm>
#include <string>

class BitArray
{
private:
    unsigned long* m_data;
    size_t m_bit_count; // Number of bits stored
    size_t m_array_size; // Size of m_data array in elements
    
    static const size_t BITS_PER_BLOCK = sizeof(unsigned long) * 8;

    // Helper methods for bit manipulation
    size_t block_index(size_t bit_pos) const { return bit_pos / BITS_PER_BLOCK; }
    size_t bit_offset(size_t bit_pos) const { return bit_pos % BITS_PER_BLOCK; }
    unsigned long bit_mask(size_t bit_pos) const { return 1UL << bit_offset(bit_pos); }

    // Proxy class for operator[] assignment
    class BitProxy {
    private:
        BitArray& m_array;
        size_t m_index;
        
    public:
        BitProxy(BitArray& array, size_t index)
            : m_array(array), m_index(index) {}
        
        // Conversion to bool (for reading) - НЕ ИСПОЛЬЗУЕМ operator[]
        operator bool() const {
            // Прямой доступ к данным вместо вызова operator[]
            if (m_array.m_data == nullptr) {
                return false;
            }
            size_t block_idx = m_array.block_index(m_index);
            size_t bit_off = m_array.bit_offset(m_index);
            return (m_array.m_data[block_idx] >> bit_off) & 1;
        }
        
        // Assignment operator (for writing from bool)
        BitProxy& operator=(bool value) {
            // Используем set() вместо прямого доступа
            m_array.set(static_cast<int>(m_index), value);
            return *this;
        }
        
        // Assignment operator from another BitProxy
        BitProxy& operator=(const BitProxy& other) {
            // Получаем значение напрямую
            bool value = static_cast<bool>(other);
            m_array.set(static_cast<int>(m_index), value);
            return *this;
        }
        
        // Prevent taking address
        BitProxy* operator&() = delete;
    };
    
public:
    // Nested classes need to be friend to access private members
    friend class BitProxy;

    // Constructs an empty bit array
    BitArray();
    
    // Destructor
    ~BitArray();
    
    // Constructs a bit array storing specified number of bits.
    // First sizeof(long) bits can be initialized with value parameter.
    explicit BitArray(int num_bits, unsigned long value = 0);
    
    // Copy constructor
    BitArray(const BitArray& other);
    
    // Swaps values of two bit arrays
    void swap(BitArray& other);
    
    // Assignment operator
    BitArray& operator=(const BitArray& other);

    // Changes array size. When expanding, new elements are initialized with value.
    void resize(int num_bits, bool value = false);
    
    // Clears the array
    void clear();
    
    // Adds a new bit to the end of array. Reallocates memory if necessary.
    void push_back(bool bit);

    // Bitwise operations on arrays. Work only on arrays of same size.
    BitArray& operator&=(const BitArray& other);
    BitArray& operator|=(const BitArray& other);
    BitArray& operator^=(const BitArray& other);
    
    // Bitwise shift with zero fill
    BitArray& operator<<=(int n);
    BitArray& operator>>=(int n);
    BitArray operator<<(int n) const;
    BitArray operator>>(int n) const;

    // Sets bit at index n to value val
    BitArray& set(int n, bool val = true);
    
    // Sets all bits to true
    BitArray& set();
    
    // Sets bit at index n to false
    BitArray& reset(int n);
    
    // Sets all bits to false
    BitArray& reset();

    // Returns true if array contains at least one true bit
    bool any() const;
    
    // Returns true if all bits are false
    bool none() const;
    
    // Bitwise inversion
    BitArray operator~() const;
    
    // Counts number of true bits
    int count() const;

    // Returns value of bit at index i (const version)
    bool operator[](int i) const;
    
    // Returns reference proxy for bit at index i (non-const version)
    BitProxy operator[](int i);
    
    // Returns size of array in bits
    int size() const;
    
    // Returns true if array is empty
    bool empty() const;
    
    // Returns string representation of array
    std::string to_string() const;
};

// Comparison operators
bool operator==(const BitArray& a, const BitArray& b);
bool operator!=(const BitArray& a, const BitArray& b);

// Bitwise operators
BitArray operator&(const BitArray& b1, const BitArray& b2);
BitArray operator|(const BitArray& b1, const BitArray& b2);
BitArray operator^(const BitArray& b1, const BitArray& b2);

#endif // BITARRAY_H