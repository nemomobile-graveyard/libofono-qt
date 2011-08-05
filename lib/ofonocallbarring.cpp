/*
 * This file is part of ofono-qt
 *
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 *
 * Contact: Alexander Kanavin <alex.kanavin@gmail.com>
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

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
#include "ofonocallbarring.h"
#include "ofonointerface.h"

#define SET_PROPERTY_TIMEOUT 300000

OfonoCallBarring::OfonoCallBarring(OfonoModem::SelectionSetting modemSetting, const QString &modemPath, QObject *parent)
    : OfonoModemInterface(modemSetting, modemPath, "org.ofono.CallBarring", OfonoGetAllOnFirstRequest, parent)
{
    connect(m_if, SIGNAL(propertyChanged(const QString&, const QVariant&)), 
            this, SLOT(propertyChanged(const QString&, const QVariant&)));
    connect(m_if, SIGNAL(setPropertyFailed(const QString&)), 
            this, SLOT(setPropertyFailed(const QString&)));
    connect(m_if, SIGNAL(requestPropertyComplete(bool, const QString&, const QVariant&)),
    	    this, SLOT(requestPropertyComplete(bool, const QString&, const QVariant&)));

    connect(modem(), SIGNAL(pathChanged(QString)), this, SLOT(pathChanged(const QString&)));

    connectDbusSignals(path());
}

OfonoCallBarring::~OfonoCallBarring()
{
}

void OfonoCallBarring::pathChanged(const QString& path)
{
    connectDbusSignals(path);
}

void OfonoCallBarring::connectDbusSignals(const QString& /*path*/)
{
}

void OfonoCallBarring::changePassword(const QString &old_password, 
					     const QString &new_password)
{
    QDBusMessage request;

    request = QDBusMessage::createMethodCall("org.ofono",
					     path(), m_if->ifname(),
					     "ChangePassword");
    request << old_password << new_password;

    QDBusConnection::systemBus().callWithCallback(request, this,
					SLOT(changePasswordResp()),
					SLOT(changePasswordErr(const QDBusError&)));
}

void OfonoCallBarring::disableAll(const QString &password)
{
    QDBusMessage request;

    request = QDBusMessage::createMethodCall("org.ofono",
					     path(), m_if->ifname(),
					     "DisableAll");
    request << password;

    QDBusConnection::systemBus().callWithCallback(request, this,
					SLOT(disableAllResp()),
					SLOT(disableAllErr(const QDBusError&)));
}

void OfonoCallBarring::disableAllIncoming(const QString &password)
{
    QDBusMessage request;

    request = QDBusMessage::createMethodCall("org.ofono",
					     path(), m_if->ifname(),
					     "DisableAllIncoming");
    request << password;

    QDBusConnection::systemBus().callWithCallback(request, this,
					SLOT(disableAllIncomingResp()),
					SLOT(disableAllIncomingErr(const QDBusError&)));
}

void OfonoCallBarring::disableAllOutgoing(const QString &password)
{
    QDBusMessage request;

    request = QDBusMessage::createMethodCall("org.ofono",
					     path(), m_if->ifname(),
					     "DisableAllOutgoing");
    request << password;

    QDBusConnection::systemBus().callWithCallback(request, this,
					SLOT(disableAllOutgoingResp()),
					SLOT(disableAllOutgoingErr(const QDBusError&)));
}


void OfonoCallBarring::requestVoiceIncoming()
{
    m_if->requestProperty("VoiceIncoming");
}

void OfonoCallBarring::setVoiceIncoming(const QString &barrings, const QString &password)
{
    m_if->setProperty("VoiceIncoming", qVariantFromValue(barrings), password);
}

void OfonoCallBarring::requestVoiceOutgoing()
{
    m_if->requestProperty("VoiceOutgoing");
}

void OfonoCallBarring::setVoiceOutgoing(const QString &barrings, const QString &password)
{
    m_if->setProperty("VoiceOutgoing", qVariantFromValue(barrings), password);
}

void OfonoCallBarring::propertyChanged(const QString& property, const QVariant& value)
{
    if (property == "VoiceIncoming") {	
        emit voiceIncomingChanged(value.value<QString>());
    } else  if (property == "VoiceOutgoing") {	
        emit voiceOutgoingChanged(value.value<QString>());
    }
}

void OfonoCallBarring::setPropertyFailed(const QString& property)
{
    if (property == "VoiceIncoming") {	
        emit setVoiceIncomingFailed();
    } else if (property == "VoiceOutgoing") {	
        emit setVoiceOutgoingFailed();
    }  
}

void OfonoCallBarring::requestPropertyComplete(bool success, const QString& property, const QVariant& value)
{
    if (property == "VoiceIncoming") {	
        emit voiceIncomingComplete(success, value.value<QString>());
    } else if (property == "VoiceOutgoing") {	
        emit voiceOutgoingComplete(success, value.value<QString>());
    }
}

void OfonoCallBarring::changePasswordResp()
{
    emit changePasswordComplete(TRUE);
}

void OfonoCallBarring::changePasswordErr(QDBusError error)
{
    m_if->setError(error.name(), error.message());
    emit changePasswordComplete(FALSE);
}

void OfonoCallBarring::disableAllResp()
{
    emit disableAllComplete(TRUE);
}

void OfonoCallBarring::disableAllErr(QDBusError error)
{
    m_if->setError(error.name(), error.message());
    emit disableAllComplete(FALSE);
}

void OfonoCallBarring::disableAllIncomingResp()
{
    emit disableAllIncomingComplete(TRUE);
}

void OfonoCallBarring::disableAllIncomingErr(QDBusError error)
{
    m_if->setError(error.name(), error.message());
    emit disableAllIncomingComplete(FALSE);
}

void OfonoCallBarring::disableAllOutgoingResp()
{
    emit disableAllOutgoingComplete(TRUE);
}

void OfonoCallBarring::disableAllOutgoingErr(QDBusError error)
{
    m_if->setError(error.name(), error.message());
    emit disableAllOutgoingComplete(FALSE);
}
