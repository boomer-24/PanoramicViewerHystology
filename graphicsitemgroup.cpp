#include "graphicsitemgroup.h"

GraphicsItemGroup::GraphicsItemGroup(QObject* _parent) : QObject(_parent)
{

}

void GraphicsItemGroup::InitializeTilePathsVector(const QVector<ZoomLevelDescription>& _vectorZoomLevelDescription)
{
    //    this->currentZoomLevel_ = 0;
    const QString ySymbols("y=");
    const QString xSymbols("x=");
    const QString fileFormatSymbols(".png");
    QString path("");

    if (!this->vectorTilePaths_.empty())
        this->vectorTilePaths_.clear();
    if (!this->vectorGraphicsPixmapItemPointers_.empty())
        this->vectorGraphicsPixmapItemPointers_.clear();

    for (int level = 0; level < _vectorZoomLevelDescription.size(); level++)
    {
        const ZoomLevelDescription& ZL = _vectorZoomLevelDescription.at(level);
        QVector<QVector<QString>> temp_2_PathVector;
        QVector<QVector<QGraphicsPixmapItem*>> temp_2_ItemVector;
        for (int h = 0; h < ZL.tileAmountH(); h++)
        {
            QVector<QString> temp_1_PathVector;
            QVector<QGraphicsPixmapItem*> temp_1_ItemVector;
            for (int w = 0; w < ZL.tileAmountW(); w++)
            {
                path.clear();
                path.append(this->storagePath_).append(QString::number(level + 1)).append("/");
                path.append(ySymbols).append(QString::number(h)).append(xSymbols).append(QString::number(w)).append(fileFormatSymbols);
                temp_1_PathVector.push_back(path);
                QGraphicsPixmapItem* pixItem;
                temp_1_ItemVector.push_back(pixItem);
            }
            temp_2_PathVector.push_back(temp_1_PathVector);
            temp_1_PathVector.clear();
            temp_2_ItemVector.push_back(temp_1_ItemVector);
            temp_1_ItemVector.clear();
        }
        this->vectorTilePaths_.push_back(temp_2_PathVector);
        this->vectorGraphicsPixmapItemPointers_.push_back(temp_2_ItemVector);
    }

    this->currentZoomLevel_ = this->vectorGraphicsPixmapItemPointers_.size() - 1;

    this->upperLeftVisibleTileNumber_.setX(vectorGraphicsPixmapItemPointers_.at(currentZoomLevel_).at(0).size() / 2 - this->itemGroupSize_.width() / 6);
    this->upperLeftVisibleTileNumber_.setY(vectorGraphicsPixmapItemPointers_.at(currentZoomLevel_).size() / 2 - this->itemGroupSize_.height() / 6); // for FirstAdditionToItemGroup
}

void GraphicsItemGroup::InitializedVectorHeightInPixAllLevels(const QVector<ZoomLevelDescription>& _vectorImageDescription)
{
    for (int i = 0; i < _vectorImageDescription.size(); i++)
    {
        this->vectorHeightInPixAllLevels_.push_back(_vectorImageDescription.at(i).height());
    }
}

void GraphicsItemGroup::FirstEdditionToItemGroup()  //  invoke once
{
    if (!this->vectorGraphicsPixmapItemPointers_.isEmpty())
    {
        this->startPoint_.setY(this->upperLeftVisibleTileNumber_.y() - this->itemGroupSize_.height() / 3);
        this->startPoint_.setX(this->upperLeftVisibleTileNumber_.x() - this->itemGroupSize_.width() / 3);
        (this->startPoint_.ry() < 0) ? (this->startPoint_.setY(0)) : (this->startPoint_.setY(this->startPoint_.ry()));
        (this->startPoint_.rx() < 0) ? (this->startPoint_.setX(0)) : (this->startPoint_.setX(this->startPoint_.rx()));

        this->finishPoint_.setY(this->startPoint_.ry() + this->itemGroupSize_.height());
        this->finishPoint_.setX(this->startPoint_.rx() + this->itemGroupSize_.width());
        (this->finishPoint_.ry() > this->vectorGraphicsPixmapItemPointers_.at(this->currentZoomLevel_).size()) ?
                    (this->finishPoint_.setY(this->vectorGraphicsPixmapItemPointers_.at(this->currentZoomLevel_).size())) :
                    (this->finishPoint_.setY(this->finishPoint_.ry()));
        (this->finishPoint_.rx() > this->vectorGraphicsPixmapItemPointers_.at(this->currentZoomLevel_).at(0).size()) ?
                    (this->finishPoint_.setX(this->vectorGraphicsPixmapItemPointers_.at(this->currentZoomLevel_).at(0).size())) :
                    (this->finishPoint_.setX(this->finishPoint_.rx()));
        this->AddTilesToPixmapItemGroup(this->startPoint_, this->finishPoint_);

//        emit this->signalForObtainMarkersCoordinated(this->startPoint_, this->finishPoint_);
        emit this->signalForPreview(this->currentZoomLevel_, this->startPoint_, this->finishPoint_);

    }
}

