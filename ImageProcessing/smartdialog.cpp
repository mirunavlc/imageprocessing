#include "smartdialog.h"

SmartDialog::SmartDialog(QString name,bool* ok, QMenu * menu)
{
    QString dialogTitle= tr(name.toStdString().c_str());
    QString dialogLabel=tr("Add double value:");
    double defaultValue=37.56;
    double minValue=-10000;
    double maxValue=10000;
    double numberOfDecimals=2;

    value= QInputDialog::getDouble(menu, dialogTitle,dialogLabel, defaultValue,minValue ,maxValue,numberOfDecimals , ok);
}

double SmartDialog::getValue()
{
    return value;
}

