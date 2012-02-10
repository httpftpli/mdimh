

#include"data.h"
#include"communicat.h"
#include "globaldata.h"
#include<QSize>
#include <QPainter>
#include<QLineEdit>
#include"qmdmessagebox.h"
#include<QFileDialog>
#include<QCoreApplication>
#include<QDataStream>
#include<QDateTime>
#include<QFileInfo>
#include<QFileInfoList>
#include "pattern.h"
#include "qparam.h"
#include<QApplication>
#include<QFileSystemModel>


const char  ProgramEncrypt::EncryptTable[256]={0xD5,0xFD,0xC3,0xB6,0xBE,0xD9,0x55,0x53,
                           0x42,0xB4,0xC5,0xC5,0xCC,0x2E,0x2E,0x2E,
                           0x55,0x53,0x42,0x20,0x45,0x6E,0x75,0x6D,
                           0x65,0x72,0x61,0x74,0x65,0x64,0x2E,0x3E,
                           0xC3,0xB6,0xBE,0xD9,0xC9,0xE8,0xB1,0xB8,
                           0xCA,0xA7,0xB0,0xDC,0x21,0x21,0x21,0xc0,
                           0x45,0x6E,0x75,0x6D,0x20,0x66,0x61,0x69,
                           0x6C,0x75,0x72,0x65,0x20,0x21,0x01,0x27,
                           0xD5,0xFD,0xD4,0xDA,0xD0,0xB4,0xC8,0xEB,
                           0xCE,0xC4,0xBC,0xFE,0x21,0x21,0x21,0x70,
                           0x50,0x6C,0x65,0x61,0x73,0x65,0x20,0x57,
                           0x61,0x69,0x74,0x69,0x6E,0x67,0x2E,0x2E,
                           0x43,0x68,0x69,0x70,0x20,0x72,0x65,0x73,
                           0x65,0x74,0x20,0x4F,0x4B,0x21,0x20,0x00,
                           0x52,0x65,0x73,0x65,0x74,0x20,0x63,0x68,
                           0x69,0x70,0x20,0x45,0x72,0x72,0x6F,0x72,
                           0x52,0xd3,0x44,0xD0,0xBE,0xC6,0xAC,0xB8,
                           0xB4,0xCE,0xB3,0xB3,0xC9,0xB9,0xA6,0xf1,
                           0x54,0xc3,0x46,0xDb,0xBE,0xC3,0xA2,0xB2,
                           0xB4,0xC3,0xBB,0xca,0xa7,0xB0,0xdc,0x31,
                           0x35,0xb3,0x48,0xDc,0xBE,0xC5,0xA3,0xC9,
                           0xE8,0xD6,0xC3,0xCA,0xA7,0xB0,0xDC,0x71,
                           0x85,0xa3,0x42,0xB4,0xC5,0xC5,0xCC,0xC9,
                           0xE8,0xD6,0xC3,0xCA,0xA7,0xB0,0xDC,0xc1,
                           0xD6,0xB8,0xC1,0xEE,0xC3,0xBB,0xD3,0xD0,
                           0xD5,0xFD,0xB3,0xA3,0xCA,0xE4,0xB3,0xF6,
                           0x55,0x79,0xab,0xDb,0x3E,0xC7,0x89,0x34,
                           0x34,0x11,0x35,0x90,0xfd,0xf3,0xf3,0xf9,
                           0x35,0xb3,0x48,0xDc,0xBE,0xC5,0xA3,0xbb,
                           0xc3,0x43,0xC3,0xCA,0xA7,0x34,0x1C,0x28,
                           0x16,0x55,0x11,0xcE,0xC3,0xB7,0xD3,0xD0,
                           0xD5,0xFD,0xB3,0xA3,0x33,0x45,0x34,0x36
                       };



void ProgramEncrypt::Decrypt(char *buf,unsigned short len){
    if(len!=512)
        return;
    for(int i=0;i<256;i++)
        buf[i]^=EncryptTable[i];
    for(int i=256;i<len;i++)
        buf[i]^=EncryptTable[i-256];
}






QPatModel::QPatModel( QPatternData *pattern,QObject * parent):
                        QAbstractTableModel(parent),pattern(pattern),bits10(0){
        int column = pattern->tatalcolumn;
        while(column){
            bits10++;
            column = column/10;
        }
}

int QPatModel::rowCount(const QModelIndex &parent)const {
    Q_UNUSED(parent)
    return pattern->tatalpatrow;

}



int QPatModel::columnCount(const QModelIndex &parent)const{
    Q_UNUSED(parent)
    return pattern->tatalcolumn;

}

QVariant QPatModel::headerData(int section, Qt::Orientation orientation,int role )const{
    if(role==Qt::DisplayRole){
        if(Qt::Horizontal==orientation){
            int temp = section+1;
            QString str=QString(temp%10+'0');
            temp = temp/10;
            for(int i=0;i<bits10-1;i++){
                str.prepend("\n").prepend(temp%10+'0');
                temp = temp/10;
            }
            return str;
        }
        else
            return pattern->tatalcntrow-section;
    }
    return QVariant();
}

QVariant QPatModel::data(const QModelIndex &index, int role) const{
    int r= pattern->tatalpatrow;
    int c = pattern->tatalcolumn;
    if(role==Qt::TextAlignmentRole){
        return int(Qt::AlignHCenter);
    }else if(role == Qt::DisplayRole){
        int row = index.row();
        int column = index.column();
        char data = pattern->patbuf[((r-row-1)*(c/2+c%2))+column/2];
        char ch=  (column%2)?(data&0x0f):((unsigned char)data>>4);
        if((ch>=0)&&(ch<=9)){
            ch = ch+'0';
        }
        else if((ch>=0x0a)&&(ch<=0x0f)){
            ch = ch-10+'A';
        }
        return QString(ch);
    }else
        return QVariant();
}

bool QPatModel::setData(const QModelIndex &index, const QVariant &value, int role){
    if((role ==Qt::EditRole)&&(value.isValid())){
        int r= pattern->tatalpatrow;
        int row = index.row();
        int column = index.column();
        QString str = value.toString();
        if(str.isEmpty())
            return FALSE;
        char cha = value.toString().at(0).toAscii();
        if((cha<='9')&&(cha>='0')){
            cha = cha-'0';
        }else if((cha<='f')&&(cha>='a')){
            cha = cha-'a'+10;
        }else if((cha<='F')&&(cha>='A')){
            cha = cha-'A'+10;
        }else{
            return FALSE;
        }
        pattern->patSetData(r-row-1,column,cha);
        return TRUE;
    }
    return FALSE;
}


Qt::ItemFlags QPatModel::flags(const QModelIndex &index) const{
    return QAbstractTableModel::flags(index)|Qt::ItemIsEditable;
}


bool QPatModel::resetBuf(){
    pattern->refreshBuf(Md::HAVEPAT);
    reset();
}