void GraphicsItemGroup::EvalLimits()
{
    QPoint thisPos(this->pos().toPoint());
    int up = thisPos.ry() - this->startPoint_.ry() * this->TILESIDE_;
    int right = thisPos.rx() + this->itemGroupSize_.width() * this->TILESIDE_ +
            (this->vectorGraphicsPixmapItemPointers_.at(this->currentZoomLevel_).size() - this->finishPoint_.rx()) * this->TILESIDE_;
    int down = thisPos.ry() + this->itemGroupSize_.height() * this->TILESIDE_ +
            (this->vectorGraphicsPixmapItemPointers_.at(this->currentZoomLevel_).at(0).size() - this->finishPoint_.ry()) * this->TILESIDE_;
    int left = thisPos.rx() - this->startPoint_.rx() * this->TILESIDE_;
    this->upLimit_ = up;
    this->rightLimit_ = right;
    this->downLimit_ = down;
    this->leftLimit_ = left;
}

void GraphicsItemGroup::CheckExtremeTileIndexes(QPoint& _startPoint, QPoint& _finishPoint)  // ДЛЯ АВТОМАТИЧЕСКИХ ПЕРЕМЕННЫХ
{
    if (_startPoint.ry() < 0)
    {
        qDebug() << "OUT OF RANGE (Y): " << _startPoint.ry();
        _startPoint.setY(0);
        //        _finishPoint.setY(_startPoint.ry() + this->itemGroupSize_.rheight());
    }
    if (_startPoint.rx() < 0)
    {
        qDebug() << "OUT OF RANGE (X): " << _startPoint.rx();
        _startPoint.setX(0);
        //        _finishPoint.setX(_startPoint.rx() + this->itemGroupSize_.rwidth());
    }
    if (_finishPoint.ry() > this->vectorGraphicsPixmapItemPointers_.at(this->currentZoomLevel_).size()/* - 1*/)
    {
        qDebug() << "OUT OF RANGE (Y): " << _finishPoint.ry() << "(max: " << this->vectorGraphicsPixmapItemPointers_.at(this->currentZoomLevel_).size()/* - 1*/ << ")";
        _finishPoint.setY(this->vectorGraphicsPixmapItemPointers_.at(this->currentZoomLevel_).size()/* - 1*/);
        //        _startPoint.setY(_finishPoint.ry() - this->itemGroupSize_.rheight());
    }
    if (_finishPoint.rx() > this->vectorGraphicsPixmapItemPointers_.at(this->currentZoomLevel_).at(_finishPoint.ry()).size()/* - 1*/)
    {
        qDebug() << "OUT OF RANGE (X): " << _finishPoint.rx() << "(max: " << this->vectorGraphicsPixmapItemPointers_.at(this->currentZoomLevel_).at(_finishPoint.ry()).size()/* - 1 */<< ")";
        _finishPoint.setX(this->vectorGraphicsPixmapItemPointers_.at(this->currentZoomLevel_).at(_finishPoint.ry()).size());
        //        _startPoint.setX(_finishPoint.rx() - this->itemGroupSize_.rwidth());
    }
}

