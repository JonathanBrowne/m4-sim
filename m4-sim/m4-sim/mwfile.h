//
//  mwfile.h
//  audio-packer
//
//  Created by Jonathan O'Neill Browne on 11/2/17.
//  Copyright © 2017 Side Dish Productions. All rights reserved.
//

#ifndef mwfile_h
#define mwfile_h

#include <stdio.h>
//also a place to dump other M4 specific utility function

void convert_u8_to_u16(unsigned char *pos, uint16_t *out_value);
void convert_u8_to_u32(unsigned char *pos, uint32_t *out_value);

void * gp_malloc_align(uint32_t size,int alignment);
void  gp_free(void *buffer);

void mwfile_save(const char *fname, unsigned char *data, int data_size);
uint32_t mwfile_size(const char *filename);
uint32_t mwfile_load(const char *filename, unsigned char *buf,uint32_t size);

#define DBG_PRINT printf

#endif /* mwfile_h */
