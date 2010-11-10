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
 

#ifndef OFONONETWORKOPERATOR_H
#define OFONONETWORKOPERATOR_H

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
#include <QVariant>
#include <QDBusVariant>
#include <QStringList>
#include "ofonointerface.h"
#include "libofono-qt_global.h"

class OFONO_QT_EXPORT OfonoNetworkOperator : public OfonoInterface
{
    Q_OBJECT
public:
    OfonoNetworkOperator(const QString& operatorId, QObject *parent=0);
    ~OfonoNetworkOperator();

    void requestRegister();
    
    QString name();
    QString status();
    QString mcc();
    QString mnc();
    QStringList technologies();
    QString additionalInfo();
    
signals:
    void registerComplete(bool success);
    
    void nameChanged(QString name);
    void statusChanged(QString status);
    void mccChanged(QString mcc);
    void mncChanged(QString mnc);
    void technologiesChanged(QStringList technologies);
    void additionalInfoChanged(QString additionalInfo);

private:

private slots:
    void propertyChanged(const QString& property, const QVariant& value);
    
    void registerResp();
    void registerErr(const QDBusError& error);


private:


};
#endif
