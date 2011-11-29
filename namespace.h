#ifndef NAMESPACE_H
#define NAMESPACE_H

#include<QFlags>

namespace Md{
    enum HAVEFILE {
             HAVENO  = 0x0,
             HAVECNT = 0x1,
             HAVEPAT = 0x2,
             HAVEPRM = 0X4,
             HAVEWRK = 0x8,
             HAVESAZ = 0x10,
             HAVEALL = 0xff
         };
    Q_DECLARE_FLAGS(HAVEFILEFLAG, HAVEFILE)

    Q_DECLARE_OPERATORS_FOR_FLAGS(Md::HAVEFILEFLAG)

    enum DIRECTION{
        RIGHT = 0x00,
        LEFT = 0x01
    };

    enum Result{
        Ok = 0x00,
        CommError = 0x01 ,
        PatternError = 0x10,
        ParamaError = 0x20,
        CustomerIdNotPass = 0x40,
        MainBoardNoRom = 0x80,
        WaitingForShoutDown = 0x100,
        BagCanNotInBoot = 0x200,
        FileOpenFail = 0x400,
        InBootState = 0x800,
        InUserState = 0x1000,
        EraseError = 0x2000,
        BurnError = 0x4000,
        BurnOK = 0x8000,
        FileSame = 0x10000,
        NotPatCntSaz = 0x20000,
        FileNotExist = 0x40000
    };
    enum POS{FRONTLEFT= 0,FORNTRIGHT,REARLEFT,REARRIGHT};


}

#endif // NAMESPACE_H
