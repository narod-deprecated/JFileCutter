#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "listener.h"
#include "progressdialog.h"

#include <QSharedPointer>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>

#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QSharedPointer<class cListener> pListener;



    QSharedPointer<QLabel>      pFilePathLabel;
    QSharedPointer<QLineEdit> pFilePathEdit;
    QSharedPointer<QPushButton> pBrowserFilePathButton;

    QSharedPointer<QLabel>      pdstFilePathLabel;
    QSharedPointer<QLineEdit> pdstFilePathEdit;
    QSharedPointer<QPushButton> pdstBrowserFilePathButton;

    QSharedPointer<QLabel> pPartSizeLabel;
    QSharedPointer<QLineEdit> pPartSizeEdit;
    QSharedPointer<QComboBox> pPartSizeComboBox;

    QSharedPointer<QLabel> pRamSizeLabel;
    QSharedPointer<QLineEdit> pRamSizeEdit;
    QSharedPointer<QComboBox> pRamSizeComboBox;

    QSharedPointer<QPushButton> pConvertButton;
    QSharedPointer<QPushButton> pRestoreButton;


    Ui::MainWindow *ui;
private:
    void resizeEvent(QResizeEvent * event);
    void mousePressEvent(QMouseEvent * event);
    void closeEvent(QCloseEvent *event);
    void showAbout();
};

#endif // MAINWINDOW_H
