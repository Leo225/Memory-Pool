#include <time.h>
#include <vector>
#include <cassert>
#include <iostream>

#include "MemoryPool.h"
#include "StackAlloc.h"

#define ELEMS 1000000
#define REPS 50

int int main(int argc, char const *argv[])
{
	/* code */
	clock_t start;

	std::cout << "Provide to compare the default allocator to MemoryPool. \n\n";
	/*Use the default allocator*/
	StackAlloc<int, std::allocator<int> > stackDefault;
	start = clock();
	for (int j = 0; j < REPS; j++)
	{
		assert(stackDefault.empty());
		for (int i = 0; i < ELEMS / 4; i++)
		{
			stackDefault.push(i);
			stackDefault.push(i);
			stackDefault.push(i);
			stackDefault.push(i);
		}
		for (int i = 0; i < ELEMS / 4; i++)
		{
			stackDefault.pop();
			stackDefault.pop();
			stackDefault.pop();
			stackDefault.pop();
		}
	}
	std::cout << "Default Allocator Time: ";
	std::cout << (((double)clock()- start) / CLOCKS_PER_SEC) << "\n\n";

	/*Use MemoryPool*/
	StackAlloc<int, MemoryPool<int> > stackPool;
	start = clock();
	for (int j = 0; j < REPS; j++)
	{
		assert(stackPool.empty());
		for (int i = 0; i < ELEMS / 4; i++)
		{
			stackPool.push(i);
			stackPool.push(i);
			stackPool.push(i);
			stackPool.push(i);
		}
		for (int i = 0; i < ELEMS / 4; i++)
		{
			stackPool.pop();
			stackPool.pop();
			stackPool.pop();
			stackPool.pop();
		}
	}
	std::cout << "MemoryPool Allocator Time: ";
	std::cout << (((double)clock() - start) / CLOCKS_PER_SEC) << "\n\n";

	std::vector<int> stackVector;
	start = clock();
	for (int j = 0; j < REPS; j++)
	{
		assert(stackVector.empty());
		for (int i = 0; i < ELEMS / 4; i++)
		{
			stackVector.push_back(i);
			stackVector.push_back(i);
			stackVector.push_back(i);
			stackVector.push_back(i);
		}
		for (int i = 0; i < ELEMS / 4; i++)
		{
			stackVector.pop_back();
			stackVector.pop_back();
			stackVector.pop_back();
			stackVector.pop_back();
		}
	}
	std::cout << "Vector time: ";
	std::cout << (((double)clock() - start) / CLOCKS_PER_SEC) << "\n\n";
	return 0;
}