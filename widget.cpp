#include "widget.h"
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

#include <QDateTime>

#include <QInputDialog>

#include <ctime>

#include "filecheckerthread.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <ctime>
#include <QDebug>
#include <QString>

#pragma execution_character_set("utf-8")



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
    //checkFilesAndUpdateExistField();



    //initleft();

    //table显示数据
    on_pushButton_clicked();


    setAcceptDrops(true);
    std::string compileDate = __DATE__;
    // 原始日期格式：Mmm dd yyyy，例如：Dec 24 2024

    std::tm tm = {};
    std::istringstream iss(compileDate);
    iss >> std::get_time(&tm, "%b %d %Y");
    if (iss.fail()) {
        std::cerr << "Failed to parse date" << std::endl;
        return ;
    }

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d");
    std::string formattedDate = oss.str();

    // 拼接最终要设置的窗口标题字符串
    std::string fullTitle = "alantop_tool_qt5 " + formattedDate;

    // 将C++的string类型转换为QString类型，用于Qt的窗口标题设置
    QString qtTitle = QString::fromStdString(fullTitle);

    qDebug() << qtTitle;

    // 设置窗口标题
    setWindowTitle(qtTitle);



    //当目录不是规定目录设置新目录

    // 获取应用程序当前运行目录
    QString appDir = QCoreApplication::applicationDirPath();
    QDir dir(appDir);
    // 切换到父目录
    if (dir.cdUp()) {
        QString parentDir = dir.absolutePath();
        qDebug() << "应用程序运行的父目录: " << parentDir;
        QString sql_dir = parentDir + "/Tools/";
        qDebug() << sql_dir;
        QString baseString = "UPDATE app SET path = REPLACE(path, 'D:/alantop_dir/Alantop_Tool/Tools/', '%1') ";
        QString result = baseString.arg(sql_dir);
        ui->lineEdit_newdir->setText(result);

    } else {
        ui->lineEdit_newdir->setText("UPDATE app SET path = REPLACE(path, 'D:/alantop_dir/Alantop_Tool/Tools/', 'd:/newdir/') ");
        qDebug() << "无法切换到父目录";
    }




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


    model.setHeaderData(6, Qt::Horizontal, "存在否");
    model.setHeaderData(7, Qt::Horizontal, "使用次数");
}

Widget::~Widget()
{
    db.close();
    delete ui;
}

/*
void Widget::checkFilesAndUpdateExistField()
{



    QSqlQuery query(db);
    // 查询表中的所有记录
    if (!query.exec("SELECT * FROM app")) {
        qDebug() << "查询失败: " << query.lastError().text();
                                        return;
    }

    while (query.next()) {
        QString path = query.value("path").toString();
        QFile file(path);
        bool fileExists = file.exists();

        QSqlQuery updateQuery(db);

        QString updateSql;

        // 根据文件是否存在更新 exist 字段的值
        if (!fileExists)
            updateSql = QString("UPDATE app SET exist = 0 WHERE path = '%1'").arg(path);
        else
            updateSql = QString("UPDATE app SET exist = 1 WHERE path = '%1'").arg(path);




        if (!updateQuery.exec(updateSql)) {
            qDebug() << "更新 exist 字段失败: " << updateQuery.lastError().text();
        }
    }
}*/

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