void GraphicsItemGroup::AddTilesToPixmapItemGroup(/*const */QPoint _startPoint, /*const */QPoint _finishPoint)  //  WHY ?!?!
{
    QString tilePath;
    QPixmap pixmapForGroup;
    QPoint localeStartPoint(_startPoint);
    QPoint localeFinishPoint(_finishPoint);
    this->CheckExtremeTileIndexes(localeStartPoint, localeFinishPoint);
    for (int h = localeStartPoint.ry(); h < localeFinishPoint.ry(); h++)
        for (int w = localeStartPoint.rx(); w < localeFinishPoint.rx(); w++)
        {
            tilePath = this->vectorTilePaths_.operator [](this->currentZoomLevel_).operator[](h).operator[](w);
            this->CreateTile(tilePath, pixmapForGroup);
            this->vectorGraphicsPixmapItemPointers_.operator[](this->currentZoomLevel_).operator[](h).operator[](w) = new QGraphicsPixmapItem;
            this->vectorGraphicsPixmapItemPointers_.operator[](this->currentZoomLevel_).operator[](h).operator[](w)->setPixmap(pixmapForGroup);
            this->vectorGraphicsPixmapItemPointers_.operator[](this->currentZoomLevel_).operator[](h).operator[](w)->setOffset(this->pos().rx() + w * this->TILESIDE_,
                                                                                                                               this->pos().ry() + h * this->TILESIDE_); //setPos suit too
            this->addToGroup(this->vectorGraphicsPixmapItemPointers_.operator[](this->currentZoomLevel_).operator[](h).operator[](w));
        }
}

void GraphicsItemGroup::RemoveTilesFromPixmapItemGroup(QPoint _startPoint, QPoint _finishPoint)
{
    QPoint localeStartPoint(_startPoint);
    QPoint localeFinishPoint(_finishPoint);
    this->CheckExtremeTileIndexes(localeStartPoint, localeFinishPoint);
    for (int h = localeStartPoint.ry(); h < localeFinishPoint.ry(); h++)
        for (int w = localeStartPoint.rx(); w < localeFinishPoint.rx(); w++)
        {
            this->removeFromGroup(this->vectorGraphicsPixmapItemPointers_.operator[](this->currentZoomLevel_).operator[](h).operator[](w));
                    delete this->vectorGraphicsPixmapItemPointers_.operator[](this->currentZoomLevel_).operator[](h).operator[](w);
        }
}

void GraphicsItemGroup::CreateTile(const QString& _tilePath, QPixmap& _pixmap)
{
    if (QFile::exists(_tilePath))
    {
        _pixmap.load(_tilePath);
        if (_pixmap.size() != QSize(this->TILESIDE_, this->TILESIDE_))
        {
            _pixmap = _pixmap.scaled(this->TILESIDE_, this->TILESIDE_);
            //            qDebug() << "_pixmap.size() != QSize(this->TILESIDE_, this->TILESIDE_)____СКЕЙЛЮ";
        }
    } else {
        //        qDebug() << "_pixmap " << _tilePath << " not exist";
        if (_pixmap.size() != QSize(this->TILESIDE_, this->TILESIDE_))
            _pixmap = _pixmap.scaled(this->TILESIDE_, this->TILESIDE_);
        _pixmap.fill(Qt::black);
    }
}

void GraphicsItemGroup::ItemGroupMovingHandler()
{
    this->offsetInPix_.first += this->arrivalMovingPoint_.rx() - this->departureMovingPoint_.rx(); // ПОЛОЖИТЕЛЬНЫЙ КОГДА СМЕЩЕНИЕ ВПРАВО => ПОДГРУЖАТЬ СЛЕВА
    this->offsetInPix_.second += this->arrivalMovingPoint_.ry() - this->departureMovingPoint_.ry(); // ПОЛОЖИТЕЛЬНЫЙ КОГДА СМЕЩЕНИЕ ВНИЗ => ПОДГРУЖАТЬ СВЕРХУ

    if (std::abs(this->offsetInPix_.first) >= this->TILESIDE_)
    {
        int offsetInTileX = this->offsetInPix_.first / this->TILESIDE_;
        this->offsetInPix_.first = this->offsetInPix_.first % this->TILESIDE_;
        // changeWidthItemGroup    в которой проверяем чтоб лишнего не хапануть/удалить
        this->ChangeWidthItemGroup(offsetInTileX);
//        if (this->currentZoomLevel_ == 0) emit this->signalForObtainMarkersCoordinated(this->startPoint_, this->finishPoint_);
        qDebug() << "ChildItems(): " << this->childItems().size();
        qDebug() << this->startPoint_ << this->finishPoint_;
    }
    if (std::abs(this->offsetInPix_.second) >= this->TILESIDE_)
    {
        int offsetInTileY = this->offsetInPix_.second / this->TILESIDE_;
        this->offsetInPix_.second = this->offsetInPix_.second % this->TILESIDE_;
        // changeHeightItemGroup    в которой проверяем чтоб лишнего не хапануть/удалить
        this->ChangeHeightItemGroup(offsetInTileY);
//        if (this->currentZoomLevel_ == 0) emit this->signalForObtainMarkersCoordinated(this->startPoint_, this->finishPoint_);
        qDebug() << "ChildItems(): " << this->childItems().size();
        qDebug() << this->startPoint_ << this->finishPoint_;
    }
}

