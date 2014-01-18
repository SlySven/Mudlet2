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

#include <QVector3D>
#include "TRoom.h"
#include "TRoomDB.h"
#include <QDebug>

TRoom::TRoom(TRoomDB * pRDB )
: x( 0 )
, y( 0 )
, z( 0 )
, environment( -1 )
, isLocked( false )
, min_x( 0 )
, min_y( 0 )
, max_x( 0 )
, max_y( 0 )
, c( 0 )
, highlight( false )
, highlightColor( QColor( 255,150,0 ) )
, rendered( false )
, id( 0 )
, areaId( 0 )
, weight( 1 )
, north( -1 )
, northeast( -1 )
, east( -1 )
, southeast( -1 )
, south( -1 )
, southwest( -1 )
, west( -1 )
, northwest( -1 )
, up( -1 )
, down( -1 )
, in( -1 )
, out( -1 )
, mpRoomDB( pRDB )
{
}

TRoom::~TRoom()
{
    mpRoomDB->__removeRoom( id );
}

int TRoom::hasExitStub(int direction){
    if (exitStubs.contains(direction))
        return 1;
    else
        return 0;
}

void TRoom::setExitStub(int direction, int status){
    if (status)
        exitStubs.append(direction);
    else
        exitStubs.removeOne(direction);
}

int TRoom::getExitWeight( QString cmd )
{
    if( exitWeights.contains( cmd ) )
    {
        return exitWeights[cmd];
    }
    else
        return weight; // NOTE: if no exit weight has been set: exit weight = room weight
}

// NOTE: needed so dialogRoomExit code can tell if an exit weight has been set
// now that they are private!
bool TRoom::hasExitWeight(QString cmd)
{
    if( exitWeights.contains(cmd) )
    {
        if( exitWeights.value(cmd) > 0 )
            return true;
        else
        {
            exitWeights.remove(cmd); // Fixup - removes any bogus (zero or less) values
            return false;
        }
    }
    else
        return false;
}
void TRoom::setWeight( int w )
{
    if( w < 1 )
        w = 1;
    weight = w;
}

// Previous implimentation did not allow for REMOVAL of an exit weight (by
// setting it to zero)
void TRoom::setExitWeight(QString cmd, int w)
{
    if( w > 0 )
        exitWeights[cmd] = w;
    else if( exitWeights.contains( cmd ) )
        exitWeights.remove( cmd );
    if( mpRoomDB )
        mpRoomDB->mpMap->mMapGraphNeedsUpdate = true;
}

// Declared in header but was missing!
// Uses lower case initials for normal xy-plane exits: n,ne,e,se,s,sw,w,nw
// Use "up", "down", "in", "out" for the non-xy plane ones
// Will use (un 0/1 prefixed) command for specials
void TRoom::setDoor( QString cmd, int doorStatus)
{
    if( doorStatus > 0 && doorStatus <=3 )
        doors[cmd] = doorStatus;
    else if ( doors.contains( cmd ) && doorStatus == 0 )
        doors.remove( cmd );

    if( mpRoomDB )
        mpRoomDB->mpMap->mMapGraphNeedsUpdate = true;
}

int TRoom::getDoor( QString cmd )
{
    if( doors.contains( cmd ) )
        return doors.value( cmd );
    else
        return 0;
}

// This is not an in-line method - and may need additional code if used
// on a non-new room to update anything that used the old id value...
void TRoom::setId( int _id )
{
    id = _id;
}

void TRoom::setAreaId( int _areaId )
{
    areaId = _areaId;
    TArea * pA = mpRoomDB->getArea( areaId );
    if( !pA )
    {
        mpRoomDB->addArea( areaId );
        pA = mpRoomDB->getArea( areaId );
        if( !pA )
        {
            QString error = "TRoom::setArea(): No area created! requested area ID=%1. Note: area IDs must be > 0";
            mpRoomDB->mpMap->logError(error);
            return;
        }
    }

    pA->addRoom( id );
    pA->fast_ausgaengeBestimmen(id);
    pA->fast_calcSpan(id);
}

bool TRoom::hasExit( int _id )
{
    if( north == _id )
        return true;
    else if( south == _id )
        return true;
    else if( northwest == _id )
        return true;
    else if( northeast == _id )
        return true;
    else if( southwest == _id )
        return true;
    else if( southeast == _id )
        return true;
    else if( east == _id )
        return true;
    else if( west == _id )
        return true;
    else if( up == _id )
        return true;
    else if( down == _id )
        return true;
    else if( out == _id )
        return true;
    else if( in == _id )
        return true;
    else
        return false;
}

