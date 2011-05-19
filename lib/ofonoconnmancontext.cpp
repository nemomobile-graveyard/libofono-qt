/*
 * This file is part of ofono-qt
 *
 * Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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
#include <QDBusArgument>

#include "ofonointerface.h"
#include "ofonoconnman.h"
#include "ofonoconnmancontext.h"


OfonoConnmanContext::OfonoConnmanContext(const QString& contextId, QObject *parent)
    : QObject(parent)
{
    m_if = new OfonoInterface(contextId, "org.ofono.ConnectionContext", OfonoGetAllOnStartup, this);

    connect(m_if, SIGNAL(propertyChanged(const QString&, const QVariant&)),
            this, SLOT(propertyChanged(const QString&, const QVariant&)));

}

OfonoConnmanContext::OfonoConnmanContext(const OfonoConnmanContext& context)
    : QObject(context.parent())
{
    m_if = new OfonoInterface(context.path(), "org.ofono.ConnectionContext", OfonoGetAllOnStartup, this);

    connect(m_if, SIGNAL(propertyChanged(const QString&, const QVariant&)),
            this, SLOT(propertyChanged(const QString&, const QVariant&)));

}

bool OfonoConnmanContext::operator==(const OfonoConnmanContext &context)
{
    return path() == context.path();
}

OfonoConnmanContext::~OfonoConnmanContext()
{
}

bool OfonoConnmanContext::active() const
{
    return m_if->properties()["Active"].value<bool>();
}

QString OfonoConnmanContext::accessPointName() const
{
    return m_if->properties()["AccessPointName"].value<QString>();
}

QString OfonoConnmanContext::name() const
{
    return m_if->properties()["Name"].value<QString>();
}

QString OfonoConnmanContext::type() const
{
    return m_if->properties()["Type"].value<QString>();
}

QString OfonoConnmanContext::username() const
{
    return m_if->properties()["Username"].value<QString>();
}

QString OfonoConnmanContext::password() const
{
    return m_if->properties()["Password"].value<QString>();
}

QString OfonoConnmanContext::protocol() const
{
    return m_if->properties()["Protocol"].value<QString>();
}

QString OfonoConnmanContext::messageProxy() const
{
    return m_if->properties()["MessageProxy"].value<QString>();
}

QString OfonoConnmanContext::messageCenter() const
{
    return m_if->properties()["MessageCenter"].value<QString>();
}

void OfonoConnmanContext::propertyChanged(const QString &property, const QVariant &value)
{
    if (property == "Active") {
        emit activeChanged(value.value<bool>());
    } else if (property == "Name") {
        emit nameChanged(value.value<QString>());
    } else if (property == "AccessPointName") {
        emit accessPointNameChanged(value.value<QString>());
    } else if (property == "Type") {
        emit typeChanged(value.value<QString>());
    } else if (property == "Username") {
        emit usernameChanged(value.value<QString>());
    } else if (property == "Password") {
        emit passwordChanged(value.value<QString>());
    } else if (property == "Protocol") {
        emit protocolChanged(value.value<QString>());
    } else if (property == "MessageProxy") {
        emit messageProxyChanged(value.value<QString>());
    } else if (property == "MessageCenter") {
        emit messageCenterChanged(value.value<QString>());
    } else if (property == "Settings") {
        QVariantMap map;
        value.value<QDBusArgument>()>>map;
        emit settingsChanged(map);
    } else if (property == "IPv6.Settings") {
        QVariantMap map;
        value.value<QDBusArgument>()>>map;
        emit IPv6SettingsChanged(map);
    }
}

QVariantMap OfonoConnmanContext::settings() const
{
    QVariantMap map;
    m_if->properties()["Settings"].value<QDBusArgument>()>>map;
    return map;
}

QVariantMap OfonoConnmanContext::IPv6Settings() const
{
    QVariantMap map;
    m_if->properties()["IPv6.Settings"].value<QDBusArgument>()>>map;
    return map;
}

QString OfonoConnmanContext::path() const
{
    return m_if->path();
}

QString OfonoConnmanContext::errorName() const
{
    return m_if->errorName();
}

QString OfonoConnmanContext::errorMessage() const
{
    return m_if->errorMessage();
}

/* Set Property*/
void OfonoConnmanContext::setActive(const bool value)
{
    m_if->setProperty("Active",QVariant(value));
}

void OfonoConnmanContext::setAccessPointName(const QString& value)
{
    m_if->setProperty("AccessPointName",QVariant(value));
}

void OfonoConnmanContext::setType(const QString& value)
{
    m_if->setProperty("Type",QVariant(value));
}

void OfonoConnmanContext::setUsername(const QString& value)
{
    m_if->setProperty("Username",QVariant(value));
}

void OfonoConnmanContext::setPassword(const QString& value)
{
    m_if->setProperty("Password",QVariant(value));
}

void OfonoConnmanContext::setProtocol(const QString& value)
{
    m_if->setProperty("Protocol",QVariant(value));
}

void OfonoConnmanContext::setName(const QString& value)
{
    m_if->setProperty("Name",QVariant(value));
}

void OfonoConnmanContext::setMessageProxy(const QString& value)
{
    m_if->setProperty("MessageProxy",QVariant(value));
}

void OfonoConnmanContext::setMessageCenter(const QString& value)
{
    m_if->setProperty("MessageCenter",QVariant(value));
}

void OfonoConnmanContext::setPropertyFailed(const QString &property)
{
    if (property == "Active") {
        emit setActiveFailed();
    } else if (property == "AccessPointName") {
        emit setAccessPointNameFailed();
    } else if (property == "Type") {
        emit setTypeFailed();
    } else if (property == "Username") {
        emit setUsernameFailed();
    } else if (property == "Password") {
        emit setPasswordFailed();
    } else if (property == "Protocol") {
        emit setProtocolFailed();
    } else if (property == "Name") {
        emit setNameFailed();
    } else if (property == "MessageProxy") {
        emit setMessageProxyFailed();
    } else if (property == "MessageCenter") {
        emit setMessageCenterFailed();
    }
}
