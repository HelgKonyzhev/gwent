#include "socket.h"
#include <common/events/event.h>
#include <common/events/error_event.h>
#include <QDebug>

Socket::Socket(QWebSocket* ws, QObject* parent)
    : QObject{parent}
    , m_ws{ws}
{
    m_ws->setParent(this);

    connect(m_ws, SIGNAL(connected()), this, SIGNAL(connected()));
    connect(m_ws, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
    connect(m_ws, &QWebSocket::textMessageReceived, this, &Socket::onTextMessageRecieved);
}

Socket::~Socket() { delete m_ws; }

void Socket::sendEvent(const Event& event)
{
    const auto rawJson = event.toRawJson();

    qDebug() << __func__ << rawJson;

    m_ws->sendTextMessage(rawJson);
}

void Socket::open(const QUrl& url) { m_ws->open(url); }

void Socket::close() { m_ws->close(); }

QUrl Socket::requestUrl() const { return m_ws->requestUrl(); }

void Socket::onTextMessageRecieved(const QString& message)
{
    qDebug() << __func__ << message;

    auto event = Event::fromRawJson(message.toLatin1());
    if (!event)
    {
        ErrorEvent error{QString{"failed to parse message: %1"}.arg(event.error())};
        qDebug() << error.description() << event.error();
        sendEvent(error);
        return;
    }

    qDebug() << "eventRecieved" << event.value()->type();

    emit eventRecieved(event.value());
}
