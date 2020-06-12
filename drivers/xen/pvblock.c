/*
 * SPDX-License-Identifier:	GPL-2.0+
 *
 * (C) Copyright 2020 EPAM Systems Inc.
 */
#include <common.h>
#include <dm.h>
#include <part.h>
#include <pvblock.h>

#define DRV_NAME	"pvblock"

struct blkfront_dev {
	char dummy;
};

static int init_blkfront(unsigned int devid, struct blkfront_dev *dev)
{
	return 0;
}

static void shutdown_blkfront(struct blkfront_dev *dev)
{
}

void pvblock_init(void)
{
}

ulong pvblock_read(struct udevice *udev, lbaint_t blknr, lbaint_t blkcnt,
		   void *buffer)
{
	return 0;
}

ulong pvblock_write(struct udevice *udev, lbaint_t blknr, lbaint_t blkcnt,
		    const void *buffer)
{
	return 0;
}

static int pvblock_probe(struct udevice *udev)
{
	struct blkfront_dev *blk_dev = dev_get_priv(udev);
	int ret;

	ret = init_blkfront(0, blk_dev);
	if (ret < 0)
		return ret;
	return 0;
}

static int pvblock_remove(struct udevice *udev)
{
	struct blkfront_dev *blk_dev = dev_get_priv(udev);

	shutdown_blkfront(blk_dev);
	return 0;
}

static const struct blk_ops pvblock_ops = {
	.read	= pvblock_read,
	.write	= pvblock_write,
};

U_BOOT_DRIVER(pvblock) = {
	.name			= DRV_NAME,
	.id			= UCLASS_BLK,
	.ops			= &pvblock_ops,
	.probe			= pvblock_probe,
	.remove			= pvblock_remove,
	.priv_auto_alloc_size	= sizeof(struct blkfront_dev),
	.flags			= DM_FLAG_OS_PREPARE,
};

