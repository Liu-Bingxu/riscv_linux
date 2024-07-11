/*
 * Sample disk driver, from the beginning.
 */

// #include <linux/config.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/sched.h>
#include <linux/kernel.h> /* printk() */
#include <linux/slab.h> /* kmalloc() */
#include <linux/fs.h> /* everything... */
#include <linux/errno.h> /* error codes */
#include <linux/timer.h>
#include <linux/types.h> /* size_t */
#include <linux/fcntl.h> /* O_ACCMODE */
#include <linux/hdreg.h> /* HDIO_GETGEO */
#include <linux/kdev_t.h>
#include <linux/vmalloc.h>
// #include <linux/genhd.h>
#include <linux/blkdev.h>
#include <linux/buffer_head.h> /* invalidate_bdev */
#include <linux/bio.h>

// typedef struct request_queue request_queue_t;

MODULE_LICENSE("Dual BSD/GPL");

static DEFINE_IDR(sztulbx_blk_index_idr);

static int sztulbx_blk_major = 0;
module_param(sztulbx_blk_major, int, 0);
static int sztulbx_blk_hardsect_size = 512;
module_param(sztulbx_blk_hardsect_size, int, 0);
static int sztulbx_blk_nsectors = 2 * 1024 * 1024; /* How big the drive is */
module_param(sztulbx_blk_nsectors, int, 0);
static int sztulbx_blk_ndevices = 1;
module_param(sztulbx_blk_ndevices, int, 0);

/*
 * Minor number and partition management.
 */
#define sztulbx_blk_MINORS 2

/*
 * We can tweak our hardware sector size, but the kernel talks to us
 * in terms of small sectors, always.
 */
#define KERNEL_SECTOR_SIZE 512

/*
 * The internal representation of our device.
 */
struct sztulbx_blk_dev {
	int size; /* Device size in sectors */
	spinlock_t lock; /* For mutual exclusion */
	struct gendisk *gd; /* The gendisk structure */
    uint64_t __iomem *sztulbx_blk_mmio;
    // char __iomem *debug;
    int device_id;
};

static struct sztulbx_blk_dev *Devices = NULL;

/*
 * Handle an I/O request.
 */
static void sztulbx_blk_transfer(struct sztulbx_blk_dev *dev,
				 unsigned long sector, unsigned long nsect,
				 char *buffer, int write)
{
	unsigned long offset = sector * KERNEL_SECTOR_SIZE;
	unsigned long nbytes = nsect * KERNEL_SECTOR_SIZE;

	if ((offset + nbytes) > dev->size) {
		printk(KERN_NOTICE "Beyond-end write (%ld %ld)\n", offset,
		       nbytes);
		return;
	}
	dev->sztulbx_blk_mmio[0] = (uint64_t)sector;
	dev->sztulbx_blk_mmio[1] = (uint64_t)nsect;
	dev->sztulbx_blk_mmio[2] = (uint64_t)buffer;
	if (write)
		dev->sztulbx_blk_mmio[3] = 1;
	else
		dev->sztulbx_blk_mmio[3] = 0;
}

static void sztulbx_blk_read(struct sztulbx_blk_dev *dev, struct bio *bio)
{
	// unsigned long start_time = bio_start_io_acct(bio);
	// struct bvec_iter iter = bio->bi_iter;

	// do {
	// 	u32 index = iter.bi_sector >> SECTORS_PER_PAGE_SHIFT;
	// 	u32 offset = (iter.bi_sector & (SECTORS_PER_PAGE - 1)) <<
	// 			SECTOR_SHIFT;
	// 	struct bio_vec bv = bio_iter_iovec(bio, iter);

	// 	bv.bv_len = min_t(u32, bv.bv_len, PAGE_SIZE - offset);

	// 	if (zram_bvec_read(zram, &bv, index, offset, bio) < 0) {
	// 		atomic64_inc(&zram->stats.failed_reads);
	// 		bio->bi_status = BLK_STS_IOERR;
	// 		break;
	// 	}

	// 	bio_advance_iter_single(bio, &iter, bv.bv_len);
	// } while (iter.bi_size);

	// bio_end_io_acct(bio, start_time);
	// bio_endio(bio);

	struct bvec_iter iter = bio->bi_iter;
	struct bio_vec bvec;
	sector_t sector = bio->bi_iter.bi_sector;

	bio_for_each_segment(bvec, bio, iter) {
		// phys_addr_t paddr = PFN_PHYS(page_to_pfn(bvec.bv_page)) + bvec.bv_offset;
		void *vaddr = page_address(bvec.bv_page) + bvec.bv_offset;
		char *buffer = (char *)vaddr;
		sztulbx_blk_transfer(dev, sector, (bvec.bv_len >> 9), buffer, 0);
		sector += (bvec.bv_len >> 9);
	}
	bio_endio(bio);
}