/////////////////////////QMdItemDelegate//////////////////////////////////////////////



bool QMdItemDelegate::fechValidatorstruct(ValidatorStruct &val,int row,int column)const{
    QPair<int,int> rc;
    rc.first = row;
    rc.second = column;
    if(validatorhash.contains(rc)){
        val =  validatorhash[rc];
        return TRUE;
    }
    rc.first = row;
    rc.second = -1;
    if(validatorhash.contains(rc)){
        val =  validatorhash[rc];
        return TRUE;
    }
    rc.first = -1;
    rc.second =column;
    if(validatorhash.contains(rc)){
        val =  validatorhash[rc];
        return TRUE;
    }
    rc.first = -1;
    rc.second = -1;
    if(validatorhash.contains(rc)){
        val =  validatorhash[rc];
        return TRUE;
    }
    return FALSE;
}

void QMdItemDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const{
    if(isitemsimplepaint){
        QString str = index.data(Qt::DisplayRole).toString();
        painter->drawText(option.rect,Qt::TextWordWrap,str);
    }else
        QStyledItemDelegate::paint(painter,option,index);
}
void QMdItemDelegate::setItemSimplePaint(bool val){
    isitemsimplepaint = val;
}

void QMdItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const{
    QLineEdit *edit = qobject_cast<QLineEdit *>(editor);
    if(edit){
        oldval = index.model()->data(index,Qt::DisplayRole).toString();
        edit->setText(oldval);
        emit editing(index);
        this->editor = QPointer<QLineEdit>(edit);
        int row = index.row();
        int column = index.column();
        QPair<int,int> rc;
        rc.first = row;
        rc.second = column;
        ValidatorStruct val;
        if(fechValidatorstruct(val,row,column)){
            switch(val.validatortype){
            case IntValidator:
                intv.setRange(val.bottom,val.top);
                edit->setValidator(&intv);
                break;
            case RegExpValidator:
                regexpv.setRegExp(QRegExp(val.regexp));
                edit->setValidator(&regexpv);
                break;
            case DoubleValidator:
                break;
            default:
                break;
            }
        }
    }
    return;
}

void QMdItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const{
    QLineEdit *edit = qobject_cast<QLineEdit *>(editor);
    if(edit){
        QString str = edit->text();
        int pos = 0;
        QValidator::State valstate;
        int row = index.row();
        int column = index.column();
        QPair<int,int> rc(row,column);
        ValidatorStruct val;
        if(fechValidatorstruct(val,row,column)){
            ValidatorType  validatortype =val.validatortype;
            switch(validatortype){
            case IntValidator:
                valstate = intv.validate(str,pos);
                break;
            case RegExpValidator:
                valstate = regexpv.validate(str,pos);
                break;
            default:
                break;
            }
        }else
            valstate = QValidator::Acceptable;
        if((valstate==QValidator::Acceptable)&&(str.trimmed()!=oldval.trimmed()))
            model->setData(index,str,Qt::EditRole);
        //else
            //this->setEditorData();
    }
}


void QMdItemDelegate::setRange(int bottom,int top,int row ,int column){
    if(bottom>top)
        return;
    QPair<int,int> rc(row,column) ;
    ValidatorStruct val;
    val.validatortype = IntValidator;
    val.bottom = bottom;
    val.top = top;
    validatorhash.insert(rc,val);
}

void QMdItemDelegate::setRegExp(QString regexp,int row ,int column){
    QPair<int,int> rc(row,column);
    ValidatorStruct val;
    val.validatortype = RegExpValidator;
    val.regexp = regexp;
    validatorhash.insert(rc,val);
}


///////////////////QDMZModel////////////////////////////
int QDMZModel::rowCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
    return 24;
}

int QDMZModel::columnCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
#if DUAL_SYSTEM
    return 8;
#else
    return 4;
#endif
}

QVariant  QDMZModel::data(const QModelIndex &index, int role) const{
    if(!index.isValid())
        return QVariant();
    int row = index.row();
    int column = index.column();
    if(role==Qt::DisplayRole){
#if DUAL_SYSTEM
        short dat = patterndata->wrk_fechData(WrkItemHd_DuMuZi,8*row+column);
#else
        short dat = patterndata->wrk_fechData(WrkItemHd_DuMuZi,4*row+column);

#endif
        return QString::number(dat);
    }
    if(Qt::BackgroundColorRole==role){
        if((row==0)||(row==22)||(row==23))
            return QVariant();
        else{
            if(patterndata->cnt_dumuUsed()&(1<<(row+1)))
                return QVariant();
            else
                return QColor(0,0,0,100);
        }
    }
    return QVariant();
}



bool  QDMZModel::setData(const QModelIndex &index, const QVariant &value, int role){
    if(this->patterndata == NULL)
        return FALSE;
    if(!index.isValid())
        return FALSE;
    if(role==Qt::EditRole){
        int row = index.row();
        int column = index.column();
        short dat = value.toString().toInt();
#if DUAL_SYSTEM
        patterndata->wrk_setData(WrkItemHd_DuMuZi,8*row+column,dat);
#else
        patterndata->wrk_setData(WrkItemHd_DuMuZi,4*row+column,dat);
#endif
        return TRUE;
    }else
        return FALSE;
}


Qt::ItemFlags  QDMZModel::flags(const QModelIndex &index) const{
    return QAbstractTableModel::flags(index)|Qt::ItemIsEditable;
}

QVariant QDMZModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(role!=Qt::DisplayRole)
        return QVariant();
    if(orientation == Qt::Vertical)
        return QString::number(section+1);
    else{
        QStringList list;
#if DUAL_SYSTEM
        list <<"后1"<<"后3"<<"前1"<<"前3"<<"后2"<<"后4"<<"前2"<<"前4";
#else
        list <<"后"<<"前"<<"后"<<"前";
#endif

        return list.at(section);
    }
    return QVariant();
}



//////LL//////////////////////////////////////////////////
int QLLModel::rowCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
    return 24;
}

int QLLModel::columnCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
    return 2;
}

QVariant  QLLModel::data(const QModelIndex &index, int role) const{
    if(this->patterndata == NULL)
        return QVariant();
    if(!index.isValid())
        return QVariant();
    if(role==Qt::DisplayRole){
        int row = index.row();
        int column = index.column();
        WrkItemHd handle = column?WrkItemHd_JuanBuFZ:
                                         WrkItemHd_JuanBu;
        short dat = patterndata->wrk_fechData(handle,row);
        return QString::number(dat);
    }else{
        return QVariant();
    }
}


bool  QLLModel::setData(const QModelIndex &index, const QVariant &value, int role){
    if(this->patterndata == NULL)
        return FALSE;
    if(!index.isValid())
        return FALSE;
    if(role==Qt::EditRole){
        int row = index.row();
        int column = index.column();
        short dat = value.toString().toInt();
        WrkItemHd handle = column?WrkItemHd_JuanBuFZ:
                                         WrkItemHd_JuanBu;
        patterndata->wrk_setData(handle,row,dat);
        return TRUE;
    }else
        return FALSE;
}


