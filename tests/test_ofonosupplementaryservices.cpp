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

#include <ofonosupplementaryservices.h>

#include <QtDebug>


class TestOfonoSupplementaryServices : public QObject
{
    Q_OBJECT

public slots:

    void stateChanged(QString state)
    {
	qDebug() << "stateChanged" << state;
    }
    
    void initiateFailed()
    {
    qDebug() << "initiateFailed" << m->errorName() << m->errorMessage();
    }
    
    void barringComplete(QString ssOp, QString cbService, QVariantMap cbMap)
    {
    qDebug() << "barringComplete" << ssOp << cbService << cbMap;
    }

    void forwardingComplete(QString ssOp, QString cfService, QVariantMap cfMap)
    {
    qDebug() << "forwardingComplete" << ssOp << cfService << cfMap;
    }

private slots:
    void initTestCase()
    {
	m = new OfonoSupplementaryServices(QString(), this);
	connect(m, SIGNAL(stateChanged(QString)), 
		this, SLOT(stateChanged(QString)));
	connect(m, SIGNAL(initiateFailed()), 
		this, SLOT(initiateFailed()));
	connect(m, SIGNAL(barringComplete(QString, QString, QVariantMap)), 
		this, SLOT(barringComplete(QString, QString, QVariantMap)));
	connect(m, SIGNAL(forwardingComplete(QString, QString, QVariantMap)), 
		this, SLOT(forwardingComplete(QString, QString, QVariantMap)));
    }

    void testOfonoNetworkOperator()
    {
	qDebug() << "state():" << m->state();
	m->requestInitiate("*#002**11#");

	QTest::qWait(120000);
    }


    void cleanupTestCase()
    {

    }


private:
    OfonoSupplementaryServices *m;
};

QTEST_MAIN(TestOfonoSupplementaryServices)
#include "test_ofonosupplementaryservices.moc"