static void sztulbx_blk_write(struct sztulbx_blk_dev *dev, struct bio *bio)
{
	struct bvec_iter iter = bio->bi_iter;
	struct bio_vec bvec;
	sector_t sector = bio->bi_iter.bi_sector;

	// do {
	// 	u32 index = iter.bi_sector >> SECTORS_PER_PAGE_SHIFT;
	// 	u32 offset = (iter.bi_sector & (SECTORS_PER_PAGE - 1)) <<
	// 			SECTOR_SHIFT;

	// 	bv.bv_len = min_t(u32, bv.bv_len, PAGE_SIZE - offset);

	// 	if (zram_bvec_write(zram, &bv, index, offset, bio) < 0) {
	// 		panic("%s-%d: error when write", __func__, __LINE__);
	// 	}
    //     sztulbx_blk_transfer(struct sztulbx_blk_dev *dev,
	// 			 unsigned long sector, unsigned long nsect,
	// 			 char *buffer, int write);

	// 	bio_advance_iter_single(bio, &iter, bv.bv_len);
	// } while (iter.bi_size);
    bio_for_each_segment(bvec, bio, iter){
		// phys_addr_t paddr = PFN_PHYS(page_to_pfn(bvec.bv_page)) + bvec.bv_offset;
		void *vaddr = page_address(bvec.bv_page) + bvec.bv_offset;
		char *buffer = (char *)vaddr;
        sztulbx_blk_transfer(dev, sector, (bvec.bv_len >> 9), buffer, 1);
        sector += (bvec.bv_len >> 9);
    }

	bio_endio(bio);
}

/*
 * Handler function for all zram I/O requests.
 */
static void sztulbx_blk_submit_bio(struct bio *bio)
{
    struct sztulbx_blk_dev *dev = bio->bi_bdev->bd_disk->private_data;

	switch (bio_op(bio)) {
	case REQ_OP_READ:
		sztulbx_blk_read(dev, bio);
		break;
	case REQ_OP_WRITE:
		sztulbx_blk_write(dev, bio);
		break;
	default:
		WARN_ON_ONCE(1);
		bio_endio(bio);
	}
}

/*
 * Open and close.
 */

static int sztulbx_blk_open(struct gendisk *gd, fmode_t mode)
{
	// struct sztulbx_blk_dev *dev = gd->private_data;
	return 0;
}

static void sztulbx_blk_release(struct gendisk *gd){
	// struct sztulbx_blk_dev *dev = gd->private_data;
}


/*
 * The device operations structure.
 */
static struct block_device_operations sztulbx_blk_ops = {
	.owner = THIS_MODULE,
	.open = sztulbx_blk_open,
	.release = sztulbx_blk_release,
	.submit_bio = sztulbx_blk_submit_bio,
};

/*
 * Set up our internal device.
 */
