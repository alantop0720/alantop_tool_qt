﻿#include "widget.h"
#include "ui_widget.h"
#include <QStringList>
#include <QSqlDatabase>
#include <string>
#include <QMessageBox>
#include  <QSqlQuery>

#include <QDragEnterEvent>
#include <QUrl>
#include <QFile>
#include <QTextStream>
#include <QMimeData>
#include <QFileInfo>

#include <QProcess>
#include <QDir>


#include <QDesktopServices>

#include <QFileDialog>



class QDetachableProcess
        : public QProcess {
public:
    QDetachableProcess(QObject *parent = 0)
        : QProcess(parent) {
    }
    void detach() {
        waitForStarted();
        setProcessState(QProcess::NotRunning);
    }
};




Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);


    initdb();



    //initleft();

    //table显示数据
    on_pushButton_clicked();


    setAcceptDrops(true);

    setWindowTitle("alantop_tool");

    ui->lineEdit_newdir->setText("UPDATE app SET path = REPLACE(path, 'D:/alantop_dir/Alantop_Tool/Tools/', 'd:/newdir/') WHERE id = 105");


    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    //设置tableview一次只能可选一行
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotContextMenu(QPoint)));



    QPalette label_palette;
    label_palette.setColor(QPalette::Window, Qt::white);
    label_palette.setColor(QPalette::WindowText, Qt::red);
    ui->label_info->setAutoFillBackground(true);
    ui->label_info->setPalette(label_palette);
    QFont font ( "Microsoft YaHei", 10, 75); //第一个属性是字体（微软雅黑），第二个是大小，第三个是加粗（权重是75）
    ui->label_info->setFont(font);
}

Widget::~Widget()
{
    db.close();
    delete ui;
}


void Widget::initdb()
{
    QStringList drivers = QSqlDatabase::drivers();

    foreach(QString driver, drivers)
        qDebug() << driver;

    //QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "con");

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("my.db");
    if(!db.open())
    {
        QMessageBox::critical(0, "can not open alantop_tool.db", "unable to establish a database connection.", QMessageBox::Cancel);

    }

    //QSqlQuery query(db);
    //query.exec("select * from app");


    //    QSqlQuery query("select * from app");

    //    while (query.next())
    //    {
    //        qDebug() << query.value(0).toInt() << query.value(1).toString();
    //    }

}

void Widget::initleft()
{
    QSqlQuery query;
    QStringList strList;

    query.exec("select type,count(*) as '统计结果' from app group by type");
    while(query.next())
    {
        QString id = query.value(0).toString();

        //qDebug()<<id;
        strList.append(id);
    }

    ItemModel = new QStandardItemModel(this);

    /*QStringList strList;
          strList.append("网络");
          strList.append("文本编辑");
          strList.append("驱动");
          strList.append("远程");
          strList.append("开发调试");
          strList.append("系统工具");
          strList.append("截图录屏");
          strList.append("文件共享");
          strList.append("外网");
          strList.append("图片查看");
          strList.append("数据库");
          strList.append("数据转发");*/


    int nCount = strList.size();
    for(int i = 0; i < nCount; i++)
    {
        QString string = static_cast<QString>(strList.at(i));
        QStandardItem *item = new QStandardItem(string);
        ItemModel->appendRow(item);
    }
    ui->listView->setModel(ItemModel);




    connect(ui->listView,SIGNAL(clicked(QModelIndex)),this,SLOT(showClick(QModelIndex)));


}

void Widget::on_pushButton_clicked()
{
    initleft();


    //        QSqlQuery query;
    //        QString str = QString("select * from app");
    //        model.setQuery(str);
    //        ui->tableView->setModel(&model);

    query(ui->lineEdit_search->text());
}


void Widget::showClick(QModelIndex index)
{
    QString strTemp;
    strTemp = index.data().toString();


    ui->lineEdit_type->setText(strTemp);

    //    ui->lineEdit_id->setText("");
    //    ui->lineEdit_title->setText("");
    //    ui->lineEdit_count->setText("");


    querytype(strTemp);





}

void Widget::query(QString search)
{
    QSqlQuery query;
    QString str = QString("select * from app where type like '%%1%' or name like '%%1%' or filename like '%%1%' or path like '%%1%' or remark like '%%1%' ").arg(search);
    model.setQuery(str);
    ui->tableView->setModel(&model);

    ui->tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(5,QHeaderView::ResizeToContents);

    ui->label_info->setText(str);
}


void Widget::querytype(QString search)
{
    QSqlQuery query;
    QString str = QString("select * from app where type like '%%1%' ").arg(search);
    model.setQuery(str);
    ui->tableView->setModel(&model);

    ui->tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(5,QHeaderView::ResizeToContents);

    ui->label_info->setText(str);
}


