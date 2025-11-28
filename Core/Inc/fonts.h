#ifndef __FONTS_H__
#define __FONTS_H__

#include <stdint.h>

typedef struct {
    const uint8_t width;
    uint8_t height;
    const uint16_t *data;
} FontDef;


extern FontDef const Font_6x8;
extern FontDef const Font_7x10;
extern FontDef const Font_10x14;
extern FontDef const Font_11x18;
extern FontDef const Font_16x26;

#endif // __FONTS_H__
