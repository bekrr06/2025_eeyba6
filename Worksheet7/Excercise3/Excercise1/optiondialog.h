#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>

namespace Ui {
class OptionDialog;
}

class OptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionDialog(QWidget *parent = nullptr);
    ~OptionDialog();
    void setInitialValues(const QString &name, bool visible, int r, int g, int b);
    QString getName() const;
    bool getIsVisible() const;
    int getR() const;
    int getG() const;
    int getB() const;

private:
    Ui::OptionDialog *ui;
};

#endif // OPTIONDIALOG_H
