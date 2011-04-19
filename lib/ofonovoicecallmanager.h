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

#ifndef OFONOVOICECALLMANAGER_H
#define OFONOVOICECALLMANAGER_H

#include <QtCore/QObject>
#include <QStringList>
#include <QDBusError>
#include <QDBusObjectPath>

#include "ofonomodeminterface.h"
#include "libofono-qt_global.h"

struct OfonoVoiceCallManagerStruct {
    QDBusObjectPath path;
    QVariantMap properties;
};
typedef QList<OfonoVoiceCallManagerStruct> OfonoVoiceCallManagerList;

Q_DECLARE_METATYPE(OfonoVoiceCallManagerStruct)
Q_DECLARE_METATYPE(OfonoVoiceCallManagerList)

//! This class is used to access oFono voice call manager API
/*!
 * The API is documented in
 * http://git.kernel.org/?p=network/ofono/ofono.git;a=blob_plain;f=doc/voicecallmanager-api.txt
 */
class OFONO_QT_EXPORT OfonoVoiceCallManager : public OfonoModemInterface
{
    Q_OBJECT

    Q_PROPERTY(QStringList emergencyNumbers READ emergencyNumbers NOTIFY emergencyNumbersChanged)

public:
    OfonoVoiceCallManager(OfonoModem::SelectionSetting modemSetting, const QString &modemPath, QObject *parent=0);
    ~OfonoVoiceCallManager();

    /* Properties */
    QStringList emergencyNumbers() const;
    QStringList getCalls() const;
    void dial(const QString &number, const QString &callerid_hide);
    void hangupAll();
    void sendTones(const QString &tonestring);
    void transfer();
    void swapCalls();
    void releaseAndAnswer();
    void holdAndAnswer();

signals:
    void emergencyNumbersChanged(const QStringList &numbers);
    void callAdded(const QString &call);
    void callRemoved(const QString &call);
    void dialComplete(const bool status);
    void hangupAllComplete(const bool status);
    void sendTonesComplete(const bool status);
    void transferComplete(const bool status);
    void swapCallsComplete(const bool status);
    void releaseAndAnswerComplete(const bool status);
    void holdAndAnswerComplete(const bool status);

private slots:
    void propertyChanged(const QString &property, const QVariant &value);
    void callAddedChanged(const QDBusObjectPath &call, const QVariantMap &properties);
    void callRemovedChanged(const QDBusObjectPath &call);
    void dialResp();
    void dialErr(const QDBusError &error);
    void hangupAllResp();
    void hangupAllErr(const QDBusError &error);
    void sendTonesResp();
    void sendTonesErr(const QDBusError &error);
    void transferResp();
    void transferErr(const QDBusError &error);
    void swapCallsResp();
    void swapCallsErr(const QDBusError &error);
    void releaseAndAnswerResp();
    void releaseAndAnswerErr(const QDBusError &error);
    void holdAndAnswerResp();
    void holdAndAnswerErr(const QDBusError &error);


private:
    QStringList m_calllist;
};

#endif  /* !OFONOVOICECALLMANAGER_H */