Qt::ItemFlags  QLLModel::flags(const QModelIndex &index) const{
    return QAbstractTableModel::flags(index)|Qt::ItemIsEditable;
}

QVariant QLLModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(role!=Qt::DisplayRole)
        return QVariant();
    if(orientation == Qt::Vertical)
       return QString::number(section+1);
    else{
        QStringList list;
        list <<"主罗拉"<<"辅助罗拉";
        return list.at(section);
    }
    return QVariant();
}

////////////////////ZDJ/////////////////////////////

int QZDJModel::rowCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
    return 24;
}

int QZDJModel::columnCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
    return 1;
}

QVariant  QZDJModel::data(const QModelIndex &index, int role) const{
    if(this->patterndata == NULL)
        return QVariant();
    if(!index.isValid())
        return QVariant();
    if(role==Qt::DisplayRole){
        int row = index.row();
        short dat = patterndata->wrk_fechData(WrkItemHd_SuDuZi,row);
        return QString::number(dat);
    }else{
        return QVariant();
    }
}


bool  QZDJModel::setData(const QModelIndex &index, const QVariant &value, int role){
    if(this->patterndata == NULL)
        return FALSE;
    if(!index.isValid())
        return FALSE;
    if(role==Qt::EditRole){
        int row = index.row();
        short dat = value.toString().toInt();
        patterndata->wrk_setData(WrkItemHd_SuDuZi,row,dat);
        return TRUE;
    }else
        return FALSE;
}


Qt::ItemFlags  QZDJModel::flags(const QModelIndex &index) const{
    return QAbstractTableModel::flags(index)|Qt::ItemIsEditable;
}

QVariant QZDJModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(role!=Qt::DisplayRole)
        return QVariant();
    if(orientation == Qt::Vertical)
        return QString::number(section+1);
    else{
        QStringList list;
        list <<"主马达速度";
        return list.at(section);
    }
    return QVariant();
}


//////////////////////ss//////////////////////////////////////
int QSSModel::rowCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
    return 24;
}

int QSSModel::columnCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
    return 2;
}

QVariant  QSSModel::data(const QModelIndex &index, int role) const{
    if(this->patterndata == NULL)
        return QVariant();
    if(!index.isValid())
        return QVariant();
    if(role==Qt::DisplayRole){
        int row = index.row();
        int column = index.column();
        if(0==column){
            short dat = patterndata->wrk_fechData(WrkItemHd_LeftSongSa,row);
            return QString::number(dat);
        }else if(1==column){
            short dat = patterndata->wrk_fechData(WrkItemHd_RightSongSa,row);
            return QString::number(dat);
        }
    }
      return QVariant();
}


bool  QSSModel::setData(const QModelIndex &index, const QVariant &value, int role){
    if(this->patterndata == NULL)
        return FALSE;
    if(!index.isValid())
        return FALSE;
    if(role==Qt::EditRole){
        int row = index.row();
        int column = index.column();
        short dat = value.toString().toInt();
        WrkItemHd handle;
        if(0==column){
           handle =  WrkItemHd_LeftSongSa;
        }else if(1==column){
           handle = WrkItemHd_RightSongSa;
        }else
            return FALSE;
        patterndata->wrk_setData(handle,row,dat);
        return TRUE;
    }else
        return FALSE;
}


Qt::ItemFlags  QSSModel::flags(const QModelIndex &index) const{
    return QAbstractTableModel::flags(index)|Qt::ItemIsEditable;
}

QVariant QSSModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(role!=Qt::DisplayRole)
        return QVariant();
    if(orientation == Qt::Vertical)
        return QString::number(section+1);
    else{
        QStringList list;
        list <<"左送纱"<<"右送纱";
        return list.at(section);
    }
    return QVariant();
}



//////////////////SZTF/////////////////////////////////

int QSZTFModel::rowCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
    return 9;
}

int QSZTFModel::columnCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
    return 16;
}

QVariant  QSZTFModel::data(const QModelIndex &index, int role) const{
    if(this->patterndata == NULL)
        return QVariant();
    if(!index.isValid())
        return QVariant();
    int row = index.row();
    int column = index.column();
    if(role==Qt::DisplayRole){
        if(0==row)
            return QString::number(column/2+1)+QString("号沙嘴");
        else{
            short dat = patterndata->wrk_fechData(WrkItemHd_ShaZuiTF,(column/2)*16+(row-1)*2+column%2);
            return QString::number(dat);
        }
    }
    if(Qt::BackgroundRole==role){
        if(0==row)
            return QColor(Qt::gray);
        else if(column/2%2==0)
            return QColor(Qt::white);
    }
    if(Qt::SizeHintRole==role){
        if(0==row)
            return QSize(0,QApplication::fontMetrics().height());
    }
    return QVariant();
}


bool  QSZTFModel::setData(const QModelIndex &index, const QVariant &value, int role){
    if(this->patterndata == NULL)
        return FALSE;
    if(!index.isValid())
        return FALSE;
    if(role==Qt::EditRole){
        int row = index.row();
        int column = index.column();
        short dat = value.toString().toInt();
        patterndata->wrk_setData(WrkItemHd_ShaZuiTF,(column/2)*16+(row-1)*2+column%2,dat);
        return TRUE;
    }else
        return FALSE;
}


Qt::ItemFlags  QSZTFModel::flags(const QModelIndex &index) const{
    int row = index.row();
    if(0==row){
        return QAbstractTableModel::flags(index);
    }
    return QAbstractTableModel::flags(index)|Qt::ItemIsEditable;
}

QVariant QSZTFModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(role!=Qt::DisplayRole)
        return QVariant();
    if(orientation == Qt::Vertical)
        if(0==section)
            return QString();
    return QString::number(section);

}


///////////////////ytxszXZ////////////////////////////////

int QYTXSZXZModel::rowCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
    return 13;
}

int QYTXSZXZModel::columnCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
    return 16;
}

QVariant  QYTXSZXZModel::data(const QModelIndex &index, int role) const{
    if(this->patterndata == NULL)
        return QVariant();
    if(!index.isValid())
        return QVariant();
    int row = index.row();
    int column = index.column();
    if(role==Qt::DisplayRole){
        if(0==row){
            return QString::number(column/2+1)+QString("号沙嘴");
        }
        else{
            short dat = patterndata->wrk_fechData(WrkItemHd_YTXXiuZen,(column/2)*24+(row-1)*2+column%2);
            return QString::number(dat);
        }
    }
    if(Qt::SizeHintRole==role){
        if(0==row)
            return QSize(0,QApplication::fontMetrics().height());
    }
    if(Qt::BackgroundRole==role){
        if(0==row)
            return QColor(Qt::gray);
        else if(column/2%2==0)
            return QColor(Qt::white);
    }
    return QVariant();
}


