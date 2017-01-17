#ifndef StaticFlatMap_h
#define StaticFlatMap_h

#define NOT_MAPPED -1

#include <iostream>
#include <memory>
#include <type_traits>

#include "StaticArray.h"
#include "AssertMacros.h"

namespace nova {
    enum class MapError {
        OKAY,
        FULL,
        OBJECT_DOES_NOT_EXIST,
    };
    
    template<typename TYPE, size_t SIZE>
    class ID final {
        friend class StaticArray<TYPE, SIZE>;
        ID() = delete;
        ID(ID const & rhs) = default;
    private:
        ID(size_t index, size_t version) : m_index(index), m_version(version){};
        size_t m_index;
        size_t m_version;
    };
    
    
    template <typename TYPE, size_t SIZE>
    class StaticFlatMap final {
    public:
        StaticFlatMap();
        StaticFlatMap(StaticFlatMap const & rhs) = default;
        StaticFlatMap(StaticFlatMap && rhs)      = default;
        ~StaticFlatMap()                         = default;
        
        StaticFlatMap & operator = (StaticFlatMap const & rhs) = default;
        StaticFlatMap & operator = (StaticFlatMap && rhs)      = default;
        
        MapError addObject(TYPE const & object);
        MapError addObject(TYPE const & object, size_t & staticID);
        MapError getObject(size_t staticID, TYPE & object);
        MapError removeObject(size_t staticID);
        bool     isMapped(size_t index);
        size_t   size() const;
        size_t   capacity() const;
        
        // C++ conform begin & end - loops over contig pool
        TYPE * begin();
        TYPE const * begin() const;
        TYPE * end();
        TYPE const * end() const;
        
        // custom begin end - loops over static pool
        size_t  beginStatic();
        size_t  endStatic();
        
        
    private:
        StaticArray<int, SIZE * 2> m_forwardReverseMap;
        StaticArray<TYPE, SIZE>    m_pool;
        TYPE                       m_failReturn;
        int                        m_partition;
    };
    
    //-----------------------------------------------------------------
    template<typename TYPE, size_t SIZE>
    StaticFlatMap<TYPE, SIZE>::StaticFlatMap()
    :
    m_partition{0}
    ,     m_failReturn{}
    {
        m_forwardReverseMap.fill(NOT_MAPPED);
    }
    
    //-----------------------------------------------------------------
    template<typename TYPE, size_t SIZE> MapError
    StaticFlatMap<TYPE, SIZE>::addObject(TYPE const & object) {
        if (m_partition == SIZE) {
            return MapError::FULL; // check if the pool is full before adding an object
        }
        
        m_forwardReverseMap[m_partition]        = m_partition;
        m_forwardReverseMap[m_partition + SIZE] = m_partition;
        m_pool[m_partition]                     = object;
        
        m_partition++;
        
        return MapError::OKAY;
    }
    
    //-----------------------------------------------------------------
    template<typename TYPE, size_t SIZE> MapError
    StaticFlatMap<TYPE, SIZE>::addObject(TYPE const & object, size_t & staticID) {
        if (m_partition == SIZE) {
            return MapError::FULL; // check if the pool is full before adding an object
        }
        
        m_forwardReverseMap[m_partition]        = m_partition;
        m_forwardReverseMap[m_partition + SIZE] = m_partition;
        m_pool[m_partition]                     = object;
        staticID                                = m_partition;
        
        m_partition++;
        
        return MapError::OKAY;
    }
    
    //-----------------------------------------------------------------
    template<typename TYPE, size_t SIZE> MapError
    StaticFlatMap<TYPE, SIZE>::getObject(size_t staticID, TYPE & object) {
        assert_on_range(staticID, m_partition);
        
        int forwardMapping = m_forwardReverseMap[staticID];
        
        if(forwardMapping == NOT_MAPPED) {
            object = m_failReturn;
            return MapError::OBJECT_DOES_NOT_EXIST;
        }
        
        object = m_pool[forwardMapping];
        return MapError::OKAY;
    }
    
    //-----------------------------------------------------------------
    template<typename TYPE, size_t SIZE> MapError
    StaticFlatMap<TYPE, SIZE>::removeObject(size_t staticID) {
        assert_on_range(staticID, m_partition);
        
        int poolIndextoDelete = m_forwardReverseMap[staticID];
        
        if (poolIndextoDelete == NOT_MAPPED) {
            return MapError::OBJECT_DOES_NOT_EXIST;
        }
        
        std::swap(m_pool[poolIndextoDelete], m_pool[--m_partition]);
        
        // re-map after swap
        int reverseMapMapping                  = m_forwardReverseMap[m_partition + SIZE];
        m_forwardReverseMap[reverseMapMapping] = poolIndextoDelete;
        m_forwardReverseMap[staticID]          = m_forwardReverseMap[m_partition + SIZE] = NOT_MAPPED;
        
        if (poolIndextoDelete != m_partition) {
            m_forwardReverseMap[poolIndextoDelete + SIZE] = reverseMapMapping;
        } else {
            m_forwardReverseMap[poolIndextoDelete + SIZE] = NOT_MAPPED;
        }
        
        return MapError::OKAY;
    }
    
    //-----------------------------------------------------------------
    template<typename TYPE, size_t SIZE> bool
    StaticFlatMap<TYPE, SIZE>::isMapped(size_t index) {
        assert_on_range(index, m_partition);
        assert_on_range(index, SIZE);
        
        return m_forwardReverseMap[index] != -1 ? 1 : 0;
    }
    
    //-----------------------------------------------------------------
    template<typename TYPE, size_t SIZE> size_t
    StaticFlatMap<TYPE, SIZE>::size() const {
        return m_partition;
    }
    
    //-----------------------------------------------------------------
    template<typename TYPE, size_t SIZE> size_t
    StaticFlatMap<TYPE, SIZE>::capacity() const {
        return SIZE;
    }
    
    //-----------------------------------------------------------------
    template<typename TYPE, size_t SIZE> TYPE *
    StaticFlatMap<TYPE, SIZE>::begin() {
        return &(this->m_pool[0]);
    }
    
    //-----------------------------------------------------------------
    template<typename TYPE, size_t SIZE> TYPE const *
    StaticFlatMap<TYPE, SIZE>::begin() const {
        return &(this->m_pool[0]);
    }
    
    //-----------------------------------------------------------------
    template<typename TYPE, size_t SIZE> TYPE *
    StaticFlatMap<TYPE, SIZE>::end() {
        return &(this->m_pool[m_partition]);
    }
    //-----------------------------------------------------------------
    template<typename TYPE, size_t SIZE> TYPE const *
    StaticFlatMap<TYPE, SIZE>::end() const {
        return &(this->m_pool[m_partition]);
    }
    
    //-----------------------------------------------------------------
    template<typename TYPE, size_t SIZE> size_t
    StaticFlatMap<TYPE, SIZE>::beginStatic() {
        return 0;
    }
    
    // being static returns int 0
    // begin end return m_part
    // T& getStatic(index, valid)
    
    //-----------------------------------------------------------------
    template<typename TYPE, size_t SIZE> size_t
    StaticFlatMap<TYPE, SIZE>::endStatic() {
        return m_partition;
    }
    
    
} // namespace nova
#endif // StaticFlatMap_h
