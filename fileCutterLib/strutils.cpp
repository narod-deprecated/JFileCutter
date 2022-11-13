#include "strutils.h"

cStrUtils::cStrUtils(QObject *parent) :
    QObject(parent)
{
}

QString *cStrUtils::replaceFileName(QString *dst, const QString &filePath)
{
    int pos=-1;
    if(!dst || filePath.isEmpty())
    {
        return dst;
    }
#ifdef WIN32
    pos=filePath.lastIndexOf('/');
#else
    pos=filePath.lastIndexOf('/');
#endif
    if(pos>-1)
    {
        dst->operator =(filePath);
        dst->remove(0,pos+1);
    }
    return dst;
}

QString *cStrUtils::replaceFilePath(QString *dst, const QString &filePath)
{
    int pos=-1;
    if(!dst || filePath.isEmpty())
    {
        return dst;
    }
#ifdef WIN32
    pos=filePath.lastIndexOf('/');
#else
    pos=filePath.lastIndexOf('/');
#endif
    if(pos>-1)
    {
        dst->operator =(filePath);
        dst->remove(pos+1,filePath.length()-(pos+1));
    }
    return dst;
}
