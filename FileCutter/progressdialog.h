#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QDialog>
#include <QPushButton>

namespace Ui {
class cProgressDialog;
}

class cProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit cProgressDialog(QWidget *parent = 0);
    ~cProgressDialog();
    int     GetMaxValue();
    int     GetCurrValue();
    void    SetMaxValue(int maxvalue);
    void    SetCurrValue(int value);
    QPushButton *GetCancelHandle()const;
private:
    Ui::cProgressDialog *ui;
    void closeEvent(QCloseEvent * e);
};

#endif // PROGRESSDIALOG_H
