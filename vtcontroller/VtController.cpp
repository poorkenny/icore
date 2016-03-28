#include "VtController.h"

VtController::VtController(){
    
}

void VtController::init(char* input){
    string data(input);
    vnode_number=atoi(data.c_str());
    vnodes=new vnode[vnode_number];
    for(int i=0;i<vnode_number;i++){
        vnodes[i].barrier=0;
        vnodes[i].id=i+1;
        vnodes[i].offset=0;
        vnodes[i].clock=0;
        vnodes[i].is_complete=0;
        vnodes[i].stop_t=0;
        vnodes[i].is_sleep=false;
        vnodes[i].sleep_time=0;
        vnodes[i].monitor_port=MONITOR_PORT_BASE+i+1;
        vnodes[i].is_runnable=true;
        pthread_mutex_init(&vnodes[i].mutex, NULL);
        pthread_create(&vnodes[i].monitor_thread, 0, monitor_vn, (void *)&vnodes[i]); 
        pthread_detach(vnodes[i].monitor_thread);
    }
}

void VtController::proceed(string data){
    int sync_win=atoi(data.c_str());
    int i;
    pthread_t thread[vnode_number];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    void *status;
    for(i=0;i<vnode_number;i++){
        vnodes[i].barrier=vnodes[i].barrier+sync_win;
        vnodes[i].is_complete=false;
        vnodes[i].stop_t=vnodes[i].barrier-vnodes[i].offset;
        if((vnodes[i].stop_t-vnodes[i].clock)<=vnodes[i].sleep_time){   
            vnodes[i].sleep_time=vnodes[i].sleep_time-(vnodes[i].stop_t-vnodes[i].clock);
            vnodes[i].clock=vnodes[i].barrier;
            vnodes[i].offset=0;
            vnodes[i].is_runnable=false;
        }
        else{
            vnodes[i].clock+=vnodes[i].sleep_time;
            vnodes[i].sleep_time=0;
            vnodes[i].is_runnable=true;
            vnodes[i].is_sleep=false;
        }
    }
    
    
    for(i=0; i<vnode_number; i++) {
        if(vnodes[i].is_runnable){
      printf("Vt control Main: creating thread %d\n", i);
      pthread_create(&thread[i], &attr, control_vn, (void *)&vnodes[i]); 
        }
   //  pthread_create(&thread[i], &attr, test1, (void *)&vnodes[i]); 
    }

   /* Free attribute and wait for the other threads */
    pthread_attr_destroy(&attr);
    for(i=0; i<vnode_number; i++) {
        
     if(vnodes[i].is_runnable){
        
      if(pthread_join(thread[i], &status)==0){
         printf("Main: thread %d has finished with clock %f, offset %f, remaining sleep_time %f\n",i,vnodes[i].clock,vnodes[i].offset,vnodes[i].sleep_time);
      }
      else{
          printf("Main: thread %d join error!\n",i);
      }
        }
    }
 
      printf("Main: Vt control completed.\n");
      return;
}
VtController::~VtController(){
    
    for(int i=0;i<vnode_number;i++){
      pthread_mutex_destroy(&vnodes[i].mutex);
      pthread_cancel(vnodes[i].monitor_thread);
    }
    delete[] vnodes;
    
}