// 左边 点击 查询类型
void Widget::showClick(QModelIndex index)
{
    //QString strTemp;
    //strTemp = index.data().toString();

    //ui->lineEdit_type->setText(strTemp);

    //querytype(strTemp);

    //QSqlQuery query;
    QString str = QString("select * from app where type == '%1' ORDER BY CAST(clicknumber AS INTEGER) DESC").arg(index.data().toString());
    model.setQuery(str);
    ui->tableView->setModel(&model);

    ui->tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(5,QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(6,QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(7,QHeaderView::ResizeToContents);

    ui->label_info->setText(str);





}

void Widget::query(QString search)
{
    QString str = QString("select * from app where type like '%%1%' or name like '%%1%' or filename like '%%1%' or path like '%%1%' or remark like '%%1%' ORDER BY CAST(clicknumber AS INTEGER) DESC").arg(search);
    model.setQuery(str);
    ui->tableView->setModel(&model);

    ui->tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(5,QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(6,QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(7,QHeaderView::ResizeToContents);

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

    initleft();

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

    QString type;


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
    type=datatemp.toString();//name即为所选择行的第一列的值。。。
    ui->lineEdit_type->setText(type);

    Iindex = Imodel->index(index.row(),5);//index.row()为算选择的行号。1为所选中行的第一列。。
    datatemp=Imodel->data(Iindex);
    name=datatemp.toString();//name即为所选择行的第一列的值。。。
    ui->lineEdit_remark->setText(name);

    Iindex = Imodel->index(index.row(),7);
    datatemp=Imodel->data(Iindex);
    name=datatemp.toString();
    ui->lineEdit_clicknumber->setText(name);


    if (type == "dir")
    {
        QDir dir(app_path);
        if (dir.exists()) {
            ui->label_info->setText("目录存在");
            ui->label_info->setStyleSheet("color: green; font - weight: bold;");
            return;
        } else {
            ui->label_info->setText("目录不存在");
            ui->label_info->setStyleSheet("color: red; font - weight: bold;");
            return;
        }
    }






    QFileInfo fileInfo(app_path);
    if (fileInfo.isFile()) {
        ui->label_info->setText("文件存在");
        ui->label_info->setStyleSheet("color: green; font - weight: bold;");
    } else {
        ui->label_info->setText("文件不存在");
        ui->label_info->setStyleSheet("color: red; font - weight: bold;");
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
    QString clicknumber = ui->lineEdit_clicknumber->text();

    //如果有引号，需要引号替换功能 加转义符

    QString str = QString("update app set name = '%1', filename ='%2',path='%3',type='%4', remark='%5', clicknumber='%6' where id=%7").arg(name).arg(filename).arg(path).arg(type).arg(remark).arg(clicknumber).arg(id);

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

    initleft();

    querytype(ui->lineEdit_type->text());

}


void Widget::on_pushButton_2_clicked()
{







    bool ok;
    QString password = QInputDialog::getText(this, "密码输入", "请输入密码:", QLineEdit::Password, "", &ok);
    if (ok && password == "136") {
        // 密码正确，执行后续操作，这里简单输出提示信息
        //QMessageBox::information(this, "成功", "密码正确，继续操作。");
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

    } else {
        // 密码错误，给出提示
        QMessageBox::warning(this, "错误", "密码错误，请重新输入。");
    }






}


void Widget::on_pushButton_path_clicked()
{


    QString path = ui->lineEdit_path->text();

    QDir dir(path);
    if (dir.exists()) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(path));
    } else {
        QFileInfo fi(path);

        qDebug() << fi.canonicalPath() ;

        QDesktopServices::openUrl(QUrl::fromLocalFile(fi.canonicalPath()));
    }




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

        Iindex = Imodel->index(index.row(),7);//index.row()为算选择的行号。1为所选中行的第一列。。
        datatemp=Imodel->data(Iindex);
        name=datatemp.toString();//name即为所选择行的第一列的值。。。
        ui->lineEdit_clicknumber->setText(name);
    }


}

int Widget::add_clicknumber(QString id)
{
    // 根据给定的id查询获取当前的clicknumber值
    QSqlQuery query(db);
    query.prepare("SELECT clicknumber FROM app WHERE id = :id");
    query.bindValue(":id", id);
    if (!query.exec()) {
        qDebug() << "查询失败: " << query.lastError().text();
                                       // db.close();
        return -1;
    }

    int currentClickNumber;
    if (query.next()) {
    currentClickNumber = query.value(0).toInt();
    } else {
        qDebug() << "未找到符合条件的数据（给定id对应的记录不存在）";
        //db.close();
        return -1;
    }

    // 将clicknumber值加1
    currentClickNumber++;

    // 根据id更新数据库中的clicknumber值
    query.prepare("UPDATE app SET clicknumber = :newClickNumber WHERE id = :id");
    query.bindValue(":newClickNumber", currentClickNumber);
    query.bindValue(":id", id);
    if (!query.exec()) {
        qDebug() << "更新失败: " << query.lastError().text();
                                        //db.close();
        return -1;
    }

    return 0;
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

    add_clicknumber(name);

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


    QString path = ui->lineEdit_path->text();

    QDir dir(path);
    if (dir.exists()) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(path));
    } else {
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

        qDebug() << "app_path" + app_path;

        if (app_path.length() >= 4) {
            QString suffix = app_path.right(4);
            if (suffix == ".bat") {
                bool opened = QDesktopServices::openUrl(app_path);
            } else {
                QString cmd = "\"" + app_path + "\"";
                qDebug() << cmd;
                myProcess->startDetached(cmd);

            }
        } else {
            qDebug() << "app_path 的长度不足 4，无法判断后四个字符是否为.bat";
        }




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





        qDebug() << "setCurrent=" << QDir::setCurrent(oldCurrent);




    }



/*
 * 上面的代码不是没有执行,而是在后台执行了.输出都在debug了.界面看不出来了.
 *
    命令行执行方法
     QUrl url = QUrl::fromLocalFile("D:/mygit/bsjy/ip_adb/install.bat");
     bool opened = QDesktopServices::openUrl(url);
     if (!opened)
     {
         qDebug() << "无法打开快捷方式文件。";
     }
 */


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

    initleft();

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

    if (fileName.isEmpty()) {
        QMessageBox::information(this, "提示", tr("未选择文件"));
        return;
    }


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

        bool ok;
        QString password = QInputDialog::getText(this, "密码输入", "请输入密码:", QLineEdit::Password, "", &ok);
        if (ok && password == "136") {
            // 密码正确，执行后续操作，这里简单输出提示信息
            //QMessageBox::information(this, "成功", "密码正确，继续操作。");
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

                QSqlQuery query;
                query.prepare("DELETE FROM app WHERE type = :type");
                query.bindValue(":type", strText);
                bool result = query.exec();
                qDebug() << "result: " << result;

                QMessageBox::information(this, "提示", tr("选择的分类已删除！"));
                on_pushButton_clicked();




            }
        } else {
            // 密码错误，给出提示
            QMessageBox::warning(this, "错误", "密码错误，请重新输入。");
        }





}


