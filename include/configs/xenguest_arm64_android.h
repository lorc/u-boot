/*
 * SPDX-License-Identifier: GPL-2.0+
 *
 * (C) Copyright 2020 EPAM Systemc Inc.
 */
#ifndef __XENGUEST_ARM64_ANDROID_H
#define __XENGUEST_ARM64_ANDROID_H

#include <configs/xenguest_arm64.h>

#define CONFIG_SYS_BOOTM_LEN	      (20 * 1024 * 1024)

#undef CONFIG_EXTRA_ENV_SETTINGS

/* NOTE: In case of bootm * boot , u-boot  will set/append
 * env variable bootargs with boot_img_hdr->cmdline and further overwrite
 * /chosen node of the fdt. Since /chosen node is the main mechanism to pass cmdline
 * from Xen domain config to bootloader and Linux kernel, we will prior to all that 
 * create bootargs variable with /chosen node(using command "fdt get value bootargs /chosen bootargs").
 * So in at the end bootargs will contain /cosen node + boot_img_hdr->cmdline. */

#define CONFIG_EXTRA_ENV_SETTINGS	\
	"bootcmd=run pvblockboot;\0" \
	"fdt_addr=0x48000000\0" \
	"boot_image_addr=0x90000000\0" \
	"boot_image_part_blk=0x008dc800\0" \
	"boot_image_length_blk=0xa000\0" \
	"loadimage=pvblock read ${boot_image_addr} ${boot_image_part_blk} ${boot_image_length_blk};\0" \
	"pvblockboot=run loadimage; fdt addr ${fdt_addr}; fdt get value bootargs /chosen bootargs;" \
		"bootm ${boot_image_addr} - ${fdt_addr};\0"

#endif /* __XENGUEST_ARM64_H */
