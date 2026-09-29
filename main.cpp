#include <QApplication>
#include <QPushButton>
#include <QMainWindow>
#include "TerminalWidget.h"
#include "AIWidget.h"
#include <QFrame>
#include <QLabel>
#include "benchmarkImplem.h"
#include <QtConcurrent/QtConcurrent>
#include "systMon.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QMainWindow window;

    // Main background window
    QWidget *centralWidget = new QWidget;
    centralWidget->setStyleSheet("background-color: #D3D3D3");
    window.setCentralWidget(centralWidget);

    // Terminal button
    QPushButton *terminalButton = new QPushButton(" Terminal ", centralWidget);
    terminalButton->setGeometry(QRect(30, 180, 90, 38));
    terminalButton->setStyleSheet(
    "QPushButton {"
    "    background-color: #6D9696;"
    "    color: white;"
    "    font-size: 13px;"
    "    font-weight: bold;"
    "    border: 1px;"
    "    border-radius: 5px;"
    "}"
    "QPushButton:hover {"
    "    background-color: #6D8196;"
    "}"
    "QPushButton:pressed {"
    "    background-color: #555555;"
    "}"
);
    QObject::connect(terminalButton, &QPushButton::clicked, []() {
        TerminalWidget *terminal = new TerminalWidget();
        terminal->resize(800, 480);
        terminal->setWindowTitle("MEDUSA Terminal");
        terminal->show();
        terminal->setAttribute(Qt::WA_DeleteOnClose);
    });

    // AI chatbot button
    QPushButton *aiButton = new QPushButton(" AI chatbot ", centralWidget);
    aiButton->setGeometry(QRect(30, 240, 90, 38));
    aiButton->setStyleSheet(
    "QPushButton {"
    "    background-color: #6D9696;"
    "    color: white;"
    "    font-size: 13px;"
    "    font-weight: bold;"
    "    border: 1px;"
    "    border-radius: 5px;"
    "}"
    "QPushButton:hover {"
    "    background-color: #6D8196;"
    "}"
    "QPushButton:pressed {"
    "    background-color: #555555;"
    "}"
);
    QObject::connect(aiButton, &QPushButton::clicked, []() {
        AIWidget *aiWidget = new AIWidget();
        aiWidget->resize(800, 480);
        aiWidget->setWindowTitle("AI chatbot");
        aiWidget->show();
        aiWidget->setAttribute(Qt::WA_DeleteOnClose);
    });


    //logo
    QLabel *labelLogo = new QLabel("MEDUSA OS", centralWidget);
    labelLogo->move(300, 170);
    QFont font("Arial", 40, QFont::Bold);
    labelLogo->setFont(font);
    labelLogo->setStyleSheet("color: #6D6D96;");
    labelLogo->adjustSize();
    labelLogo->raise();

    //systInfo
    QFrame *infoframe = new QFrame(centralWidget);
    infoframe->setGeometry(530, 340, 360, 160);
    infoframe->setFrameShape(QFrame::Box);
    infoframe->setLineWidth(2);
    QLabel *infoLabel = new QLabel(infoframe);
    infoLabel->setGeometry(10, 10, 340, 140);
    infoLabel->setStyleSheet("font-size: 13px; font-family: monospace;");

    static SystemMonitoring monitor;

    // QTimer setup
    QTimer *timer = new QTimer(centralWidget);

    // update
    auto updateStats = [infoLabel]() {
        CPUStats cpu = monitor.getCPUStats();
        MemStats mem = monitor.getMemStats();

        double totalGB = mem.total / (1024.0 * 1024.0);
        double usedGB = mem.used / (1024.0 * 1024.0);

        QString tempStr = cpu.temperature.has_value()
            ? QString::number(cpu.temperature.value(), 'f', 1) + " °C"
            : "N/A";

        QString text = QString(
            "<b>CPU Usage:</b> %1%<br>"
            "<b>CPU Temperature:</b> %2<br>"
            "<b>Processes:</b> %3 / %4<br>"
            "<b>Uptime:</b> %5 s<br>"
            "<hr>"
            "<b>RAM Usage:</b> %6 GB / %7 GB"
        )
        .arg(cpu.usage)
        .arg(tempStr)
        .arg(cpu.processesCount)
        .arg(cpu.threadsCount)
        .arg(cpu.uptime)
        .arg(usedGB, 0, 'f', 2)
        .arg(totalGB, 0, 'f', 2);

        infoLabel->setText(text);
    };

    // connect
    QObject::connect(timer, &QTimer::timeout, updateStats);

    // first update
    updateStats();

    // timer starting
    timer->start(1000);



    // Benchmark zone frame
    QFrame *frame = new QFrame(centralWidget);
    frame->setGeometry(20, 340, 500, 160);
    frame->setFrameShape(QFrame::Box);
    frame->setLineWidth(2);

    // Benchmark button (Pus în interiorul frame-ului sau adus deasupra)
    QPushButton *benchmarkButton = new QPushButton("Start Benchmark", centralWidget);
    benchmarkButton->setGeometry(QRect(30, 350, 120, 30));
    benchmarkButton->setStyleSheet(
    "QPushButton {"
     "    background-color: #6D9696;"
     "    color: white;"
     "    font-size: 13px;"
     "    font-weight: bold;"
     "    border: 1px;"
     "    border-radius: 5px;"
     "}"
     "QPushButton:hover {"
     "    background-color: #6D8196;"
     "}"
     "QPushButton:pressed {"
     "    background-color: #555555;"
     "}"
);
    benchmarkButton->raise();

    // creare labels
