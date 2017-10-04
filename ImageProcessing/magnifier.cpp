#include "magnifier.h"
#include "ui_magnifier.h"
#include<QPainter>

Magnifier::Magnifier(QWidget *parent) :  QDialog(parent),  ui(new Ui::Magnifier)
{
    QWidget::setWindowFlags(Qt::WindowStaysOnTopHint);
    ui->setupUi(this);
}

Magnifier::~Magnifier()
{
    delete ui;
}

/*
    Plots a NUMBER_OF_PIXELS_PER_LINE_AND_COLUMN_IN_ZOOMED_IMGxNUMBER_OF_PIXELS_PER_LINE_AND_COLUMN_IN_ZOOMED_IMG zoom in surrounding the given coordinate(x,y).
    Every plotted pixel is has its intensity ( from 0 to 255) written over in red text.
*/
void Magnifier::drawMagnifiedImage(QImage* image, int x, int y)
{
    if(image== nullptr)
    {
        return;
    }
    QImage imageMagnified = QPixmap(ZOOM_RATE*NUMBER_OF_PIXELS_PER_LINE_AND_COLUMN_IN_ZOOMED_IMG+1 , ZOOM_RATE*NUMBER_OF_PIXELS_PER_LINE_AND_COLUMN_IN_ZOOMED_IMG+1).toImage();

    int aboveLines= NUMBER_OF_PIXELS_PER_LINE_AND_COLUMN_IN_ZOOMED_IMG/2;
    int belowLines= NUMBER_OF_PIXELS_PER_LINE_AND_COLUMN_IN_ZOOMED_IMG/2;
    int aheadColumns=NUMBER_OF_PIXELS_PER_LINE_AND_COLUMN_IN_ZOOMED_IMG/2;
    int afterColumns=NUMBER_OF_PIXELS_PER_LINE_AND_COLUMN_IN_ZOOMED_IMG/2;

    for( int i=y-aboveLines, yStart=0;i<=y+belowLines;i++,yStart+=ZOOM_RATE)
    {
        for(int j=x-aheadColumns, xStart=0;j<=x+afterColumns;j++,xStart+=ZOOM_RATE)
        {
            magnifySpecificPixel(image->pixel(j,i),&imageMagnified,ZOOM_RATE,xStart,yStart);
            writeTextOverMagnifiedPixel(&imageMagnified,xStart,yStart,QString::number(qGray(image->pixel(j,i))));
        }
    }
    QPixmap magnifiedPixmap=QPixmap::fromImage(imageMagnified);
    ui->label->setPixmap(magnifiedPixmap);
}

/*
 * Writes the text in red color over the magnified pixel given by coordinates(x,y).
 */
void Magnifier:: writeTextOverMagnifiedPixel(QImage* imageMagnified, int x, int y, QString text)
{
    QPainter painter(imageMagnified);
    int inferiorY=y+(ZOOM_RATE/2);
    painter.setPen(Qt::red);
    painter.setFont(QFont("Arial", 10));
    painter.drawText(x,inferiorY,text);
}

/*
   Magnifies the given pixel ZOOM_RATE times in order to make it visible.
   Then plots it to the given imageMagnified.
*/
void Magnifier::magnifySpecificPixel(QRgb color, QImage* imageMagnified, int zoomRate, int xStartPixel, int yStartPixel )
{
    for (int i=yStartPixel;i< yStartPixel+zoomRate;i++)
    {
        for (int j=xStartPixel; j < xStartPixel+ zoomRate; j++)
        {
            imageMagnified->setPixel(j,i, color);
        }
    }

}


