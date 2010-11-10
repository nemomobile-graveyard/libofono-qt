#include <QtTest/QtTest>
#include <QtCore/QObject>

#include <ofonomodem.h>

#include <QtDebug>

class TestOfonoModem : public QObject
{
    Q_OBJECT

private slots:
    void validityChanged(bool validity)
    {
    qDebug() << "ValidityChanged" << validity;
	qDebug() << "validity" << m->isValid() << "modemid" << m->modemId();
    }

    void modemIdChanged(QString modemId)
    {
    qDebug() << "ModemIdChanged" << modemId;
	qDebug() << "validity" << m->isValid() << "modemid" << m->modemId();
    }

    void poweredChanged(bool powered)
    {
    qDebug() << "PoweredChanged" << powered;
    }

    void onlineChanged(bool online)
    {
    qDebug() << "OnlineChanged" << online;
    }

    void nameChanged(QString name)
    {
    qDebug() << "nameChanged" << name;
    }
    
    void manufacturerChanged(QString manufacturer)
    {
    qDebug() << "manufacturerChanged" << manufacturer;
    }

    void modelChanged(QString model)
    {
    qDebug() << "modelChanged" << model;
    }

    void revisionChanged(QString revision)
    {
    qDebug() << "revisionChanged" << revision;
    }

    void serialChanged(QString serial)
    {
    qDebug() << "serialChanged" << serial;
    }

    void featuresChanged(QStringList features)
    {
    qDebug() << "FeaturesChanged" << features;
    }

    void interfacesChanged(QStringList interfaces)
    {
    qDebug() << "InterfacesChanged" << interfaces;
    }

    void initTestCase()
    {
	m = new OfonoModem("/isimodem0", this);
//	m = new OfonoModem(this);
	connect(m, SIGNAL(validityChanged(bool)), this, SLOT(validityChanged(bool)));
    connect(m, SIGNAL(modemIdChanged(QString)),
	    this, SLOT(modemIdChanged(QString)));
	connect(m, SIGNAL(poweredChanged(bool)), this, SLOT(poweredChanged(bool)));
 	connect(m, SIGNAL(onlineChanged(bool)), this, SLOT(onlineChanged(bool)));
   connect(m, SIGNAL(nameChanged(QString)),
	    this, SLOT(nameChanged(QString)));
    connect(m, SIGNAL(manufacturerChanged(QString)),
	    this, SLOT(manufacturerChanged(QString)));
    connect(m, SIGNAL(modelChanged(QString)),
	    this, SLOT(modelChanged(QString)));
    connect(m, SIGNAL(revisionChanged(QString)),
	    this, SLOT(revisionChanged(QString)));
    connect(m, SIGNAL(serialChanged(QString)),
	    this, SLOT(serialChanged(QString)));
	connect(m, SIGNAL(featuresChanged(QStringList)), this, SLOT(featuresChanged(QStringList)));
	connect(m, SIGNAL(interfacesChanged(QStringList)), this, SLOT(interfacesChanged(QStringList)));
    }

    void testOfonoModem()
    {
	qDebug() << "validity" << m->isValid() << "modemid" << m->modemId();
    qDebug() << m->powered() << m->online() << m->manufacturer() << m->model() << m->revision() << m->serial() << m->features() << m->interfaces();
    m->setOnline(true);
    QTest::qWait(120000);
    }


    void cleanupTestCase()
    {

    }


private:
    OfonoModem *m;
};

QTEST_MAIN(TestOfonoModem)
#include "test_ofonomodem.moc"
