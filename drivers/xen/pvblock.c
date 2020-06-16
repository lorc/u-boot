/*
 * SPDX-License-Identifier:	GPL-2.0+
 *
 * (C) Copyright 2020 EPAM Systems Inc.
 */
#include <common.h>
#include <dm.h>
#include <dm/device-internal.h>
#include <malloc.h>
#include <part.h>
#include <pvblock.h>

#include <xen/xenbus.h>

#define DRV_NAME	"pvblock"

struct blkfront_dev {
	char dummy;
};

struct blkfront_platdata {
	unsigned int devid;
};

static int init_blkfront(unsigned int devid, struct blkfront_dev *dev)
{
	return 0;
}

static void shutdown_blkfront(struct blkfront_dev *dev)
{
}

typedef int (*enum_vbd_callback)(unsigned int devid);

static int on_new_vbd(unsigned int devid)
{
	struct driver_info info;
	struct udevice *udev;
	struct blkfront_platdata *platdata;
	int ret;

	printf("New " DRV_NAME ", device ID %d\n", devid);

	platdata = malloc(sizeof(struct blkfront_platdata));
	if (!platdata) {
		printf("Failed to allocate platform data\n");
		return -ENOMEM;
	}

	platdata->devid = devid;

	info.name = DRV_NAME;
	info.platdata = platdata;

	ret = device_bind_by_name(gd->dm_root, false, &info, &udev);
	if (ret < 0) {
		printf("Failed to bind " DRV_NAME " to device with ID %d, ret: %d\n",
		       devid, ret);
		free(platdata);
	}
	return ret;
}

static int xenbus_enumerate_vbd(enum_vbd_callback clb)
{
	char **dirs, *msg;
	int i, ret;

	msg = xenbus_ls(XBT_NIL, "device/vbd", &dirs);
	if (msg) {
		printf("Failed to read device/vbd directory: %s\n", msg);
		free(msg);
		return -ENODEV;
	}

	for (i = 0; dirs[i]; i++) {
		int devid;

		sscanf(dirs[i], "%d", &devid);
		ret = clb(devid);
		if (ret < 0)
			goto fail;

		free(dirs[i]);
	}
	ret = 0;

fail:
	for (; dirs[i]; i++)
		free(dirs[i]);
	free(dirs);
	return ret;
}

void pvblock_init(void)
{
	struct udevice *bus;
	int ret;

	if (xenbus_enumerate_vbd(on_new_vbd) < 0)
		return;

	/* Enumerate all known block devices: we only expect pvblock */
	ret = uclass_first_device(UCLASS_BLK, &bus);
	if (ret) {
		printf("UCLASS_BLK class has no registered entities\n");
		return;
	}

	while (bus) {
		ret = uclass_next_device(&bus);
		if (ret) {
			printf("Failed to get " DRV_NAME " device, ret %d\n",
			       ret);
			break;
		}
	}
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

static int pvblock_bind(struct udevice *udev)
{
	struct blk_desc *desc = dev_get_uclass_platdata(udev);
	int devnum;

	desc->if_type = IF_TYPE_PVBLOCK;
	/*
	 * Initialize the devnum to -ENODEV. This is to make sure that
	 * blk_next_free_devnum() works as expected, since the default
	 * value 0 is a valid devnum.
	 */
	desc->devnum = -ENODEV;
	devnum = blk_next_free_devnum(IF_TYPE_PVBLOCK);
	if (devnum < 0)
		return devnum;
	desc->devnum = devnum;
	desc->part_type = PART_TYPE_UNKNOWN;
	desc->bdev = udev;

	strncpy(desc->vendor, "Xen", sizeof(desc->vendor));
	strncpy(desc->revision, "1", sizeof(desc->revision));
	strncpy(desc->product, "Virtual disk", sizeof(desc->product));

	return 0;
}

static int pvblock_probe(struct udevice *udev)
{
	struct blkfront_dev *blk_dev = dev_get_priv(udev);
	struct blkfront_platdata *platdata = dev_get_platdata(udev);
	int ret, devid;

	devid = platdata->devid;
	free(platdata);

	ret = init_blkfront(devid, blk_dev);
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
	.bind			= pvblock_bind,
	.probe			= pvblock_probe,
	.remove			= pvblock_remove,
	.priv_auto_alloc_size	= sizeof(struct blkfront_dev),
	.flags			= DM_FLAG_OS_PREPARE,
};

