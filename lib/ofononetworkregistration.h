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

#ifndef OFONONETWORKREGISTRATION_H
#define OFONONETWORKREGISTRATION_H

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
#include <QStringList>
#include <QDBusError>
#include "ofonomodeminterface.h"
#include "libofono-qt_global.h"

struct OfonoOperatorStruct {
    QDBusObjectPath path;
    QVariantMap properties;
};
typedef QList<OfonoOperatorStruct> OfonoOperatorList;
Q_DECLARE_METATYPE(OfonoOperatorStruct)
Q_DECLARE_METATYPE(OfonoOperatorList)
// Marshall the ModemStruct data into a D-Bus argument
QDBusArgument &operator<<(QDBusArgument &argument, const OfonoOperatorStruct &modem);
    
// Retrieve the ModemStruct data from the D-Bus argument
const QDBusArgument &operator>>(const QDBusArgument &argument, OfonoOperatorStruct &modem);


class OFONO_QT_EXPORT OfonoNetworkRegistration : public OfonoModemInterface
{
    Q_OBJECT

public:
    OfonoNetworkRegistration(QString modemId, QObject *parent=0);
    ~OfonoNetworkRegistration();
    
    /* Properties */
    QString mode();
    QString status();
    uint locationAreaCode();
    uint cellId();
    QString mcc();
    QString mnc();
    QString technology();
    QString name();
    uint strength();
    QString baseStation();
    
    void requestRegister();
    void requestDeregister();
    void requestGetOperators();
    void requestScan();
    
signals:
    void modeChanged(QString mode);
    void statusChanged(QString status);
    void locationAreaCodeChanged(uint locationAreaCode);
    void cellIdChanged(uint cellId);
    void mccChanged(QString mcc);
    void mncChanged(QString mnc);
    void technologyChanged(QString technology);
    void nameChanged(QString name);
    void strengthChanged(uint strength);
    void baseStationChanged(QString baseStation);

    void registerComplete(bool success);
    void deregisterComplete(bool success);
    void getOperatorsComplete(bool success, QStringList operatorIds);
    void scanComplete(bool success, QStringList operatorIds);

private slots:
    void propertyChanged(const QString& property, const QVariant& value);
    void registerResp();
    void registerErr(QDBusError error);
    void deregisterResp();
    void deregisterErr(QDBusError error);
    void getOperatorsResp(OfonoOperatorList list);
    void getOperatorsErr(QDBusError error);
    void scanResp(OfonoOperatorList list);
    void scanErr(QDBusError error);

private:

};

#endif  /* !OFONONETWORKREGISTRATION_H */