bool  QYTXSZXZModel::setData(const QModelIndex &index, const QVariant &value, int role){
    if(this->patterndata == NULL)
        return FALSE;
    if(!index.isValid())
        return FALSE;
    if(role==Qt::EditRole){
        int row = index.row();
        int column = index.column();
        short dat = value.toString().toInt();
        patterndata->wrk_setData(WrkItemHd_YTXXiuZen,(column/2)*24+(row-1)*2+column%2,dat);
        return TRUE;
    }else
        return FALSE;
}


Qt::ItemFlags  QYTXSZXZModel::flags(const QModelIndex &index) const{
    int row = index.row();
    if(0==row){
        return Qt::ItemIsEnabled;
    }
    return QAbstractTableModel::flags(index)|Qt::ItemIsEditable;
}

QVariant QYTXSZXZModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(role!=Qt::DisplayRole)
        return QVariant();
    if(orientation == Qt::Vertical)
        if(0==section)
            return QString();
    return QString::number(section);
}


///////////////////ytxszTF////////////////////////////////

int QYTXSZTFModel::rowCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
    return 2;
}

int QYTXSZTFModel::columnCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
    return 8;
}

QVariant  QYTXSZTFModel::data(const QModelIndex &index, int role) const{
    if(this->patterndata == NULL)
        return QVariant();
    if(!index.isValid())
        return QVariant();
    int column = index.row();
    int row = index.column();
    if(role==Qt::DisplayRole){
        short dat = patterndata->wrk_fechData(WrkItemHd_YTXTingFang,row*2+column);
        return QString::number(dat);
    }
    return QVariant();
}


bool  QYTXSZTFModel::setData(const QModelIndex &index, const QVariant &value, int role){
    if(this->patterndata == NULL)
        return FALSE;
    if(!index.isValid())
        return FALSE;
    if(role==Qt::EditRole){
        int column = index.row();
        int row = index.column();
        short dat = value.toString().toInt();
        patterndata->wrk_setData(WrkItemHd_YTXTingFang,row*2+column,dat);
        return TRUE;
    }else
        return FALSE;
}


Qt::ItemFlags  QYTXSZTFModel::flags(const QModelIndex &index) const{
    return Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsEditable;
}

QVariant QYTXSZTFModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(role!=Qt::DisplayRole)
        return QVariant();
    if(orientation == Qt::Horizontal)
        return QString::number(section+1)+QString("号纱嘴");
    return QVariant();
 }
////////////////CJP///////////////////////////////////////////////

int QCJPModel::rowCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
    return 15;
}

int QCJPModel::columnCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
    return 8;
}

QVariant  QCJPModel::data(const QModelIndex &index, int role) const{
    if(this->patterndata == NULL)
        return QVariant();
    if(!index.isValid())
        return QVariant();
    int row = index.row();
    int column = index.column();
    if(role==Qt::DisplayRole){        
        if(row == 0){
            QString str = column/4?"编织位":"翻针位";
            return str;
        }
        if(row==1){
            QString str = (column/2)%2?"左行使用":"右行使用";
            return str;
        }
        if(row ==2){
            QString str = column%2?"前":"后";
            return str;
        }
        if(row>2){
            row = row -3;
            WrkItemHd handle = column/4?WrkItemHd_CJP_BianZi:
                                             WrkItemHd_CJP_FanZen;
            column = column%4;
            short dat = patterndata->wrk_fechData(handle,(column/2)*24+row*2+column%2);
            return QString::number(dat);
        }
    }
    if(role==Qt::TextAlignmentRole)
        return Qt::AlignCenter;
    if(Qt::BackgroundRole==role){
        if(row<3)
            return QColor(Qt::gray);
        else if(column/2%2==0)
            return QColor(Qt::white);
    }
    if(Qt::SizeHintRole==role){
        if(row<3)
            return QSize(0,QApplication::fontMetrics().height());
    }
    return QVariant();
}



bool  QCJPModel::setData(const QModelIndex &index, const QVariant &value, int role){
    if(this->patterndata == NULL)
        return FALSE;
    if(!index.isValid())
        return FALSE;
    if(index.row()<3)
        return FALSE;
    if(role==Qt::EditRole){
        int row = index.row()-3;
        short dat = value.toString().toInt();
        int column = index.column();
        WrkItemHd handle = column/4?WrkItemHd_CJP_BianZi:
                                         WrkItemHd_CJP_FanZen;
        column = column%4;
        patterndata->wrk_setData(handle,(column/2)*24+row*2+column%2,dat);
        return TRUE;
    }else
        return FALSE;
}


Qt::ItemFlags  QCJPModel::flags(const QModelIndex &index) const{
    int row = index.row();
    if(row<3)
        return Qt::ItemIsEnabled;
    else
        return Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsEditable;
}

QVariant QCJPModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(role!=Qt::DisplayRole)
        return QVariant();
    if(orientation == Qt::Vertical){
        if(section<3)
            return QVariant();
        return QString::number(section-2);
    }
    else{
       return QVariant();
    }
    return QVariant();
}

//////////////pzk////////////////////////////////////////////////

void QPZKModel::modelReset(){
    QAbstractTableModel::reset();
}

int QPZKModel::rowCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
            count = this->patterndata->wrk_fechData(WrkItemHd_ZanKaiPianSu,0);
    return count+1;
}

int QPZKModel::columnCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
    return 9;
}

QVariant  QPZKModel::data(const QModelIndex &index, int role) const{
    if(this->patterndata == NULL)
        return QVariant();
    if(!index.isValid())
        return QVariant();
    int column = index.column();
    int row = index.row();
    if(role==Qt::DisplayRole){
        if(0==row){
            return column?(tr("沙嘴")+QString::number(column)):QString();
        }
        if(column==0){
            return QString::number(row);
        }
        short dat = this->patterndata->wrk_fechData(WrkItemHd_PzkSaZui,(row-1)*8+column-1);
        return QString::number(dat);
    }
    if(role==Qt::CheckStateRole){
        if(column == 0){
            if(row ==0)
                return QVariant();
            short dat = patterndata->wrk_fechData(WrkItemHd_PzkSaZui,64);
            return (0==(dat&(1<<(row-1))))?Qt::Checked:Qt::Unchecked;
        }
        return QVariant();
    }
    if(role==Qt::BackgroundColorRole){
        if(0==row){
            unsigned char shazuiraw =  this->patterndata->shazuiused_l|
                                       this->patterndata->shazuiused_r;
            unsigned char shazuikb = this->patterndata->shaZuiKb;
            if(shazuiraw&(1<<(column-1)))
                return QColor(Qt::green);
            if(shazuikb&(1<<(column-1)))
                return QColor(Qt::yellow);
            return QColor(Qt::gray);
        }else if(0==column)
            return QColor(Qt::gray);
        else{
            short dat = patterndata->wrk_fechData(WrkItemHd_PzkSaZui,64);
            return (0==(dat&(1<<(row-1))))?QColor(0,0,200,100):QColor(0,0,0,100);
        }
    }
    if(Qt::SizeHintRole==role){
        if(0==row)
            return QSize(0,QApplication::fontMetrics().height());
    }
    return  QVariant();
}


