/* 
 * File:   SyncController.h
 * Author: stan
 *
 * Created on July 4, 2013, 1:42 PM
 */

#ifndef SYNCCONTROLLER_H
#define	SYNCCONTROLLER_H
#include "Util.h"
#define SYNC_CONTROLLER_PORT 9019
#define VT_CONTROLLER_PORT 9009
using namespace std;
class SyncController{
private:
    string virtualStartTime;
    string virtualStopTime;
    int vnodeNumber;
// all are unix time stamp, which means the time zone is UCT. 
// The input string is treated as local time zone, can use function localtime to convert to 
// readable local time.
    unsigned long int virtualStartUnixTime;
    unsigned long int virtualStopUnixTime;
    unsigned long int virtualCurrentUnixTime;
    struct timeval  realStartTime, realStopTime;
    void shutDownVtController();
public:
    SyncController();
    void init(char* input);
    void proceed(string data);
    string queryGridlabdName(string coreId);
    unsigned long int convertToUnixTime(string t);
    void shutDownSession();
    ~SyncController();
};

#endif	/* SYNCCONTROLLER_H */

