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

#ifndef OFONOCALLFORWARDING_H
#define OFONOCALLFORWARDING_H
 
#include <QtCore/QObject>
#include "ofonomodeminterface.h"
#include "libofono-qt_global.h"

//! This class is used to access oFono call forwarding API
/*!
 * The API is documented in
 * http://git.kernel.org/?p=network/ofono/ofono.git;a=blob_plain;f=doc/call-forwarding-api.txt
 */
class OFONO_QT_EXPORT OfonoCallForwarding : public OfonoModemInterface
{
    Q_OBJECT

public:
    OfonoCallForwarding(OfonoModem::SelectionSetting modemSetting, const QString &modemPath, QObject *parent=0);
    ~OfonoCallForwarding();

    /* Properties */
    void requestVoiceUnconditional(); /* "" | phonenumber */
    void setVoiceUnconditional(const QString &property); 
    void requestVoiceBusy();
    void setVoiceBusy(const QString &property);
    void requestVoiceNoReply();
    void setVoiceNoReply(const QString &property);
    void requestVoiceNoReplyTimeout();
    void setVoiceNoReplyTimeout(ushort timeout);
    void requestVoiceNotReachable();
    void setVoiceNotReachable(const QString &property);
    
    /* Methods */
    void requestDisableAll(const QString &type); /* "all" | "conditional" */
    
signals:
    void voiceUnconditionalComplete(bool success, const QString &property);
    void voiceBusyComplete(bool success, const QString &property);
    void voiceNoReplyComplete(bool success, const QString &property);
    void voiceNoReplyTimeoutComplete(bool success, ushort timeout);
    void voiceNotReachableComplete(bool success, const QString &property);

    void voiceUnconditionalChanged(const QString &property);
    void voiceBusyChanged(const QString &property);
    void voiceNoReplyChanged(const QString &property);
    void voiceNoReplyTimeoutChanged(ushort timeout);
    void voiceNotReachableChanged(const QString &property);

    void setVoiceUnconditionalFailed();
    void setVoiceBusyFailed();
    void setVoiceNoReplyFailed();
    void setVoiceNoReplyTimeoutFailed();
    void setVoiceNotReachableFailed();

    void disableAllComplete(bool success); 

private slots:
    void propertyChanged(const QString& property, const QVariant& value);
    void setPropertyFailed(const QString& property);
    void requestPropertyComplete(bool success, const QString& property, const QVariant& value);
      
    void disableAllResp();
    void disableAllErr(QDBusError error);
};

#endif  /* !OFONOCALLFORWARDING_H */
