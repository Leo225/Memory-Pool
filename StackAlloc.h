#ifndef MEMORYPOOL_STACKALLOC_H
#define MEMORYPOOL_STACKALLOC_H

#include <memory>

template <typename T>
struct StackNode_
{
	/* data */
	T data;
	StackNode_* prev;
};

template <class T, class Alloc = std::allocator<T> >
class StackAlloc
{
public:
	typedef StackNode_<T> Node;
	typedef typename Alloc::template rebind<Node>::other allocator;
	StackAlloc() { head_ = 0; }
	~StackAlloc() { clear(); }

	bool empty() { return (head_ == 0); }
	void clear()
	{
		Node* curr = head_;
		while (curr != 0)
		{
			Node* tmp = curr->prev;
			allocator_.destory(curr);
			allocator_.deallocate(curr, 1);
			curr = tmp;
		}
		head_ = 0;
	}

	void push(T element)
	{
		Node* newNode = allocator_.allocate(1);
		allocator_.construct(newNode, Node());
		newNode->data = element;
		newNode->prev = head_;
		head_ = newNode;
	}

	T pop()
	{
		T result = head_->data;
		Node* tmp = head_->prev;
		allocator_.destory(head_);
		allocator_.deallocate(head_, 1);
		head_ = tmp;
		return result;
	}

	T top() { return (head_->data); }
	/* data */
private:
	allocate allocator_;
	Node* head_;
};
#endif /*MEMORYPOOL_STACKALLOC_H*/