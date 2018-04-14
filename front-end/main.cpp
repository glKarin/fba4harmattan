#include <QtGui/QApplication>
#include <QDeclarativeComponent>
#include "qmlapplicationviewer.h"
#include <QLocale>
#include <QTranslator>
#include "QDebug"
#include "src/kzfbacapex.h"
#include "src/kfilebrowser.h"
#include "src/kfileinfo.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
        QApplication *a = createApplication(argc, argv);
        QString locale = QLocale::system().name();
        QTranslator translator;

        translator.load(QString("qmlfbacapex.") + locale,"/opt/qmlfbacapex4harmattan/i18n/");
        //qDebug()<<b;

        a->installTranslator(&translator);
        QScopedPointer<QApplication> app(a);


        qmlRegisterType<KFileInfo>("karin.file.browser",1,3,"KFileBrowser");
        qmlRegisterType<KFileBrowser>("karin.file.browser",1,3,"KFileBrowser");
        qmlRegisterType<KZFbaCapex>("karin.fba.capex",1,2,"KZFbaCapex");
        qmlRegisterType<KZFbaRomInfo>("karin.fba.rominfo",1,2,"KZFbaRomInfo");

        QmlApplicationViewer viewer;
        viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
        viewer.setMainQmlFile(QLatin1String("qml/qmlfbacapex4harmattan/main.qml"));
        viewer.showExpanded();

        return app->exec();
}
