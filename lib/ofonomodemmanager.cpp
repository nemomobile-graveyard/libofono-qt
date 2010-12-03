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

#include "ofonomodemmanager.h"
#include "ofonointerface.h"

struct OfonoModemStruct {
    QDBusObjectPath path;
    QVariantMap properties;
};
typedef QList<OfonoModemStruct> OfonoModemList;
Q_DECLARE_METATYPE(OfonoModemStruct)
Q_DECLARE_METATYPE(OfonoModemList)

QDBusArgument &operator<<(QDBusArgument &argument, const OfonoModemStruct &modem)
{
    argument.beginStructure();
    argument << modem.path << modem.properties;
    argument.endStructure();
    return argument;
}   

const QDBusArgument &operator>>(const QDBusArgument &argument, OfonoModemStruct &modem)
{   
    argument.beginStructure();
    argument >> modem.path >> modem.properties; 
    argument.endStructure();
    return argument;
}

OfonoModemManager::OfonoModemManager(QObject *parent)
    : QObject(parent)
{
    QDBusReply<OfonoModemList> reply;
    OfonoModemList modems;

    QDBusMessage request;

    qDBusRegisterMetaType<OfonoModemStruct>();
    qDBusRegisterMetaType<OfonoModemList>();

    request = QDBusMessage::createMethodCall("org.ofono",
					     "/", "org.ofono.Manager",
					     "GetModems");
    reply = QDBusConnection::systemBus().call(request);

    modems = reply;
    foreach(OfonoModemStruct modem, modems) {
	m_modems << modem.path.path();
    }

    QDBusConnection::systemBus().connect("org.ofono","/","org.ofono.Manager",
					 "ModemAdded", this, 
					 SLOT(onModemAdded(const QDBusObjectPath&, const QVariantMap&)));
    QDBusConnection::systemBus().connect("org.ofono","/","org.ofono.Manager",
					 "ModemRemoved", this, 
					 SLOT(onModemRemoved(const QDBusObjectPath&)));

}

OfonoModemManager::~OfonoModemManager()
{
}

QStringList OfonoModemManager::modems() const
{
    return m_modems;
}

void OfonoModemManager::onModemAdded(const QDBusObjectPath& path, const QVariantMap& /*map*/)
{
    m_modems << path.path();
    emit modemAdded(path.path());
}

void OfonoModemManager::onModemRemoved(const QDBusObjectPath& path)
{
    m_modems.removeAll(path.path()); 
    emit modemRemoved(path.path());
}


