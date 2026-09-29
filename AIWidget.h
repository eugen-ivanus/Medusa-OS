#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QString>

class AIWidget:public QWidget {
    Q_OBJECT
public:
    explicit AIWidget(QWidget *parent=nullptr);
    ~AIWidget() override;

 private :
    QPlainTextEdit *outputArea;
    QLineEdit *inputLine;
    QVBoxLayout *boxLayout;
    QNetworkAccessManager *networkManager;
    void sendQuestion(const QString &question);

};
