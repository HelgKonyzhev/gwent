#pragma once
#include <QWebSocket>

class Event;

class Socket : public QObject
{
    Q_OBJECT

public:
    Socket(QWebSocket* ws, QObject* parent = nullptr);
    ~Socket() override;

    void sendEvent(const Event& event);
    void open(const QUrl& url);
    void close();
    QUrl requestUrl() const;
    QAbstractSocket::SocketState state() const { return m_ws->state(); }

signals:
    void eventRecieved(Event* event);
    void connected();
    void disconnected();

private:
    void onTextMessageRecieved(const QString& message);

    QWebSocket* m_ws;
};
