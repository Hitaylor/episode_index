#ifndef _LINUX_EPISODE_FS_H
#define _LINUX_EPISODE_FS_H

#include <linux/types.h>
#include <linux/magic.h>

/*
 * The episode filesystem constants/structures
 */

#define EPISODE_ROOT_INO 1

/* Not the same as the bogus LINK_MAX in <linux/limits.h>. Oh well. */
#define EPISODE_LINK_MAX	65530

#define EPISODE_I_MAP_SLOTS	8
#define EPISODE_Z_MAP_SLOTS	64
#define EPISODE_VALID_FS		0x0001		/* Clean fs. */
#define EPISODE_ERROR_FS		0x0002		/* fs has errors. */

#define EPISODE_INODES_PER_BLOCK ((BLOCK_SIZE)/(sizeof (struct episode_inode)))

#define EPISODE_SUPER_MAGIC 0xeeee

/*
 * The episode inode has all the time entries, as well as
 * long block numbers and a third indirect block (7+1+1+1
 * instead of 7+1+1). Also, some previously 8-bit values are
 * 16-bit. The inode is 64 bytes.
 */
struct episode_inode {
	__u16 i_mode;
	__u16 i_nlinks;
	__u16 i_uid;
	__u16 i_gid;
	__u32 i_size;
	__u32 i_atime;
	__u32 i_mtime;
	__u32 i_ctime;
	__u32 i_indexcount;//jin 和i_index[]配合，用于记录总共有多少条索引记录
	__u32 i_zone[10];
	__u32 i_index[15];//jin 类似三级索引结构，记录所有记录的索引，每个索引16字节<key,dataZoneAddr,innerOffset,recordLength>,将来改成<key,zoneAddr,offset>
	//在原始数据中，recordLenth	还是要放在数据之前，以应对顺序读取
};

/*
 * episode super-block data on disk
 */
struct episode_super_block {
	__u32 s_ninodes; 		/* inode数量*/ 
	__u16 s_pad0;   		/* 填充值0*/
	__u16 s_imap_blocks; 	/* imap所占blocks数量*/
	__u16 s_zmap_blocks;	/* zmap所占blocks数量*/
	__u16 s_firstdatazone;	/* 第一个数据块data的位置-第几个block是数据区的开始*/
	__u16 s_log_zone_size;	/* block的大小除以1024后得到的数值，然后求以2为底的log值*/
	__u16 s_pad1;			/* 填充值1*/
	__u32 s_max_size;		/* 单文件最大存储量*/
	__u32 s_zones;			/* 数据区块数*/
	__u16 s_magic;			/* 魔术值 位于第24字节开始*/
	__u16 s_pad2;			/* 填充值2*/
	__u16 s_blocksize;		/* 单个block的大小（字节表示）*/
	__u8  s_disk_version;	/* 版本信息标记*/
};

struct episode_dir_entry {
	__u32 inode;
	char name[0];
};
#endif
