/***************************************************************************
 *   Copyright (C) 2012 by Heiko Koehn (KoehnHeiko@googlemail.com)         *
 *                                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#ifndef TROOM_H
#define TROOM_H

#include <QVector3D>
#include <QMap>
#include <QColor>
//#include "TRoomDB.h"

#define DIR_NORTH             1
#define DIR_NORTHEAST         2
#define DIR_NORTHWEST         3
#define DIR_EAST              4
#define DIR_WEST              5
#define DIR_SOUTH             6
#define DIR_SOUTHEAST         7
#define DIR_SOUTHWEST         8
#define DIR_UP                9
#define DIR_DOWN             10
#define DIR_IN               11
#define DIR_OUT              12
#define DIR_OTHER            13
// Some extras for the 2D Mapper, to keep track of where the other end of an
// in-area exit is so the drawing of exits can be modified based on relative z
// values (and x or y - use the DIR_ALTERNATE_FORM for negative relative values
// from this room to the other "exit" room, usages will vary between the
// different exit directions)
#define DIR_CIRCULAR_MODIFIER    16  /*Exit returns directly to the SAME room*/
#define DIR_ABOVE_MODIFIER       32  /*Exit room is drawn on a higher Z value level*/
#define DIR_BELOW_MODIFIER       48  /*Exit room is drawn on a lower Z value level*/
#define DIR_MASK                 15  /*Mask to determine base direction without modifiers*/
#define DIR_NO_EXIT_LINE_MASK    48  /*Mask to determine if we do NOT have to draw a line to exit room*/
#define DIR_ALT_MODIFIER         64  /*Exit room should be drawn on "other" side to normal*/
#define DIR_ADJACENT_MODIFIER   128  /*Exit room is immediately adjacent, so can't extend fixed drawing beyond half inter-room space*/
#define DIR_ACTUAL_STUB         256  /*Needed to encode/handle a real exit stub*/

class XMLimport;
class XMLexport;
class TRoomDB;

class TRoom
{
public:
    TRoom( TRoomDB* pRDB );
    ~TRoom();
    void setId( int );
    bool setExit( int to, int direction );
    int getExit( int direction );
    bool hasExit( int direction );
    void setWeight( int );
    void setExitLock( int, bool );
    void setSpecialExitLock( int to, QString cmd, bool doLock );
    bool setSpecialExitLock( QString cmd, bool doLock );
    bool hasExitLock( int to );
    bool hasSpecialExitLock( int, QString );
    void removeAllSpecialExitsToRoom(int _id );
    void setSpecialExit( int to, QString cmd );
    void clearSpecialExits() { other.clear(); }
    const QMultiMap<int, QString> & getOtherMap() const { return other; }
    const QMap<QString, int> & getExitWeights() const { return exitWeights; }
    void setExitWeight( QString cmd, int w );
    bool hasExitWeight( QString cmd );
    void setDoor( QString cmd, int doorStatus );//0=no door, 1=open door, 2=closed, 3=locked
    void setDoor( int direction, int doorStatus ); // Non-special exits
    int getDoor( QString cmd );
    int getDoor( int direction );
    bool hasExitStub( int direction );
    void setExitStub( int direction, bool status );
    void calcRoomDimensions();
    void setAreaID( int _areaID );
    int getExitWeight( QString cmd );

    int getWeight() { return weight; }
    int getNorth() { return north; }
    void setNorth( int id ) { north=id; }
    int getNorthwest() { return northwest; }
    void setNorthwest( int id ) { northwest=id; }
    int getNortheast() { return northeast; }
    void setNortheast( int id ) { northeast=id; }
    int getSouth() { return south; }
    void setSouth( int id ) { south=id; }
    int getSouthwest() { return southwest; }
    void setSouthwest( int id ) { southwest=id; }
    int getSoutheast() { return southeast; }
    void setSoutheast( int id ) { southeast=id; }
    int getWest() { return west; }
    void setWest( int id ) { west=id; }
    int getEast() { return east; }
    void setEast( int id ) { east=id; }
    int getUp() { return up; }
    void setUp( int id ) { up=id; }
    int getDown() { return down; }
    void setDown( int id ) { down=id; }
    int getIn() { return in; }
    void setIn( int id ) { in=id; }
    int getOut() { return out; }
    void setOut( int id ) { out=id; }
    int getId() { return id; }
    int getAreaID() { return area; }
    void auditExits();
    /*bool*/ void restore( QDataStream & ifs, int roomID, int version );
    QString getCustomExitName( quint16 direction );

    int x;
    int y;
    int z;
    int environment;

    bool isLocked;
    qreal min_x;
    qreal min_y;
    qreal max_x;
    qreal max_y;
    qint8 c;
    QString name;
    QVector3D v;
    QList<int> exitStubs; //contains a list of: exittype (according to defined values above)
    QMap<QString, QString> userData;
    QList<int> exitLocks;
    QMap<QString, QList<QPointF> > customLines;
    QMap<QString, QList<int> > customLinesColor;
    QMap<QString, QString> customLinesStyle;
    QMap<QString, bool> customLinesArrow;
    bool highlight;
    QColor highlightColor;
    QColor highlightColor2;
    float highlightRadius;
    bool rendered;
    QMap<QString, int> doors; //0=no door 1=open 2=closed 3=locked


private:
    int id;
    int area;
    int weight;
    QMap<QString, int> exitWeights;
    int north;
    int northeast;
    int east;
    int southeast;
    int south;
    int southwest;
    int west;
    int northwest;
    int up;
    int down;
    int in;
    int out;

    QMultiMap<int, QString> other; // es knnen mehrere exits zum gleichen raum verlaufen
                                   //verbotene exits werden mit 0 geprefixed, offene mit 1

    TRoomDB * mpRoomDB;
    friend class XMLimport;
    friend class XMLexport;
};

#endif // TROOM_H

