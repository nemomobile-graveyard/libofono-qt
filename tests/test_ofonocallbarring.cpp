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

#include <ofonocallbarring.h>

#include <QtDebug>


class TestOfonoCallBarring : public QObject
{
    Q_OBJECT

public slots:

    void validityChanged(bool validity)
    {
	qDebug() << "ValidityChanged" << validity;
    }

    void voiceIncomingComplete(bool success, QString barrings)
    {
	qDebug() << "voiceIncomingComplete" << success << barrings;
    }

    void voiceOutgoingComplete(bool success, QString barrings)
    {
	qDebug() << "voiceOutgoingComplete" << success << barrings;
    }

    void voiceIncomingChanged(QString barrings)
    {
	qDebug() << "voiceIncomingChanged" << barrings;
    }

    void voiceOutgoingChanged(QString barrings)
    {
	qDebug() << "voiceOutgoingChanged" << barrings;
    }

    void setVoiceIncomingFailed()
    {
	qDebug() << "setVoiceIncomingFailed" << m->errorName() << m->errorMessage();
    }

    void setVoiceOutgoingFailed()
    {
	qDebug() << "setVoiceOutgoingFailed" << m->errorName() << m->errorMessage();
    }

    void incomingBarringInEffect()
    {
	qDebug() << "incomingBarringInEffect signal";
    }

    void outgoingBarringInEffect()
    {
	qDebug() << "outgoingBarringInEffect signal";
    }


    void disableAllComplete(bool success)
    {
	qDebug() << "disableAllComplete:" << success;
    }

    void changePasswordComplete(bool success)
    {
	qDebug() << "changePasswordComplete:" << success;
    }

private slots:

    void initTestCase()
    {
	m = new OfonoCallBarring(OfonoModem::AutomaticSelect, QString(), this);
	connect(m, SIGNAL(validityChanged(bool)), this, 
		SLOT(validityChanged(bool)));
	connect(m, SIGNAL(voiceIncomingComplete(bool, QString)), this, 
		SLOT(voiceIncomingComplete(bool, QString)));
	connect(m, SIGNAL(voiceOutgoingComplete(bool, QString)), this, 
		SLOT(voiceOutgoingComplete(bool, QString)));
	connect(m, SIGNAL(voiceIncomingChanged(QString)), this, 
		SLOT(voiceIncomingChanged(QString)));
	connect(m, SIGNAL(voiceOutgoingChanged(QString)), this, 
		SLOT(voiceOutgoingChanged(QString)));
	connect(m, SIGNAL(setVoiceIncomingFailed()), this, 
		SLOT(setVoiceIncomingFailed()));
	connect(m, SIGNAL(setVoiceOutgoingFailed()), this, 
		SLOT(setVoiceOutgoingFailed()));
	connect(m, SIGNAL(incomingBarringInEffect()), this, 
		SLOT(incomingBarringInEffect()));
	connect(m, SIGNAL(outgoingBarringInEffect()), this, 
		SLOT(outgoingBarringInEffect()));
	// Wait for properties to arrive...
	QTest::qWait(1000);
    }

    void testOfonoCallbarring()
    {
	qDebug() << "validity:" << m->isValid();
	m->requestVoiceIncoming();
	QTest::qWait(5000);
	m->requestVoiceOutgoing();
	QTest::qWait(5000);

	qDebug() << "setVoiceIncoming(always)";
	m->setVoiceIncoming("always", "3579");
	QTest::qWait(1000);
	qDebug() << "setVoiceOutgoing(international)";
	m->setVoiceOutgoing("international", "3579");
	QTest::qWait(1000);

	connect(m, SIGNAL(disableAllComplete(bool)), 
		this, SLOT(disableAllComplete(bool)));
	m->requestDisableAll("3579");
	QTest::qWait(1000);
	m->requestVoiceIncoming();
	QTest::qWait(5000);
	m->requestVoiceOutgoing();
	QTest::qWait(5000);

	connect(m, SIGNAL(changePasswordComplete(bool)), 
		this, SLOT(changePasswordComplete(bool)));
	m->requestChangePassword("3579", "1234");
	QTest::qWait(1000);
	m->requestChangePassword("1234", "3579");

	QTest::qWait(2000);
    }


    void cleanupTestCase()
    {

    }


private:
    OfonoCallBarring *m;
};

QTEST_MAIN(TestOfonoCallBarring)
#include "test_ofonocallbarring.moc"
