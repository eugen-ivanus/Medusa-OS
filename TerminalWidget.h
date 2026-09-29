#pragma once

#include <QWidget>
#include <QProcess>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QNetworkAccessManager>
#include <QNetworkReply>
class TerminalWidget : public QWidget {
    Q_OBJECT

public:
    explicit TerminalWidget(QWidget *parent = nullptr);
    ~TerminalWidget() override;

private slots:
    void sendCommand();
    void readOutput();

private:
    QPlainTextEdit *outputArea;
    QLineEdit *inputLine;
    QVBoxLayout *layout;
    QProcess *shellProcess;
};