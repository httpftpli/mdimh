#include "qmytablewidget.h"
#include <QIcon>
#include <QApplication>
#include <QMessageBox>
#include <QDebug>
extern QApplication a;



QMyTableWidget::QMyTableWidget(QWidget *parent):QTableWidget(parent){

}

void QMyTableWidget::SetItems(QVector<MyFileinfo> fileinfoVector){
    int count = fileinfoVector.size();
    itemVectorArray[0].clear();
    itemVectorArray[1].clear();
    itemVectorArray[2].clear();
    for(int i=0;i<count;i++){
       if(fileinfoVector[i].isDir){
          QTableWidgetItem item(QTableWidgetItem::UserType+1);
          item.setText(fileinfoVector[i].dispname);
           QIcon icon = a.style()->standardIcon(QStyle::SP_DirIcon);
           item.setIcon(icon);
           itemVectorArray[0]<<item;
           QTableWidgetItem datatemp;
           itemVectorArray[1]<<datatemp;
           itemVectorArray[2]<<datatemp;
       }
       else{
         QTableWidgetItem item(QTableWidgetItem::UserType);
         item.setText(fileinfoVector[i].dispname);
         QIcon icon = a.style()->standardIcon(QStyle::SP_FileIcon);
         item.setIcon(icon);
         itemVectorArray[0]<<item;
         QTableWidgetItem datatemp;
         datatemp.setText(fileinfoVector[i].datetime.toString("yyyy.MM.dd  hh:mm:ss"));
         itemVectorArray[1]<<datatemp;
         QString str=fileinfoVector[i].cntfilename.right(4)+" "
                 +fileinfoVector[i].patfilename.right(4)+" "
                 +fileinfoVector[i].wrkfilename.right(4);
         datatemp.setText(str);
         itemVectorArray[2]<<datatemp;

       }       
    }
    QTableWidgetItem headItem;
    for(int i =0;i<itemVectorArray[0].size();i++){
        setRowCount(itemVectorArray[0].size());
        headItem.setText(QString::number(i,10));
        headItemVector<<headItem;
        setItem(i,0,&itemVectorArray[0][i]);
        setItem(i,1,&itemVectorArray[1][i]);
        setItem(i,2,&itemVectorArray[2][i]);
    }
}


QMyDirModel::QMyDirModel(QObject *parent):QObject(parent){
    namefilter<<"*.cnt"<<"*.pat"<<"*.wrk";
}



QFileInfoList QMyDirModel::listfileinfo(QDir dir,QDir::Filters Filter){//reenterable 列出符合CNT PAT格式的fileinfo
    if(!qdir.exists()){
        QFileInfoList temp1list;
        return temp1list;
    }
    QFileInfoList list;
    if(Filter&QDir::Dirs)
        list = dir.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot|QDir::NoSymLinks,QDir::Name);//获取目录
    if(Filter&QDir::Files){
        QFileInfoList infolist = dir.entryInfoList(namefilter,QDir::Files,QDir::Name|QDir::IgnoreCase);//获取文件
        int num = infolist.size();
        int i=0;
        while(i<num-1){
            QFileInfo fileinfom = infolist[i++];
            QString rawcntfilename = fileinfom.fileName();          //获取文件名
            QString cntfilename = rawcntfilename.toLower();
            if (cntfilename.right(4)==".cnt"){                      //判断*CNT文件
                QString prename = cntfilename.left(cntfilename.size()-4);   //获取文件名前缀
                QString rawpatfilename = infolist[i].fileName();     //获取后一个文件的文件名
                QString patfilename = rawpatfilename.toLower();      //小写
                if (patfilename == prename+".pat"){                  //判断是不是同名.pat文件
                    list<<infolist.at(i-1);
                    list<<infolist.at(i++);
                    QString rawwrkfilename = infolist[i].fileName();
                    QString wrkfilename = rawwrkfilename.toLower();
                    if(wrkfilename==prename+".wrk"){          //判断第三个文件是是否是同名.wrk文件，如果是索引后移
                        list<<infolist.at(i++);
                    }
                }
            }
        }
    }
    return list;
}

