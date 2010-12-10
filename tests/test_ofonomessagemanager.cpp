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

#include <ofonomessagemanager.h>

#include <QtDebug>


class TestOfonoMessageManager : public QObject
{
    Q_OBJECT

private slots:

    void initTestCase()
    {
	m = new OfonoMessageManager(OfonoModem::ManualSelect, "/phonesim", this);
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

    void testOfonoMessageManagerSca()
    {
        QSignalSpy scaComplete(m, SIGNAL(serviceCenterAddressComplete(bool, QString)));
        m->requestServiceCenterAddress();
        for (int i=0; i<30; i++) {
            if (scaComplete.count() > 0)
                break;
            QTest::qWait(1000);
        }
	QCOMPARE(scaComplete.count(), 1);
	QVariantList params = scaComplete.takeFirst();
	QCOMPARE(params.at(0).toBool(), true);
	QString sca = params.at(1).toString();
        QVERIFY(sca.length() > 0);
        qDebug() << sca;

        QSignalSpy setScaFailed(m, SIGNAL(setServiceCenterAddressFailed()));
        QSignalSpy scaChanged(m, SIGNAL(serviceCenterAddressChanged(QString)));

        m->setServiceCenterAddress("+12345678");
        for (int i=0; i<30; i++) {
            if (setScaFailed.count() > 0 || scaChanged.count() > 0)
                break;
            QTest::qWait(1000);
        }
	QCOMPARE(setScaFailed.count(), 0);
	QCOMPARE(scaChanged.count(), 1);
	QCOMPARE(scaChanged.takeFirst().at(0).toString(), QString("+12345678"));

        m->setServiceCenterAddress("");
        for (int i=0; i<30; i++) {
            if (setScaFailed.count() > 0 || scaChanged.count() > 0)
                break;
            QTest::qWait(1000);
        }
	QCOMPARE(setScaFailed.count(), 1);
	setScaFailed.takeFirst();
	QCOMPARE(scaChanged.count(), 0);
	QCOMPARE(m->errorName(), QString("org.ofono.Error.InvalidFormat"));
	QCOMPARE(m->errorMessage(), QString("Argument format is not recognized"));
	
	m->setServiceCenterAddress(sca);
        for (int i=0; i<30; i++) {
            if (setScaFailed.count() > 0 || scaChanged.count() > 0)
                break;
            QTest::qWait(1000);
        }
	QCOMPARE(setScaFailed.count(), 0);
	QCOMPARE(scaChanged.count(), 1);
	QCOMPARE(scaChanged.takeFirst().at(0).toString(), sca);
    }


    void cleanupTestCase()
    {

    }


private:
    OfonoMessageManager *m;
};

QTEST_MAIN(TestOfonoMessageManager)
#include "test_ofonomessagemanager.moc"
