
#include "TreeModel.h"
#include <QScreen>
#include <QApplication>

#include <QTreeView>

int main(int argc, char *argv[])
{
    //Q_INIT_RESOURCE(trial2);

    QApplication app(argc, argv);

    TREE *tree;
    //construct the tree..
    TreeModel model(tree);


    QTreeView view;
    view.setModel(&model);
    view.setWindowTitle(QObject::tr("Disk Analyzer"));
    view.setAnimated(true);
    view.setIndentation(20);
    view.setSortingEnabled(true);
    const QSize availableSize = view.screen()->availableGeometry().size();
    view.resize(availableSize / 2);
    view.setColumnWidth(0, view.width() / 3);
    view.show();
    return app.exec();
}
