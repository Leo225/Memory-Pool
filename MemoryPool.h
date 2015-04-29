#ifndef MEMORYPOOL_MEMORYPOOL_H
#define MEMORYPOOL_MEMORYPOOL_H

#include <stddef.h>
#include <limits.h>

template <typename T, size_t BlockSize = 4096>
class MemoryPool
{
public:
	/*Member types*/
	typedef T 	value_type;
	typedef T* 	pointer;
	typedef T&	reference;
	typedef	const T*	const_pointer;
	typedef const T&	const_reference;
	typedef ptrdiff_t	difference_type;

	template <typename U>
	struct rebind
	{
		/* data */
		typedef MemoryPool<U> other;
	};

	MemoryPool() throw;
	MemoryPool(const MemoryPool& memoryPool) throw();
	template <class U>
	MemoryPool(const MemoryPool<U>& memoryPool) throw();
	~MemoryPool() throw();

	pointer address(reference x) const throw();
	const_pointer address(const_reference x) const throw();
	pointer allocate(size_t n = 1, const_pointer hint = 0);
	void deallocate(pointer p, size_t n = 1);
	size_t max_size() const throw();
	void construct(pointer p, const_reference val);
	void destory(pointer p);
	pointer newElement(const_reference val);
	void deleteElement(pointer p);
	/* data */
private:
	union Slot_
	{
		value_type element;
		Slot_* next;
	};

	typedef char* data_pointer_;
	typedef	Slot_ slot_type_;
	typedef Slot_* slot_pointer_;

	slot_pointer_ currentBlock_;
	slot_pointer_ currentSlot_;
	slot_pointer_ lastSlot_;
	slot_pointer_ freeSlots_;

	size_t padPointer(data_pointer_ p, size_t align) const throw();
	void allocateBlock();

};
#endif /*MEMORYPOOL_MEMORYPOOL_H*/