void TRoom::setExitLock(int exit, bool state )
{
    if( ! state )
    {
        exitLocks.removeAll( exit );
        return;
    }
    switch( exit )
    {
        case DIR_NORTH: exitLocks.push_back(DIR_NORTH); break;
        case DIR_NORTHEAST: exitLocks.push_back(DIR_NORTHEAST); break;
        case DIR_NORTHWEST: exitLocks.push_back(DIR_NORTHWEST); break;
        case DIR_SOUTHEAST: exitLocks.push_back(DIR_SOUTHEAST); break;
        case DIR_SOUTHWEST: exitLocks.push_back(DIR_SOUTHWEST); break;
        case DIR_SOUTH: exitLocks.push_back(DIR_SOUTH); break;
        case DIR_EAST: exitLocks.push_back(DIR_EAST); break;
        case DIR_WEST: exitLocks.push_back(DIR_WEST); break;
        case DIR_UP: exitLocks.push_back(DIR_UP); break;
        case DIR_DOWN: exitLocks.push_back(DIR_DOWN); break;
        case DIR_IN: exitLocks.push_back(DIR_IN); break;
        case DIR_OUT: exitLocks.push_back(DIR_OUT); break;
    }
}

void TRoom::setSpecialExitLock(int to, QString cmd, bool doLock)
{
    QMapIterator<int, QString> it( other );
    while(it.hasNext() )
    {
        it.next();
        if( it.key() != to ) continue;
        if( it.value().size() < 1 ) continue;
        if( it.value().mid(1) != cmd )
        {
            if( it.value() != cmd )
            {
                continue;
            }
        }
        if( doLock )
        {
            QString _cmd = it.value();
            _cmd.replace( 0, 1, '1' );
            other.replace( to, _cmd );
        }
        else
        {
            QString _cmd = it.value();
            _cmd.replace( 0, 1, '0');
            other.replace( to, _cmd );
        }
        return;
    }
}

bool TRoom::hasExitLock( int exit )
{
    return exitLocks.contains(exit);
}

// 0=offen 1=zu
bool TRoom::hasSpecialExitLock(int to, QString cmd)
{
    if( other.contains( to ) )
    {
        QMapIterator<int, QString> it( other );
        while(it.hasNext() )
        {
            it.next();
            if( it.key() != to ) continue;
            if( it.value().size() < 2 ) continue;
            return it.value().mid(0,1) == "1";
        }
        return false;
    }
    else
        return false;
}

void TRoom::addSpecialExit( int to, QString cmd )
{
    QString _cmd;
    // replace if this special exit exists, otherwise add
    QMapIterator<int, QString> it( other );
    while(it.hasNext() )
    {
        it.next();
        if( it.key() != to ) continue;
        if( it.value().size() > 0 )
        {
            QString _cmd;
            if( cmd.startsWith('0') || cmd.startsWith('1') )
            {
                _cmd = cmd;
            }
            else
            {
                _cmd.prepend("0");
                _cmd.append( cmd );
            }

            other.replace( to, _cmd );
            goto UPDATE_AREAS;
        }
    }
    // it doesnt exit -> add

    if( cmd.startsWith('0') || cmd.startsWith('1') )
    {
        _cmd = cmd;
    }
    else
    {
        _cmd.prepend("0");
        _cmd.append( cmd );
    }
    other.insertMulti( to, _cmd );

UPDATE_AREAS: TArea * pA = mpRoomDB->getArea( getArea() );
    if( pA )
    {
        pA->fast_ausgaengeBestimmen(getId());
    }

}





void TRoom::removeAllSpecialExitsToRoom( int _id )
{
    QList<int> keyList = other.keys();
    QList<QString> valList = other.values();
    for( int i=0; i<keyList.size(); i++ )
    {
        if( keyList[i] == _id )
        {
            // guaranteed to be in synch according to Qt docs
            other.remove(keyList[i], valList[i]);
        }
    }
}

