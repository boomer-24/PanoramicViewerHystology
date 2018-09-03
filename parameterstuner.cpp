#include "parameterstuner.h"

ParametersTuner::ParametersTuner()
{    
    this->srcImage_ = new QImage();
    this->destImage_ = new QImage();
    this->green_.setRgb(40, 240, 70, 255);
    this->turquoise_.setRgb(110, 250, 250, 255);
    this->white_.setRgb(255,255,255,255);
    this->green_.setRgb(40, 240, 70, 255);
    this->red_.setRgb(240, 30, 30, 255);
    this->blue_.setRgb(0, 0, 255, 255);
    this->marker_.setRgb(125, 255, 120);
    this->black_.setRgb(0,0,0,255);
    this->dark_green_.setRgb(0, 128, 0, 225);

    //    gray_.setRgb(128,128,128,255);
    //    yellow_.setRgb(255, 255, 0);
}

ParametersTuner::~ParametersTuner()
{
    delete this->srcImage_;
    delete this->destImage_;
}

void ParametersTuner::GainPixels(QPoint _point)
{
    if (_point.rx() > 1 && _point.rx() < srcImage_->width() - 1 && _point.ry() > 1 && _point.ry() < srcImage_->height() - 1)
    {
        QColor colorNucl(srcImage_->pixel(_point));
        int Rc = std::abs(colorNucl.red() - this->selectedPixelColor_.red());
        int Gc = std::abs(colorNucl.green() - this->selectedPixelColor_.green());
        int Bc = std::abs(colorNucl.blue() - this->selectedPixelColor_.blue());

        if (Rc < this->delta_ && Gc < this->delta_ && Bc < this->delta_ && QColor(destImage_->pixel(_point)).rgba() != green_.rgba()/* && QColor(dest_->pixel(_x, _y)).rgba() != tmp.rgba()*/)
        {
            this->square_++;
            vRed_.push_back(colorNucl.red());
            vGreen_.push_back(colorNucl.green());
            vBlue_.push_back(colorNucl.blue());

            QPoint p_ul, p_uc, p_ur, p_cl, p_cr, p_dl, p_dc, p_dr;
            p_ul.setX(_point.rx() - 1);
            p_ul.setY(_point.ry() - 1);
            p_uc.setX(_point.rx());
            p_uc.setY(_point.ry() - 1);
            p_ur.setX(_point.rx() + 1);
            p_ur.setY(_point.ry() - 1);
            p_cl.setX(_point.rx() - 1);
            p_cl.setY(_point.ry());
            p_cr.setX(_point.rx() + 1);
            p_cr.setY(_point.ry());
            p_dl.setX(_point.rx() - 1);
            p_dl.setY(_point.ry() + 1);
            p_dc.setX(_point.rx());
            p_dc.setY(_point.ry() + 1);
            p_dr.setX(_point.rx() + 1);
            p_dr.setY(_point.ry() + 1);

            destImage_->setPixel(_point, green_.rgb());

            GainPixels(p_ul);
            GainPixels(p_uc);
            GainPixels(p_ur);

            GainPixels(p_cl);
            GainPixels(p_cr);

            GainPixels(p_dl);
            GainPixels(p_dc);
            GainPixels(p_dr);
        }
    }
}

void ParametersTuner::NearBorderMark()
{
    QColor pix_w_mh;
    QColor pix_mw_h;
    QColor pix_pw_h;
    QColor pix_w_ph;
    QColor bord;
    for (int h = 3; h < this->destImage_->height() - 3; h++)
        for (int w = 3; w < this->destImage_->width() - 3; w++)
        {
            bord.setRgb(destImage_->pixel(w, h));
            if (bord.rgb() != green_.rgb())
            {
                pix_w_mh.setRgb(destImage_->pixel(w, h - 1));
                pix_mw_h.setRgb(destImage_->pixel(w - 1, h));
                pix_pw_h.setRgb(destImage_->pixel(w + 1, h));
                pix_w_ph.setRgb(destImage_->pixel(w, h + 1));

                if (pix_w_mh.rgb() == green_.rgb() || pix_mw_h.rgb() == green_.rgb() || pix_pw_h.rgb() == green_.rgb() || pix_w_ph.rgb() == green_.rgb())
                    destImage_->setPixel(w, h, white_.rgb());
            }
        }
}

