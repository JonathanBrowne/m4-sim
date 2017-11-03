//
//  install.c
//  audio-packer
//
//  Created by Jonathan O'Neill Browne on 10/30/17.
//  Copyright © 2017 Side Dish Productions. All rights reserved.
//

#include "install.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mwfile.h"



unsigned char * install_load(const char * filename,int *file_size)
{
    uint32_t size =  mwfile_size(filename);
    if(size == -1 )
    {
        printf("install could not find %s\n",filename);
        return 0;
    }
    
    unsigned char * buf = (unsigned char *)gp_malloc_align(size,4);
    
    uint32_t r =  mwfile_load(filename, buf, size);
    *file_size = r;
    return buf;
}




unsigned char install_single(const char * filename)
{
    unsigned char version = 0;
    int size =0;
    unsigned char * buf = install_load(filename,&size);
    if(size < 1 )
    {
        return 0;
    }
    
    char fname[255+1];
    int offset =0;
    //first byte is version
    
    version=buf[0];
    offset++;
    
    while(offset < size)
    {
        uint32_t message_size = 0;
        convert_u8_to_u32(&buf[offset],&message_size);  //4 bytes
        offset+=4;
        
        unsigned char len= buf[offset];   //1 byte length of filename including null
        offset++;
        
        //filename including null
        memcpy(fname,&buf[offset],len);
        offset+=len;
        
        uint32_t data_size=0;
        convert_u8_to_u32(&buf[offset],&data_size); //4 byte size of data
        offset+=4;
        
        //data
        mwfile_save(fname, &buf[offset], data_size);
        
        printf("installed %s\n",fname);
        offset+=data_size;
        
    }
    
    //free buffer
    free(buf);
    buf=0;
    
   // mwfile_rm(filename);
    
    return version;
}


