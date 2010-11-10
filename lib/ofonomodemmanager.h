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
 
#ifndef OFONOMODEMMANAGER_H
#define OFONOMODEMMANAGER_H

#include <QtDBus/QtDBus>
#include <QtCore/QObject>
#include "libofono-qt_global.h"

struct OfonoModemStruct {
    QDBusObjectPath path;
    QVariantMap properties;
};
typedef QList<OfonoModemStruct> OfonoModemList;
Q_DECLARE_METATYPE(OfonoModemStruct)
Q_DECLARE_METATYPE(OfonoModemList)
// Marshall the ModemStruct data into a D-Bus argument
QDBusArgument &operator<<(QDBusArgument &argument, const OfonoModemStruct &modem);
    
// Retrieve the ModemStruct data from the D-Bus argument
const QDBusArgument &operator>>(const QDBusArgument &argument, OfonoModemStruct &modem);


class OfonoInterface;

class OFONO_QT_EXPORT OfonoModemManager : public QObject {

Q_OBJECT

public:

    OfonoModemManager(QObject *parent);

    ~OfonoModemManager();

    QStringList modems();

private slots:
    void onModemAdded(const QDBusObjectPath& path, const QVariantMap& map);
    void onModemRemoved(const QDBusObjectPath& path);
    
signals:
    void modemAdded(QString modem);
    void modemRemoved(QString modem);

private:
    QStringList m_modems;
};

#endif
