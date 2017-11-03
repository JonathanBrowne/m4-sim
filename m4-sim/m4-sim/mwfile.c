//
//  mwfile.c
//  audio-packer
//
//  Created by Jonathan O'Neill Browne on 11/2/17.
//  Copyright © 2017 Side Dish Productions. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "mwfile.h"
void convert_u8_to_u16(unsigned char *pos, uint16_t *out_value)
{
    *out_value = *(uint16_t*)pos;
}

void convert_u8_to_u32(unsigned char *pos, uint32_t *out_value)
{
    *out_value = *(uint32_t*)pos;
}

void * gp_malloc_align(uint32_t size,int alignment)
{
    return malloc(size);
}
void  gp_free(void *buffer)
{
    free(buffer);
}

uint32_t mwfile_size(const char *filename)
{
    struct stat sb;
    stat(filename,&sb);
    return (int)sb.st_size;
}

void mwfile_save(const char *fname, unsigned char *data, int data_size)
{
    FILE *fd = fopen(fname, "wb");
    fwrite(data,data_size,1,fd);
    fclose(fd);
}

uint32_t mwfile_load(const char *filename, unsigned char *buf, uint32_t size)
{
    FILE *fd = fopen(filename, "rb");
    fread(buf,size,1,fd);
    fclose(fd);
    
    return size;
}


