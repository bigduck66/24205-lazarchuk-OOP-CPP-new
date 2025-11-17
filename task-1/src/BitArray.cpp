#include "BitArray.h"
#include <algorithm>
#include <stdexcept>
#include <string>

// Constructs an empty bit array
BitArray::BitArray() : m_data(nullptr), m_bit_count(0), m_array_size(0) {}

// Destructor
BitArray::~BitArray() {
    delete[] m_data;
}

// Constructs a bit array storing specified number of bits
BitArray::BitArray(int num_bits, unsigned long value) {
    if (num_bits < 0) {
        throw std::invalid_argument("BitArray size cannot be negative");
    }
    m_bit_count = static_cast<size_t>(num_bits);
    m_array_size = (m_bit_count + BITS_PER_BLOCK - 1) / BITS_PER_BLOCK;

    if (m_array_size > 0) {
        m_data = new unsigned long[m_array_size]();
        m_data[0] = value; // Initialize only first block as per specification
    } else {
        m_data = nullptr;
    }
}

// Copy constructor
BitArray::BitArray(const BitArray& other) 
    : m_bit_count(other.m_bit_count), m_array_size(other.m_array_size) {
    if (m_array_size > 0) {
        m_data = new unsigned long[m_array_size];
        std::copy(other.m_data, other.m_data + m_array_size, m_data);
    } else {
        m_data = nullptr;
    }
}

// Swaps values of two bit arrays
void BitArray::swap(BitArray& other) {
    std::swap(m_data, other.m_data);
    std::swap(m_bit_count, other.m_bit_count);
    std::swap(m_array_size, other.m_array_size);
}

// Assignment operator
BitArray& BitArray::operator=(const BitArray& other) {
    if (this != &other) {
        delete[] m_data;
        m_bit_count = other.m_bit_count;
        m_array_size = other.m_array_size;

        if (m_array_size > 0) {
            m_data = new unsigned long[m_array_size];
            std::copy(other.m_data, other.m_data + m_array_size, m_data);
        } else {
            m_data = nullptr;
        }
    }
    return *this;
}

// Changes array size. When expanding, new elements are initialized with value.
void BitArray::resize(int num_bits, bool value) {
    if (num_bits < 0) {
        throw std::invalid_argument("Number of bits cannot be negative");
    }
    size_t new_bit_count = static_cast<size_t>(num_bits);

    if (new_bit_count == m_bit_count) {
        return;
    }

    size_t new_array_size = (new_bit_count + BITS_PER_BLOCK - 1) / BITS_PER_BLOCK;
    unsigned long* new_data = new unsigned long[new_array_size];
    unsigned long fill_value = value ? ~0UL : 0UL;
    
    // Initialize new array with fill value
    for (size_t i = 0; i < new_array_size; ++i) {
        new_data[i] = fill_value;
    }
    
    // Copy existing data
    size_t min_array_size = std::min(m_array_size, new_array_size);
    if (min_array_size > 0) {
        std::copy(m_data, m_data + min_array_size, new_data);
    }

    // Trim excess bits if shrinking
    if (new_bit_count < m_bit_count && new_array_size > 0) {
        size_t bits_in_last_block = new_bit_count % BITS_PER_BLOCK;
        if (bits_in_last_block > 0) {
            unsigned long mask = (1UL << bits_in_last_block) - 1;
            new_data[new_array_size - 1] &= mask;
        }
    }
    
    delete[] m_data;
    m_data = new_data;
    m_bit_count = new_bit_count;
    m_array_size = new_array_size;
}

// Clears the array
void BitArray::clear() {
    delete[] m_data;
    m_data = nullptr;
    m_bit_count = 0;
    m_array_size = 0;
}

// Adds a new bit to the end of array
void BitArray::push_back(bool bit) {
    // Check if reallocation is needed
    if (m_bit_count >= m_array_size * BITS_PER_BLOCK) {
        size_t new_array_size = (m_array_size == 0) ? 1 : m_array_size * 2;
        unsigned long* new_data = new unsigned long[new_array_size]();
        
        if (m_data != nullptr) {
            std::copy(m_data, m_data + m_array_size, new_data);
            delete[] m_data;
        }
        
        m_data = new_data;
        m_array_size = new_array_size;
    }
    
    // Set the bit if needed
    if (bit) {
        size_t block_idx = block_index(m_bit_count);
        size_t bit_off = bit_offset(m_bit_count);
        m_data[block_idx] |= (1UL << bit_off);
    }
    
    m_bit_count++;
}

// Bitwise AND operation
BitArray& BitArray::operator&=(const BitArray& other) {
    if (m_bit_count != other.m_bit_count) {
        throw std::invalid_argument("Bit arrays must be of same size for AND operation");
    }
    
    for (size_t i = 0; i < m_array_size; ++i) {
        m_data[i] &= other.m_data[i];
    }
    
    return *this;
}

// Bitwise OR operation
BitArray& BitArray::operator|=(const BitArray& other) {
    if (m_bit_count != other.m_bit_count) {
        throw std::invalid_argument("Bit arrays must be of same size for OR operation");
    }
    
    for (size_t i = 0; i < m_array_size; ++i) {
        m_data[i] |= other.m_data[i];
    }
    
    return *this;
}

// Bitwise XOR operation
BitArray& BitArray::operator^=(const BitArray& other) {
    if (m_bit_count != other.m_bit_count) {
        throw std::invalid_argument("Bit arrays must be of same size for XOR operation");
    }
    
    for (size_t i = 0; i < m_array_size; ++i) {
        m_data[i] ^= other.m_data[i];
    }
    
    return *this;
}

