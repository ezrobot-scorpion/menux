/*!
********************************************************************************
* @file     : mm_heap.c
*
* @brief    :
*
* @version  : V0.0.1
*
* @copyright: All Rights Reserved.
********************************************************************************
* @changelog: L.H.T 2017-10-01 first version
*******************************************************************************/
#include <config.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <menux/nuxos/mem/heapimpl.h>

/*!
********************************************************************************
* LOCAL VARIABLES
*******************************************************************************/
static mm_heap_t gx_memheap;

/*!
********************************************************************************
* PUBLIC FUNCTIONS
*******************************************************************************/
/*!
********************************************************************************
* @brief :
*
* @param :
*
* @retval:
*******************************************************************************/
void mm_heap_init(void *heapstart, os_size_t heapsize)
{
	os_size_t i;

	gx_memheap.mm_heapsize = 0;

	memset(gx_memheap.mm_nodelist, 0, sizeof(mm_freenode_t) * MEM_NNODES);
	for (i = 1; i < MEM_NNODES; i++)
	{
		gx_memheap.mm_nodelist[i - 1].flink = &gx_memheap.mm_nodelist[i];
		gx_memheap.mm_nodelist[i].blink = &gx_memheap.mm_nodelist[i - 1];
	}

	mm_heap_mutexinit();

	mm_heap_addregion(&gx_memheap, heapstart, heapsize);
}

/*!
********************************************************************************
* @brief :
*
* @param :
*
* @retval:
*******************************************************************************/
void mm_heap_addfreechunk(mm_heap_t *heap, mm_freenode_t *node)
{
	mm_freenode_t *next;
	mm_freenode_t *prev;

	/* Convert the size to a nodelist index */
	int ndx = mm_heap_size2nodeindex(node->size);

	/* Now put the new node int the next */
	prev = &heap->mm_nodelist[ndx];
	next = heap->mm_nodelist[ndx].flink;
	while (next && next->size && next->size < node->size)
	{
		prev = next, next = next->flink;
	}

	prev->flink = node;
	node->blink = prev;
	node->flink = next;

	if (next != (void *) 0)
	{
		next->blink = node;
	}
}

/*!
********************************************************************************
* @brief :
*
* @param :
*
* @retval:
*******************************************************************************/
void mm_heap_addregion(mm_heap_t *heap, void *heapstart, os_size_t heapsize)
{
	mm_freenode_t *node;
	uintptr_t heapbase;
	uintptr_t heapend;

	heapbase = MEM_ALIGN_UP((uintptr_t )heapstart);
	heapend = MEM_ALIGN_DN((uintptr_t )heapstart + (uintptr_t )heapsize);
	heapsize = heapend - heapbase;

	heap->mm_heapsize += heapsize;

	heap->mm_heapstart = (mm_allocnode_t *) heapbase;
	heap->mm_heapstart->size = SIZEOF_MM_ALLOCNODE;
	heap->mm_heapstart->preceding = MEM_ALLOC_BIT;

	node = (mm_freenode_t *) (heapbase + SIZEOF_MM_ALLOCNODE);
	node->size = heapsize - 2 * SIZEOF_MM_ALLOCNODE;
	node->preceding = SIZEOF_MM_ALLOCNODE;

	heap->mm_heapend = (mm_allocnode_t *) (heapend - SIZEOF_MM_ALLOCNODE);
	heap->mm_heapend->size = SIZEOF_MM_ALLOCNODE;
	heap->mm_heapend->preceding = node->size | MEM_ALLOC_BIT;

	mm_heap_addfreechunk(heap, node);
}

/*!
********************************************************************************
* @brief :
*
* @param :
*
* @retval:
*******************************************************************************/
void *os_mem_brkaddr(void)
{
	uintptr_t brkaddr;

	brkaddr = (uintptr_t) gx_memheap.mm_heapend;
	return brkaddr ? (void *) (brkaddr + SIZEOF_MM_ALLOCNODE) : 0;
}

/*!
********************************************************************************
* @brief :
*
* @param :
*
* @retval:
*******************************************************************************/
void *rt_mem_calloc(os_size_t n, os_size_t elem_size)
{
	void *ret = NULL;

	if ((n > 0) && (elem_size > 0))
	{
		ret = os_heap_zalloc(n * elem_size);
	}

	return ret;
}

