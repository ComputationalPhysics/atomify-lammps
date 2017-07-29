// #include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QSurfaceFormat>
#include <QOpenGLContext>
#include <QQuickWindow>
#ifdef Q_OS_LINUX
#include <locale>
#endif

#include "mysimulator.h"
#include "highlighter.h"
#include "LammpsWrappers/cpcompute.h"
#include "LammpsWrappers/cpvariable.h"
#include "LammpsWrappers/nvt.h"
#include "scripthandler.h"
#include "LammpsWrappers/fixaveragetime.h"
#include "LammpsWrappers/fixhistogram.h"
#include "LammpsWrappers/rdf.h"
#include "LammpsWrappers/modifiers/modifiers.h"
#include "LammpsWrappers/atoms.h"
#include "LammpsWrappers/system.h"
#include "LammpsWrappers/data1d.h"
#include <vendor.h>

#include <vendor.h>

int main(int argc, char *argv[])
{
    qmlRegisterType<AtomifySimulator>("Atomify", 1, 0, "AtomifySimulator");
    qmlRegisterType<HighlighterWrapper>("Atomify", 1, 0, "Highlighter");
    qmlRegisterType<ScriptHandler>("Atomify", 1, 0, "ScriptHandler");
    qmlRegisterType<CPCompute>("Atomify", 1, 0, "Compute");
    qmlRegisterType<CPFix>("Atomify", 1, 0, "Fix");
    qmlRegisterType<CPVariable>("Atomify", 1, 0, "Variable");
    qmlRegisterType<Data1D>("Atomify", 1, 0, "Data1D");
    qmlRegisterType<NVT>("Atomify", 1, 0, "NVT");
    qmlRegisterType<FixAverageTime>("Atomify",1,0,"FixAverageTime");
    qmlRegisterType<FixHistogram>("Atomify", 1, 0, "FixHistogram");
    // qmlRegisterType<RDF>("Atomify", 1, 0, "RDF");
    qmlRegisterType<Atoms>("Atomify", 1, 0, "Atoms");
    qmlRegisterType<ColorModifier>("Atomify", 1, 0, "ColorModifier");
    qmlRegisterType<PeriodicImages>("Atomify", 1, 0, "PeriodicImages");
    qmlRegisterType<System>("Atomify", 1, 0, "System");

    // QGuiApplication app(argc, argv);
    QApplication app(argc, argv);
    app.setOrganizationName("Ovilab");
    app.setOrganizationDomain("ovilab");
    app.setApplicationName("Atomify");

    QQmlApplicationEngine engine;
    qpm::init(app, engine);

//    QSurfaceFormat format;
//    format.setMajorVersion(3);
//    format.setMinorVersion(0);
//    format.setRenderableType(QSurfaceFormat::OpenGLES);
//    format.setProfile(QSurfaceFormat::CoreProfile);
//    QSurfaceFormat::setDefaultFormat(format);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
#ifdef Q_OS_LINUX
    setlocale(LC_ALL, "C");
    setlocale(LC_NUMERIC, "C");
#endif

    for(QKeySequence k : QKeySequence::keyBindings(QKeySequence::FullScreen)) {
        qDebug() << "Use " << k.toString() << " to toggle fullscreen.";
    }

    return app.exec();
}
