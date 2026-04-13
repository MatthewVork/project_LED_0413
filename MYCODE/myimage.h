#ifndef __MYIMAGE_H__
#define __MYIMAGE_H__

extern const unsigned char g_image_pic_01_320x240[153600];

// 图像信息结构体
typedef struct {
    const char* name;              // 图像名字，方便索引指定图像数据
    const unsigned char* address;  // 图像数组入口地址
    unsigned int width;            // 图像宽度
    unsigned int height;           // 图像高度
    unsigned int size;             // 图像数组大小
} image_info_t;

static const image_info_t g_image_tbl[1] = {
    {"pic", g_image_pic_01_320x240, 320, 240, 153600 },
};

#endif //__MYIMAGE_H__
