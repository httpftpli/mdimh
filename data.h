

#ifndef DATA_H
#define DATA_H


#include<QString>
#include<QAbstractListModel>
#include<QAbstractTableModel>
#include<QFile>
#include<QStyledItemDelegate>
#include<QSplashScreen>
#include<QPixmap>
#include<QSettings>
#include"communicat.h"
#include"QSortFilterProxyModel"
#include<pattern.h>
#include<QVariant>
#include<namespace.h>
#include<QIntValidator>
#include<QLineEdit>
#include<QPointer>
#include<QDateTime>
//#include<QApplication>


class QParam;


#define ADDR_ParaXiTong_BASE 0
#define ADDR_ParaXiTong ADDR_ParaXiTong_BASE+0           //Bag4 //共24个:系统参数起始地址(有密码)
#define ADDR_ParaDanMianDMBC ADDR_ParaXiTong_BASE+0x50     //Bag4 //共8个  :单面度目补偿[0]后左,[1]后右,[2]前左,[3]前右
#define ADDR_ParaSiPinDMBC ADDR_ParaXiTong_BASE+0x160    //Bag4 //共8个  :四平度目补偿[0]后左,[1]后右,[2]前左,[3]前右
#define ADDR_ParaLingWeiDMBC ADDR_ParaXiTong_BASE+0x170  //Bag4 //共8个  :不工作度目零位补偿[0]后左,[1]后右,[2]前左,[3]前右
#define ADDR_ParaYaoCuang ADDR_ParaXiTong_BASE+0x180     //Bag4 //共33个:摇床位置修正,33个
#define ADDR_ParaFanZenYC ADDR_ParaXiTong_BASE+0x200     //Bag4 //共16个:翻针摇床位置修正
#define ADDR_ParaFanZenZYC ADDR_ParaXiTong_BASE+0x220    //Bag4 //共16个:+翻针摇床位置修正
#define ADDR_ParaFanZenFYC ADDR_ParaXiTong_BASE+0x240    //Bag4 //共16个:-翻针摇床位置修正
#define ADDR_ParaJiQiGongZuo ADDR_ParaXiTong_BASE+0x260  //Bag1 //共34个:机器工作参数起始地址



class QSysLogModel;



/////////////////////////////////////
class ProgramEncrypt{
private:
    static const unsigned char  EncryptTable[256];
public:
    void Decrypt(unsigned char *buf,unsigned short len);
};







class QPatModel :public QAbstractTableModel{
    Q_OBJECT
public:

    QPatModel( QPatternData *pattern,QObject * parent = 0);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant headerData(int section, Qt::Orientation orientation,int role = Qt::DisplayRole )const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool resetBuf();
signals:

private:
    QPatternData *pattern;
    int bits10;
};

class QDMZModel :public QAbstractTableModel{
    Q_OBJECT
public:
    QDMZModel( QPatternData *data ,QObject * parent = 0 ):QAbstractTableModel(parent),patterndata(data){
    }
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
private:
    QPatternData *patterndata;

};



class QMdItemDelegate :public QStyledItemDelegate{
    Q_OBJECT
public:
    enum ValidatorType{IntValidator=0,DoubleValidator=1,RegExpValidator=2};
    QMdItemDelegate( QObject * parent = 0):QStyledItemDelegate(parent),isitemsimplepaint(FALSE){
    }
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    //virtual QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    //virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
    void setRange(int bottom,int top,int row = -1,int column =-1);
    void setRegExp(QString regexp,int row = -1,int column = -1);
    virtual void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    void setItemSimplePaint(bool val);
    mutable QPointer<QLineEdit > editor;
signals:
    void editing(QModelIndex index)const;
public slots:
private:
    typedef struct _ValidatorStruct{
        ValidatorType validatortype;
        int bottom;
        int top;
        QString regexp;
    } ValidatorStruct;
    bool isitemsimplepaint;
    mutable QHash<QPair<int,int> ,ValidatorStruct > validatorhash;
    mutable QIntValidator intv;
    mutable QRegExpValidator regexpv;
    mutable QDoubleValidator doublev;
    mutable QString oldval;
    bool fechValidatorstruct(ValidatorStruct &val,int row,int column) const;
};


