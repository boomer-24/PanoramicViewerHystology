#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui_(new Ui::MainWindow), graphicsScene_(new QGraphicsScene), itemGroup_(new GraphicsItemGroup)
{
    ui_->setupUi(this);
    this->setWindowTitle("Viewer");
    this->graphicsScene_->setSceneRect(-100000, -100000, 200000, 200000);
    this->ui_->graphicsViewMain->setScene(this->graphicsScene_);
    this->graphicsScene_->addItem(this->itemGroup_);
    this->itemGroup_->setFlag(QGraphicsItem::ItemIsMovable);

    this->ui_->label_previewImage->hide();
    this->ui_->label_zoomLevel_simplyText->hide();
    this->ui_->pbTestDB->hide();

    QObject::connect(this->itemGroup_, SIGNAL(signalGetCoordinateToView(QPoint)), this, SLOT(slotTemp(QPoint)));
    QObject::connect(this->itemGroup_, SIGNAL(signalEnsureVisible(QPoint, QPoint)), this, SLOT(slotEnsureVisible(QPoint, QPoint)));
    QObject::connect(this->itemGroup_, SIGNAL(signalGetSelectedPixelInItemGroup(QPoint)), this, SLOT(slotGetSelectedPixelFromItemGroup(QPoint)));
    QObject::connect(this->itemGroup_, SIGNAL(signalChangeZoomLevel(int)), this, SLOT(slotZoomLevelChange(int)));
    QObject::connect(this->itemGroup_, SIGNAL(signalForPreview(int, QPoint, QPoint)), this, SLOT(slotForShowVisibleAreaInPreview(int, QPoint, QPoint)));
    QObject::connect(this->itemGroup_, SIGNAL(signalForObtainMarkersCoordinated(QPoint, QPoint)), this, SLOT(slotForGetMarkesCoordinatesToItemGroup(QPoint, QPoint)));
}

MainWindow::~MainWindow()
{
    delete this->itemGroup_;
    delete this->graphicsScene_;
    delete ui_;
}

void MainWindow::PrepareForShow()
{
    QString filePath(QFileDialog::getOpenFileName(this, tr("Select xml-file with description"), "D:/___TILES/w_tif", tr("*.xml")));  //  по-хорошему на "C:/" бы сослаться
    if (!filePath.isEmpty())
    {
        QString pathToStorage;

        QStringList pathToStorageList = filePath.split("/");
        for (int i = 0; i < pathToStorageList.size() - 1; i++)// -1 чтобы без ".xml"
        {
            pathToStorage.append(pathToStorageList.at(i)).append("/");
        }
        QString pathToPreviewImage = pathToStorage;
        this->LoadPreviewImage(pathToPreviewImage);
        pathToStorage.append("1_");
        this->imageDescription_.Initialize(filePath);
        this->itemGroup_->SetStoragePath(pathToStorage);
        this->itemGroup_->InitializeTilePathsVector(this->imageDescription_.GetImageDescription());
        this->itemGroup_->InitializedVectorHeightInPixAllLevels(this->imageDescription_.GetImageDescription());
        this->itemGroup_->FirstEdditionToItemGroup();
    }
}

void MainWindow::on_pbOpenXml_clicked()
{
    this->PrepareForShow();
    this->ui_->graphicsViewMain->centerOn(this->itemGroup_);
    this->ui_->label_zoomLevel_simplyText->show();
    this->ui_->label_zoomLevel->setNum(this->itemGroup_->GetCurrentZoomLevel());
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    this->ui_->tabWidget->setFixedSize(this->size() - QSize(this->ui_->tabWidget->pos().rx(), this->ui_->tabWidget->pos().ry() + 20));
    this->ui_->graphicsViewMain->setFixedSize(this->ui_->tabWidget->size() - QSize(this->ui_->graphicsViewMain->pos().rx() + 15, this->ui_->graphicsViewMain->pos().ry() + 30));
    this->ui_->label_forTuner->setFixedSize(this->ui_->tabWidget->size() - QSize(this->ui_->label_forTuner->pos().rx() + 15, this->ui_->label_forTuner->pos().ry() + 30));

    int width = this->ui_->graphicsViewMain->width();
    int height = this->ui_->graphicsViewMain->height();
    width /= this->itemGroup_->GetTileSide();
    height /= this->itemGroup_->GetTileSide();
    width++;
    height++;
    width *= 3;
    height *= 3;
    this->itemGroup_->SetItemGroupSize(QSize(width, height));
}

