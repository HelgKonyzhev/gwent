#pragma once
#include <optional>
#include <sstream>
#include <variant>
#include <QString>
#include <QTextStream>

class ResultError
{
public:
    ResultError(const ResultError&) = default;
    ResultError(ResultError&&) = default;
    ResultError& operator=(const ResultError&) = default;
    ResultError& operator=(ResultError&&) = default;

    template <typename Arg0, typename... Args,
             std::enable_if_t<!std::is_same_v<std::remove_reference_t<Arg0>, ResultError>, bool> = true>
    explicit ResultError(Arg0&& arg0, Args&&... args)
        : m_error{}
    {
        QTextStream ss{&m_error};
        ss << arg0;
        ((ss << args), ...);
    }

    const QString& get() const { return m_error; }

private:
    QString m_error;
};

class DerefError : public std::logic_error
{
public:
    DerefError(const QString& error)
        : logic_error{error.toStdString()}
    {
    }
};

template <typename ValueT>
class ResultValue
{
    static_assert(!std::is_same_v<ValueT, ResultError>, "Result value can't be Error");

    using ValueOrError = std::variant<ValueT, ResultError>;

    enum Index
    {
        ValueIdx,
        ErrorIdx
    };

public:
    using Value = ValueT;

    ResultValue(const ValueT& value)
        : m_valueOrError{value}
    {
    }

    ResultValue(ValueT&& value)
        : m_valueOrError{std::move(value)}
    {
    }

    ResultValue(const ResultError& error)
        : m_valueOrError{error}
    {
    }

    ResultValue(ResultError&& error)
        : m_valueOrError{std::move(error)}
    {
    }

    ResultValue(ResultValue&&) = default;
    ResultValue& operator=(ResultValue&&) = default;
    ResultValue(const ResultValue&) = default;
    ResultValue& operator=(const ResultValue&) = default;

    template <typename OtherValue,
              typename std::enable_if_t<std::is_integral_v<OtherValue> || std::is_enum_v<OtherValue> ||
                                            std::is_floating_point_v<OtherValue>,
                                        bool> = true>
    ResultValue<OtherValue> convertTo() const
    {
        if (m_valueOrError.index() == ErrorIdx)
            return std::get<ErrorIdx>(m_valueOrError);
        return static_cast<OtherValue>(std::get<ValueIdx>(m_valueOrError));
    }

    template <typename OtherValue,
              typename std::enable_if_t<!std::is_integral_v<OtherValue> && !std::is_enum_v<OtherValue> &&
                                            !std::is_floating_point_v<OtherValue>,
                                        bool> = true>
    ResultValue<OtherValue> convertTo() const
    {
        if (m_valueOrError.index() == ErrorIdx)
            return std::get<ErrorIdx>(m_valueOrError);
        return OtherValue{std::get<ValueIdx>(m_valueOrError)};
    }

    operator bool() const { return m_valueOrError.index() == ValueIdx; }

    const QString& error() const noexcept(false)
    {
        if (m_valueOrError.index() == ValueIdx)
            throw DerefError("Result is not containing error");
        return std::get<ErrorIdx>(m_valueOrError).get();
    }

    // This overload will be called on lvalue instance of the class, so the value returned by reference.
    const ValueT& value() const& noexcept(false)
    {
        if (m_valueOrError.index() == ErrorIdx)
            throw DerefError(error());
        return std::get<ValueIdx>(m_valueOrError);
    }

    // This overload will be called on rvalue instance of the class, so the value should be moved.
    ValueT value() && noexcept(false)
    {
        if (m_valueOrError.index() == ErrorIdx)
            throw DerefError(error());
        return std::move(std::get<ValueIdx>(m_valueOrError));
    }

private:
    ValueOrError m_valueOrError;
};

template <>
class ResultValue<void>
{
public:
    ResultValue() {}
    ResultValue(ResultValue&&) = default;
    ResultValue& operator=(ResultValue&&) = default;
    ResultValue(const ResultValue&) = default;
    ResultValue& operator=(const ResultValue&) = default;

    ResultValue(ResultError&& error)
        : m_error{std::move(error).get()}
    {
    }

    operator bool() const { return !m_error.has_value(); }

    const QString& error() const
    {
        if (!m_error.has_value())
            throw DerefError("Result is not containing error");
        return m_error.value();
    }

    void value() const
    {
        if (m_error.has_value())
            throw DerefError(error());
    }

private:
    std::optional<QString> m_error;
};

using Result = ResultValue<void>;