void GraphicsItemGroup::ChangeWidthItemGroup(int _offsetInTileX)
{
    int addWstart, addWfinish;
    int removeWstart, removeWfinish;
    if (_offsetInTileX > 0) // подгружать слева
    {
        if (this->startPoint_.rx() - _offsetInTileX < 0)
        {
            while (this->startPoint_.rx() - _offsetInTileX < 0)
                _offsetInTileX--;
        }
        // когда равно 0 - поставить ограничитель   !!!!!
        addWstart = this->startPoint_.rx() - _offsetInTileX;
        addWfinish = this->startPoint_.rx();
        removeWstart = this->finishPoint_.rx() - _offsetInTileX;
        removeWfinish = this->finishPoint_.rx();

        this->startPoint_.rx() -= _offsetInTileX;
        this->finishPoint_.rx() -= _offsetInTileX;

        this->AddTilesToPixmapItemGroup(QPoint(addWstart, this->startPoint_.ry()), QPoint(addWfinish, this->finishPoint_.ry()));
        this->RemoveTilesFromPixmapItemGroup(QPoint(removeWstart, this->startPoint_.ry()), QPoint(removeWfinish, this->finishPoint_.ry()));
        qDebug() << this->startPoint_.rx();
        qDebug() << this->finishPoint_.rx();
        qDebug() << "________________________________";
    }
    else if (_offsetInTileX < 0) // подгружать справа
    {
        if (this->finishPoint_.rx() - _offsetInTileX > this->vectorGraphicsPixmapItemPointers_.at(this->currentZoomLevel_).at(0).size())
        {
            while (this->finishPoint_.rx() - _offsetInTileX > this->vectorGraphicsPixmapItemPointers_.at(this->currentZoomLevel_).at(0).size())
                _offsetInTileX++;
        }
        // когда равно 0 - поставить ограничитель   !!!!!
        addWstart = this->finishPoint_.rx();
        addWfinish = this->finishPoint_.rx() - _offsetInTileX;
        removeWstart = this->startPoint_.rx();
        removeWfinish = this->startPoint_.rx() - _offsetInTileX;

        this->startPoint_.rx() -= _offsetInTileX;
        this->finishPoint_.rx() -= _offsetInTileX;

        this->AddTilesToPixmapItemGroup(QPoint(addWstart, this->startPoint_.ry()), QPoint(addWfinish, this->finishPoint_.ry()));
        this->RemoveTilesFromPixmapItemGroup(QPoint(removeWstart, this->startPoint_.ry()), QPoint(removeWfinish, this->finishPoint_.ry()));
        qDebug() << this->startPoint_.rx();
        qDebug() << this->finishPoint_.rx();
        qDebug() << "________________________________";
    }
}

