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

#include <ofonovoicecallmanager.h>

#include <QtDebug>


class TestOfonoVoiceCallManager : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase()
    {
	m = new OfonoVoiceCallManager(OfonoModem::ManualSelect, "/phonesim", this);
	QCOMPARE(m->modem()->isValid(), true);	

	if (!m->modem()->powered()) {
  	    m->modem()->setPowered(true);
            QTest::qWait(5000);
        }
        if (!m->modem()->online()) {
  	    m->modem()->setOnline(true);
            QTest::qWait(5000);
        }
	QCOMPARE(m->isValid(), true);
    }

    void testOfonoVoiceCallManager()
    {
    	QVERIFY(m->emergencyNumbers().count() > 0);

	QSignalSpy emergencyNumbers(m, SIGNAL(emergencyNumbersChanged(QStringList)));
	m->modem()->setPowered(false);
        QTest::qWait(5000);
	m->modem()->setPowered(true);
        QTest::qWait(5000);
  	m->modem()->setOnline(true);
        QTest::qWait(5000);
        QCOMPARE(emergencyNumbers.count(), 1);
        QVERIFY(emergencyNumbers.takeFirst().at(0).toStringList().count() > 0);
    }


    void cleanupTestCase()
    {

    }


private:
    OfonoVoiceCallManager *m;
};

QTEST_MAIN(TestOfonoVoiceCallManager)
#include "test_ofonovoicecallmanager.moc"