void MainWindow::wheelEvent(QWheelEvent* _event)
{
    if (QRect(this->ui_->graphicsViewMain->rect()).contains(_event->pos()))
    {
        qDebug() << "bla";
    }
}

void MainWindow::slotTemp(QPoint _point)
{
    qDebug() << _point; //  ТОЧКА СО СЦЕНЫ
    this->posInScene_ = _point;
    qDebug() << "slot mapFromScene: " << this->ui_->graphicsViewMain->mapFromScene(_point); // ЭТО ПОЗИЦИЯ В ВЬЮ !!!
    this->posInView_ = this->ui_->graphicsViewMain->mapFromScene(_point);
    qDebug() << "POS IN VIEW: " << this->posInView_;
}

void MainWindow::slotEnsureVisible(QPoint _pixInOldItemGroup, QPoint _pixInNewItemGroup)
{
    QPoint forRect(this->itemGroup_->boundingRect().x(), this->itemGroup_->boundingRect().y());
    qDebug() << forRect;
    QPoint globalPos(forRect + _pixInNewItemGroup);
    qDebug() << globalPos;
    QPoint rectLeftUp(globalPos - this->posInView_);
    qDebug() << rectLeftUp;
    //    if (this->itemGroup_->isIncrease())
    //        rectLeftUp.operator -=(QPoint(20, 20));  //  ШИРИНА ScrollBar
    QRect rectForEnsureVisible(rectLeftUp, QPoint(rectLeftUp.rx() + this->ui_->graphicsViewMain->width() /*- 100*/, rectLeftUp.ry() + this->ui_->graphicsViewMain->height() /*- 100*/));//-5?
    qDebug() << "rectForEnsureVisible: " << rectForEnsureVisible;

    //    this->ui_->graphicsViewMain->centerOn(this->itemGroup_->scenePos().rx() + _pixInNewItemGroup.rx(), this->itemGroup_->scenePos().ry() + _pixInNewItemGroup.ry());  //  НА ПОДУМАТЬ
    this->ui_->graphicsViewMain->ensureVisible(rectForEnsureVisible, 0, 0);
    qDebug() << "SLOT ENSURE VISIBLE: " << _pixInOldItemGroup;
}

void MainWindow::slotGetSelectedPixelFromItemGroup(QPoint _pixel)
{
    qDebug() << "!!!!!!!!!!!!!!!!!!!!            " << _pixel;
    qDebug() << this->ui_->graphicsViewMain->mapFromScene(_pixel);
}

void MainWindow::slotZoomLevelChange(int _level)
{
    this->ui_->label_zoomLevel->setNum(_level);
}

