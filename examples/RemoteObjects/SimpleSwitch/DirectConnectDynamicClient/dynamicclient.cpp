/****************************************************************************
**
** Copyright (C) 2014 Ford Motor Company
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "dynamicclient.h"
 #include <QMetaMethod>

// constructor
DynamicClient::DynamicClient(QSharedPointer<QRemoteObjectDynamicReplica> ptr) :
    QObject(Q_NULLPTR), reptr(ptr)
{
    //connect signal for replica valid changed with signal slot initialization
    QObject::connect(reptr.data(), SIGNAL(initialized()), this, SLOT(initConnection_slot()));
}

//destructor
DynamicClient::~DynamicClient()
{

}

// Function to initialize connections between slots and signals
void DynamicClient::initConnection_slot()
{

    // connect source replica signal currStateChanged() with client's recSwitchState() slot to receive source's current state
   QObject::connect(reptr.data(), SIGNAL(currStateChanged()), this, SLOT(recSwitchState_slot()));
   // connect client's echoSwitchState(..) signal with replica's server_slot(..) to echo back received state
   QObject::connect(this, SIGNAL(echoSwitchState(bool)),reptr.data(), SLOT(server_slot(bool)));
}


void DynamicClient::recSwitchState_slot()
{
   clientSwitchState = reptr->property("currState").toBool(); // use replica property to get "currState" from source
   qDebug() << "Received source state " << clientSwitchState;
   Q_EMIT echoSwitchState(clientSwitchState); // Emit signal to echo received state back to server
}

