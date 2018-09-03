#ifndef PROCESSIMAGEDBINTERRACTION_H
#define PROCESSIMAGEDBINTERRACTION_H

#include "imagedescription.h"
#include "dbhandler.h"
#include "nucleus.h"

class ProcessImageDBInterraction
{
public:
    ProcessImageDBInterraction();   

    ProcessImageDBInterraction(QVector<QVector<QVector<QString>>>& _vectorTilePaths);
    void CreateConnection();
    void CreateConnection(const QString& _pathToDB);
    void FillingTileTable(const int& _w, const int& _h);
    void FillingTileTable();
    void ProcessImage();    
    void ThresholdFilter();
    void SquareFilter();
    void KillTrash(const int& _x, const int& _y);
    void BorderMark();
    void FormFactorFilter();
    void FillOneNucleus(const int& _x, const int& _y);
    void EvalNucleusCenterCoordinate(const int& _x, const int& _y);
    void EvalTileIdAndCoordinatesInTile(const int& _x, const int& _y);

    const QVector<PositionInDB>& getPositionsFromDB(const QPoint _startPoint, const QPoint _finishPoint) /*const*/;
    void CreateTile(const QString& _tilePath, QImage& _image);
    bool isWhiteImage(const QImage& _ImageForWhiteTest);

    void setVectorTilePaths(const QVector<QVector<QVector<QString>>>& _vectorTilePaths);
    void setRGBValues(const int& _red, const int& _green, const int& _blue);
    void setRGBDeltaValues(const int& _redDelta, const int& _greenDelta, const int& _blueDelta);
    void setSquareLimMinMaxValues(const int& _minSquare, const int& _maxSquare);
    void setMinTileX_YforEvalID(const int& _minTileXforEvalID, const int& _minTileYforEvalID);
    void setAmountTilesXinBigImage(const int& _amountTilesXinBigImage);
    void setFormFactorMax(const double& _formFctorMax);
    void setZoomLevel(const int& _zoomLevel);


private:
    const int TILESIDE_ = 256;    
    QVector<QVector<QVector<QString>>> vectorTilePaths_;
    DBHandler* dataBase_;
    QImage* imageForExtractCoordinates_;
    Nucleus* Nucleus_;
    int zoomLevel_;
    QSize sizeofImageInTiles_ = QSize(11, 11);

    QColor black_, white_, /*gray_, */green_, red_, /*yellow_, */dark_green_, blue_, turquoise_, marker_;
    int redValue_, greenValue_, blueValue_;
    int deltaRed_, deltaGreen_, deltaBlue_;
    int squareMin_, squareMax_;

    QVector<QPoint> vPointElong_;
    double formFctorMax_;
    QVector<QPoint> vKillTrash_;
    QPoint minXY_, maxXY_;

    int minTileXforEvalID_, minTileYforEvalID_, amountTilesXinBigImage_;
    int tileIdToDB_, xToDB_, yToDB_;
};

#endif // PROCESSIMAGEDBINTERRACTION_H
