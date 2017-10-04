#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    qApp->installEventFilter(this);
    initialImage = nullptr;
    modifiedImage = nullptr;
    transientData= new QMap<QString,double>();
    magnifierDialog=nullptr;
    greyPlotterDialog=nullptr;
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    if(initialImage!=nullptr)
    {
        delete initialImage;
        initialImage = nullptr;
    }
    if(modifiedImage != nullptr)
    {
        delete modifiedImage;
        modifiedImage = nullptr;
    }
    if(transientData!= nullptr)
    {
        delete transientData;
        transientData = nullptr;
    }
    if(magnifierDialog!=nullptr)
    {
        delete magnifierDialog;
        magnifierDialog=nullptr;
    }
    if(greyPlotterDialog!=nullptr)
    {
        delete greyPlotterDialog;
        greyPlotterDialog=nullptr;
    }
    delete ui;
}

/*
    Loads an image from disk without modifying its coloration.
*/
void MainWindow::on_actionColor_triggered()
{
    loadImages();
    updateImages(true);
}

/*
    Loads an image from disk and transforms it into a greyscaleimage.
*/
void MainWindow::on_actionGreyscale_triggered()
{
    loadImages();
    toGreyScale(initialImage);
    updateImages(true);
}

/*
    The SaveAs button saves the modifiedImage on disk on a given path.
*/
void MainWindow::on_actionSave_as_triggered()
{
    QString imagePath = QFileDialog::getSaveFileName(this, tr("Open file"),"",tr("IMAGE (*.jpg *.jpeg *.png *.bmp)"));
    if(imagePath.length()<2)
    {
        return;
    }
    if(modifiedImage != nullptr)
    {
        modifiedImage->save(imagePath);
    }
}

/*
    The method transforms the given image into a greyscale image.
 */
void MainWindow::toGreyScale(QImage* image)
{
    if(image== nullptr)
    {
        return;
    }
    for (int i = 0;  i < image->width();  i++)
    {
        for (int j = 0; j < image->height(); j++)
        {
            int gray = qGray(image->pixel(i,j));
            image->setPixel(i,j, QColor(gray, gray, gray).rgb());
        }
    }
}


/*
    At loading, initialImage and modifiedImage are both initialized with the image taken from the given path.
    But only the initialImage is shown in the UI.
*/
void MainWindow::loadImages()
{
    QString imagePath = QFileDialog::getOpenFileName(this, tr("Open file"),"",tr("IMAGE (*.jpg *.jpeg *.png *.bmp)"));
    if(imagePath.length()<5)
    {
        qDebug("Path too short!");
        return;
    }
    if(initialImage != nullptr)
    {
        delete initialImage;
    }

    initialImage = new QImage(imagePath);
    initialImagePath=imagePath;

    if(modifiedImage != nullptr)
    {
        delete modifiedImage;
    }
    modifiedImage = new QImage(imagePath);
}

/*
    The method updates and shows the initialImage and the modifiedImage. It is supposed to be called after a modification.
    The modifiedImage is shown only if relevant.
 */
void MainWindow::updateImages(bool isOpen)
{
    if(initialImage != nullptr && modifiedImage != nullptr)
    {
        QPixmap pixMapInitial = QPixmap::fromImage(*initialImage);
        QPixmap pixMapModified = QPixmap::fromImage(*modifiedImage);
        ui->label->setPixmap(pixMapInitial);
        if(!isOpen) {
            ui->label_2->setPixmap(pixMapModified);
        }
    }
}

/*
 * The SaveAsInitialImage button replaces the initialImage with the modifiedImage on disk and also on the UI.
 */
void MainWindow::on_btnSaveAsInitialImage_clicked()
{
    if(modifiedImage!= nullptr)
    {
        initialImage= new QImage(*modifiedImage);
        initialImage->save(initialImagePath);
        updateImages(true);
    }
}

/*
    The lamba property launches a dialog that permits saving into the transientData a needed double value.
*/
void MainWindow::on_actionLamba_triggered()
{
    bool ok;
    SmartDialog lambda("Lambda", &ok);
    transientData->insert("Lambda",lambda.getValue());
    //Example of usage CTRL+/ to uncomment
    //    if (ok)
    //    {
    //        ui->label->setText(QString::number(transientData->value("Lambda")));
    //    }

}

