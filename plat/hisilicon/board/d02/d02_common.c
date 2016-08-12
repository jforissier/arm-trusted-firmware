/*
 * Copyright (c) 2015-2016, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2016, Huawei Technologies/Hisilicon. All rights reserved.
 * Copyright (c) 2016, Linaro Limited. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * Neither the name of ARM nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific
 * prior written permission.
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
#include <arch.h>
#include <arch_helpers.h>
#include <debug.h>
#include <platform_def.h>
#include <xlat_tables.h>
#include "d02_private.h"

static const mmap_region_t d02_mmap[] = {

	{ TRUSTED_MAILBOXES_BASE, TRUSTED_MAILBOXES_BASE,
	  TRUSTED_MAILBOXES_SIZE, MT_DEVICE | MT_RW | MT_SECURE },

	{ FLASH_NS_BASE, FLASH_NS_BASE, FLASH_SIZE,
	  MT_DEVICE | MT_RO | MT_SECURE },

	{ DEVICE0_BASE, DEVICE0_BASE, DEVICE0_SIZE,
	  MT_DEVICE | MT_RW | MT_SECURE },

	{ DEVICE1_BASE, DEVICE1_BASE, DEVICE1_SIZE,
	  MT_DEVICE | MT_RW | MT_SECURE },

	{ DRAM_BASE, DRAM_BASE, DRAM_SIZE, MT_MEMORY | MT_RW | MT_NS },

	{ 0 }
};

/*******************************************************************************
 * Macro generating the code for the function setting up the pagetables as per
 * the platform memory map & initialize the mmu, for the given exception level
 ******************************************************************************/
#if USE_COHERENT_MEM
#define DEFINE_CONFIGURE_MMU_EL(_el)					\
	void d02_configure_mmu_el##_el(unsigned long total_base,	\
				   unsigned long total_size,		\
				   unsigned long ro_start,		\
				   unsigned long ro_limit,		\
				   unsigned long coh_start,		\
				   unsigned long coh_limit)		\
	{								\
		mmap_add_region(total_base, total_base,			\
				total_size,				\
				MT_MEMORY | MT_RW | MT_SECURE);		\
		mmap_add_region(ro_start, ro_start,			\
				ro_limit - ro_start,			\
				MT_MEMORY | MT_RO | MT_SECURE);		\
		mmap_add_region(coh_start, coh_start,			\
				coh_limit - coh_start,			\
				MT_DEVICE | MT_RW | MT_SECURE);		\
		mmap_add(d02_mmap);				\
		init_xlat_tables();					\
									\
		enable_mmu_el##_el(0);					\
	}
#else
#define DEFINE_CONFIGURE_MMU_EL(_el)					\
	void d02_configure_mmu_el##_el(unsigned long total_base,	\
				   unsigned long total_size,		\
				   unsigned long ro_start,		\
				   unsigned long ro_limit)		\
	{								\
		mmap_add_region(total_base, total_base,			\
				total_size,				\
				MT_MEMORY | MT_RW | MT_SECURE);		\
		mmap_add_region(ro_start, ro_start,			\
				ro_limit - ro_start,			\
				MT_MEMORY | MT_RO | MT_SECURE);		\
		mmap_add(d02_mmap);				\
		init_xlat_tables();					\
									\
		enable_mmu_el##_el(0);					\
	}
#endif

/* Define EL1 and EL3 variants of the function initialising the MMU */
DEFINE_CONFIGURE_MMU_EL(1)
DEFINE_CONFIGURE_MMU_EL(3)

/*
 * TODO: explain what really happens, because in reality we just need to
 * return to the saved UEFI address
 */
uintptr_t plat_get_ns_image_entrypoint(void)
{
	return 0;
}

/*******************************************************************************
 * Gets SPSR for BL32 entry
 ******************************************************************************/
uint32_t arm_get_spsr_for_bl32_entry(void)
{
	/*
	 * The Secure Payload Dispatcher service is responsible for
	 * setting the SPSR prior to entry into the BL32 image.
	 */
	return 0;
}

unsigned long long plat_get_syscnt_freq(void)
{
	return SYS_COUNTER_FREQ;
}

void d02_flush_l1l2l3_cache(void)
{
	uint32_t val;

	/* Flush L1 + L2 cache to PoC */
	dcsw_op_all(DCCISW);

	/* Flush L3 cache */
	d02_djtag_write(0x20, 0x7, 0x4, CDIEC_ID);
	do {
		val = d02_djtag_read(0x20, 0x4, CDIEC_ID);
	} while (val & 0x1);

	isb();
	dsb();
}
