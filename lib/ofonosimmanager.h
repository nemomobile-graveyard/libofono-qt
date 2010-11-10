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

#ifndef OFONOSIMMANAGER_H
#define OFONOSIMMANAGER_H

#include <QtCore/QObject>
#include <QStringList>
#include <QDBusError>
#include "ofonomodeminterface.h"
#include "libofono-qt_global.h"

class OFONO_QT_EXPORT OfonoSimManager : public OfonoModemInterface
{
    Q_OBJECT

public:
    OfonoSimManager(QString modemId, QObject *parent=0);
    ~OfonoSimManager();

    /* Properties */
    bool present();
    QString subscriberIdentity();
    QString mobileCountryCode();
    QString mobileNetworkCode();
    QStringList subscriberNumbers();
    QMap<QString, QString> serviceNumbers();
    QString pinRequired();
    QStringList lockedPins();
    QString cardIdentifier();
    QStringList preferredLanguages();

    void requestChangePin(QString pintype, QString oldpin, QString newpin);
    void requestEnterPin(QString pintype, QString pin);
    void requestResetPin(QString pintype, QString puk, QString newpin);
    void requestLockPin(QString pintype, QString pin);
    void requestUnlockPin(QString pintype, QString pin);

    void setSubscriberNumbers(QStringList numbers);

signals:
    void presenceChanged(bool ispresent);
    void subscriberIdentityChanged(QString imsi);
    void mobileCountryCodeChanged(QString mcc);
    void mobileNetworkCodeChanged(QString mnc);
    void subscriberNumbersChanged(QStringList msisdns);
    void setSubscriberNumbersFailed();
    void serviceNumbersChanged(QMap<QString, QString> sdns);
    void pinRequiredChanged(QString pintype);
    void lockedPinsChanged(QStringList pins);
    void cardIdentifierChanged(QString iccid);
    void preferredLanguagesChanged(QStringList languages);

    void changePinComplete(bool success);
    void enterPinComplete(bool success);
    void resetPinComplete(bool success);
    void lockPinComplete(bool success);
    void unlockPinComplete(bool success);


private slots:
    void propertyChanged(const QString& property, const QVariant& value);
    void setPropertyFailed(const QString& property);    

    void changePinResp();
    void changePinErr(QDBusError error);
    void enterPinResp();
    void enterPinErr(QDBusError error);
    void resetPinResp();
    void resetPinErr(QDBusError error);
    void lockPinResp();
    void lockPinErr(QDBusError error);
    void unlockPinResp();
    void unlockPinErr(QDBusError error);

private:

};

#endif  /* !OFONOSIMMANAGER_H */
