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
