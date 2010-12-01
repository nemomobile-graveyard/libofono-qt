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

#ifndef OFONOCALLMETER_H
#define OFONOCALLMETER_H

#include <QtCore/QObject>
#include "ofonomodeminterface.h"
#include "libofono-qt_global.h"

//! This class is used to access oFono call meter API
/*!
 * The API is documented in
 * http://git.kernel.org/?p=network/ofono/ofono.git;a=blob_plain;f=doc/call-meter-api.txt
 */
class OFONO_QT_EXPORT OfonoCallMeter : public OfonoModemInterface
{
    Q_OBJECT

public:
    OfonoCallMeter(OfonoModem::SelectionSetting modemSetting, const QString &modemPath, QObject *parent=0);
    ~OfonoCallMeter();

    /* Properties */
    void requestCallMeter();
    
signals:
    void callMeterComplete(bool success, uint value);

    void callMeterChanged(uint value);

private slots:
    void propertyChanged(const QString& property, const QVariant& value);
    void requestPropertyComplete(bool success, const QString& property, const QVariant& value);
};

#endif  /* !OFONOCALLMETER_H */
