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

#include <QtTest/QtTest>
#include <QtCore/QObject>

#include <ofonomessagewaiting.h>

#include <QtDebug>

class TestOfonoMessageWaiting : public QObject
{
    Q_OBJECT

private slots:

    void voicemailWaitingChanged(bool waiting)
    {
	qDebug() << "voicemailWaitingChanged" << waiting;
    }

    void voicemailMessageCountChanged(int count)
    {
	qDebug() << "voicemailMessageCountChanged" << count;
    }

    void voicemailMailboxNumberChanged(QString mailboxnumber)
    {
	qDebug() << "voicemailMailboxNumberChanged" << mailboxnumber;
    }

    void validityChanged(bool validity)
    {
	qDebug() << "ValidityChanged" << validity;
    }

    void initTestCase()
    {
	m = new OfonoMessageWaiting(OfonoModem::AutomaticSelect, QString(), this);
	connect(m, SIGNAL(validityChanged(bool)), this, 
		SLOT(validityChanged(bool)));
	connect(m, SIGNAL(voicemailWaitingChanged(bool)), this, 
		SLOT(voicemailWaitingChanged(bool)));
	connect(m, SIGNAL(voicemailMessageCountChanged(int)), this, 
		SLOT(voicemailMessageCountChanged(int)));
	connect(m, SIGNAL(voicemailMailboxNumberChanged(QString)), this, 
		SLOT(voicemailMailboxNumberChanged(QString)));
    }

    void testOfonoMessageWaiting()
    {
	qDebug() << "validity:" << m->isValid();
	qDebug() << "voicemailWaiting():" << m->voicemailWaiting();
	qDebug() << "voicemailMessageCount():" << m->voicemailMessageCount();
	qDebug() << "voicemailMailboxNumber():" << m->voicemailMailboxNumber();
	QTest::qWait(120000);
    }


    void cleanupTestCase()
    {

    }


private:
    OfonoMessageWaiting *m;
};

QTEST_MAIN(TestOfonoMessageWaiting)
#include "test_ofonomessagewaiting.moc"
