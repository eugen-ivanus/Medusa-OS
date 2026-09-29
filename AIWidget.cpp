#include "AIWidget.h"
#include <QTextCursor>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QUrl>

#include <QUrlQuery>

AIWidget::AIWidget(QWidget *parent):QWidget(parent) {
    boxLayout=new QVBoxLayout(this);

     //outut area making
    outputArea=new QPlainTextEdit(this);
    outputArea-> setReadOnly(true);
    outputArea->setStyleSheet("background-color: #121212; color: #00FF66; font-family: monospace; font-size: 13px;");

    //input area making
    inputLine = new QLineEdit(this);
    inputLine->setStyleSheet("background-color: #1E1E1E; color: #FFFFFF; font-family: monospace; font-size: 13px;");
    inputLine->setPlaceholderText("Ask anything...");

    //connecting output area with input area in the same layout
    boxLayout->addWidget(outputArea);
    boxLayout->addWidget(inputLine);

    //conecting the api to input-output area (layout) by siglanls &slots
    networkManager = new QNetworkAccessManager(this);
    connect (inputLine,&QLineEdit::returnPressed,[this]() {
        //lambda -> execution inputLine it send questions .if question send then clear
        const QString question = inputLine ->text().trimmed();
        if (!question.isEmpty()) {
            outputArea->appendPlainText("User: " + question);
            sendQuestion(question);
            inputLine->clear();
        }
    });

}
AIWidget::~AIWidget() {

}
// sendQuestion function who belog to AIWidget object
void AIWidget::sendQuestion(const QString &question ) {
   //creating api key string
    const QString apiKey = "MY APY KEY";

    // creating url
    QUrl url(
     "https://generativelanguage.googleapis.com/v1beta/models/"
     "gemini-3.5-flash:generateContent"
 );
    // request creation
    QUrlQuery query;
    query.addQueryItem("key", apiKey);
    url.setQuery(query);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    //adding and transforming the question in array  addinfg  in Qjson
    QJsonObject part;
    part[QString("text")] = question;
    QJsonArray parts;
    parts.append(part);
    QJsonObject content;
    content[QString("parts")]=parts;
    QJsonArray contents;
    contents.append(content);
    QJsonObject json;
    json[QString("contents")]=contents;

    // sending json by http to api
   QByteArray data =QJsonDocument(json).toJson();
    QNetworkReply *reply = networkManager->post(request ,data);
    connect (reply , &QNetworkReply::finished, [this,reply]() {
        if (reply -> error() != QNetworkReply::NoError) {
            outputArea-> appendPlainText("ERROR: "+ reply -> errorString());
            reply -> deleteLater();
            return;
        }

        // reversing to respond
        const QByteArray response = reply -> readAll();
        QJsonDocument doc = QJsonDocument::fromJson(response);
        QJsonObject root = doc.object();
         QJsonArray candidates = root[QString("candidates")].toArray();
        if (!candidates.isEmpty()) {
            QJsonObject candidate= candidates[0].toObject();
            QJsonObject content = candidate["content"].toObject();
            QJsonArray parts = content[QString("parts")].toArray();
            if (!parts.isEmpty()) {
                QString answer = parts[0].toObject()["text"].toString();
                outputArea-> appendPlainText("AI: " + answer);
            }
        }
        reply->deleteLater();
    });
}
