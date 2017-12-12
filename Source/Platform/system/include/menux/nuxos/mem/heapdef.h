/*!
********************************************************************************
* @file     : nuxos_boot.c
*
* @brief    :
*
* @version  : V0.0.1
*
* @copyright: All Rights Reserved.
********************************************************************************
* @changelog: L.H.T 2017-10-01 first version
*******************************************************************************/
#ifndef INCLUDE_MENUX_NUXOS_MEM_HEAP_DEF_H
#define INCLUDE_MENUX_NUXOS_MEM_HEAP_DEF_H

/*!
********************************************************************************
* INCLUDES
*******************************************************************************/
#include <menux/nuxos/mem/heap.h>

#ifdef __cplusplus
extern "C"
{
#endif

/*!
********************************************************************************
* CONTANTS
*******************************************************************************/
#define MEM_MIN_SHIFT		4
#define MEM_MAX_SHIFT		22

#define MEM_MIN_CHUNK		(1 << MEM_MIN_SHIFT)
#define MEM_MAX_CHUNK		(1 << MEM_MAX_SHIFT)

#define MEM_NNODES			(MEM_MAX_SHIFT - MEM_MIN_SHIFT + 1)

#define MEM_GRAN_MASK		(MEM_MIN_CHUNK - 1)
#define MEM_ALIGN_UP(a)		(((a) + MEM_GRAN_MASK) & ~MEM_GRAN_MASK)
#define MEM_ALIGN_DN(a)		((a) & ~MEM_GRAN_MASK)

#define MEM_ALLOC_BIT		0x80000000

#define SIZEOF_MM_ALLOCNODE	8
#define MM_PTR_SIZE 		sizeof(struct mm_freenode_s *)
#define SIZEOF_MM_FREENODE 	(SIZEOF_MM_ALLOCNODE + 2*MM_PTR_SIZE)

/*!
********************************************************************************
* TYPE DEFINES
*******************************************************************************/
struct mm_allocnode_s
{
	os_size_t size;
	os_size_t preceding;
};
typedef struct mm_allocnode_s mm_allocnode_t;

struct mm_freenode_s
{
	os_size_t size;                  
	os_size_t preceding;             
	struct mm_freenode_s *flink;
	struct mm_freenode_s *blink;
};
typedef struct mm_freenode_s mm_freenode_t;

struct mm_heap_s
{
	os_size_t       mm_heapsize;
	mm_allocnode_t *mm_heapstart;
	mm_allocnode_t *mm_heapend;
	mm_freenode_t   mm_nodelist[MEM_NNODES];
};
typedef struct mm_heap_s mm_heap_t;

/*!
********************************************************************************
* PUBLIC FUNCTIONS
*******************************************************************************/
void  mm_heap_init 				(void *heapbase, os_size_t heapsize);
void  mm_heap_addregion 		(mm_heap_t *, void *, os_size_t);
void  mm_heap_addfreechunk 		(mm_heap_t *, mm_freenode_t *);
void  mm_heap_shrinkchunk		(mm_heap_t *, mm_allocnode_t *, os_size_t);

#ifdef __cplusplus
}
#endif
#endif