void ParametersTuner::BorderMark()
{
    this->amountBlue_ = 0;
    this->amountRed_ = 0;

    QColor pix_w_mh;
    QColor pix_mw_h;
    QColor pix_pw_h;
    QColor pix_w_ph;
    QColor bord;

    for (int h = 5; h < this->destImage_->height() - 5; h++)
        for (int w = 5; w < this->destImage_->width() - 5; w++)
        {
            bord.setRgb(destImage_->pixel(w, h));
            if (bord.rgb() == green_.rgb())
            {
                pix_w_mh.setRgb(destImage_->pixel(w, h - 1));
                pix_mw_h.setRgb(destImage_->pixel(w - 1, h));
                pix_pw_h.setRgb(destImage_->pixel(w + 1, h));
                pix_w_ph.setRgb(destImage_->pixel(w, h + 1));

                if (pix_w_mh.rgb() == white_.rgb() || pix_mw_h.rgb() == white_.rgb() || pix_pw_h.rgb() == white_.rgb() || pix_w_ph.rgb() == white_.rgb())
                {
                    destImage_->setPixel(w, h, blue_.rgb());
                    this->amountBlue_++;
                }
            }
        }
    QColor two;
    QColor u, l, r, d;
    for (int i = 5; i < destImage_->height() - 5; i++)
        for (int j = 5; j < destImage_->width() - 5; j++)
        {
            two.setRgb(destImage_->pixel(j, i));
            if (two.rgb() == green_.rgb())
            {
                u.setRgb(destImage_->pixel(j, i - 1));
                l.setRgb(destImage_->pixel(j - 1, i));
                r.setRgb(destImage_->pixel(j + 1, i));
                d.setRgb(destImage_->pixel(j, i + 1));
                if ((u.rgb() == blue_.rgb() && l.rgb() == blue_.rgb()) || (u.rgb() == blue_.rgb() && r.rgb() == blue_.rgb()) ||
                        (d.rgb() == blue_.rgb() && l.rgb() == blue_.rgb()) || (d.rgb() == blue_.rgb() && r.rgb() == blue_.rgb()))
                {
                    destImage_->setPixel(j, i, red_.rgb());
                    this->amountRed_++;
                }
            }
        }
}

void ParametersTuner::CalculatePerimeter()
{
    this->perimeter_ = this->amountBlue_ + this->amountRed_ * 0.5857;
}

void ParametersTuner::CalculateFormFactor()
{
    this->formFactor_ = perimeter_ / (sqrt(square_) * sqrt(3.14) * 2);
}

void ParametersTuner::CalculateAverage()
{
    this->averageRed_ = 0; this->averageGreen_ = 0; this->averageRed_ = 0;

    this->sumR = 0; this->Rmin = 255; this->Rmax = 0;
    for (int i = 0; i < this->vRed_.size(); i++)
    {
        if (this->Rmin > this->vRed_.at(i))
            this->Rmin = this->vRed_.at(i);
        if (this->Rmax < this->vRed_.at(i))
            this->Rmax = this->vRed_.at(i);
        this->sumR += this->vRed_.at(i);
    }
    if (this->vRed_.size() != 0) this->averageRed_ = this->sumR / this->vRed_.size();

    this->sumG = 0; this->Gmin = 255; this->Gmax = 0;
    for (int i = 0; i < this->vGreen_.size(); i++)
    {
        if (this->Gmin > this->vGreen_.at(i))
            this->Gmin = this->vGreen_.at(i);
        if (this->Gmax < this->vGreen_.at(i))
            this->Gmax = this->vGreen_.at(i);
        this->sumG += this->vGreen_.at(i);
    }
    if (this->vGreen_.size() != 0) this->averageGreen_ = this->sumG / this->vGreen_.size();

    this->sumB = 0; this->Bmin = 255; this->Bmax = 0;
    for (int i = 0; i < this->vBlue_.size(); i++)
    {
        if (this->Bmin > this->vBlue_.at(i))
            this->Bmin = this->vBlue_.at(i);
        if (this->Bmax < this->vBlue_.at(i))
            this->Bmax = this->vBlue_.at(i);
        this->sumB += this->vBlue_.at(i);
    }
    if (this->vBlue_.size() != 0) this->averageBlue_ = this->sumB / this->vBlue_.size();
}

