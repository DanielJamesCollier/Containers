#pragma once
#include <algorithm>
#include <iostream>	

/*
	the m_forward reverse map stores to mappings in one contiguous memory block

	m_forwardReverseMap[0 to (m_poolSize -1)]        = the forward map
	m_forwardReverseMap[m_poolSize to m_poolSize -1] = the reverse map
*/

template <class T>
class StaticFlatMap
{
public:
	explicit StaticFlatMap(size_t poolSize)
		:
		m_poolSize(poolSize),
		m_partition(0),
		m_pool(new T[poolSize]),
		m_forwardReverseMap(new int[poolSize * 2]),
		m_failReturn(T())
	{
		// initialiastation
		for (size_t i = 0; i < poolSize; i++)
		{
			m_forwardReverseMap[i] = m_forwardReverseMap[i + poolSize] = -1;
		}
	}

	~StaticFlatMap()
	{
		delete[] m_pool, m_forwardReverseMap;
	}

	int AddObject(T object)
	{
		if (m_partition == m_poolSize) return -1; // check if the pool is full befor adding an object 

		m_forwardReverseMap[m_partition] = m_forwardReverseMap[m_partition + m_poolSize] = m_partition;
		m_pool[m_partition] = object;
		return m_partition++;
	}

	int AddObject(unsigned int id, T object)
	{
		if (m_partition == m_poolSize) return -1; // check if the pool is full befor adding an object 

		if (m_forwardReverseMap[id] != -1) return -1;

		m_forwardReverseMap[id] = m_partition;
		m_forwardReverseMap[m_partition + m_poolSize] = id;

		m_pool[m_partition] = object;
		return m_partition++;
	}


	T& GetObjectWithStaticID(unsigned int id, bool& valid)
	{
		int componentPoolIndex = m_forwardReverseMap[id];

		if (id > m_poolSize || componentPoolIndex == -1 || componentPoolIndex > m_partition)
		{
			valid = false;
			return m_failReturn;
		}

		valid = true;
		return m_pool[componentPoolIndex];
	}

	T& GetObjectWithPoolID(unsigned int id, bool& valid)
	{
		if (id > m_partition || id > m_poolSize)
		{
			valid = false;
			return m_failReturn;
		}

		valid = true;
		return m_pool[id];
	}

	T& GetObjectWithPoolID(unsigned int id,unsigned int& staticID, bool& valid)
	{
		if (id > m_partition || id > m_poolSize)
		{
			valid    = false;
			staticID = 0;
			return m_failReturn;
		}

		valid    = true;
		staticID = m_forwardReverseMap[id + m_poolSize];

		return m_pool[id];
	}
	
	void RemoveObject(unsigned int index)
	{
		if (index > m_poolSize)
		{
			std::cout << "greater than pool size" << std::endl;
			return;
		}

		int poolIndextoDelete = m_forwardReverseMap[index];

		// there is no object mapped so return
		if (poolIndextoDelete == -1) 
		{
			std::cout << "the object at " << index << " could not be deleted as it does not exits" << std::endl;
			return;
		}

		std::swap(m_pool[poolIndextoDelete], m_pool[--m_partition]);

		// re-map after swap
		unsigned int test = m_forwardReverseMap[m_partition + m_poolSize];
		m_forwardReverseMap[test] = poolIndextoDelete;
		m_forwardReverseMap[index] = m_forwardReverseMap[m_partition + m_poolSize] = -1;

		if (poolIndextoDelete != m_partition)
		{
			m_forwardReverseMap[poolIndextoDelete + m_poolSize] = test;
		}
		else
		{
			m_forwardReverseMap[poolIndextoDelete + m_poolSize] = -1;
		}
	}

	unsigned int GetMaxPoolSizeInElements() const
	{
		return m_poolSize;
	}

	// get the number of items store in the pool
	unsigned int GetPoolSizeInElements() const
	{
		return m_partition;
	}

	// returns the size of the pool
	unsigned int GetPoolSizeInBytes() const
	{
		return (sizeof(T)) * m_poolSize;
	}

	// gets the size of the pool plus the size of the data required from handle auto defragmentation 
	unsigned int GetPoolSizeImplementation() const
	{

	}

	void PrintPool() const
	{
		std::cout << "  Rev   |  Forw  |  Pool " << std::endl;
		for (unsigned int i = 0; i < 20; i++)
		{
			
			std::cout << "| " << i << " - " << m_forwardReverseMap[i + m_poolSize] << " | "; // prints first row reverse map
			std::cout << i << " -  " << m_forwardReverseMap[i] << " | "; // prints the forward mapp
			if (i < m_partition)
			{
				std::cout << i << "- A";
			}
			else
			{
				std::cout << i << "- D";
			}
			std::cout << std::endl;
		}
	}

private:
	int* m_forwardReverseMap;
	T*	 m_pool;
	T    m_failReturn;

	size_t m_poolSize;
	size_t m_partition; // dead alive partition
};