/*!
********************************************************************************
* @brief :
*
* @param :
*
* @retval:
*******************************************************************************/
void os_heap_extend(void *mem, os_size_t size)
{
	mm_allocnode_t *oldnode;
	mm_allocnode_t *newnode;
	uintptr_t blockstart;
	uintptr_t blockend;

	/* Make sure that the memory region are properly aligned */
	blockstart = (uintptr_t) mem;
	blockend = blockstart + size;

	/* Take the memory manager semaphore */
	mm_heap_mutexwaitone();

	oldnode = gx_memheap.mm_heapend;

	oldnode->size = size;

	/* Get and initialize the new terminal node in the heap */
	newnode = (mm_allocnode_t *) (blockend - SIZEOF_MM_ALLOCNODE);
	newnode->size = SIZEOF_MM_ALLOCNODE;
	newnode->preceding = oldnode->size | MEM_ALLOC_BIT;

	gx_memheap.mm_heapend = newnode;
	mm_heap_mutexrelease();

	os_heap_free((void *) mem);
}

/*!
********************************************************************************
* @brief :
*
* @param :
*
* @retval:
*******************************************************************************/
void os_heap_free(void *mem)
{
	mm_freenode_t *node;
	mm_freenode_t *prev;
	mm_freenode_t *next;

	if (mem == (void *) 0)
	{
		return;
	}

	mm_heap_mutexwaitone();

	node = (mm_freenode_t *) ((char *) mem - SIZEOF_MM_ALLOCNODE);
	node->preceding &= ~MEM_ALLOC_BIT;

	next = (mm_freenode_t *) ((char *) node + node->size);
	if ((next->preceding & MEM_ALLOC_BIT) == 0)
	{
		mm_allocnode_t *andbeyond;

		andbeyond = (mm_allocnode_t *) ((char *) next + next->size);

		next->blink->flink = next->flink;
		if (next->flink)
		{
			next->flink->blink = next->blink;
		}

		/* Then merge the two chunks */
		node->size += next->size;
		andbeyond->preceding = node->size
				| (andbeyond->preceding & MEM_ALLOC_BIT);
		next = (mm_freenode_t *) andbeyond;
	}

	prev = (mm_freenode_t *) ((char *) node - node->preceding);
	if ((prev->preceding & MEM_ALLOC_BIT) == 0)
	{
		prev->blink->flink = prev->flink;
		if (prev->flink)
		{
			prev->flink->blink = prev->blink;
		}

		prev->size += node->size;
		next->preceding = prev->size | (next->preceding & MEM_ALLOC_BIT);
		node = prev;
	}

	mm_heap_addfreechunk(&gx_memheap, node);
	mm_heap_mutexrelease();
}

/*!
********************************************************************************
* @brief :
*
* @param :
*
* @retval:
*******************************************************************************/
void *os_heap_malloc(os_size_t size)
{
	mm_freenode_t *node;
	void *ret = NULL;
	int ndx;

	if (size < 1)
	{
		return NULL;
	}

	size = MEM_ALIGN_UP(size + SIZEOF_MM_ALLOCNODE);

	mm_heap_mutexwaitone();

	ndx = mm_heap_size2nodeindex(size);

	for (node = gx_memheap.mm_nodelist[ndx].flink; node && node->size < size;
			node = node->flink)
		;

	if (node)
	{
		mm_freenode_t *remainder;
		mm_freenode_t *next;
		os_size_t remaining;

		node->blink->flink = node->flink;
		if (node->flink)
		{
			node->flink->blink = node->blink;
		}

		remaining = node->size - size;
		if (remaining >= SIZEOF_MM_FREENODE)
		{
			next = (mm_freenode_t *) (((char *) node) + node->size);
			remainder = (mm_freenode_t *) (((char *) node) + size);
			remainder->size = remaining;
			remainder->preceding = size;
			node->size = size;
			next->preceding = remaining | (next->preceding & MEM_ALLOC_BIT);

			/* Add the remainder back into the nodelist */
			mm_heap_addfreechunk(&gx_memheap, remainder);
		}

		/* Handle the case of an exact size match */
		node->preceding |= MEM_ALLOC_BIT;
		ret = (void *) ((char *) node + SIZEOF_MM_ALLOCNODE);
	}

	mm_heap_mutexrelease();

	return ret;
}