/*
 * The method manages the behavior when the mouse is pressed.
 * If the mouse is pressed over the initialImage or the modifiedImage, specific algorithms (like zoom in, grey plotter etc.) are triggered.
 */
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if(initialImage != nullptr && modifiedImage !=nullptr)
    {
        if(isMousedPressedOnInitialImage(obj, event)||isMousedPressedOnModifiedImage(obj,event))
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            int mouseX = mouseEvent->pos().x();
            int mouseY = mouseEvent->pos().y();
            qDebug() << QString("Mouse move (%1,%2)").arg(mouseX).arg(mouseY);

            if(isMousedPressedOnInitialImage(obj, event))
            {
                launchMagnifierDialog(mouseX,mouseY,initialImage);
            }else
            {
                launchMagnifierDialog(mouseX,mouseY, modifiedImage);
            }
            plotGreyPixels(mouseY, initialImage);
            plotGreyPixels(mouseY,modifiedImage);
            return true;
        }
    }
    return false;
}

bool MainWindow::isMousedPressedOnInitialImage(QObject *obj, QEvent *event)
{
    return (qobject_cast<QLabel*>(obj) == ui->label) && event->type() == QEvent::MouseButtonPress;
}

bool MainWindow::isMousedPressedOnModifiedImage(QObject *obj, QEvent *event)
{
    return  qobject_cast<QLabel*>(obj) == ui->label_2 && event->type() == QEvent::MouseButtonPress;
}

/*
    The method zooms over a 9x9 area that surrounds the given coordinate (x,y).
 */
void MainWindow::launchMagnifierDialog(int x, int y, QImage* image)
{
    if(magnifierDialog == nullptr)
    {
        return;
    }
    magnifierDialog->drawMagnifiedImage(image,x,y);
}

/*
    The method creates a reverted image to the initialImage and puts it in the modifiedImage.
 */
void MainWindow::on_actionRevert_colors_triggered()
{
    if(initialImage == nullptr)
    {
        return;
    }
    for (int i = 0;  i < initialImage->width();  i++)
    {
        for (int j = 0; j < initialImage->height(); j++)
        {
            QColor color = initialImage->pixelColor(i, j);
            modifiedImage->setPixel(i,j,qRgb(255-color.red(),255-color.green(),255-color.blue()));
        }
    }
    updateImages(false);
}

/*
    Sets to visible the greyPlotterDialog.
 */
void MainWindow::on_actionPlot_grey_level_triggered()
{
    if(greyPlotterDialog==nullptr)
    {
        greyPlotterDialog= new Plotter();
        greyPlotterDialog->activateWindow();
        greyPlotterDialog->setUpPlot();
    }
    greyPlotterDialog->show();
}

/*
   The method plots to the greyPlotDialog the grey level of a given level-y from a given image.
 */
void MainWindow::plotGreyPixels(int y, QImage* image) const
{

    if(greyPlotterDialog==nullptr)
    {
        return;
    }

    for( int j=0;j<initialImage->width();j++)
    {

        if(image!=nullptr)
        {

            int gray = qGray(image->pixel(j,y));
            if(image==initialImage)
            {
                greyPlotterDialog->plotValuesToFirstGraph(j,gray);
            }else
            {
                greyPlotterDialog->plotValuesToSecondGraph(j,gray);
            }
            qDebug() << QString("qGray (%1)").arg(gray);
        }
    }

    greyPlotterDialog->refresh();
}

/*
   Sets to visible the magnifier dialog.
 */
void MainWindow::on_actionLaunch_magnifier_triggered()
{
    if(magnifierDialog == nullptr)
    {
        magnifierDialog = new Magnifier();
        magnifierDialog->activateWindow();
    }
    magnifierDialog->show();
}

/*
    The gama property launches a dialog that permits saving into the transientData a needed double value.
*/
void MainWindow::on_actionGama_triggered()
{
    bool ok=false;
    SmartDialog gama("Gama",&ok);
    transientData->insert("Gama",gama.getValue());
    //Example of usage CTRL+/ to uncomment

    //    if(ok){
    //        ui->label->setText(QString::number(transientData->value("Gama")));
    //    }
}

