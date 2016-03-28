/* 
 * File:   main.cpp
 * Author: stan
 *
 * Created on June 13, 2013, 5:36 PM
 */

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
#include "VtController.h"
#include <fstream>
#include "Util.h"
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
using namespace std;

VtController* pre=NULL;


void* SocketHandler(void* lp);

int main(int argc, char** argv) {
    
       util::daemonize();
    /*
     Setting up the main socket for the program.
     */
    
     int sockfd, newsockfd, portno;

     socklen_t clilen;
     struct sockaddr_in serv_addr, cli_addr;   
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
       printf("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = 9009;
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);     
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)
              printf("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     

     /*
      Thread related data structure. One thread takes care of one session.
      */
   
  
     pthread_t thread_id=0;
     while(true){
    
        cout<<"Virtual time controller stands by:"<<endl;
        
        newsockfd = accept(sockfd,
                 (struct sockaddr *) &cli_addr,
                 &clilen);
        if (newsockfd < 0)
            printf("ERROR on accept");
        else{
          pthread_create(&thread_id,0,&SocketHandler, (void*)&newsockfd);
          pthread_detach(thread_id);
        }
     
     }
     close(sockfd);
     delete pre;
    
    return 0;
}

 void* SocketHandler(void* lp){
     int newsockfd=*(int*)lp;
     long int n;
     char receive_buffer[SOCKET_BUFFER_SIZE];
     bzero(receive_buffer,SOCKET_BUFFER_SIZE);
     
     n=util::receive_data(newsockfd,receive_buffer,SOCKET_BUFFER_SIZE,MAXIMUM_PACKET_SIZE);  
    
     if (n < 0)
         printf("ERROR reading from socket");
     printf("Message received:\n");
     printf("*********************************\n");
     printf("%s",receive_buffer);
     printf("*********************************\n");
     
     string line;
     string result;
     std::istringstream iss(receive_buffer);
      /* 
      * The first received line decides which branches to go.
      */
     getline(iss,line);
   if(line !=""){
     if(line.substr(0,sizeof("create new session:")-1).compare("create new session:")==0){
         /*
          * Create new session based on input from GUI,
          * Return the result to GUI.
          */
         string data(receive_buffer+sizeof("create new session:")-1);
         if(pre!=NULL){
           delete pre;
           pre=NULL;
         }
         VtController *vt_c=new VtController();
         pre=vt_c;
         (*vt_c).init((char*)data.c_str());
         cout<<"Session has been created!"<<endl;  
          close(newsockfd);
          
     }      
     else if(line.substr(0,sizeof("proceed session:")-1).compare("proceed session:")==0){
         /*
          * Update session based on the input from energy daemons.
          * Updated result is returned to GUI.
          */
         if(pre==NULL){
             cout<<"No session created yet!"<<endl;
             close(newsockfd);
         }
         else{
            string data(receive_buffer+sizeof("proceed session:")-1);
            (*pre).proceed(data);    
            result="success";
            util::send_data(newsockfd,result,MAXIMUM_PACKET_SIZE);        
            cout<<"Session has been proceeded!"<<endl;     
            close(newsockfd);
         }
                
     }
     else if(line.substr(0,sizeof("shutdown vt session:")-1).compare("shutdown vt session:")==0){
         /*
          * Update session based on the input from energy daemons.
          * Updated result is returned to GUI.
          */
         if(pre==NULL){
            cout<<"No session created yet!"<<endl;
            close(newsockfd);
         }else{
             delete pre;
             pre=NULL;
          cout<<"virtual time control has been shutdown!"<<endl;     
          close(newsockfd);
         
         }
                
     }
//     else if(line.substr(0,sizeof("vt_sleep:")-1).compare("vt_sleep:")==0){
//         /*
//          * Update session based on the input from energy daemons.
//          * Updated result is returned to GUI.
//          */
//         if(pre==NULL){
//            cout<<"No session created yet!"<<endl;
//            close(newsockfd);
//         }
//         else{
//            string data(receive_buffer+sizeof("vt_sleep:")-1);
//            (*pre).vt_sleep(data);    
//            cout<<"virtual sleep time has been processed!"<<endl;     
//            close(newsockfd);
//         
//         }
                
//     }
     
      
    }
  //   pthread_exit(NULL);
     return 0;
 }
