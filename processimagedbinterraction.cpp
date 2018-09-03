#include "processimagedbinterraction.h"

ProcessImageDBInterraction::ProcessImageDBInterraction()
{

}

ProcessImageDBInterraction::ProcessImageDBInterraction(QVector<QVector<QVector<QString>>>& _vectorTilePaths)
{    
    //    this->imgDescription_ = _importedImageDescription;
    this->vectorTilePaths_ = _vectorTilePaths;

    this->green_.setRgb(40, 240, 70, 255);
    this->turquoise_.setRgb(110, 250, 250, 255);
    this->white_.setRgb(255,255,255,255);
    this->green_.setRgb(40, 240, 70, 255);
    this->red_.setRgb(240, 30, 30, 255);
    this->blue_.setRgb(0, 0, 255, 255);
    this->marker_.setRgb(125, 255, 120);
    this->black_.setRgb(0,0,0,255);
    this->dark_green_.setRgb(0, 128, 0, 225);
}

void ProcessImageDBInterraction::CreateConnection()
{
    this->dataBase_ = new DBHandler;
    this->dataBase_->CreateConnection();
    this->dataBase_->CreateTables();

//    this->FillingTileTable(this->vectorTilePaths_.at(this->zoomLevel_).at(0).size(), this->vectorTilePaths_.at(this->zoomLevel_).size());
}

void ProcessImageDBInterraction::CreateConnection(const QString& _pathToDB)
{
    this->dataBase_ = new DBHandler;
    this->dataBase_->CreateConnection(_pathToDB);
}

void ProcessImageDBInterraction::FillingTileTable(const int& _w, const int& _h)
{
    for (int y = 1; y <= _h; y++)
        for (int x = 1; x <= _w; x++)
        {
            this->dataBase_->InsertIntoTileTable(x, y);
        }
}

void ProcessImageDBInterraction::FillingTileTable()
{
    int _h = this->vectorTilePaths_.at(0).size();
    int _w = this->vectorTilePaths_.at(0).at(0).size();
    for (int y = 1; y <= _h; y++)
        for (int x = 1; x <= _w; x++)
        {
            this->dataBase_->InsertIntoTileTable(x, y);
        }
}

void ProcessImageDBInterraction::ProcessImage()
{
//    this->FillingTileTable(this->vectorTilePaths_.at(this->zoomLevel_).at(0).size(), this->vectorTilePaths_.at(this->zoomLevel_).size());
    this->imageForExtractCoordinates_ = new QImage(this->sizeofImageInTiles_.rwidth() * this->TILESIDE_, this->sizeofImageInTiles_.rheight() * this->TILESIDE_, QImage::Format_ARGB32);
    this->setAmountTilesXinBigImage(this->vectorTilePaths_.at(this->zoomLevel_).at(0).size());

    for (int h = 0; h < this->vectorTilePaths_.at(this->zoomLevel_).size(); h += this->sizeofImageInTiles_.rheight() - 1)
        for (int w = 0; w < this->vectorTilePaths_.at(this->zoomLevel_).at(0).size(); w += this->sizeofImageInTiles_.rwidth() - 1)
        {
            this->setMinTileX_YforEvalID(w, h);
            qDebug() << "ProcessImage::h: " << h << " w: " << w;
            for (int hh = h, hCount = 0; hCount < this->sizeofImageInTiles_.rheight(); hh++, hCount++)
                for (int ww = w, wCount = 0; wCount < this->sizeofImageInTiles_.rwidth(); ww++, wCount++)
                {
                    QImage imgTile(this->vectorTilePaths_.at(this->zoomLevel_).at(hh).at(ww));
                    for (int height = 0; height < this->TILESIDE_; height++)
                        for (int width = 0; width < this->TILESIDE_; width++)
                        {
                            this->imageForExtractCoordinates_->setPixel(wCount * this->TILESIDE_ + width, hCount * this->TILESIDE_ + height, imgTile.pixel(width, height));
                        }
                }
            if (!this->isWhiteImage(*this->imageForExtractCoordinates_))
            {
                this->ThresholdFilter();
                this->SquareFilter();
                this->FormFactorFilter();
            }
        }
    delete this->imageForExtractCoordinates_;
}

