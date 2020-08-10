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
	"bootcmd=bcb load 0 3 pvblock; if bcb test command = boot-recovery; then run boot_pvblock_recovery; else run boot_pvblock; fi;\0" \
	"recovery_bootargs=androidboot.boot_devices=51712 root=/dev/ram0 ip=dhcp androidboot.hardware=xenvm init=/init console=hvc0 ro rootwait cma=256M@1-2G androidboot.selinux=permissive pvrsrvkm.DriverMode=1 androidboot.android_dt_dir=/proc/device-tree/firmware#1/android/ xt_page_pool=2097152 xt_cma=4194304;\0" \
	"boot_pvblock_recovery=setenv bootargs $recovery_bootargs; pvblock read ${boot_image_addr}  0x008aa000 0x31fff; run update_bootargs; bootm ${boot_image_addr} ${boot_image_addr} ${fdt_addr};\0" \
	"boot_pvblock=run loadimage; fdt addr ${fdt_addr}; fdt get value bootargs /chosen bootargs; run update_bootargs; bootm ${boot_image_addr} - ${fdt_addr};\0"\
	"fdt_addr=0x48000000\0" \
	"boot_image_addr=0x90000000\0" \
	"update_bootargs=ab_select slot pvblock 0#misc; set bootargs ${bootargs}\ androidboot.slot_suffix=_${slot};\0" \
	"boot_image_part_blk=0x008dc800\0" \
	"boot_image_length_blk=0xa000\0" \
	"loadimage=pvblock read ${boot_image_addr} ${boot_image_part_blk} ${boot_image_length_blk};\0" \
	"pvblockboot=run loadimage; fdt addr ${fdt_addr}; fdt get value bootargs /chosen bootargs;" \
		"bootm ${boot_image_addr} - ${fdt_addr};\0"

#endif /* __XENGUEST_ARM64_H */