/*QVector<MyFileinfo>  QMyDirModel::listmyfileinfo(QDir dir,QDir::Filter){//reenterable
    QVector<MyFileinfo>  vector;
    if(!qdir.exists()){
        return vector;
    }
    QLinkedList<MyFileinfo> list;
    if(Filter&QDir::Dirs){
        QFileInfo dirlist = dir.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot|QDir::NoSymLinks,QDir::Name);//获取目录
        for(int i=0;i<dirlist.size();i++){
            MyFileinfo fileinfo;
            fileinfo.dispname = dirlist[i];
            fileinfo.isDir = TRUE;
            vector<<fileinfo;
        }
    }
    if(Filter&QDir::Files){
        QFileInfoList infolist = qdir.entryInfoList(namefilter,QDir::Files,QDir::Name|QDir::IgnoreCase);//获取文件
        int num = infolist.size();
        int i=0;
        while(i<num-1){
            QFileInfo fileinfom = infolist[i++];
            QString rawcntfilename = fileinfom.fileName();          //获取文件名
            QString cntfilename = rawcntfilename.toLower();

            if (cntfilename.right(4)==".cnt"){                      //判断*CNT文件
                QString prename = cntfilename.left(cntfilename.size()-4);   //获取文件名前缀
                QString rawpatfilename = infolist[i].fileName();     //获取后一个文件的文件名
                QString patfilename = rawpatfilename.toLower();      //小写
                if (patfilename == prename+".pat"){                  //判断是不是同名.pat文件
                    list<<infolist.at(i-1);
                    list<<infolist.at(i++);
                    QString rawwrkfilename = infolist[i].fileName();
                    QString wrkfilename = rawwrkfilename.toLower();
                    if(wrkfilename==prename+".wrk"){          //判断第三个文件是是否是同名.wrk文件，如果是索引后移
                        list<<infolist.at(i++);
                    }
                }
            }
        }
    }
    return list;

}*/



/*void QMyDirModel::setdir(QString path){
     currentpath = path;
     qdir.setPath(path);
     if(!qdir.exists()){
         qWarning()<<"QMyDirModel::setdir():  dir:"<< path <<"is not exit,can't open";
         return;
     }
     fileinfovector.clear();
     QStringList dirlist = qdir.entryList(QDir::Dirs|QDir::NoDotAndDotDot|QDir::NoSymLinks,QDir::Name);
      for(int i=0;i<dirlist.size();i++){
          MyFileinfo fileinfo;
          fileinfo.dispname = dirlist[i];
          fileinfo.isDir = TRUE;
          fileinfovector<<fileinfo;
      }
      QFileInfoList infolist = qdir.entryInfoList(namefilter,QDir::Files,QDir::Name|QDir::IgnoreCase);
      int num = infolist.size();
      int i=0;
      while(i<num-1){
          QFileInfo fileinfom = infolist[i++];
          QString rawcntfilename = fileinfom.fileName();          //获取文件名                                                 //获取文件名
          QString cntfilename = rawcntfilename.toLower();

          if (cntfilename.right(4)==".cnt"){                      //判断*CNT文件
              QString prename = cntfilename.left(cntfilename.size()-4);   //获取文件名前缀
              QString rawpatfilename = infolist[i].fileName();     //获取后一个文件的文件名
              QString patfilename = rawpatfilename.toLower();      //小写
              if (patfilename == prename+".pat"){                  //判断是不是同名.pat文件
                  MyFileinfo fileinfo;
                  fileinfo.dispname = prename;
                  fileinfo.cntfilename = rawcntfilename;
                  fileinfo.patfilename = rawpatfilename;
                  fileinfo.datetime = fileinfom.lastModified();
                  fileinfo.isDir = FALSE;
                  ///////add//////
                  ///////add////////

                  i++;
                  QString rawwrkfilename = infolist[i].fileName();
                  QString wrkfilename = rawwrkfilename.toLower();
                  if(wrkfilename==prename+".wrk"){          //判断第三个文件是是否是同名.wrk文件，如果是索引后移
                      i++;
                      fileinfo.wrkfilename = rawwrkfilename;
                  }
                  else{
                      fileinfo.wrkfilename = "";
                  }
                  fileinfovector<<fileinfo;                  //写入私有变量
              }
          }
      }
}*/