void Widget::dragEnterEvent(QDragEnterEvent *event) // 拖动进入事件
{
    if(event->mimeData()->hasUrls())                    // 数据中是否包含URL
        event->acceptProposedAction();                  // 如果是则接收动作
    else event->ignore();                               // 否则忽略该事件
}
void Widget::dropEvent(QDropEvent *event)           // 放下事件
{
    const QMimeData *mimeData = event->mimeData();      // 获取MIME数据
    if(mimeData->hasUrls()){                            // 如果数据中包含URL
        QList<QUrl> urlList = mimeData->urls();         // 获取URL列表
        // 将其中第一个URL表示为本地文件路径
        QString fileName = urlList.at(0).toLocalFile();

        ui->lineEdit_path->setText(fileName);  // 将文件中所有内容读入编辑器


        QFileInfo fi(fileName);


        ui->lineEdit_filename->setText(fi.fileName());

        ui->lineEdit_name->setText(fi.baseName());

        qDebug() << fi.filePath();


        QString suffix =   fi.suffix();

        if(suffix.compare(QString::fromLocal8Bit("lnk")) == 0)
        {
            qDebug() << "compare相同";
            QFileInfo fileinfo(fileName);
            QString pro = fileinfo.canonicalFilePath();

            ui->lineEdit_path ->setText(pro);

            QFileInfo fi_temp(pro);

            ui->lineEdit_filename->setText(fi_temp.fileName());
            ui->lineEdit_name->setText(fi_temp.baseName());


        }




        //        if(!fileName.isEmpty()){                        // 如果文件路径不为空
        //            QFile file(fileName);     // 建立QFile对象并且以只读方式打开该文件
        //            if(!file.open(QIODevice::ReadOnly)) return;
        //            QTextStream in(&file);                      // 建立文本流对象
        //            ui->lineEdit_path->setText(in.readAll());  // 将文件中所有内容读入编辑器
        //        }
    }
}

void Widget::on_pushButton_insert_clicked()
{
    QSqlQuery query;
    //int id = ui->lineEdit_id->text().toInt();

    QString name = ui->lineEdit_name->text();
    QString filename = ui->lineEdit_filename->text();
    QString path = ui->lineEdit_path->text();
    QString type = ui->lineEdit_type->text();
    QString remark = ui->lineEdit_remark->text();


    QDateTime curDateTime=QDateTime::currentDateTime();

    QString str = QString("insert into app (name, filename,path,type,remark) values('%1', '%2','%3','%4','%5')")
            .arg(name).arg(filename).arg(path).arg(type).arg(remark);

    if (query.exec(str) == false)
    {
        qDebug() << str;
        ui->label_info->setText(str +  curDateTime.toString("yyyy-MM-dd hh:mm:ss") );
    }
    else
    {
        qDebug() << "insert ok";
        ui->label_info->setText("insert ok " +  curDateTime.toString("yyyy-MM-dd hh:mm:ss"));
    }

    querytype(ui->lineEdit_type->text());


}


void Widget::on_tableView_clicked(const QModelIndex &index)
{
    //左键点击 显示

    //读取数据到lineedit控件
    QAbstractItemModel *Imodel=ui->tableView->model();
    QModelIndex Iindex ;
    QVariant datatemp;
    QString name;


    Iindex = Imodel->index(index.row(),0);//index.row()为算选择的行号。1为所选中行的第一列。。
    datatemp=Imodel->data(Iindex);
    name=datatemp.toString();//name即为所选择行的第一列的值。。。
    ui->lineEdit_id->setText(name);

    Iindex = Imodel->index(index.row(),1);//index.row()为算选择的行号。1为所选中行的第一列。。
    datatemp=Imodel->data(Iindex);
    name=datatemp.toString();//name即为所选择行的第一列的值。。。
    ui->lineEdit_name->setText(name);


    Iindex = Imodel->index(index.row(),2);//index.row()为算选择的行号。1为所选中行的第一列。。
    datatemp=Imodel->data(Iindex);
    name=datatemp.toString();//name即为所选择行的第一列的值。。。
    ui->lineEdit_filename->setText(name);



    Iindex = Imodel->index(index.row(),3);//index.row()为算选择的行号。1为所选中行的第一列。。
    datatemp=Imodel->data(Iindex);
    name=datatemp.toString();//name即为所选择行的第一列的值。。。
    ui->lineEdit_path->setText(name);
    QString app_path = name;

    Iindex = Imodel->index(index.row(),4);//index.row()为算选择的行号。1为所选中行的第一列。。
    datatemp=Imodel->data(Iindex);
    name=datatemp.toString();//name即为所选择行的第一列的值。。。
    ui->lineEdit_type->setText(name);

    Iindex = Imodel->index(index.row(),5);//index.row()为算选择的行号。1为所选中行的第一列。。
    datatemp=Imodel->data(Iindex);
    name=datatemp.toString();//name即为所选择行的第一列的值。。。
    ui->lineEdit_remark->setText(name);





    QFileInfo fileInfo(app_path);
    if(!fileInfo.isFile())
    {
        ui->label_info->setText("文件不存在");
        return;
    }



}


