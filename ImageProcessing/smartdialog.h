#ifndef SMARTDIALOG_H
#define SMARTDIALOG_H

#include <QAction>
#include <QMenu>
#include <QInputDialog>

class SmartDialog : public QAction
{
public:
    SmartDialog(QString name, bool* ok, QMenu * menu=nullptr);
    double getValue();
private:
    double value;
};

#endif // SMARTDIALOG_H