bool  QPZKModel::setData(const QModelIndex &index, const QVariant &value, int role){

    if(this->patterndata == NULL)
        return FALSE;
    if(!index.isValid())
        return FALSE;
    int row = index.row();
    int column = index.column();
    if(role==Qt::EditRole)
        return FALSE;
    if(role ==Qt::CheckStateRole){
        short val = 1<<(row-1);
        short dat = patterndata->wrk_fechData(WrkItemHd_PzkSaZui,64);
        patterndata->wrk_setData(WrkItemHd_PzkSaZui,64,dat^val);
        reset();
        return TRUE;
    }
    return FALSE;
}



Qt::ItemFlags  QPZKModel::flags(const QModelIndex &index) const{
    int row = index.row();
    int column = index.column();
    if(row>count)
        return Qt::NoItemFlags;
    else if(0==column)
        return (1==row)?Qt::ItemIsEnabled:(Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
    else{
        unsigned char shazuiraw =  this->patterndata->shazuiused_l|
                                   this->patterndata->shazuiused_r;
        unsigned char shazuikb = this->patterndata->shaZuiKb;
        if(shazuiraw&(1<<(column-1)))
            return Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsEditable;
        if(shazuikb&(1<<(column-1)))
            return Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsEditable;

    }
    return Qt::NoItemFlags;
}


QVariant QPZKModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(role!=Qt::DisplayRole)
        return QVariant();
    if(orientation == Qt::Horizontal){
        return section?QString::number(section):QString();
    }
    return  QVariant();

}

////////////loop////////////////////////////////////

int QCntLoopModel::rowCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
    return looplistchanged.size();
}
int QCntLoopModel::columnCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
    return 3;
}
QVariant QCntLoopModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(role != Qt::DisplayRole)
        return QVariant();
    QStringList list;
    list<<tr("行首")<<tr("行尾")<<tr("循环次数");
    if(orientation == Qt::Horizontal){
        return list.at(section);
    }else
        return QString::number(section+1);
    return QVariant();
}

QVariant QCntLoopModel::data(const QModelIndex &index, int role) const{
    if(role!=Qt::DisplayRole)
        return QVariant();
    int row = index.row();
    int column = index.column();
    if(column ==0)
        return QString::number(looplistchanged.at(row).startline);
    if(column ==1)
        return QString::number(looplistchanged.at(row).endline);
    if(column ==2)
        return QString::number(looplistchanged.at(row).numofloop);
    return QString();
}

QCntLoopModel::QCntLoopModel( QPatternData *data ,QObject * parent):QAbstractTableModel(parent),
                                                        patterndata(data){
    QFile *cntfile = patterndata->cntfile.data();
    if(!cntfile->open(QIODevice::ReadWrite)){
        qDebug("in data.h QCntLoopModel::QCntLoopModel  cntfile open fail ");
        return;
    }
    unsigned char buf[128];
    int i = 1;
    do{
        int pos = 0x80*i;
        if(pos>=cntfile->size())
            break;
        cntfile->seek(pos);
        int size = cntfile->read((char *)buf,128);
        if(size!=128)
            break;
        if(*(unsigned short *)(buf+CNT_LoopNum)){
            struct LoopType loop;
            loop.startline = *(unsigned short *)(buf+CNT_LoopStart);
            loop.endline = i;
            loop.numofloop = *(unsigned short *)(buf+CNT_LoopNum);
            looplist<<loop;
        }
        i++;
    }while(buf[CNT_End]!=0x01);
    looplistchanged = looplist;
    cntfile->close();
}

bool QCntLoopModel::setData(const QModelIndex &index, const QVariant &value, int role){
    if(role!=Qt::EditRole)
        return false;
    int row = index.row();
    int column = index.column();
    if(column ==0)
        looplistchanged[row].startline = value.toInt();
    if(column ==1)
        looplistchanged[row].endline= value.toInt();
    if(column ==2)
        looplistchanged[row].numofloop= value.toInt();
    emit dataChanged(index,index);
    emit datasValid(checkdatavalid());
    return TRUE;
}

Qt::ItemFlags QCntLoopModel::flags(const QModelIndex &index) const{
    return QAbstractTableModel::flags(index)|Qt::ItemIsEditable;
}

bool QCntLoopModel::insertRows(int row,int count, const QModelIndex &parent){
    Q_UNUSED(parent)
    beginInsertRows(QModelIndex(),row,row+count-1);
    struct LoopType loop;
    for(int i=0;i<count;i++){
        loop.startline = 0;
        loop.endline = 0;
        loop.numofloop = 0;
        looplistchanged.insert(row,loop);
    }
    endInsertRows();
    emit datasValid(checkdatavalid());
    return TRUE;
}

bool QCntLoopModel::removeRows(int row,int count, const QModelIndex &parent){
    Q_UNUSED(parent);
    beginRemoveRows(QModelIndex(),row,row+count-1);
    for(int i=0;i<count;i++){
        looplistchanged.removeAt(row);
    }
    endRemoveRows();
    emit datasValid(checkdatavalid());
    return TRUE;
}

void QCntLoopModel::save(bool send){
    struct LoopType loop;
    unsigned short start,end,num;
    for(int i=0;i<looplist.size();i++){
        loop = looplist.at(i);
        end = loop.endline;
        patterndata->cntSetData(end-1,CNT_LoopStart,0,2);
        patterndata->cntSetData(end-1,CNT_LoopNum,0,2);
    }
    for(int i=0;i<looplistchanged.size();i++){
        loop = looplistchanged.at(i);
        start = loop.startline;
        end = loop.endline;
        num = loop.numofloop;
        patterndata->cntSetData(end-1,CNT_LoopStart,start,2);
        patterndata->cntSetData(end-1,CNT_LoopNum,num,2);
    }
    patterndata->Save(Md::HAVECNT,send?Md::HAVECNT:Md::HAVENO);
}

void QCntLoopModel::resetVal(){

}

bool QCntLoopModel::checkdatavalid(){
    foreach(LoopType loop,looplistchanged){
        if(((loop.startline%2==0)||(loop.endline%2==1)
        ||(loop.startline>loop.endline)))
        return FALSE;
    }
    return TRUE;
}

////////////////沙嘴捆绑//////////////////

int QSzkbModel::rowCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
    return szkblist.size();
}
int QSzkbModel::columnCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
    return 4;
}
QVariant QSzkbModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(role != Qt::DisplayRole)
        return QVariant();
    if(orientation==Qt::Horizontal){
        switch(section){
        case 0:
            return tr("主纱嘴");
        case 1:
            return tr("副纱嘴");
        case 2:
            return tr("开始行");
        case 3:
            return tr("结束行");
        default:
            return QVariant();
        }
    }else{
        return section+1;
    }
    return QVariant();
}

