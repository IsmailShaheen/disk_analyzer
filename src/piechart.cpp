#include "piechart.h"
#include "mainslice.h"
#include <QtCharts/QPieSlice>
#include <QtCharts/QPieLegendMarker>

QT_CHARTS_USE_NAMESPACE

PieChart::PieChart(QGraphicsItem *parent, Qt::WindowFlags wFlags)
    : QChart(QChart::ChartTypeCartesian, parent, wFlags)
{
    // Setting up visuals
    this->setAnimationOptions(QChart::AllAnimations);
    this->legend()->setVisible(false);
}

void PieChart::addBreakdownSeries(QPieSeries *breakdownSeries, QColor color, qreal sum)
{
    QFont font("Arial", 8);

    // add breakdown series as a slice to center pie
    MainSlice *mainSlice = new MainSlice(breakdownSeries);
    mainSlice->setName(breakdownSeries->name());
    mainSlice->setValue((sum == 0 ? breakdownSeries->sum() : sum));
    m_mainSeries->append(mainSlice);

    // customize the slice
    mainSlice->setBrush(color);
    mainSlice->setLabelColor(Qt::black);
    mainSlice->setLabelPosition(QPieSlice::LabelOutside);
    mainSlice->setLabelArmLengthFactor(0.7);
    mainSlice->setLabelFont(font);

    // position and customize the breakdown series
    breakdownSeries->setPieSize(0.8);
    breakdownSeries->setHoleSize(0.55);
    breakdownSeries->setLabelsVisible(false);
    const auto slices = breakdownSeries->slices();
    for (QPieSlice *slice : slices) {
        color = color.lighter(102);
        slice->setBrush(color);
        slice->setLabelFont(font); 
    }

    // add the series to the chart
    QChart::addSeries(breakdownSeries);

    // recalculate breakdown donut segments
    recalculateAngles();
}

void PieChart::setNode(NODE *node)
{
    root = node;
    this->setTitle(root->name);

    QChart::removeAllSeries();

    // create the series for main center pie
    m_mainSeries = new QPieSeries();
    connect(this->m_mainSeries, &QPieSeries::clicked, this, &PieChart::updateChart);
    m_mainSeries->setPieSize(0.5);
    m_mainSeries->setHoleSize(0.25);
    QChart::addSeries(m_mainSeries);

    // Setting up the sub serieses
    QColor color = Qt::red;
    for (int i = 0; i < root->child_count; i++) {
        QPieSeries *series = new QPieSeries();
        series->setName(root->childs[i]->name);
        for (int j = 0; j < root->childs[i]->child_count; j++)
            series->append(root->childs[i]->childs[j]->name, root->childs[i]->childs[j]->size);
        color.setGreen((color.green() + i * 15) % 255);
        this->addBreakdownSeries(series, color, root->childs[i]->size);
    }
}

NODE *PieChart::node()
{
    return root;
}

void PieChart::recalculateAngles()
{
    qreal angle = 0;
    const auto slices = m_mainSeries->slices();
    for (QPieSlice *slice : slices) {
        QPieSeries *breakdownSeries = qobject_cast<MainSlice *>(slice)->breakdownSeries();
        breakdownSeries->setPieStartAngle(angle);
        angle += slice->percentage() * 360.0; // full pie is 360.0
        breakdownSeries->setPieEndAngle(angle);
    }
}

void PieChart::updateChart(QPieSlice *slice)
{
    int index = m_mainSeries->slices().indexOf(slice);
    if (root->childs[index]->child_count != 0)
        return
    setNode(root->childs[index]);
}