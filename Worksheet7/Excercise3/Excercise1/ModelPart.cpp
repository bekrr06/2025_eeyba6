/**     @file ModelPart.cpp
  *
  *     EEEE2076 - Software Engineering & VR Project
  *
  *     Template for model parts that will be added as treeview items
  *
  *     P Evans 2022
  */

#include "ModelPart.h"


/* Commented out for now, will be uncommented later when you have
 * installed the VTK library
 */
#include <vtkSmartPointer.h>
#include <vtkDataSetMapper.h>
#include <vtkPolyDataMapper.h>



ModelPart::ModelPart(const QList<QVariant>& data, ModelPart* parent )
    : m_itemData(data), m_parentItem(parent) {

    /* You probably want to give the item a default colour */
}


ModelPart::~ModelPart() {
    qDeleteAll(m_childItems);
}


void ModelPart::appendChild( ModelPart* item ) {
    /* Add another model part as a child of this part
     * (it will appear as a sub-branch in the treeview)
     */
    item->m_parentItem = this;
    m_childItems.append(item);
}


ModelPart* ModelPart::child( int row ) {
    /* Return pointer to child item in row below this item.
     */
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}

int ModelPart::childCount() const {
    /* Count number of child items
     */
    return m_childItems.count();
}


int ModelPart::columnCount() const {
    /* Count number of columns (properties) that this item has.
     */
    return m_itemData.count();
}

QVariant ModelPart::data(int column) const {
    /* Return the data associated with a column of this item 
     *  Note on the QVariant type - it is a generic placeholder type
     *  that can take on the type of most Qt classes. It allows each 
     *  column or property to store data of an arbitrary type.
     */
    if (column < 0 || column >= m_itemData.size())
        return QVariant();
    return m_itemData.at(column);
}


void ModelPart::set(int column, const QVariant &value) {
    /* Set the data associated with a column of this item 
     */
    if (column < 0 || column >= m_itemData.size())
        return;

    m_itemData.replace(column, value);
}


ModelPart* ModelPart::parentItem() {
    return m_parentItem;
}


int ModelPart::row() const {
    /* Return the row index of this item, relative to it's parent.
     */
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<ModelPart*>(this));
    return 0;
}

void ModelPart::setColour(const unsigned char R, const unsigned char G, const unsigned char B) {
    /* This is a placeholder function that you will need to modify if you want to use it */
    
    /* As the name suggests ... */
}

unsigned char ModelPart::getColourR() {
    /* This is a placeholder function that you will need to modify if you want to use it */
    
    /* As the name suggests ... */
    return 0;   // needs updating
}

unsigned char ModelPart::getColourG() {
    /* This is a placeholder function that you will need to modify if you want to use it */
    
    /* As the name suggests ... */
    return 0;   // needs updating
}


unsigned char ModelPart::getColourB() {
   /* This is a placeholder function that you will need to modify if you want to use it */
    
    /* As the name suggests ... */
    return 0;   // needs updating
}


void ModelPart::setVisible(bool isVisible) {
    /* This is a placeholder function that you will need to modify if you want to use it */
    
    /* As the name suggests ... */
}

bool ModelPart::visible() {
    /* This is a placeholder function that you will need to modify if you want to use it */
    
    /* As the name suggests ... */
    return false;
}

void ModelPart::loadSTL( QString fileName ) {
        // 1. Create the reader
        file = vtkSmartPointer<vtkSTLReader>::New();
        file->SetFileName(fileName.toStdString().c_str());
        file->Update(); // Force a read to check the file

        // 2. Initialise the mapper
        mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputConnection(file->GetOutputPort());

        // 3. Initialise the actor and link to mapper
        actor = vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(mapper);

}

vtkSmartPointer<vtkActor> ModelPart::getActor() {
    /* This is a placeholder function that you will need to modify if you want to use it */
    return(actor);
    /* Needs to return a smart pointer to the vtkActor to allow
     * part to be rendered.
     */
}

//vtkActor* ModelPart::getNewActor() {
    /* This is a placeholder function that you will need to modify if you want to use it
     * 
     * The default mapper/actor combination can only be used to render the part in 
     * the GUI, it CANNOT also be used to render the part in VR. This means you need
     * to create a second mapper/actor combination for use in VR - that is the role
     * of this function. */
     
     
     /* 1. Create new mapper */
     
     /* 2. Create new actor and link to mapper */
     
     /* 3. Link the vtkProperties of the original actor to the new actor. This means 
      *    if you change properties of the original part (colour, position, etc), the
      *    changes will be reflected in the GUI AND VR rendering.
      *    
      *    See the vtkActor documentation, particularly the GetProperty() and SetProperty()
      *    functions.
      */
    

    /* The new vtkActor pointer must be returned here */
//    return nullptr;
    
//}