QLabel *label500 = new QLabel("Time (ms) for 500 tests: -", centralWidget);
label500->setStyleSheet("font-weight: bold; color: #1a1a1a;");
label500->move(30, 390);
label500->adjustSize();

QLabel *label1k = new QLabel("Time (ms) for 1000 tests: -", centralWidget);
label1k->setStyleSheet("font-weight: bold; color: #1a1a1a;");
label1k->move(30, 415);
label1k->adjustSize();

QLabel *label2k = new QLabel("Time (ms) for 2000 tests: -", centralWidget);
label2k->setStyleSheet("font-weight: bold; color: #1a1a1a;");
label2k->move(30, 440);
label2k->adjustSize();


label500->raise();
label1k->raise();
label2k->raise();

//conectare butoane
QObject::connect(benchmarkButton, &QPushButton::clicked, [benchmarkButton, label500, label1k, label2k]() {
    benchmarkButton->setEnabled(false);
    benchmarkButton->setText("Running...");

    label500->setText("Calculating...");
    label1k->setText("Calculating...");
    label2k->setText("Calculating...");
    label500->adjustSize();
    label1k->adjustSize();
    label2k->adjustSize();

    QtConcurrent::run([benchmarkButton, label500, label1k, label2k]() {
        long long ben500, ben1k, ben2k;
        runBenchmark(ben500, ben1k, ben2k);

        // Actualizare threads
        QMetaObject::invokeMethod(benchmarkButton, [benchmarkButton, label500, label1k, label2k, ben500, ben1k, ben2k]() {
            benchmarkButton->setEnabled(true);
            benchmarkButton->setText("Start Benchmark");

            label500->setText("Time (ms) for 500 tests: " + QString::number(ben500));
            label1k->setText("Time (ms) for 1000 tests: " + QString::number(ben1k));
            label2k->setText("Time (ms) for 2000 tests: " + QString::number(ben2k));

            label500->adjustSize();
            label1k->adjustSize();
            label2k->adjustSize();
        });
    });
});

    // Comment label
    QLabel *label1 = new QLabel("(It might take a few seconds for the results to be displayed.)", centralWidget);
    label1->move(160, 355);
    label1->adjustSize();
    label1->raise();

    window.resize(960, 540);
    window.show();
    return QApplication::exec();
}