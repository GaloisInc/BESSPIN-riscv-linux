// SPDX-License-Identifier: GPL-2.0
/*
 *  Based on linux/arch/arm/mm/dma-mapping-nommu.c
 *
 *  Copyright (C) 2019 Darius Rad
 */

#include <linux/dma-direct.h>
#include <linux/mm.h>
#include <linux/swiotlb.h>

#include <asm/dma.h>

static void *riscv_uncached_dma_alloc(struct device *dev, size_t size,
				      dma_addr_t *dma_handle, gfp_t gfp,
				      unsigned long attrs)

{
	void *ret = dma_alloc_from_global_coherent(size, dma_handle);

	/*
	 * dma_alloc_from_global_coherent() may fail because:
	 *
	 * - no consistent DMA region has been defined
	 * - there is no space left in consistent DMA region
	 */

	WARN_ON_ONCE(ret == NULL);
	return ret;
}

static void riscv_uncached_dma_free(struct device *dev, size_t size,
				    void *cpu_addr, dma_addr_t dma_addr,
				    unsigned long attrs)
{
	int ret = dma_release_from_global_coherent(get_order(size),
						   cpu_addr);

	WARN_ON_ONCE(ret == 0);
	return;
}

const struct dma_map_ops riscv_uncached_dma_ops = {
	.mapping_error		= dma_direct_mapping_error,
	.alloc			= riscv_uncached_dma_alloc,
	.free			= riscv_uncached_dma_free,
	.sync_single_for_cpu	= swiotlb_sync_single_for_cpu,
	.sync_single_for_device	= swiotlb_sync_single_for_device,
	.sync_sg_for_cpu	= swiotlb_sync_sg_for_cpu,
	.sync_sg_for_device	= swiotlb_sync_sg_for_device,
	.map_sg			= swiotlb_map_sg_attrs,
	.unmap_sg		= swiotlb_unmap_sg_attrs,
	.map_page		= swiotlb_map_page,
	.unmap_page		= swiotlb_unmap_page,
	.dma_supported		= dma_direct_supported,
};
EXPORT_SYMBOL(riscv_uncached_dma_ops);
