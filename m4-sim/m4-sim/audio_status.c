//
//  audio_status.c
//  m4-sim
//
//  Created by Jonathan O'Neill Browne on 11/2/17.
//  Copyright © 2017 Side Dish Productions. All rights reserved.
//

#include <stdlib.h>
#include "audio_status.h"

void audio_play(const char *fn)
{
    char buf[256]="";
    sprintf(buf,"afplay %s.wav",fn);
    system(buf);
}
