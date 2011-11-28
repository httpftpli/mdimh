/*#ifndef PATTERNDATA_H
#define PATTERNDATA_H
#include<QFile>


class ParttenData{
#pragma pack(1)
public:
  struct _data{
  unsigned short Frontline1;
  unsigned char FrontAAction1;
  unsigned short FrontAColor1;
  unsigned char pading;
  unsigned char FrontHAction1;
  unsigned short  FrontHColor1;
  unsigned char pading1;

  unsigned short Backline1;
  unsigned char BackAAction1;
  unsigned short BackAColor1;
  unsigned char pading2;
  unsigned char BackHAction1;
  unsigned short BackHColor1;
  unsigned char pading3;         ///////19

  unsigned short Frontline2;     ///////20
  unsigned char FrontAAction2;
  unsigned short FrontAColor2;
  unsigned char pading4;
  unsigned char FrontHAction2;
  unsigned short  FrontHColor2;
  unsigned char pading5;

  unsigned short Backline2;
  unsigned char BackAAction2;
  unsigned short BackAColor2;
  unsigned char pading6;
  unsigned char BackHAction2;
  unsigned short BackHColor2;
  unsigned char pading7;         ///////39

  unsigned char padarray[40];

  unsigned short muslin1;       //80
  unsigned char pading8;
  unsigned short muslin2;
  unsigned char pading9;

  unsigned char padarray1[6];

  unsigned char dumo;
  unsigned char speed;
  unsigned char roll;
  unsigned char assist_roll;
  unsigned char assist_roll_open;
  unsigned char parttenalign;
  unsigned char subprogram;
  unsigned char stop;          //99

  unsigned char beddirection;
  unsigned char bedval;
  unsigned char padarray2[5];
  unsigned char muslinstop_position;
  unsigned char backoffset;
  unsigned char padarray3[4];
  unsigned char endofpage;
  unsigned short loopstart;
  unsigned short loopNum;   //
  unsigned char padarray4[6];
  unsigned char yajiaoduan;   //127
} data;
#pragma pack()

  void SetFAction1_AposA(unsigned char);//////////
  void SetFAction1_HposA(unsigned char);/////////////
  void SetFAction2_AposA(unsigned char);////////////
  void SetFAction2_HposA(unsigned char);//////////
  void SetBAction1_AposA(unsigned char);///////////
  void SetBAction1_HposA(unsigned char);//////////
  void SetBAction2_AposA(unsigned char);////////////
  void SetBAction2_HposA(unsigned char);/////////////
  void SetMuslin1(unsigned char val);
  void SetMuslin2(unsigned char val);
  unsigned char Muslin1();
  unsigned char Muslin2();
  void SetDumo(unsigned char val);
  void SetSpeed(unsigned char val);
  void SetRoll(unsigned char val);
  void SetAssistRoll(unsigned char val);
  void SetBed();///////////////////////////////////////
  void SetMuslinStop(unsigned char val);
  void SetFile()

private:

  unsigned char action_Apos;
  unsigned char action_Hpos;
  unsigned char  updataFrontAAction1();
  QFile *cntfile;

};

#endif // PATTERNDATA_H
*/
