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

#ifndef OFONOCALLSETTINGS_H
#define OFONOCALLSETTINGS_H

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
#include "ofonomodeminterface.h"
#include "libofono-qt_global.h"

class OFONO_QT_EXPORT OfonoCallSettings : public OfonoModemInterface
{
    Q_OBJECT

public:
    OfonoCallSettings(QString modemId, QObject *parent=0);
    ~OfonoCallSettings();

    /* Properties */
    void requestCallingLinePresentation();
    void requestCalledLinePresentation();
    void requestCalledLineRestriction();
    void requestCallingLineRestriction();
    void requestHideCallerId();
    void setHideCallerId(QString setting);
    void requestVoiceCallWaiting();
    void setVoiceCallWaiting(QString setting);
    
signals:
    void callingLinePresentationComplete(bool success, QString setting);
    void calledLinePresentationComplete(bool success, QString setting);
    void calledLineRestrictionComplete(bool success, QString setting);
    void callingLineRestrictionComplete(bool success, QString setting);
    void hideCallerIdComplete(bool success, QString setting);
    void voiceCallWaitingComplete(bool success, QString setting);

    void callingLinePresentationChanged(QString setting);
    void calledLinePresentationChanged(QString setting);
    void calledLineRestrictionChanged(QString setting);
    void callingLineRestrictionChanged(QString setting);
    void hideCallerIdChanged(QString setting);
    void voiceCallWaitingChanged(QString setting);

    void setHideCallerIdFailed();
    void setVoiceCallWaitingFailed();

private slots:
    void propertyChanged(const QString& property, const QVariant& value);
    void setPropertyFailed(const QString& property);
    void requestPropertyComplete(bool success, const QString& property, const QVariant& value);      
};

#endif  /* !OFONOCALLSETTINGS_H */
