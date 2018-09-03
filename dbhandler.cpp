#include "dbhandler.h"

DBHandler::DBHandler()
{

}

bool DBHandler::CreateConnection()
{
    this->dataBase_ = QSqlDatabase::addDatabase("QSQLITE");    
    this->dataBase_.setDatabaseName(QString("nucleus").append(QDateTime::currentDateTime().toString("dd.MM.yyyy_hh.mm")));
//    this->dataBase_.setUserName("Freddy");
//    this->dataBase_.setHostName("ASM");
//    this->dataBase_.setPassword("password");
    if (!this->dataBase_.open())
    {
        qDebug() << "Cannot open database:" << this->dataBase_.lastError();
        qDebug() << this->dataBase_.lastError().text();
        return false;
    }
    this->query_ = new QSqlQuery(this->dataBase_);
    this->record_ = new QSqlRecord;
    return true;
}

bool DBHandler::CreateConnection(const QString _pathToDB)
{
    this->dataBase_ = QSqlDatabase::addDatabase("QSQLITE");
    this->dataBase_.setDatabaseName(_pathToDB);

    if (!this->dataBase_.open())
    {
        qDebug() << "Cannot open database:" << this->dataBase_.lastError();
        qDebug() << this->dataBase_.lastError().text();
        //        return false;
    }
    this->query_ = new QSqlQuery(this->dataBase_);
    this->record_ = new QSqlRecord;
}

void DBHandler::CreateTables()
{
    this->strForQuery_ = "CREATE TABLE TILE (id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE NOT NULL, "
                         "x INTEGER NOT NULL, "
                         "y INTEGER NOT NULL);";
    if (!this->query_->exec(this->strForQuery_))
    {
        qDebug() << "Unable to create a table TILE";
    }
    this->strForQuery_ = "CREATE TABLE OBJECT (id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE NOT NULL, "
                         "tile_id INTEGER REFERENCES TILE (id), "
                         "x INTEGER NOT NULL, "
                         "y INTEGER NOT NULL);";
    if (!this->query_->exec(this->strForQuery_))
    {
        qDebug() << "Unable to create a table OBJECT";
    }
}

void DBHandler::InsertIntoTileTable(const int& _x, const int& _y)
{
    QString x(QString::number(_x));
    QString y(QString::number(_y));
    //"INSERT INTO TILE (x, y) VALUES(%1, %2);"
    QString tmpForTileQuery = this->insertIntoTile_.arg(x).arg(y);
    if (!this->query_->exec(tmpForTileQuery))
        qDebug() << "Unable to make insert operation";
}

void DBHandler::InsertIntoObjectTable(const int& _tileId, const int& _coordX, const int& _coordY)
{
    QString tileId(QString::number(_tileId));
    QString coordinateX(QString::number(_coordX));
    QString coordinateY(QString::number(_coordY));
    //"INSERT INTO OBJECT (tile_id, x, y) VALUES(%1, %2, %3);"
    QString tmpForObjectQuery = this->insertIntoObject_.arg(tileId).arg(coordinateX).arg(coordinateY);
    if (!this->query_->exec(tmpForObjectQuery))
        qDebug() << "Unable to make insert operation";
}

void DBHandler::getObjectsCoordinates(const QPoint& _startPoint, const QPoint& _finishPoint)
{
    this->vObjectsCoordinates_.clear();
    //  (TILE.x BETWEEN          %1                 AND                %2)     AND        (TILE.y BETWEEN     %3               AND                    %4)
    QString tmpSelectQuery = this->getObjectsWithTilesPositions_.arg
            (QString::number(_startPoint.x())).arg(QString::number(_finishPoint.x())).arg(QString::number(_startPoint.y())).arg(QString::number(_finishPoint.y()));
    if (!this->query_->exec(tmpSelectQuery))
    {
        qDebug() << "Unable to execute query — exiting";
    } else
    {
        *this->record_ = this->query_->record();
        int tileX = 0, tileY = 0, objectX = 0, objectY = 0;
        while (this->query_->next())
        {
            tileX = this->query_->value(this->record_->indexOf("tileX")).toInt() - 1;
            tileY = this->query_->value(this->record_->indexOf("tileY")).toInt() - 1;
            objectX = this->query_->value(this->record_->indexOf("objectX")).toInt();
            objectY = this->query_->value(this->record_->indexOf("objectY")).toInt();

            PositionInDB pfdb(tileX, tileY, objectX, objectY);
            this->vObjectsCoordinates_.push_back(pfdb);
            qDebug() << "result: " << tileX << ",  " << tileY << ",  " << objectX << ",  " << objectY;
        }
    }
}

const QVector<PositionInDB>& DBHandler::getObjectsCoordinatesVector()
{
    return this->vObjectsCoordinates_;
}

void DBHandler::FillingTileTable(const int& _w, const int& _h)
{
    for (int y = 1; y <= _h; y++)
        for (int x = 1; x <= _w; x++)
        {
            this->InsertIntoTileTable(x, y);
        }
}