class QLLModel :public QAbstractTableModel{
    Q_OBJECT
public:
    QLLModel( QPatternData *data ,QObject * parent = 0 ):QAbstractTableModel(parent),
                    patterndata(data){}
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QPatternData *patterndata;
};


class QZDJModel :public QAbstractTableModel{
    Q_OBJECT
public:
    QZDJModel( QPatternData *data ,QObject * parent = 0 ):QAbstractTableModel(parent),
                               patterndata(data)  {}
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    //QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QPatternData *patterndata;
};


class QSSModel :public QAbstractTableModel{
    Q_OBJECT
public:
    QSSModel( QPatternData *data ,QObject * parent = 0 ):QAbstractTableModel(parent),
                patterndata(data)  {}
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    //QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QPatternData *patterndata;
};

class QSZTFModel :public QAbstractTableModel{
    Q_OBJECT
public:
    QSZTFModel( QPatternData *data ,QObject * parent = 0 ):QAbstractTableModel(parent),
                  patterndata(data)  {}
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    //QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QPatternData *patterndata;
};

class QYTXSZXZModel :public QAbstractTableModel{
    Q_OBJECT
public:
    QYTXSZXZModel( QPatternData *data ,QObject * parent = 0 ):QAbstractTableModel(parent),
                patterndata(data)
    {
    }
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QPatternData *patterndata;
private:
};


class QYTXSZTFModel :public QAbstractTableModel{
    Q_OBJECT
public:
    QYTXSZTFModel( QPatternData *data ,QObject * parent = 0 ):QAbstractTableModel(parent),
                                         patterndata(data)  {}
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    //QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QPatternData *patterndata;
};

class QCJPModel :public QAbstractTableModel{
    Q_OBJECT
public:
    QCJPModel( QPatternData *data ,QObject * parent = 0 ):QAbstractTableModel(parent),
                           patterndata(data) {}
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    //QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QPatternData *patterndata;
};


class QPZKModel :public QAbstractTableModel{
    Q_OBJECT
public:
    QPZKModel( QPatternData *data,int zkcount=8,QObject * parent = 0 ):
                    QAbstractTableModel(parent),patterndata(data),count(zkcount){}
    void modelReset();
    int	rowCount ( const QModelIndex & parent = QModelIndex() ) const ;
    int columnCount(const QModelIndex &parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    //QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QPatternData *patterndata;
private:
    mutable unsigned short count;
};



class QCntLoopModel:public QAbstractTableModel{
    Q_OBJECT
public:
    QCntLoopModel( QPatternData *data ,QObject * parent = 0 );
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool insertRows(int row, int count,const QModelIndex &parent);
    bool removeRows(int row, int count,const QModelIndex &parent);
    void save();
    void resetVal();
signals:
    void datasValid(bool);
private:
    struct LoopType{
        int startline;
        int endline;
        int numofloop;
    };
    QList<struct LoopType> looplist;
    QPatternData *patterndata;
    bool checkdatavalid();
};



class QSzkbModel:public QAbstractTableModel{
     Q_OBJECT
public:
   struct SzkbData{
        unsigned short ZuSa;
        unsigned short FuSa; //纱嘴号1~8
        unsigned short Start;
        unsigned short End;
    };
   QSzkbModel(QPatternData *pattern ,QObject * parent = 0):QAbstractTableModel(parent),
                patterndata(pattern),szkblist(pattern->sazbuf) {
   }
   void saveToFile();
   int rowCount(const QModelIndex &parent) const;
   int columnCount(const QModelIndex &parent) const;
   QVariant headerData(int section, Qt::Orientation orientation, int role) const;
   QVariant data(const QModelIndex &index, int role) const;
   bool setData(const QModelIndex &index, const QVariant &value, int role);
   Qt::ItemFlags flags(const QModelIndex &index) const;
   bool insertRows(int row, int count,const QModelIndex &parent);
   bool removeRows(int row, int count,const QModelIndex &parent);
signals:
   void datasValid(bool);
private:
   QPatternData *patterndata;
   QList<QPatternData::SzkbData> szkblist;
   bool checkdatavalid();
};


class QTingcheModel:public QAbstractTableModel{
     Q_OBJECT
public:
   QTingcheModel(QPatternData *pattern ,QObject * parent = 0);
   void saveToFile();
   int rowCount(const QModelIndex &parent) const;
   int columnCount(const QModelIndex &parent) const;
   QVariant headerData(int section, Qt::Orientation orientation, int role) const;
   QVariant data(const QModelIndex &index, int role) const;
   bool setData(const QModelIndex &index, const QVariant &value, int role);
   Qt::ItemFlags flags(const QModelIndex &index) const;
   bool insertRows(int row, int count,const QModelIndex &parent);
   bool removeRows(int row, int count,const QModelIndex &parent);
signals:
   void datasValid(bool);
private:
   QPatternData *patterndata;
   QList<unsigned short > cntrows;
   bool checkdatavalid();
};


class QDMBCModel :public QAbstractTableModel{
    Q_OBJECT
public:
    QDMBCModel( QParam *data ,QObject * parent = 0 ):QAbstractTableModel(parent),param(data){
    }
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
private:
    QParam *param;

};

/////////////摇床位置修正/////////////////////////////
class QYCWZXZModel :public QAbstractTableModel{
    Q_OBJECT
public:
    QYCWZXZModel( QParam *data ,QObject * parent = 0 ):QAbstractTableModel(parent),
                    paramadata(data){}
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QParam *paramadata;
};

/////////////翻针摇床位置修正/////////////////////////////
class QFZYCWZXZModel :public QAbstractTableModel{
    Q_OBJECT
public:
    QFZYCWZXZModel( QParam *data ,QObject * parent = 0 ):QAbstractTableModel(parent),
                    param(data){}
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags ( const QModelIndex & index ) const ;
    QParam *param;
};

/*////////////机器工作参数/////////////////////////////
class QJQGZCSModel :public QAbstractTableModel{
    Q_OBJECT
public:
    enum DataType{BOOL,INT};
    QJQGZCSModel( QParam *data ,QObject * parent = 0 );
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
private:
    QParam *paramadata;
    QStringList namelist;
    int range[30][2];
    DataType datatype[30];

};*/





class QMdFileNode{
public:
    QMdFileNode():  DisplayName(""),cntFilePath(""),patFilePath(""),
                    wrkFilePath(""),sazFilePath(""),prmFilePath(""),
                    wight(0),hight(0),parent(NULL){

    }