void Widget::on_pushButton_modify_clicked()
{
    QDateTime curDateTime=QDateTime::currentDateTime();
    QSqlQuery query;
    int id = ui->lineEdit_id->text().toInt();
    if (0 == id)
    {
        ui->label_info->setText("id为空."+ curDateTime.toString("yyyy-MM-dd hh:mm:ss") );
        return;
    }

    QString name = ui->lineEdit_name->text();
    QString filename = ui->lineEdit_filename->text();
    QString path = ui->lineEdit_path->text();
    QString type = ui->lineEdit_type->text();
    QString remark = ui->lineEdit_remark->text();

    //如果有引号，需要引号替换功能 加转义符

    QString str = QString("update app set name = '%1', filename ='%2',path='%3',type='%4', remark='%5' where id=%6").arg(name).arg(filename).arg(path).arg(type).arg(remark).arg(id);

    if (query.exec(str) == false)
    {
        qDebug() << str ;

        ui->label_info->setText("更新失败\n" + str + curDateTime.toString("yyyy-MM-dd hh:mm:ss"));
        //ui->label_info->setText("更新失败");

    }
    else
    {
        qDebug() << "update ok " ;
        ui->label_info->setText("update ok."+ curDateTime.toString("yyyy-MM-dd hh:mm:ss") );
    }

    querytype(ui->lineEdit_type->text());

}


void Widget::on_pushButton_2_clicked()
{
    QDateTime curDateTime=QDateTime::currentDateTime();


    if (QMessageBox::question(this,"info","请确认旧目录和新目录，确认是否更新程序目录，are you ok?", QMessageBox::Yes|QMessageBox::No) == QMessageBox::No)
    {
        ui->label_info->setText("no update. "+ curDateTime.toString("yyyy-MM-dd hh:mm:ss") );
        return;
    }

    //query.exec("select * from app");
    QSqlQuery query;

    if (!query.exec(ui->lineEdit_newdir->text()))
    {
        //ui->label_info->setText("update new dir ok."+ curDateTime.toString("yyyy-MM-dd hh:mm:ss") );

        ui->label_info->setText(query.lastError().text());
        qDebug () << "sql errror" <<  query.lastError();
    }

    ui->label_info->setText("update new dir ok."+ curDateTime.toString("yyyy-MM-dd hh:mm:ss") );



}


void Widget::on_pushButton_path_clicked()
{


    QString path = ui->lineEdit_path->text();

    QFileInfo fi(path);

    qDebug() << fi.canonicalPath() ;

    QDesktopServices::openUrl(QUrl::fromLocalFile(fi.canonicalPath()));
}


void Widget::slotContextMenu(QPoint pos)
{
    qDebug() << "right" ;
    QModelIndex index = ui->tableView->indexAt(pos);

    if (index.isValid())
    {
        //读取数据到lineedit控件
        QAbstractItemModel *Imodel=ui->tableView->model();
        QModelIndex Iindex = Imodel->index(index.row(),1);//index.row()为算选择的行号。1为所选中行的第一列。。
        QVariant datatemp=Imodel->data(Iindex);
        QString name=datatemp.toString();//name即为所选择行的第一列的值。。。
        qDebug() << name;
        ui->lineEdit_name->setText(name);

        Iindex = Imodel->index(index.row(),0);//index.row()为算选择的行号。1为所选中行的第一列。。
        datatemp=Imodel->data(Iindex);
        name=datatemp.toString();//name即为所选择行的第一列的值。。。
        ui->lineEdit_id->setText(name);

        Iindex = Imodel->index(index.row(),1);//index.row()为算选择的行号。1为所选中行的第一列。。
        datatemp=Imodel->data(Iindex);
        name=datatemp.toString();//name即为所选择行的第一列的值。。。
        ui->lineEdit_name->setText(name);


        Iindex = Imodel->index(index.row(),2);//index.row()为算选择的行号。1为所选中行的第一列。。
        datatemp=Imodel->data(Iindex);
        name=datatemp.toString();//name即为所选择行的第一列的值。。。
        ui->lineEdit_filename->setText(name);



        Iindex = Imodel->index(index.row(),3);//index.row()为算选择的行号。1为所选中行的第一列。。
        datatemp=Imodel->data(Iindex);
        name=datatemp.toString();//name即为所选择行的第一列的值。。。
        ui->lineEdit_path->setText(name);
        QString app_path = name;

        Iindex = Imodel->index(index.row(),4);//index.row()为算选择的行号。1为所选中行的第一列。。
        datatemp=Imodel->data(Iindex);
        name=datatemp.toString();//name即为所选择行的第一列的值。。。
        ui->lineEdit_type->setText(name);

        Iindex = Imodel->index(index.row(),5);//index.row()为算选择的行号。1为所选中行的第一列。。
        datatemp=Imodel->data(Iindex);
        name=datatemp.toString();//name即为所选择行的第一列的值。。。
        ui->lineEdit_remark->setText(name);
    }

}





