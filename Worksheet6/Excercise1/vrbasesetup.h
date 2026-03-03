#ifndef VRBASESETUP_H
#define VRBASESETUP_H

#include <QMainWindow>
#include "ModelPart.h"
#include "ModelPartList.h"
#include "optiondialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class VRBaseSetUp;
}
QT_END_NAMESPACE

class VRBaseSetUp : public QMainWindow
{
    Q_OBJECT

public:
    VRBaseSetUp(QWidget *parent = nullptr);
    ~VRBaseSetUp();

public slots:
    void handleButton();
    void handleTreeClicked();
    void on_pushButton_2_clicked();

signals:
    void statusUpdateMessage(const QString & message, int timeout);

private slots:
    void on_actionOpen_File_triggered();
    void on_actionItem_Options_triggered();

private:
    Ui::VRBaseSetUp *ui;
    ModelPartList *partList;

};
#endif // VRBASESETUP_H
