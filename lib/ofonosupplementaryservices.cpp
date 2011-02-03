/*
 * This file is part of ofono-qt
 *
 * Copyright (C) 2010-2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "ofonosupplementaryservices.h"
#include "ofonointerface.h"

#define REQUEST_TIMEOUT 60000

OfonoSupplementaryServices::OfonoSupplementaryServices(OfonoModem::SelectionSetting modemSetting, const QString &modemPath, QObject *parent)
    : OfonoModemInterface(modemSetting, modemPath, "org.ofono.SupplementaryServices", OfonoGetAllOnStartup, parent)
{

    connect(m_if, SIGNAL(propertyChanged(const QString&, const QVariant&)), 
            this, SLOT(propertyChanged(const QString&, const QVariant&)));
    QDBusConnection::systemBus().connect("org.ofono", path(), m_if->ifname(), 
					     "NotificationReceived",
					     this,
					     SIGNAL(notificationReceived(QString)));
    QDBusConnection::systemBus().connect("org.ofono", path(), m_if->ifname(), 
					     "RequestReceived",
					     this,
					     SIGNAL(requestReceived(QString)));
}

OfonoSupplementaryServices::~OfonoSupplementaryServices()
{
}

void OfonoSupplementaryServices::initiate(const QString &command)
{
    QDBusMessage request;

    request = QDBusMessage::createMethodCall("org.ofono",
					     path(), m_if->ifname(),
					     "Initiate");
    request << command;

    QDBusConnection::systemBus().callWithCallback(request, this,
					SLOT(initiateResp(QString, QDBusVariant)),
					SLOT(initiateErr(const QDBusError&)),
					REQUEST_TIMEOUT);
}

void OfonoSupplementaryServices::respond(const QString &reply)
{
    QDBusMessage request;

    request = QDBusMessage::createMethodCall("org.ofono",
					     path(), m_if->ifname(),
					     "Respond");
    request << reply;

    QDBusConnection::systemBus().callWithCallback(request, this,
					SLOT(respondResp(QString)),
					SLOT(respondErr(const QDBusError&)),
					REQUEST_TIMEOUT);
}

void OfonoSupplementaryServices::cancel()
{
    QDBusMessage request;

    request = QDBusMessage::createMethodCall("org.ofono",
					     path(), m_if->ifname(),
					     "Cancel");

    QDBusConnection::systemBus().callWithCallback(request, this,
					SLOT(cancelResp()),
					SLOT(cancelErr(const QDBusError&)),
					REQUEST_TIMEOUT);
}

QString OfonoSupplementaryServices::state() const
{
    return m_if->properties()["State"].value<QString>();
}

void OfonoSupplementaryServices::propertyChanged(const QString& property, const QVariant& value)
{
    if (property == "State") {	
        emit stateChanged(value.value<QString>());
    } 
}

void OfonoSupplementaryServices::initiateResp(QString message, QDBusVariant details)
{
    const QDBusArgument argument = details.variant().value<QDBusArgument>();
    if (message == "USSD") {
    	QString ussdResp = details.variant().toString();
        emit initiateUSSDComplete(ussdResp);
    } else if (message == "CallBarring") {
        QString ssOp, cbService;
        QVariantMap cbMap;
        argument.beginStructure();
        argument >> ssOp >> cbService >> cbMap;
        argument.endStructure();
        emit barringComplete(ssOp, cbService, cbMap);
    } else if (message == "CallForwarding") {
        QString ssOp, cfService;
        QVariantMap cfMap;
        argument.beginStructure();
        argument >> ssOp >> cfService >> cfMap;
        argument.endStructure();
        emit forwardingComplete(ssOp, cfService, cfMap);
    } else if (message == "CallWaiting") {
        QString ssOp;
        QVariantMap cwMap;
        argument.beginStructure();
        argument >> ssOp >> cwMap;
        argument.endStructure();
        emit waitingComplete(ssOp, cwMap);
    } else if (message == "CallingLinePresentation") {
        QString ssOp, status;
        argument.beginStructure();
        argument >> ssOp >> status;
        argument.endStructure();
        emit callingLinePresentationComplete(ssOp, status);
    } else if (message == "ConnectedLinePresentation") {
        QString ssOp, status;
        argument.beginStructure();
        argument >> ssOp >> status;
        argument.endStructure();
        emit connectedLinePresentationComplete(ssOp, status);
    } else if (message == "CallingLineRestriction") {
        QString ssOp, status;
        argument.beginStructure();
        argument >> ssOp >> status;
        argument.endStructure();
        emit callingLineRestrictionComplete(ssOp, status);
    } else if (message == "ConnectedLineRestriction") {
        QString ssOp, status;
        argument.beginStructure();
        argument >> ssOp >> status;
        argument.endStructure();
        emit connectedLineRestrictionComplete(ssOp, status);
    } else {
        m_if->setError(QString(), QString("Unknown initiate response"));
        emit initiateFailed();
    }
}

void OfonoSupplementaryServices::initiateErr(QDBusError error)
{
    qDebug() << "Initiate failed" << error;
    m_if->setError(error.name(), error.message());
    emit initiateFailed();
}


void OfonoSupplementaryServices::respondResp(QString message)
{
    emit respondComplete(TRUE, message);
}

void OfonoSupplementaryServices::respondErr(QDBusError error)
{
    qDebug() << "Respond failed" << error;
    m_if->setError(error.name(), error.message());
    emit respondComplete(FALSE, QString());
}

void OfonoSupplementaryServices::cancelResp()
{
    emit cancelComplete(TRUE);
}

void OfonoSupplementaryServices::cancelErr(QDBusError error)
{
    qDebug() << "Cancel failed" << error;
    m_if->setError(error.name(), error.message());
    emit cancelComplete(FALSE);
}

