#include "result.h"

ResultError::ResultError(const QString& error)
    : m_error{error}
{
    qRegisterMetaType<ResultError>();
}

ResultValue::ResultValue(const QVariant& value)
    : m_valueOrError{value}
{
    qRegisterMetaType<ResultValue>();
}

ResultValue::ResultValue(QVariant&& value)
    : m_valueOrError{std::move(value)}
{
    qRegisterMetaType<ResultValue>();
}

ResultValue::ResultValue(const ResultError& error)
    : m_valueOrError{QVariant::fromValue(error)}
{
    qRegisterMetaType<ResultValue>();
}

ResultValue::ResultValue(ResultError&& error)
    : m_valueOrError{QVariant::fromValue(std::move(error))}
{
    qRegisterMetaType<ResultValue>();
}

bool ResultValue::valid() const { return m_valueOrError.typeId() != qMetaTypeId<ResultError>(); }

ResultValue::operator bool() const { return valid(); }

QString ResultValue::error() const { return valid() ? QString{} : m_valueOrError.value<ResultError>().get(); }

QVariant ResultValue::value() const { return valid() ? m_valueOrError : QVariant{}; }

Result::Result() { qRegisterMetaType<Result>(); }

Result::Result(const ResultError& error)
    : m_error{error.get()}
{
    qRegisterMetaType<Result>();
}

Result::Result(ResultError&& error)
    : m_error{std::move(error).get()}
{
    qRegisterMetaType<Result>();
}
