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
