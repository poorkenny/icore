#include "SyncController.h"
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>

using namespace std;
SyncController::SyncController(){
    
}
unsigned long int SyncController::convertToUnixTime(string t){
    unsigned long int ss=0;
    //use struct tm needs the initialization process as follow@!!!!!!
    time_t rawtime;
    struct tm * tm1;
    time ( &rawtime );
    tm1 = localtime ( &rawtime );
    string year=t.substr(0,4);
    string month=t.substr(5,2);
    string day=t.substr(8,2);
    string hour=t.substr(11,2);
    string min=t.substr(14,2);
    string sec=t.substr(17);
    int iyear=atoi(year.c_str());
    int imonth=atoi(month.c_str());
    int iday=atoi(day.c_str());
    int ihour=atoi(hour.c_str());
    int imin=atoi(min.c_str());
    int isec=atoi(sec.c_str());  
    tm1->tm_year=iyear-1900;
    tm1->tm_mon=imonth-1;
    tm1->tm_mday=iday;
    tm1->tm_hour=ihour;
    tm1->tm_min=imin;
    tm1->tm_sec=isec;
    ss=mktime(tm1);
    return ss;
}
void SyncController::init(char* inputFile){
    string fileName(inputFile);
    fileName=fileName.erase(fileName.find_last_not_of(" \n\r\t")+1);
    std::ifstream infile((const char*)fileName.c_str());  
    std::string line;
    int i=0;
  while(std::getline(infile, line))
  {
    if(i!=3){
     cout<<line<<endl;
     if(i==0){
        vnodeNumber=atoi(line.c_str());   
     }   
     else if(i==1){
       virtualStartTime=line;
       virtualStartUnixTime=convertToUnixTime(virtualStartTime);
     }
     else if(i==2){
        virtualStopTime=line;
        virtualStopUnixTime=convertToUnixTime(virtualStopTime);
     }
    }
    else{
          break;
    }
    i++;
  }
    virtualCurrentUnixTime=virtualStartUnixTime;
    gettimeofday(&realStartTime,NULL);
    
    
    
    
    int sock;
    struct hostent *host1;
    struct sockaddr_in server_addr1;
    char send_buffer[SOCKET_BUFFER_SIZE];
    string ip="localhost";
    int port =VT_CONTROLLER_PORT;
    int s=vnodeNumber;
    char vn_number[5];
     
    bzero(send_buffer,SOCKET_BUFFER_SIZE);
    host1 = gethostbyname(ip.c_str());
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
         printf("Socket creation failed!");
         exit(1);
    }

     server_addr1.sin_family = AF_INET;
     server_addr1.sin_port = htons(port);
     server_addr1.sin_addr = *((struct in_addr *)host1->h_addr);
     bzero(&(server_addr1.sin_zero),8);

     if (connect(sock, (struct sockaddr *)&server_addr1,
                    sizeof(struct sockaddr)) == -1)
     {
         printf("Connect with Virtual time control failed!\n");
         exit(1);
           
     }
     strcpy(send_buffer,"create new session:");
     sprintf(vn_number,"%d\n",s);
     strcat(send_buffer,vn_number);
     send(sock,send_buffer,strlen(send_buffer), 0);
     close(sock);      
     
     
     
     
   
