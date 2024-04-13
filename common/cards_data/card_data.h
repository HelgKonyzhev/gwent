#pragma once
#include <QJsonObject>

class CardsData;

class CardData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name CONSTANT);
    Q_PROPERTY(QString image READ image CONSTANT);
    Q_PROPERTY(QString fraction READ fraction CONSTANT);
    Q_PROPERTY(QStringList tags READ tags CONSTANT);

public:
    CardData(const QJsonObject& data, CardsData* parent);

    QString name() const { return m_data["name"].toString(); }
    QString image() const { return m_data["image"].toString(); }
    QString fraction() const { return m_data["fraction"].toString(); }
    QStringList tags() const { return m_data["tags"].toVariant().toStringList(); }

private:
    QJsonObject m_data;
};
