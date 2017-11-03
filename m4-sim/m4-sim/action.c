//
//  action.c
//  audio-packer
//
//  Created by Jonathan O'Neill Browne on 10/30/17.
//  Copyright © 2017 Side Dish Productions. All rights reserved.
//

#include <string.h>
#include "action.h"
#include "mwfile.h"
#include "sdp_string.h"



const char * action_command_string(uint16_t c)
{
    switch(c)
    {
        case LABEL: return "LABEL";
        case GOTO:  return "GOTO";
        case GOSUB: return "GOSUB";
        case ACTION: return "ACTION";
        case MACRO: return "MACRO";
        case TRIGGER: return "TRIGGER";
        case WAV:return "WAV";
        case WAV_RANDOM: return "WAV_RANDOM";
        case MOVE_MOTOR: return "MOVE_MOTOR";
        case MOVE_HEAD: return "MOVE_HEAD";
        case WAIT_ONE_MOTOR: return "WAIT_ONE_MOTOR";
        case WAIT_ALL: return "WAIT_ALL";
        case WAIT_TURN: return "WAIT_TURN";
        case WAIT_WALK: return "WAIT_WALK";
        case WAIT_WAV: return "WAIT_WAV";
        case WAIT_HEAD_MOTORS:return "WAIT_HEAD_MOTORS";
        case WAIT_MOTORS: return "WAIT_MOTORS";
        case WAIT_LISTEN:return "WAIT_LISTEN";
        case RETURN: return "RETURN";
        case END_LOOP: return "END_LOOP";
        case BREAK:return "BREAK";
        case WAIT_MOTION:return "WAIT_MOTION";
        case WAIT_FACE:return "WAIT_FACE";
        case WAIT_IDENTIFY:return "WAIT_IDENTIFY";
        case WAIT_TRAIN:return "WAIT_TRAIN";
        case DELAY: return "DELAY";
        case LOOP: return "LOOP";
        case LOOK: return "LOOK";
        case LISTEN: return "LISTEN";
        case LISTEN_RESULT: return "LISTEN_RESULT";
    }
    
    return "CMD_UNKNOWN";
}

const char * action_motor_id_string(unsigned char c)
{
    
    switch(c)
    {
         case LEFT_ARM : return "LEFT_ARM";
         case RIGHT_ARM: return "RIGHT_ARM";
         case LEFT_LEG: return "LEFT_LEG";
         case RIGHT_LEG: return "RIGHT_LEG";
         case HEAD_LR: return "HEAD_LR";
         case HEAD_UD: return "HEAD_UD";
    }
    
    return "MOTOR_UNKNOWN";
}

const char * action_look_string(unsigned char c)
{
    switch(c)
    {
        case MOTION : return "MOTION";
        case IDENTIFY: return "IDENTIFY";
        case FACE: return "FACE";
        case TRAIN: return "TRAIN";
    }
    
    return "LOOK_UNKNOWN";
}

const char * action_listen_string(unsigned char c)
{
    switch(c)
    {
        case OFF : return "OFF";
        case WORDGROUP: return "WORDGROUP";
        case ANY: return "ANY";
    }
    
    return "LISTEN_UNKNOWN";
}

const char * action_listen_result_string(unsigned char c)
{
    switch(c)
    {
        case PLAY_ACTION : return "PLAY_ACTION";
        case GOTO_LABEL: return "GOTO_LABEL";
        case BREAK_LOOP: return "BREAK_LOOP";
    }
    
    return "LISTEN_RESULT_UNKNOWN";
}

