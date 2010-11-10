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

#ifndef OFONOMODEMINTERFACE_H
#define OFONOMODEMINTERFACE_H

#include <QtCore/QObject>
#include <QVariant>
#include <QDBusVariant>
#include <QStringList>
#include "ofonointerface.h"
#include "libofono-qt_global.h"

class OfonoModem;

class OFONO_QT_EXPORT OfonoModemInterface : public OfonoInterface
{
    Q_OBJECT
public:
    OfonoModemInterface(const QString& modemId, const QString& ifname, GetPropertySetting setting, QObject *parent=0);
    ~OfonoModemInterface();

    bool isValid() const;
    OfonoModem *modem() const;

signals:
    void validityChanged(bool validity);

private:
    bool checkValidity();
    void updateValidity();

private slots:
    void modemValidityChanged(bool validity);
    void interfacesChanged(QStringList interfaces);

private:
    OfonoModem *m_m;
    bool m_isValid;
};
#endif