void QMyDirModel::setdir(QString path){
    currentpath = path;
    qdir.setPath(path);
    if(!qdir.exists()){
        qWarning()<<"QMyDirModel::setdir():  dir:"<< path <<"is not exit,can't open";
        return;
    }
    fileinfovector.clear();
    MyFileinfo myfileinfo;
    QFileInfoList  fileinfolist = listfileinfo(qdir,QDir::Dirs);
    for(int i=0;i<fileinfolist.size();i++){
        myfileinfo.dispname = fileinfolist.at(i).fileName();
        myfileinfo.isDir = TRUE;
        fileinfovector<<myfileinfo;
    }

    fileinfolist = listfileinfo(qdir,QDir::Files);
    int  i = 0;
    while(i<fileinfolist.size()-1){
        myfileinfo.dispname = fileinfolist.at(i).fileName().toLower().left(fileinfolist.at(i).fileName().size()-4);
        myfileinfo.datetime = fileinfolist.at(i).lastModified();
        myfileinfo.isDir = FALSE;
        myfileinfo.cntfilename = fileinfolist.at(i++).fileName();
        myfileinfo.patfilename = fileinfolist.at(i++).fileName();
        if(i==fileinfolist.size()) {
            myfileinfo.wrkfilename = "";
            break;
        }
        if(fileinfolist.at(i).suffix().toLower()=="wrk"){
            myfileinfo.wrkfilename = fileinfolist.at(i++).fileName();
        }else{
            myfileinfo.wrkfilename = "";
        }
        fileinfovector<<myfileinfo;
    }

}





void QMyDirModel::updatadir(){
    setdir(currentpath);
    emit dirupdated(fileinfovector);
}


QStringList QMyDirModel::index(int row){
    QStringList stringlist;
    MyFileinfo myfileinfo;
    myfileinfo = fileinfovector.at(row);
    if(myfileinfo.isDir){
        stringlist<<myfileinfo.dispname;
    }else{
        stringlist<<myfileinfo.cntfilename<<myfileinfo.patfilename;
        if(myfileinfo.wrkfilename != ""){
            stringlist<<myfileinfo.wrkfilename;
        }
    }
    return stringlist;
}



void QMyDirModel::copy_p(QString srcdirname,QString desdirname){
    QDir srcdir(srcdirname);
    if(!srcdir.exists()){
        qWarning()<<"in QMyDirModel::copyfile   srcdirname:"<< srcdirname<< "directory not exist";
        return ;
    }
    QDir desdir(desdirname);
    if(!desdir.exists()){
        qWarning()<<"in QMyDirModel::copyfile   desdir:"<< desdirname<< "directory not exist";
        return ;
    }
    desdir.mkdir(srcdir.dirName());
    QFileInfoList fileinfolist_file = listfileinfo(desdirname+'/'+srcdir.dirName(),QDir::Files); //获取目标文件下的文件列表（不包括文件夹）
    QStringList filenamelist_file;
    for(int j=0;j<fileinfolist_file.size();j++){
       filenamelist_file<<fileinfolist_file.at(j).fileName();
    }                                                         //获取目标文件下的文件名列表（不包括文件夹）
    QFileInfoList fileinfolist = listfileinfo(srcdir);
    for(int i=0;i<fileinfolist.size();i++){
        QFileInfo fileinfo = fileinfolist.at(i);
        if(fileinfo.isDir()){
             copy_p(fileinfo.filePath(),desdir.filePath(srcdir.dirName()));     //递归调用
        }else if(fileinfo.isFile()){



            if(filenamelist_file.contains(fileinfo.fileName(),Qt::CaseInsensitive)){      //判断目标文件夹下是否有同名文件
                int result = QMessageBox::warning ( 0, QObject::tr("文件冲突"),desdirname+"/"+srcdir.dirName()+"/" + QObject::tr("下已包含文件")  +fileinfo.fileName()+QObject::tr("是否覆盖"),
                                                     QMessageBox::Yes|QMessageBox::No, QMessageBox::No );   //弹出对话框
                if(result == QMessageBox::Yes){
                    QFile::remove(fileinfolist_file.at(i).filePath());
                    QFile::copy(fileinfo.filePath(),desdirname+"/"+srcdir.dirName()+"/"+fileinfo.fileName());
                }
            }else{
                QFile::copy(fileinfo.filePath(),desdirname+'/'+srcdir.dirName()+"/"+fileinfo.fileName());
            }
        }
    }
}



