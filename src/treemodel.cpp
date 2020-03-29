#include "treemodel.h"

TreeModel::TreeModel(const TREE *tree, QObject *parent)
    : QAbstractItemModel(parent)
{
    rootItem = new TreeItem({tr("path"), tr("size"), tr("type")});
    setupModelData(tree,rootItem);
}

TreeModel::~TreeModel()
{
    delete rootItem;
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    //if a top-level item is being referred to
    if (!parent.isValid())
        parentItem = rootItem;
    else
        //obtain the data pointer from the model index with its internalPointer() function and use it to reference a TreeItem object
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    //since the row and column arguments to this function refer to a child item of the corresponding parent item,
    //we obtain the item using the TreeItem::child() function.
    TreeItem *childItem = parentItem->child(row);
    if (childItem)
         //create a model index to be returned. it can be used later to obtain the item's data
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    //to be consistent with the way that the index() function is implemented,
    //we return an invalid model index for the parent of any top-level items in the model.
    if (parentItem == rootItem)
        return QModelIndex();

    //we follow a convention of specifying 0 as the column number of the parent.
    return createIndex(parentItem->row(), 0, parentItem);
}

//returns the number of child items for the TreeItem that corresponds to a given model index,
//or the number of top-level items if an invalid index is specified:
int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

//determine how many columns are present for a given model index
int TreeModel::columnCount(const QModelIndex &parent) const
{

    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    //if an invalid model index is specified, the number of columns returned is determined from the root item:
    return rootItem->columnCount();
}

//obtain data from the model
QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    //DisplayRole is the key data to be rendered in the form of text and it = 0
    if (role != Qt::DisplayRole)
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    return item->data(index.column());
}

//We use the flags() function to ensure that views know that the model is read-only:
Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

//The headerData() function returns data that we conveniently stored in the root item:
QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}


/*void TreeModel::setupModelData(const TREE *tree, TreeItem *parent)
{
    QVector<TreeItem*> parents;
    QVector<int> glbl_lvl;
    parents << parent;
    glbl_lvl << 0;

    int number = 0;

    while (number < tree->root->child_count) {
        int level = tree->root->level;

        if (tree->root!=NULL) {

            QVector<QVariant> columnData;
            columnData.reserve(3);
            columnData << tree->root->name;
            columnData << tree->root->size;
            columnData << tree->root->type;

            if (level > glbl_lvl.last()) {
                // The last child of the current parent is now the new parent
                // unless the current parent has no children.

                if (parents.last()->childCount() > 0) {
                    parents << parents.last()->child(parents.last()->childCount()-1);
                    glbl_lvl << level;
                }
            } else {
                while (level < glbl_lvl.last() && parents.count() > 0) {
                    parents.pop_back();
                    glbl_lvl.pop_back();
                }
            }


            // Append a new item to the current parent's list of children.
            parents.last()->appendChild(new TreeItem(columnData, parents.last()));
        }
        ++number;
    }
}
*/
void TreeModel::setupModelDataHelper(const NODE *root, QVector<TreeItem*> parents, QVector<int> glbl_lvl)
{
    if (root == NULL)
        return;
    QVector<QVariant> columnData;
    columnData.reserve(3);
    columnData << root->name;
    columnData << root->size;
    columnData << root->type;

    if (root->level > glbl_lvl.last())
    {
        if (parents.last()->childCount() > 0)
        {
            parents << parents.last()->child(parents.last()->childCount()-1);
        }
    }else {
        while (root->level < glbl_lvl.last() && parents.count() > 0) {
            parents.pop_back();
            glbl_lvl.pop_back();
        }
    }

    // Append a new item to the current parent's list of children.
    parents.last()->appendChild(new TreeItem(columnData, parents.last()));

    for (int i = 0; i < root->child_count; i++) {
        setupModelDataHelper(root->childs[i], parents, glbl_lvl);
    }

}


void TreeModel::setupModelData(const TREE *tree,TreeItem *parent)
{   
    QVector<TreeItem*> parents;
    QVector<int> glbl_lvl;
    parents << parent;
    glbl_lvl << 0;
    setupModelDataHelper(tree->root, parents, glbl_lvl);
}
