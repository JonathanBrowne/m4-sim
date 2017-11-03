//
//  main.c
//  m4-sim
//
//  Created by Jonathan O'Neill Browne on 11/2/17.
//  Copyright © 2017 Side Dish Productions. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include "stage.h"
#include <unistd.h>
#include "install.h"
#include "action.h"
#include "animate.h"

void install(const char *filename)
{
    printf("installing %s ...\n",filename);
    install_single(filename);
    
  printf("%s installed\n",filename);
    
}

void run(const char * filename)
{
    printf("running %s ...\n",filename);
    Action *a = action_load(filename);
    animate_play(filename);
    action_print(a);
    stage_loop();
    
}

void print_options()
{
   printf ("usage: m4-sim [install,run] [filename] \n");
}

void set_cwd(const char *newpath)
{
    char path[256]="";
    getcwd(path, sizeof(path));
    printf("old path = %s\n",path);
    
    chdir(newpath);
    getcwd(path, sizeof(path));
    printf("current path = %s\n",path);
}

int main(int argc, const char * argv[])
{
      stage_init();
      set_cwd("/project/m4-sim/data");
    
      if(argc == 3 && !strcmp(argv[1],"install"))
      {
          install(argv[2]);
          return 0;
      }
      else  if(argc == 3 && !strcmp(argv[1],"run"))
      {
          run(argv[2]);
          return 0;
          
      }
    
    print_options();
    return 0;
}
