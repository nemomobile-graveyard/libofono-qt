#include <QtTest/QtTest>
#include <QtCore/QObject>

#include <ofonophonebook.h>

#include <QtDebug>

class TestOfonoPhonebook : public QObject
{
    Q_OBJECT

private slots:
    void validityChanged(bool validity)
    {
    qDebug() << "ValidityChanged" << validity;
    }

    void importComplete(bool success, QString entries)
    {
    qDebug() << "importComplete" << success << entries;
    }

    void initTestCase()
    {
//	m = new OfonoPhonebook("/isimodem0", this);
	m = new OfonoPhonebook(QString(), this);
	connect(m, SIGNAL(validityChanged(bool)), this, SLOT(validityChanged(bool)));
    connect(m, SIGNAL(importComplete(bool, QString)),
	    this, SLOT(importComplete(bool, QString)));
    }

    void testOfonoPhonebook()
    {
	qDebug() << "validity" << m->isValid();
	m->requestImport();
    QTest::qWait(120000);
    }


    void cleanupTestCase()
    {

    }


private:
    OfonoPhonebook *m;
};

QTEST_MAIN(TestOfonoPhonebook)
#include "test_ofonophonebook.moc"