void GraphicsItemGroup::ChangeHeightItemGroup(int _offsetInTileY)
{
    int addHstart, addHfinish;
    int removeHstart, removeHfinish;
    if (_offsetInTileY > 0) // подгружать сверху
    {
        if (this->startPoint_.ry() - _offsetInTileY < 0)
        {
            while (this->startPoint_.ry() - _offsetInTileY < 0)
                _offsetInTileY--;
        }
        // когда равно 0 - поставить ограничитель   !!!!!
        addHstart = this->startPoint_.ry() - _offsetInTileY;
        addHfinish = this->startPoint_.ry();
        removeHstart = this->finishPoint_.ry() - _offsetInTileY;
        removeHfinish = this->finishPoint_.ry();

        this->startPoint_.ry() -= _offsetInTileY;
        this->finishPoint_.ry() -= _offsetInTileY;

        this->AddTilesToPixmapItemGroup(QPoint(this->startPoint_.rx(), addHstart), QPoint(this->finishPoint_.rx(), addHfinish));
        this->RemoveTilesFromPixmapItemGroup(QPoint(this->startPoint_.rx(), removeHstart), QPoint(this->finishPoint_.rx(), removeHfinish));
        qDebug() << "________________________________";
        qDebug() << this->startPoint_.ry();
        qDebug() << this->finishPoint_.ry();
    }
    else if (_offsetInTileY < 0) //подгружать снизу
    {
        if (this->finishPoint_.ry() - _offsetInTileY > this->vectorGraphicsPixmapItemPointers_.at(this->currentZoomLevel_).size())
        {
            while (this->finishPoint_.ry() - _offsetInTileY > this->vectorGraphicsPixmapItemPointers_.at(this->currentZoomLevel_).size())
                _offsetInTileY++;
        }
        // когда равно 0 - поставить ограничитель   !!!!! lol
        addHstart = this->finishPoint_.ry();
        addHfinish = this->finishPoint_.ry() - _offsetInTileY;
        removeHstart = this->startPoint_.ry();
        removeHfinish = this->startPoint_.ry() - _offsetInTileY;

        this->startPoint_.ry() -= _offsetInTileY;
        this->finishPoint_.ry() -= _offsetInTileY;

        this->AddTilesToPixmapItemGroup(QPoint(this->startPoint_.rx(), addHstart), QPoint(this->finishPoint_.rx(), addHfinish));
        this->RemoveTilesFromPixmapItemGroup(QPoint(this->startPoint_.rx(), removeHstart), QPoint(this->finishPoint_.rx(), removeHfinish));
        qDebug() << "________________________________";
        qDebug() << this->startPoint_.ry();
        qDebug() << this->finishPoint_.ry();
    }
}

QPoint GraphicsItemGroup::EvalNewTilesWithChangeZoom(int _newZoomLevel, QPoint _selectedPixelInGroup)
{
    int selectedTileWidthInGroup = _selectedPixelInGroup.rx() / this->TILESIDE_;  // 6)
    int selectedTileHeightInGroup = _selectedPixelInGroup.ry() / this->TILESIDE_;
    //    qDebug() << "selectedTileWidthOffset: " << selectedTileWidthInGroup;
    //    qDebug() << "selectedTileHeightOffset: " << selectedTileHeightInGroup;
    int selectedTileWidthInGeneral = this->startPoint_.rx() + selectedTileWidthInGroup;
    int selectedTileHeightInGeneral = this->startPoint_.ry() + selectedTileHeightInGroup;
    //    qDebug() << "selectedTileWidthInGeneral: " << selectedTileWidthInGeneral;
    //    qDebug() << "selectedTileHeightInGeneral: " << selectedTileHeightInGeneral;

    QPair<int, int> selectedPixelInWhole(this->startPoint_.rx() * this->TILESIDE_ + _selectedPixelInGroup.rx(), this->startPoint_.ry() * this->TILESIDE_ + _selectedPixelInGroup.ry());// 3)
    //    double coefficient = double(this->vectorGraphicsPixmapItemPointers_.at(_newZoomLevel).size()) / double(this->vectorGraphicsPixmapItemPointers_.at(this->currentZoomLevel_).size());// 4)
    double coefficient = double(this->vectorHeightInPixAllLevels_.at(_newZoomLevel)) / double(this->vectorHeightInPixAllLevels_.at(this->currentZoomLevel_));  // 4)
    QPair<int, int> selectedPixelInNewZoomLevel(selectedPixelInWhole.first * coefficient, selectedPixelInWhole.second * coefficient);   // 5)
    QPair<int, int> selectedTileInNewZoomLevel(selectedPixelInNewZoomLevel.first / this->TILESIDE_, selectedPixelInNewZoomLevel.second / this->TILESIDE_);
    //    qDebug() << selectedTileInNewZoomLevel;

    QPoint newStartPoint(selectedTileInNewZoomLevel.first - selectedTileWidthInGroup, selectedTileInNewZoomLevel.second - selectedTileHeightInGroup);  // 7
    QPoint newFinishPoint(newStartPoint.rx() + this->itemGroupSize_.rwidth(), newStartPoint.ry() + this->itemGroupSize_.rheight());
    //    this->CheckExtremeTileIndexes(newStartPoint, newFinishPoint);

    //   qDebug
    {    qDebug() << "__________________________________________________";
        qDebug() << "this->startPoint_: " << this->startPoint_;
        qDebug() << "this->finishPoint_: " << this->finishPoint_;
        qDebug()<< "newStartPoint: " << newStartPoint;
        qDebug() << "newFinishPoint: " << newFinishPoint;
        qDebug() << "__________________________________________________";}

    QPoint selectedPixelInNewItemGroup(selectedPixelInNewZoomLevel.first - newStartPoint.rx() * this->TILESIDE_, selectedPixelInNewZoomLevel.second - newStartPoint.ry() * this->TILESIDE_);

    this->RemoveTilesFromPixmapItemGroup(this->startPoint_, this->finishPoint_);
    this->currentZoomLevel_ = _newZoomLevel;
    this->AddTilesToPixmapItemGroup(newStartPoint, newFinishPoint);

    this->startPoint_ = newStartPoint;
    this->finishPoint_ = newFinishPoint;

    return selectedPixelInNewItemGroup;
    //    return QPoint(selectedPixelInNewZoomLevel.first, selectedPixelInNewZoomLevel.second);     ТОЖЕ НА ПОДУМАТЬ
}

