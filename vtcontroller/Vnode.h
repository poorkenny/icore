/* 
 * File:   vnode.h
 * Author: stan
 *
 * Created on June 14, 2013, 8:10 AM
 */

#ifndef VNODE_H
#define	VNODE_H
#include<pthread.h>
  struct vnode{
        int id;
        int barrier;
        bool is_complete;
        double stop_t;
        double clock;
        double offset;
        bool is_sleep;
        double sleep_time;
        pthread_t monitor_thread;
        pthread_mutex_t mutex;
        int monitor_port;
        bool is_runnable;
    };
void *control_vn(void *t);
void* monitor_vn(void* param);
void unfreeze_vn(int id);
void freeze_vn(int id);
void cleanup_handler(void*);
void* test1(void *t);
static int cleanup_pop_arg = 0;
#define TIMESLICE 500000 //the best choice.
#define VT_PROXY_PORT 9010
#define MONITOR_PORT_BASE 10000


#endif	/* VNODE_H */

