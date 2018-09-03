#ifndef GRAPHICSITEMGROUP_H
#define GRAPHICSITEMGROUP_H

#include "imagedescription.h"

#include <QObject>
#include <QGraphicsItemGroup>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>

class GraphicsItemGroup : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT
public:
    explicit GraphicsItemGroup(QObject* _parent = 0);
    void InitializeTilePathsVector(const QVector<ZoomLevelDescription>& _vectorZoomLevelDescription);
    void InitializedVectorHeightInPixAllLevels(const QVector<ZoomLevelDescription>& _vectorImageDescription);
    void FirstEdditionToItemGroup();
    void EvalLimits();
    void CheckExtremeTileIndexes(QPoint& _startPoint, QPoint& _finishPoint);
    void AddTilesToPixmapItemGroup(/*const */QPoint _startPoint, /*const */QPoint _finishPoint);
    void RemoveTilesFromPixmapItemGroup(QPoint _startPoint, QPoint _finishPoint);
    void CreateTile(const QString& _tilePath, QPixmap& _pixmap);
    void ItemGroupMovingHandler();
    void ChangeWidthItemGroup(int _offsetInTileX);
    void ChangeHeightItemGroup(int _offsetInTileY);
    QPoint EvalNewTilesWithChangeZoom(int _newZoomLevel, QPoint _selectedPixelInGroup);
    void SetStoragePath(const QString& _storagePath);
    void SetItemGroupSize(const QSize& _itemGroupSize);
    const int& GetTileSide() const;
    const QSize& GetItemGroupSize()const;
    const bool& isIncrease() const;
    const int& GetCurrentZoomLevel() const;
    const QPair<QPoint, QPoint> GetStartFinishPoints() const;

    QPixmap GetPixmap();
    QImage GetImageForTuner();

    QVector<QVector<QVector<QString>>> GetVectorTilePaths() const;

protected:
    // QGraphicsItem interface
    void mousePressEvent(QGraphicsSceneMouseEvent* _event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* _event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* _event);
    void wheelEvent(QGraphicsSceneWheelEvent* _event);    
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* _event);

signals:
    void signalGetCoordinateToView(QPoint _point);
    void signalEnsureVisible(QPoint _oldPoint, QPoint _newPoint);
    void signalGetSelectedPixelInItemGroup(QPoint _point);
    void signalChangeZoomLevel(int _level);
    void signalForPreview(int _level, QPoint _startPoint, QPoint _finishPoint);
    void signalForObtainMarkersCoordinated(QPoint _startPoint, QPoint _finishPoint);

public slots:

private:
    const int TILESIDE_ = 256;
    QVector<QVector<QVector<QString>>> vectorTilePaths_;
    QVector<QVector<QVector<QGraphicsPixmapItem*>>> vectorGraphicsPixmapItemPointers_;
    QString storagePath_;
    QPoint upperLeftVisibleTileNumber_;
    int currentZoomLevel_;
    QSize itemGroupSize_;
    QPoint startPoint_, finishPoint_;
    QPoint departureMovingPoint_, arrivalMovingPoint_;
    QPair<int, int> offsetInPix_; // first - X, second - Y

    QVector<int> vectorHeightInPixAllLevels_;
    bool increase;

    int upLimit_, rightLimit_, downLimit_, leftLimit_;  // изменять при каждом ZOOM`е


};

#endif // GRAPHICSITEMGROUP_H
