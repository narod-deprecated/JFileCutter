#include "filecutterlib.h"
#include "strutils.h"

FileCutterLib::FileCutterLib(QObject *parent):
    QObject(parent)
{
    stopThread=true;
}

FileCutterLib::FileCutterLib(const QString &_filePath,
                             qint64 _partSize,
                             const QString &_newFilesDir,
                             bool _withThread, QObject *parent):
    QObject(parent)
{
    emit on_Debug("CONSTRUCTOR");
    filePath    =   QSharedPointer<QString>(new QString(_filePath));
    newFilesDir =   QSharedPointer<QString>(new QString(_newFilesDir));
    withThread  =   _withThread;
    partSize    =   _partSize;
    progress    =   0;
    stopThread  =   false;
    ram_buff    =   1024*1024*5; //5mb
}

FileCutterLib::~FileCutterLib()
{
    emit on_Debug("FileCutterLib::~FileCutterLib()");
}

qint64 FileCutterLib::GetRAMBuffer() const
{
    return ram_buff;
}

qint64 FileCutterLib::GetProgressMaxValue()
{
    qint64 result=0;
    if(filePath->endsWith(".part.000"))
    {
        emit on_Debug("GetProgressMaxValue()::selected restore");
        QString cpyFilePath=*filePath.data();
        cpyFilePath.remove(".part.000");
        int k=0;
        while(true && !stopThread)
        {
            QString buff;
            if(k<10)
                buff=QString(".part.00%1").arg(k);
            else if(k<100)
                buff=QString(".part.0%1").arg(k);
            else if(k<1000)
                buff=QString(".part.%1").arg(k);

            QSharedPointer<QFile> hfile=
                        QSharedPointer<QFile>(new QFile(cpyFilePath+buff));
            if(!hfile->exists())
            {
                return result;
            }
            QVector<STRUCTPART> ve;
            GetParts(&ve,hfile->size(),ram_buff);
            for (int i=0;i<ve.size();i++)
                ++result;
            emit on_Debug(QString("GetProgressMaxValue(): %1").arg(result));
            k++;
        }
    }
    else{
        emit on_Debug("GetProgressMaxValue()::selected cutter");
        QSharedPointer<QFile> oldFile=QSharedPointer<QFile>
                                              (new QFile(*filePath.data()));
        if(oldFile->open(QIODevice::ReadOnly))
        {
            qint64 fsize=oldFile->size();
            emit on_Debug("GetProgressMaxValue():fsize: "+QString("%1").arg(fsize));
            QVector<STRUCTPART> ve;
            GetParts(&ve,fsize,partSize);
            for(int i=0;i<ve.size() && !stopThread;i++)
            {
                QVector<STRUCTPART> subve;
                GetParts(&subve,ve[i].epos-ve[i].spos,ram_buff);
                for(int j=0;j<subve.size() && !stopThread;j++)
                    ++result;
                emit on_Debug(QString("GetProgressMaxValue(): %1").arg(result));
            }
        }
    }
    return result;
}

void FileCutterLib::SetRAMBuffer(qint64 _buffer)
{
    ram_buff=_buffer;
}

void FileCutterLib::CutterFile()
{
    emit on_Start();
    bool noError=false;

    QSharedPointer<QFile> oldFile=QSharedPointer<QFile>
                                          (new QFile(*filePath.data()));
    if(oldFile->open(QIODevice::ReadOnly))
    {
        qint64 size=oldFile->size();
        QVector<STRUCTPART> ve;
        GetParts(&ve,size,partSize);

        QString _filePath;
        QString _fileName;
        if(newFilesDir->isEmpty())
        {
            cStrUtils::replaceFilePath(&_filePath,*filePath.data());
            emit on_Debug("FileCutterLib::CutterFile():newFilesDir->isEmpty()");
        }else
        {
            _filePath=*newFilesDir.data()+"/";
            emit on_Debug("FileCutterLib::CutterFile():!newFilesDir->isEmpty()");
        }
        cStrUtils::replaceFileName(&_fileName,*filePath.data());

        for(int i=0;i<ve.size() && !stopThread;i++)
        {
            QString buff;
            if(i<10)
                buff=QString("00%1").arg(i);
            else if(i<100)
                buff=QString("0%1").arg(i);
            createFile(_fileName+".part."+buff,_filePath,*oldFile.data(),
                       &(ve[i]),true);
        }
        noError=true;
        oldFile->close();
    }
    if(stopThread)                   //если была остановка из за пользователя
        emit on_Cancel();            //формируем определенный сигнал
    else                             //иначе
        emit on_Finish(noError);     //все действия прошли своим путем
}

