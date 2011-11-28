
#include<patterndata.h>

 void ParttenData::SetMuslin1(unsigned char val){
     if(val == 0)
         data.muslin1 = 0x0081;
     else
         data.muslin1 = ((unsigned short) val<<8) +0x80;

 }

 void ParttenData::SetDumo(unsigned char val){
     if(val>24) val=24;
     if(val<1) val =1;
       data.dumo = val;
 }

 void ParttenData::SetSpeed(unsigned char val){
     if(val>24) val=24;
     if(val<1) val =1;
     data.speed = val;
 }

 void ParttenData::SetRoll(unsigned char val){
     if(val>24) val=24;
     if(val<1) val =1;
     data.roll = val;
 }

 void ParttenData::SetAssistRoll(unsigned char val){
     if(val>24) val=24;
     if(val<1) val =1;
     data.assist_roll = val;
 }

 void ParttenData::SetMuslinStop(unsigned char val){
     if(val>8) val=8;
     if(val<1) val =1;
     data.muslinstop_position = val;
 }


