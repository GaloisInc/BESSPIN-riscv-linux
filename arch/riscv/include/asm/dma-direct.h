/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_RISCV_DMA_DIRECT_H
#define _ASM_RISCV_DMA_DIRECT_H 1

#include <linux/dma-mapping.h>

static inline dma_addr_t __phys_to_dma(struct device *dev, phys_addr_t paddr)
{
	dma_addr_t dev_addr = (dma_addr_t)paddr;

	return dev_addr - ((dma_addr_t)dev->dma_pfn_offset << PAGE_SHIFT);
}

static inline phys_addr_t __dma_to_phys(struct device *dev, dma_addr_t dev_addr)
{
	phys_addr_t paddr = (phys_addr_t)dev_addr;

	return paddr + ((phys_addr_t)dev->dma_pfn_offset << PAGE_SHIFT);
}

static inline bool dma_capable(struct device *dev, dma_addr_t addr, size_t size)
{
	if (!dev->dma_mask)
		return false;

	/* todo: this should be based on default dma reserved memory region */
	if ((addr < 0xa0000000) ||
	    ((addr + size) > 0xc0000000))
		return false;

	return true;
}

#endif /* _ASM_RISCV_DMA_DIRECT_H */
