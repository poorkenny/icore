/* 
 * File:   util.h
 * Author: stan
 *
 * Created on July 4, 2013, 2:00 PM
 */
#ifndef UTIL_H
#define	UTIL_H

#define MAXIMUM_PACKET_SIZE 1024
#define TO_CONSOLE 0
#define TO_LOGFILE 1
#define LOGFILE "/tmp/SyncController.log"
#define OUT_FILE "/tmp/SyncController.output"
#define LOGFLAG TO_LOGFILE
#define SOCKET_BUFFER_SIZE 1024
#include<fstream>
using namespace std;
class LogFile{
public:
    ofstream log_file;
    LogFile(){
        log_file.open(LOGFILE);
    }
    ~LogFile(){
        log_file.close();
    }
};
class util{
private:
    static LogFile logFile;
public:
/*
 * daemonize the whole program
 */
static void daemonize(void);

/*
 * send large amount of data through sock.
 */
static void send_data(int sock,string data,int max_packet_size);
/*
 * receive large amount of data.
 */
static long int receive_data(int sock, char* receive_buffer,long int buffer_size, int max_packet_size);
/*
 * send data to power slave.
 */
static void log(string log);

};

#endif	/* UTIL_H */


