#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTreeView>
#include <QtCharts/QChartView>
#include <QtGui/QScreen>
#include "piechart.h"
#include "treemodel.h"
#include "tree.h"

QT_CHARTS_USE_NAMESPACE

int glb_lvl = 0;
TREE *tree;

int
append_tree(const char *fpath, const struct stat *sb,
            int tflag, struct FTW *ftwbuf);

int
main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Backend
    tree = make_tree();
    int flags = 0;

    if (argc > 2 && strchr(argv[2], 'd') != NULL)
        flags |= FTW_DEPTH;
    if (argc > 2 && strchr(argv[2], 'p') != NULL)
        flags |= FTW_PHYS;

    if (nftw((argc < 2) ? "." : argv[1], append_tree, 20, flags) == -1) {
        perror("nftw");
    }

    // Frontend
    PieChart *pie = new PieChart();
    pie->setNode(tree->root);
    TreeModel model(tree);

    QChartView *chartView = new QChartView(pie);
    chartView->setRenderHint(QPainter::Antialiasing);

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

    QMainWindow window;
    window.setCentralWidget(chartView);
    window.resize(800, 500);
    window.show();

    return a.exec();
}

int
append_tree(const char *fpath, const struct stat *sb,
            int tflag, struct FTW *ftwbuf)
{
    char *name = strcat((char *)fpath + ftwbuf->base, "\0");
    char *name2 = (char *)malloc((strlen(name) + 1) * sizeof(char));
    strcpy(name2, name);
    NODE *node  = make_node(tflag,
                            ftwbuf->level, 
                            sb->st_size, 
                            name2);

    if (ftwbuf->level > glb_lvl || tree->root == NULL) {
        add_child(node, tree);
    } else if (ftwbuf->level == glb_lvl) {
        tree->current = tree->current->parent;
        add_child(node, tree);
    } else if (ftwbuf->level < glb_lvl) {
        tree->current = tree->current->parent->parent;
        add_child(node, tree);
    }
    
    tree->current = node;
    glb_lvl = ftwbuf->level;

    return 0;
}
