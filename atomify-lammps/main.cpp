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
#include "atomstyle.h"
#include "fileio.h"
#include "LammpsWrappers/nvt.h"
#include "scripthandler.h"
#include "LammpsWrappers/fixaveragetime.h"
#include "LammpsWrappers/fixhistogram.h"
#include "QMLPlot/figure.h"
#include "QMLPlot/linegraph.h"

int main(int argc, char *argv[])
{
    qmlRegisterType<AtomifySimulator>("Atomify", 1, 0, "AtomifySimulator");
    qmlRegisterType<HighlighterWrapper>("Atomify", 1, 0, "Highlighter");
    qmlRegisterType<AtomStyle>("Atomify", 1, 0, "AtomStyle");
    qmlRegisterType<ScriptHandler>("Atomify", 1, 0, "ScriptHandler");
    qmlRegisterType<Figure>("QMLPlot", 1, 0, "Figure");
    qmlRegisterType<LineGraph>("QMLPlot", 1, 0, "LineGraph");
    qmlRegisterType<LineGraphDataSource>("QMLPlot", 1, 0, "LineGraphDataSource");
    qmlRegisterType<CPCompute>("Atomify", 1, 0, "Compute");
    qmlRegisterType<CPFix>("Atomify", 1, 0, "Fix");
    qmlRegisterType<CPVariable>("Atomify", 1, 0, "Variable");
    qmlRegisterType<NVT>("Atomify", 1, 0, "NVT");
    qmlRegisterType<FixAverageTime>("Atomify",1,0,"FixAverageTime");
    qmlRegisterType<FixHistogram>("Atomify", 1, 0, "FixHistogram");
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

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
