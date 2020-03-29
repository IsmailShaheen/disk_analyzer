#ifndef PIECHART_H
#define PIECHART_H

#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include "tree.h"

QT_CHARTS_USE_NAMESPACE

class PieChart : public QChart
{
public:
    PieChart(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);
    void addBreakdownSeries(QPieSeries *series, QColor color, qreal sum = 0);
    void setNode(NODE *node);
    NODE *node();

private:
    void recalculateAngles();
    void updateLegendMarkers();

private:
    QPieSeries *m_mainSeries;
    NODE *root;
};

#endif // PIECHART_H