void ParametersTuner::CalculateMedianValue()
{
    qSort(this->vRed_);
    qSort(this->vGreen_);
    qSort(this->vBlue_);
    this->medianRed_ = this->vRed_.at(this->vRed_.size() / 2);
    this->medianGreen_ = this->vGreen_.at(this->vGreen_.size() / 2);
    this->medianBlue_ = this->vBlue_.at(this->vBlue_.size() / 2);
}

void ParametersTuner::CalculateAsymmetry()
{
    long r_temp_sum = 0;
    long g_temp_sum = 0;
    long b_temp_sum = 0;
    int size_vect = vRed_.size();
    for (int i = 0; i < size_vect; i++)
    {
        r_temp_sum += std::pow(vRed_.at(i) - averageRed_, 3);
        g_temp_sum += std::pow(vGreen_.at(i) - averageGreen_, 3);
        b_temp_sum += std::pow(vBlue_.at(i) - averageBlue_, 3);
    }
    double chislitelR = r_temp_sum / size_vect;
    double chislitelG = g_temp_sum / size_vect;
    double chislitelB = b_temp_sum / size_vect;

    double znamenatelR = this->stdDevR_;
    double znamenatelG = this->stdDevG_;
    double znamenatelB = this->stdDevB_;

    this->asymmetryR_ = chislitelR / std::pow(znamenatelR, 3);
    this->asymmetryG_ = chislitelG / std::pow(znamenatelG, 3);
    this->asymmetryB_ = chislitelB / std::pow(znamenatelB, 3);

}

void ParametersTuner::CalculateStdDeviation()
{
    int r_temp_sum = 0;
    int g_temp_sum = 0;
    int b_temp_sum = 0;
    int size_vect = vRed_.size();
    for (int i = 0; i < size_vect; i++)
    {
        r_temp_sum += std::pow(vRed_.at(i) - averageRed_, 2);
        g_temp_sum += std::pow(vGreen_.at(i) - averageGreen_, 2);
        b_temp_sum += std::pow(vBlue_.at(i) - averageBlue_, 2);
    }
    this->stdDevR_ = sqrt(r_temp_sum / (size_vect /*- 1*/));  // maybe size_vect - 1 ???
    this->stdDevG_ = sqrt(g_temp_sum / (size_vect /*- 1*/));
    this->stdDevB_ = sqrt(b_temp_sum / (size_vect /*- 1*/));
}

void ParametersTuner::CalculateExcess()
{
    long r_temp_sum = 0;
    long g_temp_sum = 0;
    long b_temp_sum = 0;
    int size_vect = vRed_.size();
    for (int i = 0; i < size_vect; i++)
    {
        r_temp_sum += std::pow(vRed_.at(i) - averageRed_, 4);
        g_temp_sum += std::pow(vGreen_.at(i) - averageGreen_, 4);
        b_temp_sum += std::pow(vBlue_.at(i) - averageBlue_, 4);
    }
    double chislitelR = r_temp_sum / size_vect;
    double chislitelG = g_temp_sum / size_vect;
    double chislitelB = b_temp_sum / size_vect;

    double znamenatelR = this->stdDevR_;
    double znamenatelG = this->stdDevG_;
    double znamenatelB = this->stdDevB_;

    this->excessR_ = chislitelR / std::pow(znamenatelR, 4) - 3;
    this->excessG_ = chislitelG / std::pow(znamenatelG, 4) - 3;
    this->excessB_ = chislitelB / std::pow(znamenatelB, 4) - 3;
}

