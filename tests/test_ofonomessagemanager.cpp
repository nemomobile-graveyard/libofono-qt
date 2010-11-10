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

    void serviceCenterAddressChanged(const QString numbers)
    {
	qDebug() << "serviceCenterAddressChanged" << numbers;
    }

    void setServiceCenterAddressFailed()
    {
	qDebug() << "serviceCenterAddressFailed";
    }

    void serviceCenterAddressComplete(bool success, const QString numbers)
    {
	qDebug() << "serviceCenterAddressComplete" << success << numbers;
	m_sca = numbers;
    }

    void validityChanged(bool validity)
    {
	qDebug() << "ValidityChanged" << validity;
    }

    void initTestCase()
    {
	m = new OfonoMessageManager(QString(), this);
	connect(m, SIGNAL(validityChanged(bool)), this, 
		SLOT(validityChanged(bool)));
	connect(m, SIGNAL(serviceCenterAddressChanged(QString)), 
		this, 
		SLOT(serviceCenterAddressChanged(QString)));
	connect(m, SIGNAL(setServiceCenterAddressFailed()), 
		this, 
		SLOT(setServiceCenterAddressFailed()));
	connect(m, SIGNAL(serviceCenterAddressComplete(bool, QString)), 
		this, 
		SLOT(serviceCenterAddressComplete(bool, QString)));
    }

    void testOfonoMessageManager()
    {
	qDebug() << "validity:" << m->isValid();
	if (m->isValid()) {
	    m->requestServiceCenterAddress();
	    QTest::qWait(2000);
	    if (m_sca.length() == 0)
	        return;
	    qDebug() << "setServiceCenterAddress(+123456)";
	    m->setServiceCenterAddress("+123456");
	    QTest::qWait(1000);
	    qDebug() << "setServiceCenterAddress(original)";
	    m->setServiceCenterAddress(m_sca);
	}
	QTest::qWait(1000);
    }


    void cleanupTestCase()
    {

    }


private:
    OfonoMessageManager *m;
    QString m_sca;
};

QTEST_MAIN(TestOfonoMessageManager)
#include "test_ofonomessagemanager.moc"
