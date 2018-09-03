#ifndef PARAMETERSTUNER_H
#define PARAMETERSTUNER_H

#include "nucleus.h"

#include <QImage>
#include <QtAlgorithms>

class ParametersTuner
{
public:
    ParametersTuner();
    ~ParametersTuner();
    void GainPixels(QPoint _point);
    void NearBorderMark();
    void BorderMark();
    void CalculatePerimeter();
    void CalculateFormFactor();
    void CalculateAverage();
    void CalculateMedianValue();
    void CalculateAsymmetry();
    void CalculateStdDeviation();
    void CalculateExcess();
    const QPoint& GetSelectedPixelForGain();
    void SetSrcImage(QImage _forSrcImage);
    const QImage GetDestImage() const;
    void SetSelectedPixel(const QPoint& _coordinated);
    void SetDelta(int _delta);
    void Reset();

    void ProcessImage();
    void ThresholdFilter();
    void SquareFilter();
    void KillTrash(const int _x, const int _y);
    void BorderMarkForAll();
    void FormFactorFilter();
    void FillOneNucleus(const int& _x, const int& _y);

    void setRedForTreshold(int _redForTreshold);
    void setGreenForTreshold(int _greenForTreshold);
    void setBlueForTreshold(int _blueForTreshold);
    void setRedDeltaForTreshold(int _redDeltaForTreshold);
    void setGreenDeltaForTreshold(int _greenDeltaForTreshold);
    void setBlueDeltaForTreshold(int _blueDeltaForTreshold);

    int getAverageRed() const;
    int getAverageGreen() const;
    int getAverageBlue() const;
    int getRmin() const;
    int getRmax() const;
    int getGmin() const;
    int getGmax() const;
    int getBmin() const;
    int getBmax() const;
    double getStdDevR() const;
    double getStdDevG() const;
    double getStdDevB() const;
    double getAsymmetryR() const;
    double getAsymmetryG() const;
    double getAsymmetryB() const;
    double getExcessR() const;
    double getExcessG() const;
    double getExcessB() const;
    int getMedianRed() const;
    int getMedianGreen() const;
    int getMedianBlue() const;
    double getFormFactor() const;
    int getSquare() const;

    void setRed(int _Red);
    void setRedDelta(int _redDelta);
    void setGreen(int _Green);
    void setGreenDelta(int _greenDelta);
    void setBlue(int _Blue);
    void setBlueDelta(int _blueDelta);
    void setSquareMin(int _squareMin);
    void setSquareMax(int _squareMax);
    void setFormFactorForProcessImage(double _formFactorForProcessImage);

private:
    QImage *srcImage_, *destImage_;
    QPoint selectedPixel_;
    QColor selectedPixelColor_;
    QColor green_, turquoise_, white_, blue_, red_, marker_, black_, dark_green_;
    QVector<int> vRed_;
    QVector<int> vGreen_;
    QVector<int> vBlue_;
    int delta_, square_;
    double perimeter_, formFactor_;
    int averageRed_ = 0, averageGreen_ = 0, averageBlue_ = 0;    
    int medianRed_ = 0, medianGreen_ = 0, medianBlue_ = 0;

    int redForTreshold_, greenForTreshold_, blueForTreshold_;
    int redDeltaForTreshold_, greenDeltaForTreshold_, blueDeltaForTreshold_;
    int amountRed_, amountBlue_;

    int sumR = 0;
    int Rmin = 255;
    int Rmax = 0;
    int sumG = 0;
    int Gmin = 255;
    int Gmax = 0;
    int sumB = 0;
    int Bmin = 255;
    int Bmax = 0;

    double stdDevR_, stdDevG_, stdDevB_;
    double asymmetryR_, asymmetryG_, asymmetryB_;
    double excessR_, excessG_, excessB_;

    int squareMin_, squareMax_;
    int Red_, redDelta_, Green_, greenDelta_, Blue_, blueDelta_;
    double formFactorForProcessImage_;

    QVector<QPoint> kill_trash_vector_;
    QVector<QPoint> Point_Elong_Vect_;
    Nucleus* Test_Nucl_;
};

#endif // PARAMETERSTUNER_H
