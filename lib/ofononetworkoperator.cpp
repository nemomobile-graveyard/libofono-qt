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

#include "ofonointerface.h"
#include "ofononetworkoperator.h"

#define REGISTER_TIMEOUT 300000

OfonoNetworkOperator::OfonoNetworkOperator(const QString& operatorId, QObject *parent)
    : QObject(parent)
{
    m_if = new OfonoInterface(operatorId, "org.ofono.NetworkOperator", OfonoGetAllOnStartup, this);
    connect(m_if, SIGNAL(propertyChanged(const QString&, const QVariant&)), 
            this, SLOT(propertyChanged(const QString&, const QVariant&)));
}

OfonoNetworkOperator::OfonoNetworkOperator(const OfonoNetworkOperator& op)
    : QObject(op.parent())
{
    m_if = new OfonoInterface(op.path(), "org.ofono.NetworkOperator", OfonoGetAllOnStartup, this);
    connect(m_if, SIGNAL(propertyChanged(const QString&, const QVariant&)), 
            this, SLOT(propertyChanged(const QString&, const QVariant&)));
}    
    
bool OfonoNetworkOperator::operator==(const OfonoNetworkOperator &op)
{
    return path() == op.path();
}

OfonoNetworkOperator::~OfonoNetworkOperator()
{
}

void OfonoNetworkOperator::registerOp()
{
    QDBusMessage request;

    request = QDBusMessage::createMethodCall("org.ofono",
					     path(), m_if->ifname(),
					     "Register");

    QDBusConnection::systemBus().callWithCallback(request, this,
					SLOT(registerResp()),
					SLOT(registerErr(const QDBusError&)),
					REGISTER_TIMEOUT);
}

void OfonoNetworkOperator::registerResp()
{
    emit registerComplete(TRUE);
}

void OfonoNetworkOperator::registerErr(const QDBusError& error)
{
    m_if->setError(error.name(), error.message());
    emit registerComplete(FALSE);
}

QString OfonoNetworkOperator::name() const
{
    return m_if->properties()["Name"].value<QString>();
}

QString OfonoNetworkOperator::status() const
{
    return m_if->properties()["Status"].value<QString>();
}

QString OfonoNetworkOperator::mcc() const
{
    return m_if->properties()["MobileCountryCode"].value<QString>();
}

QString OfonoNetworkOperator::mnc() const
{
    return m_if->properties()["MobileNetworkCode"].value<QString>();
}

QStringList OfonoNetworkOperator::technologies() const
{
    return m_if->properties()["Technologies"].value<QStringList>();
}

QString OfonoNetworkOperator::additionalInfo() const
{
    return m_if->properties()["AdditionalInformation"].value<QString>();
}

void OfonoNetworkOperator::propertyChanged(const QString& property, const QVariant& value)
{
    if (property == "Name") {	
        emit nameChanged(value.value<QString>());
    } else if (property == "Status") {	
        emit statusChanged(value.value<QString>());
    } else if (property == "MobileCountryCode") {	
        emit mccChanged(value.value<QString>());
    } else if (property == "MobileNetworkCode") {	
        emit mncChanged(value.value<QString>());
    } else if (property == "Technologies") {	
        emit technologiesChanged(value.value<QStringList>());
    } else if (property == "AdditionalInformation") {	
        emit additionalInfoChanged(value.value<QString>());
    }
}

QString OfonoNetworkOperator::path() const
{
    return m_if->path();
}
    
QString OfonoNetworkOperator::errorName() const
{
    return m_if->errorName();
}

QString OfonoNetworkOperator::errorMessage() const
{
    return m_if->errorMessage();
}

