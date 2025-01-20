#ifndef FILECHECKERTHREAD_H
#define FILECHECKERTHREAD_H

#include <QObject>
#include <QThread>
#include <QSqlQuery>
#include <QFile>


class FileCheckerThread : public QThread
{
    Q_OBJECT
public:
    explicit FileCheckerThread(QSqlDatabase db, QObject *parent = nullptr);
signals:
    void updateLabel(const QString &text);
    void selectTable();
protected:
    void run() override;
private:
    QSqlDatabase m_db;

};

#endif // FILECHECKERTHREAD_H





