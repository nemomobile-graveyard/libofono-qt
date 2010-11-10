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

#include "ofonosimmanager.h"


OfonoSimManager::OfonoSimManager(QString modemId, QObject *parent)
    : OfonoModemInterface(modemId, "org.ofono.SimManager", OfonoInterface::GetAllOnStartup, parent)
{
    connect(this, SIGNAL(propertyChanged(const QString&, const QVariant&)), 
            this, SLOT(propertyChanged(const QString&, const QVariant&)));
    connect(this, SIGNAL(setPropertyFailed(const QString&)), 
            this, SLOT(setPropertyFailed(const QString&)));

}

OfonoSimManager::~OfonoSimManager()
{
}

void OfonoSimManager::requestChangePin(QString pintype, QString oldpin, QString newpin)
{
    QDBusMessage request;

    request = QDBusMessage::createMethodCall("org.ofono",
					     path(), ifname(),
					     "ChangePin");
    request << pintype << oldpin << newpin;

    QDBusConnection::systemBus().callWithCallback(request, this,
					SLOT(changePinResp()),
					SLOT(changePinErr(const QDBusError&)));
}

void OfonoSimManager::requestEnterPin(QString pintype, QString pin)
{
    QDBusMessage request;

    request = QDBusMessage::createMethodCall("org.ofono",
					     path(), ifname(),
					     "EnterPin");
    request << pintype << pin;

    QDBusConnection::systemBus().callWithCallback(request, this,
					SLOT(enterPinResp()),
					SLOT(enterPinErr(const QDBusError&)));
}

void OfonoSimManager::requestResetPin(QString pintype, QString puk, QString newpin)
{
    QDBusMessage request;

    request = QDBusMessage::createMethodCall("org.ofono",
					     path(), ifname(),
					     "ResetPin");
    request << pintype << puk << newpin;

    QDBusConnection::systemBus().callWithCallback(request, this,
					SLOT(resetPinResp()),
					SLOT(resetPinErr(const QDBusError&)));
}

void OfonoSimManager::requestLockPin(QString pintype, QString pin)
{
    QDBusMessage request;

    request = QDBusMessage::createMethodCall("org.ofono",
					     path(), ifname(),
					     "LockPin");
    request << pintype << pin;

    QDBusConnection::systemBus().callWithCallback(request, this,
					SLOT(lockPinResp()),
					SLOT(lockPinErr(const QDBusError&)));
}

void OfonoSimManager::requestUnlockPin(QString pintype, QString pin)
{
    QDBusMessage request;

    request = QDBusMessage::createMethodCall("org.ofono",
					     path(), ifname(),
					     "UnlockPin");
    request << pintype << pin;

    QDBusConnection::systemBus().callWithCallback(request, this,
					SLOT(unlockPinResp()),
					SLOT(unlockPinErr(const QDBusError&)));
}

void OfonoSimManager::setSubscriberNumbers(QStringList numbers)
{
    setProperty("SubscriberNumbers", qVariantFromValue(numbers));
}

bool OfonoSimManager::present()
{
    return properties()["Present"].value<bool>();
}

QString OfonoSimManager::subscriberIdentity()
{
    return properties()["SubscriberIdentity"].value<QString>();
}

QString OfonoSimManager::mobileCountryCode()
{
    return properties()["MobileCountryCode"].value<QString>();
}

QString OfonoSimManager::mobileNetworkCode()
{
    return properties()["MobileNetworkCode"].value<QString>();
}

QStringList OfonoSimManager::subscriberNumbers()
{
    return properties()["SubscriberNumbers"].value<QStringList>();
}

QMap<QString, QString> OfonoSimManager::serviceNumbers()
{
    QMap<QString, QString> map;
    properties()["ServiceNumbers"].value<QDBusArgument>() >> map;
    return map;
}

QString OfonoSimManager::pinRequired()
{
    return properties()["PinRequired"].value<QString>();
}

QStringList OfonoSimManager::lockedPins()
{
    return properties()["LockedPins"].value<QStringList>();
}

QString OfonoSimManager::cardIdentifier()
{
    return properties()["CardIdentifier"].value<QString>();
}

QStringList OfonoSimManager::preferredLanguages()
{
    return properties()["PreferredLanguages"].value<QStringList>();
}


void OfonoSimManager::propertyChanged(const QString& property, const QVariant& value)
{
    if (property == "Present") {	
        emit presenceChanged(value.value<bool>());
    } else if (property == "SubscriberIdentity") {	
        emit subscriberIdentityChanged(value.value<QString>());
    } else if (property == "MobileCountryCode") {	
        emit mobileCountryCodeChanged(value.value<QString>());
    } else if (property == "MobileNetworkCode") {	
        emit mobileNetworkCodeChanged(value.value<QString>());
    } else if (property == "SubscriberNumbers") {	
        emit subscriberNumbersChanged(value.value<QStringList>());
    } else if (property == "ServiceNumbers") {	
        QMap<QString, QString> map;
        value.value<QDBusArgument>() >> map;
        emit serviceNumbersChanged(map);
    } else if (property == "PinRequired") {	
        emit pinRequiredChanged(value.value<QString>());
    } else if (property == "LockedPins") {	
        emit lockedPinsChanged(value.value<QStringList>());
    } else if (property == "CardIdentifier") {	
        emit cardIdentifierChanged(value.value<QString>());
    } else if (property == "PreferredLanguages") {	
        emit preferredLanguagesChanged(value.value<QStringList>());
    }
}

void OfonoSimManager::setPropertyFailed(const QString& property)
{
    if (property == "SubscriberNumbers")
        emit setSubscriberNumbersFailed();
}

void OfonoSimManager::changePinResp()
{
    emit changePinComplete(TRUE);
}

void OfonoSimManager::changePinErr(QDBusError error)
{
    qDebug() << "ChangePin failed" << error;
    m_errorName = error.name();
    m_errorMessage = error.message();
    emit changePinComplete(FALSE);
}

void OfonoSimManager::enterPinResp()
{
    emit enterPinComplete(TRUE);
}

void OfonoSimManager::enterPinErr(QDBusError error)
{
    qDebug() << "EnterPin failed" << error;
    m_errorName = error.name();
    m_errorMessage = error.message();
    emit enterPinComplete(FALSE);
}

void OfonoSimManager::resetPinResp()
{
    emit resetPinComplete(TRUE);
}

void OfonoSimManager::resetPinErr(QDBusError error)
{
    qDebug() << "ResetPin failed" << error;
    m_errorName = error.name();
    m_errorMessage = error.message();
    emit resetPinComplete(FALSE);
}

void OfonoSimManager::lockPinResp()
{
    emit lockPinComplete(TRUE);
}
void OfonoSimManager::lockPinErr(QDBusError error)
{
    qDebug() << "LockPin failed" << error;
    m_errorName = error.name();
    m_errorMessage = error.message();
    emit lockPinComplete(FALSE);
}

void OfonoSimManager::unlockPinResp()
{
    emit unlockPinComplete(TRUE);
}
void OfonoSimManager::unlockPinErr(QDBusError error)
{
    qDebug() << "UnlockPin failed" << error;
    m_errorName = error.name();
    m_errorMessage = error.message();
    emit unlockPinComplete(FALSE);
}