void ProcessImageDBInterraction::ThresholdFilter()
{
    QColor qwerty_thresh;
    int redMax = this->redValue_ + this->deltaRed_;
    int greenMax = this->greenValue_ + this->deltaGreen_;
    int blueMax = this->blueValue_ + this->deltaBlue_;
    int redMin = this->redValue_ - this->deltaRed_;
    int greenMin = this->greenValue_ - this->deltaGreen_;
    int blueMin = this->blueValue_ - this->deltaBlue_;
    (redMax > 255) ? redMax = 255 : redMax = redMax;
    (redMin < 0) ? redMin = 0 : redMin = redMin;
    (greenMax > 255) ? greenMax = 255 : greenMax = greenMax;
    (greenMin < 0) ? greenMin = 0 : greenMin = greenMin;
    (blueMax > 255) ? blueMax = 255 : blueMax = blueMax;
    (blueMin < 0) ? blueMin = 0 : blueMin = blueMin;

    for (int h = 0; h < this->imageForExtractCoordinates_->height(); h++)
        for (int w = 0; w < this->imageForExtractCoordinates_->width(); w++)
        {
            qwerty_thresh.setRgb(this->imageForExtractCoordinates_->pixel(w, h));
            if (redMin < qwerty_thresh.red() && qwerty_thresh.red() < redMax &&
                    greenMin < qwerty_thresh.green() && qwerty_thresh.green() < greenMax &&
                    blueMin < qwerty_thresh.blue() && qwerty_thresh.blue() < blueMax)
                qwerty_thresh.setRgb(this->black_.rgb());
            else qwerty_thresh.setRgb(this->white_.rgb());
            this->imageForExtractCoordinates_->setPixel(w, h, qwerty_thresh.rgb());
        }
}

void ProcessImageDBInterraction::SquareFilter()
{
    for (int h = 0; h < this->imageForExtractCoordinates_->height(); h++)
    {
        this->imageForExtractCoordinates_->setPixel(0, h, this->white_.rgb());
        this->imageForExtractCoordinates_->setPixel(this->imageForExtractCoordinates_->width() - 1, h, this->white_.rgb());
    }
    for (int w = 0; w < this->imageForExtractCoordinates_->width(); w++)
    {
        this->imageForExtractCoordinates_->setPixel(w, 0, this->white_.rgb());
        this->imageForExtractCoordinates_->setPixel(w, this->imageForExtractCoordinates_->height() - 1, this->white_.rgb());
    }
    // //////////////////////////////////////////НАЧЕРТИЛ ГРАНИЦУ
    QColor kill;
    for (int j = 1; j < this->imageForExtractCoordinates_->height() - 1; j++)
        for (int i = 1; i < this->imageForExtractCoordinates_->width() - 1; i++)
        {
            kill.setRgb(this->imageForExtractCoordinates_->pixel(i, j));
            if (kill.rgb() != this->white_.rgb())
            {
                this->vKillTrash_.clear();
                this->KillTrash(i, j);
                if (this->vKillTrash_.size() != 0)
                {
                    if (this->vKillTrash_.size() < this->squareMin_ || this->vKillTrash_.size() > this->squareMax_)
                    {
                        for (int i = 0; i < this->vKillTrash_.size(); i++)
                            this->imageForExtractCoordinates_->setPixel(this->vKillTrash_.at(i), this->white_.rgb());
                    }
                }
            }
        }
}

