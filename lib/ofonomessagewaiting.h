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
 
#ifndef OFONOMESSAGEWAITING_H
#define OFONOMESSAGEWAITING_H

#include <QtCore/QObject>
#include "ofonomodeminterface.h"
#include "libofono-qt_global.h"

//! This class is used to access oFono message waiting API
/*!
 * This class is used to access oFono message waiting API
 * oFono message manager API is documented in
 * http://git.kernel.org/?p=network/ofono/ofono.git;a=blob_plain;f=doc/message-waiting-api.txt
 */
class OFONO_QT_EXPORT OfonoMessageWaiting : public OfonoModemInterface
{
    Q_OBJECT

public:
    OfonoMessageWaiting(OfonoModem::SelectionSetting modemSetting, const QString &modemPath, QObject *parent=0);
    ~OfonoMessageWaiting();

    bool voicemailWaiting() const;
    int voicemailMessageCount() const;
    QString voicemailMailboxNumber() const;
    void setVoicemailMailboxNumber(QString mailboxnumber);

signals:
    void voicemailWaitingChanged(bool waiting);
    void voicemailMessageCountChanged(int count);
    void voicemailMailboxNumberChanged(const QString &mailboxnumber);

    void setVoicemailMailboxNumberFailed();
    
private slots:
    void propertyChanged(const QString& property, const QVariant& value);
    void setPropertyFailed(const QString& property);    
};

#endif  /* !OFONOMESSAGEWAITING_H */
