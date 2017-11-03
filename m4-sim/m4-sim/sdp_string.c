//
//  sdp_string.c
//  audio-packer
//
//  Created by Jonathan O'Neill Browne on 11/2/17.
//  Copyright © 2017 Side Dish Productions. All rights reserved.
//



#include "sdp_string.h"
#include "string.h"
#include "mwfile.h"

typedef struct
{
    unsigned int status; //0 = virgin 1 = in use 2 = free (after use)
    unsigned int id; //id of string item ,
    unsigned int size; //size including the null terminator
    unsigned int offset; //offset in buffer when the string starts
}SDPStringItem;

static unsigned int sequence = 1;
static unsigned int count = 0; //number of elements in use in strings list
static SDPStringItem strings[SDP_STRING_MAX_COUNT];
static char pool[SDP_STRING_POOL_SIZE];

#define SDP_STRING_MAX_COUNT 1024
#define SDP_STRING_STATUS_VIRGIN 0
#define SDP_STRING_STATUS_INUSE 1
#define SDP_STRING_STATUS_FREE 2

void string_defrag()
{
    //TODO
    printf("\nstring_defrag not implemented\n");
}

int string_find_free(unsigned int size)
{
    
    int i;
    
    //find closest free size and use it
    int index = -1;
    int last_size =0;
    for(i=0; i < count; i++)
    {
        if(strings[i].status == SDP_STRING_STATUS_FREE &&  strings[i].size <= size && strings[i].size > last_size )
        {
            last_size = strings[i].size;
            index =i;
        }
    }
    
    //add on to end if nothing free
    if(index ==-1)
    {
        unsigned int offset = (count)?strings[count-1].offset+strings[count-1].size:0;
        if(count < SDP_STRING_MAX_COUNT-1 && offset+size < SDP_STRING_POOL_SIZE )
        {
            //we have space in the pool and space in our index
            index =count;
            strings[index].id=sequence++;
            strings[index].offset = offset;
            strings[index].size = size;
            strings[index].status =SDP_STRING_STATUS_INUSE;
            count++;
        }
    }
    
    
    return index;
}

unsigned int _string_alloc(int size)
{
    int offset =0; //offset in block for pointer
    int index = string_find_free(size);
    if(index == -1)
    {
        string_defrag();
        index =string_find_free(size);
        if(index == -1)
        {
            DBG_PRINT("\n_string_alloc failed for size %d\n\n",size);
            
            return index;
        }
    }
    
    return index;
}
unsigned int string_alloc(const char * s)
{
    int len = (int)strlen(s)+1;
    
    int index = _string_alloc(len);
    if(index != -1)
    {
        char *p =&pool[strings[index].offset];
        strcpy(p,s);
    }
    return strings[index].id;
}


unsigned int string_calloc(unsigned int size)
{
    
    int index = _string_alloc(size);
    if(index != -1)
    {
        char *p =&pool[strings[index].offset];
        memset(p,0,strings[index].size);
    }
    return strings[index].id;
}

SDPStringItem * string_get_item(unsigned int id)
{
    int i;
    for(i=0; i< count; i++)
    {
        if(strings[i].id == id)
        {
            return &strings[i];
        }
    }
    return 0;
}

char * string_get(unsigned int id)
{
    SDPStringItem * si =string_get_item(id);
    if(si)
    {
        return &pool[si->offset];
    }
    printf("\n\nstring_get() nothng found for %d\n",id);
    return 0;
}

void string_free(unsigned int id)
{
    SDPStringItem * si =string_get_item(id);
    if(si)
    {
        si->status = SDP_STRING_STATUS_FREE;
    }
    DBG_PRINT("\n\nstring_free() nothing found for %d\n",id);
    
}

void string_init()
{
    memset(pool,0,sizeof(pool));
    memset(strings,0,sizeof(strings));
    
}
