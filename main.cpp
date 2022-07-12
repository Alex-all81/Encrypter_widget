#include "encrypterwindow.h"

#include <iostream>
#include "collection.h"
#include "encrypter.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
using namespace Encrypt;
int main(int argc, char *argv[])
{
   /* QString source="A QVariant containing a pointer to a type derived ." ;//≈сли ключ не может быть найден в первом месте, поиск продолжаетс€ во втором месте и так далее.";
    QString result;
    QString preset = "Cesar classic";
    Collection *col=Collection::getCollection();
    col->init();
    Diapazones *diap=col->getDiapazones();
    diap->push_back({getCode('A'),getCode('Z')});
    diap->push_back({getCode('a'),getCode('z')});
    Collection::Presets *set = col->getPresets();
    if(preset.isEmpty() || !set || set->find(preset)==set->end())
        return 1;

    QString res;
    std::cout<<" sizes: "<<source.size()<<' '<<res.size()<<std::endl;
    std::cout<<" cap: "<<source.capacity()<<' '<<res.capacity()<<std::endl;
    //res.reserve(doc->toRawText().size());
    set->operator[](preset)->operator()(source,res);
    std::cout<<res.toStdString()<<std::endl;
    std::cout<<" sizes: "<<source.size()<<' '<<res.size()<<std::endl;
    std::cout<<" cap: "<<source.capacity()<<' '<<res.capacity()<<std::endl;*/
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
