/*
 * Copyright (c) 2011 Intel Corporation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of the copyright holder nor the names of its
 *      contributors may be used to endorse or promote products derived from
 *      this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef HAX_WINDOWS_HAX_TYPES_WINDOWS_H_
#define HAX_WINDOWS_HAX_TYPES_WINDOWS_H_

//#include <ntddk.h>
#include <ntifs.h>
#include <errno.h>

#if defined(_WIN32) && !defined(__cplusplus)
#define inline __inline
typedef unsigned char bool;
#define true 1
#define false 0

/* Remove this later */
#define is_leaf(x)  1
#endif

typedef KAFFINITY cpumap_t;
inline cpumap_t cpu2cpumap(int cpu)
{
    return ((KAFFINITY)0x1 << cpu);
}

typedef KIRQL preempt_flag;

// Signed Types
typedef signed char         int8;
typedef signed short        int16;
typedef signed int          int32;
typedef signed long long    int64;

// Unsigned Types
typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
typedef unsigned int        uint;
typedef unsigned long long  uint64;
typedef unsigned long       ulong;

typedef unsigned char       uint8_t;
typedef unsigned short      uint16_t;
typedef unsigned int        uint32_t;
typedef unsigned long long  uint64_t;
typedef unsigned long       ulong_t;

#include "../hax_list.h"
struct hax_page {
    void *kva;
    PMDL pmdl;
    uint64_t pa;
    uint32_t order;
    uint32_t flags;
    struct hax_link_list list;
};

typedef struct hax_memdesc_user {
    PMDL pmdl;
} hax_memdesc_user;

typedef struct hax_kmap_user {
    PMDL pmdl;
} hax_kmap_user;

typedef struct hax_memdesc_phys {
    PMDL pmdl;
} hax_memdesc_phys;

typedef struct hax_kmap_phys {
    PVOID kva;
    // size == PAGE_SIZE_4K
} hax_kmap_phys;

typedef struct {
    KSPIN_LOCK lock;
    uint32_t flags;
    KIRQL old_irq;
} hax_spinlock;

typedef FAST_MUTEX *hax_mutex;

#define PAGE_MASK (((mword)0x1 << 12) - 1)
/* In DDK, the InterlockedXXX using ULONG, which is in fact 32bit */
typedef LONG hax_atomic_t;

/* Return the value before add */
static hax_atomic_t hax_atomic_add(volatile hax_atomic_t *atom, ULONG value)
{
    return InterlockedExchangeAdd(atom, value);
}

/* Return the value before the increment */
static hax_atomic_t hax_atomic_inc(volatile hax_atomic_t *atom)
{
    return InterlockedIncrement(atom) - 1;
}

/* Return the value before the decrement */
static hax_atomic_t hax_atomic_dec(volatile hax_atomic_t *atom)
{
    return InterlockedDecrement(atom) + 1;
}

#define ALIGNED(x) __declspec(align(x))
#define PACKED

#if defined(_X86_)
typedef uint32_t mword;
#endif

#if defined (_AMD64_)
typedef uint64_t mword;
#endif

typedef mword HAX_VADDR_T;

static inline void smp_mb(void)
{
    KeMemoryBarrier();
}

#endif  // HAX_WINDOWS_HAX_TYPES_WINDOWS_H_
