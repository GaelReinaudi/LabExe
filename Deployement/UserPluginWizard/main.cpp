#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>

#include "UserPluginWizard.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(UserPluginWizard);
    
    QApplication app(argc, argv);

    QString translatorFileName = QLatin1String("qt_");
    translatorFileName += QLocale::system().name();
    QTranslator *translator = new QTranslator(&app);
    if (translator->load(translatorFileName, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        app.installTranslator(translator);

    UserPluginWizard wizard;
    wizard.show();
    return app.exec();
}
