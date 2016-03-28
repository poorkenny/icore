#include "VtFunctions.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <sys/utsname.h>
void vt_sleep(int s){
     int sock;
     struct hostent *host1;
     struct sockaddr_in server_addr1;
     char send_buffer[SOCKET_BUFFER_SIZE];
     char receive_buffer[SOCKET_BUFFER_SIZE];
     char sleep_time[V_ARRAY_SIZE];
     char *ip=CORE_LOCALHOST_IP;
     char node_name[V_ARRAY_SIZE];
     struct utsname uts;
     int node_id;     
     
    
     uname(&uts);
     strcpy(node_name, uts.nodename);
     printf("%s\n",node_name);
     node_id=atoi(node_name+1);
     
     
     bzero(send_buffer,SOCKET_BUFFER_SIZE);
     bzero(receive_buffer,SOCKET_BUFFER_SIZE);
     bzero(sleep_time,V_ARRAY_SIZE);
     host1 = gethostbyname(ip);
     if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
         printf("Socket creation failed!");
         exit(1);
     }

     server_addr1.sin_family = AF_INET;
     server_addr1.sin_port = htons(VT_CONTROL_MONITOR_PORT_BASE+node_id);
     server_addr1.sin_addr = *((struct in_addr *)host1->h_addr);
     bzero(&(server_addr1.sin_zero),8);

loop:     
     while (connect(sock, (struct sockaddr *)&server_addr1,
                    sizeof(struct sockaddr)) == -1)
     {
        printf("Connect with Virtual time control failed!");
        usleep(100000);
           
     }
     strcpy(send_buffer,"vt_sleep:");
     strcat(send_buffer,node_name);
     strcat(send_buffer,":");
     sprintf(sleep_time,"%f\n",s-TIMESLICE);
     strcat(send_buffer,sleep_time);
     send(sock,send_buffer,strlen(send_buffer), 0);
     shutdown(sock, SHUT_WR); 
     read(sock,receive_buffer,SOCKET_BUFFER_SIZE);
     close(sock);
     if(strcmp(receive_buffer,"success")==0){
      sleep(ACTUAL_SLEEP_TIME);
       return;
     }
     else{
         goto loop;
     }
      
}
void vt_gettimeofday(struct timeval* tv,struct timezone *tz){
     int sock;
     struct hostent *host1;
     struct sockaddr_in server_addr1;
     char send_buffer[SOCKET_BUFFER_SIZE];
     char receive_buffer[SOCKET_BUFFER_SIZE];
     char *ip=CORE_LOCALHOST_IP;
     double clock_time;
     
     char node_name[V_ARRAY_SIZE];
     int node_id;
     struct utsname uts;
     uname(&uts);
     strcpy(node_name, uts.nodename);
     
     node_id=atoi(node_name+1);
     bzero(send_buffer,SOCKET_BUFFER_SIZE);
     bzero(receive_buffer,SOCKET_BUFFER_SIZE);
     
     host1 = gethostbyname(ip);
     if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
         printf("Socket creation failed!");
         exit(1);
     }

     server_addr1.sin_family = AF_INET;
     server_addr1.sin_port = htons(VT_CONTROL_MONITOR_PORT_BASE+node_id);
     server_addr1.sin_addr = *((struct in_addr *)host1->h_addr);
     bzero(&(server_addr1.sin_zero),8);

     while (connect(sock, (struct sockaddr *)&server_addr1,
                    sizeof(struct sockaddr)) == -1)
     {
         printf("Connect with Virtual time control failed!");
         usleep(100000);
           
     }
     strcpy(send_buffer,"gettimeofday:");
     strcat(send_buffer,node_name);
     strcat(send_buffer,"\n");
     send(sock,send_buffer,strlen(send_buffer), 0);
     shutdown(sock, SHUT_WR); 
     read(sock,receive_buffer,SOCKET_BUFFER_SIZE);
     clock_time=atof(receive_buffer);
     close(sock);
    tv->tv_sec=(int)clock_time;
    tv->tv_usec=(clock_time-(int)clock_time)*1000000;
    
}