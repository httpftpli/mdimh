#ifndef GLOBALDATA_H
#define GLOBALDATA_H

#include <qsyslog.h>
#include <namespace.h>
#define DUAL_SYSTEM 1
//#define SYSTEST

class QHMIData;
class QComm;
class QPatternData;
class QParam;
class QSysLog;
class QProgressIndicator;
extern QHMIData hmiData;
extern QComm qComm;
extern QPatternData patternData;  //defiled in glabaldata.cpp file
extern QParam paramaData;    //defiled in glabaldata.cpp file
extern QSysLog sysLog;
extern QProgressIndicator *ProgressIndiForm;

#define WARNLOG(A) sysLog.warnLog(A)
#define INFORMLOG(A) sysLog.informLog(A)
#define ERRORLOG(A) sysLog.errorLog(A)

extern Md::Result sysInit();
extern QString secondToString(int sec);

#endif // GLOBALDATA_H
