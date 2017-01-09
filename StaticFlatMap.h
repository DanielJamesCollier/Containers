#ifndef StaticFlatMap_h
#define StaticFlatMap_h
#define NOT_MAPPED -1

#include <iostream>
#include <memory>
#include <array>
#include <type_traits>

enum class MapError {
    OKAY,
    FULL,
    ID_GREATER_THAN_POOLSIZE,
    ID_GREATER_THAN_PARTITION,
    OBJECT_DOES_NOT_EXIST,
};

template <typename T, size_t poolSize>
class StaticFlatMap
{
public:
    StaticFlatMap()
    :
		  m_poolSize{poolSize}
    ,     m_partition{0}
    ,     m_failReturn{}
    {
        m_forwardReverseMap.fill(NOT_MAPPED);
    }
    
    StaticFlatMap(StaticFlatMap &)  = default;
    StaticFlatMap(StaticFlatMap &&) = default;
    ~StaticFlatMap()                = default;
    
    MapError addObject(T const & object) {
        if (m_partition == m_poolSize) {
            return MapError::FULL; // check if the pool is full before adding an object
        }
        
        m_forwardReverseMap[m_partition]              = m_partition;
        m_forwardReverseMap[m_partition + m_poolSize] = m_partition;
        m_pool[m_partition]                           = object;
        
        m_partition++;
        
        return MapError::OKAY;
    }
    
    MapError addObject(T const & object, size_t & staticID) {
        if (m_partition == m_poolSize) {
            return MapError::FULL; // check if the pool is full before adding an object
        }
        
        m_forwardReverseMap[m_partition]              = m_partition;
        m_forwardReverseMap[m_partition + m_poolSize] = m_partition;
        m_pool[m_partition]                           = object;
        staticID                                      = m_partition;
        
        m_partition++;
        
        return MapError::OKAY;
    }
    
    MapError getObjectWithStaticID(size_t staticID, T & object) {
        int forwardMapping = m_forwardReverseMap[staticID];
        
        if (staticID > m_poolSize) {
            object = m_failReturn;
            return MapError::ID_GREATER_THAN_POOLSIZE;
        } else if(staticID > m_partition) {
            object = m_failReturn;
            return MapError::ID_GREATER_THAN_PARTITION;
        } else if(forwardMapping == NOT_MAPPED) {
            object = m_failReturn;
            return MapError::OBJECT_DOES_NOT_EXIST;
        }
        
        object = m_pool[forwardMapping];
        return MapError::OKAY;
    }
    
    MapError getObjectWithPoolID(size_t id, T & object) {
        if (id > m_partition) {
            object = m_failReturn;
            return MapError::ID_GREATER_THAN_PARTITION;
        } else if (id > m_poolSize) {
            object = m_failReturn;
            return MapError::ID_GREATER_THAN_POOLSIZE;
        }
        
        object = m_pool[id];
        return MapError::OKAY;
    }
    
    MapError removeObjectStaticID(size_t staticID) {
        int poolIndextoDelete = m_forwardReverseMap[staticID];
        
        if(staticID > m_partition) {
            return MapError::ID_GREATER_THAN_PARTITION;
        } else if (staticID > m_poolSize) {
            return MapError::ID_GREATER_THAN_POOLSIZE;
        } else if (poolIndextoDelete == NOT_MAPPED) {
            return MapError::OBJECT_DOES_NOT_EXIST;
        }
        
        std::swap(m_pool[poolIndextoDelete], m_pool[--m_partition]);
        
        // re-map after swap
        int reverseMapMapping                  = m_forwardReverseMap[m_partition + m_poolSize];
        m_forwardReverseMap[reverseMapMapping] = poolIndextoDelete;
        m_forwardReverseMap[staticID]          = m_forwardReverseMap[m_partition + m_poolSize] = NOT_MAPPED;
        
        if (poolIndextoDelete != m_partition) {
            m_forwardReverseMap[poolIndextoDelete + m_poolSize] = reverseMapMapping;
        } else {
            m_forwardReverseMap[poolIndextoDelete + m_poolSize] = NOT_MAPPED;
        }
        
        return MapError::OKAY;
    }
    
    size_t getPoolSizeInElements() const {
        return m_poolSize;
    }
    
    size_t getPoolSizeInActiveElements() const {
        return m_partition;
    }
    
    void printPool(size_t printDepth = poolSize) const {
        if(printDepth > poolSize) {
            printDepth = poolSize;
        }
        
        for (size_t i = 0; i < printDepth; i++) {
            
            std::cout << "| " << i << " - " << m_forwardReverseMap[i + m_poolSize]  <<" | "; // print("|index - reverseMap[index]|")
            std::cout << i << " - " << m_forwardReverseMap[i] << " | "; // prints the forward mapp
            
            if (i < m_partition) {
                std::cout << i << "- A";
            } else {
                std::cout << i << "- D";
            }
            std::cout << std::endl;
        }
    }
    
private:
    std::array<int, poolSize * 2> m_forwardReverseMap;
    std::array<T, poolSize>       m_pool;
    T                             m_failReturn;
    int                           m_poolSize;
    int                           m_partition;
};
#endif // StaticFlatMap_h
