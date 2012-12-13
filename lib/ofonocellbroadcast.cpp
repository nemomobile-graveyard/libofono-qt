/*
 * This file is part of ofono-qt
 *
 * Copyright (C) 2012 Jolla Ltd.
 *
 * Contact: Lorn Potter <lorn.potter@jollamobile.com>
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

#include "ofonocellbroadcast.h"
#include "ofonointerface.h"

OfonoCellBroadcast::OfonoCellBroadcast(OfonoModem::SelectionSetting modemSetting, const QString &modemPath, QObject *parent)
    : OfonoModemInterface(modemSetting, modemPath, "org.ofono.CellBroadcast", OfonoGetAllOnStartup, parent)
{
    connect(m_if, SIGNAL(propertyChanged(const QString&, const QVariant&)),
            this, SLOT(propertyChanged(const QString&, const QVariant&)));

    QDBusConnection::systemBus().connect("org.ofono",path(),m_if->ifname(),
                                         "IncomingBroadcast", this,
                                         SLOT(inBroadcast(const QString &, quint16)));
    QDBusConnection::systemBus().connect("org.ofono",path(),m_if->ifname(),
                                         "EmergencyBroadcast", this,
                                         SLOT(emBroadcast(const QString &,const QVariantMap &)));
}

OfonoCellBroadcast::~OfonoCellBroadcast()
{
}

QString OfonoCellBroadcast::path() const
{
    return m_if->path();
}

void OfonoCellBroadcast::propertyChanged(const QString& property, const QVariant& value)
{
    if (property == "Powered") {
        Q_EMIT powerChanged(value.value<bool>());
    } else if (property == "Topics") {
        Q_EMIT topicsChanged(value.value<QString>());
    }
}

void OfonoCellBroadcast::inBroadcast(const QString &message, quint16 channel)
{
    Q_EMIT incomingBroadcast(message,channel);
}

/*
  0x1100 - 0x1104 'Channel' in phonesim: (4352 - 4356)
    QMap(("EmergencyAlert", QVariant(bool, false) ) ( "EmergencyType" ,
    QVariant(QString, "Earthquake+Tsunami") ) ( "Popup" ,  QVariant(bool, false) ) )
*/
void OfonoCellBroadcast::emBroadcast(const QString &message, const QVariantMap &properties)
{
    Q_EMIT emergencyBroadcast(message,properties);
}

bool OfonoCellBroadcast::powered() const
{
    return m_if->properties()["Powered"].value<bool>();
}

void OfonoCellBroadcast::setPowered(bool b)
{
    m_if->setProperty("Powered",qVariantFromValue(b));
}

/*
  empty means only emergency should only be received.
  "20,50-51,60" called channels in phonesim
  */
QString OfonoCellBroadcast::topics() const
{
   return m_if->properties()["Topics"].value<QString>();
}

void OfonoCellBroadcast::setTopics(const QString &list) const
{
    m_if->setProperty("Topics",qVariantFromValue(list));
}

QString OfonoCellBroadcast::errorName() const
{
    return m_if->errorName();
}

QString OfonoCellBroadcast::errorMessage() const
{
    return m_if->errorMessage();
}
