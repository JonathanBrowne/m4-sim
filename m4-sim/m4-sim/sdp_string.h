//
//  sdp_string.h
//  audio-packer
//
//  Created by Jonathan O'Neill Browne on 11/2/17.
//  Copyright © 2017 Side Dish Productions. All rights reserved.
//

#ifndef sdp_string_h
#define sdp_string_h

#include <stdio.h>
//uses a single pre-allocated block of memory to store null terminated char *
//This is not thread safe, only use on the main thread
//sdp_string will take care of defragmenting itself and will only do when there is no slots avalable  for the size you need

//this is meant to handle lots of small strings and will create a 16K pool and a limit of 1024 allocations
//these can be changed here
#define SDP_STRING_POOL_SIZE 16384
#define SDP_STRING_MAX_COUNT 1024

//string_alloc -allocates a buffer of size strlen+1 and copies s into it returns 0 if fails to allocate
//returns an ID to
unsigned int string_alloc(const char * s);

//string_alloc  - creates a clock at size and clears to all 0  - returns 0 if fails to allocate
unsigned int string_calloc(unsigned int size);

//string_get - returns the char * from ID  always call this befor using as the actual pointer can change after a defragmentation operation
char * string_get(unsigned int id);

void string_init(void); //must be called from stage to clear

void string_free(unsigned int id);

#endif /* sdp_string_h */
