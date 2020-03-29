#include "piechart.h"
#include "mainslice.h"
#include <QtCharts/QPieSlice>
#include <QtCharts/QPieLegendMarker>

QT_CHARTS_USE_NAMESPACE

//![1]
PieChart::PieChart(QGraphicsItem *parent, Qt::WindowFlags wFlags)
    : QChart(QChart::ChartTypeCartesian, parent, wFlags)
{
    // Setting up visuals
    this->setAnimationOptions(QChart::AllAnimations);
    this->legend()->setAlignment(Qt::AlignRight);
}
//![1]

//![2]
void PieChart::addBreakdownSeries(QPieSeries *breakdownSeries, QColor color)
{
    QFont font("Arial", 8);

    // add breakdown series as a slice to center pie
    MainSlice *mainSlice = new MainSlice(breakdownSeries);
    mainSlice->setName(breakdownSeries->name());
    mainSlice->setValue(breakdownSeries->sum());
    m_mainSeries->append(mainSlice);

    // customize the slice
    mainSlice->setBrush(color);
    mainSlice->setLabelVisible();
    mainSlice->setLabelColor(Qt::white);
    mainSlice->setLabelPosition(QPieSlice::LabelInsideHorizontal);
    mainSlice->setLabelFont(font);

    // position and customize the breakdown series
    breakdownSeries->setPieSize(0.8);
    breakdownSeries->setHoleSize(0.65);
    breakdownSeries->setLabelsVisible();
    const auto slices = breakdownSeries->slices();
    for (QPieSlice *slice : slices) {
        color = color.lighter(105);
        slice->setBrush(color);
        slice->setLabelFont(font);
    }

    // add the series to the chart
    QChart::addSeries(breakdownSeries);

    // recalculate breakdown donut segments
    recalculateAngles();

    // update customize legend markers
    updateLegendMarkers();
}
//![2]

void PieChart::setNode(NODE *node)
{
    root = node;
    this->setTitle(root->name);

    QChart::removeAllSeries();

    // create the series for main center pie
    m_mainSeries = new QPieSeries();
    m_mainSeries->setPieSize(0.6);
    QChart::addSeries(m_mainSeries);

    // Setting up the sub serieses
    for (int i = 0; i < root->child_count; i++) {
        QPieSeries *series = new QPieSeries();
        series->setName(root->childs[i]->name);
        for (int j = 0; j < root->childs[i]->child_count; j++)
            series->append(root->childs[i]->childs[j]->name, root->childs[i]->childs[j]->size);
        this->addBreakdownSeries(series, Qt::red);
    }

    // QPieSeries *series2 = new QPieSeries();
    // series2->setName("Renewables");
    // series2->append("Wood fuels", 319663);
    // series2->append("Hydro power", 45875);
    // series2->append("Wind power", 1060);

    // QPieSeries *series3 = new QPieSeries();
    // series3->setName("Others");
    // series3->append("Nuclear energy", 238789);
    // series3->append("Import energy", 37802);
    // series3->append("Other", 32441);
}

NODE *PieChart::node()
{
    return root;
}

//![3]
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
//![3]

//![4]
void PieChart::updateLegendMarkers()
{
    // go through all markers
    const auto allseries = series();
    for (QAbstractSeries *series : allseries) {
        const auto markers = legend()->markers(series);
        for (QLegendMarker *marker : markers) {
            QPieLegendMarker *pieMarker = qobject_cast<QPieLegendMarker *>(marker);
            if (series == m_mainSeries) {
                // hide markers from main series
                pieMarker->setVisible(false);
            } else {
                // modify markers from breakdown series
                pieMarker->setLabel(QString("%1 %2%")
                                    .arg(pieMarker->slice()->label())
                                    .arg(pieMarker->slice()->percentage() * 100, 0, 'f', 2));
                pieMarker->setFont(QFont("Arial", 8));
            }
        }
    }
}
//![4]