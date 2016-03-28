#include "GridLabD.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/ioctl.h>
double getPropertyValue(char* propertyName){
    double result;
    char* queryHost=GRIDLABD_SERVER_QUERY;
    char queryObject[ARRAY_SIZE];
    char* querySuffix=" -q -O - |xmllint --xpath '/property/value/text()' -";
    char query[ARRAY_SIZE];
    char resultArray[ARRAY_SIZE];
    FILE* fp;
    if(strlen(gridlabdName)==0){
        getGridlabdName();
        strcpy(queryObject,gridlabdName);
    }
    else{
        strcpy(queryObject,gridlabdName);
    }
    strcpy(query,queryHost);
    strcat(query,queryObject);
    strcat(query,"/");
    strcat(query,propertyName);
    strcat(query,querySuffix);
     printf("%s",query);
    fp = popen(query, "r");
    if(fp!=NULL){
     fgets(resultArray,ARRAY_SIZE,fp);
     result=atof(resultArray);
    }
    else{
        result=-1;
    }
    return result;
} 

void setPropertyValue(char* propertyName, double value){
    char* queryHost=GRIDLABD_SERVER_QUERY;
    char queryObject[ARRAY_SIZE];
    
    char* querySuffix=" -q";
    char query[ARRAY_SIZE];
    char valueArray[ARRAY_SIZE];
    
    if(strlen(gridlabdName)==0){
        getGridlabdName();
        strcpy(queryObject,gridlabdName);
    }
   else{
        strcpy(queryObject,gridlabdName);
    }
   
    strcpy(query,queryHost);
    strcat(query,queryObject);
    strcat(query,"/");
    sprintf(valueArray,"%f",value);
    strcat(query,propertyName);
    strcat(query,"=");
    strcat(query,valueArray);
    strcat(query,querySuffix);
    printf("%s",query);
    system(query);
    
}

void getGridlabdName(){
    char nodeName[ARRAY_SIZE];
    struct utsname uts;   
    FILE* fp;
    char resultArray[ARRAY_SIZE];
    uname(&uts);
    strcpy(nodeName, uts.nodename);
    char query[ARRAY_SIZE];
    strcpy(query,"awk '$1==\"");
    strcat(query,nodeName);
    strcat(query,"\"{ print $2 }'");
    strcat(query," ");
    strcat(query,SCOREPLUS_SESSION);
    fp = popen(query, "r");
    if(fp!=NULL){
     fgets(resultArray,ARRAY_SIZE,fp);
    }
    strncpy(gridlabdName,resultArray,strlen(resultArray)-1);
}