const QPoint& ParametersTuner::GetSelectedPixelForGain()
{
    return this->selectedPixel_;
}

void ParametersTuner::SetSrcImage(QImage _forSrcImage)
{
    *this->srcImage_ = _forSrcImage;
    *this->destImage_ = *this->srcImage_;
}

const QImage ParametersTuner::GetDestImage() const
{
    return *this->destImage_;
}

void ParametersTuner::SetSelectedPixel(const QPoint& _coordinated)
{
    this->selectedPixel_ = _coordinated;
    this->selectedPixelColor_ = this->srcImage_->pixel(_coordinated);
    this->destImage_->setPixel(_coordinated, this->turquoise_.rgba());
}

void ParametersTuner::SetDelta(int _delta)
{
    this->delta_ = _delta;
}

void ParametersTuner::Reset()
{
    this->vBlue_.clear();
    this->vGreen_.clear();
    this->vRed_.clear();
    this->square_ = 0;
    this->perimeter_ = 0.0;
    this->formFactor_ = 0.0;
    this->stdDevR_ = 0.0;
    this->stdDevG_ = 0.0;
    this->stdDevB_ = 0.0;
    this->asymmetryR_ = 0.0;
    this->asymmetryG_ = 0.0;
    this->asymmetryB_ = 0.0;
    this->excessR_ = 0.0;
    this->excessG_ = 0.0;
    this->excessB_ = 0.0;
}

void ParametersTuner::ProcessImage()
{
    this->ThresholdFilter();
    this->SquareFilter();
    this->FormFactorFilter();
}

void ParametersTuner::ThresholdFilter()
{
    QColor qwerty_thresh;
    int redMax = this->Red_ + redDelta_;
    int greenMax = this->Green_ + greenDelta_;
    int blueMax = this->Blue_ + blueDelta_;
    int redMin = this->Red_ - redDelta_;
    int greenMin = this->Green_ - greenDelta_;
    int blueMin = this->Blue_ - blueDelta_;
    (redMax > 255) ? redMax = 255 : redMax = redMax;
    (redMin < 0) ? redMin = 0 : redMin = redMin;
    (greenMax > 255) ? greenMax = 255 : greenMax = greenMax;
    (greenMin < 0) ? greenMin = 0 : greenMin = greenMin;
    (blueMax > 255) ? blueMax = 255 : blueMax = blueMax;
    (blueMin < 0) ? blueMin = 0 : blueMin = blueMin;

    for (int h = 0; h < this->srcImage_->height(); h++)
        for (int w = 0; w < this->srcImage_->width(); w++)
        {
            qwerty_thresh.setRgb(this->srcImage_->pixel(w, h));
            if (redMin < qwerty_thresh.red() && qwerty_thresh.red() < redMax &&
                    greenMin < qwerty_thresh.green() && qwerty_thresh.green() < greenMax &&
                    blueMin < qwerty_thresh.blue() && qwerty_thresh.blue() < blueMax)
                qwerty_thresh.setRgb(this->marker_.rgb());
            else qwerty_thresh.setRgb(this->srcImage_->pixel(w, h));
            this->destImage_->setPixel(w, h, qwerty_thresh.rgb());
        }
}

