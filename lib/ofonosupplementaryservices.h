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

#ifndef OFONOSUPPLEMENTARYSERVICES_H
#define OFONOSUPPLEMENTARYSERVICES_H

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
#include <QStringList>
#include <QDBusError>
#include "ofonomodeminterface.h"
#include "libofono-qt_global.h"

class OFONO_QT_EXPORT OfonoSupplementaryServices : public OfonoModemInterface
{
    Q_OBJECT

public:
    OfonoSupplementaryServices(QString modemId, QObject *parent=0);
    ~OfonoSupplementaryServices();
    
    /* Properties */
    QString state();
    
    void requestInitiate(QString command);
    void requestRespond(QString reply);
    void requestCancel();
    
signals:
    void notificationReceived(QString message);
    void requestReceived(QString message);

    void stateChanged(QString state);

    void initiateUSSDComplete(QString ussdResp);
    void barringComplete(QString ssOp, QString cbService, QVariantMap cbMap);
    void forwardingComplete(QString ssOp, QString cfService, QVariantMap cfMap);
    void waitingComplete(QString ssOp, QVariantMap cwMap);
    void callingLinePresentationComplete(QString ssOp, QString status);
    void calledLinePresentationComplete(QString ssOp, QString status);
    void callingLineRestrictionComplete(QString ssOp, QString status);
    void calledLineRestrictionComplete(QString ssOp, QString status);
    void initiateFailed();
    void respondComplete(bool success, QString message);
    void cancelComplete(bool success);
    
private slots:
    void propertyChanged(const QString& property, const QVariant& value);
    void initiateResp(QString message, QDBusVariant details);
    void initiateErr(QDBusError error);
    void respondResp(QString message);
    void respondErr(QDBusError error);
    void cancelResp();
    void cancelErr(QDBusError error);

private:

};

#endif  /* !OFONOSUPPLEMETARYSERVICES_H */
