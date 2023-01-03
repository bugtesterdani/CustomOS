#pragma once

#include "stdint.h"

#define	VGA_AC_INDEX		    0x3C0
#define	VGA_AC_WRITE		    0x3C0
#define	VGA_AC_READ		        0x3C1
#define	VGA_MISC_WRITE		    0x3C2
#define VGA_SEQ_INDEX		    0x3C4
#define VGA_SEQ_DATA		    0x3C5
#define	VGA_DAC_READ_INDEX	    0x3C7
#define	VGA_DAC_WRITE_INDEX	    0x3C8
#define	VGA_DAC_DATA		    0x3C9
#define	VGA_MISC_READ		    0x3CC
#define VGA_GC_INDEX 		    0x3CE
#define VGA_GC_DATA 		    0x3CF
#define VGA_CRTC_INDEX_C	    0x3D4   // C Stands for Colour
#define VGA_CRTC_INDEx_M        0x3B4   // M Stands for Mono
#define VGA_CRTC_DATA_C		    0x3D5
#define VGA_CRTC_DATA_M         0x3B5
#define	VGA_INSTAT_READ		    0x3DA

#define	VGA_NUM_SEQ_REGS	    5
#define	VGA_NUM_CRTC_REGS	    25
#define	VGA_NUM_GC_REGS		    9
#define	VGA_NUM_AC_REGS		    21
#define	VGA_NUM_REGS		    (1 + VGA_NUM_SEQ_REGS + VGA_NUM_CRTC_REGS + \
				                VGA_NUM_GC_REGS + VGA_NUM_AC_REGS)

void get_offset_address();
void write_regs(uint8_t *regs);
void setupmode(uint16_t x, uint16_t y, uint16_t depth);
void putpixel(uint16_t x, uint16_t y, uint8_t color);
uint8_t supported_size(uint16_t x, uint16_t y, uint16_t depth);
void DrawLineHorizontal(uint16_t x_start, uint16_t x_size, uint16_t y_start, uint8_t line_thickness, uint8_t color);
void DrawLineVertical(uint16_t y_start, uint16_t y_size, uint16_t x_start, uint8_t line_thickness, uint8_t color);
void DrawRectAngle(uint16_t x_start, uint16_t y_start, uint16_t x_size, uint16_t y_size, uint8_t line_thickness_pixel, uint8_t color);
void FillRectangle(uint16_t x_start, uint16_t y_start, uint16_t x_size, uint16_t y_size, uint8_t color);
