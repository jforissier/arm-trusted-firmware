/*
 * Copyright (c) 2014-2015, ARM Limited and Contributors. All rights reserved.
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

#ifndef __D02_PRIVATE_H__
#define __D02_PRIVATE_H__

#include <stdint.h>
#include <debug.h>

enum power_state {
	power_on,
	power_retention,
	power_off
};

enum system_state {
	system_shutdown,
	system_reboot
};

/* d02_common.c */

void d02_configure_mmu_el1(unsigned long total_base, unsigned long total_size,
			   unsigned long ro_start, unsigned long ro_limit,
			   unsigned long coh_start, unsigned long coh_limit);

void d02_configure_mmu_el3(unsigned long total_base, unsigned long total_size,
			   unsigned long ro_start, unsigned long ro_limit,
			   unsigned long coh_start, unsigned long coh_limit);

void d02_io_setup(void);

void d02_flush_l1l2l3_cache(void);

/* d02_sysconf.c */

void d02_get_nm_config(uint64_t mpidr, uint32_t flags, uint32_t *domain_cluster);

uint32_t d02_djtag_read(uint32_t offset, uint32_t chain_id, uint32_t node_id);

#define DJTAG_READ_INVALID_VALUE 0xFFFFFFFF

void d02_djtag_write(uint32_t offset, uint32_t value, uint32_t chain_id,
		     uint32_t node_id);

int d02_set_sys_power_state(enum power_state state);

/* d02_gic.c */

void d02_gic_setup(void);

void d02_gic_cpuif_setup(unsigned int gicc_base);

void d02_gic_cpuif_deactivate(unsigned int gicc_base);

void d02_ic_enable_one_cpu(unsigned int gicd_base, unsigned int core_id);

void d02_ic_disable_one_cpu(unsigned int gicd_base, unsigned int core_id);

#endif /* __D02_PRIVATE_H__ */
