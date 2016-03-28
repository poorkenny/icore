/* 
 * File:   main.cpp
 * Author: stan
 *
 * Created on June 14, 2013, 8:22 AM
 */

#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <sstream>
#include <sys/time.h>
#include "SyncController.h"

using namespace std;

SyncController* pre=NULL;


void* SocketHandler(void* lp);
/*
 * 
 */
int main(int argc, char** argv) {
     util::daemonize();
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     struct sockaddr_in serv_addr, cli_addr;   
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
       printf("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = SYNC_CONTROLLER_PORT;
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);     
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)
              printf("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     
     pthread_t thread_id=0;
     while(true){
    
        cout<<"Sync controller stands by:"<<endl;
        
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
/*     if(line.substr(0,sizeof("create new session|")-1).compare("create new session|")==0){
         
         string data(receive_buffer+sizeof("create new session|")-1);
         if(pre!=NULL)
           delete pre;
         SyncController *syn=new SyncController();
         pre=syn;
         (*syn).init((char*)data.c_str());
         cout<<"Session has been created!"<<endl;  
          close(newsockfd);
          
     }   */   
    if(line.substr(0,sizeof("synctime:")-1).compare("synctime:")==0){
         if(pre==NULL){
             cout<<"No session created yet!"<<endl;
             close(newsockfd);
         }
         else{
            close(newsockfd); 
            string data(receive_buffer+sizeof("synctime:")-1);
            (*pre).proceed(data);          
            cout<<"Session has been proceeded!"<<endl;     
            
         }
                
     }
     else if(line.substr(0,sizeof("session completed!")-1).compare("session completed!")==0){
         close(newsockfd);
         (*pre).shutDownSession();
         delete pre;
         pre=NULL;
     }
     else if(line.substr(0,sizeof("gridlabd name query:")-1).compare("gridlabd name query:")==0){
         string data(receive_buffer+sizeof("gridlabd name query:")-1); 
          result=(*pre).queryGridlabdName(data);    
          util::send_data(newsockfd,result,MAXIMUM_PACKET_SIZE);        
          cout<<"GridLabd name has been returned!"<<endl;     
          close(newsockfd);
         
     }
     else{
         // receive .sync file name
         string data(receive_buffer);
         if(pre!=NULL){
           delete pre;
           pre=NULL;
         }
         SyncController *syn=new SyncController();
         pre=syn;
         (*syn).init((char*)data.c_str());
         cout<<"Session has been created!"<<endl;  
          close(newsockfd);
         
     }
      
    }
     return 0;
 }
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
 
   
     
    
   