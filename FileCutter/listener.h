#ifndef LISTENER_H
#define LISTENER_H

#include <QObject>
#include "mainwindow.h"
#include "threadrunner.h"

class cListener : public QObject
{
    Q_OBJECT
public:
    explicit    cListener(QObject *parent = 0);
    class       MainWindow        *wnd;
    class       cProgressDialog   *progress;
private:
    qint64 MaxValueProgress;
    cThreadRunner *thread;
    void flibRun(const QString &_path, qint64 _sizepart,
                 qint64 _ram_buff,
                 const QString _dstDir=QString(),
                 bool _iscutter=true);
signals:

public slots:
    void    buttons_clicked();
    void	LineEdits_textChanged(const QString & text);
    void    on_debug(const QString &text);
    void    on_start();
    void    on_progress(int _progress);
    void    on_finish(bool noError);
    void    on_cancel();
};

#endif // LISTENER_H
