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
    QDBusConnection::systemBus().connect("org.ofono", path(), m_if->ifname(), 
					 "IncomingBarringInEffect",
					 this,
					 SIGNAL(incomingBarringInEffect()));
    QDBusConnection::systemBus().connect("org.ofono", path(), m_if->ifname(), 
					 "OutgoingBarringInEffect",
					 this,
					 SIGNAL(outgoingBarringInEffect()));
    m_pendingProperty = QString();
}

OfonoCallBarring::~OfonoCallBarring()
{
}

void OfonoCallBarring::setProperty(const QString& name, 
				   const QVariant& property, 
				   const QString& password)
{
    if (m_pendingProperty.length() > 0) {
        // FIXME: should indicate that a setProperty is already in progress
        m_if->setError(QString(), QString("Operation already in progress"));
        emit setPropertyFailed(name);
        return;
    }


    QDBusMessage request;

    request = QDBusMessage::createMethodCall("org.ofono",
					     path(), m_if->ifname(),
					     "SetProperty");
    request.setArguments(QList<QVariant>() 
			 << QVariant(name) 
			 << QVariant::fromValue(QDBusVariant(property))
			 << QVariant(password));

    bool result = QDBusConnection::systemBus().callWithCallback(request, this,
    					SLOT(setPropertyResp()),
    					SLOT(setPropertyErr(const QDBusError&)),
    					SET_PROPERTY_TIMEOUT);
    if (!result) {
        // FIXME: should indicate that sending a message failed
        m_if->setError(QString(), QString("DBUS sending failed"));
    	emit setPropertyFailed(name);
    	return;
    }
    m_pendingProperty = name;


}

void OfonoCallBarring::setPropertyResp()
{
    m_pendingProperty = QString();
    // emit nothing; we will get a PropertyChanged signal
}

void OfonoCallBarring::setPropertyErr(const QDBusError& error)
{
    QString prop = m_pendingProperty;
    m_if->setError(error.name(), error.message());
    m_pendingProperty = QString();
    emit setPropertyFailed(prop);
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
    setProperty("VoiceIncoming", qVariantFromValue(barrings), password);
}

void OfonoCallBarring::requestVoiceOutgoing()
{
    m_if->requestProperty("VoiceOutgoing");
}

void OfonoCallBarring::setVoiceOutgoing(const QString &barrings, const QString &password)
{
    setProperty("VoiceOutgoing", qVariantFromValue(barrings), password);
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
        success ? emit voiceIncomingComplete(true, value.value<QString>()) : emit voiceIncomingComplete(false, QString());
    } else if (property == "VoiceOutgoing") {	
        success ? emit voiceOutgoingComplete(true, value.value<QString>()) : emit voiceOutgoingComplete(false, QString());
    }
}

void OfonoCallBarring::changePasswordResp()
{
    emit changePasswordComplete(TRUE);
}

void OfonoCallBarring::changePasswordErr(QDBusError error)
{
    qDebug() << "ChangePassword failed" << error;
    m_if->setError(error.name(), error.message());
    emit changePasswordComplete(FALSE);
}

void OfonoCallBarring::disableAllResp()
{
    emit disableAllComplete(TRUE);
}

void OfonoCallBarring::disableAllErr(QDBusError error)
{
    qDebug() << "DisableAll failed" << error;
    m_if->setError(error.name(), error.message());
    emit disableAllComplete(FALSE);
}

void OfonoCallBarring::disableAllIncomingResp()
{
    emit disableAllIncomingComplete(TRUE);
}

void OfonoCallBarring::disableAllIncomingErr(QDBusError error)
{
    qDebug() << "DisableAllIncoming failed" << error;
    m_if->setError(error.name(), error.message());
    emit disableAllIncomingComplete(FALSE);
}

void OfonoCallBarring::disableAllOutgoingResp()
{
    emit disableAllOutgoingComplete(TRUE);
}

void OfonoCallBarring::disableAllOutgoingErr(QDBusError error)
{
    qDebug() << "DisableAllOutgoing failed" << error;
    m_if->setError(error.name(), error.message());
    emit disableAllOutgoingComplete(FALSE);
}
