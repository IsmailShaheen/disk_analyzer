#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QSplitter>
#include <QtCharts/QChartView>
#include <QtGui/QScreen>
#include "piechart.h"
#include "treemodel.h"
#include "tree.h"
#define HEIGHT 500
#define WIDTH 1000
QT_CHARTS_USE_NAMESPACE

int glb_lvl = 0;
TREE *tree;

int
append_tree(const char *fpath, const struct stat *sb,
            int tflag, struct FTW *ftwbuf);
int 
set_size(NODE *node);

int
main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Backend
    tree = make_tree();
    int flags = 0;
    flags |= FTW_PHYS;

    if (nftw((argc < 2) ? "." : argv[1], append_tree, 20, flags) == -1) {
        perror("nftw");
    }
    traverse_post(tree, set_size);

    // Frontend
    PieChart *pie = new PieChart();
    pie->setNode(tree->root);
    TreeModel model(tree);

    QChartView *chartView = new QChartView(pie);
    chartView->setRenderHint(QPainter::Antialiasing);
    //pie->setNode(tree->root->childs[0]);

    QTreeView *treeView = new QTreeView;
    treeView->setModel(&model);
    treeView->setWindowTitle(QObject::tr("Disk Analyzer"));
    treeView->setAnimated(true);
    treeView->setIndentation(20);
    const QSize availableSize = treeView->screen()->availableGeometry().size();
    treeView->resize(availableSize / 2);
    //treeView->setColumnWidth(2, treeView->width() / 6);
    treeView->setSortingEnabled(false);
    treeView->sortByColumn(1,Qt::DescendingOrder);
    //treeView->show();

    QSplitter *splitter = new QSplitter;
    splitter->addWidget(treeView);
    splitter->addWidget(chartView);
    QList<int> list;
    list.append(WIDTH/3);
    list.append(2*(WIDTH/3));
    splitter->setSizes(list);
    QMainWindow window;
    //window.connect(treeView,treeView->expanded(treeView->selectionModel()->currentIndex()),chartView, chartView->)
    window.setCentralWidget(splitter);
    window.resize(WIDTH, HEIGHT);
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
        for (int i = 0; i < (glb_lvl - ftwbuf->level) + 1; i++)
            tree->current = tree->current->parent;
        add_child(node, tree);
    }
    
    tree->current = node;
    glb_lvl = ftwbuf->level;

    return 0;
}

int 
set_size(NODE *node)
{
    for (int i = 0; i < node->child_count; i++)
        node->size += node->childs[i]->size;
    
    return 0;
}
