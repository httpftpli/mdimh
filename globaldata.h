#ifndef GLOBALDATA_H
#define GLOBALDATA_H

#include <qsyslog.h>
#include <namespace.h>
//#define SYSTEST

class QHMIData;
class QComm;
class QPattern;
class QParam;
class QSysLog;
class QProgressIndicator;
extern QHMIData hmiData;
extern QComm qComm;
extern QPattern patternData;  //defiled in glabaldata.cpp file
extern QParam paramaData;    //defiled in glabaldata.cpp file
extern QSysLog sysLog;
extern QProgressIndicator *ProgressIndiForm;

#define WARNLOG(A) sysLog.warnLog(A)
#define INFORMLOG(A) sysLog.informLog(A)
#define ERRORLOG(A) sysLog.errorLog(A)
#define RUN_ASSERT(VAL,CONDITION)  do{if(!(CONDITION)){ qWarning() \
    <<"RUN_ASSERT:  "#CONDITION"," <<#VAL"="<<index<<" ,in file "\
    <<__FILE__<<",in line"<<__LINE__;}}\
    while(0)
extern Md::Result sysInit();
extern QString secondToString(int sec);
extern void MessageOutput(QtMsgType type, const char *msg);


#endif // GLOBALDATA_H