void ParametersTuner::SquareFilter()
{
    QColor tobin;
    for (int h = 0; h < this->destImage_->height(); h++)
        for (int w = 0; w < this->destImage_->width(); w++)
        {
            tobin.setRgb(this->destImage_->pixel(w, h));
            if (tobin.rgb() == this->marker_.rgb())
                this->destImage_->setPixel(w, h, black_.rgb());
            else this->destImage_->setPixel(w, h, white_.rgb());
        }
    // ////////////////////  БИНАРИЗОВАЛ ВСЁ
    for (int h = 0; h < this->destImage_->height(); h++)
    {
        this->destImage_->setPixel(0, h, white_.rgb());
        this->destImage_->setPixel(this->destImage_->width() - 1, h, white_.rgb());
    }
    for (int w = 0; w < this->destImage_->width(); w++)
    {
        this->destImage_->setPixel(w, 0, white_.rgb());
        this->destImage_->setPixel(w, this->destImage_->height() - 1, white_.rgb());
    }
    // //////////////////////////////////////////НАЧЕРТИЛ ГРАНИЦУ
    QColor kill;
    for (int j = 1; j < this->destImage_->height() - 1; j++)
        for (int i = 1; i < this->destImage_->width() - 1; i++)
        {
            kill.setRgb(this->destImage_->pixel(i, j));
            if (kill.rgb() != white_.rgb())
            {
                this->kill_trash_vector_.clear();
                KillTrash(i, j);
                if (this->kill_trash_vector_.size() != 0)
                {
                    if (kill_trash_vector_.size() < this->squareMin_ || kill_trash_vector_.size() > this->squareMax_)
                    {
                        for (int i = 0; i < kill_trash_vector_.size(); i++)
                            this->destImage_->setPixel(kill_trash_vector_.at(i), white_.rgb());
                    }
                }
            }
        }
    QColor is_green;
    for (int h = 0; h < this->destImage_->height(); h++)
        for (int w = 0; w < this->destImage_->width(); w++)
        {
            is_green.setRgb(this->destImage_->pixel(w, h));
            if (is_green.rgb() != white_.rgb())
                is_green.setRgb(marker_.rgb());
            else (is_green.setRgb(QColor(this->srcImage_->pixel(w, h)).rgb()));
            this->destImage_->setPixel(w, h, is_green.rgb());
        }

}