QVariant QSzkbModel::data(const QModelIndex &index, int role) const{
    if(role!=Qt::DisplayRole)
        return QVariant();
    int row = index.row();
    int column = index.column();
    QPatternData::SzkbData szkbdata = szkblist.at(row);
    if(column ==0)
        return QString::number(szkbdata.ZuSa);
    if(column ==1)
        return QString::number(szkbdata.FuSa);
    if(column ==2)
        return QString::number(szkbdata.Start);
    if(column ==3)
        return QString::number(szkbdata.End);
    return QVariant();
}

bool QSzkbModel::setData(const QModelIndex &index, const QVariant &value, int role){
    if(role!=Qt::EditRole)
        return false;
    int row = index.row();
    int column = index.column();
    unsigned short a = (unsigned short)value.toString().toInt();
    if(column ==0)
        szkblist[row].ZuSa = a;
    if(column ==1)
        szkblist[row].FuSa = a;
    if(column ==2)
        szkblist[row].Start = a;
    if(column ==3)
        szkblist[row].End = a;
    emit dataChanged(index,index);
    emit datasValid(checkdatavalid());
    return TRUE;
}

Qt::ItemFlags QSzkbModel::flags(const QModelIndex &index) const{
    return QAbstractTableModel::flags(index)|Qt::ItemIsEditable;
}

bool QSzkbModel::insertRows(int row,int count, const QModelIndex &parent){
    Q_UNUSED(parent)
    beginInsertRows(QModelIndex(),row,row+count-1);
    QPatternData::SzkbData szkb;
    for(int i=0;i<count;i++){
        szkb.ZuSa = 0;
        szkb.FuSa = 0;
        szkb.Start = 0;
        szkb.End = 0;
        this->szkblist.append(szkb);
    }
    endInsertRows();
    emit datasValid(checkdatavalid());
    return TRUE;
}

bool QSzkbModel::removeRows(int row,int count, const QModelIndex &parent){
    Q_UNUSED(parent);
    beginRemoveRows(QModelIndex(),row,row+count-1);
    for(int i=0;i<count;i++)
        this->szkblist.removeAt(row);
    endRemoveRows();
    emit datasValid(checkdatavalid());
    return TRUE;
}

void QSzkbModel::saveToFile(){
    this->patterndata->sazbuf = this->szkblist;
    this->patterndata->Save(Md::HAVESAZ,Md::HAVESAZ);
}

bool QSzkbModel::checkdatavalid(){
    unsigned char shazuiraw = patterndata->shazuiused_l|
                              patterndata->shazuiused_r;
    foreach(QPatternData::SzkbData data,szkblist){
        unsigned char zs = 1<<(data.ZuSa-1);
        unsigned char fs = 1<<(data.FuSa-1);
        int start = data.Start;
        int end = data.End;
        if(((zs&shazuiraw)==0)||((fs&shazuiraw)!=0)||
                    (start<1)||(start>=end))
            return FALSE;
    }
    return TRUE;
}


////////////////停车编辑//////////////////

QTingcheModel::QTingcheModel(QPatternData *pattern ,QObject * parent):QAbstractTableModel(parent),
             patterndata(pattern){
        for(int i=0;i<patterndata->tatalcntrow;i++){
            if(patterndata->cntFechData(i,CNT_TingCe,1)==1)
                cntrows.insert(i,1);
        }
}


int QTingcheModel::rowCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
    return cntrows.size();
}

int QTingcheModel::columnCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
    return 1;
}

QVariant QTingcheModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(role != Qt::DisplayRole)
        return QVariant();
    if(orientation==Qt::Horizontal){
        return tr("CNT行号");
    }else{
        return section+1;
    }
    return QVariant();
}

QVariant QTingcheModel::data(const QModelIndex &index, int role) const{
    if(role!=Qt::DisplayRole)
        return QVariant();
    int row = index.row();
    return this->cntrows.keys().at(row)+1;
}


bool QTingcheModel::insertCntRow (unsigned short cntrow){
        cntrows.insert(cntrow,1);
        patterndata->cntSetData(cntrow,CNT_TingCe,1,1);
        reset();
}

bool QTingcheModel::removeRows(int row,int count, const QModelIndex &parent){
    Q_UNUSED(parent);
    beginRemoveRows(QModelIndex(),row,row+count-1);
    for(int i=0;i<count;i++){
        unsigned short cntrow = cntrows.keys().at(row+i);
        cntrows.remove(cntrow);
        patterndata->cntSetData(cntrow,CNT_TingCe,0,1);
    }
    endRemoveRows();
    return TRUE;
}


///////////////////QDMBCModel/////////////////////////////////////////

int QDMBCModel::rowCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
#if DUAL_SYSTEM
    return 8;
#else
    return 4;
#endif
}

int QDMBCModel::columnCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
    return 3;
}

QVariant  QDMBCModel::data(const QModelIndex &index, int role) const{
    if(!index.isValid())
        return QVariant();
    if(role==Qt::DisplayRole){
        int row = index.row();
        int column = index.column();
        switch(column){
        case 0:
            return QString::number(param->fechData(SpaItemHd_Dmdmbc,row));
        case 1:
            return QString::number(param->fechData(SpaItemHd_Spmdbc,row));
        case 2:
            return QString::number(param->fechData(SpaItemHd_Bgzdmlwbc,row));
        default:
            break;
        }
        return QVariant();
    }
     return QVariant();
}


bool  QDMBCModel::setData(const QModelIndex &index, const QVariant &value, int role){
    if(!index.isValid())
        return FALSE;
    if(Qt::EditRole==role){
        int row = index.row();
        int column = index.column();
        int val = value.toString().toInt();
        switch(column){
        case 0:
            param->setData(SpaItemHd_Dmdmbc,row,val);
            break;
        case 1:
            param->setData(SpaItemHd_Spmdbc,row,val);
            break;
        case 2:
            param->setData(SpaItemHd_Bgzdmlwbc,row,val);
            break;
        default:
            break;
        }
        return TRUE;
    }
    return FALSE;
}


Qt::ItemFlags  QDMBCModel::flags(const QModelIndex &index) const{
    return QAbstractTableModel::flags(index)|Qt::ItemIsEditable;
}

QVariant QDMBCModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(role!=Qt::DisplayRole)
        return QVariant();
    if(orientation == Qt::Vertical){
#if DUAL_SYSTEM
        return QString("#0%1").arg(section+1);
#else
        switch(section){
        case 0:
            return tr("后左");
        case 1:
            return tr("后右");
        case 2:
            return tr("前左");
        case 3:
            return tr("前右");
        default:
            break;
        }
#endif
    } else{
        switch(section){
        case 0:
            return tr("单面度目补偿");
        case 1:
            return tr("四平度目补偿");
        case 2:
            return tr("不工作度目补偿");
        default:
            break;
        }
    }
    return QVariant();
}


