#include "optiondialog.h"
#include "ui_optiondialog.h"

OptionDialog::OptionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OptionDialog)
{
    ui->setupUi(this);
}

OptionDialog::~OptionDialog()
{
    delete ui;
}

void OptionDialog::setInitialValues(const QString &name, bool visible, int r, int g, int b) {
    ui->lineEdit->setText(name);
    ui->checkBox->setChecked(visible);
    // Set the spin boxes (assuming these objectNames)
    ui->spinBoxR->setValue(r);
    ui->spinBoxG->setValue(g);
    ui->spinBoxB->setValue(b);
}

int OptionDialog::getR() const { return ui->spinBoxR->value(); }
int OptionDialog::getG() const { return ui->spinBoxG->value(); }
int OptionDialog::getB() const { return ui->spinBoxB->value(); }

QString OptionDialog::getName() const {
    // Return whatever text the user typed
    return ui->lineEdit->text();
}

bool OptionDialog::getIsVisible() const {
    // Return whether the box is checked [cite: 728, 741]
    return ui->checkBox->isChecked();
}

