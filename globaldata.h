#ifndef GLOBALDATA_H
#define GLOBALDATA_H

#include <qsyslog.h>
#include <namespace.h>
#define DUAL_SYSTEM 0
//#define SYSTEST

class QHMIData;
class QSend;
class QRcv;
class QMutex;
class QPatternData;
class QParam;
class QSysLog;
class QProgressIndicator;
extern QHMIData hmiData;
extern QSend qSend;
extern QRcv  qRcv;      // qRcv is defined in glabaldata.h file , inited in mainwindow construct
extern QMutex mutex,interComMutex;
extern QPatternData patternData;  //defiled in glabaldata.cpp file
extern QParam paramaData;    //defiled in glabaldata.cpp file
extern QSysLog sysLog;
extern QProgressIndicator *ProgressIndiForm;

#define WARNLOG(A) sysLog.warnLog(A)
#define INFORMLOG(A) sysLog.informLog(A)
#define ERRORLOG(A) sysLog.errorLog(A)

extern Md::InitResult sysInit();
extern QString secondToString(int sec);

#endif // GLOBALDATA_H
