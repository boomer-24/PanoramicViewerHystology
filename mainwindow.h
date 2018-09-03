#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graphicsitemgroup.h"
#include "parameterstuner.h"
#include "processimagedbinterraction.h"

#include <QMainWindow>
#include <QFileDialog>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QWheelEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void PrepareForShow(); //...and show itemGroup

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *);
    void wheelEvent(QWheelEvent* _event);
    void mousePressEvent(QMouseEvent * _event);

private slots:
    void on_pbOpenXml_clicked();
    void slotTemp(QPoint _point);
    void slotEnsureVisible(QPoint _pixInOldItemGroup, QPoint _pixInNewItemGroup);
    void slotGetSelectedPixelFromItemGroup(QPoint _pixel);
    void slotZoomLevelChange(int _level);
    void slotForShowVisibleAreaInPreview(int _level, QPoint _startPoint, QPoint _finishPoint);
    void slotForGetMarkesCoordinatesToItemGroup(QPoint _startPoint, QPoint _finishPoint);
    void LoadPreviewImage(QString& _pathToBaseDirrectory);
    void on_pbTunerImage_clicked();
    void on_pushButton_TunerLaunching_clicked();
    void on_pushButton_processAllTestImage_clicked();
    void on_pushButton_moveValuesToProcessImage_clicked();
    void on_pushButton_selectDBConnect_clicked();
    void on_pbTestDB_clicked();
    void on_pushButton_showHidePreview_clicked();

private:
    Ui::MainWindow *ui_;
    ImageDescription imageDescription_;
    GraphicsItemGroup* itemGroup_;
    QGraphicsScene* graphicsScene_;
    QPoint posInView_, posInScene_;
    ParametersTuner tuner_;
    ProcessImageDBInterraction processImageDBInterraction_;
    QImage previewImageSrc_, previewImageDest_;
};

#endif // MAINWINDOW_H
