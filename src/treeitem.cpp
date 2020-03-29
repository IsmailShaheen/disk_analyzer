#include "treeitem.h"

//The constructor is only used to record the item's parent and the data associated with each column.
TreeItem::TreeItem(const QVector<QVariant> &data, TreeItem *parent)
    : m_itemData(data), m_parentItem(parent)
{}

TreeItem::~TreeItem()
{
    qDeleteAll(m_childItems);
}

//Add child to the tree
void TreeItem::appendChild(TreeItem *item)
{
    m_childItems.append(item);
}

//The child() function returns the child that corresponds to the specified row number in the item's list of child items:
TreeItem *TreeItem::child(int row)
{
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}

//The number of child items held
int TreeItem::childCount() const
{
    return m_childItems.count();
}

//The TreeModel uses this function to determine the number of rows that exist for a given parent item.
//The row() function reports the item's location within its parent's list of items
int TreeItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));

    return 0;
}

//The number of columns of data in the item is trivially returned by the columnCount() function.
int TreeItem::columnCount() const
{
    return m_itemData.count();
}

//Column data is returned by the data() function. The bounds are checked before accessing the container with the data:
QVariant TreeItem::data(int column) const
{
    if (column < 0 || column >= m_itemData.size())
        return QVariant();
    return m_itemData.at(column);
}

//The item's parent is found with parent():
TreeItem *TreeItem::parentItem()
{
    return m_parentItem;
}