void ProcessImageDBInterraction::KillTrash(const int& _x, const int& _y)
{
    QColor is_black;
    is_black.setRgb(this->imageForExtractCoordinates_->pixel(_x, _y));
    if ((is_black.rgb() != this->white_.rgb()) && (is_black.rgb() != this->dark_green_.rgb()))
    {
        QPoint dot(_x, _y);
        this->vKillTrash_.push_back(dot);

        QPoint p_ul, p_uc, p_ur, p_cl, p_cr, p_dl, p_dc, p_dr;
        p_ul.setX(_x - 1);
        p_ul.setY(_y - 1);
        p_uc.setX(_x);
        p_uc.setY(_y - 1);
        p_ur.setX(_x + 1);
        p_ur.setY(_y - 1);
        p_cl.setX(_x - 1);
        p_cl.setY(_y);
        p_cr.setX(_x + 1);
        p_cr.setY(_y);
        p_dl.setX(_x - 1);
        p_dl.setY(_y + 1);
        p_dc.setX(_x);
        p_dc.setY(_y + 1);
        p_dr.setX(_x + 1);
        p_dr.setY(_y + 1);

        this->imageForExtractCoordinates_->setPixel(_x, _y, this->dark_green_.rgb());

        KillTrash(p_ul.x(), p_ul.y());
        KillTrash(p_uc.x(), p_uc.y());
        KillTrash(p_ur.x(), p_ur.y());

        KillTrash(p_cl.x(), p_cl.y());
        KillTrash(p_cr.x(), p_cr.y());

        KillTrash(p_dl.x(), p_dl.y());
        KillTrash(p_dc.x(), p_dc.y());
        KillTrash(p_dr.x(), p_dr.y());
    }

}

void ProcessImageDBInterraction::BorderMark()
{
    //  RED-GREEN MARK
    QColor bord;
    QColor pix_w_mh;
    QColor pix_mw_h;
    QColor pix_pw_h;
    QColor pix_w_ph;

    for (int h = 1; h < this->imageForExtractCoordinates_->height() - 1; h++)
        for (int w = 1; w < this->imageForExtractCoordinates_->width() - 1; w++)
        {
            bord.setRgb(this->imageForExtractCoordinates_->pixel(w, h));
            if (bord.rgb() == this->dark_green_.rgb())
            {
                pix_w_mh.setRgb(this->imageForExtractCoordinates_->pixel(w, h - 1));
                pix_mw_h.setRgb(this->imageForExtractCoordinates_->pixel(w - 1, h));
                pix_pw_h.setRgb(this->imageForExtractCoordinates_->pixel(w + 1, h));
                pix_w_ph.setRgb(this->imageForExtractCoordinates_->pixel(w, h + 1));

                if (pix_w_mh.rgb() == this->white_.rgb() || pix_mw_h.rgb() == this->white_.rgb() ||
                        pix_pw_h.rgb() == this->white_.rgb() || pix_w_ph.rgb() == this->white_.rgb())
                    this->imageForExtractCoordinates_->setPixel(w, h, this->green_.rgb());
            }
        }
    QColor two;
    QColor u, l, r, d;
    for (int i = 1; i < this->imageForExtractCoordinates_->height() - 1; i++)
        for (int j = 1; j < this->imageForExtractCoordinates_->width() - 1; j++)
        {
            two.setRgb(this->imageForExtractCoordinates_->pixel(j, i));
            if (two.rgb() == this->dark_green_.rgb())
            {
                u.setRgb(this->imageForExtractCoordinates_->pixel(j, i - 1));
                l.setRgb(this->imageForExtractCoordinates_->pixel(j - 1, i));
                r.setRgb(this->imageForExtractCoordinates_->pixel(j + 1, i));
                d.setRgb(this->imageForExtractCoordinates_->pixel(j, i + 1));
                if ((u.rgb() == this->green_.rgb() && l.rgb() == this->green_.rgb()) ||
                        (u.rgb() == this->green_.rgb() && r.rgb() == this->green_.rgb()) ||
                        (d.rgb() == this->green_.rgb() && l.rgb() == this->green_.rgb()) ||
                        (d.rgb() == this->green_.rgb() && r.rgb() == this->green_.rgb()))
                    this->imageForExtractCoordinates_->setPixel(j, i, this->red_.rgb());
            }
        }
}

