#include "imagedescription.h"

ImageDescription::ImageDescription()
{

}

void ImageDescription::Initialize(const QString& _xmlFilePath)
{
    if (!this->zoomLevelsVector_.isEmpty())
        this->zoomLevelsVector_.clear();

    QDomDocument domDocument;
    QFile xmlFile(_xmlFilePath);
    if (xmlFile.open(QIODevice::ReadOnly))
    {
        if (domDocument.setContent(&xmlFile))
        {
            QDomElement domElement = domDocument.documentElement(); //<image>
            QDomNode domNode = domElement.firstChild();
            while (!domNode.isNull())
            {
                if (domNode.isElement())
                {
                    QDomElement domElement2 = domNode.toElement(); //<scale>
                    if (!domElement.isNull())
                    {
                        ZoomLevelDescription ZLDescription;
                        if (domElement2.tagName() == "scale")
                        {
                            ZLDescription.setValue(domElement2.attribute("value").toInt());
                            ZLDescription.setWidth(domElement2.attribute("width").toInt());
                            ZLDescription.setHeight(domElement2.attribute("height").toInt());
                            ZLDescription.setTileAmountW(domElement2.attribute("tile_amount_w").toInt() - 1);
                            ZLDescription.setTileAmountH(domElement2.attribute("tile_amount_h").toInt() - 1);
                            this->zoomLevelsVector_.push_back(ZLDescription);
                        }
                    }
                    domNode = domNode.nextSibling();
                }
            }
        } else qDebug() << "It`s no XML";
    } else qDebug() << "File not open =/";
}

const QVector<ZoomLevelDescription>& ImageDescription::GetImageDescription()
{
    return this->zoomLevelsVector_;
}
