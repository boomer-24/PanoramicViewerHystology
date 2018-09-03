#ifndef IMAGEDESCRIPTION_H
#define IMAGEDESCRIPTION_H

#include "zoomleveldescription.h"
#include <QVector>
#include <QtXml>

class ImageDescription
{
private:
    QVector<ZoomLevelDescription> zoomLevelsVector_;
public:
    ImageDescription(); //  МБ НУЖЕН COPY CONSTRUCTOR
    void Initialize(const QString& _xmlFilePath);
    const QVector<ZoomLevelDescription>& GetImageDescription();
};

#endif // IMAGEDESCRIPTION_H
