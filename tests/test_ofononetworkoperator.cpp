#include <QtTest/QtTest>
#include <QtCore/QObject>

#include <ofononetworkoperator.h>

#include <QtDebug>


class TestOfonoNetworkOperator : public QObject
{
    Q_OBJECT

private slots:

    void statusChanged(QString status)
    {
	qDebug() << "statusChanged" << status;
    }

    void registerComplete(bool success)
    {
	qDebug() << "registerComplete:" << success;
    }

    void initTestCase()
    {
	m = new OfonoNetworkOperator("/phonesim/operator/23406", this);
	connect(m, SIGNAL(registerComplete(bool)), this, 
		SLOT(registerComplete(bool)));
	connect(m, SIGNAL(statusChanged(QString)), 
		this, 
		SLOT(statusChanged(QString)));
    }

    void testOfonoNetworkOperator()
    {
	qDebug() << "name():" << m->name();
	qDebug() << "status():" << m->status();
	qDebug() << "mobileCountryCode():" << m->mcc();
	qDebug() << "mobileNetworkCode():" << m->mnc();
	qDebug() << "technologies():" << m->technologies();
	qDebug() << "additionalInfo():" << m->additionalInfo();
	m->requestRegister();

	QTest::qWait(120000);
    }


    void cleanupTestCase()
    {

    }


private:
    OfonoNetworkOperator *m;
};

QTEST_MAIN(TestOfonoNetworkOperator)
#include "test_ofononetworkoperator.moc"