void Widget::on_tableView_doubleClicked(const QModelIndex &index)
{
    //左键双击 启动程序

    //读取数据到lineedit控件
    QAbstractItemModel *Imodel=ui->tableView->model();
    QModelIndex Iindex ;
    QVariant datatemp;
    QString name;


    Iindex = Imodel->index(index.row(),0);//index.row()为算选择的行号。1为所选中行的第一列。。
    datatemp=Imodel->data(Iindex);
    name=datatemp.toString();//name即为所选择行的第一列的值。。。
    ui->lineEdit_id->setText(name);

    Iindex = Imodel->index(index.row(),1);//index.row()为算选择的行号。1为所选中行的第一列。。
    datatemp=Imodel->data(Iindex);
    name=datatemp.toString();//name即为所选择行的第一列的值。。。
    ui->lineEdit_name->setText(name);


    Iindex = Imodel->index(index.row(),2);//index.row()为算选择的行号。1为所选中行的第一列。。
    datatemp=Imodel->data(Iindex);
    name=datatemp.toString();//name即为所选择行的第一列的值。。。
    ui->lineEdit_filename->setText(name);



    Iindex = Imodel->index(index.row(),3);//index.row()为算选择的行号。1为所选中行的第一列。。
    datatemp=Imodel->data(Iindex);
    name=datatemp.toString();//name即为所选择行的第一列的值。。。
    ui->lineEdit_path->setText(name);
    QString app_path = name;

    Iindex = Imodel->index(index.row(),4);//index.row()为算选择的行号。1为所选中行的第一列。。
    datatemp=Imodel->data(Iindex);
    name=datatemp.toString();//name即为所选择行的第一列的值。。。
    ui->lineEdit_type->setText(name);

    Iindex = Imodel->index(index.row(),5);//index.row()为算选择的行号。1为所选中行的第一列。。
    datatemp=Imodel->data(Iindex);
    name=datatemp.toString();//name即为所选择行的第一列的值。。。
    ui->lineEdit_remark->setText(name);





    QFileInfo fileInfo(app_path);
    if(!fileInfo.isFile())
    {
        ui->label_info->setText("文件不存在");
        return;
    }



    QProcess *myProcess = new QProcess(this);

    QFileInfo fileinfo = QFileInfo(app_path);

    QString filepath = fileinfo.absolutePath();


    QDir dir(filepath);
    if(!dir.exists())
    {

        ui->label_info->setStyleSheet("color:red;");
        ui->label_info->setText("路径不存在");
        return ;
    }

    qDebug() << QDir::currentPath();

    qDebug() << "filepath = " << filepath;

    //        qDebug() << "workingDirectory=" ;

    //        qDebug() << myProcess->workingDirectory();

    //        myProcess->setWorkingDirectory(filepath);

    //        qDebug() << "workingDirectory=" << myProcess->workingDirectory();

    //        QMessageBox msgBox1;
    //        msgBox1.setText(myProcess->workingDirectory());
    //        msgBox1.exec();


    QString oldCurrent = QDir::currentPath();
    QDir::setCurrent(filepath);

    qDebug() << "oldCurrent=" << oldCurrent;



    //         QMessageBox msgBox;
    //         msgBox.setText(QDir::currentPath());
    //         msgBox.exec();


    qDebug() << QDir::currentPath();

    //转斜杠的方法
    //QString cmd =QDir::fromNativeSeparators("d:\\1 1\\中文.exe");
    //qDebug()<<cmd;

    QString cmd = "\"" + app_path + "\"";
    qDebug() << cmd;

    //        qDebug() << "processEnvironment ="<<(myProcess->processEnvironment()).toStringList();

    //        //QProcess process;
    //        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    //        env.insert("path",  filepath); // Add an environment variable
    //        myProcess->setProcessEnvironment(env);
    //        //process.start("myapp");

    //        QStringList list = (myProcess->processEnvironment()).toStringList();
    //        qDebug() << "list.size=" << list.size();

    //        for(int i = 0; i< list.size();++i)
    //        {
    //            QString tmp = list.at(i);
    //            qDebug()<<"tmp ="<< i << tmp;
    //        }



    myProcess->startDetached(cmd);

    qDebug() << "setCurrent=" << QDir::setCurrent(oldCurrent);
}


