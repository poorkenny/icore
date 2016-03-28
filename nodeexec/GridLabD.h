/* 
 * File:   gridlabd.h
 * Author: stan
 *
 * Created on July 8, 2013, 9:27 AM
 */

#ifndef GRIDLABD_H
#define	GRIDLABD_H
double getPropertyValue(char* propertyName);
void setPropertyValue(char* propertyName, double value);
char gridlabdName[100];
void getGridlabdName();
#define GRIDLABD_SERVER_QUERY "wget http://172.16.0.254:20000/"
#define SCOREPLUS_SESSION "/tmp/scoreplus.session"
#define ARRAY_SIZE 200
#endif	/* GRIDLABD_H */