void QMyDirModel::copy(int row,QString desdirname){
    MyFileinfo myfileinfo =  fileinfovector.at(row);
    if(myfileinfo.isDir){
       copy_p(currentpath+"/"+myfileinfo.dispname,desdirname) ;
   }else{
       QFileInfoList list = listfileinfo(desdirname,QDir::Files); //获取目标文件下的文件列表（不包括文件夹）
       QStringList filenamelist_file;
       for(int i=0;i<list.size();i++){
          filenamelist_file<<list.at(i).fileName();//获取目标文件下的文件名列表（不包括文件夹）
       }
       if(filenamelist_file.contains(fileinfovector.at(row).cntfilename)){
           int result = QMessageBox::warning ( 0, tr("文件冲突"),desdirname+ tr("下已包含文件")  +fileinfovector.at(row).cntfilename+tr("是否覆盖"),
                                          QMessageBox::Yes|QMessageBox::No, QMessageBox::No );   //弹出对话框
           if (result == QMessageBox::Yes){
               QFile::remove(desdirname+"/"+fileinfovector.at(row).cntfilename);
           }
       }
       if(filenamelist_file.contains(fileinfovector.at(row).patfilename)){
           int result = QMessageBox::warning ( 0, tr("文件冲突"),desdirname+ tr("下已包含文件")  +fileinfovector.at(row).patfilename+tr("是否覆盖"),
                                               QMessageBox::Yes|QMessageBox::No, QMessageBox::No );   //弹出对话框
           if (result == QMessageBox::Yes){
               QFile::remove(desdirname+"/"+fileinfovector.at(row).patfilename);
           }
       }
       if(filenamelist_file.contains(fileinfovector.at(row).wrkfilename)){
           int result = QMessageBox::warning ( 0, tr("文件冲突"),desdirname+ tr("下已包含文件")  +fileinfovector.at(row).wrkfilename+tr("是否覆盖"),
                                               QMessageBox::Yes|QMessageBox::No, QMessageBox::No );   //弹出对话框
           if (result == QMessageBox::Yes){
               QFile::remove(desdirname+"/"+fileinfovector.at(row).wrkfilename);
           }
       }
       QFile::copy(currentpath+"/"+fileinfovector.at(row).cntfilename,desdirname+"/"+fileinfovector.at(row).cntfilename);
       QFile::copy(currentpath+"/"+fileinfovector.at(row).patfilename,desdirname+"/"+fileinfovector.at(row).patfilename);
       QFile::copy(currentpath+"/"+fileinfovector.at(row).wrkfilename,desdirname+"/"+fileinfovector.at(row).wrkfilename);

   }

}