/*!
********************************************************************************
* @brief :
*
* @param :
*
* @retval:
*******************************************************************************/
void *os_heap_mallocaligned(os_size_t alignment, os_size_t size)
{
	mm_allocnode_t *node;
	os_size_t rawchunk;
	os_size_t alignedchunk;
	os_size_t mask = (os_size_t) (alignment - 1);
	os_size_t allocsize;

	if (alignment <= MEM_MIN_CHUNK)
	{
		return os_heap_malloc(size);
	}

	size = MEM_ALIGN_UP(size); /* Make multiples of our granule size */
	allocsize = size + 2 * alignment; /* Add double full alignment size */

	/* Then malloc that size */
	rawchunk = (os_size_t) os_heap_malloc(allocsize);
	if (rawchunk == 0)
	{
		return NULL;
	}

	mm_heap_mutexwaitone();

	node = (mm_allocnode_t *) (rawchunk - SIZEOF_MM_ALLOCNODE);

	/* Find the aligned subregion */
	alignedchunk = (rawchunk + mask) & ~mask;

	/* Check if there is free space at the beginning of the aligned chunk */
	if (alignedchunk != rawchunk)
	{
		mm_allocnode_t *newnode;
		mm_allocnode_t *next;
		os_size_t precedingsize;

		/* Get the node the next node after the allocation. */

		next = (mm_allocnode_t *) ((char *) node + node->size);
		newnode = (mm_allocnode_t *) (alignedchunk - SIZEOF_MM_ALLOCNODE);
		precedingsize = (os_size_t) newnode - (os_size_t) node;

		if (precedingsize < SIZEOF_MM_FREENODE)
		{
			alignedchunk += alignment;
			newnode = (mm_allocnode_t *) (alignedchunk - SIZEOF_MM_ALLOCNODE);
			precedingsize = (os_size_t) newnode - (os_size_t) node;
		}

		newnode->size = (os_size_t) next - (os_size_t) newnode;
		newnode->preceding = precedingsize | MEM_ALLOC_BIT;
		node->size = precedingsize;
		node->preceding &= ~MEM_ALLOC_BIT;
		next->preceding = newnode->size | (next->preceding & MEM_ALLOC_BIT);
		allocsize = newnode->size - SIZEOF_MM_ALLOCNODE;

		/* Add the original, newly freed node to the free nodelist */
		mm_heap_addfreechunk(&gx_memheap, (mm_freenode_t *) node);

		node = newnode;
	}

	/* Check if there is free space at the end of the aligned chunk */

	if (allocsize > size)
	{
		mm_heap_shrinkchunk(&gx_memheap, node, size + SIZEOF_MM_ALLOCNODE);
	}

	//mem_givesemaphore(heap);
	return (void *) alignedchunk;
}

/*!
********************************************************************************
* @brief :
*
* @param :
*
* @retval:
*******************************************************************************/
void *os_heap_realloc(void *oldmem, os_size_t size)
{
	mm_allocnode_t *oldnode;
	mm_freenode_t *prev;
	mm_freenode_t *next;
	os_size_t newsize;
	os_size_t oldsize;
	os_size_t prevsize = 0;
	os_size_t nextsize = 0;
	void *newmem;

	/* If oldmem is RT_NULL, then realloc is equivalent to malloc */
	if (oldmem == NULL)
	{
		return os_heap_malloc(size);
	}

	/* If size is zero, then realloc is equivalent to free */
	if (size < 1)
	{
		os_heap_free(oldmem);
		return NULL;
	}

	newsize = MEM_ALIGN_UP(size + SIZEOF_MM_ALLOCNODE);

	/* Map the memory chunk into an allocated node structure */
	oldnode = (mm_allocnode_t *) ((char *) oldmem - SIZEOF_MM_ALLOCNODE);

	/* We need to hold the MM semaphore while we muck with the nodelist. */
	mm_heap_mutexwaitone();

	/* Check if this is a request to reduce the size of the allocation. */
	oldsize = oldnode->size;
	if (newsize <= oldsize)
	{
		if (newsize < oldsize)
		{
			mm_heap_shrinkchunk(&gx_memheap, oldnode, newsize);
		}

		mm_heap_mutexrelease();

		return oldmem;
	}

	next = (mm_freenode_t *) ((char *) oldnode + oldnode->size);
	if ((next->preceding & MEM_ALLOC_BIT) == 0)
	{
		nextsize = next->size;
	}

	prev = (mm_freenode_t *) ((char *) oldnode
			- (oldnode->preceding & ~MEM_ALLOC_BIT));
	if ((prev->preceding & MEM_ALLOC_BIT) == 0)
	{
		prevsize = prev->size;
	}

	/* Now, check if we can extend the current allocation or not */
	if (nextsize + prevsize + oldsize >= newsize)
	{
		os_size_t needed = newsize - oldsize;
		os_size_t takeprev = 0;
		os_size_t takenext = 0;

		if (prevsize > 0 && (nextsize >= prevsize || nextsize < 1))
		{
			if (needed > prevsize)
			{
				takeprev = prevsize;
				takenext = needed - prevsize;
			}
			else
			{
				takeprev = needed;
				takenext = 0;
			}

			needed = 0;
		}

		if (nextsize > 0 && needed)
		{
			if (needed > nextsize)
			{
				takeprev = needed - nextsize;
				takenext = nextsize;
			}
			else
			{
				/* Yes, take what we need from the previous chunk */
				takeprev = 0;
				takenext = needed;
			}
		}

		/* Extend into the previous free chunk */
		newmem = oldmem;
		if (takeprev)
		{
			mm_allocnode_t *newnode;
			prev->blink->flink = prev->flink;
			if (prev->flink)
			{
				prev->flink->blink = prev->blink;
			}

			newnode = (mm_allocnode_t *) ((char *) oldnode - takeprev);
			if (takeprev < prevsize)
			{
				prev->size -= takeprev;
				newnode->size = oldsize + takeprev;
				newnode->preceding = prev->size | MEM_ALLOC_BIT;
				next->preceding = newnode->size
						| (next->preceding & MEM_ALLOC_BIT);
				mm_heap_addfreechunk(&gx_memheap, prev);
			}
			else
			{
				newnode->size += oldsize;
				newnode->preceding |= MEM_ALLOC_BIT;
				next->preceding = newnode->size
						| (next->preceding & MEM_ALLOC_BIT);
			}

			/* Now we want to return newnode */
			oldnode = newnode;
			oldsize = newnode->size;
			newmem = (void *) ((char *) newnode + SIZEOF_MM_ALLOCNODE);
			memcpy(newmem, oldmem, oldsize - SIZEOF_MM_ALLOCNODE);
		}

		/* Extend into the next free chunk */
		if (takenext)
		{
			mm_freenode_t *newnode;
			mm_allocnode_t *andbeyond;

			andbeyond = (mm_allocnode_t *) ((char *) next + nextsize);

			next->blink->flink = next->flink;
			if (next->flink)
			{
				next->flink->blink = next->blink;
			}

			/* Extend the node into the next chunk */
			oldnode->size = oldsize + takenext;
			newnode = (mm_freenode_t *) ((char *) oldnode + oldnode->size);

			/* Did we consume the entire preceding chunk? */
			if (takenext < nextsize)
			{
				newnode->size = nextsize - takenext;
				newnode->preceding = oldnode->size;
				andbeyond->preceding = newnode->size
						| (andbeyond->preceding & MEM_ALLOC_BIT);
				mm_heap_addfreechunk(&gx_memheap, newnode);
			}
			else
			{
				andbeyond->preceding = oldnode->size
						| (andbeyond->preceding & MEM_ALLOC_BIT);
			}
		}

		mm_heap_mutexrelease();
		return newmem;
	}
	else
	{
		mm_heap_mutexrelease();
		newmem = (void *) os_heap_malloc(size);
		if (newmem)
		{
			memcpy(newmem, oldmem, oldsize);
			os_heap_free(oldmem);
		}

		return newmem;
	}
}

