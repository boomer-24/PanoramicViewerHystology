#ifndef ZOOMLEVELDESCRIPTION_H
#define ZOOMLEVELDESCRIPTION_H


class ZoomLevelDescription
{
private:
    int value_, width_, height_, tileAmountW_, tileAmountH_;
public:
    ZoomLevelDescription();
    ZoomLevelDescription(int _value, int _width, int _height, int _tileAmountW, int _tileAmountH);

    int value() const;
    void setValue(int _value);

    int width() const;
    void setWidth(int _width);

    int height() const;
    void setHeight(int _height);

    int tileAmountW() const;
    void setTileAmountW(int _tileAmountW);

    int tileAmountH() const;
    void setTileAmountH(int _tileAmountH);
};

#endif // ZOOMLEVELDESCRIPTION_H
