//
//  animate.c
//  m4-sim
//
//  Created by Jonathan O'Neill Browne on 11/2/17.
//  Copyright © 2017 Side Dish Productions. All rights reserved.
//

#include "animate.h"
#include "action.h"
#include "mwfile.h"
#include "stage.h"
#include "audio_status.h"
#include "sdp_string.h"

#define ANIMATE_STATE_IDLE 0
#define ANIMATE_STATE_WORKING 1
static int state = ANIMATE_STATE_IDLE;
static uint32_t timer =0;
static Action * a=0;


void animate_init()
{
    
}

static void execute(Command *c)
{
    if(c->cmd == DELAY)
    {
        timer= stage_time();
    }
    else if(c->cmd == GOSUB)
    {
        Command * nc= action_gosub_label(a,c->param[0]);
        action_print_command(nc);
    }
    else if(c->cmd == GOTO)
    {
        Command * nc= action_goto_label(a,c->param[0]);
        action_print_command(nc);
    }
    else if(c->cmd == RETURN)
    {
        Command * nc= action_return(a);
        action_print_command(nc);
    }
    else if(c->cmd == WAV)
    {
        const char *fn = string_get(c->param[0]);
        audio_play(fn);
    }
    
   
}

static void next()
{
    state = ANIMATE_STATE_WORKING;
    Command *c=0;
    //need to process action until WAIT
    
      DBG_PRINT("\n\nnext()\n\n");
    
    if(a->current ==0)
    {
        c = action_command_current(a);
        action_print_command(c);
        execute(c);
    }
    
    while(!action_command_is_wait(c))
    {
         c = action_command_next(a);
        if(c)
        {
          action_print_command(c);
          execute(c);
        }
        else
        {
            state = ANIMATE_STATE_IDLE;
            DBG_PRINT("\n\nACTION DONE\n\n");
            action_free(a);
            a=0;
            break;
        }
    }
    
    
    //we should be at a wait here
   
}

static void wait()
{
    Command *c = action_command_current(a);
    if(c->cmd == DELAY && stage_time()-timer > c->param[0])
    {
        next();
    }
    else if(c->cmd == WAIT_WAV)
    {
        //OSX wav player blocks
        next();
    }
}

void animate_process()
{
    if(a && state == ANIMATE_STATE_IDLE)
    {
        next();
    }
    else if(state ==ANIMATE_STATE_WORKING)
    {
        wait();
    }
}

void animate_play(const char *action_filename)
{
    a = action_load(action_filename);
    action_print(a);
}
