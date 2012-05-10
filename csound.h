#ifndef CSOUND_H
#define CSOUND_H
#include<QString>
#include<QSharedPointer>
#include<QTimer>

class QFile;

#pragma pack(1)
struct Riff_Header
 {
    unsigned char szRiffID[4];  // 'R','I','F','F'
    unsigned int  dwRiffSize;
    unsigned char szRiffFormat[4]; // 'W','A','V','E'
};
#pragma pack()

#pragma pack(1)
struct Wave_Format
{
    unsigned short   wFormatTag;
    unsigned short   wChannels;
    unsigned int     dwSamplesPerSec;
    unsigned int     dwAvgBytesPerSec;
    unsigned short   wBlockAlign;
    unsigned short   wBitsPerSample;
    unsigned short   pack;        //附加信息
};
#pragma pack()


class cSound
{
public:
    cSound(const QString &filename);
    cSound();
    void setFile(const QString &filename);
    void play();
private:
    QSharedPointer<QFile> file;
    unsigned int datasize;
    unsigned int dataoffset;
    unsigned int channels;
    unsigned int samplerate;
    unsigned int samplebit;
    unsigned int bytespersec;
    unsigned int blockalign;
    void writesound();
    QTimer timer;
};

#endif // CSOUND_H
