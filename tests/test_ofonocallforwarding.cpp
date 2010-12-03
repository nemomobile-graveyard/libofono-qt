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

#include <ofonocallforwarding.h>

#include <QtDebug>


class TestOfonoCallForwarding : public QObject
{
    Q_OBJECT

private slots:

    void validityChanged(bool validity)
    {
	qDebug() << "ValidityChanged" << validity;
    }
    void voiceUnconditionalComplete(bool success, QString property)
    {
	qDebug() << "voiceUnconditionalComplete" << success <<  property;
    }
    void voiceBusyComplete(bool success, QString property)
    {
	qDebug() << "voiceBusyComplete" << success <<  property;
    }
    void voiceNoReplyComplete(bool success, QString property)
    {
	qDebug() << "voiceNoReplyComplete" << success <<  property;
    }
    void voiceNoReplyTimeoutComplete(bool success, ushort timeout)
    {
	qDebug() << "voiceNoReplyTimeoutComplete" << success <<  timeout;
    }
    void voiceNotReachableComplete(bool success, QString property)
    {
	qDebug() << "voiceNotReachableComplete" << success <<  property;
    }

    void voiceUnconditionalChanged(QString property)
    {
	qDebug() << "voiceUnconditionalChanged" << property;
    }
    void voiceBusyChanged(QString property)
    {
	qDebug() << "voiceBusyChanged" << property;
    }
    void voiceNoReplyChanged(QString property)
    {
	qDebug() << "voiceNoReplyChanged" << property;
    }
    void voiceNoReplyTimeoutChanged(ushort timeout)
    {
	qDebug() << "voiceNoReplyTimeoutChanged" << timeout;
    }
    void voiceNotReachableChanged(QString property)
    {
	qDebug() << "voiceNotReachableChanged" << property;
    }

    void disableAllComplete(bool success)
    {
	qDebug() << "disableAllComplete:" << success;
    }


    void initTestCase()
    {
	m = new OfonoCallForwarding(OfonoModem::AutomaticSelect, QString(), this);
	connect(m, SIGNAL(validityChanged(bool)), this, 
		SLOT(validityChanged(bool)));
	connect(m, SIGNAL(voiceUnconditionalChanged(QString)), this, 
		SLOT(voiceUnconditionalChanged(QString)));
	connect(m, SIGNAL(voiceBusyChanged(QString)), this, 
		SLOT(voiceBusyChanged(QString)));
	connect(m, SIGNAL(voiceNoReplyChanged(QString)), this, 
		SLOT(voiceNoReplyChanged(QString)));
	connect(m, SIGNAL(voiceNoReplyTimeoutChanged(ushort)), this, 
		SLOT(voiceNoReplyTimeoutChanged(ushort)));
	connect(m, SIGNAL(voiceNotReachableChanged(QString)), this, 
		SLOT(voiceNotReachableChanged(QString)));
	connect(m, SIGNAL(voiceUnconditionalComplete(bool, QString)), this, 
		SLOT(voiceUnconditionalComplete(bool, QString)));
	connect(m, SIGNAL(voiceBusyComplete(bool, QString)), this, 
		SLOT(voiceBusyComplete(bool, QString)));
	connect(m, SIGNAL(voiceNoReplyComplete(bool, QString)), this, 
		SLOT(voiceNoReplyComplete(bool, QString)));
	connect(m, SIGNAL(voiceNoReplyTimeoutComplete(bool, ushort)), this, 
		SLOT(voiceNoReplyTimeoutComplete(bool, ushort)));
	connect(m, SIGNAL(voiceNotReachableComplete(bool, QString)), this, 
		SLOT(voiceNotReachableComplete(bool, QString)));
	//FIXME: should connect to setSomethingFailed signals too
    }

    void testOfonoCallForwarding()
    {
	qDebug() << "validity:" << m->isValid();
	m->requestVoiceUnconditional();
	QTest::qWait(1000);
	m->requestVoiceBusy();
	QTest::qWait(1000);
	m->requestVoiceNoReply();
	QTest::qWait(1000);
	m->requestVoiceNoReplyTimeout();
	QTest::qWait(1000);
	m->requestVoiceNotReachable();
	QTest::qWait(1000);

	qDebug() << "setVoiceBusy():";
	m->setVoiceBusy("12345678");
	qDebug() << "setVoiceNoReply():";
	m->setVoiceNoReply("12345678");
	qDebug() << "setVoiceNoReplyTimeout():";
	m->setVoiceNoReplyTimeout(30);
	qDebug() << "setVoiceNotReachable():";
	m->setVoiceNotReachable("12345678");
	qDebug() << "setVoiceUnconditional():";
	m->setVoiceUnconditional("12345678");
	QTest::qWait(1000);
	qDebug() << "setVoiceBusy():";
	m->setVoiceBusy("");
	qDebug() << "setVoiceNoReply():";
	m->setVoiceNoReply("");
	qDebug() << "setVoiceNotReachable():";
	m->setVoiceNotReachable("");
	qDebug() << "setVoiceUnconditional():";
	m->setVoiceUnconditional("");


	connect(m, SIGNAL(disableAllComplete(bool)), 
		this, SLOT(disableAllComplete(bool)));
	m->disableAll("all");

	QTest::qWait(120000);
    }


    void cleanupTestCase()
    {

    }


private:
    OfonoCallForwarding *m;
};

QTEST_MAIN(TestOfonoCallForwarding)
#include "test_ofonocallforwarding.moc"
