#ifndef STRUTILS_H
#define STRUTILS_H

#include <QObject>
#include <QString>
class cStrUtils : public QObject
{
    Q_OBJECT
public:
    explicit cStrUtils(QObject *parent = 0);

    static QString* replaceFileName(QString *dst,const QString &filePath);
    static QString* replaceFilePath(QString *dst,const QString &filePath);
signals:

public slots:

};

#endif // STRUTILS_H
