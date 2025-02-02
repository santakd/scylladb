/*
 * Copyright (C) 2015-present ScyllaDB
 */

/*
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#pragma once

#include "multiprecision_int.hh"
#include <boost/multiprecision/cpp_int.hpp>
#include <ostream>
#include <compare>
#include <concepts>

#include "bytes.hh"

uint64_t from_varint_to_integer(const utils::multiprecision_int& varint);

class big_decimal {
private:
    int32_t _scale;
    boost::multiprecision::cpp_int _unscaled_value;
public:
    enum class rounding_mode {
        HALF_EVEN,
    };

    explicit big_decimal(sstring_view text);
    big_decimal();
    big_decimal(int32_t scale, boost::multiprecision::cpp_int unscaled_value);
    big_decimal(std::integral auto v) : big_decimal(0, v) {}

    int32_t scale() const { return _scale; }
    const boost::multiprecision::cpp_int& unscaled_value() const { return _unscaled_value; }
    boost::multiprecision::cpp_rational as_rational() const;

    sstring to_string() const;

    std::strong_ordering compare(const big_decimal& other) const;

    big_decimal& operator+=(const big_decimal& other);
    big_decimal& operator-=(const big_decimal& other);
    big_decimal operator+(const big_decimal& other) const;
    big_decimal operator-(const big_decimal& other) const;
    big_decimal div(const ::uint64_t y, const rounding_mode mode) const;
    friend bool operator<(const big_decimal& x, const big_decimal& y) { return x.compare(y) < 0; }
    friend bool operator<=(const big_decimal& x, const big_decimal& y) { return x.compare(y) <= 0; }
    friend bool operator==(const big_decimal& x, const big_decimal& y) { return x.compare(y) == 0; }
    friend bool operator!=(const big_decimal& x, const big_decimal& y) { return x.compare(y) != 0; }
    friend bool operator>=(const big_decimal& x, const big_decimal& y) { return x.compare(y) >= 0; }
    friend bool operator>(const big_decimal& x, const big_decimal& y) { return x.compare(y) > 0; }
};

inline std::ostream& operator<<(std::ostream& s, const big_decimal& v) {
    return s << v.to_string();
}
