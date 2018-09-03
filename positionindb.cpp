#include "positionindb.h"

PositionInDB::PositionInDB()
{

}

PositionInDB::PositionInDB(const int& _tX, const int& _tY, const int& _oX, const int& _oY)
{
    this->tileX_ = _tX;
    this->tileY_ = _tY;
    this->objectX_ = _oX;
    this->objectY_ = _oY;
}

const int& PositionInDB::getTileX()
{
    return this->tileX_;
}

const int& PositionInDB::getTileY()
{
    return this->tileY_;
}

const int& PositionInDB::getObjectX()
{
    return this->objectX_;
}

const int& PositionInDB::getObjectY()
{
    return this->objectY_;
}
