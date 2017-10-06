#include "smartdialog.h"

SmartDialog::SmartDialog(QString name,bool* ok, short numberOfLabels,QWidget* widget)
{
    dialog= new QDialog(widget);
    dialog->setWindowTitle(name);
    QFormLayout form(dialog);
    form.addRow(new QLabel("Add double values:"));
    for(int i = 0; i < numberOfLabels; ++i)
    {
        QLineEdit *lineEdit = new QLineEdit(dialog);
        QString label = QString(name+"%1").arg(i + 1);
        form.addRow(label, lineEdit);
        inputValues[label] = lineEdit;
    }
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,Qt::Horizontal, dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), dialog, SLOT(reject()));
    if(dialog->exec()== QDialog::Accepted)
    {
        (*ok)=true;
    }
}


QMap<QString,double> SmartDialog::getValues()
{
    QMap<QString,double> smartValues;
    QMap<QString, QLineEdit*>::iterator i;
    for (i = inputValues.begin(); i != inputValues.end(); ++i)
    {
        double value= (i.value()->text()).toDouble();
        QString key= i.key();
        smartValues[key]=value;
    }
    return smartValues;
}

double SmartDialog::getFirstValue()
{
    QString firstKey=QString(dialog->windowTitle()+"1");
    double firstValue=inputValues[firstKey]->text().toDouble();
    return firstValue;
}
