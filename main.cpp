#include <QtCore>
#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>

#include "progressform.h"
#include "raytracer.h"
using namespace std;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("Simple multi-thread raytracer with octrees optimizations support.");
    parser.addHelpOption();

    parser.addPositionalArgument("filename", "File with scene description");

    QCommandLineOption threadsOption(QStringList() << "t" << "threads",
                                     "Number of threads to use (default is number of CPU cores. Sometimes greater number gives better performance).",
                                     "threads", "0");
    parser.addOption(threadsOption);

    QCommandLineOption childrenOption(QStringList() << "c" << "children",
                                      "Minimal number of children to prevent octree node from branching further (default is 8).",
                                      "children", "8");
    parser.addOption(childrenOption);

    QCommandLineOption depthOption(QStringList() << "d" << "depth",
                                   "Maximal depth of octree (default is 8. Should be adapted for each separate case).",
                                   "depth", "8");
    parser.addOption(depthOption);

    parser.process(app);
    const QStringList args = parser.positionalArguments();
    if (args.size() != 1) {
        fprintf(stderr, "%s\n", "Error: Must specify one filename argument.");
        parser.showHelp(1);
        return 1;
    }

    RayTracer *raytracer = new RayTracer(args.at(0).toStdString().c_str(),
                                         parser.value(threadsOption).toInt(),
                                         parser.value(childrenOption).toInt(),
                                         parser.value(depthOption).toInt());

    ProgressForm *form = new ProgressForm();

    QObject::connect(raytracer, SIGNAL(started()), form, SLOT(show()));
    QObject::connect(raytracer, SIGNAL(progressUpdated(float)), form, SLOT(updateProgress(float)));
    QObject::connect(raytracer, SIGNAL(finished()), form, SLOT(finish()));

    raytracer->start();

    return app.exec();
}