void GraphicsItemGroup::SetStoragePath(const QString& _storagePath)
{
    this->storagePath_ = _storagePath;
}

void GraphicsItemGroup::SetItemGroupSize(const QSize& _itemGroupSize)  //  мб изменять при MW::ResizeEvent    NOPE !!!
{
    itemGroupSize_ = _itemGroupSize;
}

const int& GraphicsItemGroup::GetTileSide() const
{    
    return this->TILESIDE_;
}

const QSize& GraphicsItemGroup::GetItemGroupSize() const
{
    return this->itemGroupSize_;
}

const bool& GraphicsItemGroup::isIncrease() const
{
    return this->increase;
}

const int& GraphicsItemGroup::GetCurrentZoomLevel() const
{
    return this->currentZoomLevel_;
}

const QPair<QPoint, QPoint> GraphicsItemGroup::GetStartFinishPoints() const
{
    QPair<QPoint, QPoint> pairForExport = QPair<QPoint, QPoint>(this->startPoint_, this->finishPoint_);
    return pairForExport;
}

QPixmap GraphicsItemGroup::GetPixmap()
{    
    int width = this->itemGroupSize_.width() * this->TILESIDE_;
    int height = this->itemGroupSize_.height() * this->TILESIDE_;
    QImage img(width, height, QImage::Format_ARGB32);

    for (int h = this->startPoint_.ry(), hCounter = 0; hCounter < this->itemGroupSize_.rheight(); h++, hCounter++)
        for (int w = this->startPoint_.rx(), wCounter = 0; wCounter < this->itemGroupSize_.rwidth(); w++, wCounter++)
        {
            QImage tempImg(this->vectorTilePaths_.at(this->currentZoomLevel_).at(h).at(w));

            for (int h = 0; h < this->TILESIDE_; h++)
                for (int w = 0; w < this->TILESIDE_; w++)
                {
                    img.setPixel(wCounter * this->TILESIDE_ + w, hCounter * this->TILESIDE_ + h, tempImg.pixel(w, h));
                }
        }
    return QPixmap::fromImage(img);
}

QImage GraphicsItemGroup::GetImageForTuner()
{
    int widthVisibleAreaInTile = this->itemGroupSize_.width() / 3;
    int heightVisibleAreaInTile = this->itemGroupSize_.height() / 3;

    int widthVisibleAreaInPix = (widthVisibleAreaInTile) * this->TILESIDE_;
    int heightVisibleAreaInPix = (heightVisibleAreaInTile) * this->TILESIDE_;

    int tempStartPointY = this->startPoint_.ry() + widthVisibleAreaInTile/* + 1*/;
    int tempStartPointX = this->startPoint_.rx() + widthVisibleAreaInTile/* + 1*/;
//    tempStartPointY += heightVisibleAreaInTile + 1;
//    tempStartPointX += widthVisibleAreaInTile + 1;

    QImage img(widthVisibleAreaInPix, heightVisibleAreaInPix, QImage::Format_ARGB32);

    for (int h = tempStartPointY, hCounter = 0; hCounter < heightVisibleAreaInTile /*- 1*/; h++, hCounter++)
        for (int w = tempStartPointX, wCounter = 0; wCounter < widthVisibleAreaInTile /*- 1*/; w++, wCounter++)
        {
            QImage tempImg(this->vectorTilePaths_.at(this->currentZoomLevel_).at(h).at(w));

            for (int h = 0; h < this->TILESIDE_; h++)
                for (int w = 0; w < this->TILESIDE_; w++)
                {
                    img.setPixel(wCounter * this->TILESIDE_ + w, hCounter * this->TILESIDE_ + h, tempImg.pixel(w, h));
                }
        }
    return img;
}

