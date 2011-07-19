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

#ifndef OFONOCALLBARRING_H
#define OFONOCALLBARRING_H

#include <QtCore/QObject>
#include <QDBusError>
#include "ofonomodeminterface.h"
#include "libofono-qt_global.h"

//! This class is used to access oFono call barring API
/*!
 * The API is documented in
 * http://git.kernel.org/?p=network/ofono/ofono.git;a=blob_plain;f=doc/call-barring-api.txt
 */
class OFONO_QT_EXPORT OfonoCallBarring : public OfonoModemInterface
{
    Q_OBJECT

public:
    OfonoCallBarring(OfonoModem::SelectionSetting modemSetting, const QString &modemPath, QObject *parent=0);
    ~OfonoCallBarring();

public slots:
    /* Properties */
    void requestVoiceIncoming();
    void setVoiceIncoming(const QString &barrings, const QString &password);
    void requestVoiceOutgoing();
    void setVoiceOutgoing(const QString &barrings, const QString &password);

    /* Methods */
    void changePassword(const QString &old_password, const QString &new_password);
    void disableAll(const QString &password);
    void disableAllIncoming(const QString &password);
    void disableAllOutgoing(const QString &password);

signals:
    void incomingBarringInEffect();
    void outgoingBarringInEffect();

    void voiceIncomingComplete(bool success, const QString &barrings);
    void voiceOutgoingComplete(bool success, const QString &barrings);
    void voiceIncomingChanged(const QString &barrings);
    void voiceOutgoingChanged(const QString &barrings);
    void setVoiceIncomingFailed();
    void setVoiceOutgoingFailed();

    void changePasswordComplete(bool success);
    void disableAllComplete(bool success);
    void disableAllIncomingComplete(bool success);
    void disableAllOutgoingComplete(bool success);

private slots:
    void pathChanged(const QString& path);
    void propertyChanged(const QString& property, const QVariant& value);
    void setPropertyFailed(const QString& property);
    void changePasswordResp();
    void changePasswordErr(QDBusError error);
    void disableAllResp();
    void disableAllErr(QDBusError error);
    void disableAllIncomingResp();
    void disableAllIncomingErr(QDBusError error);
    void disableAllOutgoingResp();
    void disableAllOutgoingErr(QDBusError error);
    void requestPropertyComplete(bool success, const QString& property, const QVariant& value);
private:
    void connectDbusSignals(const QString& path);
};

#endif  /* !OFONOCALLBARRING_H */
