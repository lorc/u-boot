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
	"bootcmd=run pvblockboot;\0" \
	"loadimage=ext4load pvblock 0:7 0x90000000 /Image;\0" \
	"pvblockboot=run loadimage;" \
		"booti 0x90000000 - 0x48000000;\0"

#endif /* __XENGUEST_ARM64_H */