void FileCutterLib::RestoreFile()
{
    bool noError=false;
    emit on_Start();
    if(filePath->endsWith(".part.000"))
    {
        noError=true;
        QString newPath;
        if(newFilesDir->isEmpty())
        {
            cStrUtils::replaceFilePath(&newPath,*filePath.data());
        }else
        {
            newPath=*newFilesDir.data()+"/";
        }
        QString buff;
        cStrUtils::replaceFileName(&buff,*filePath.data());
        buff.remove(".part.000");
        emit on_Debug("RestoreFile()::buff=="+buff);
        QString dirPath;
        cStrUtils::replaceFilePath(&dirPath,*filePath.data());

        if(QFile::exists(dirPath+buff))
        {
            QFile::remove(dirPath+buff);
        }

        int countFiles=0;
        while (true && !stopThread) {
            QString strPart;
            if(countFiles<10)
                strPart=QString("00%1").arg(countFiles);
            else if(countFiles<100)
                strPart=QString("0%1").arg(countFiles);
            emit on_Debug(dirPath+buff+".part."+strPart);
            QFile _oldFile(dirPath+buff+".part."+strPart);
            if(!_oldFile.exists())
            {
                noError=true;
                break;
            }
            if(!_oldFile.open(QIODevice::ReadOnly))
            {
                noError=false;
                break;
            }
            STRUCTPART part;
            part.spos=0;
            part.epos=_oldFile.size();
            if(!createFile(buff,newPath,_oldFile,&part,true))
            {
                noError=false;
                break;
            }
            countFiles++;
        }
     }
    if(stopThread)                  //если была остановка из за пользователя
        emit on_Cancel();           //формируем определенный сигнал
    else                            //иначе
        emit on_Finish(noError);    //все действия прошли своим путем
}

void FileCutterLib::ThreadStop()
{
    stopThread=true;
}

QVector<STRUCTPART> *FileCutterLib::GetParts(QVector<STRUCTPART> *dst,
                                             qint64 _size,
                                             qint64 _partSize)
{
    qint64 k=0;
    emit on_Debug(QString("GetParts():_size: %1").arg(_size));

    for(qint64 i=0;i<_size && !stopThread;i++)
    {
        if(k==_partSize || i==_size-1)
        {
            STRUCTPART _part;
            if(dst->isEmpty())
                _part.spos=0;
            else
            {
                _part.spos=dst->last().epos;
            }
            if(i==_size-1)
                _part.epos=i+1;
            else
                _part.epos=i;
            emit on_Debug(QString("GetParts(): startp(%1)X").arg(_part.spos)+
                          QString("endp(%1)").arg(_part.epos));
            dst->append(_part);
            k=0;
        }
        k++;
    }
    return dst;
}

bool FileCutterLib::createFile(const QString     &_fileName,
                               const QString     &_fileDir,
                               QFile             &_oldFile,
                               const pSTRUCTPART positions,
                               bool              _setprogress)
{
    bool res=false;
    QFile file(_fileDir+_fileName);
    if(file.open(QIODevice::ReadWrite) | QIODevice::Truncate)
    {
        file.close();
        QVector<STRUCTPART> ve;
        emit on_Debug(QString("createFile: length==%1").arg(
                          (positions->epos)-(positions->spos)));
        GetParts(&ve,positions->epos-positions->spos,ram_buff);

        for(int i=0;i<ve.size() && !stopThread;i++)
        {
            _oldFile.seek(positions->spos+ve[i].spos);
            emit on_Debug(QString("createFile: offset %1").arg(
                              positions->spos+ve[i].spos));
            emit on_Debug(QString("createFile: read %1").arg(
                              ve[i].epos-ve[i].spos));
            QByteArray data=_oldFile.read(
                        ve[i].epos-ve[i].spos);
            file.open(QIODevice::ReadWrite | QIODevice::Append);
            file.write(data);
            file.close();
            if(_setprogress)
                emit on_Progress(++progress);
        }
        res=true;
    }
    return res;
}
