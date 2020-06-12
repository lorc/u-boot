/*
 * SPDX-License-Identifier:	GPL-2.0+
 *
 * (C) 2020 EPAM Systems Inc.
 */

#ifndef _PVBLOCK_H
#define _PVBLOCK_H

#include <blk.h>

void pvblock_init(void);
ulong pvblock_write(struct udevice *dev, lbaint_t blknr, lbaint_t blkcnt,
		    const void *buffer);
ulong pvblock_write(struct udevice *dev, lbaint_t blknr, lbaint_t blkcnt,
		    const void *buffer);

#endif /* _PVBLOCK_H */
