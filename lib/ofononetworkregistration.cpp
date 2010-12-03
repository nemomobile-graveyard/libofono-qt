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

#include "ofononetworkregistration.h"
#include "ofonointerface.h"

#define REGISTER_TIMEOUT 300000
#define SCAN_TIMEOUT 300000

QDBusArgument &operator<<(QDBusArgument &argument, const OfonoOperatorStruct &op)
{
    argument.beginStructure();
    argument << op.path << op.properties;
    argument.endStructure();
    return argument;
}   

const QDBusArgument &operator>>(const QDBusArgument &argument, OfonoOperatorStruct &op)
{   
    argument.beginStructure();
    argument >> op.path >> op.properties; 
    argument.endStructure();
    return argument;
}


OfonoNetworkRegistration::OfonoNetworkRegistration(OfonoModem::SelectionSetting modemSetting, const QString &modemPath, QObject *parent)
    : OfonoModemInterface(modemSetting, modemPath, "org.ofono.NetworkRegistration", OfonoGetAllOnStartup, parent)
{
    qDBusRegisterMetaType<OfonoOperatorStruct>();
    qDBusRegisterMetaType<OfonoOperatorList>();

    connect(m_if, SIGNAL(propertyChanged(const QString&, const QVariant&)), 
            this, SLOT(propertyChanged(const QString&, const QVariant&)));
}

OfonoNetworkRegistration::~OfonoNetworkRegistration()
{
}

void OfonoNetworkRegistration::registerOp()
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

void OfonoNetworkRegistration::deregister()
{
    QDBusMessage request;

    request = QDBusMessage::createMethodCall("org.ofono",
					     path(), m_if->ifname(),
					     "Deregister");

    QDBusConnection::systemBus().callWithCallback(request, this,
					SLOT(deregisterResp()),
					SLOT(deregisterErr(const QDBusError&)),
					REGISTER_TIMEOUT);
}

void OfonoNetworkRegistration::scan()
{
    QDBusMessage request;

    request = QDBusMessage::createMethodCall("org.ofono",
					     path(), m_if->ifname(),
					     "Scan");

    QDBusConnection::systemBus().callWithCallback(request, this,
					SLOT(scanResp(OfonoOperatorList)),
					SLOT(scanErr(const QDBusError&)),
					REGISTER_TIMEOUT);
}

void OfonoNetworkRegistration::getOperators()
{
    QDBusMessage request;

    request = QDBusMessage::createMethodCall("org.ofono",
					     path(), m_if->ifname(),
					     "GetOperators");

    QDBusConnection::systemBus().callWithCallback(request, this,
					SLOT(getOperatorsResp(OfonoOperatorList)),
					SLOT(getOperatorsErr(const QDBusError&)),
					SCAN_TIMEOUT);
}

QString OfonoNetworkRegistration::mode() const
{
    return m_if->properties()["Mode"].value<QString>();
}

QString OfonoNetworkRegistration::status() const
{
    return m_if->properties()["Status"].value<QString>();
}

uint OfonoNetworkRegistration::locationAreaCode() const
{
    return m_if->properties()["LocationAreaCode"].value<uint>();
}

uint OfonoNetworkRegistration::cellId() const
{
    return m_if->properties()["CellId"].value<uint>();
}

QString OfonoNetworkRegistration::mcc() const
{
    return m_if->properties()["MobileCountryCode"].value<QString>();
}

QString OfonoNetworkRegistration::mnc() const
{
    return m_if->properties()["MobileNetworkCode"].value<QString>();
}

QString OfonoNetworkRegistration::technology() const
{
    return m_if->properties()["Technology"].value<QString>();
}

QString OfonoNetworkRegistration::name() const
{
    return m_if->properties()["Name"].value<QString>();
}

uint OfonoNetworkRegistration::strength() const
{
    return m_if->properties()["Strength"].value<uint>();
}

QString OfonoNetworkRegistration::baseStation() const
{
    return m_if->properties()["BaseStation"].value<QString>();
}

void OfonoNetworkRegistration::propertyChanged(const QString& property, const QVariant& value)
{
    if (property == "Mode") {	
        emit modeChanged(value.value<QString>());
    } else if (property == "Status") {	
        emit statusChanged(value.value<QString>());
    } else if (property == "LocationAreaCode") {	
        emit locationAreaCodeChanged(value.value<uint>());
    } else if (property == "CellId") {	
        emit cellIdChanged(value.value<uint>());
    } else if (property == "MobileCountryCode") {	
        emit mccChanged(value.value<QString>());
    } else if (property == "MobileNetworkCode") {	
        emit mncChanged(value.value<QString>());
    } else if (property == "Technology") {	
        emit technologyChanged(value.value<QString>());
    } else if (property == "Name") {	
        emit nameChanged(value.value<QString>());
    } else if (property == "Strength") {	
        emit strengthChanged(value.value<uint>());
    } else if (property == "BaseStation") {	
        emit baseStationChanged(value.value<QString>());
    }
}

void OfonoNetworkRegistration::registerResp()
{
    emit registerComplete(TRUE);
}

void OfonoNetworkRegistration::registerErr(QDBusError error)
{
    qDebug() << "Register failed" << error;
    m_if->setError(error.name(), error.message());
    emit registerComplete(FALSE);
}

void OfonoNetworkRegistration::deregisterResp()
{
    emit deregisterComplete(TRUE);
}

void OfonoNetworkRegistration::deregisterErr(QDBusError error)
{
    qDebug() << "Deregister failed" << error;
    m_if->setError(error.name(), error.message());
    emit deregisterComplete(FALSE);
}

void OfonoNetworkRegistration::getOperatorsResp(OfonoOperatorList list)
{
    QStringList oplist;
    foreach(OfonoOperatorStruct op, list) {
    	oplist << op.path.path();
    }
    emit getOperatorsComplete(TRUE, oplist);
}

void OfonoNetworkRegistration::getOperatorsErr(QDBusError error)
{
    qDebug() << "GetOperators failed" << error;
    m_if->setError(error.name(), error.message());
    emit getOperatorsComplete(FALSE, QStringList());
}

void OfonoNetworkRegistration::scanResp(OfonoOperatorList list)
{
    QStringList oplist;
    foreach(OfonoOperatorStruct op, list) {
    	oplist << op.path.path();
    }
    emit scanComplete(TRUE, oplist);
}

void OfonoNetworkRegistration::scanErr(QDBusError error)
{
    qDebug() << "Scan failed" << error;
    m_if->setError(error.name(), error.message());
    emit scanComplete(FALSE, QStringList());
}

