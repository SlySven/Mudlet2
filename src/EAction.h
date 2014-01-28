#ifndef _E_ACTION_H
#define _E_ACTION_H
/***************************************************************************
 *   Copyright (C) 2008 by Heiko Koehn                                     *
 *   KoehnHeiko@googlemail.com                                             *
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



#include <QAction>
#include <QIcon>
#include <QString>
#include <QWidget>

class mudlet;
class Host;

class EAction : public QAction
{
    Q_OBJECT


public:
                    EAction( QIcon &, QString &, mudlet * );
    QWidget *       createWidget( QWidget * );

    int             mID;
    Host *          mpHost;

public slots:
    void            slot_execute( bool checked );


signals:
    void            triggered( QAction * );


};


#endif //_E_ACTION_H