    ~QMdFileNode(){
        qDeleteAll(children);
    }
    enum FileInclude{CNTFILE=0x01,PATFILE=0x02,WRKFILE=0X04,SAZFILE=0X08,PRMFILE=0X10};
    Q_DECLARE_FLAGS(FileIncludeFlag, FileInclude);
    bool dirOrFile;
    FileIncludeFlag fileIncludeFlag;
    QString DisplayName;
    QString cntFilePath;
    QString patFilePath;
    QString wrkFilePath;
    QString sazFilePath;
    QString prmFilePath;
    QDateTime modifyTime;
    unsigned short wight;
    unsigned short hight;
    QMdFileNode *parent;
    QList<QMdFileNode *> children;
};






class QMdSortFilterProxyModel:public QSortFilterProxyModel{
    Q_OBJECT
public:
    QMdSortFilterProxyModel(QObject *parent =0):QSortFilterProxyModel(parent),psrcmodel_colomn0list(new QStringList),
                                                psrcparentindex(new QModelIndex())
                                               {
    }
    ~QMdSortFilterProxyModel(){
        delete psrcmodel_colomn0list;
        delete psrcparentindex;

    }

    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    QString upPath(const QModelIndex &index)const;
    QString preName(const QModelIndex &index)const;
    QString patternName(const QModelIndex &index,const QString &suffname="")const;
    Md::HAVEFILEFLAG haveFile(const QModelIndex &index);
    QString haveFileString(const QModelIndex &index);
    QSize patternSize(const QModelIndex &index)const;
    QString currentDirPath(const QModelIndex &parentindex);
    void copy(const QModelIndex &srcindex, QMdSortFilterProxyModel &desmodel,const QModelIndex &desindex,QWidget *parent);
    //void active(const QModelIndex index);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex & source_parent ) const;


private:
    QString preName(const QString &name)const;
    QString suffName(const QString &name)const;
    QStringList *psrcmodel_colomn0list;
    QModelIndex *psrcparentindex;
    void copy(const QString &srcpath,const QString &despath,QWidget *parent);

};


#endif // DATA_H



