#ifndef _FATFS
#define _FATFS

#include "config.h"

#ifdef __SMART_CPU__
typedef u32	 CLUST;
/* Directory object structure */
typedef struct _DIR
{
    u32 	clust;		        /* Current cluster */
    u32		csect;		        /* Current sector */
    u32		sclust;		        /* Start cluster */
    u16		cindex;		        /* Current index */
} DIR;


/* File system object structure */
typedef struct _FATFS
{
    u8 		_xdata * win;		        /* Disk access window for Directory/FAT/File */
    u32		winsect;		/* Current sector appearing in the win[] */
    u32		fatbase;		/* FAT start sector */
    u32		dirbase;		/* Root directory start sector */
    u32		database;		/* Data start sector */
    u32 	max_clust;		/* Maximum cluster# + 1 */
    //u32     Reserved_sector;
#ifdef DISK_USED_SPACE
    u32     used_space;     /* Disk used space */
#endif
    u16		n_rootdir;		/* Number of root directory entries */
    u8		s_size;			/* sector size, 2 power n*/
    u8		csize;			/* Number of sectors per cluster,2 power n */
    u8		fs_type;		/* FAT sub type */
    u8		secotr_512size;	/* size of per sector */
    u8		(*disk_read)(u8 _xdata * buf, u32 lba);		/* device read function */
} FATFS;

//typedef u8 (*disk_read)(u8 _xdata * buf, u32 lba);

/* DIR status structure */
typedef struct _DIRINFO
{
    DIR     dj;             /* 父目录信息 */
    u32 	sclust;			/* File start cluster */
    u32		fsize;			/* Size */
    u8		fattrib;		/* Attribute */
    u8      fname[15];      /* 为了使exFAT更好计算,长度取15Bytes*/
} DIRINFO;

/* File system API info */
typedef struct _FSAPIMSG
{
    u16     fileCounter;
    u16	    fileNumber;			// 当前文件序号
    u16	    musicDirCounter;    // 包含指定文件的文件夹序号
    u16     dirCounter;			// 文件夹序号
    u16     fileTotalInDir;     // 当前目录的根下有效文件的个数
    u16     fileTotalOutDir;	// 当前目录前的文件总数，目录循环模式下，需要用它来计算文件序号
    u8	    deepth;				// dir deepth for search
    u16     sclust_id;
    u16     fsize_id;
} FSAPIMSG;

/* File object structure */
typedef struct _FIL
{
    u32		fptr;				// File R/W pointer
    u32		fsize;				// File size
    u8      flag;
//解码过程中会使用到前面的三个变量
    u32		curr_sect;			// Current sector
    u32 	curr_clust;			// Current cluster
    u32     start_clust;		// 连续簇开始
    u32     end_clust;          // 连续簇结束
    u32 	org_clust;			// File start cluster
} FIL;


enum
{
    SEEK_SET = 0x01,
    SEEK_CUR = 0x02
};


typedef struct _FF_APIS
{
    FSAPIMSG fs_msg;
    DIRINFO  dir_info;
    DIR path_dj[10];				/* 用于搜索文件时，作为搜索路径的缓存 */
}FF_APIS;


/* File function return code (FRESULT) */

typedef enum
{
    FR_OK = 0,
    FR_FIND_DIR = 0x80,
    FR_FIND_FILE,
    FR_DIR_END,
    FR_NO_FILE,
    FR_NO_PATH,
    FR_INVALID_NAME,
    FR_INVALID_DRIVE,
    FR_DENIED,
    FR_RW_ERROR,
    FR_WRITE_PROTECTED,
    FR_NO_FILESYSTEM,
    FR_DEEP_LIMIT,
    FR_END_PATH,
    FR_FILE_LIMIT,
    FR_END_FILE,
    FR_LFN_ERR,
    FR_MKFS_ABORTED,
    FR_DIR_DELETE,
    FR_DISK_ERROR,
    FR_FILE_END,
    FR_USER_END,
} FRESULT;

extern _no_init _no_init FATFS _pdata fat_fs;
extern _no_init FIL music_file;



/*******************************************/
/*
            Maskrom API
                --File System Part
*/
/*******************************************/
_near_func FRESULT f_seek (FIL _xdata *fp, u8 type, u32 offsize);
_near_func u16 f_read(FIL _xdata *fp ,u8 _xdata *buff , u16 btr);
_near_func FRESULT  f_readnextdir (DIR _xdata * dj, DIRINFO _xdata *dinfo);
_near_func FRESULT f_readprevdir (DIR _xdata* dj, DIRINFO _xdata* dinfo);
_near_func void  f_opendir(DIR _xdata* dj) ;
_near_func FRESULT  f_mount(u32 sect) ;
_near_func void f_parse_fat_lfn(DIRINFO _xdata* dinfo);
_near_func FRESULT  f_open(FIL _xdata *ptr, DIRINFO _xdata * finfo);
_near_func u8 check_fs (u32 sec);
_near_func u32 f_tell_fptr(FIL _xdata *fp);					//获得文件读指针
_near_func u32 f_tell_fsize(FIL _xdata *fp);					//获得文件的大小
_near_func void f_set_sector_size(u8);
_near_func u8 f_tell_status(FIL _xdata *fp);

_near_func void f_init(u8 (_near_func *disk_read)(u8 _xdata * buf, u32 lba), u8 sector_sizeof512);
_near_func bool fs_scan_disk(FF_APIS *f_api);
//bool fs_scan_disk_app(FF_APIS *f_api);
//_near_func u16 fs_get_filenumber(ENUM_PLAY_MODE playmode, u8 searchMode);
_near_func u16 fs_getfile_totalindir(FF_APIS *f_api);
//u16 fs_getfile_totalindir_app(FF_APIS *f_api);
_near_func bool fs_getfile_bynumber(FF_APIS *f_api, u16 fileNumber);
//bool fs_getfile_bynumber_app(FF_APIS *f_api, u16 fileNumber);
_near_func void fs_ext_setting(u8 const _code * str);
_near_func void fast_scan_files (DIR _xdata * dj);
//void fast_scan_files_app (DIR _xdata * dj);
_near_func bool fs_getfile_bypath(FF_APIS *f_api, u8 _xdata* fpath, u8 mode);
//u16 fs_getfile_totalindir_app(FF_APIS *f_api);

#endif

/* FAT sub type */
#define FS_FAT12	1
#define FS_FAT16	2
#define FS_FAT32	3
#define FS_EXFAT	4


#endif /* _FATFS */
