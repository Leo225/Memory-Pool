#include "MemoryPool.h"

template <typename T, size_t BlockSize>
inline size_t MemoryPool<T, BlockSize>::padPointer(data_pointer_ p, size_t align) const throw()
{
	size_t result = reinterpret_cast<size_t>(p);
	return ((align - result) % align);
}

template <typename T, size_t BlockSize>
MemoryPool<T, BlockSize>::MemoryPool() throw()
{
	currentBlock_ = 0;
	currentSlot_ = 0;
	lastSlot_ = 0;
	freeSlots_ = 0;
}

template <typename T, size_t BlockSize>
MemoryPool<T, BlockSize>::MemoryPool(const MemoryPool& memoryPool) throw()
{
	MemoryPool();
}

template <typename T, size_t BlockSize>
template <class U>
MemoryPool<T, BlockSize>::MemoryPool(const MemoryPool<U>& memoryPool) throw()
{
	MemoryPool();
}

template <typename T, size_t BlockSize>
MemoryPool<T, BlockSize>::~MemoryPool() throw()
{
	slot_pointer_ curr = currentBlock_;
	while (curr != 0)
	{
		slot_pointer_ prev = curr->next;
		operator delete(reinterpret_cast<void*>(curr));
		curr = prev;
	}
}

template <typename T, size_t BlockSize>
inline typename MemoryPool<T, BlockSize>::pointer
MemoryPool<T, BlockSize>::address(reference x) const throw()
{
	return &x;
}

template <typename T, size_t BlockSize>
inline typename MemoryPool<T, BlockSize>::const_pointer
MemoryPool<T, BlockSize>::address(const_reference x) const throw()
{
	return &x;
}

template <typename T, size_t BlockSize>
void MemoryPool<T, BlockSize>::allocateBlock()
{
	data_pointer_ newBlock = reinterpret_cast<data_pointer_>(operator new(BlockSize));
	reinterpret_cast<slot_pointer_>(newBlock)->next = currentBlock_;
	currentBlock_ =reinterpret_cast<slot_pointer_>(newBlock);
	data_pointer_ body = newBlock + sizeof(slot_pointer_);
	size_t bodyPadding = padPointer(body, sizeof(slot_type_));
	currentSlot_ = reinterpret_cast<slot_pointer_>(body + bodyPadding);
	lastSlot_ = reinterpret_cast<slot_pointer_>(newBlock + BlockSize - sizeof(slot_type_) + 1);
}

template <typename T, size_t BlockSize>
inline typename MemoryPool<T, BlockSize>::pointer
MemoryPool<T, BlockSize>::allocate(size_t, const_pointer)
{
	if (freeSlots_ != 0)
	{
		pointer result = reinterpret_cast<pointer>(freeSlots_);
		freeSlots_ = freeSlots_->next;
		return result;
	}
	else
	{
		if (currentSlot_>= lastSlot_) allocateBlock();
		return reinterpret_cast<pointer>(currentSlot_++);
	}
}

template <typename T, size_t BlockSize>
inline void MemoryPool<T, BlockSize>::deallocate(pointer p, size_t)
{
	if (p != 0)
	{
		reinterpret_cast<slot_pointer_>(p)->next = freeSlots_;
		freeSlots_ = reinterpret_cast<slot_pointer_>(p);
	}
}

template <typename T, size_t BlockSize>
inline size_t MemoryPool<T, BlockSize>::max_size() const throw()
{
	size_t maxBlocks = -1 / BlockSize;
	return (BlockSize - sizeof(data_pointer_)) / sizeof(slot_type_) * maxBlocks;
}

template <typename T, size_t BlockSize>
inline void MemoryPool<T, BlockSize>::construct(pointer p, const_reference val)
{
	new (p) value_type(val);
}

template <typename T, size_t BlockSize>
inline void MemoryPool<T, BlockSize>::destroy(pointer p)
{
	p->~value_type();
}

template <typename T, size_t BlockSize>
inline typename MemoryPool<T, BlockSize>::pointer
MemoryPool<T, BlockSize>::newElement(const_reference val)
{
	pointer result = allocate();
	construct(result, val);
	return result;
}

template <typename T, size_t BlockSize>
inline void MemoryPool<T, BlockSize>::deleteElement(pointer p)
{
	if (p != 0)
	{
		p->~value_type();
		deallocate(p);
	}
}