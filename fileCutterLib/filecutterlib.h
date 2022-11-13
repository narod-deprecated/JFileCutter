/*
 * Данная библиотека содержит класс для нарезки и склеивания файлов
*/

#ifndef FILECUTTERLIB_H
#define FILECUTTERLIB_H

#include "filecutterlib_global.h"
#include <QObject>
#include <QString>
#include <QSharedPointer>
#include <QVector>
#include <QFile>

typedef struct _STRUCTPART
{
    qint64 spos;
    qint64 epos;
} STRUCTPART,*pSTRUCTPART;

class FILECUTTERLIBSHARED_EXPORT FileCutterLib: public QObject
{
    Q_OBJECT
public:
    FileCutterLib(QObject *parent = 0);
    FileCutterLib(const QString &_filePath,
                  qint64 _partSize,
                  const QString &_newFilesDir=QString(),
                  bool _withThread=false,QObject *parent = 0);
    ~FileCutterLib();
    qint64   GetRAMBuffer()const;
    qint64   GetProgressMaxValue();

    void            SetRAMBuffer(qint64 _buffer);
    void            CutterFile();
    void            RestoreFile();
    void            ThreadStop();
protected:
    QVector<STRUCTPART> *GetParts(QVector<STRUCTPART> *dst,
                                  qint64 _size,
                                  qint64 _partSize);
    bool createFile(const QString &_fileName, const QString &_fileDir,
                    QFile &_oldFile, const pSTRUCTPART positions,
                    bool _setprogress=false);
private:
    QSharedPointer<QString> filePath;
    QSharedPointer<QString> newFilesDir;
    bool                    withThread;
    int                     progress;
    qint64           partSize;
    bool                    stopThread;
    qint64           ram_buff;
signals:
    void    on_Start();
    void    on_Progress(int _progress);
    void    on_Cancel();
    void    on_Finish(bool noError);
    void    on_Debug(const QString &text);
};

#endif // FILECUTTERLIB_H
