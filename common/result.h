#pragma once
#include <QString>
#include <QVariant>

class ResultError
{
public:
    ResultError() = default;
    ResultError(const ResultError&) = default;
    ResultError(ResultError&&) = default;
    ResultError& operator=(const ResultError&) = default;
    ResultError& operator=(ResultError&&) = default;

    ResultError(const QString& error);

    const QString& get() const { return m_error; }
    void set(const QString& error) { m_error = error; }

private:
    QString m_error;
};

Q_DECLARE_METATYPE(ResultError);

class ResultValue
{
    Q_PROPERTY(bool valid READ isValid CONSTANT)
    Q_PROPERTY(QString error READ error CONSTANT)
    Q_PROPERTY(QVariant value READ value CONSTANT)

public:
    ResultValue(const QVariant& value);
    ResultValue(QVariant&& value);
    ResultValue(const ResultError& error);
    ResultValue(ResultError&& error);

    template <typename T>
    ResultValue(const T& value)
        : m_valueOrError{QVariant::fromValue(value)}
    {
    }

    template <typename T>
    ResultValue(T&& value)
        : m_valueOrError{QVariant::fromValue(std::move(value))}
    {
    }

    ResultValue(ResultValue&&) = default;
    ResultValue& operator=(ResultValue&&) = default;
    ResultValue(const ResultValue&) = default;
    ResultValue& operator=(const ResultValue&) = default;

    bool valid() const;
    Q_INVOKABLE operator bool() const;
    QString error() const;
    QVariant value() const;

    template <typename T>
    T value() const
    {
        return value().value<T>();
    }

private:
    QVariant m_valueOrError;
};

Q_DECLARE_METATYPE(ResultValue);

class Result
{
    Q_PROPERTY(bool valid READ isValid CONSTANT)
    Q_PROPERTY(QString error READ error CONSTANT)

public:
    Result();
    Result(Result&&) = default;
    Result& operator=(Result&&) = default;
    Result(const Result&) = default;
    Result& operator=(const Result&) = default;

    Result(const ResultError& error);
    Result(ResultError&& error);

    bool valid() const { return m_error.isNull(); }
    Q_INVOKABLE operator bool() const { return valid(); }
    void value() {}
    QString error() const { return m_error; }

private:
    QString m_error;
};

Q_DECLARE_METATYPE(Result);
