#ifndef MAGNIFIER_H
#define MAGNIFIER_H

#include <QDialog>

namespace Ui {
class Magnifier;
}

class Magnifier : public QDialog
{
    Q_OBJECT

public:
    explicit Magnifier(QWidget *parent = 0);
    ~Magnifier();
    void drawMagnifiedImage(QImage* image, int x, int y);
private:
    void writeTextOverMagnifiedPixel(QImage* imageMagnified, int x, int y, QString text);
    void magnifySpecificPixel(QRgb color, QImage* imageMagnified, int zoomRate, int xStartPixel, int yStartPixel);
private:
    Ui::Magnifier *ui;
    static const int ZOOM_RATE=40;
    static const int NUMBER_OF_PIXELS_PER_LINE_AND_COLUMN_IN_ZOOMED_IMG=9;
};

#endif // MAGNIFIER_H
