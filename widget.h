#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QtDebug>

#include <QStringListModel>
#include <QStandardItemModel>
#include <QModelIndex>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();



private slots:
    void showClick(QModelIndex index);
    void on_pushButton_clicked();


    void on_pushButton_insert_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_pushButton_del_clicked();

    void on_pushButton_modify_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_path_clicked();

    void slotContextMenu(QPoint pos);

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_pushButton_import_clicked();

    void on_pushButton_export_clicked();

    void on_pushButton_deltype_clicked();

    void on_lineEdit_search_textChanged(const QString &arg1);



private:
    Ui::Widget *ui;


    QSqlDatabase db;
    QSqlQueryModel model;

    QStringListModel *Model;
    QStandardItemModel *ItemModel;


    void initdb();



    void initleft();

    //table显示数据
    

    void query(QString search);
    void querytype(QString search);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override; // 拖动进入事件
    void dropEvent(QDropEvent *event) override;           // 放下事件

    //

};
#endif // WIDGET_H