void TRoom::calcRoomDimensions()
{
    if( customLines.size() < 1 ) return;
    min_x = 0.0;
    min_y = 0.0;
    max_x = 0.0;
    max_y = 0.0;
    bool needInit = true;

    QMapIterator<QString, QList<QPointF> > it(customLines);
    while( it.hasNext() )
    {
        it.next();
        const QList<QPointF> & _pL= it.value();
        if( _pL.size() < 1 ) continue;
        if( needInit )
        {
            needInit = false;
            min_x = _pL[0].x();
            max_x = min_x;
            min_y = _pL[0].y();
            max_y = min_y;
            qDebug()<<"\nNEXT ROOM: rID:"<<getId()<<" FIRST: custom lines span: x("<<min_x<<"/"<<max_x<<") y("<<min_y<<"/"<<max_y<<")";
        }
        for( int i=0; i<_pL.size(); i++ )
        {
            qreal _x = _pL[i].x();
            qreal _y = _pL[i].y();
            if( _x < min_x )
                min_x = _x;
            if( _x > max_x )
                max_x = _x;
            if( _y < min_y )
                min_y = _y;
            if( _y > max_y )
                max_y = _y;
        }
        qDebug()<<"roomID:"<<getId()<<" custom lines span exit("<<it.key()<<"): x("<<min_x<<"/"<<max_x<<") y("<<min_y<<"/"<<max_y<<")";
    }
    qDebug()<<"-->customLines:"<<customLines;
}

#include <QDataStream>

/*bool - N/U: no return value created or used */
void TRoom::restore( QDataStream & ifs, int roomId, int version )
{

    id = roomId;
    ifs >> areaId;
    ifs >> x;
    ifs >> y;
    ifs >> z;
    ifs >> north;
    ifs >> northeast;
    ifs >> east;
    ifs >> southeast;
    ifs >> south;
    ifs >> southwest;
    ifs >> west;
    ifs >> northwest;
    ifs >> up;
    ifs >> down;
    ifs >> in;
    ifs >> out;
    ifs >> environment;
    ifs >> weight;

    // force room weight >= 1 otherwise pathfinding choses random pathes.
    if( weight < 1 )
    {
        weight = 1;
    }

    if( version < 8 )
    {
        float f1,f2,f3,f4;
        ifs >> f1;//rooms[i]->xRot;
        ifs >> f2;//rooms[i]->yRot;
        ifs >> f3;//rooms[i]->zRot;
        ifs >> f4;//rooms[i]->zoom;
    }
    ifs >> name;
    ifs >> isLocked;
    if( version >= 6 )
    {
        ifs >> other;
    }
    if( version >= 9 )
    {
        ifs >> c;
    }
    if( version >= 10 )
    {
        ifs >> userData;
    }
    if( version >= 11 )
    {
        ifs >> customLines;
        ifs >> customLinesArrow;
        ifs >> customLinesColor;
        ifs >> customLinesStyle;
        ifs >> exitLocks;
    }
    if( version >= 13 )
    {
        ifs >> exitStubs;
    }
    if( version >= 16 )
    {
        ifs >> exitWeights;
        ifs >> doors;
    }
    calcRoomDimensions();
}

void TRoom::auditExits()
{
    if( ! mpRoomDB->getRoom(north) ) north = -1;
    if( ! mpRoomDB->getRoom(south) ) south = -1;
    if( ! mpRoomDB->getRoom(northwest) ) northwest = -1;
    if( ! mpRoomDB->getRoom(northeast) ) northeast = -1;
    if( ! mpRoomDB->getRoom(southwest) ) southwest = -1;
    if( ! mpRoomDB->getRoom(southeast) ) southeast = -1;
    if( ! mpRoomDB->getRoom(west) ) west = -1;
    if( ! mpRoomDB->getRoom(east) ) east = -1;
    if( ! mpRoomDB->getRoom(in) ) in = -1;
    if( ! mpRoomDB->getRoom(out) ) out = -1;

    AUDIT_SPECIAL_EXITS: QMapIterator<int, QString> it( other );
    while( it.hasNext() )
    {
        it.next();
        QString _cmd = it.value();
        if( _cmd.size() <= 0 )
        {
            other.remove( it.key(), it.value() );
            qDebug()<<"AUDIT_SPECIAL_EXITS: roomID:"<<id<<" REMOVING invalid special exit:"<<_cmd;
            goto AUDIT_SPECIAL_EXITS;
        }
        else if( ! ( _cmd.startsWith('1') || _cmd.startsWith('0') ) )
        {
            QString _nc = it.value();
            int _nk = it.key();
            _nc.prepend('0');
            other.remove( it.key(), it.value() );
            other.insertMulti( _nk, _nc );
            qDebug()<<"AUDIT_SPECIAL_EXITS: roomID:"<<id<<" PATCHING invalid special exit:"<<_cmd << " new:"<<_nc;
            goto AUDIT_SPECIAL_EXITS;
        }
    }
}



