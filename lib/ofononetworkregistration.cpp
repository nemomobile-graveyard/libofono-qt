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


OfonoNetworkRegistration::OfonoNetworkRegistration(QString modemId, QObject *parent)
    : OfonoModemInterface(modemId, "org.ofono.NetworkRegistration", OfonoInterface::GetAllOnStartup, parent)
{
    qDBusRegisterMetaType<OfonoOperatorStruct>();
    qDBusRegisterMetaType<OfonoOperatorList>();

    connect(this, SIGNAL(propertyChanged(const QString&, const QVariant&)), 
            this, SLOT(propertyChanged(const QString&, const QVariant&)));
}

OfonoNetworkRegistration::~OfonoNetworkRegistration()
{
}

void OfonoNetworkRegistration::requestRegister()
{
    QDBusMessage request;

    request = QDBusMessage::createMethodCall("org.ofono",
					     path(), ifname(),
					     "Register");

    QDBusConnection::systemBus().callWithCallback(request, this,
					SLOT(registerResp()),
					SLOT(registerErr(const QDBusError&)),
					REGISTER_TIMEOUT);
}

void OfonoNetworkRegistration::requestDeregister()
{
    QDBusMessage request;

    request = QDBusMessage::createMethodCall("org.ofono",
					     path(), ifname(),
					     "Deregister");

    QDBusConnection::systemBus().callWithCallback(request, this,
					SLOT(deregisterResp()),
					SLOT(deregisterErr(const QDBusError&)),
					REGISTER_TIMEOUT);
}

void OfonoNetworkRegistration::requestScan()
{
    QDBusMessage request;

    request = QDBusMessage::createMethodCall("org.ofono",
					     path(), ifname(),
					     "Scan");

    QDBusConnection::systemBus().callWithCallback(request, this,
					SLOT(scanResp(OfonoOperatorList)),
					SLOT(scanErr(const QDBusError&)),
					REGISTER_TIMEOUT);
}

void OfonoNetworkRegistration::requestGetOperators()
{
    QDBusMessage request;

    request = QDBusMessage::createMethodCall("org.ofono",
					     path(), ifname(),
					     "GetOperators");

    QDBusConnection::systemBus().callWithCallback(request, this,
					SLOT(getOperatorsResp(OfonoOperatorList)),
					SLOT(getOperatorsErr(const QDBusError&)),
					SCAN_TIMEOUT);
}

QString OfonoNetworkRegistration::mode()
{
    return properties()["Mode"].value<QString>();
}

QString OfonoNetworkRegistration::status()
{
    return properties()["Status"].value<QString>();
}

uint OfonoNetworkRegistration::locationAreaCode()
{
    return properties()["LocationAreaCode"].value<uint>();
}

uint OfonoNetworkRegistration::cellId()
{
    return properties()["CellId"].value<uint>();
}

QString OfonoNetworkRegistration::mcc()
{
    return properties()["MobileCountryCode"].value<QString>();
}

QString OfonoNetworkRegistration::mnc()
{
    return properties()["MobileNetworkCode"].value<QString>();
}

QString OfonoNetworkRegistration::technology()
{
    return properties()["Technology"].value<QString>();
}

QString OfonoNetworkRegistration::name()
{
    return properties()["Name"].value<QString>();
}

uint OfonoNetworkRegistration::strength()
{
    return properties()["Strength"].value<uint>();
}

QString OfonoNetworkRegistration::baseStation()
{
    return properties()["BaseStation"].value<QString>();
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
    m_errorName = error.name();
    m_errorMessage = error.message();
    emit registerComplete(FALSE);
}

void OfonoNetworkRegistration::deregisterResp()
{
    emit deregisterComplete(TRUE);
}

void OfonoNetworkRegistration::deregisterErr(QDBusError error)
{
    qDebug() << "Deregister failed" << error;
    m_errorName = error.name();
    m_errorMessage = error.message();
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
    m_errorName = error.name();
    m_errorMessage = error.message();
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
    m_errorName = error.name();
    m_errorMessage = error.message();
    emit scanComplete(FALSE, QStringList());
}

