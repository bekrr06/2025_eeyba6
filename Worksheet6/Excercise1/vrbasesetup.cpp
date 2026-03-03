#include "vrbasesetup.h"
#include "ui_vrbasesetup.h"
#include <QMessageBox>
#include <QFileDialog>

VRBaseSetUp::VRBaseSetUp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::VRBaseSetUp)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::released, this, &VRBaseSetUp::handleButton);
    connect(this, &VRBaseSetUp::statusUpdateMessage, ui->statusbar, &QStatusBar::showMessage);
    connect(ui->treeView, &QTreeView::clicked, this, &VRBaseSetUp::handleTreeClicked);

    this->partList = new ModelPartList("PartsList");
    ui->treeView->setModel(this->partList);

    ui->treeView->addAction(ui->actionItem_Options);

    QIcon openIcon(":/Icons/fileopen.png");
    if (openIcon.isNull()) {
        emit statusUpdateMessage("Error: Icon failed to load from resource", 0);
    } else {
        ui->actionOpen_File->setIcon(openIcon);
    }

    ModelPart *rootItem = this->partList->getRootItem();
    for (int i = 0; i < 3; i++)
    {
        QString name = QString("TopLevel %1").arg(i);
        QString visible("true");

        ModelPart *childItem = new ModelPart({ name , visible, "0", "0", "0" });
        rootItem->appendChild(childItem);
        for (int j = 0; j < 5; j++)
        {
            QString subName = QString("Item %1,%2").arg(i).arg(j);
            QString subVisible("true");
            ModelPart *childChildItem = new ModelPart({ subName , subVisible, "0", "0", "0" });
            childItem->appendChild(childChildItem);
        }
    }
}

VRBaseSetUp::~VRBaseSetUp()
{
    delete ui;
}

void VRBaseSetUp::handleButton() {
    emit statusUpdateMessage(QString("Add button was clicked"), 0);
}

void VRBaseSetUp::handleTreeClicked() {
    QModelIndex index = ui->treeView->currentIndex();
    ModelPart *selectedPart = static_cast<ModelPart*>(index.internalPointer());
    if (selectedPart)
    {
        QString text = selectedPart->data(0).toString();
        emit statusUpdateMessage(QString("The selected item is: ") + text, 0);
    }
}

void VRBaseSetUp::on_actionOpen_File_triggered() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "C:\\", tr("STL Files (*.stl);;Text Files (*.txt)"));
        if (!fileName.isEmpty()) {
        QModelIndex index = ui->treeView->currentIndex();
            if (index.isValid()) {
            ModelPart *selectedPart = static_cast<ModelPart*>(index.internalPointer());
            selectedPart->set(0, fileName);
        }
        emit statusUpdateMessage(QString("File Opened: ") + fileName, 0);
    }
}

void VRBaseSetUp::on_pushButton_2_clicked() {
    // Create an instance of your custom dialog
    OptionDialog dialog(this);

        // dialog.exec() blocks the main window until the dialog is closed (Modal)
        if (dialog.exec() == QDialog::Accepted) {
            emit statusUpdateMessage(QString("Dialog accepted"), 0);
    } else {
        emit statusUpdateMessage(QString("Dialog rejected"), 0);
    }
}

void VRBaseSetUp::on_actionItem_Options_triggered() {
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid()) return;

    ModelPart *selectedPart = static_cast<ModelPart*>(index.internalPointer());
    QString name = selectedPart->data(0).toString();
    bool visible = (selectedPart->data(1).toString() == "true");
    int r = selectedPart->data(2).toInt();
    int g = selectedPart->data(3).toInt();
    int b = selectedPart->data(4).toInt();

    OptionDialog dialog(this);
    dialog.setInitialValues(name, visible, r, g, b);

    if (dialog.exec() == QDialog::Accepted) {
        // PUSH the identifiers back to the model
        selectedPart->set(0, dialog.getName());
        selectedPart->set(1, dialog.getIsVisible() ? "true" : "false");
        selectedPart->set(2, QString::number(dialog.getR()));
        selectedPart->set(3, QString::number(dialog.getG()));
        selectedPart->set(4, QString::number(dialog.getB()));

        emit statusUpdateMessage(QString("Updated: ") + dialog.getName(), 0);
    }
}
