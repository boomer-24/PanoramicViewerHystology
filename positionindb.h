#ifndef POSITIONINDB_H
#define POSITIONINDB_H


class PositionInDB
{
public:
    PositionInDB();
    PositionInDB(const int& _tX, const int& _tY, const int& _oX, const int& _oY);
    const int& getTileX();
    const int& getTileY();
    const int& getObjectX();
    const int& getObjectY();

private:
    int tileX_, tileY_, objectX_, objectY_;
};

#endif // POSITIONINDB_H
