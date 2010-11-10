#include <QtTest/QtTest>
#include <QtCore/QObject>

#include <ofonomodemmanager.h>

#include <QtDebug>


class TestOfonoModemManager : public QObject
{
    Q_OBJECT

private slots:
    void modemAdded(QString modem)
    {
    qDebug() << "Modem added" << modem;
    }

    void modemRemoved(QString modem)
    {
    qDebug() << "Modem removed" << modem;
    }

    void initTestCase()
    {
	mm = new OfonoModemManager(this);
	connect(mm, SIGNAL(modemAdded(QString)), this, SLOT(modemAdded(QString)));
	connect(mm, SIGNAL(modemRemoved(QString)), this, SLOT(modemRemoved(QString)));
    }

    void testOfonoModemManager()
    {
    qDebug() << mm->modems();
    QTest::qWait(30000);
    }


    void cleanupTestCase()
    {

    }


private:
    OfonoModemManager *mm;
};

QTEST_MAIN(TestOfonoModemManager)
#include "test_ofonomodemmanager.moc"
