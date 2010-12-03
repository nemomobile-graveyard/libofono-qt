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

//! This class is used to access oFono SIM API
/*!
 * The API is documented in
 * http://git.kernel.org/?p=network/ofono/ofono.git;a=blob_plain;f=doc/sim-api.txt
 */
class OFONO_QT_EXPORT OfonoSimManager : public OfonoModemInterface
{
    Q_OBJECT

public:
    OfonoSimManager(OfonoModem::SelectionSetting modemSetting, const QString &modemPath, QObject *parent=0);
    ~OfonoSimManager();

    /* Properties */
    bool present() const;
    QString subscriberIdentity() const;
    QString mobileCountryCode() const;
    QString mobileNetworkCode() const;
    QStringList subscriberNumbers() const;
    QMap<QString, QString> serviceNumbers() const;
    QString pinRequired() const;
    QStringList lockedPins() const;
    QString cardIdentifier() const;
    QStringList preferredLanguages() const;

    void changePin(const QString &pintype, const QString &oldpin, const QString &newpin);
    void enterPin(const QString &pintype, const QString &pin);
    void resetPin(const QString &pintype, const QString &puk, const QString &newpin);
    void lockPin(const QString &pintype, const QString &pin);
    void unlockPin(const QString &pintype, const QString &pin);

    void setSubscriberNumbers(const QStringList &numbers);

signals:
    void presenceChanged(bool ispresent);
    void subscriberIdentityChanged(const QString &imsi);
    void mobileCountryCodeChanged(const QString &mcc);
    void mobileNetworkCodeChanged(const QString &mnc);
    void subscriberNumbersChanged(const QStringList &msisdns);
    void setSubscriberNumbersFailed();
    void serviceNumbersChanged(const QMap<QString, QString> &sdns);
    void pinRequiredChanged(const QString &pintype);
    void lockedPinsChanged(const QStringList &pins);
    void cardIdentifierChanged(const QString &iccid);
    void preferredLanguagesChanged(const QStringList &languages);

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
