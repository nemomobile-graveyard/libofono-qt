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

#include <QtDBus/QtDBus>
#include <QtCore/QObject>

#include "ofonophonebook.h"

#define IMPORT_TIMEOUT 300000

OfonoPhonebook::OfonoPhonebook(QString modemId, QObject *parent)
    : OfonoModemInterface(modemId, "org.ofono.Phonebook", OfonoInterface::GetAllOnFirstRequest, parent)
{

}

OfonoPhonebook::~OfonoPhonebook()
{
}

void OfonoPhonebook::requestImport()
{
    QDBusMessage request;

    request = QDBusMessage::createMethodCall("org.ofono",
					     path(), ifname(),
					     "Import");
    request.setArguments(QList<QVariant>());

    QDBusConnection::systemBus().callWithCallback(request, this,
					SLOT(importResp(QString)),
					SLOT(importErr(const QDBusError&)),
					IMPORT_TIMEOUT);
}

void OfonoPhonebook::importResp(QString entries)
{
    emit importComplete(true, entries);
}

void OfonoPhonebook::importErr(const QDBusError& error)
{
    m_errorName = error.name();
    m_errorMessage = error.message();

    emit importComplete(false, QString());
}

