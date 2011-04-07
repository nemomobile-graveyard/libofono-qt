/*
 * This file is part of ofono-qt
 *
 * Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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
#ifndef OFONOCONNMANCONTEXT_H
#define OFONOCONNMANCONTEXT_H

#include <QtCore/QObject>
#include <QVariant>
#include <QStringList>
#include <QDBusError>

#include "libofono-qt_global.h"

class OfonoInterface;

//! This class is used to access oFono connman context API
/*!
 * The API is documented in
 * http://git.kernel.org/?p=network/ofono/ofono.git;a=blob;f=doc/connman-api.txt
 */
class OFONO_QT_EXPORT OfonoConnmanContext : public QObject
{
    Q_OBJECT
public:
    OfonoConnmanContext(const QString &contextPath, QObject *parent=0);
    OfonoConnmanContext(const OfonoConnmanContext &op);
    ~OfonoConnmanContext();

    OfonoConnmanContext operator=(const OfonoConnmanContext &op);
    bool operator==(const OfonoConnmanContext &op);

    //! Returns the D-Bus object path of the voice call object
    QString path() const;

    //! Get the D-Bus error name of the last operation.
    /*!
     * Returns the D-Bus error name of the last operation (setting a property
     * or calling a method) if it has failed
     */
    QString errorName() const;

    //! Get the D-Bus error message of the last operation.
    /*!
     * Returns the D-Bus error message of the last operation (setting a property
     * or calling a method) if it has failed
     */
    QString errorMessage() const;
    /* Properties for context*/

    bool active() const;
    QString accessPointName() const;
    QString type() const;
    QString username() const;
    QString password() const;
    QString protocol() const;
    QString name() const;
    QString messageProxy() const;
    QString messageCenter() const;
    QVariantMap settings() const;

    void setActive(const bool);
    void setAccessPointName(const QString&);
    void setType(const QString&);
    void setUsername(const QString&);
    void setPassword(const QString&);
    void setProtocol(const QString&);
    void setName(const QString&);
    void setMessageProxy(const QString&);
    void setMessageCenter(const QString&);

signals:
    void activeChanged(const bool);
    void accessPointNameChanged(const QString &apn);
    void nameChanged(const QString &name);
    void typeChanged(const QString &type);
    void userNameChanged(const QString &uname);
    void passwordChanged(const QString &pw);
    void protocolChanged(const QString &proto);
    void messageProxyChanged(const QString &proxy);
    void messageCenterChanged(const QString &msc);

    void setActiveFailed();
    void setAccessPointNameFailed();
    void setTypeFailed();
    void setUsernameFailed();
    void setPasswordFailed();
    void setProtocolFailed();
    void setNameFailed();
    void setMessageProxyFailed();
    void setMessageCenterFailed();

    /* Settings change notification*/
    void settingsChanged(const QVariantMap&);

private slots:
    void propertyChanged(const QString &property, const QVariant &value);
    void setPropertyFailed(const QString& property);

private:
    OfonoInterface *m_if;

};

#endif //OFONOCONNMANCONTEXT_H