void MainWindow::slotForShowVisibleAreaInPreview(int _level, QPoint _startPoint, QPoint _finishPoint)
{
    const ZoomLevelDescription& currentLevelDescription = this->imageDescription_.GetImageDescription().at(_level);
    this->previewImageDest_ = this->previewImageSrc_;
    QPoint startPointInPixelsInPreview, finishPointInPixelsInPreview;
    startPointInPixelsInPreview.setX((_startPoint.rx() * this->previewImageSrc_.width()) / currentLevelDescription.tileAmountW());
    startPointInPixelsInPreview.setY((_startPoint.ry() * this->previewImageSrc_.height()) / currentLevelDescription.tileAmountH());
    finishPointInPixelsInPreview.setX((_finishPoint.rx() * this->previewImageSrc_.width()) / currentLevelDescription.tileAmountW());
    finishPointInPixelsInPreview.setY((_finishPoint.ry() * this->previewImageSrc_.height()) / currentLevelDescription.tileAmountH());

    int thick = 2;

    for (int h = startPointInPixelsInPreview.ry(); h < finishPointInPixelsInPreview.ry(); h++)
    {
        for (int ww = startPointInPixelsInPreview.rx() - thick; ww < startPointInPixelsInPreview.rx() + thick; ww++)
        {
            if (ww < 5) ww = 5;
            if (ww > this->previewImageDest_.width() - 5) ww = this->previewImageDest_.width() - 5;
            this->previewImageDest_.setPixel(ww, h, Qt::white);
        }
    }
    for (int h = startPointInPixelsInPreview.ry(); h < finishPointInPixelsInPreview.ry(); h++)
    {
        for (int ww = finishPointInPixelsInPreview.rx() - thick; ww < finishPointInPixelsInPreview.rx() + thick; ww++)
        {
            if (ww < 5) ww = 5;
            if (ww > this->previewImageDest_.width() - 5) break;
//            qDebug() << "this->previewImageDest_.setPixel(ww, h, Qt::white): " << ww << "__" << h;
            this->previewImageDest_.setPixel(ww, h, Qt::white);
        }
    }
    for (int w = startPointInPixelsInPreview.rx(); w < finishPointInPixelsInPreview.rx(); w++)
    {
        for (int hh = startPointInPixelsInPreview.ry() - thick; hh < startPointInPixelsInPreview.ry() + thick; hh++)
        {
            if (hh < 5) hh = 5;
            if (hh > this->previewImageDest_.height() - 5) hh = this->previewImageDest_.height() - 5;
            this->previewImageDest_.setPixel(w, hh, Qt::white);
        }
    }
    for (int w = startPointInPixelsInPreview.rx(); w < finishPointInPixelsInPreview.rx(); w++)
    {
        for (int hh = finishPointInPixelsInPreview.ry() - thick; hh < finishPointInPixelsInPreview.ry() + thick; hh++)
        {
            if (hh < 5) hh = 5;
            if (hh > this->previewImageDest_.height() - 5) break;
            this->previewImageDest_.setPixel(w, hh, Qt::white);
        }
    }

    this->ui_->label_previewImage->setPixmap(QPixmap::fromImage(this->previewImageDest_.scaled(this->ui_->label_previewImage->size(), Qt::KeepAspectRatio)));
//    this->ui_->label_previewImage->setPixmap(QPixmap::fromImage(this->previewImageDest_).scaled(this->ui_->label_previewImage->size(), Qt::KeepAspectRatio));
//    this->previewImageDest_.save("D:/Новая папка/img.png");
}

void MainWindow::slotForGetMarkesCoordinatesToItemGroup(QPoint _startPoint, QPoint _finishPoint)
{
    QVector<PositionInDB> vTransit = this->processImageDBInterraction_.getPositionsFromDB(_startPoint, _finishPoint);
    qDebug() << "NYAN";
}

void MainWindow::LoadPreviewImage(QString& _pathToBaseDirrectory)
{
    QString pathToPreviewImage =_pathToBaseDirrectory.append("Preview/1_50.png");
    if (QFile(pathToPreviewImage).exists())
        this->previewImageSrc_.load(pathToPreviewImage);
    else qDebug() << "not found image";
    this->previewImageSrc_ = this->previewImageSrc_.scaled(this->ui_->label_previewImage->size(), Qt::KeepAspectRatio);
    this->previewImageDest_ = this->previewImageSrc_;
//    this->previewImageDest_.save("D:/Новая папка/1.png");
    this->ui_->label_previewImage->setPixmap(QPixmap::fromImage(this->previewImageDest_));
}

void MainWindow::on_pbTunerImage_clicked()
{
    this->tuner_.SetSrcImage(this->itemGroup_->GetImageForTuner());
    this->ui_->label_forTuner->setPixmap(QPixmap::fromImage(this->tuner_.GetDestImage()));
    this->ui_->label_forTuner->setAlignment(Qt::AlignAbsolute);
    //    QImage img(this->tuner_.GetDestImage());
    //    img.save("D:/Новая папка/img.bmp");
}

