#include <QtTest/QtTest>
#include <QtCore/QObject>

#include <ofonointerface.h>

#include <QtDebug>

class TestOfonoInterface : public QObject
{
    Q_OBJECT

private slots:

    void onPropertyChanged(const QString& name, const QVariant& property)
    {
    qDebug() << name << property;
    qDebug () << oi->properties();
    }

    void initTestCase()
    {
	oi = new OfonoInterface("/isimodem1", "org.ofono.Modem", OfonoInterface::GetAllOnStartup, this);
	connect(oi, SIGNAL(propertyChanged(const QString&, const QVariant&)), this, SLOT(onPropertyChanged(const QString&, const QVariant&)));
    }

    void testOfonoInterface()
    {
    oi->setProperty("Online", qVariantFromValue(true));
    QTest::qWait(30000);
//    qDebug() << oi->properties();
    }


    void cleanupTestCase()
    {

    }


private:
    OfonoInterface *oi;
};

QTEST_MAIN(TestOfonoInterface)
#include "test_ofonointerface.moc"
