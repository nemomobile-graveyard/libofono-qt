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

#ifndef OFONOSUPPLEMENTARYSERVICES_H
#define OFONOSUPPLEMENTARYSERVICES_H

#include <QtCore/QObject>
#include <QStringList>
#include <QDBusError>
#include <QDBusVariant>
#include "ofonomodeminterface.h"
#include "libofono-qt_global.h"

//! This class is used to access oFono supplementary services API
/*!
 * The API is documented in
 * http://git.kernel.org/?p=network/ofono/ofono.git;a=blob_plain;f=doc/supplementaryservices-api.txt
 */
class OFONO_QT_EXPORT OfonoSupplementaryServices : public OfonoModemInterface
{
    Q_OBJECT

public:
    OfonoSupplementaryServices(OfonoModem::SelectionSetting modemSetting, const QString &modemPath, QObject *parent=0);
    ~OfonoSupplementaryServices();
    
    /* Properties */
    QString state() const;
    
    void initiate(const QString &command);
    void respond(const QString &reply);
    void cancel();
    
signals:
    void notificationReceived(const QString &message);
    void requestReceived(const QString &message);

    void stateChanged(const QString &state);

    void initiateUSSDComplete(const QString &ussdResp);
    void barringComplete(const QString &ssOp, const QString &cbService, const QVariantMap &cbMap);
    void forwardingComplete(const QString &ssOp, const QString &cfService, const QVariantMap &cfMap);
    void waitingComplete(const QString &ssOp, const QVariantMap &cwMap);
    void callingLinePresentationComplete(const QString &ssOp, const QString &status);
    void connectedLinePresentationComplete(const QString &ssOp, const QString &status);
    void callingLineRestrictionComplete(const QString &ssOp, const QString &status);
    void connectedLineRestrictionComplete(const QString &ssOp, const QString &status);
    void initiateFailed();
    void respondComplete(bool success, const QString &message);
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
