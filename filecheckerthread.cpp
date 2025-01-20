#include "filecheckerthread.h"

#include <QDebug>
#include <QSqlError>
#include <QDir>

FileCheckerThread::FileCheckerThread(QSqlDatabase db, QObject *parent) : QThread(parent), m_db(db)
{
}

void FileCheckerThread::run() {
    QSqlQuery query(m_db);
    // 查询表中的所有记录
    if (!query.exec("SELECT * FROM app")) {
        qDebug() << "查询失败: " << query.lastError().text();
                                        emit updateLabel("更新数据失败");
        return;
    }

    while (query.next()) {
        QString path = query.value("path").toString();
        QFile file(path);
        bool fileExists = file.exists();
        bool dirExists = false;
        if (!fileExists) {
            QDir dir(path);
            if (dir.exists()) {
                // 尝试打开目录，若成功则认为是目录
                QFileInfo fileInfo(path);
                dirExists = fileInfo.isDir();
            }
        }

        QSqlQuery updateQuery(m_db);

        QString updateSql;

        // 根据文件或目录是否存在更新 exist 字段的值
        if (!fileExists &&!dirExists)
            updateSql = QString("UPDATE app SET exist = 0 WHERE path = '%1'").arg(path);
        else
            updateSql = QString("UPDATE app SET exist = 1 WHERE path = '%1'").arg(path);

        if (!updateQuery.exec(updateSql)) {
            qDebug() << "更新 exist 字段失败: " << updateQuery.lastError().text();
        }
    }
    emit updateLabel("更新数据完成");
    emit selectTable();
}
