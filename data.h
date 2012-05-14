

#ifndef DATA_H
#define DATA_H


#include<QString>
#include<QAbstractListModel>
#include<QAbstractTableModel>
#include<QStyledItemDelegate>
#include<QSortFilterProxyModel>
#include<pattern.h>
#include<namespace.h>
#include<QIntValidator>
#include<QPointer>
#include<QDateTime>
#include"constdata.h"
//#include<QApplication>


class QParam;
class QLineEdit;
class QVariant;
class QSysLogModel;



/////////////////////////////////////
class ProgramEncrypt{
private:
    static const char  EncryptTable[256];
public:
    void Decrypt(char *buf,unsigned short len);
};







class QPatModel :public QAbstractTableModel{
    Q_OBJECT
public:

    QPatModel( QPattern *pattern,QObject * parent = 0);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant headerData(int section, Qt::Orientation orientation,int role = Qt::DisplayRole )const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
signals:

private:
    QPattern *pattern;
    int bits10;
};

class QDMZModel :public QAbstractTableModel{
    Q_OBJECT
public:
    QDMZModel( QPattern *data ,QObject * parent = 0 ):QAbstractTableModel(parent),patterndata(data){
        dumuused = patterndata->cnt_dumuUsed();
    }
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
private:
    QPattern *patterndata;
    unsigned int dumuused;

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
    QLLModel( QPattern *data ,QObject * parent = 0 ):QAbstractTableModel(parent),
                    patterndata(data){}
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QPattern *patterndata;
};


class QZDJModel :public QAbstractTableModel{
    Q_OBJECT
public:
    QZDJModel( QPattern *data ,QObject * parent = 0 ):QAbstractTableModel(parent),
                               patterndata(data)  {}
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    //QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QPattern *patterndata;
};


class QSSModel :public QAbstractTableModel{
    Q_OBJECT
public:
    QSSModel( QPattern *data ,QObject * parent = 0 ):QAbstractTableModel(parent),
                patterndata(data)  {}
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    //QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QPattern *patterndata;
};

class QSZTFModel :public QAbstractTableModel{
    Q_OBJECT
public:
    QSZTFModel( QPattern *data ,QObject * parent = 0 ):QAbstractTableModel(parent),
                  patterndata(data)  {}
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    //QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QPattern *patterndata;
};

class QYTXSZXZModel :public QAbstractTableModel{
    Q_OBJECT
public:
    QYTXSZXZModel( QPattern *data ,QObject * parent = 0 ):QAbstractTableModel(parent),
                patterndata(data)
    {
    }
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QPattern *patterndata;
private:
};


class QYTXSZTFModel :public QAbstractTableModel{
    Q_OBJECT
public:
    QYTXSZTFModel( QPattern *data ,QObject * parent = 0 ):QAbstractTableModel(parent),
                                         patterndata(data)  {}
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    //QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QPattern *patterndata;
};

class QCJPModel :public QAbstractTableModel{
    Q_OBJECT
public:
    QCJPModel( QPattern *data ,QObject * parent = 0 ):QAbstractTableModel(parent),
                           patterndata(data) {}
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    //QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QPattern *patterndata;
};


class QPZKModel :public QAbstractTableModel{
    Q_OBJECT
public:
    QPZKModel( QPattern *data,int zkcount=8,QObject * parent = 0 ):
        QAbstractTableModel(parent),patterndata(data),count(zkcount){
        unsigned char shazuiright,shazuileft;
        patterndata->cnt_shaZuiUsed(shazuileft,shazuiright);
        cntshazuiused = shazuileft|shazuiright;
    }
    void modelReset();
    int	rowCount ( const QModelIndex & parent = QModelIndex() ) const ;
    int columnCount(const QModelIndex &parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    //QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QPattern *patterndata;
private:
    mutable unsigned short count;
    unsigned char cntshazuiused;
};



class QCntLoopModel:public QAbstractTableModel{
    Q_OBJECT
public:
    QCntLoopModel( QPattern *data ,QObject * parent = 0 );
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
    QMap<int,QPattern::CntLoopType> loopmap;
    QMap<int,QPattern::CntLoopType> loopmapcopy;
    QPattern *patterndata;
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
   QSzkbModel(QPattern *pattern ,QObject * parent = 0):QAbstractTableModel(parent),
                patterndata(pattern),szkblist(pattern->sazbuf) {
       unsigned char szl,szr;
       patterndata->cnt_shaZuiUsed(szl,szr);
       cntshazuiuse = szl|szr;
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
   unsigned char cntShaZuiUsed();
signals:
   void datasValid(bool);
private:
   QPattern *patterndata;
   QList<QPattern::SzkbData> szkblist;
   bool checkdatavalid();
   unsigned char cntshazuiuse;

};


class QTingcheModel:public QAbstractTableModel{
     Q_OBJECT
public:
   QTingcheModel(QPattern *pattern ,QObject * parent = 0);
   void saveToFile();
   int rowCount(const QModelIndex &parent) const;
   int columnCount(const QModelIndex &parent) const;
   QVariant headerData(int section, Qt::Orientation orientation, int role) const;
   QVariant data(const QModelIndex &index, int role) const;
   //bool setData(const QModelIndex &index, const QVariant &value, int role);
   //Qt::ItemFlags flags(const QModelIndex &index) const;
   //bool insertRows(int row, int count,const QModelIndex &parent);
   bool removeRows(int row, int count,const QModelIndex &parent);
   bool	insertCntRow (unsigned short cntrow);
private:
   QPattern *patterndata;
   QMap<unsigned short,unsigned char > cntrows;
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
    QYCWZXZModel( QParam *data ,QComm *com,QObject * parent = 0 ):QAbstractTableModel(parent),
        paramadata(data),pcomm(com){}
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
private:
    QParam *paramadata;
    QComm *pcomm;
};

/////////////翻针摇床位置修正/////////////////////////////
class QFZYCWZXZModel :public QAbstractTableModel{
    Q_OBJECT
public:
    QFZYCWZXZModel( QParam *data ,QComm *com,QObject * parent = 0 ):QAbstractTableModel(parent),
        param(data),pcomm(com){}
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags ( const QModelIndex & index ) const ;
private:
    QParam *param;
    QComm *pcomm;
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



