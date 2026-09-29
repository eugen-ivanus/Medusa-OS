#include "TerminalWidget.h"
#include <QTextCursor>

TerminalWidget::TerminalWidget(QWidget *parent) : QWidget(parent) {
    layout= new QVBoxLayout(this);

    outputArea = new QPlainTextEdit(this);
    outputArea->setReadOnly(true);
    outputArea->setStyleSheet("background-color: #121212; color: #00FF66; font-family: monospace; font-size: 13px;");

    inputLine = new QLineEdit(this);
    inputLine->setStyleSheet("background-color: #1E1E1E; color: #FFFFFF; font-family: monospace; font-size: 13px;");
    inputLine->setPlaceholderText("Enter the command (ex: ls, pwd, uname -a)...");

    layout->addWidget(outputArea);
    layout->addWidget(inputLine);

    shellProcess = new QProcess(this);
    shellProcess->setProcessChannelMode(QProcess::MergedChannels);
    // macros whit slots and signals
    connect(inputLine, SIGNAL(returnPressed()), this, SLOT(sendCommand()));
    connect(shellProcess, SIGNAL(readyRead()), this, SLOT(readOutput()));


    // shell connecting
    shellProcess->start(QString("/bin/sh"), QStringList());
}

TerminalWidget::~TerminalWidget() {
    if (shellProcess->state() == QProcess::Running) {
        shellProcess->terminate();
        shellProcess->waitForFinished(1000);
    }
}

void TerminalWidget::sendCommand() {
    QString cmd = inputLine->text();
    if (cmd.isEmpty()) return;

    outputArea->appendPlainText("$ " + cmd);
    shellProcess->write((cmd + "\n").toUtf8());
    inputLine->clear();
}

void TerminalWidget::readOutput() {
    QByteArray data = shellProcess->readAll();
    if (!data.isEmpty()) {
        outputArea->appendPlainText(QString::fromUtf8(data).trimmed());
        outputArea->moveCursor(QTextCursor::End);
    }
}