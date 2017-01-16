#ifndef StaticArray_h
#define StaticArray_h

#include <cassert>

template<typename TYPE, size_t SIZE>
class StaticArray {
public:
    StaticArray() = default;
    ~StaticArray() = default;
    StaticArray(StaticArray const & rhs) = default;
    StaticArray(StaticArray && rhs) = default;
    
    StaticArray & operator = (StaticArray const & rhs) = default;
    StaticArray & operator = (StaticArray && rhs) = default;
    
    // read and write access
    TYPE & operator [] (size_t index);
    
    // read only access
    TYPE const & operator [] (size_t index) const;
    
    // get the number of elements in the array
    size_t size() const;
    
    // fill the array with a value
    void fill(TYPE const & val);
    
    // C++ conform begin and end functions
    TYPE * begin();
    TYPE const * begin() const;
    TYPE * end();
    TYPE const * end() const;
 
private:
    TYPE m_data[SIZE];
};

//-----------------------------------------------------------------
template<typename TYPE, size_t SIZE> TYPE &
StaticArray<TYPE, SIZE>::operator [] (size_t index) {
    assert(index > SIZE);
    return this->m_data[index];
}

//-----------------------------------------------------------------
template<typename TYPE, size_t SIZE> TYPE const &
StaticArray<TYPE, SIZE>::operator [] (size_t index) const {
    assert(index > SIZE);
    return this->m_data[index];
}

//-----------------------------------------------------------------
template<typename TYPE, size_t SIZE> size_t
StaticArray<TYPE, SIZE>::size() const {
    return SIZE;
}

//-----------------------------------------------------------------
template<typename TYPE, size_t SIZE> void
StaticArray<TYPE, SIZE>::fill(TYPE const & val) {
    for(auto & i : this->m_data) {
        i = val;
    }
}

//-----------------------------------------------------------------
template<typename TYPE, size_t SIZE> TYPE *
StaticArray<TYPE, SIZE>::begin() {
    return &(this->m_data[0]);
}

//-----------------------------------------------------------------
template<typename TYPE, size_t SIZE> TYPE const *
StaticArray<TYPE, SIZE>::begin() const {
    return &(this->m_data[0]);
}

//-----------------------------------------------------------------
template<typename TYPE, size_t SIZE> TYPE *
StaticArray<TYPE, SIZE>::end() {
    return &(this->m_data[SIZE]);
}

//-----------------------------------------------------------------
template<typename TYPE, size_t SIZE> TYPE const *
StaticArray<TYPE, SIZE>::end() const {
    return &(this->m_data[SIZE]);
}

#endif // StaticArray_h