void MainWindow::mousePressEvent(QMouseEvent* _event)
{
    if (QRect(this->ui_->label_forTuner->pos(), this->tuner_.GetDestImage().size()).contains(_event->pos()))
    {
        QPoint positionInLabel(_event->pos() - this->ui_->label_forTuner->pos() - QPoint(4, 27));
        qDebug() << positionInLabel;
        this->tuner_.SetSelectedPixel(positionInLabel);
        this->ui_->label_forTuner->setPixmap(QPixmap::fromImage(this->tuner_.GetDestImage()));
        qDebug() << "destImageSize: " << this->tuner_.GetDestImage().size();
    }
}

void MainWindow::on_pushButton_TunerLaunching_clicked()
{
    this->tuner_.Reset();
    qDebug() << "Reset()";
    this->tuner_.SetDelta(this->ui_->spinBox_delta->value());
    this->tuner_.GainPixels(this->tuner_.GetSelectedPixelForGain());
    qDebug() << "this->tuner_.GainPixels(this->tuner_.GetSelectedPixelForGain());";
    this->ui_->label_forTuner->setPixmap(QPixmap::fromImage(this->tuner_.GetDestImage()));

    this->tuner_.NearBorderMark();  //  white
    qDebug() << "this->tuner_.NearBorderMark()";
    this->tuner_.BorderMark();      //  blue, red
    qDebug() << "this->tuner_.BorderMark()";
    this->tuner_.CalculateAverage();
    qDebug() << "this->tuner_.CalculateAverage();";
    this->tuner_.CalculatePerimeter();
    qDebug() << "this->tuner_.CalculatePerimeter();";
    this->tuner_.CalculateFormFactor();
    qDebug() << "this->tuner_.CalculateFormFactor();";
    this->tuner_.CalculateMedianValue();
    qDebug() << "this->tuner_.CalculateMedianValue();";
    this->tuner_.CalculateStdDeviation();
    qDebug() << "this->tuner_.CalculateStdDeviation();";
    this->tuner_.CalculateAsymmetry();
    qDebug() << "this->tuner_.CalculateAsymmetry();";
    this->tuner_.CalculateExcess();
    qDebug() << "this->tuner_.CalculateExcess();";

    this->ui_->label_squareValue->setNum(this->tuner_.getSquare());
    this->ui_->label_formFactorValue->setText(QString::number(this->tuner_.getFormFactor()));

    this->ui_->label_RedMinValue->setNum(this->tuner_.getRmin());
    this->ui_->label_RedMaxValue->setNum(this->tuner_.getRmax());
    this->ui_->label_GreenMinValue->setNum(this->tuner_.getGmin());
    this->ui_->label_GreenMaxValue->setNum(this->tuner_.getGmax());
    this->ui_->label_BlueMinValue->setNum(this->tuner_.getBmin());
    this->ui_->label_BlueMaxValue->setNum(this->tuner_.getBmax());
    this->ui_->label_RedAverage->setNum(this->tuner_.getAverageRed());
    this->ui_->label_GreenAverage->setNum(this->tuner_.getAverageGreen());
    this->ui_->label_BlueAverage->setNum(this->tuner_.getAverageBlue());
    this->ui_->label_RedMedian->setNum(this->tuner_.getMedianRed());
    this->ui_->label_GreenMedian->setNum(this->tuner_.getMedianGreen());
    this->ui_->label_BlueMedian->setNum(this->tuner_.getMedianBlue());
    this->ui_->label_redStdDevValue->setText(QString::number(this->tuner_.getStdDevR()));
    this->ui_->label_greenStdDevValue->setText(QString::number(this->tuner_.getStdDevG()));
    this->ui_->label_blueStdDevValue->setText(QString::number(this->tuner_.getStdDevB()));
    this->ui_->label_redAsymmetryValue->setText(QString::number(this->tuner_.getAsymmetryR()));
    this->ui_->label_greenAsymmetryValue->setText(QString::number(this->tuner_.getAsymmetryG()));
    this->ui_->label_blueAsymmetryValue->setText(QString::number(this->tuner_.getAsymmetryB()));
    this->ui_->label_redExcessValue->setText(QString::number(this->tuner_.getExcessR()));
    this->ui_->label_greenExcessValue->setText(QString::number(this->tuner_.getExcessG()));
    this->ui_->label_blueExcessValue->setText(QString::number(this->tuner_.getExcessB()));

    this->ui_->label_forTuner->setPixmap(QPixmap::fromImage(this->tuner_.GetDestImage()));
}

