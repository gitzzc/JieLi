#ifndef _FF_API
#define _FF_API

#include "tff.h"

#ifdef __SMART_CPU__
enum
{
    SEARCH_FILE_BY_NUMBER,  //按文件序号搜索文件
    SEARCH_FILE_BY_PATH,    //按文件路径全设备搜索文件，并找出文件序号
    SEARCH_SPEC_FILE,       //搜索指定的文件
    SEARCH_CUR_DIR_BY_PATH, //在当前目录下，按文件路径搜索下一个文件名匹配的文件，并找出文件序号
    SEARCH_DISK,            //搜索总有效文件数
    SEARCH_FOLDER           //搜索当前目录根下的有效文件数。
};

/* function list */
bool fs_scan_disk(u32 sclust, u32 fsize);
u16 fs_getfile_totalindir(DIR _xdata * dj);
bool fs_getfile_bynumber(FIL *file_ptr, u16 fileNumber);
void fs_ext_setting(u8 const _code * str);
void fs_api_init(void);

#endif

#endif