void QMyDirModel::copy(int row,QMyDirModel & desmydirmodel){
    MyFileinfo myfileinfo =  fileinfovector.at(row);
    if(myfileinfo.isDir){
       copy_p(currentpath+"/"+myfileinfo.dispname,desmydirmodel.currentpath) ;
   }else{
       QFileInfoList list = listfileinfo(desmydirmodel.qdir,QDir::Files); //获取目标文件下的文件列表（不包括文件夹）
       QStringList filenamelist_file;
       for(int i=0;i<list.size();i++){
          filenamelist_file<<list.at(i).fileName();//获取目标文件下的文件名列表（不包括文件夹）
       }
       if(filenamelist_file.contains(fileinfovector.at(row).cntfilename)){
           int result = QMessageBox::warning ( 0, tr("文件冲突"),desmydirmodel.currentpath+ tr("下已包含文件")  +fileinfovector.at(row).cntfilename+tr("是否覆盖"),
                                          QMessageBox::Yes|QMessageBox::No, QMessageBox::No );   //弹出对话框
           if (result == QMessageBox::Yes){
               QFile::remove(desmydirmodel.currentpath+"/"+fileinfovector.at(row).cntfilename);
           }
       }
       if(filenamelist_file.contains(fileinfovector.at(row).patfilename)){
           int result = QMessageBox::warning ( 0, tr("文件冲突"),desmydirmodel.currentpath+ tr("下已包含文件")  +fileinfovector.at(row).patfilename+tr("是否覆盖"),
                                               QMessageBox::Yes|QMessageBox::No, QMessageBox::No );   //弹出对话框
           if (result == QMessageBox::Yes){
               QFile::remove(desmydirmodel.currentpath+"/"+fileinfovector.at(row).patfilename);
           }
       }
       if(filenamelist_file.contains(fileinfovector.at(row).wrkfilename)){
           int result = QMessageBox::warning ( 0, tr("文件冲突"),desmydirmodel.currentpath+ tr("下已包含文件")  +fileinfovector.at(row).wrkfilename+tr("是否覆盖"),
                                               QMessageBox::Yes|QMessageBox::No, QMessageBox::No );   //弹出对话框
           if (result == QMessageBox::Yes){
               QFile::remove(desmydirmodel.currentpath+"/"+fileinfovector.at(row).wrkfilename);
           }
       }
       QFile::copy(currentpath+"/"+fileinfovector.at(row).cntfilename,desmydirmodel.currentpath+"/"+fileinfovector.at(row).cntfilename);
       QFile::copy(currentpath+"/"+fileinfovector.at(row).patfilename,desmydirmodel.currentpath+"/"+fileinfovector.at(row).patfilename);
       QFile::copy(currentpath+"/"+fileinfovector.at(row).wrkfilename,desmydirmodel.currentpath+"/"+fileinfovector.at(row).wrkfilename);

   }
   emit desmydirmodel.updatadir();

}




void QMyDirModel::del_p(QFileInfo fileinfo){       //删除所有路径指定的所有文件和文件夹，如果是文件夹递归删除所有子项/
    if(!fileinfo.exists()) return;
    if(fileinfo.isFile()){
        fileinfo.absoluteDir().remove(fileinfo.fileName());
    }
    else if(fileinfo.isDir()){

        QString dirname = fileinfo.filePath();
        QDir dir(dirname);
        dir.setFilter(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);
        dir.setSorting(QDir::DirsLast);
        QFileInfoList list = dir.entryInfoList();
        int i = 0;
        while(i<list.size()){
            del_p(list.at(i++));
        }
        fileinfo.absoluteDir().rmpath(fileinfo.fileName());
    }
}


void QMyDirModel::del(int row){
    if(row==fileinfovector.size()){
        qWarning()<<"in QMyDirModel::del  row:"<<row<<"larger than number of entry";
        return;
    }
    MyFileinfo myfileinfo =fileinfovector.at(row);
    if(myfileinfo.isDir){
        QFileInfo fileinfo(qdir.path()+"/"+myfileinfo.dispname);
        del_p(fileinfo);
    }else{
        QString cntname = myfileinfo.cntfilename;
        QString patname = myfileinfo.patfilename;
        qdir.remove(cntname);
        qdir.remove(patname);
        if(myfileinfo.wrkfilename == "")
            return ;
        else{
            QString wrkname = myfileinfo.wrkfilename;
            qdir.remove(wrkname);
        }
    }
    updatadir();
}



QVector<MyFileinfo> QMyDirModel::listEntry(){
   return fileinfovector;
}

