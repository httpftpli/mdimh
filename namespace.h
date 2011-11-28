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

    enum InitResult{
        CommError ,
        PatternError,
        ParamaError,
        CustomerIdError,
        MainBoardNoRom,
        OK
    };
    enum POS{FRONTLEFT= 0,FORNTRIGHT,REARLEFT,REARRIGHT};


}

#endif // NAMESPACE_H