void ProcessImageDBInterraction::FormFactorFilter()
{
    this->BorderMark();

    QColor test_col;
    this->Nucleus_ = new Nucleus;
    for (int h = 1; h < this->imageForExtractCoordinates_->height(); h++)
        for (int w = 1; w < this->imageForExtractCoordinates_->width(); w++)
        {
            test_col.setRgb(this->imageForExtractCoordinates_->pixel(w, h));
            if (test_col.rgb() != this->white_.rgb() && test_col.rgb() != this->blue_.rgb())
            {
                if (!this->vPointElong_.empty())
                    this->vPointElong_.clear();
                this->Nucleus_->reset_all();

                this->minXY_ = QPoint(w, h);
                this->maxXY_ = QPoint(w, h);
                this->FillOneNucleus(w, h);

                this->Nucleus_->calculate_perimeter();
                this->Nucleus_->calculate_square();      //подумать...такое вроде уже есть
                this->Nucleus_->calculate_form_factor();
                if (this->Nucleus_->return_form_factor() > this->formFctorMax_)
                {
                    for (int i = 0; i < this->vPointElong_.size(); i++)
                    {
                        this->imageForExtractCoordinates_->setPixel(this->vPointElong_.at(i), this->white_.rgb());
                    }
                } else
                {
                    int centerX = this->minXY_.rx() + (this->maxXY_.rx() - this->minXY_.rx()) / 2;
                    int centerY = this->minXY_.ry() + (this->maxXY_.ry() - this->minXY_.ry()) / 2;
                    this->EvalTileIdAndCoordinatesInTile(centerX, centerY);
                    this->dataBase_->InsertIntoObjectTable(this->tileIdToDB_, this->xToDB_, this->yToDB_);
                }
            }
        }
    delete this->Nucleus_;
//    this->imageForExtractCoordinates_->save("D:/blablabla.bmp");
}

void ProcessImageDBInterraction::FillOneNucleus(const int& _x, const int& _y)
{
    QColor one_more_test;
    one_more_test.setRgb(this->imageForExtractCoordinates_->pixel(_x, _y));

    if (one_more_test.rgb() != this->blue_.rgb() && one_more_test.rgb() != this->white_.rgb())
    {
        this->EvalNucleusCenterCoordinate(_x, _y);

        QPoint p_ul, p_uc, p_ur, p_cl, p_cr, p_dl, p_dc, p_dr;
        p_ul.setX(_x - 1);
        p_ul.setY(_y - 1);
        p_uc.setX(_x);
        p_uc.setY(_y - 1);
        p_ur.setX(_x + 1);
        p_ur.setY(_y - 1);
        p_cl.setX(_x - 1);
        p_cl.setY(_y);
        p_cr.setX(_x + 1);
        p_cr.setY(_y);
        p_dl.setX(_x - 1);
        p_dl.setY(_y + 1);
        p_dc.setX(_x);
        p_dc.setY(_y + 1);
        p_dr.setX(_x + 1);
        p_dr.setY(_y + 1);

        if (one_more_test.rgb() == this->green_.rgb())
            this->Nucleus_->inc_green();
        else if (one_more_test.rgb() == this->red_.rgb())
            this->Nucleus_->inc_red();
        else if (one_more_test.rgb() == this->dark_green_.rgb())
            this->Nucleus_->inc_black();

        this->imageForExtractCoordinates_->setPixel(_x, _y, this->blue_.rgb());

        this->vPointElong_.push_back(QPoint(_x, _y));

        FillOneNucleus(p_ul.x(), p_ul.y());
        FillOneNucleus(p_uc.x(), p_uc.y());
        FillOneNucleus(p_ur.x(), p_ur.y());

        FillOneNucleus(p_cl.x(), p_cl.y());
        FillOneNucleus(p_cr.x(), p_cr.y());

        FillOneNucleus(p_dl.x(), p_dl.y());
        FillOneNucleus(p_dc.x(), p_dc.y());
        FillOneNucleus(p_dr.x(), p_dr.y());
    }
}

void ProcessImageDBInterraction::EvalNucleusCenterCoordinate(const int& _x, const int& _y)
{
    if (_x < this->minXY_.rx())
        this->minXY_.setX(_x);
    if (_y < this->minXY_.ry())
        this->minXY_.setY(_y);
    if (_x > this->maxXY_.rx())
        this->maxXY_.setX(_x);
    if (_y > this->maxXY_.ry())
        this->maxXY_.setY(_y);
}

