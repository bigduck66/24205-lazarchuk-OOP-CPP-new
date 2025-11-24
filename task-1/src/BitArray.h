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

public:
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

    // Returns value of bit at index i
    bool operator[](int i) const;
    
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