void Widget::on_lineEdit_search_textChanged(const QString &arg1)
{
    on_pushButton_clicked();
}





void Widget::on_pushButton_notexist_clicked()
{
    FileCheckerThread *thread = new FileCheckerThread(db, this);
    connect(thread, &FileCheckerThread::updateLabel, this, [this](const QString &text) {
        ui->label_info->setText(text);
    });




    connect(thread, &FileCheckerThread::finished, thread, &FileCheckerThread::deleteLater);

    connect(thread, &FileCheckerThread::selectTable, this, &Widget::updateTableAfterThread);


    ui->label_info->setText("开始更新");
    thread->start();






}

void Widget::updateTableAfterThread()
{
    initleft();

    QString str = QString("select * from app where exist = 0");
    model.setQuery(str);

    ui->tableView->setModel(&model);

    ui->tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(5,QHeaderView::ResizeToContents);
}


void Widget::on_pushButton_usual_clicked()
{
    initleft();


    //QSqlQuery query;
    //QString str = QString("SELECT * FROM app ORDER BY CAST(clicknumber AS INTEGER) DESC");
    model.setQuery("SELECT * FROM app ORDER BY CAST(clicknumber AS INTEGER) DESC");
    ui->tableView->setModel(&model);


    ui->tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(5,QHeaderView::ResizeToContents);
}




void Widget::on_pushButton_del_noexist_clicked()
{
        bool ok;
        QString password = QInputDialog::getText(this, "密码输入", "请输入密码:", QLineEdit::Password, "", &ok);
        if (ok && password == "136") {
            // 密码正确，执行后续操作，这里简单输出提示信息
            QSqlQuery query;


            if (QMessageBox::question(this,"info","are you ok?", QMessageBox::Yes|QMessageBox::No) == QMessageBox::No)
            {
                return;
            }

            QString str = QString ("delete from app where exist = 0");

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

            initleft();

            querytype(ui->lineEdit_type->text());

        } else {
            // 密码错误，给出提示
            QMessageBox::warning(this, "错误", "密码错误，请重新输入。");
        }


}