void MainWindow::on_pushButton_processAllTestImage_clicked()
{
    this->tuner_.setRed(this->ui_->spinBox_redTuner->value());
    this->tuner_.setRedDelta(this->ui_->spinBox_redDeltaTuner->value());
    this->tuner_.setGreen(this->ui_->spinBox_greenTuner->value());
    this->tuner_.setGreenDelta(this->ui_->spinBox_greenDeltaTuner->value());
    this->tuner_.setBlue(this->ui_->spinBox_blueTuner->value());
    this->tuner_.setBlueDelta(this->ui_->spinBox_blueDeltaTuner->value());
    this->tuner_.setSquareMin(this->ui_->spinBox_squareMin->value());
    this->tuner_.setSquareMax(this->ui_->spinBox_squareMax->value());
    this->tuner_.setFormFactorForProcessImage(this->ui_->doubleSpinBox_formFactorMax->value());

    this->tuner_.ProcessImage();
    this->ui_->label_forTuner->setPixmap(QPixmap::fromImage(this->tuner_.GetDestImage()));
}

void MainWindow::on_pushButton_moveValuesToProcessImage_clicked()
{
    this->processImageDBInterraction_.setVectorTilePaths(this->itemGroup_->GetVectorTilePaths());
    this->processImageDBInterraction_.setZoomLevel(0);
    this->processImageDBInterraction_.setRGBValues(this->ui_->spinBox_redTuner->value(), this->ui_->spinBox_greenTuner->value(), this->ui_->spinBox_blueTuner->value());
    this->processImageDBInterraction_.setRGBDeltaValues(this->ui_->spinBox_redDeltaTuner->value(), this->ui_->spinBox_greenDeltaTuner->value(), this->ui_->spinBox_blueDeltaTuner->value());
    this->processImageDBInterraction_.setSquareLimMinMaxValues(this->ui_->spinBox_squareMin->value(), this->ui_->spinBox_squareMax->value());
    this->processImageDBInterraction_.setFormFactorMax(this->ui_->doubleSpinBox_formFactorMax->value());

    this->processImageDBInterraction_.CreateConnection();
    this->processImageDBInterraction_.FillingTileTable();   // ЗАПОЛНЯТЬ ТУТ TILE-table ТОЛЬКО ТАЙЛЫ С ЯДРЫШКАМИ
    this->processImageDBInterraction_.ProcessImage();
}

void MainWindow::on_pushButton_selectDBConnect_clicked()
{
    QString pathToDB = QFileDialog::getOpenFileName(this, "Select database", "D:/");
    if (!pathToDB.isEmpty())
    {
        this->ui_->pushButton_selectDBConnect->setEnabled(false);
        this->processImageDBInterraction_.CreateConnection(pathToDB);
    }
}

void MainWindow::on_pbTestDB_clicked()
{
    QVector<PositionInDB> testV = this->processImageDBInterraction_.getPositionsFromDB(QPoint(16, 130), QPoint(17, 131));
    for (auto a : testV)
    {
        qDebug() << a.getTileX();
        qDebug() << a.getTileY();
        qDebug() << a.getObjectX();
        qDebug() << a.getObjectY();
    }
}

void MainWindow::on_pushButton_showHidePreview_clicked()
{
    if (this->ui_->label_previewImage->isHidden())
    {
        this->ui_->label_previewImage->show();
        this->ui_->pushButton_showHidePreview->setText("\nСкрыть\nпревью");
    } else
    {
        this->ui_->label_previewImage->hide();
        this->ui_->pushButton_showHidePreview->setText("Показать\n\nпревью");
    }
}