///////////////摇床位置修正//////////////////////////////////////

int QYCWZXZModel::rowCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
    return 9;
}

int QYCWZXZModel::columnCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
    return 8;
}

QVariant QYCWZXZModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(Qt::DisplayRole!=role)
        return QVariant();
    if(Qt::Horizontal==orientation){
        if(section%2)
            return tr("修正值");
        else
            return tr("位置");
    }
    return QVariant();
}

QVariant  QYCWZXZModel::data(const QModelIndex &index, int role) const{
    if(!index.isValid())
        return QVariant();
    if(role==Qt::DisplayRole){
        int row = index.row();
        int column = index.column();
        if(row ==0){
            if(column==0)
                return "0";
            else if(column==1)
                return QString::number(paramadata->fechData(SpaItemHd_Ycwzxz,0));
            else
                return "";
        }else{
            switch(column){
            case 0:
                return QString("R%1---").arg(row);
            case 1:
                return QString::number(paramadata->fechData(SpaItemHd_Ycwzxz,row));
            case 2:
                return QString("L%1---").arg(row);
            case 3:
                return QString::number(paramadata->fechData(SpaItemHd_Ycwzxz,row+8));
            case 4:
                return QString("R%1*--").arg(row-1);
            case 5:
                return QString::number(paramadata->fechData(SpaItemHd_Ycwzxz,row+16));
            case 6:
                return QString("L%1*--").arg(row-1);
            case 7:
                return QString::number(paramadata->fechData(SpaItemHd_Ycwzxz,row+24));
            default:
                break;
            }
        }
    }
    return QVariant();
}


bool  QYCWZXZModel::setData(const QModelIndex &index, const QVariant &value, int role){
    if(!index.isValid())
        return FALSE;
    if(Qt::EditRole!=role)
        return FALSE;
    int row = index.row();
    int column = index.column();
    int val = value.toString().toInt();
    if(0==row)
        paramadata->setData(SpaItemHd_Ycwzxz,0,val,FALSE);
    else
        paramadata->setData(SpaItemHd_Ycwzxz,column/2*8+row,val,FALSE);

    switch(column){
    case 1:
        pcomm->bedMotorTest(2,row*100,val);
        break;
    case 3:
        pcomm->bedMotorTest(2,-100*row,val);
        break;
    case 5:
        pcomm->bedMotorTest(3,100*(row-1),val);
        break;
    case 7:
        pcomm->bedMotorTest(3,-100*(row-1),val);
        break;
    default:
        break;
    }
    return TRUE;
}


Qt::ItemFlags  QYCWZXZModel::flags(const QModelIndex &index) const{
    int column = index.column();
    if(column%2)
        return Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsEditable;
    else
        return Qt::NoItemFlags;

}

//////////////////////////翻针摇床位置修正//////////////////////////


int QFZYCWZXZModel::rowCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
    return 17;
}
int QFZYCWZXZModel::columnCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
    return 6;
}
QVariant QFZYCWZXZModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(role!=Qt::DisplayRole)
        return QVariant();
    if(Qt::Vertical==orientation){
        if(0==section)
            return QString();
        else
            return QString::number(section);
    }
    return QVariant();
}

QVariant QFZYCWZXZModel::data(const QModelIndex &index, int role) const{

    if(!index.isValid())
        return QVariant();
    int row =index.row();
    int column = index.column();
    if(role==Qt::DisplayRole){
        if(0==row){
            switch(column){
            case 0:
                return tr("翻针摇床");
            case 2:
                return tr("(+)翻针摇床");
            case 4:
                return tr("(-)翻针摇床");
            default:
                return QString();
            }
        }
        switch(column){
        case 0:
            return (1==row)?QString("0.0"):((row<10)?QString("R%1.0").arg(row-1):QString("L%1.0").arg(row-9));
        case 1:
            return param->fechData(SpaItemHd_Fzycwzxz,row-1);
        case 2:
            return (1==row)?QString("0.+"):((row<10)?QString("R%1.*").arg(row-1):QString("L%1.*").arg(row-9));
        case 3:
            return param->fechData(SpaItemHd_Fzycwzxz_z,row-1);
        case 4:
            return (1==row)?QString("0.-"):((row<10)?QString("R%1.-").arg(row-1):QString("L%1.-").arg(row-9));
        case 5:
            return param->fechData(SpaItemHd_Fzycwzxz_f,row-1);
        default:
            return QString();
        }
    }
    if(role==Qt::BackgroundRole){
        if(0==row)
            return QColor(Qt::gray);
    }
    if((role==Qt::SizeHintRole)&&(0==row)){
        return QSize(0,QApplication::fontMetrics().height());
    }
    if((role==Qt::TextAlignmentRole)&&(0==row)){
        return Qt::AlignCenter;
    }
    return QVariant();
}

bool QFZYCWZXZModel::setData(const QModelIndex &index, const QVariant &value, int role){
    if(!index.isValid())
        return FALSE;
    int row = index.row();
    int column = index.column();
    if(0==row)
        return FALSE;
    if(Qt::EditRole==role){
        int val = value.toInt();
        switch(column){
        case 1:
            param->setData(SpaItemHd_Fzycwzxz,row-1,val,FALSE);
            row--;
            if(row>8)
                row = -(row-8);
            pcomm->bedMotorTest(5,100*row,val);
            break;
        case 3:
            param->setData(SpaItemHd_Fzycwzxz_z,row-1,val,FALSE);
            row--;
            if(row>8)
                row = -(row-8);
            pcomm->bedMotorTest(4,100*row,val);
            break;
        case 5:
            param->setData(SpaItemHd_Fzycwzxz_f,row-1,val,FALSE);
            row--;
            if(row>8)
                row = -(row-8);
            pcomm->bedMotorTest(6,100*row,val);
            break;
        default:
            break;
        }
    }
    return TRUE;
}

Qt::ItemFlags QFZYCWZXZModel::flags(const QModelIndex &index) const{
    if(!index.isValid())
        return Qt::NoItemFlags;
    int row = index.row();
    int column = index.column();
    if(0==row){
        return Qt::ItemIsEnabled;
    }
    else{
        return (column%2)?(Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsEditable):
                Qt::NoItemFlags;
    }
    return Qt::NoItemFlags;
}




