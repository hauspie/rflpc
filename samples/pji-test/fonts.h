/*
  Author: Christophe Bacara <christophe.bacra@etudiant.univ-lille1.fr>
  Created: <2013-12-16 00:00:00 (bacara)>
*/

#ifndef _FONTS_H_
#define _FONTS_H_

#define FONT_GET_WIDTH(font)     font[0][0]
#define FONT_GET_HEIGHT(font)    font[0][1]
#define FONT_GET_COUNT(font)     font[0][2]
#define FONT_GET_LENGTH(font)    font[0][3]

#define FONT_IS_VALID_INDEX(font,index)   \
  (index > 0 && index < FONT_GET_COUNT (font))

const unsigned char default_font_8x8[97][8] = {
  { /* Width */ 8,  /* Height */ 8,
    /* Count */ 97, /* Length */ 8,     0, 0, 0, 0 },
  /* Characters' bytes */
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* SPACE */
  { 0x00, 0x06, 0x5F, 0x5F, 0x06, 0x00, 0x00, 0x00 }, /*   !   */
  { 0x00, 0x07, 0x07, 0x00, 0x07, 0x07, 0x00, 0x00 }, /*   "   */
  { 0x14, 0x7F, 0x7F, 0x14, 0x7F, 0x7F, 0x14, 0x00 }, /*   #   */
  { 0x00, 0x24, 0x2E, 0x6B, 0x6B, 0x3A, 0x12, 0x00 }, /*   $   */
  { 0x00, 0x46, 0x66, 0x30, 0x18, 0x0C, 0x66, 0x62 }, /*   %   */
  { 0x00, 0x30, 0x7A, 0x4F, 0x5D, 0x37, 0x7A, 0x48 }, /*   &   */
  { 0x00, 0x04, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00 }, /*   '   */
  { 0x00, 0x00, 0x1C, 0x3E, 0x63, 0x41, 0x00, 0x00 }, /*   (   */
  { 0x00, 0x00, 0x41, 0x63, 0x3E, 0x1C, 0x00, 0x00 }, /*   )   */
  { 0x08, 0x2A, 0x3E, 0x1C, 0x1C, 0x3E, 0x2A, 0x08 }, /*   *   */
  { 0x08, 0x08, 0x3E, 0x3E, 0x08, 0x08, 0x00, 0x00 }, /*   +   */
  { 0x00, 0x00, 0x80, 0xE0, 0x60, 0x00, 0x00, 0x00 }, /*   ,   */
  { 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00 }, /*   -   */
  { 0x00, 0x00, 0x00, 0x60, 0x60, 0x00, 0x00, 0x00 }, /*   .   */
  { 0x00, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x01 }, /*   /   */
  { 0x00, 0x3E, 0x7F, 0x41, 0x49, 0x41, 0x7F, 0x3E }, /*   0   */
  { 0x00, 0x44, 0x42, 0x7F, 0x7F, 0x40, 0x40, 0x00 }, /*   1   */
  { 0x00, 0x62, 0x73, 0x59, 0x49, 0x6F, 0x66, 0x00 }, /*   2   */
  { 0x00, 0x22, 0x63, 0x49, 0x49, 0x7F, 0x36, 0x00 }, /*   3   */
  { 0x00, 0x18, 0x1C, 0x16, 0x53, 0x7F, 0x7F, 0x50 }, /*   4   */
  { 0x00, 0x27, 0x67, 0x45, 0x45, 0x7D, 0x39, 0x00 }, /*   5   */
  { 0x00, 0x3C, 0x7E, 0x4B, 0x49, 0x79, 0x30, 0x00 }, /*   6   */
  { 0x00, 0x03, 0x03, 0x71, 0x79, 0x0F, 0x07, 0x00 }, /*   7   */
  { 0x00, 0x36, 0x7F, 0x49, 0x49, 0x7F, 0x36, 0x00 }, /*   8   */
  { 0x00, 0x06, 0x4F, 0x49, 0x69, 0x3F, 0x1E, 0x00 }, /*   9   */
  { 0x00, 0x00, 0x00, 0x66, 0x66, 0x00, 0x00, 0x00 }, /*   :   */
  { 0x00, 0x00, 0x80, 0xe6, 0x66, 0x00, 0x00, 0x00 }, /*   ;   */
  { 0x00, 0x08, 0x1c, 0x36, 0x63, 0x41, 0x00, 0x00 }, /*   <   */
  { 0x00, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x00 }, /*   =   */
  { 0x00, 0x00, 0x41, 0x63, 0x36, 0x1c, 0x08, 0x00 }, /*   >   */
  { 0x00, 0x02, 0x03, 0x51, 0x59, 0x0F, 0x06, 0x00 }, /*   ?   */
  { 0x00, 0x3e, 0x7f, 0x41, 0x5d, 0x55, 0x57, 0x1e }, /*   @   */
  { 0x00, 0x7c, 0x7e, 0x13, 0x13, 0x7e, 0x7c, 0x00 }, /*   A   */
  { 0x00, 0x41, 0x7f, 0x7f, 0x49, 0x49, 0x7f, 0x36 }, /*   B   */
  { 0x00, 0x1c, 0x3e, 0x63, 0x41, 0x41, 0x63, 0x22 }, /*   C   */
  { 0x00, 0x41, 0x7f, 0x7f, 0x41, 0x63, 0x3e, 0x1c }, /*   D   */
  { 0x00, 0x41, 0x7f, 0x7f, 0x49, 0x5d, 0x41, 0x63 }, /*   E   */
  { 0x00, 0x41, 0x7f, 0x7f, 0x49, 0x1d, 0x01, 0x03 }, /*   F   */
  { 0x00, 0x1c, 0x3e, 0x63, 0x41, 0x51, 0x73, 0x72 }, /*   G   */
  { 0x00, 0x7f, 0x7f, 0x08, 0x08, 0x7f, 0x7f, 0x00 }, /*   H   */
  { 0x00, 0x00, 0x41, 0x7f, 0x7f, 0x41, 0x00, 0x00 }, /*   I   */
  { 0x00, 0x30, 0x70, 0x40, 0x41, 0x7f, 0x3f, 0x01 }, /*   J   */
  { 0x00, 0x41, 0x7f, 0x7f, 0x08, 0x1c, 0x77, 0x63 }, /*   K   */
  { 0x00, 0x41, 0x7f, 0x7f, 0x41, 0x40, 0x60, 0x70 }, /*   L   */
  { 0x00, 0x7f, 0x7f, 0x0E, 0x1c, 0x0E, 0x7f, 0x7f }, /*   M   */
  { 0x00, 0x7f, 0x7f, 0x06, 0x0C, 0x18, 0x7f, 0x7f }, /*   N   */
  { 0x00, 0x3e, 0x7f, 0x41, 0x41, 0x41, 0x7f, 0x3e }, /*   O   */
  { 0x00, 0x41, 0x7f, 0x7f, 0x49, 0x09, 0x0F, 0x06 }, /*   P   */
  { 0x00, 0x1e, 0x3f, 0x21, 0x71, 0x7f, 0x5e, 0x00 }, /*   Q   */
  { 0x00, 0x41, 0x7f, 0x7f, 0x09, 0x19, 0x7f, 0x66 }, /*   R   */
  { 0x00, 0x22, 0x67, 0x4d, 0x59, 0x73, 0x22, 0x00 }, /*   S   */
  { 0x00, 0x03, 0x41, 0x7f, 0x7f, 0x41, 0x03, 0x00 }, /*   T   */
  { 0x00, 0x7f, 0x7f, 0x40, 0x40, 0x7f, 0x7f, 0x00 }, /*   U   */
  { 0x00, 0x1f, 0x3f, 0x60, 0x60, 0x3f, 0x1f, 0x00 }, /*   V   */
  { 0x00, 0x7f, 0x7f, 0x30, 0x18, 0x30, 0x7f, 0x7f }, /*   W   */
  { 0x00, 0x43, 0x67, 0x3c, 0x18, 0x3c, 0x67, 0x43 }, /*   X   */
  { 0x00, 0x07, 0x4f, 0x78, 0x78, 0x4f, 0x07, 0x00 }, /*   Y   */
  { 0x00, 0x47, 0x63, 0x71, 0x59, 0x4d, 0x67, 0x73 }, /*   Z   */
  { 0x00, 0x00, 0x7f, 0x7f, 0x41, 0x41, 0x00, 0x00 }, /*   [   */
  { 0x00, 0x01, 0x03, 0x06, 0x0C, 0x18, 0x30, 0x60 }, /*   \   */
  { 0x00, 0x00, 0x41, 0x41, 0x7f, 0x7f, 0x00, 0x00 }, /*   ]   */
  { 0x00, 0x08, 0x0C, 0x06, 0x03, 0x06, 0x0C, 0x08 }, /*   ^   */
  { 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80 }, /*   _   */
  { 0x00, 0x00, 0x00, 0x03, 0x07, 0x04, 0x00, 0x00 }, /*   `   */
  { 0x00, 0x20, 0x74, 0x54, 0x54, 0x3c, 0x78, 0x40 }, /*   a   */
  { 0x00, 0x41, 0x7f, 0x3f, 0x44, 0x44, 0x7c, 0x38 }, /*   b   */
  { 0x00, 0x38, 0x7c, 0x44, 0x44, 0x6c, 0x28, 0x00 }, /*   c   */
  { 0x00, 0x38, 0x7c, 0x44, 0x45, 0x3f, 0x7f, 0x40 }, /*   d   */
  { 0x00, 0x38, 0x7c, 0x54, 0x54, 0x5c, 0x18, 0x00 }, /*   e   */
  { 0x00, 0x48, 0x7e, 0x7f, 0x49, 0x03, 0x02, 0x00 }, /*   f   */
  { 0x00, 0x98, 0xbc, 0xa4, 0xa4, 0xf8, 0x7c, 0x04 }, /*   g   */
  { 0x00, 0x41, 0x7f, 0x7f, 0x08, 0x04, 0x7c, 0x78 }, /*   h   */
  { 0x00, 0x00, 0x44, 0x7d, 0x7d, 0x40, 0x00, 0x00 }, /*   i   */
  { 0x00, 0x60, 0xe0, 0x80, 0x80, 0xfd, 0x7d, 0x00 }, /*   j   */
  { 0x00, 0x41, 0x7f, 0x7f, 0x10, 0x38, 0x6c, 0x44 }, /*   k   */
  { 0x00, 0x00, 0x41, 0x7f, 0x7f, 0x40, 0x00, 0x00 }, /*   l   */
  { 0x00, 0x7c, 0x7c, 0x18, 0x38, 0x1c, 0x7c, 0x78 }, /*   m   */
  { 0x00, 0x7c, 0x7c, 0x04, 0x04, 0x7c, 0x78, 0x00 }, /*   n   */
  { 0x00, 0x38, 0x7c, 0x44, 0x44, 0x7c, 0x38, 0x00 }, /*   o   */
  { 0x00, 0x84, 0xfc, 0xf8, 0xa4, 0x24, 0x3c, 0x18 }, /*   p   */
  { 0x00, 0x18, 0x3c, 0x24, 0xa4, 0xf8, 0xfc, 0x84 }, /*   q   */
  { 0x00, 0x44, 0x7c, 0x78, 0x4c, 0x04, 0x1c, 0x18 }, /*   r   */
  { 0x00, 0x48, 0x5c, 0x54, 0x54, 0x74, 0x24, 0x00 }, /*   s   */
  { 0x00, 0x00, 0x04, 0x3e, 0x7f, 0x44, 0x24, 0x00 }, /*   t   */
  { 0x00, 0x3c, 0x7c, 0x40, 0x40, 0x3c, 0x7c, 0x40 }, /*   u   */
  { 0x00, 0x1c, 0x3c, 0x60, 0x60, 0x3c, 0x1c, 0x00 }, /*   v   */
  { 0x00, 0x3c, 0x7c, 0x70, 0x38, 0x70, 0x7c, 0x3c }, /*   w   */
  { 0x00, 0x44, 0x6c, 0x38, 0x10, 0x38, 0x6c, 0x44 }, /*   x   */
  { 0x00, 0x9c, 0xbc, 0xa0, 0xa0, 0xfc, 0x7c, 0x00 }, /*   y   */
  { 0x00, 0x4c, 0x64, 0x74, 0x5c, 0x4c, 0x64, 0x00 }, /*   z   */
  { 0x00, 0x08, 0x08, 0x3e, 0x77, 0x41, 0x41, 0x00 }, /*   {   */
  { 0x00, 0x00, 0x00, 0x00, 0x77, 0x77, 0x00, 0x00 }, /*   |   */
  { 0x00, 0x41, 0x41, 0x77, 0x3e, 0x08, 0x08, 0x00 }, /*   }   */
  { 0x00, 0x02, 0x03, 0x01, 0x03, 0x02, 0x03, 0x01 }, /*   ~   */
  { 0x00, 0x00, 0x06, 0x0F, 0x09, 0x0F, 0x06, 0x00 }, /*  DEL  */
};

#endif /* _FONTS_H_ */