void GraphicsItemGroup::mousePressEvent(QGraphicsSceneMouseEvent* _event)
{
    QGraphicsItemGroup::mousePressEvent(_event);

    this->departureMovingPoint_.setX(this->pos().rx());
    this->departureMovingPoint_.setY(this->pos().ry());
}

void GraphicsItemGroup::mouseReleaseEvent(QGraphicsSceneMouseEvent* _event)
{
    QGraphicsItemGroup::mouseReleaseEvent(_event);

    this->arrivalMovingPoint_.setX(this->pos().rx());
    this->arrivalMovingPoint_.setY(this->pos().ry());
    this->ItemGroupMovingHandler();
//    emit this->signalForPreview(this->currentZoomLevel_, this->startPoint_ + QPoint(this->itemGroupSize_.rwidth() / 3, this->itemGroupSize_.rheight() / 3),
//                                this->finishPoint_ - QPoint(this->itemGroupSize_.rwidth() / 3, this->itemGroupSize_.rheight() / 3));
    emit this->signalForPreview(this->currentZoomLevel_, this->startPoint_, this->finishPoint_);
}

void GraphicsItemGroup::mouseMoveEvent(QGraphicsSceneMouseEvent* _event)
{
    QGraphicsItemGroup::mouseMoveEvent(_event);
    //    if (this->pos().rx() < this->leftLimit_)
    //        this->setPos(this->leftLimit_, this->pos().ry());
    // HERE CHECK THE BORDERS DUE TO NOT LEAVE SPACE AND SHOW WHITE BAND
}

void GraphicsItemGroup::wheelEvent(QGraphicsSceneWheelEvent* _event)    // НАФИГ НЕ НУЖЕН
{
    qDebug() << "!!!!!_event->pos() - QPointF(this->boundingRect().x(), this->boundingRect().y()): "
             << _event->pos() - QPointF(this->boundingRect().x(), this->boundingRect().y());
    emit this->signalGetSelectedPixelInItemGroup(_event->pos().toPoint());
}

void GraphicsItemGroup::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* _event)
{    
    qDebug() << "EVENT.POS(): " << _event->pos();
    qDebug() << "!!!!!_event->pos() - QPointF(this->boundingRect().x(), this->boundingRect().y()): "
             << _event->pos() - QPointF(this->boundingRect().x(), this->boundingRect().y());

    QPoint selectedPixelInOldGroup(QPointF(_event->pos() - QPointF(this->boundingRect().x(), this->boundingRect().y())).toPoint());  // 1)
    if (_event->button() == Qt::LeftButton && this->currentZoomLevel_ > 0)
    {
        int newZoomLevel = this->currentZoomLevel_ - 1;
        emit this->signalGetCoordinateToView(_event->pos().toPoint());    // 2)
        QPoint pixInNewItemGroup(this->EvalNewTilesWithChangeZoom(newZoomLevel, selectedPixelInOldGroup));
        //        QPointF selectedPointInScene(this->mapToScene(QPointF(selectedPixelInOldGroup)));
        this->increase = true;
        emit this->signalEnsureVisible(selectedPixelInOldGroup, pixInNewItemGroup);
        emit this->signalChangeZoomLevel(newZoomLevel);
        emit this->signalForPreview(this->currentZoomLevel_, this->startPoint_, this->finishPoint_);
    } else if (_event->button() == Qt::RightButton && this->currentZoomLevel_ < this->vectorGraphicsPixmapItemPointers_.size() - 1)
    {
        int newZoomLevel = this->currentZoomLevel_ + 1;
        emit this->signalGetCoordinateToView(_event->pos().toPoint());    // 2)
        QPoint pixInNewItemGroup(this->EvalNewTilesWithChangeZoom(newZoomLevel, selectedPixelInOldGroup));
        //        QPointF selectedPointInScene(this->mapToScene(QPointF(selectedPixelInOldGroup)));
        this->increase = false;
        emit this->signalEnsureVisible(selectedPixelInOldGroup, pixInNewItemGroup);
        emit this->signalChangeZoomLevel(newZoomLevel);
        emit this->signalForPreview(this->currentZoomLevel_, this->startPoint_, this->finishPoint_);
    }
}

QVector<QVector<QVector<QString>>> GraphicsItemGroup::GetVectorTilePaths() const
{
    return vectorTilePaths_;
}
