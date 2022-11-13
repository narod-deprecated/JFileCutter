#include "threadrunner.h"


cThreadRunner::cThreadRunner(QObject *parent) :
    QThread(parent)
{

}

cThreadRunner::cThreadRunner(const QString &_FilePath, const int _partSize,
                             const QString &_dstDir,
                             bool _isCutter, unsigned long _ram_buff,
                             QObject *parent):
    QThread(parent)
{
    pStrFilePath=QSharedPointer<QString>
                        (new QString(_FilePath));
    PartSize=_partSize;
    pFLib=QSharedPointer<FileCutterLib>
            (new FileCutterLib(_FilePath,_partSize,_dstDir));
    pFLib->SetRAMBuffer(_ram_buff);
    iscut=_isCutter;
}

cThreadRunner::~cThreadRunner()
{
    qDebug("cThreadRunner::~cThreadRunner()");
}

FileCutterLib *cThreadRunner::GetLibHandle() const
{

    return pFLib.data();
}


void cThreadRunner::run()
{
    if(iscut)
        pFLib->CutterFile();
    else
        pFLib->RestoreFile();
}
