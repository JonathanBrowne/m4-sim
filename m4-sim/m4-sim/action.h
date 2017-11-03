//
//  action.h
//  audio-packer
//
//  Created by Jonathan O'Neill Browne on 10/30/17.
//  Copyright © 2017 Side Dish Productions. All rights reserved.
//

#ifndef action_h
#define action_h

#include <stdio.h>

#define LABEL 1090
#define GOTO 1091
#define LOOP 1092
#define END_LOOP 1093
#define BREAK 1094
#define MACRO 1095
#define GOSUB 1096
#define RETURN 1097
#define ACTION 1099
#define TRIGGER 1010
#define MOVE_MOTOR 1020
#define MOVE_HEAD 1021
#define LEFT_ARM 3
#define RIGHT_ARM 2
#define LEFT_LEG 5
#define RIGHT_LEG 4
#define HEAD_LR 0
#define HEAD_UD 1
#define WAV 1040
#define WAV_RANDOM 1041
#define DELAY 1050
#define WAIT_MOTORS 1051
#define WAIT_ONE_MOTOR 1052
#define WAIT_WAV 1054
#define WAIT_WALK 1055
#define WAIT_TURN 1056
#define WAIT_ALL 1057
#define WAIT_HEAD_MOTORS 1058
#define WALK_FORWARD 1060
#define WALK_BACKWARD 1061
#define TURN 1062
#define LISTEN 1070
#define WAIT_LISTEN 1059
#define LISTEN_RESULT 1071
#define OFF 0
#define WORDGROUP 1
#define ANY 2
#define PLAY_ACTION 0
#define GOTO_LABEL 1
#define BREAK_LOOP 2
#define LOOK 2010
#define MOTION 0
#define FACE 1
#define IDENTIFY 2
#define TRAIN 3
#define WAIT_MOTION 2020
#define WAIT_FACE 2021
#define WAIT_IDENTIFY 2022
#define WAIT_TRAIN 2023


struct Action;
//all commands only have at most cmd + 3 numbers , because tring are stored in sdp_string.c
typedef struct
{
    unsigned short cmd;
    unsigned int param[6];  //up to 6 params (using 3 right now), depends on cmd , strings are sdp_string IDs
    void *action; //this is a pointer to an Action in the case cmd = MACRO, should change that to ACTION
} Command;

typedef struct
{
    uint32_t count;
    Command *commands; //needs to be dynamically allocated
    uint32_t name; //sdp_string id for the name
    uint32_t current;
    uint32_t return_index; //index to return from a gosub
    
}Action;


Action *action_load(const char *filename);
void action_free(Action *a);
void action_print(Action *a);
void action_print_command(Command *c);
Command * action_command_current(Action *a);
Command * action_command_next(Action *a);
int action_command_is_wait(Command *c);
Command * action_goto_label(Action *a,uint32_t id);
Command * action_gosub_label(Action *a,uint32_t id);
Command * action_return(Action *a);

#endif /* action_h */