// Left shift assignment
BitArray& BitArray::operator<<=(int n) {
    if (n < 0) {
        throw std::invalid_argument("Shift amount cannot be negative");
    }
    
    if (n == 0 || m_bit_count == 0) {
        return *this;
    }
    
    size_t shift_bits = static_cast<size_t>(n);
    
    if (shift_bits >= m_bit_count) {
        // Shift more than size - clear all bits
        reset();
        return *this;
    }
    
    // Perform shift
    for (size_t i = 0; i < m_bit_count - shift_bits; ++i) {
        set(i, (*this)[i + shift_bits]);
    }
    
    // Fill remaining bits with zeros
    for (size_t i = m_bit_count - shift_bits; i < m_bit_count; ++i) {
        reset(i);
    }
    
    return *this;
}

// Right shift assignment
BitArray& BitArray::operator>>=(int n) {
    if (n < 0) {
        throw std::invalid_argument("Shift amount cannot be negative");
    }
    
    if (n == 0 || m_bit_count == 0) {
        return *this;
    }
    
    size_t shift_bits = static_cast<size_t>(n);
    
    if (shift_bits >= m_bit_count) {
        // Shift more than size - clear all bits
        reset();
        return *this;
    }
    
    // Perform shift from the end to avoid overwriting
    for (int i = static_cast<int>(m_bit_count) - 1; i >= static_cast<int>(shift_bits); --i) {
        set(i, (*this)[i - shift_bits]);
    }
    
    // Fill beginning bits with zeros
    for (size_t i = 0; i < shift_bits; ++i) {
        reset(i);
    }
    
    return *this;
}

// Left shift
BitArray BitArray::operator<<(int n) const {
    BitArray result(*this);
    result <<= n;
    return result;
}

// Right shift
BitArray BitArray::operator>>(int n) const {
    BitArray result(*this);
    result >>= n;
    return result;
}

// Sets bit at index n to value val
BitArray& BitArray::set(int n, bool val) {
    if (n < 0 || static_cast<size_t>(n) >= m_bit_count) {
        throw std::out_of_range("Bit index out of range");
    }
    
    size_t block_idx = block_index(n);
    size_t bit_off = bit_offset(n);
    
    if (val) {
        m_data[block_idx] |= (1UL << bit_off);
    } else {
        m_data[block_idx] &= ~(1UL << bit_off);
    }
    
    return *this;
}

// Sets all bits to true
BitArray& BitArray::set() {
    for (size_t i = 0; i < m_array_size; ++i) {
        m_data[i] = ~0UL;
    }
    
    // Clear excess bits in last block
    if (m_array_size > 0) {
        size_t bits_in_last_block = m_bit_count % BITS_PER_BLOCK;
        if (bits_in_last_block > 0) {
            unsigned long mask = (1UL << bits_in_last_block) - 1;
            m_data[m_array_size - 1] &= mask;
        }
    }
    
    return *this;
}

// Sets bit at index n to false
BitArray& BitArray::reset(int n) {
    return set(n, false);
}

// Sets all bits to false
BitArray& BitArray::reset() {
    for (size_t i = 0; i < m_array_size; ++i) {
        m_data[i] = 0UL;
    }
    return *this;
}

// Returns true if array contains at least one true bit
bool BitArray::any() const {
    for (size_t i = 0; i < m_array_size; ++i) {
        if (m_data[i] != 0) {
            return true;
        }
    }
    return false;
}

// Returns true if all bits are false
bool BitArray::none() const {
    return !any();
}

// Bitwise inversion
BitArray BitArray::operator~() const {
    BitArray result(*this);
    for (size_t i = 0; i < m_array_size; ++i) {
        result.m_data[i] = ~result.m_data[i];
    }
    
    // Clear excess bits in last block
    if (result.m_array_size > 0) {
        size_t bits_in_last_block = result.m_bit_count % BITS_PER_BLOCK;
        if (bits_in_last_block > 0) {
            unsigned long mask = (1UL << bits_in_last_block) - 1;
            result.m_data[result.m_array_size - 1] &= mask;
        }
    }
    
    return result;
}

// Counts number of true bits
int BitArray::count() const {
    int count = 0;
    for (size_t i = 0; i < m_bit_count; ++i) {
        if ((*this)[i]) {
            count++;
        }
    }
    return count;
}

// Returns value of bit at index i
bool BitArray::operator[](int i) const {
    if (i < 0 || static_cast<size_t>(i) >= m_bit_count) {
        throw std::out_of_range("Bit index out of range");
    }
    
    size_t block_idx = block_index(i);
    size_t bit_off = bit_offset(i);
    return (m_data[block_idx] >> bit_off) & 1;
}

// Returns size of array in bits
int BitArray::size() const {
    return static_cast<int>(m_bit_count);
}

// Returns true if array is empty
bool BitArray::empty() const {
    return m_bit_count == 0;
}

// Returns string representation of array
std::string BitArray::to_string() const {
    std::string result;
    for (size_t i = 0; i < m_bit_count; ++i) {
        result += ((*this)[i] ? '1' : '0');
    }
    return result;
}

// Comparison operators
bool operator==(const BitArray& a, const BitArray& b) {
    if (a.size() != b.size()) {
        return false;
    }
    
    for (int i = 0; i < a.size(); ++i) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    
    return true;
}

bool operator!=(const BitArray& a, const BitArray& b) {
    return !(a == b);
}

// Bitwise AND
BitArray operator&(const BitArray& b1, const BitArray& b2) {
    BitArray result(b1);
    result &= b2;
    return result;
}

// Bitwise OR
BitArray operator|(const BitArray& b1, const BitArray& b2) {
    BitArray result(b1);
    result |= b2;
    return result;
}

// Bitwise XOR
BitArray operator^(const BitArray& b1, const BitArray& b2) {
    BitArray result(b1);
    result ^= b2;
    return result;
}