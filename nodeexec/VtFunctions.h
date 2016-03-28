/* 
 * File:   vt_functions.h
 * Author: stan
 *
 * Created on June 4, 2013, 3:27 PM
 */

#ifndef VT_FUNCTIONS_H
#define	VT_FUNCTIONS_H
#include <sys/time.h>
#ifdef	__cplusplus
extern "C" {
#endif

    void vt_sleep(int s);
    void vt_gettimeofday(struct timeval* tv,struct timezone *tz);
    
#define VT_CONTROL_MONITOR_PORT_BASE 10000
#define SOCKET_BUFFER_SIZE 1024
#define ACTUAL_SLEEP_TIME 1 //best choice as 1, the larger, the more accurate, but slower
#define TIMESLICE 0.5
#define CORE_LOCALHOST_IP "172.16.0.254"
#define V_ARRAY_SIZE 100
#ifdef	__cplusplus
}
#endif

#endif	/* VT_FUNCTIONS_H */

