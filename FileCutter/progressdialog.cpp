#include "progressdialog.h"
#include "ui_progressdialog.h"
#include <QCloseEvent>

cProgressDialog::cProgressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cProgressDialog)
{
    ui->setupUi(this);
}

cProgressDialog::~cProgressDialog()
{
    delete ui;
}

void cProgressDialog::SetMaxValue(int maxvalue)
{
    ui->progressBar->setMaximum(maxvalue);
}

void cProgressDialog::SetCurrValue(int value)
{
    ui->progressBar->setValue(value);
}

QPushButton *cProgressDialog::GetCancelHandle() const
{
    return ui->pushButton;
}

void cProgressDialog::closeEvent(QCloseEvent *e)
{
    e->ignore();
}
