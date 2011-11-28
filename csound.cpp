#include "csound.h"
#include <QtCore>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/soundcard.h>


cSound::cSound(const QString &filename ):datasize(0)
{
    setFile(filename);
}

cSound::cSound():datasize(0)
{

}

void cSound::setFile(const QString &filename){
    datasize = 0;
    file.clear();
    QSharedPointer<QFile> file = QSharedPointer<QFile>(new QFile(filename));
    file->open(QIODevice::ReadOnly);
    struct Riff_Header riff;
    struct Wave_Format format;
    if(file->read((char *)&riff,sizeof riff)<sizeof riff)
        return;
    if((riff.szRiffFormat[0]!='W')||(riff.szRiffFormat[1]!='A')||
       (riff.szRiffFormat[2]!='V')||(riff.szRiffFormat[3]!='E'))
        return;
    file->seek(file->pos()+4);
    int size;
    file->read((char *)&size,4);
    if(file->read((char *)&format,size)<size)
        return;
    channels =  format.wChannels;
    samplerate = format.dwSamplesPerSec;
    samplebit = format.wBitsPerSample;
    bytespersec = format.dwSamplesPerSec;
    blockalign = format.wBlockAlign;
    while(1){
        unsigned char datatag[4];
        file->read((char *)datatag,4);
        if((datatag[0]=='d')&&(datatag[1]=='a')&&
           (datatag[2]=='t')&&(datatag[3]=='a')){
            break;
        }else{
            unsigned int len;
            file->read((char *)&len,4);
            if(len==0)
                return;
            file->seek(file->pos()+len);
        }
        if(file->atEnd())
            return;
    }
    file->read((char *)&size,4);
    datasize = size;
    if(size){
        this->file = file;
        dataoffset = file->pos();
    }
    return;
}

void cSound::writesound(){
    int handle = open("/dev/dsp", O_WRONLY);
    if(handle<0){
        close(handle);
        return;
    }
    /*int setting = (0x7fff<<16)|16; //buff size 65k;
    int result = ioctl(handle, SNDCTL_DSP_SETFRAGMENT, &setting);
    if (result <0){
        close(handle);
        return;
    }*/

    int channels  = this->channels-1; // 0=mono 1=stereo
    int result = ioctl(handle, SNDCTL_DSP_STEREO, &channels);
    if ( result < 0 ){
        close(handle);
        return;
    }
    int format;
    if(samplebit==8)
        format = AFMT_U8;
    else
        format = AFMT_S16_BE;
    result = ioctl(handle, SNDCTL_DSP_SETFMT, &format);
    if ( result < 0){
        close(handle);
        return;
    }
    int rate = samplerate;
    result = ioctl(handle, SNDCTL_DSP_SPEED, &rate);
    if ( result< 0 ){
        close(handle);
        return;
    }
    int vol=100;
    ioctl(handle, SOUND_MIXER_VOLUME, &vol);

    if(datasize>65536)
        datasize = 65536;
    file->seek(dataoffset);
    char *buf = new char[datasize];
    int len = file->read(buf,datasize);
    write(handle,buf,len);
    close(handle);
    delete [] buf;
}

void cSound::play(){
    if(datasize>0)
        writesound();
}
