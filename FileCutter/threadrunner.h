#ifndef THREADRUNNER_H
#define THREADRUNNER_H

#include <QThread>
#include <QSharedPointer>
#include "filecutterlib.h"

class cThreadRunner : public QThread
{
    Q_OBJECT
    void run() Q_DECL_OVERRIDE;
public:
    explicit cThreadRunner(QObject *parent = 0);
    cThreadRunner(const QString &_FilePath, const int _partSize,
                  const QString &_dstDir,
                  bool _isCutter=true,
                  unsigned long _ram_buff=5,
                  QObject *parent = 0);
    ~cThreadRunner();
    FileCutterLib *GetLibHandle()const;
private:
    QSharedPointer<QString>         pStrFilePath;
    QSharedPointer<FileCutterLib>   pFLib;
    int                             PartSize;
    bool                            iscut;
signals:

public slots:

};

#endif // THREADRUNNER_H