bool QMdSortFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex & source_parent ) const{
    QModelIndex srcmodeindex;
    if(source_parent!=*psrcparentindex){
        *psrcparentindex = source_parent;
        psrcmodel_colomn0list->clear();
        int srcrow = sourceModel()->rowCount(source_parent);
        QFileSystemModel *psourcemodel;
        for(int i =0;i<srcrow;i++){
            srcmodeindex = sourceModel()->index(i,0,source_parent);
            psourcemodel = static_cast<QFileSystemModel *>(sourceModel());
            if(!psourcemodel->isDir(srcmodeindex)){                
                QString str = psourcemodel->data(srcmodeindex,Qt::DisplayRole).toString();
                *psrcmodel_colomn0list<<str;
            }
        }
    }

    if(static_cast<QFileSystemModel *>(sourceModel())->isDir(sourceModel()->index(source_row,0,source_parent)))
        return TRUE;
    QString c0 = sourceModel()->data(sourceModel()->index(source_row,0,source_parent),Qt::DisplayRole).toString();
    if(!c0.endsWith(".pat",Qt::CaseInsensitive))
        return FALSE;
    QString prename = preName(c0);
    if(!psrcmodel_colomn0list->contains(prename+".cnt",Qt::CaseInsensitive))
        return FALSE;
    return TRUE;
}


QVariant QMdSortFilterProxyModel::headerData ( int section, Qt::Orientation orientation, int role ) const{
    if((role ==Qt::DisplayRole)&(orientation==Qt::Vertical))
        return section+1;
    else
        return QSortFilterProxyModel::headerData(section,orientation,role);
}

QString QMdSortFilterProxyModel::preName(const QString &name)const{
    int index = name.indexOf(QString("."),-4);
    return name.left(index);
}


QString QMdSortFilterProxyModel::preName(const QModelIndex &index)const{
    QFileSystemModel *model = static_cast<QFileSystemModel *>(sourceModel());
    QModelIndex scrindex = mapToSource(index);
    QString filename = model->fileName(scrindex);
    int i = filename.indexOf('.',-4);
    return filename.left(i);
}


QString QMdSortFilterProxyModel::suffName(const QString &name)const{
    return name.right(name.size()-name.indexOf(".",-1,Qt::CaseInsensitive));
}

QVariant QMdSortFilterProxyModel::data ( const QModelIndex & index, int role ) const{
    int column = index.column();
    if((role==Qt::DisplayRole)&(column==0)){
        QString str = QSortFilterProxyModel::data(index,role).toString();
        if(str.endsWith(".pat",Qt::CaseInsensitive))
            return str.left(str.size()-4);
        else
            return str;
    }
    else
        return QSortFilterProxyModel::data(index,role);
}



QString QMdSortFilterProxyModel::upPath(const QModelIndex &index)const{
   QFileSystemModel *model = static_cast<QFileSystemModel *>(sourceModel());
   QModelIndex srcindex = mapToSource(index);
   return model->filePath(model->parent(srcindex));
}


QString QMdSortFilterProxyModel::patternName(const QModelIndex & index,const QString &suffname)const{
    QString str = upPath(index);
    QDir dir(str);
    QStringList filenamelist = dir.entryList(QDir::Files);
    QStringList filenamelisttemp;
    QString name = data(index,Qt::DisplayRole).toString();
    if(suffname==""){
        return name;
    }else{
        for(int i=0;i<filenamelist.size();i++){
            filenamelisttemp<<filenamelist.at(i).toLower();
        }
        int i = filenamelisttemp.indexOf(name.toLower()+suffname.toLower());
        if(i!=-1)
            return filenamelist.at(i);
        else
            return QString();
    }
}



Md::HAVEFILEFLAG QMdSortFilterProxyModel::haveFile(const QModelIndex &index){
    Md::HAVEFILEFLAG flag = Md::HAVENO;
    QString str = upPath(index);
    QDir dir(str);
    QStringList filenamelist = dir.entryList(QDir::Files);
    QStringList filenamelisttemp;
    for(int i=0;i<filenamelist.size();i++){
        filenamelisttemp<<filenamelist.at(i).toLower();
    }
    QString prename = preName(index);
    int i = filenamelisttemp.indexOf(prename.toLower()+".saz");
    if(i!=-1)
        flag |= Md::HAVESAZ;
    i = filenamelisttemp.indexOf(prename.toLower()+".wrk");
    if(i!=-1)
        flag |= Md::HAVEWRK;
    i = filenamelisttemp.indexOf(prename.toLower()+".prm");
    if(i!=-1)
        flag |= Md::HAVEPRM;
    return flag;

}

QString QMdSortFilterProxyModel::haveFileString(const QModelIndex &index){
    Md::HAVEFILEFLAG  flag =haveFile(index);
    QString str = "cnt pat ";
    if(flag&Md::HAVEWRK)
        str.append(" wrk");
    if(flag&Md::HAVESAZ)
        str.append(" saz");
    if(flag&Md::HAVEPRM)
        str.append(" prm");
    return str;
}

QSize QMdSortFilterProxyModel::patternSize(const QModelIndex &index)const{
    QString patfilename = upPath(index)+"/"+patternName(index,".pat");
    QFile patfile(patfilename);
    patfile.open(QIODevice::ReadOnly);
    QDataStream stream(&patfile);
    stream.setByteOrder(QDataStream::LittleEndian);
    unsigned short wight,hight;
    stream>>wight>>hight;
    patfile.close();
    return QSize(wight,hight);
}

QString QMdSortFilterProxyModel::currentDirPath(const QModelIndex &parentindex){
    QFileSystemModel *model = static_cast<QFileSystemModel *>(sourceModel());
    QString path =  model->filePath(mapToSource(parentindex));
    return path;
}


void QMdSortFilterProxyModel::copy(const QString &srcpath,const QString &dirpath,QWidget *parent){
    QFile file(srcpath);
    QFileInfo fileinfo(file);
    QString filename = fileinfo.fileName();
    QFile desfile(dirpath+"/"+filename);
    if(desfile.exists()){
        if(QMessageBox::question(parent,tr("花型拷贝"),
                                 tr("目标文件夹包含")+filename+tr("是否覆盖?"),QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes){

            desfile.remove();
            file.copy(dirpath+"/"+filename);
        }
    }else{
        file.copy(dirpath+"/"+filename);
    }
}


void QMdSortFilterProxyModel::copy(const QModelIndex &srcindex, QMdSortFilterProxyModel &desmodel,
                                            const QModelIndex &desindex,QWidget *parent){
    QString cntfilename = patternName(srcindex,".cnt");
    QString patfilename = patternName(srcindex,".pat");

    QString srcdirpath = upPath(srcindex);
    QString desdirpath = desmodel.currentDirPath(desindex);

    copy(srcdirpath+"/"+cntfilename,desdirpath,parent);
    copy(srcdirpath+"/"+patfilename,desdirpath,parent);
    Md::HAVEFILEFLAG flag =  haveFile(srcindex);
    if(flag&Md::HAVEWRK){
        QString wrkfilename = patternName(srcindex,".wrk");
        copy(srcdirpath+"/"+wrkfilename,desdirpath,parent);
    }
    if(flag&Md::HAVESAZ){
        QString sazfilename = patternName(srcindex,".saz");
        copy(srcdirpath+"/"+sazfilename,desdirpath,parent);
    }
    if(flag&Md::HAVEPRM){
        QString prmfilename = patternName(srcindex,".prm");
        copy(srcdirpath+"/"+prmfilename,desdirpath,parent);
    }
}



