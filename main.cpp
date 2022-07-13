#include "encrypterwindow.h"

#include <iostream>
//#include "collection.h"
//#include "encrypter.h"

#include <QApplication>
//#include <QLocale>
#include <QTranslator>
using namespace Encrypt;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("QtProject");
    QCoreApplication::setApplicationName("Encrypter");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    QCoreApplication::setOrganizationDomain("QtProject");
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Encrypter_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    EncrypterWindow w;
    w.show();
    return a.exec();
}
