#include "plotter.h"
#include "ui_plotter.h"

Plotter::Plotter(QWidget *parent) : QDialog(parent), ui(new Ui::Plotter)
{
    QWidget::setWindowFlags(Qt::WindowStaysOnTopHint);
    ui->setupUi(this);
}

Plotter::~Plotter()
{
    delete ui;
}

/*
    Adds the given pair to the initialValues and then updates the plotter's first graph.
*/
void Plotter::plotValuesToFirstGraph(int key, int value)
{
    initialValues[key]=value;
    ui->customPlot->graph(0)->setData(initialValues.keys().toVector(),initialValues.values().toVector());
}

/*
    Adds the given pair to the modifiedValues and then updates the plotter's second graph.
*/
void Plotter::plotValuesToSecondGraph(int key, int value)
{
    modifiedValues[key]=value;
    ui->customPlot->graph(1)->setData(modifiedValues.keys().toVector(),modifiedValues.values().toVector());
}

/*
    Initializes the plotter with two graphs and a legend.
*/
void Plotter::setUpPlot()
{
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
    ui->customPlot->addGraph();
    ui->customPlot->graph(1)->setPen(QPen(Qt::red));
    ui->customPlot->xAxis2->setVisible(true);
    ui->customPlot->xAxis2->setTickLabels(false);
    ui->customPlot->yAxis2->setVisible(true);
    ui->customPlot->yAxis2->setTickLabels(false);
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->customPlot->legend->setVisible(true);
    ui->customPlot->legend->setFont(QFont("Helvetica", 9));
    QStringList lineNames;
    lineNames << "initial image" << "modified image";
    ui->customPlot->graph(0)->setName(lineNames.at(0));
    ui->customPlot->graph(1)->setName(lineNames.at(1));
}


/*
    The method assures the update of the plotter. It is supposed to be called after a modification has been made.
*/
void Plotter::refresh()
{
    ui->customPlot->replot();
    ui->customPlot->yAxis->setRange(-1, 257);
    ui->customPlot->xAxis->rescale();
    ui->customPlot->yAxis2->setRange(-1, 257);
    ui->customPlot->xAxis2->rescale();
    ui->customPlot->update();
}

