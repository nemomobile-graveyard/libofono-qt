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

#ifndef OFONOCALLBARRING_H
#define OFONOCALLBARRING_H

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
#include "ofonomodeminterface.h"
#include "libofono-qt_global.h"

class OFONO_QT_EXPORT OfonoCallBarring : public OfonoModemInterface
{
    Q_OBJECT

public:
    OfonoCallBarring(QString modemId, QObject *parent=0);
    ~OfonoCallBarring();

    /* Properties */
    void requestVoiceIncoming();
    void setVoiceIncoming(QString barrings, QString password);
    void requestVoiceOutgoing();
    void setVoiceOutgoing(QString barrings, QString password);

    /* Methods */
    void requestChangePassword(QString old_password, QString new_password);
    void requestDisableAll(QString password);
    void requestDisableAllIncoming(QString password);
    void requestDisableAllOutgoing(QString password);

signals:
    void incomingBarringInEffect();
    void outgoingBarringInEffect();

    void voiceIncomingComplete(bool success, QString barrings);
    void voiceOutgoingComplete(bool success, QString barrings);
    void voiceIncomingChanged(QString barrings);
    void voiceOutgoingChanged(QString barrings);
    void setVoiceIncomingFailed();
    void setVoiceOutgoingFailed();

    void changePasswordComplete(bool success);
    void disableAllComplete(bool success);
    void disableAllIncomingComplete(bool success);
    void disableAllOutgoingComplete(bool success);

private slots:
    void propertyChanged(const QString& property, const QVariant& value);
    void setPropertyFailed(const QString& property);
    void setPropertyResp();
    void setPropertyErr(const QDBusError& error);
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
    void setProperty(const QString& name, const QVariant& property,
			const QString& password);

   QString m_pendingProperty;
};

#endif  /* !OFONOCALLBARRING_H */