/*!
********************************************************************************
* @brief :
*
* @param :
*
* @retval:
*******************************************************************************/
void mm_heap_shrinkchunk(mm_heap_t *heap, mm_allocnode_t *node, os_size_t size)
{
	mm_freenode_t *next;

	next = (mm_freenode_t *) ((char *) node + node->size);

	if ((next->preceding & MEM_ALLOC_BIT) == 0)
	{
		mm_allocnode_t *andbeyond;
		mm_freenode_t *newnode;

		andbeyond = (mm_allocnode_t *) ((char *) next + next->size);
		next->blink->flink = next->flink;
		if (next->flink)
		{
			next->flink->blink = next->blink;
		}

		newnode = (mm_freenode_t *) ((char *) node + size);

		newnode->size = next->size + node->size - size;
		newnode->preceding = size;
		node->size = size;
		andbeyond->preceding = newnode->size
				| (andbeyond->preceding & MEM_ALLOC_BIT);

		mm_heap_addfreechunk(heap, newnode);
	}
	else if (node->size >= size + SIZEOF_MM_FREENODE)
	{
		mm_freenode_t *newnode;
		newnode = (mm_freenode_t *) ((char *) node + size);

		newnode->size = node->size - size;
		newnode->preceding = size;
		node->size = size;
		next->preceding = newnode->size | MEM_ALLOC_BIT;

		mm_heap_addfreechunk(heap, newnode);
	}
}

/*!
********************************************************************************
* @brief :
*
* @param :
*
* @retval:
*******************************************************************************/
int mm_heap_size2nodeindex(os_size_t size)
{
	int ndx = 0;

	if (size >= MEM_MAX_CHUNK)
	{
		return MEM_NNODES - 1;
	}

	size >>= MEM_MIN_SHIFT;
	while (size > 1)
	{
		ndx++;
		size >>= 1;
	}

	return ndx;
}

/*!
********************************************************************************
* @brief :
*
* @param :
*
* @retval:
*******************************************************************************/
void *os_heap_zalloc(os_size_t size)
{
	void *alloc = os_heap_malloc(size);
	if (alloc)
	{
		memset(alloc, 0, size);
	}

	return alloc;
}
