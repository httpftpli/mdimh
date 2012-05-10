#ifndef CONSTDATA_H
#define CONSTDATA_H

enum WrkAddr{
    WrkAddr_Base = 0x0000, //铁电存储器中参数的起始地址
    WrkAddr_GongZuo = WrkAddr_Base+0x0, //(未用)
    WrkAddr_DuMuZi = WrkAddr_Base+0x000, //Bag2 //共192个  :度目值24档
    WrkAddr_SuDuZi = WrkAddr_Base+0x180, //Bag1 //共24个  :速度24档
    WrkAddr_ShaZuiTF = WrkAddr_Base+0x200, //Bag3 //共128个:纱嘴停放位置
    WrkAddr_JuanBu = WrkAddr_Base+0x300, //Bag1 //共24个  :步进卷布24档
    WrkAddr_JuanBuFZ = WrkAddr_Base+0x350 ,//Bag2 //共24个  :辅助卷布24档
    WrkAddr_LeftSongSa = WrkAddr_Base+0x400, //Bag5 //共24个  :左送纱
    WrkAddr_RightSongSa = WrkAddr_Base+0x420, //Bag5 //共24个  :右送纱
    WrkAddr_YTXTingFang = WrkAddr_Base+0x450, //Bag6 //共16个  :引塔夏纱嘴停放
    WrkAddr_YTXXiuZen = WrkAddr_Base+0x500, //Bag6 //共192个:引塔夏纱嘴修正
    WrkAddr_CJP_FanZen = WrkAddr_Base+0x600, //Bag5 //共48个  :后,前沉降片翻针位段数
    WrkAddr_CJP_BianZi = WrkAddr_Base+0x640, //Bag5 //共48个  :后,前沉降片编织位段数
    WrkAddr_PzkSaZui = WrkAddr_Base+0x680, //Bag6 //共64个  :片展开纱嘴对应表
    WrkAddr_PzkClose = WrkAddr_Base+0x6E0, //Bag6 //共1个    :片展开关闭状态(1表示关闭)
    WrkAddr_ZanKaiPianSu =WrkAddr_Base+0x6F0, //Bag1 // 1个         :展开片数
    WrkAddr_QiZenDian = WrkAddr_Base+0x6F5   //Bag1 //机器工作参数中的起针点
};

enum   SpaAddr{
 SpaAddr_Base  = 0,
 SpaAddr_XiTong  = SpaAddr_Base+0 ,         //Bag4 //共24个:系统参数起始地址(有密码)
 SpaAddr_DanMianDMBC =SpaAddr_Base+0x50,    //Bag4 //共8个  :单面度目补偿[0]后左,[1]后右,[2]前左,[3]前右
 SpaAddr_SiPinDMBC=SpaAddr_Base+0x160,    //Bag4 //共8个  :四平度目补偿[0]后左,[1]后右,[2]前左,[3]前右
 SpaAddr_LingWeiDMBC= SpaAddr_Base+0x170,  //Bag4 //共8个  :不工作度目零位补偿[0]后左,[1]后右,[2]前左,[3]前右
 SpaAddr_YaoCuang= SpaAddr_Base+0x180,    //Bag4 //共33个:摇床位置修正,33个
 SpaAddr_FanZenYC =SpaAddr_Base+0x200,   //Bag4 //共16个:翻针摇床位置修正
 SpaAddr_FanZenZYC =SpaAddr_Base+0x220,   //Bag4 //共16个:+翻针摇床位置修正
 SpaAddr_FanZenFYC= SpaAddr_Base+0x240 ,   //Bag4 //共16个:-翻针摇床位置修正
 SpaAddr_JiQiGongZuo=SpaAddr_Base+0x260  //Bag1 //共34个:机器工作参数起始地址
};

#endif // CONSTDATA_H
