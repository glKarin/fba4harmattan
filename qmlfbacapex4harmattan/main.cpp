#include <QtGui/QApplication>
#include <QDeclarativeComponent>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include "qmlapplicationviewer.h"
#include <QLocale>
#include <QTranslator>
#include <QFileInfo>
#include "QDebug"
#include "src/kzfbacapex.h"
#include "src/kfilebrowser.h"
#include "src/kfileinfo.h"

#define APPLICATION_NAME "QFbaCapex"
#define APPLICATION_VERSION "0.5.0harmattan1"
#define APPLICATION_DEVELOPER "Karin"

QString adjust_path(const QString &path)
{
#ifdef Q_OS_UNIX
#ifdef Q_OS_MAC
    if (!QDir::isAbsolutePath(path))
        return QString::fromLatin1("%1/../Resources/%2")
                .arg(QCoreApplication::applicationDirPath(), path);
#else
    const QString pathInInstallDir =
            QString::fromLatin1("%1/../%2").arg(QCoreApplication::applicationDirPath(), path);
    if (QFileInfo(pathInInstallDir).exists())
        return pathInInstallDir;
#endif
#endif
    return path;
}

Q_DECL_EXPORT int main(int argc, char *argv[])
{
		QDeclarativeContext *context;
        QApplication *a = createApplication(argc, argv);
				a->setApplicationName(APPLICATION_NAME);
				a->setOrganizationName(APPLICATION_DEVELOPER);
				a->setApplicationVersion(APPLICATION_VERSION);
        QString locale = QLocale::system().name();
        QTranslator translator;

        translator.load(QString("qmlfbacapex.") + locale, adjust_path("i18n/"));
        //qDebug()<<b;

        a->installTranslator(&translator);
        QScopedPointer<QApplication> app(a);


        qmlRegisterType<KFileInfo>("karin.file.browser",1,3,"KFileInfo");
        qmlRegisterType<KFileBrowser>("karin.file.browser",1,3,"KFileBrowser");
        qmlRegisterType<KZFbaCapex>("karin.fba.capex",1,2,"KZFbaCapex");
        qmlRegisterType<KZFbaRomInfo>("karin.fba.rominfo",1,2,"KZFbaRomInfo");

        QmlApplicationViewer viewer;

		context = viewer.engine()->rootContext();
		context->setContextProperty("_CPP_name", APPLICATION_NAME);
		context->setContextProperty("_CPP_dev", APPLICATION_DEVELOPER);
		context->setContextProperty("_CPP_version", APPLICATION_VERSION);
		context->setContextProperty("_CPP_tmo", "http://talk.maemo.org/member.php?u=70254");
		context->setContextProperty("_CPP_openrepos_fba", "https://openrepos.net/content/karinzhao/fba");
		context->setContextProperty("_CPP_openrepos", "https://openrepos.net/content/karinzhao/qfbacapex");
		context->setContextProperty("_CPP_github", "https://github.com/glKarin/fba4harmattan");

        viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
        viewer.setMainQmlFile(QLatin1String("qml/qmlfbacapex4harmattan/main.qml"));
        viewer.showExpanded();

        return app->exec();
}