void action_print_command(Command *cmd)
{
    uint16_t c = cmd->cmd;
    if(c==LABEL || c==GOTO || c==GOSUB || c ==ACTION || c == MACRO||
       c== TRIGGER || c==WAV || c== WAV_RANDOM)
    {
        DBG_PRINT("%s %s \n",action_command_string(c),string_get(cmd->param[0]));
       
    }
    else if(c==MOVE_MOTOR || c==MOVE_HEAD)
    {
         DBG_PRINT("%s %s  %d %d\n",action_command_string(c),
                                    action_motor_id_string(cmd->param[0]),
                                    cmd->param[1],cmd->param[2]
                   );
        
    }
    else if(c==WAIT_ONE_MOTOR)
    {
        DBG_PRINT("%s %s\n",action_command_string(c),
                  action_motor_id_string(cmd->param[0]));
       
    }
    else if(c ==WAIT_ALL || c == WAIT_TURN || c == WAIT_WALK || c == WAIT_WAV ||
            c ==WAIT_HEAD_MOTORS || c == WAIT_MOTORS|| c == WAIT_LISTEN || c == RETURN ||
            c == END_LOOP || c== BREAK || c == WAIT_MOTION || c == WAIT_FACE ||
            c == WAIT_IDENTIFY || c == WAIT_TRAIN)
    {
        DBG_PRINT("%s\n",action_command_string(c));
    }
    else if(c == DELAY  || c == LOOP)
    {
         DBG_PRINT("%s %d\n",action_command_string(c),cmd->param[0]);
    }
    else if(c == LOOK)
    {
        //LOOK MOTION 30000 search_got_motion
        
         DBG_PRINT("%s %s %d %s\n",action_command_string(c),
                             action_look_string(cmd->param[0]),
                             cmd->param[1],
                             string_get(cmd->param[2])
                   );
    }
    else if(c == LISTEN )
    {
        //LISTEN WORDGROUP sentry 6000
        
        DBG_PRINT("%s %s %s %d\n",action_command_string(c),
                  action_listen_string(cmd->param[0]),
                  string_get(cmd->param[1]),
                  cmd->param[2]
                  );
    }
    else if(c == LISTEN_RESULT )
    {
        //LISTEN_RESULT NO_ANSWER GOTO wrong_no
        //LISTEN_RESULT Start Patrol GOTO start_patrol
        
        DBG_PRINT("%s %s %s %s\n",action_command_string(c),
                   string_get(cmd->param[0]),
                   action_listen_result_string(cmd->param[1]),
                   string_get(cmd->param[2])
                  );
        
      
    }
}
void action_print(Action *a)
{
    DBG_PRINT("Action %s\n", string_get(a->name));
    int i;
    for (i=0; i < a->count; i++)
    {
        action_print_command(&a->commands[i]);
    }
}
uint32_t action_count(unsigned char *buf,uint32_t size)
{
    uint32_t count =0;
    uint32_t offset=0;
  
    while (offset < size)
    {
         uint32_t cmd_size=0;
         convert_u8_to_u32(&buf[offset], &cmd_size);
         count++;
         offset+=cmd_size;
    }
    return count;
}

void action_parse_command(Action *a, uint32_t index,unsigned char *b)
{
    int i;
    for(i=0; i< 6; i++)
    {
        a->commands[index].param[i]=0;
    }
    
    uint16_t c = a->commands[index].cmd;
    if(c==LABEL || c==GOTO || c==GOSUB || c ==ACTION || c == MACRO||
       c== TRIGGER || c==WAV || c== WAV_RANDOM)
    {
       //unsigned char name_size = b[0];  dont need the size since its null terminated
       a->commands[index].param[0]= string_alloc((const char *)&b[1]);
    }
    else if(c==MOVE_MOTOR || c==MOVE_HEAD)
    {
        a->commands[index].param[0] = b[0]; //motor_id or position LR
        a->commands[index].param[1] = b[1]; // position or position UD
        uint16_t duration=0;
        convert_u8_to_u16(&b[2], &duration);
        a->commands[index].param[2] = duration;
    }
    else if(c==WAIT_ONE_MOTOR)
    {
        a->commands[index].param[0] = b[0]; //motor id
    }
    else if(c ==WAIT_ALL || c == WAIT_TURN || c == WAIT_WALK || c == WAIT_WAV ||
            c ==WAIT_HEAD_MOTORS || c == WAIT_MOTORS|| c == WAIT_LISTEN || c == RETURN ||
            c == END_LOOP || c== BREAK || c == WAIT_MOTION || c == WAIT_FACE ||
            c == WAIT_IDENTIFY || c == WAIT_TRAIN)
    {
        //dont need to write anything here only command
    }
    else if(c == DELAY  || c == LOOP)
    {
        uint16_t duration=0;
        convert_u8_to_u16(&b[0], &duration);
        a->commands[index].param[0] = duration;
    }
    else if(c == LOOK)
    {
        //LOOK MOTION 30000 search_got_motion
  
         a->commands[index].param[0]= b[0]; // MOTION (0),FACE (1),IDENTIFY(2), TRAIN (3)
        
        uint32_t timeout=0;
        convert_u8_to_u32(&b[1], &timeout);
        a->commands[index].param[1]=timeout;
  
        //label for when we got motion
        a->commands[index].param[2]= string_alloc((const char *)&b[6]);
  
    }
    else if(c == LISTEN )
    {
        //LISTEN WORDGROUP sentry 6000
          a->commands[index].param[0]= b[0]; //OFF(0), WORDGROUP(1), ANY (2)
        
          //timeout is second in file but last in command
         uint16_t timeout=0;
         convert_u8_to_u16(&b[1], &timeout);
         a->commands[index].param[2]=timeout;
        // unsigned char fname_size = b[3];
         a->commands[index].param[1]= string_alloc((const char *)&b[4]);  //filename
        
    }
    else if(c == LISTEN_RESULT )
    {
        //LISTEN_RESULT NO_ANSWER GOTO wrong_no
        //LISTEN_RESULT Start Patrol GOTO start_patrol
        
         a->commands[index].param[1]= b[0]; //PLAY_ACTION(0), GOTO_LABEL(1), BREAK_LOOP (2)
         unsigned char fname_size =b[1];
         a->commands[index].param[0]= string_alloc((const char *)&b[2]);  //result
         a->commands[index].param[2]= string_alloc((const char *)&b[2+fname_size]);  //label
    }
}


