/*
 * SPDX-License-Identifier: GPL-2.0+
 *
 * (C) Copyright 2020 EPAM Systemc Inc.
 */
#ifndef __XENGUEST_ARM64_ANDROID_H
#define __XENGUEST_ARM64_ANDROID_H

#include <configs/xenguest_arm64.h>

#undef CONFIG_EXTRA_ENV_SETTINGS
#define CONFIG_EXTRA_ENV_SETTINGS	\
	"loadimage=ext4load pvblock 0 0x90000000 /boot/Image;\0" \
	"pvblockboot=run loadimage;" \
		"booti 0x90000000 - 0x88000000;\0"

#endif /* __XENGUEST_ARM64_H */
