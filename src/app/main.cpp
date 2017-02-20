#include <QString>
#include <QStringList>
#include <QUrl>
#include <QJsonObject>
#include <QCommandLineParser>
#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "qtlib_config.h"

#include "handlers/ocsurlhandler.h"

int main(int argc, char *argv[])
{
    // Init
    QGuiApplication app(argc, argv);

    qtlib::Config config(":/configs");
    QJsonObject configApplication = config.get("application");

    app.setApplicationName(configApplication["name"].toString());
    app.setApplicationVersion(configApplication["version"].toString());
    app.setOrganizationName(configApplication["organization"].toString());
    app.setOrganizationDomain(configApplication["domain"].toString());
    app.setWindowIcon(QIcon::fromTheme(configApplication["id"].toString(), QIcon(configApplication["icon"].toString())));

    // Setup CLI
    QCommandLineParser clParser;
    clParser.setApplicationDescription(configApplication["description"].toString());
    clParser.addHelpOption();
    clParser.addVersionOption();
    clParser.addPositionalArgument("ocsurl", "OCS-URL");
    clParser.process(app);

    QStringList args = clParser.positionalArguments();

    if (args.size() != 1) {
        clParser.showHelp(1);
    }

    QString ocsUrl = args.at(0);

    // Setup QML
    QQmlApplicationEngine qmlAppEngine;
    QQmlContext *qmlContext = qmlAppEngine.rootContext();
    qmlContext->setContextProperty("ocsUrlHandler", new OcsUrlHandler(ocsUrl, config, &qmlAppEngine));
    qmlAppEngine.load(QUrl("qrc:/qml/main.qml"));

    return app.exec();
}
