#include <cstddef>
#include <stdexcept>
#include <algorithm>

class BitArray
{
    //private...
    unsigned long* m_data;
    size_t m_bit_count; //сколько битов храним
    size_t m_array_size; //размер массива m_data

    static const size_t BITS_PER_BLOCK = sizeof(unsigned long) * 8;

    public:
    BitArray() : m_data(nullptr), m_bit_count(0), m_array_size(0) {}
    ~BitArray() {
        delete [] m_data;
    }
    explicit BitArray(int num_bits, unsigned long value = 0){ //value = 0 - default parametr
        if (num_bits < 0){
            throw std::invalid_argument("BitArray size cannot be <0");
        }
        m_bit_count = static_cast<size_t>(num_bits);
        m_array_size = (m_bit_count + BITS_PER_BLOCK - 1) / BITS_PER_BLOCK;

        if (m_array_size > 0){
            m_data = new unsigned long[m_array_size]();//()-mean [0,0,0,...] zeros
            m_data[0] = value;//first block
        }
        else{
            m_data = nullptr;//null pointer
        }
    }
    BitArray(const BitArray& other) : m_bit_count(other.m_bit_count), m_array_size(other.m_array_size) {
        m_data = new unsigned long[m_array_size];
        std::copy(other.m_data, other.m_data + m_array_size, m_data);
    }
    void swap(BitArray& other){
        std::swap(m_data, other.m_data);
        std::swap(m_bit_count, other.m_bit_count);
        std::swap(m_array_size, other.m_array_size);
    }
    BitArray& operator=(const BitArray& other){
        if (this != &other){
            delete[] m_data;
            m_bit_count = other.m_bit_count;
            m_array_size = other.m_array_size;

            if (m_array_size > 0){
                m_data = new unsigned long[m_array_size];
                std::copy(other.m_data, other.m_data + m_array_size, m_data);
            } else {
                m_data = nullptr;
            }
        }
        return *this; //ссылку на себя
    }
    
    void resize(int num_bits, bool value = false){
        if (num_bits < 0) {
            throw std::invalid_argument("number of bits can't be < 0");
        }
        size_t new_bit_count = static_cast<size_t>(num_bits);

        if (new_bit_count == m_bit_count){
            return;
        }

        size_t new_array_size = (new_bit_count + BITS_PER_BLOCK - 1) / BITS_PER_BLOCK;
        unsigned long* new_data = new unsigned long[new_array_size];
        unsigned long fill_value = value ? ~0UL : 0UL;//~0UL - all bits 1
        for (size_t i = 0; i < new_array_size; ++i){
            new_data[i] = fill_value;
        }
        size_t min_array_size = std::min(m_array_size, new_array_size);
        if (min_array_size > 0){
            std::copy(m_data, m_data + min_array_size, new_data);
        }

        if (new_bit_count < m_bit_count && new_array_size > 0){
            size_t bits_in_last_block = new_bit_count % BITS_PER_BLOCK;
            if (bits_in_last_block > 0){
                unsigned long mask = (1UL << bits_in_last_block) - 1;
                new_data[new_array_size -1] &= mask;
            }
        }
        delete[] m_data;
        m_data = new_data;
        m_bit_count = new_bit_count;
        m_array_size = new_array_size;
    }
    void clear(){
        delete [] m_data;
        m_data = nullptr;
        m_bit_count = 0;
        m_array_size = 0;
    }
};