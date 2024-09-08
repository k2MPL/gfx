#pragma once

template<typename TYPE, typename VALUE_TYPE, VALUE_TYPE INVALID_VALUE>
struct ProtectedID
{
    VALUE_TYPE value{INVALID_VALUE};

    ProtectedID() = default;
    explicit ProtectedID(const VALUE_TYPE& _value)
    : value{_value}
    {

    }

    bool isValid() const
    {
        return value != INVALID_VALUE;
    }

    void invalidate()
    {
        value = INVALID_VALUE;
    }

    bool operator == (const ProtectedID<TYPE, VALUE_TYPE, INVALID_VALUE>& _id) const
    {
        return _id.value == value;
    }
};