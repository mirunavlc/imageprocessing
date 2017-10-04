#ifndef PLOTTER_H
#define PLOTTER_H

#include <QDialog>
#include "qcustomplot.h"
#include <QMap>
namespace Ui {
class Plotter;
}

class Plotter : public QDialog
{
    Q_OBJECT

public:
    explicit Plotter(QWidget *parent = 0);
    ~Plotter();
    void plotValuesToFirstGraph(int key, int value);
    void plotValuesToSecondGraph(int key, int value);
    void setUpPlot();
    void refresh();
private:
    QMap<qreal,qreal> initialValues;
    QMap<qreal,qreal> modifiedValues;
    Ui::Plotter *ui;
};

#endif // PLOTTER_H