static void setup_device(struct sztulbx_blk_dev *dev, int which)
{
	// static char debug_info[1024];
	/*
	 * Get some memory.
	 */
	memset(dev, 0, sizeof(struct sztulbx_blk_dev));
	dev->size = sztulbx_blk_nsectors * sztulbx_blk_hardsect_size;
	spin_lock_init(&dev->lock);

    struct queue_limits lim = {
		.logical_block_size		    = KERNEL_SECTOR_SIZE,
		.physical_block_size		= KERNEL_SECTOR_SIZE,
		.io_min				        = KERNEL_SECTOR_SIZE,
		.io_opt				        = KERNEL_SECTOR_SIZE,
		.max_hw_discard_sectors		= UINT_MAX,
	};

	dev->sztulbx_blk_mmio = ioremap(0x10100000, 0x100);
	// dev->debug = ioremap(0x10000000, 0x100);
	// asm volatile("ebreak" :::);
	if (!dev->sztulbx_blk_mmio) {
		// sprintf(debug_info, "error, unable to ioremap");
		// for (int i = 0; debug_info[i];i++){
		// 	*(dev->debug) = debug_info[i];
        // }
		panic("error, unable to ioremap");
    }

    int ret = idr_alloc(&sztulbx_blk_index_idr, dev, 0, 0, GFP_KERNEL);
    // asm volatile("ebreak" :::);
    if (ret < 0) {
	    // sprintf(debug_info, "error, unable to ioremap");
	    // for (int i = 0; debug_info[i]; i++) {
		//     *(dev->debug) = debug_info[i];
	    // }
	    panic("error, unable to alloc id");
    }
	dev->device_id = ret;

	dev->gd = blk_alloc_disk(&lim, NUMA_NO_NODE);
	// asm volatile("ebreak" :::);
	if (IS_ERR(dev->gd)) {
		// sprintf(debug_info, "error, unable to ioremap");
		// for (int i = 0; debug_info[i]; i++) {
		// 	*(dev->debug) = debug_info[i];
		// }
		pr_err("Error allocating disk structure for device %d\n",
		       dev->device_id);
		panic("error, unable to alloc disk");
	}

    dev->gd->major = sztulbx_blk_major;
	dev->gd->first_minor = 0;
	dev->gd->minors = 2;
	dev->gd->fops = &sztulbx_blk_ops;
	dev->gd->private_data = dev;
	snprintf(dev->gd->disk_name, 16, "sztulbxd%c", dev->device_id + 'a');
	set_capacity(dev->gd, (dev->size >> 9));

	ret = add_disk(dev->gd);
	// asm volatile("ebreak" :::);
	if (ret){
		// sprintf(debug_info, "error, unable to ioremap");
		// for (int i = 0; debug_info[i]; i++) {
		// 	*(dev->debug) = debug_info[i];
		// }
		panic("error, unable to add disk");
    }
    // iounmap(dev->debug);
    // asm volatile("ebreak" :::);

    return;
}

static int __init sztulbx_blk_init(void)
{
	int i;
	/*
	 * Get registered.
	 */
	printk("\n\n\n\n");
	printk("sztulbx_blk: %s-%d:now start init sztulbx_blk\n", __func__, __LINE__);
	sztulbx_blk_major = register_blkdev(sztulbx_blk_major, "sztulbx_blk");
	if (sztulbx_blk_major <= 0) {
		printk(KERN_WARNING "sztulbx_blk: unable to get major number\n");
		return -EBUSY;
	}
	printk("sztulbx_blk: %s-%d:now register_blkdev success\n", __func__,
	       __LINE__);
	/*
	 * Allocate the device array, and initialize each one.
	 */
	printk("sztulbx_blk: %s-%d:now start Allocate the device array, and initialize each one.\n", __func__, __LINE__);
	Devices = kmalloc(sztulbx_blk_ndevices * sizeof(struct sztulbx_blk_dev), GFP_KERNEL);
	if (Devices == NULL)
		goto out_unregister;
	for (i = 0; i < sztulbx_blk_ndevices; i++)
		setup_device(Devices + i, i);
	printk("sztulbx_blk: %s-%d:now finish Allocate the device array, and initialize each one.\n",__func__, __LINE__);
	printk("\n\n\n\n");

	return 0;

out_unregister:
	unregister_blkdev(sztulbx_blk_major, "sztulbx_blk");
	return -ENOMEM;
}

static void __exit sztulbx_blk_exit(void)
{
	int i;

	for (i = 0; i < sztulbx_blk_ndevices; i++) {
		struct sztulbx_blk_dev *dev = Devices + i;

		if (dev->gd) {
			del_gendisk(dev->gd);
			put_disk(dev->gd);
		}
	}
	unregister_blkdev(sztulbx_blk_major, "sztulbx_blk");
	kfree(Devices);
}

module_init(sztulbx_blk_init);
module_exit(sztulbx_blk_exit);
