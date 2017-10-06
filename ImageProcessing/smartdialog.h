#ifndef SMARTDIALOG_H
#define SMARTDIALOG_H

#include <QAction>
#include <QMenu>
#include <QInputDialog>
#include <QDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QDebug>
#include <QLabel>
#include <QString>

class SmartDialog : public QAction
{
public:
    SmartDialog(QString name,bool* ok, short numberOfLabels =1 ,QWidget* widget= nullptr);
    QMap<QString,double> getValues();
    double getFirstValue();
private:
    QMap<QString, QLineEdit*> inputValues;
    QDialog* dialog;
};

#endif // SMARTDIALOG_H