/*
 *   test without gridlabd, virtual time speeded up.
 *   finished within 12.319169s, 3 VNs.
 *   finished within 15.354691, 30 VNs.
 */    
  /*  sleep(3);
    struct timeval tv1,tv2;
    gettimeofday(&tv1,NULL);
     int sync_win=1;
  for(int i=0;i<30;i++){
    int sock;
    struct hostent *host1;
    struct sockaddr_in server_addr1;
    char send_buffer[SOCKET_BUFFER_SIZE];
    char receive_buffer[SOCKET_BUFFER_SIZE];
    const string ip="localhost";
    int port =VT_CONTROLLER_PORT;
    char s_sync_win[10];
    host1 = gethostbyname(ip.c_str());
    bzero(send_buffer,SOCKET_BUFFER_SIZE);
    bzero(receive_buffer,SOCKET_BUFFER_SIZE);
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
         printf("Socket creation failed!");
         exit(1);
    }

    server_addr1.sin_family = AF_INET;
    server_addr1.sin_port = htons(port);
    server_addr1.sin_addr = *((struct in_addr *)host1->h_addr);
    bzero(&(server_addr1.sin_zero),8);
     
    if (connect(sock, (struct sockaddr *)&server_addr1,
                    sizeof(struct sockaddr)) == -1)
    {
         cout<<"connect with virtual control failed!"<<endl;
         exit(1);
    }
    strcpy(send_buffer,"proceed session:");
    sprintf(s_sync_win,"%d\n",sync_win);
    strcat(send_buffer,s_sync_win);
    if(send(sock,send_buffer,strlen(send_buffer), 0)!=-1){
         printf("proceed info sent!\n");
         shutdown(sock, SHUT_WR); 
    }
    if(read(sock,receive_buffer,SOCKET_BUFFER_SIZE)!=-1){
        printf("receive: %s\n",receive_buffer);
    }
    close(sock);
  }
     gettimeofday(&tv2,NULL);
     printf ("total execute time = %f seconds\n",
             (double) (tv2.tv_usec - tv1.tv_usec)/1000000 +
             (double) (tv2.tv_sec -tv1.tv_sec));
    */ 
     
}
void SyncController::proceed(string data){
    
    string timeWithoutTimeZone=data.substr(0,19);
    
    unsigned long int barrier=convertToUnixTime(timeWithoutTimeZone);
    int sync_win=0;
  if(barrier>virtualCurrentUnixTime){
        sync_win=barrier-virtualCurrentUnixTime;
   
    
    int sock;
    struct hostent *host1;
    struct sockaddr_in server_addr1;
    char send_buffer[SOCKET_BUFFER_SIZE];
    char receive_buffer[SOCKET_BUFFER_SIZE];
    const string ip="localhost";
    int port =VT_CONTROLLER_PORT;
    char s_sync_win[10];
    host1 = gethostbyname(ip.c_str());
    bzero(send_buffer,SOCKET_BUFFER_SIZE);
    bzero(receive_buffer,SOCKET_BUFFER_SIZE);
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
         printf("Socket creation failed!");
         exit(1);
    }

    server_addr1.sin_family = AF_INET;
    server_addr1.sin_port = htons(port);
    server_addr1.sin_addr = *((struct in_addr *)host1->h_addr);
    bzero(&(server_addr1.sin_zero),8);
     
    if (connect(sock, (struct sockaddr *)&server_addr1,
                    sizeof(struct sockaddr)) == -1)
    {
         cout<<"connect with virtual control failed!"<<endl;
         exit(1);
    }
    strcpy(send_buffer,"proceed session:");
    sprintf(s_sync_win,"%d\n",sync_win);
    strcat(send_buffer,s_sync_win);
    if(send(sock,send_buffer,strlen(send_buffer), 0)!=-1){
         printf("proceed info sent!\n");
         shutdown(sock, SHUT_WR); 
    }
    if(read(sock,receive_buffer,SOCKET_BUFFER_SIZE)!=-1){
        printf("receive: %s\n",receive_buffer);
    }
    close(sock);

    
    virtualCurrentUnixTime=barrier;
   }
      system("wget http://localhost:20000/control/resume");
      system("rm resume*");
    
}

void SyncController::shutDownSession(){
     gettimeofday(&realStopTime, NULL);
     printf ("ShutDown session: total execute time = %f seconds\n",
             (double) (realStopTime.tv_usec - realStartTime.tv_usec)/1000000 +
             (double) (realStopTime.tv_sec - realStartTime.tv_sec));
   
     shutDownVtController();
}
string SyncController::queryGridlabdName(string coreId){
    cout<<"core id:"<<coreId<<endl;
    return "tm_B_1_645";
}
SyncController::~SyncController(){
    
}
void SyncController::shutDownVtController(){
    int sock;
    struct hostent *host1;
    struct sockaddr_in server_addr1;
    char send_buffer[SOCKET_BUFFER_SIZE];
    char receive_buffer[SOCKET_BUFFER_SIZE];
    const string ip="localhost";
    int port =VT_CONTROLLER_PORT;
    host1 = gethostbyname(ip.c_str());
    bzero(send_buffer,SOCKET_BUFFER_SIZE);
    bzero(receive_buffer,SOCKET_BUFFER_SIZE);
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
         printf("Socket creation failed!");
         exit(1);
    }

    server_addr1.sin_family = AF_INET;
    server_addr1.sin_port = htons(port);
    server_addr1.sin_addr = *((struct in_addr *)host1->h_addr);
    bzero(&(server_addr1.sin_zero),8);
     
    if (connect(sock, (struct sockaddr *)&server_addr1,
                    sizeof(struct sockaddr)) == -1)
    {
         cout<<"connect with virtual control failed!"<<endl;
         exit(1);
    }
    strcpy(send_buffer,"shutdown vt session:\n");
    if(send(sock,send_buffer,strlen(send_buffer), 0)!=-1){
         printf("vt session has been shutdown!\n");
    }
    close(sock);
}