Action * action_create(unsigned char *buf,uint32_t size,const char * filename)
{
     // run through file buffer to get count of commands
     uint32_t count = action_count(buf,size);
     DBG_PRINT("count for %s = %d\n",filename,count);
    
   //allocate an action and and allocate the command list based on count
     Action *a = (Action *)gp_malloc_align(sizeof(Action),4);
     a->commands = (Command *) gp_malloc_align(sizeof(Command)*count,4);
     a->name = string_alloc(filename);
     a->count = count;
     a->current =0;
     a->return_index=0;
    
     //run through the file again parsing commands into the list
     uint32_t offset=0;
     uint32_t index = 0;
     while (offset < size)
     {
        uint32_t cmd_size=0;
        uint16_t cmd=0;
        convert_u8_to_u32(&buf[offset], &cmd_size);
        convert_u8_to_u16(&buf[offset+4], &cmd);
        a->commands[index].cmd = cmd;
         
         action_parse_command(a,index,&buf[offset+6]);
         
        index++;
        offset+=cmd_size;
      }
    
    
    return a;
}

Action *action_load(const char *filename)
{
    
    //load the file,
    uint32_t size=mwfile_size(filename);
    unsigned char * buf = (unsigned char *)gp_malloc_align(size,4);
    mwfile_load(filename, buf, size);
   //create the action
    Action *a= action_create(buf,size,filename);
    gp_free(buf);
    
    return a;
}

Command * action_command_current(Action *a)
{
    return &a->commands[a->current];
}

Command * action_command_next(Action *a)
{
    if(a->current+1 == a->count) return 0;
    a->current++;
    return action_command_current(a);
}

int action_command_is_wait(Command *cmd)
{
    if(!cmd)
    {
        return 0;
    }
    uint16_t c = cmd->cmd;
    if(c ==WAIT_ALL || c == WAIT_TURN || c == WAIT_WALK || c == WAIT_WAV ||
       c ==WAIT_HEAD_MOTORS || c == WAIT_MOTORS|| c == WAIT_LISTEN ||
       c == WAIT_MOTION || c == WAIT_FACE ||
       c == WAIT_IDENTIFY || c == WAIT_TRAIN || c==DELAY)
    {
        return 1;
    }
    
    return 0;
}

Command * action_goto_label(Action *a,uint32_t id)
{
    const char *sid = string_get(id);
    int i;
    for(i=0; i< a->count; i++)
    {
     
       if( a->commands[i].cmd == LABEL )
       {
          const char *tid =string_get(a->commands[i].param[0]);
          if( !strcmp(sid,tid))
          {
            a->current =i;
            return &a->commands[i];
          }
       }
    }
    return 0;
}

Command * action_gosub_label(Action *a,uint32_t id)
{
    //DBG_PRINT("\nACTION GOSUB LABEL\n");
    
    const char *sid = string_get(id);
    
    a->return_index=a->current+1;
    int i;
    for(i=0; i< a->count; i++)
    {
       // action_print_command(&a->commands[i]);
       
        if( a->commands[i].cmd == LABEL)
        {
            const char *tid =string_get(a->commands[i].param[0]);
            if( !strcmp(sid,tid))
            {
              a->current =i;
              return &a->commands[i];
            }
        }
    }
    return 0;
}

Command * action_return(Action *a)
{
    a->current=a->return_index;
    a->return_index = 0;
    return action_command_current(a);

}

void action_free(Action *a)
{
    //TODO delete the memeory recursivly
}


