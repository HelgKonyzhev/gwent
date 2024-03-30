#include "deck.h"

bool Check::setValue(QVariant val)
{
    if (!testValue(val))
        return false;

    m_value = val;
    emit valueChanged(m_value);
    return true;
}

class SpecialsLimitCheck : public Check
{
public:
    SpecialsLimitCheck()
        : Check{SpecialCards, 10}
    {
    }

    bool testValue(const QVariant& val) const override
    {
        if (!val.canConvert<int>())
            return false;
        return val.toInt() <= thresholdValue().toInt();
    }
};

class SquadsLimitCheck : public Check
{
public:
    SquadsLimitCheck()
        : Check{SquadCards, 22}
    {
    }

    bool testValue(const QVariant& val) const override
    {
        if (!val.canConvert<int>())
            return false;
        return val.toInt() >= thresholdValue().toInt();
    }
};

class NameCheck : public Check
{
public:
    NameCheck()
        : Check{Name, QString{""}}
    {
    }

    bool testValue(const QVariant& val) const override
    {
        if (!val.canConvert<QString>())
            return false;
        return val != thresholdValue();
    }
};

class FractionCheck : public Check
{
public:
    FractionCheck()
        : Check{Fraction, ""}
    {
    }

    bool testValue(const QVariant& val) const override
    {
        if (!val.canConvert<QString>())
            return false;
        return val != thresholdValue();
    }
};

Deck::Deck()
{
    m_checks.append(new NameCheck{});
    m_checks.append(new FractionCheck{});
    m_checks.append(new SpecialsLimitCheck{});
    m_checks.append(new SquadsLimitCheck{});
}

bool Deck::setName(const QString& name)
{
    if (!m_checks[Check::Name]->setValue(name))
        return false;

    m_name = name;
    return true;
}

bool Deck::setFraction(const QString& fraction)
{
    if (!m_checks[Check::Fraction]->setValue(fraction))
        return false;

    m_fraction = fraction;
    return true;
}

QVector<const Check*> Deck::checks() const
{
    QVector<const Check*> res;
    for (const auto c : m_checks)
        res.append(c);
    return res;
}

bool Deck::isValid() const { return check().isEmpty(); }

QVector<const Check*> Deck::check() const
{
    QVector<const Check*> failed;
    for (const auto check : m_checks)
    {
        if (check->type() == Check::Name)
        {
            if (check->setValue(m_name))
                failed.append(check);
        }
        else if (check->type() == Check::Fraction)
        {
            if (check->setValue(m_fraction))
                failed.append(check);
        }
    }
    return failed;
}
