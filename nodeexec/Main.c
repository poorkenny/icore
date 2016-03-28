/* 
 * File:   main.c
 * Author: stan
 *
 * Created on June 4, 2013, 3:26 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "VtFunctions.h"
#include "GridLabD.h"
#include <string.h>
#define VIRTUAL_TIME 
#ifdef VIRTUAL_TIME
#define sleep vt_sleep
#define gettimeofday vt_gettimeofday
#endif
/*
 * 
 */
void daemonize();
int main(int argc, char** argv) {

   struct timeval start;
   FILE *file; 
   char file_name[20]="/tmp/vt_";
   double priceWrite=0;
   double priceRead=0;      
   daemonize();
   strcat(file_name,argv[1]);
   strcat(file_name,".log");
   int i=0;
      while(1){
        if(priceWrite==0.1){
            priceWrite=0.01;
        }
        else{
            priceWrite+=0.01;
        }
        sleep(5);        
        gettimeofday(&start, NULL);
      //  setPropertyValue("price",priceWrite);
      //  priceRead=getPropertyValue("price");
        file = fopen(file_name,"a+");
        fprintf(file, "vt_gettimeofday: now is %ld sec, %ld usec\n",start.tv_sec,start.tv_usec);
        fprintf(file, "sleep: %d\n",i);
        fprintf(file, "price: %f\n",priceRead);
        fclose(file);
       
        i++;
        
     }
    return (EXIT_SUCCESS);
}

void daemonize(void)
{
    pid_t pid, sid;

    /* already a daemon */
    if ( getppid() == 1 ) return;

    /* Fork off the parent process */
    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    /* If we got a good PID, then we can exit the parent process. */
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    /* At this point we are executing as the child process */

    /* Change the file mode mask */
         umask(0);
    /* Create a new SID for the child process */
    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }
   
    /* Change the current working directory.  This prevents the current
       directory from being locked; hence not being able to remove it. */
    if ((chdir("/")) < 0) {
        exit(EXIT_FAILURE);
    }

    /* Redirect standard files to /dev/null */
    freopen( "/dev/null", "r", stdin);
    freopen( "/dev/null", "w", stdout);
    freopen( "/dev/null", "w", stderr);
}