#include "listener.h"
#include <QFile>
#include <QFileDialog>

cListener::cListener(QObject *parent) :
    QObject(parent)
{
    wnd=static_cast<MainWindow*>(parent);
}

void cListener::flibRun(const QString &_path, qint64 _sizepart,
                        qint64 _ram_buff,
                        const QString _dstDir,
                        bool _iscutter)
{
    if(_path.isEmpty() || (_iscutter && _sizepart==0)
            || (!_dstDir.isEmpty() && !QFile::exists(_dstDir))
            || _ram_buff<=0)
    {
        QMessageBox::about(wnd,"Error",
                           "The input fields are not filled correctly");
        return;
    }

    switch (wnd->pRamSizeComboBox->currentIndex()) {
    case 1:             //kb
        _ram_buff*=1024;
        break;
    case 2:             //mb
        _ram_buff*=(1024*1024);
        break;
    case 3:             //gb
        _ram_buff*=(1024*1024*1024);
        break;
    }

    thread=
            new cThreadRunner(_path,_sizepart,_dstDir,_iscutter,_ram_buff,
                              this);
    FileCutterLib *flib=thread->GetLibHandle();
    connect(flib,SIGNAL(on_Debug(QString)),this,SLOT(on_debug(QString)));
    connect(flib,SIGNAL(on_Start()),this,SLOT(on_start()));
    connect(flib,SIGNAL(on_Cancel()),this,SLOT(on_cancel()));
    connect(flib,SIGNAL(on_Finish(bool)),this,SLOT(on_finish(bool)));
    connect(flib,SIGNAL(on_Progress(int)),this,SLOT(on_progress(int)));
    connect(thread, &cThreadRunner::finished,
            thread, &QObject::deleteLater);

    MaxValueProgress=flib->GetProgressMaxValue();
    thread->start();
}

void cListener::buttons_clicked()
{
    QPushButton *push=static_cast<QPushButton*>(sender());
    if(!push)
    {
        return;
    }

    qint64 sizeram=0;

    if(push==wnd->pConvertButton.data())
    {
        qint64 sizepart=wnd->pPartSizeEdit->text().toLong();
        switch(wnd->pPartSizeComboBox->currentIndex())
        {
        case 1:
            sizepart*=1024;//KB
            break;
        case 2:
            sizepart*=(1024*1024);//MB
            break;
        case 3:
            sizepart*=(1024*1024*1024);//GB
            break;
        }

        sizeram=wnd->pRamSizeEdit->text().toLong();

        flibRun(wnd->pFilePathEdit->text(),sizepart,
                sizeram,
                wnd->pdstFilePathEdit->text(),
                true);
    }else if(push==wnd->pRestoreButton.data())
    {
        sizeram=wnd->pRamSizeEdit->text().toLong();
        flibRun(wnd->pFilePathEdit->text(),0,
                sizeram,
                wnd->pdstFilePathEdit->text(),
                false);
    }else if(push==wnd->pBrowserFilePathButton.data())
    {
        QString _path=QFileDialog::getOpenFileName(wnd,QObject::tr("Open file"));
        if(_path.isNull())
        {
            return;
        }
        wnd->pFilePathEdit->setText(_path);
    }else if(push==wnd->pdstBrowserFilePathButton.data())
    {
        QString _path=
                QFileDialog::getExistingDirectory(wnd,"Select dir...");
        wnd->pdstFilePathEdit->setText(_path);
    }else if(progress && push==progress->GetCancelHandle())
    {
        try{
            thread->GetLibHandle()->ThreadStop();
        }catch(...)
        {
            QMessageBox::about(progress,"Error","Thread not stopped");
        }
    }
}

void cListener::LineEdits_textChanged(const QString &text)
{
    QLineEdit *edit=static_cast<QLineEdit*>(sender());
    if(!edit)
    {
        return;
    }

    if(edit==wnd->pFilePathEdit.data())
    {
        if(text.isEmpty() || !QFile::exists(text))
        {
            wnd->pRestoreButton->setEnabled(false);
            wnd->pConvertButton->setEnabled(false);
            return;
        }

        if(text.endsWith(".part.000"))
        {
            wnd->pRestoreButton->setEnabled(true);
            wnd->pConvertButton->setEnabled(false);
        }else
        {
            wnd->pRestoreButton->setEnabled(false);
            wnd->pConvertButton->setEnabled(true);
        }
    }else if(edit==wnd->pdstFilePathEdit.data())
    {
        //Оставил может когда-то  сделаю...
    }
}

void cListener::on_debug(const QString &text)
{
    qDebug(text.toStdString().c_str());
}

void cListener::on_start()
{
    progress=new cProgressDialog(wnd);
    progress->setWindowFlags(Qt::Tool);
    connect(progress->GetCancelHandle(),SIGNAL(clicked()),this,
            SLOT(buttons_clicked()));
    progress->SetMaxValue(100);
    progress->setModal(true);
    progress->show();

}

void cListener::on_progress(int _progress)
{
    qDebug(QString("Progress: %1").arg(_progress).toStdString().c_str());
    progress->SetCurrValue((_progress*100)/MaxValueProgress);
}

void cListener::on_finish(bool noError)
{
    if(!progress)
        return;
    progress->hide();
    delete progress;
    progress=NULL;
    if(noError)
        QMessageBox::about(wnd,"Status","The operation is successful");
    else QMessageBox::about(wnd,"Status","The operation failed");
}

void cListener::on_cancel()
{
    if(!progress)
        return;
    progress->hide();
    delete progress;
    progress=NULL;
    QMessageBox::about(wnd,"Status","Canceled by the user");
}
