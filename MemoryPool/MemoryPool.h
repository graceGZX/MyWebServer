#ifndef MEMORYPOOL_H
#define MEMORYPOOL_H

#include <cstdio>
#include <cstddef>
#include <cstdlib>
#include <stdint.h>
#include <stdarg.h>
#include <utility>
#include "../lock/locker.h"
#include <functional>

#define BlockSize 4096

struct Slot{
	Slot* next;
};

class MemoryPool{
private:
	int slot_size;	
		
	Slot* currentBlock;
	Slot* currentSlot;
	Slot* lastSlot;
	Slot* freeSlot;
	
	locker m_freeSlot;
	locker m_other;
	size_t padPointer(char* p,size_t align);
	Slot* allocateBlock();
	Slot* nofree_solve();	

public:
	MemoryPool();
	~MemoryPool();
	void init(int size);	
	Slot* allocate();
	void deallocate(Slot* p);

};


void init_memorypool();
void* use_memory(size_t size);
void free_memory(size_t size,void *p);
MemoryPool& get_memorypool(int id);

template<class T,class... Args>
T* newElement(Args&&... args){
	T *p;
	if(p=reinterpret_cast<T *>(use_memory(sizeof(T))))
		new(p)T(std::forward<Args>(args)...);
	return p;
}

template<class T>
void deleteElement(T* p){
	if(p)
		p->~T();
	free_memory(sizeof(T),reinterpret_cast<void *>(p));
}

#endif
