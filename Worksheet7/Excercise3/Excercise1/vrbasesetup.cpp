#include "vrbasesetup.h"
#include "ui_vrbasesetup.h"
#include <QMessageBox>
#include <QFileDialog>
#include <vtkSmartPointer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkCamera.h>
#include <vtkProperty.h>

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

    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui->vtkWidget->setRenderWindow(renderWindow);

    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);

    vtkNew<vtkCylinderSource> cylinder;
    cylinder->SetResolution(8);

    vtkNew<vtkPolyDataMapper> cylinderMapper;
    cylinderMapper->SetInputConnection(cylinder->GetOutputPort());

    vtkNew<vtkActor> cylinderActor;
    cylinderActor->SetMapper(cylinderMapper);
    cylinderActor->GetProperty()->SetColor(1., 0., 0.35);
    cylinderActor->RotateX(30.0);
    cylinderActor->RotateY(-45.0);

    renderer->AddActor(cylinderActor);

    /* Reset Camera (probably needs to go in its own function that is called whenever
model is changed) */
    renderer->ResetCamera();
    renderer->GetActiveCamera()->Azimuth(30);
    renderer->GetActiveCamera()->Elevation(30);
    renderer->ResetCameraClippingRange();
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
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("STL Files (*.stl)"));
    if (!fileName.isEmpty()) {
        QModelIndex index = ui->treeView->currentIndex();

        // Add a new child to the selected item
        QList<QVariant> data = { "New Part", "true", "255", "255", "255" };
        QModelIndex newIndex = partList->appendChild(index, data);

        // Load the STL data into that new part
        ModelPart* newPart = static_cast<ModelPart*>(newIndex.internalPointer());
        newPart->loadSTL(fileName);

        updateRender(); // Refresh the 3D view
        emit statusUpdateMessage(QString("Loaded CAD: ") + fileName, 0);
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
        selectedPart->set(0, dialog.getName());
        selectedPart->set(1, dialog.getIsVisible() ? "true" : "false");
        selectedPart->set(2, QString::number(dialog.getR()));
        selectedPart->set(3, QString::number(dialog.getG()));
        selectedPart->set(4, QString::number(dialog.getB()));

        emit statusUpdateMessage(QString("Updated: ") + dialog.getName(), 0);
    }
}

void VRBaseSetUp::updateRender() {
    renderer->RemoveAllViewProps(); // Clear existing actors

    // Start crawling from the root (index 0,0)
    updateRenderFromTree(partList->index(0, 0, QModelIndex()));

    renderer->Render(); // Refresh the window
}

void VRBaseSetUp::updateRenderFromTree(const QModelIndex& index) {
    if (!index.isValid()) return;

    // Get the part and add its actor to the renderer
    ModelPart* part = static_cast<ModelPart*>(index.internalPointer());
    if (part && part->getActor()) {
        renderer->AddActor(part->getActor());
    }

    // Recursively check children
    int rows = partList->rowCount(index);
    for (int i = 0; i < rows; i++) {
        updateRenderFromTree(partList->index(i, 0, index));
    }
}
