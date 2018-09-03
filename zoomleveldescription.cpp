#include "zoomleveldescription.h"

ZoomLevelDescription::ZoomLevelDescription()
{

}

ZoomLevelDescription::ZoomLevelDescription(int _value, int _width, int _height, int _tileAmountW, int _tileAmountH)
{
    this->value_ = _value;
    this->width_ = _width;
    this->height_ = _height;
    this->tileAmountW_ = _tileAmountW - 1;
    this->tileAmountH_ = _tileAmountH - 1;
}

int ZoomLevelDescription::width() const
{
    return this->width_;
}

void ZoomLevelDescription::setWidth(int _width)
{
    this->width_ = _width;
}

int ZoomLevelDescription::height() const
{
    return this->height_;
}

void ZoomLevelDescription::setHeight(int _height)
{
    this->height_ = _height;
}

int ZoomLevelDescription::tileAmountW() const
{
    return this->tileAmountW_;
}

void ZoomLevelDescription::setTileAmountW(int _tileAmountW)
{
    this->tileAmountW_ = _tileAmountW;
}

int ZoomLevelDescription::tileAmountH() const
{
    return this->tileAmountH_;
}

void ZoomLevelDescription::setTileAmountH(int _tileAmountH)
{
    this->tileAmountH_ = _tileAmountH;
}


int ZoomLevelDescription::value() const
{
    return this->value_;
}

void ZoomLevelDescription::setValue(int _value)
{
    this->value_ = _value;
}

