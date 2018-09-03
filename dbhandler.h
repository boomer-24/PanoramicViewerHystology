#ifndef DBHANDLER_H
#define DBHANDLER_H

#include "positionindb.h"
#include <QtSql>

class DBHandler
{
public:
    DBHandler();
    bool CreateConnection();
    bool CreateConnection(const QString _pathToDB);
    void CreateTables();
    void InsertIntoTileTable(const int& _x, const int& _y);
    void InsertIntoObjectTable(const int& _tileId, const int& _coordX, const int& _coordY);
    void getObjectsCoordinates(const QPoint& _startPoint, const QPoint& _finishPoint);
    const QVector<PositionInDB> &getObjectsCoordinatesVector();
    void FillingTileTable(const int& _w, const int& _h);

private:
    QSqlDatabase dataBase_;
    QSqlQuery *query_;
    QSqlRecord *record_;
    QString strForQuery_;
    QString insertIntoTile_ = "INSERT INTO TILE (x, y) VALUES(%1, %2);";
    QString insertIntoObject_ = "INSERT INTO OBJECT (tile_id, x, y) VALUES(%1, %2, %3);";
                                //tile_id кажется можно опускать. надо перечитать
    QString getObjectsWithTilesPositions_ = "SELECT TILE.x AS tileX, TILE.y AS tileY, OBJECT.x AS objectX, OBJECT.y AS objectY FROM OBJECT INNER JOIN TILE "
                                            "ON OBJECT.tile_id IN (SELECT TILE.id FROM TILE WHERE (TILE.x BETWEEN %1 AND %2) AND (TILE.y BETWEEN %3 AND %4)) WHERE OBJECT.tile_id = TILE.id";

    QVector<PositionInDB> vObjectsCoordinates_;  //  ДЛЯ СЧИТЫВАНИЯ ВРОДЕ
};

#endif // DBHANDLER_H
