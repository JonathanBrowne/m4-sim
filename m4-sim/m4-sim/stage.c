//
//  stage.c
//  audio-packer
//
//  Created by Jonathan O'Neill Browne on 11/2/17.
//  Copyright © 2017 Side Dish Productions. All rights reserved.
//


#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "stage.h"
#include "sdp_string.h"
#include "animate.h"

void stage_init()
{
    string_init();
}

static uint32_t stage_t =0;

uint32_t stage_time()
{
    return stage_t;
}

void stage_sleep(uint32_t ms)
{

    usleep(ms*1000);
}
    


void stage_loop()
{
    while(1)
    {
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        stage_t = (uint32_t)(ts.tv_sec*1000);  //ms since epoch
        

      //  service_process();
        //call process on each service ...
        animate_process();
        
        stage_sleep(10);
        // printf("%d\n",stage_t);
    }
}


