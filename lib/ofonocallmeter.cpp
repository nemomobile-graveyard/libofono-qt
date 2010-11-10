/*
 * This file is part of ofono-qt
 *
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 *
 * Contact: Alexander Kanavin <alexander.kanavin@nokia.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */

#include <QtDBus/QtDBus>
#include <QtCore/QObject>

#include "ofonocallmeter.h"

OfonoCallMeter::OfonoCallMeter(QString modemId, QObject *parent)
    : OfonoModemInterface(modemId, "org.ofono.CallMeter", OfonoInterface::GetAllOnFirstRequest, parent)
{
    connect(this, SIGNAL(propertyChanged(const QString&, const QVariant&)), 
            this, SLOT(propertyChanged(const QString&, const QVariant&)));
    connect(this, SIGNAL(requestPropertyComplete(bool, const QString&, const QVariant&)),
    	    this, SLOT(requestPropertyComplete(bool, const QString&, const QVariant&)));
}

OfonoCallMeter::~OfonoCallMeter()
{
}

void OfonoCallMeter::requestCallMeter()
{
    requestProperty("CallMeter");
}

void OfonoCallMeter::requestPropertyComplete(bool success, const QString& property, const QVariant& value)
{
    if (property == "CallMeter") {	
        success ? emit callMeterComplete(true, value.value<uint>()) : emit callMeterComplete(false, 0);
    }
}


void OfonoCallMeter::propertyChanged(const QString& property, const QVariant& value)
{
    if (property == "CallMeter") {	
        emit callMeterChanged(value.value<uint>());
    }
}
