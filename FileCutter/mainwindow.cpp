#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QResizeEvent>
#include <QRect>
#include <QDesktopServices>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pListener=QSharedPointer<cListener>(new cListener(this));

// Source dir
    pFilePathLabel=QSharedPointer<QLabel>
            (new QLabel(QObject::tr("Source dir:"),this));

    pFilePathEdit=QSharedPointer<QLineEdit>
            (new QLineEdit(this));

    pBrowserFilePathButton=QSharedPointer<QPushButton>
            (new QPushButton(QObject::tr("..."),this));
//Dst dir
    pdstFilePathLabel=QSharedPointer<QLabel>
            (new QLabel(QObject::tr("Dist dir:"),this));

    pdstFilePathEdit=QSharedPointer<QLineEdit>
            (new QLineEdit(this));

    pdstBrowserFilePathButton=QSharedPointer<QPushButton>
            (new QPushButton(QObject::tr("..."),this));

    pPartSizeLabel=QSharedPointer<QLabel>
            (new QLabel(QObject::tr("Part size:"),this));
    pPartSizeEdit=QSharedPointer<QLineEdit>
            (new QLineEdit("0",this));
    pPartSizeComboBox=QSharedPointer<QComboBox>
            (new QComboBox(this));

    pRamSizeLabel=QSharedPointer<QLabel>
            (new QLabel(QObject::tr("buffer size:"),this));
    pRamSizeEdit=QSharedPointer<QLineEdit>
            (new QLineEdit("5",this));
    pRamSizeComboBox=QSharedPointer<QComboBox>
            (new QComboBox(this));

    pConvertButton=QSharedPointer<QPushButton>
                (new QPushButton(QObject::tr("Cutter"),this));

    pRestoreButton=QSharedPointer<QPushButton>
            (new QPushButton(QObject::tr("Restore"),this));

    connect(pFilePathEdit.data(),SIGNAL(textEdited(QString)),
            pListener.data(),SLOT(LineEdits_textChanged(QString)));
    connect(pFilePathEdit.data(),SIGNAL(textChanged(QString)),
            pListener.data(),SLOT(LineEdits_textChanged(QString)));

    connect(pdstFilePathEdit.data(),SIGNAL(textEdited(QString)),
            pListener.data(),SLOT(LineEdits_textChanged(QString)));
    connect(pdstFilePathEdit.data(),SIGNAL(textChanged(QString)),
            pListener.data(),SLOT(LineEdits_textChanged(QString)));

    connect(pConvertButton.data(),SIGNAL(clicked()),
            pListener.data(),SLOT(buttons_clicked()));
    connect(pRestoreButton.data(),SIGNAL(clicked()),
            pListener.data(),SLOT(buttons_clicked()));
    connect(pBrowserFilePathButton.data(),SIGNAL(clicked()),
            pListener.data(),SLOT(buttons_clicked()));
    connect(pdstBrowserFilePathButton.data(),SIGNAL(clicked()),
            pListener.data(),SLOT(buttons_clicked()));

    pRestoreButton->setEnabled(false);
    pConvertButton->setEnabled(false);


    pPartSizeComboBox->addItem(QObject::tr("B"));
    pPartSizeComboBox->addItem(QObject::tr("KB"));
    pPartSizeComboBox->addItem(QObject::tr("MB"));
    pPartSizeComboBox->addItem(QObject::tr("GB"));

    pRamSizeComboBox->addItem(QObject::tr("B"));
    pRamSizeComboBox->addItem(QObject::tr("KB"));
    pRamSizeComboBox->addItem(QObject::tr("MB"));
    pRamSizeComboBox->addItem(QObject::tr("GB"));

    pRamSizeComboBox->setCurrentIndex(2);
    pRamSizeEdit->setToolTip(QObject::tr(
                                 "This option changes the size of the memory allocated for the buffer file"));
    pFilePathEdit->setToolTip(QObject::tr(
                                  "In this entry field you can enter the path to the source file.Pay attention to assemble the files into one, you need to choose the first part (*.part.000)."));
    pdstFilePathEdit->setToolTip(QObject::tr(
                                  "In this entry field you can enter the path to the folder in which to put the resulting files."
                                     ));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    qDebug("resizeEvent");

    QRect area;

    QSize wnd_size=event->size();//Позиция и размеры окна

    //Src dir UI
    pFilePathLabel->setGeometry(QRect(5,20,wnd_size.width()-50,12));

    area=pFilePathLabel->geometry();

    pFilePathEdit->setGeometry(QRect(10,area.top()+area.height()+5,
                                     wnd_size.width()-50,24));

    area=pFilePathEdit->geometry();

    pBrowserFilePathButton->setGeometry(QRect(
                                            area.width()+15,
                                            area.top(),
                                            (wnd_size.width()-(area.width()+15))-10
                                            ,24));
    //Dst dir UI
    pdstFilePathLabel->setGeometry(QRect(5,area.top()+area.height()+10
                                         ,wnd_size.width()-50,12));

    area=pdstFilePathLabel->geometry();

    pdstFilePathEdit->setGeometry(QRect(10,area.top()+area.height()+5,
                                        wnd_size.width()-50,24));

    area=pdstFilePathEdit->geometry();

    pdstBrowserFilePathButton->setGeometry(QRect(
                                            area.width()+15,
                                            area.top(),
                                            (wnd_size.width()-(area.width()+15))-10
                                            ,24));

    //Part UI
    pPartSizeEdit->setGeometry(QRect((wnd_size.width()-50)-40,
                                     area.top()+area.height()+15,50,24));
    area=pPartSizeEdit->geometry();

    pPartSizeLabel->setGeometry(QRect(
                                    (area.left()-80),
                                    area.top(),
                                    90,24));

    pPartSizeComboBox->setGeometry(QRect(area.left()+area.width(),
                                         area.top(),
                                         (wnd_size.width()-(area.left()+area.width()))-10,
                                         24));

    //RAM UI
    pRamSizeEdit->setGeometry(QRect((wnd_size.width()-50)-40,
                                     area.top()+area.height()+15,50,24));
    area=pRamSizeEdit->geometry();

    pRamSizeLabel->setGeometry(QRect(
                                    (area.left()-80),
                                    area.top(),
                                    90,24));

    pRamSizeComboBox->setGeometry(QRect(area.left()+area.width(),
                                         area.top(),
                                         (wnd_size.width()-(area.left()+area.width()))-10,
                                         24));
    //Buttons UI
    pConvertButton->setGeometry(QRect((wnd_size.width()-75)/2,
                                      area.top()+area.height()+50,
                                      75, 23));
    area=pConvertButton->geometry();
    pRestoreButton->setGeometry(QRect((wnd_size.width()-75)/2,
                                      area.top()+area.height()+10,75,23));
}


void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::RightButton)
    {
        showAbout();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    showAbout();
    event->accept();
}

void MainWindow::showAbout()
{
    QMessageBox about(this);
    about.setIcon(QMessageBox::Information);
    about.setText("jFileCutter V2.6.0\nVendor: JAVAVIRYS\t\t");
    about.setInformativeText("http://java-virys.narod.ru");
    about.setStandardButtons(QMessageBox::Help | QMessageBox::Ok);
    about.setDefaultButton(QMessageBox::Help);
    int ret=about.exec();
    switch(ret)
    {
        case QMessageBox::Help:
            QDesktopServices::openUrl(QUrl("http://java-virys.narod.ru/"));
        break;
    }
}