void ProcessImageDBInterraction::EvalTileIdAndCoordinatesInTile(const int& _x, const int& _y)
{
    int x = _x / this->TILESIDE_;
    int y = _y / this->TILESIDE_;
    int precisionX  = _x % 256;
    int precisionY  = _y % 256;
    this->tileIdToDB_ = this->amountTilesXinBigImage_ * (this->minTileYforEvalID_ + 1 + y) + this->minTileXforEvalID_ + 1 + x;  //  М Б    М И М О
    //    this->tileIdToDB_ = this->amountTilesXinBigImage_ * (this->minTIleYforEvalID_ + y) + x + 1;
    this->xToDB_ = precisionX;
    this->yToDB_ = precisionY;
}

const QVector<PositionInDB> &ProcessImageDBInterraction::getPositionsFromDB(const QPoint _startPoint, const QPoint _finishPoint)
{
    this->dataBase_->getObjectsCoordinates(_startPoint, _finishPoint);
    return this->dataBase_->getObjectsCoordinatesVector();
}

void ProcessImageDBInterraction::CreateTile(const QString& _tilePath, QImage& _image)
{
    if (QFile::exists(_tilePath))
    {
        _image.load(_tilePath);
        if (_image.size() != QSize(this->TILESIDE_, this->TILESIDE_))
        {
            _image = _image.scaled(this->TILESIDE_, this->TILESIDE_);
        }
    } else {
        if (_image.size() != QSize(this->TILESIDE_, this->TILESIDE_))
            _image = _image.scaled(this->TILESIDE_, this->TILESIDE_);
        _image.fill(Qt::white);
    }
}

bool ProcessImageDBInterraction::isWhiteImage(const QImage& _ImageForWhiteTest)
{
    qint32 sum = 0;
    int counter = 0;
    QImage imgForWhiteTest(_ImageForWhiteTest);
    int step = this->TILESIDE_ / 4;
    for (int h = 0; h < imgForWhiteTest.height(); h += step)
        for (int w = 0; w < imgForWhiteTest.width(); w += step)
        {
            QColor color = imgForWhiteTest.pixel(w, h);
            int averageRGBValue = (color.red() + color.green() + color.blue()) / 3;
            sum += averageRGBValue;
            counter++;
        }
    int result = sum / counter;
    if (result > 250)
        return true;
    else
        return false;
}

void ProcessImageDBInterraction::setRGBValues(const int& _red, const int& _green, const int& _blue)
{
    this->redValue_ = _red;
    this->greenValue_ = _green;
    this->blueValue_ = _blue;
}

void ProcessImageDBInterraction::setRGBDeltaValues(const int& _redDelta, const int& _greenDelta, const int& _blueDelta)
{
    this->deltaRed_ = _redDelta;
    this->deltaGreen_ = _greenDelta;
    this->deltaBlue_ = _blueDelta;
}

void ProcessImageDBInterraction::setSquareLimMinMaxValues(const int& _minSquare, const int& _maxSquare)
{
    this->squareMin_ = _minSquare;
    this->squareMax_ = _maxSquare;
}

void ProcessImageDBInterraction::setMinTileX_YforEvalID(const int& _minTileXforEvalID, const int& _minTileYforEvalID)
{
    this->minTileXforEvalID_ = _minTileXforEvalID;
    this->minTileYforEvalID_ = _minTileYforEvalID;
}

void ProcessImageDBInterraction::setAmountTilesXinBigImage(const int& _amountTilesXinBigImage)
{
    this->amountTilesXinBigImage_ = _amountTilesXinBigImage;
}

void ProcessImageDBInterraction::setFormFactorMax(const double& _formFctorMax)
{
    formFctorMax_ = _formFctorMax;
}

void ProcessImageDBInterraction::setZoomLevel(const int& _zoomLevel)
{
    zoomLevel_ = _zoomLevel;
}

void ProcessImageDBInterraction::setVectorTilePaths(const QVector<QVector<QVector<QString>>>& _vectorTilePaths)
{
    vectorTilePaths_ = _vectorTilePaths;
}