void ParametersTuner::KillTrash(const int _x, const int _y)
{
    QColor is_black;
    is_black.setRgb(this->destImage_->pixel(_x, _y));
    if ((is_black.rgb() != white_.rgb()) && (is_black.rgb() != dark_green_.rgb()))
    {
        QPoint dot(_x, _y);
        this->kill_trash_vector_.push_back(dot);

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

        this->destImage_->setPixel(_x, _y, dark_green_.rgb());

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

void ParametersTuner::BorderMarkForAll()
{
    //  RED-GREEN MARK
    QColor tobin;
    for (int h = 0; h < this->destImage_->height(); h++)
        for (int w = 0; w < this->destImage_->width(); w++)
        {
            tobin.setRgb(this->destImage_->pixel(w, h));
            if (tobin.rgb() == marker_.rgb())
                this->destImage_->setPixel(w, h, black_.rgb());
            else this->destImage_->setPixel(w, h, white_.rgb());
        }

    QColor bord;
    QColor pix_w_mh;
    QColor pix_mw_h;
    QColor pix_pw_h;
    QColor pix_w_ph;

    for (int h = 1; h < this->destImage_->height() - 1; h++)
        for (int w = 1; w < this->destImage_->width() - 1; w++)
        {
            bord.setRgb(this->destImage_->pixel(w, h));
            if (bord.rgb() == black_.rgb())
            {
                pix_w_mh.setRgb(this->destImage_->pixel(w, h - 1));
                pix_mw_h.setRgb(this->destImage_->pixel(w - 1, h));
                pix_pw_h.setRgb(this->destImage_->pixel(w + 1, h));
                pix_w_ph.setRgb(this->destImage_->pixel(w, h + 1));

                if (pix_w_mh.rgb() == white_.rgb() || pix_mw_h.rgb() == white_.rgb() ||
                        pix_pw_h.rgb() == white_.rgb() || pix_w_ph.rgb() == white_.rgb())
                    this->destImage_->setPixel(w, h, green_.rgb());
            }
        }
    QColor two;
    QColor u, l, r, d;
    for (int i = 1; i < this->destImage_->height() - 1; i++)
        for (int j = 1; j < this->destImage_->width() - 1; j++)
        {
            two.setRgb(this->destImage_->pixel(j, i));
            if (two.rgb() == black_.rgb())
            {
                u.setRgb(this->destImage_->pixel(j, i - 1));
                l.setRgb(this->destImage_->pixel(j - 1, i));
                r.setRgb(this->destImage_->pixel(j + 1, i));
                d.setRgb(this->destImage_->pixel(j, i + 1));
                if ((u.rgb() == green_.rgb() && l.rgb() == green_.rgb()) ||
                        (u.rgb() == green_.rgb() && r.rgb() == green_.rgb()) ||
                        (d.rgb() == green_.rgb() && l.rgb() == green_.rgb()) ||
                        (d.rgb() == green_.rgb() && r.rgb() == green_.rgb()))
                    this->destImage_->setPixel(j, i, red_.rgb());
            }
        }
}

void ParametersTuner::FormFactorFilter()
{
    this->BorderMarkForAll();
    for (int h = 0; h < this->destImage_->height(); h++)
    {
        this->destImage_->setPixel(0, h, white_.rgb());
        this->destImage_->setPixel(this->destImage_->width() - 1, h, white_.rgb());
    }
    for (int w = 0; w < this->destImage_->width(); w++)
    {
        this->destImage_->setPixel(w, 0, white_.rgb());
        this->destImage_->setPixel(w, this->destImage_->height() - 1, white_.rgb());
    }
    // ////////////////////////////////////////////// red-green-black-white БИНАРИЗАЦИЯ + ГРАНИЦЫ
    QColor test_col;
    this->Test_Nucl_ = new Nucleus;
    for (int h = 1; h < this->destImage_->height(); h++)
        for (int w = 1; w < this->destImage_->width(); w++)
        {
            test_col.setRgb(this->destImage_->pixel(w, h));
            if (test_col.rgb() != white_.rgb() && test_col.rgb() != blue_.rgb())
            {
                if (!this->Point_Elong_Vect_.empty())
                    this->Point_Elong_Vect_.clear();
                this->Test_Nucl_->reset_all();

                //                this->minXY_ = QPoint(w, h);
                //                this->maxXY_ = QPoint(w, h);
                this->FillOneNucleus(w, h);

                this->Test_Nucl_->calculate_perimeter();
                this->Test_Nucl_->calculate_square();      //подумать...такое вроде уже есть____НУ И ПОХ
                this->Test_Nucl_->calculate_form_factor();
                if (this->Test_Nucl_->return_form_factor() > this->formFactorForProcessImage_)
                {
                    for (int i = 0; i < this->Point_Elong_Vect_.size(); i++)
                    {
                        this->destImage_->setPixel(this->Point_Elong_Vect_.at(i), white_.rgb());
                    }
                } else
                {
                    //                    int centerX = this->minXY_.rx() + (this->maxXY_.rx() - this->minXY_.rx()) / 2;
                    //                    int centerY = this->minXY_.ry() + (this->maxXY_.ry() - this->minXY_.ry()) / 2;
                    //                    this->EvalTileIdAndCoordinatesInTile(centerX, centerY);
                    //                    this->dataBase_->InsertIntoObjectTable(this->toDBtileId_, this->toDBx_, this->toDBy_);
                }
            }
        }
    delete this->Test_Nucl_;
    QColor is_green;
    for (int h = 0; h < this->destImage_->height(); h++)
        for (int w = 0; w < this->destImage_->width(); w++)
        {
            is_green.setRgb(this->destImage_->pixel(w, h));
            if (is_green.rgb() != white_.rgb())
                is_green.setRgb(marker_.rgb());
            else (is_green.setRgb(QColor(this->srcImage_->pixel(w, h)).rgb()));
            this->destImage_->setPixel(w, h, is_green.rgb());
        }
    //    this->dest_.save("D:/blablabla.bmp");
}

void ParametersTuner::FillOneNucleus(const int &_x, const int &_y)
{
    QColor one_more_test;
    one_more_test.setRgb(this->destImage_->pixel(_x, _y));

    if (one_more_test.rgb() != blue_.rgb() && one_more_test.rgb() != white_.rgb())
    {
        //        this->EvalNucleusCenterCoordinate(x, y);//////////// HERE

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

        if (one_more_test.rgb() == green_.rgb())
            this->Test_Nucl_->inc_green();
        else if (one_more_test.rgb() == red_.rgb())
            this->Test_Nucl_->inc_red();
        else if (one_more_test.rgb() == black_.rgb())
            this->Test_Nucl_->inc_black();

        this->destImage_->setPixel(_x, _y, blue_.rgb());

        this->Point_Elong_Vect_.push_back(QPoint(_x, _y));

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

void ParametersTuner::setRedForTreshold(int _redForTreshold)
{
    redForTreshold_ = _redForTreshold;
}

void ParametersTuner::setGreenForTreshold(int _greenForTreshold)
{
    greenForTreshold_ = _greenForTreshold;
}

void ParametersTuner::setBlueForTreshold(int _blueForTreshold)
{
    blueForTreshold_ = _blueForTreshold;
}

void ParametersTuner::setRedDeltaForTreshold(int _redDeltaForTreshold)
{
    redDeltaForTreshold_ = _redDeltaForTreshold;
}

void ParametersTuner::setGreenDeltaForTreshold(int _greenDeltaForTreshold)
{
    greenDeltaForTreshold_ = _greenDeltaForTreshold;
}

void ParametersTuner::setBlueDeltaForTreshold(int _blueDeltaForTreshold)
{
    blueDeltaForTreshold_ = _blueDeltaForTreshold;
}

void ParametersTuner::setSquareMin(int _squareMin)
{
    squareMin_ = _squareMin;
}

void ParametersTuner::setSquareMax(int _squareMax)
{
    squareMax_ = _squareMax;
}

int ParametersTuner::getAverageRed() const
{
    return averageRed_;
}

int ParametersTuner::getAverageGreen() const
{
    return averageGreen_;
}

int ParametersTuner::getAverageBlue() const
{
    return averageBlue_;
}

int ParametersTuner::getRmin() const
{
    return Rmin;
}

int ParametersTuner::getRmax() const
{
    return Rmax;
}

int ParametersTuner::getGmin() const
{
    return Gmin;
}

int ParametersTuner::getGmax() const
{
    return Gmax;
}

int ParametersTuner::getBmin() const
{
    return Bmin;
}

int ParametersTuner::getBmax() const
{
    return Bmax;
}

double ParametersTuner::getStdDevR() const
{
    return stdDevR_;
}

double ParametersTuner::getStdDevG() const
{
    return stdDevG_;
}

double ParametersTuner::getStdDevB() const
{
    return stdDevB_;
}

double ParametersTuner::getAsymmetryR() const
{
    return asymmetryR_;
}

double ParametersTuner::getAsymmetryG() const
{
    return asymmetryG_;
}

double ParametersTuner::getAsymmetryB() const
{
    return asymmetryB_;
}

double ParametersTuner::getExcessR() const
{
    return excessR_;
}

double ParametersTuner::getExcessG() const
{
    return excessG_;
}

double ParametersTuner::getExcessB() const
{
    return excessB_;
}

int ParametersTuner::getMedianRed() const
{
    return medianRed_;
}

int ParametersTuner::getMedianGreen() const
{
    return medianGreen_;
}

int ParametersTuner::getMedianBlue() const
{
    return medianBlue_;
}

double ParametersTuner::getFormFactor() const
{
    return formFactor_;
}

int ParametersTuner::getSquare() const
{
    return square_;
}

void ParametersTuner::setRed(int _Red)
{
    Red_ = _Red;
}

void ParametersTuner::setRedDelta(int _redDelta)
{
    redDelta_ = _redDelta;
}

void ParametersTuner::setGreen(int _Green)
{
    Green_ = _Green;
}

void ParametersTuner::setGreenDelta(int _greenDelta)
{
    greenDelta_ = _greenDelta;
}

void ParametersTuner::setBlue(int _Blue)
{
    Blue_ = _Blue;
}

void ParametersTuner::setBlueDelta(int _blueDelta)
{
    blueDelta_ = _blueDelta;
}

void ParametersTuner::setFormFactorForProcessImage(double _formFactorForProcessImage)
{
    formFactorForProcessImage_ = _formFactorForProcessImage;
}