void Widget::on_pushButton_del_clicked()
{
    QSqlQuery query;
    int id = ui->lineEdit_id->text().toInt();

    if (QMessageBox::question(this,"info","are you ok?", QMessageBox::Yes|QMessageBox::No) == QMessageBox::No)
    {
        return;
    }

    QString str = QString ("delete from app where id =%1").arg(id);

    if (query.exec(str) == false)
    {
        qDebug() << str;
        ui->label_info->setText(str);
    }
    else
    {
        qDebug() << "delete ok";
        ui->label_info->setText("delete ok");
    }

    querytype(ui->lineEdit_type->text());
}


void Widget::on_pushButton_import_clicked()
{
    QString strText;
    QModelIndexList indexList = ui->listView->selectionModel()->selectedIndexes();
    if (!indexList.isEmpty())
    {
        QModelIndex index = indexList.first();
        strText = index.data(Qt::DisplayRole).toString();
        qDebug() << "The selected text is: " << strText;


    }
    else
    {
        QMessageBox::information(this, tr("提示"), tr("未选中导出的分类"));
        return;

    }

    QString sql = "SELECT * FROM app WHERE type = '";
    sql.append(strText);
    sql.append("'");


    QSqlQuery query;
    query.prepare(sql);
    query.exec();

    // 创建csv文件
    QFile csvFile(strText.append(".txt"));
    if (!csvFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    // 写入csv文件头
    QTextStream out(&csvFile);
    QStringList strList;

    while(query.next())
    {
        strList.append(query.value(1).toString());
        //strList.join(",");
        strList.append(query.value(2).toString());
        //strList.append(",");
        strList.append(query.value(3).toString());
        //strList.append(",");
        strList.append(query.value(4).toString());
        //strList.append(",");
        strList.append(query.value(5).toString());
        //strList.append(",");
        //out << strList.join("\n");


    }

    out << strList.join('\n');





    csvFile.close();

    QMessageBox::information(this, strText, tr("导出成功！"));


}


void Widget::on_pushButton_export_clicked()
{
    //从文本文件导入数据库
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "/home",
                                                    tr("Text Files (*.txt)"));//按行读取文件


    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Open failed.";
    }
    QTextStream txtInput(&file);
    while(!txtInput.atEnd())
    {
        QString value1 = txtInput.readLine();
        QString value2 = txtInput.readLine();
        QString value3 = txtInput.readLine();
        QString value4 = txtInput.readLine();
        QString value5 = txtInput.readLine();
        //插入sqlite
        QSqlQuery query;
        query.prepare("INSERT INTO app(name, filename, path, type, remark) VALUES(?, ?, ?, ?, ?)");
        query.addBindValue(value1);
        query.addBindValue(value2);
        query.addBindValue(value3);
        query.addBindValue(value4);
        query.addBindValue(value5);
        query.exec();
    }
    file.close();

    QMessageBox::information(this, "提示", tr("导入完成！"));
}


void Widget::on_pushButton_deltype_clicked()
{
    //删除分类代码

    if (QMessageBox::question(this, "info", "是要选择删除的分类吗？", QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
    {
        return;
    }

    QString strText;
    QModelIndexList indexList = ui->listView->selectionModel()->selectedIndexes();
    if (!indexList.isEmpty())
    {
        QModelIndex index = indexList.first();
        strText = index.data(Qt::DisplayRole).toString();
        qDebug() << "The selected text is: " << strText;


    }
    else
    {
        QMessageBox::information(this, tr("提示"), tr("未选中导出的分类"));
        return;

    }

    //QSqlQuery query;
    //query.prepare("delete from tableName where id= ?");
    //query.addBindValue("5");
    //query.exec();


    QSqlQuery query;
    query.prepare("DELETE FROM app WHERE type = :type");
    query.bindValue(":type", strText);
    bool result = query.exec();
    qDebug() << "result: " << result;

    QMessageBox::information(this, "提示", tr("删除选择的分类！"));

}

