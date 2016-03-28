#include <sys/stat.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <cstdlib>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <fstream>
#include "Util.h"
using namespace std;
LogFile util::logFile;
void util::daemonize(void)
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
//This function will send large data using socket. After that
//it invokes shutdown to disallow sending through socket,so the other
//side can know data has been received completely. Reading is still ok
//for receiving feedback. The maximum data length is a long int type.
void util::send_data(int sock,string data,int max_packet_size){
        long int bytes_left=data.length();
        long int packet_num=0;
        long int n=0;
 //       cout<<"Sending data..."<<endl;
     while(bytes_left>0){         
         if(bytes_left<=max_packet_size){
           n=send(sock,data.c_str()+packet_num*max_packet_size,bytes_left, 0);
         }
         else {
             n=send(sock,data.c_str()+packet_num*max_packet_size,max_packet_size, 0);
         }
        packet_num++;
        bytes_left=data.length()-packet_num*max_packet_size;
    }
    if(bytes_left<=0)
  //     cout<<"Total "<<data.length()<<" bytes have been sent!"<<endl;
        // This closes the sending.
     shutdown(sock, SHUT_WR);       
        
} 
long int util::receive_data(int sockfd,char* receive_buffer,long int receive_buffer_size,
        int max_packet_size){                      
  //  cout<<"Receiving start..."<<endl;
     long int bytes_received=0;
     int n;
     char packet[max_packet_size];
     while((n = read(sockfd,packet,max_packet_size))>0){
         bytes_received+=n;
         if(bytes_received >=receive_buffer_size){
             cout<<"Receive buffer overflow!"<<endl;
             break;
         }
         if(packet[n-6]=='$' && packet[n-5]=='$' && packet[n-4]=='$' && packet[n-3]=='$'){
           for(int i=0;i<n-6;i++){
             //$$$$ is length 4,plus \r\n,  6 bytes in all from tcl side.
            receive_buffer[bytes_received-n+i]=packet[i];
           }  
              return bytes_received-6;
         }
         for(int i=0;i<n;i++){
             //$$$$ is length 4,plus \r\n,  6 bytes in all from tcl side.
            receive_buffer[bytes_received-n+i]=packet[i];
         }
        
         bzero(packet,max_packet_size);
     }
  //   cout<<"Total "<<bytes_received<<" bytes have been received!"<<endl;
     return bytes_received;
}
void util::log(string log){
    if(LOGFLAG==TO_CONSOLE){
        cout<<log<<endl;
    }
    if(LOGFLAG==TO_LOGFILE){   
     util::logFile.log_file<<log<<endl;
    }
}
