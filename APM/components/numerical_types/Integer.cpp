
/*********************************************************************/
//
//			Copyright 2022 Max J. Martin
//
//			This file is part of APM.
// 
//			APM is free software : you can redistribute it
//			and /or modify it under the terms of the GNU General
//			Public License as published by the Free Software
//			Foundation, either version 3 of the License, or
//			(at your option) any later version.
//			
//			APM is distributed in the hope that it will be
//			useful, but WITHOUT ANY WARRANTY; without even the
//			implied warranty of MERCHANTABILITY or FITNESS FOR
//			A PARTICULAR PURPOSE.See the GNU General Public
//			
//			You should have received a copy of the GNU General
//			Public License along with APM.If not, see
//			< https://www.gnu.org/licenses/>.
//			
/*********************************************************************/

#include "Integer.h"

namespace Olly {
    namespace APM {

        const Integer Integer::NaN(SIGN::nan);
        const Integer Integer::UNDEF(SIGN::undef);
        const Integer Integer::POS_INFINITY(SIGN::pos_infinity);
        const Integer Integer::NEG_INFINITY(SIGN::neg_infinity);

        Integer::Integer() : _number(), _sign(SIGN::zero) {
        }

        Integer::Integer(sys_int value) : _number(std::abs(value)), _sign(value < 0.0 ? SIGN::negative : SIGN::positive) {
            check_for_zero();
        }

        Integer::Integer(const Whole_Number& value) : _number(value), _sign(SIGN::positive) {
            check_for_zero();
        }

        Integer::Integer(Text value, sys_int base) : _number(), _sign(SIGN::positive) {

            Boolean error = false;

            if (value.size()) {

                if (value[0] == '-') {
                    _sign = SIGN::negative;
                    value[0] = ' ';
                }

                if (value[0] == '+') {
                    _sign = SIGN::positive;
                    value[0] = ' ';
                }

                lrtrim(value);

                if (value == "infinity") {
                    _sign = (_sign == SIGN::negative ? SIGN::neg_infinity : SIGN::pos_infinity);
                }

                else if (value == "undefined") {
                    _sign = SIGN::undef;
                }

                else {

                    _number = Whole_Number(value, base, error);

                    check_for_zero();
                }
            }

            if (error) {
                _sign = SIGN::nan;
            }
        }

        Integer::Integer(SIGN sign) : _number(), _sign(sign) {
        }

        Integer::~Integer() {
        }

        Boolean Integer::is() const {
            return _sign != SIGN::zero && _sign > SIGN::undef;
        }

        Boolean Integer::is_odd() const {
            return is_finite() && _number.is_odd();
        }

        Boolean Integer::is_even() const {
            return is_finite() && _number.is_even();
        }

        Boolean Integer::is_positive() const {
            return _sign > SIGN::zero;
        }

        Boolean Integer::is_negative() const {
            return _sign == SIGN::negative || _sign == SIGN::neg_infinity;
        }

        Boolean Integer::is_zero() const {
            return _sign == SIGN::zero;
        }

        Boolean Integer::is_undefined() const {
            return _sign <= SIGN::undef;
        }

        Boolean Integer::is_defined() const {
            return _sign > SIGN::undef;
        }

        Boolean Integer::is_nan() const {
            return _sign == SIGN::nan;
        }

        Boolean Integer::is_finite() const {
            return _sign > SIGN::neg_infinity && _sign < SIGN::pos_infinity;
        }

        Boolean Integer::is_infinite() const {
            return _sign == SIGN::neg_infinity || _sign == SIGN::pos_infinity;
        }

        Boolean Integer::operator==(const Integer& b) const {
            return compare(b) == 0.0;
        }

        Boolean Integer::operator!=(const Integer& b) const {
            return compare(b) != 0.0;
        }

        Boolean Integer::operator>=(const Integer& b) const {
            return compare(b) >= 0.0;
        }

        Boolean Integer::operator<=(const Integer& b) const {
            return compare(b) <= 0.0;
        }

        Boolean Integer::operator>(const Integer& b) const {
            return compare(b) > 0.0;
        }

        Boolean Integer::operator<(const Integer& b) const {
            return compare(b) < 0.0;
        }

        sys_float Integer::compare(const Integer& b) const {

            if (is_undefined() || b.is_undefined()) {
                return NOT_A_NUMBER;
            }

            if (_sign < b._sign) {

                return -1.0;
            }
            else if (_sign > b._sign) {

                return 1.0;
            }

            sys_float truth = _number.compare(b._number);

            if (is_negative()) {

                truth *= -1.0;
            }

            return truth;
        }

        Integer& Integer::operator+=(const Integer& b) {

            if (is_finite() && b.is_finite()) {

                if (_sign == b._sign) {

                    _number += b._number;

                    return *this;
                }

                sys_float n = _number.compare(b._number);

                if (n > 0.0) {
                    _number -= b._number;
                }
                else if (n < 0.0) {
                    _number = b._number - _number;
                    _sign = b._sign;
                }
                else {
                    *this = Integer();
                }

                check_for_zero();

                return *this;
            }

            if (is_infinite() || b.is_infinite()) {

                if (is_infinite() && _sign != b._sign) {
                    *this = Integer(SIGN::undef);
                }
                else if (b.is_infinite()) {
                    *this = b;
                }
            }
            else {
                *this = Integer(SIGN::undef);
            }

            return *this;
        }

        Integer& Integer::operator-=(const Integer& b) {
            return operator+=(-b);
        }

        Integer& Integer::operator*=(const Integer& b) {

            if (is_finite() && b.is_finite()) {

                _number = _number * b._number;

                if (_sign == _sign) {

                    _sign = SIGN::positive;
                }
                else {
                    _sign = SIGN::negative;
                }

                check_for_zero();

                return *this;
            }

            if (is_infinite() && b.is_infinite()) {

                if (_sign == b._sign) {
                    *this = Integer(SIGN::pos_infinity);
                }
                else {
                    *this = Integer(SIGN::neg_infinity);
                }
            }

            else if (is_infinite() || b.is_infinite()) {

                Integer x = *this;
                Integer y = b;

                if (is_infinite()) {
                    std::swap(x, y);
                }

                if ((x.is_positive() && y.is_negative()) || (x.is_negative() && y.is_positive())) {
                    *this = Integer(SIGN::neg_infinity);
                }
                else {
                    *this = Integer(SIGN::pos_infinity);
                }

                if (is_zero()) {
                    *this = Integer(SIGN::undef);
                }
            }
            else {
                *this = Integer(SIGN::undef);
            }

            return *this;  // TODO Determine sign exceptions.
        }

        Integer& Integer::operator/=(const Integer& b) {

            Integer qot, rem;
            div_rem(b._number, qot, rem);

            // TODO Determine sign exceptions.

            *this = qot;

            if (_sign == b._sign) {

                _sign = SIGN::positive;
            }
            else {
                _sign = SIGN::negative;
            }

            check_for_zero();

            return *this;
        }

        Integer& Integer::operator%=(const Integer& b) {

            Integer qot, rem;
            div_rem(b._number, qot, rem);

            // TODO Determine sign exceptions.

            *this = rem;

            check_for_zero();

            return *this;
        }

        Integer Integer::operator+(const Integer& b) const {

            Integer a = *this;

            a += b;

            return a;
        }

        Integer Integer::operator-(const Integer& b) const {

            Integer a = *this;

            a -= b;

            return a;
        }

        Integer Integer::operator*(const Integer& b) const {

            Integer a = *this;

            a *= b;

            return a;
        }

        Integer Integer::operator/(const Integer& b) const {

            Integer a = *this;

            a /= b;

            return a;
        }

        Integer Integer::operator%(const Integer& b) const {

            Integer a = *this;

            a %= b;

            return a;
        }

        Integer Integer::operator+() const {
            return *this;
        }

        Integer Integer::operator-() const {

            Integer a(*this);

            a.invert_sign();

            return a;
        }

        Integer& Integer::operator++() {

            if (is_finite()) {

                if (is_negative()) {

                    --_number;

                    check_for_zero();
                }
                else {

                    ++_number;

                    if (is_zero()) {
                        _sign = SIGN::positive;
                    }
                }
            }

            return *this;
        }

        Integer Integer::operator++(int) {

            Integer a(*this);

            operator++();

            return a;
        }

        Integer& Integer::operator--() {

            if (is_finite()) {

                if (is_positive()) {

                    --_number;

                    check_for_zero();
                }
                else {

                    ++_number;

                    if (is_zero()) {
                        _sign = SIGN::negative;
                    }
                }
            }

            return *this;
        }

        Integer Integer::operator--(int) {

            Integer a(*this);

            operator--();

            return a;
        }

        void Integer::div_rem(const Integer& other, Integer& qot, Integer& rem) const {
            /*
                Error checking for division by zero, or other issues.
                must take place prior to the invocation of the whole
                number division remainder method.
            */

            if (other.is_zero()) {
                qot = Integer(SIGN::undef);
                rem = qot;

                return;
            }

            if (is_finite() && other.is_finite()) {

                _number.div_rem(other._number, qot._number, rem._number);

                if (_sign == other._sign) {

                    qot._sign = SIGN::positive;
                }
                else {
                    qot._sign = SIGN::negative;
                }
                rem._sign = SIGN::positive;

                qot.check_for_zero();
                rem.check_for_zero();

                return;
            }

            qot = Integer(SIGN::undef);
            rem = qot;
        }

        Integer Integer::pow(Size b) const {

            if (is_finite()) {

                Integer a(*this);

                a._number = _number.pow(b);

                // Note the power is enacted on the whole number.
                // Therefore no sign manipulation is needed.

                return a;
            }

            return Integer(SIGN::undef);
        }

        Integer Integer::gcd(Integer b) const {

            sys_float comp = compare(b);

            Whole_Number x = comp > 0.0 ? _number : b._number;
            Whole_Number y = comp < 0.0 ? _number : b._number;
            Whole_Number z;

            while (y.is()) {

                z = x % y;

                x = y;
                y = z;
            }

            return x;
        }

        Integer Integer::sqrt() const {
            return is_finite() ? _number.sqrt() : Integer(SIGN::undef);
        }

        Integer Integer::abs() const {
            return is_negative() ? operator-() : *this;
        }

        Text Integer::sign() const {

            Text result = "";

            switch (_sign) {

            case (SIGN::positive):
                result = "positive";
                break;

            case (SIGN::negative):
                result = "negative";
                break;

            case (SIGN::zero):
                result = "zero";
                break;

            case (SIGN::pos_infinity):
                result = "+infinity";
                break;

            case (SIGN::neg_infinity):
                result = "-infinity";
                break;

            case (SIGN::undef):
                result = "undefined";
                break;

            case (SIGN::nan):
                result = "nan";
                break;
            }

            return result;
        }

        Text Integer::to_string() const {

            Text result = "";

            switch (_sign) {

            case (SIGN::zero):
                result = "0";
                break;

            case (SIGN::pos_infinity):
                result = "+infinity";
                break;

            case (SIGN::neg_infinity):
                result = "-infinity";
                break;

            case (SIGN::undef):
                result = "undefined";
                break;

            case (SIGN::nan):
                result = "nan";
                break;

            default:
                result = to_string(10, -1);
            }

            return result;
        }

        Text Integer::to_string(Size base, sys_int sign) const {

            Text result = "";

            if (is_negative() && sign != 0) {
                result += "-";
            }
            else if (is_positive() && sign > 0) {
                result += "+";
            }

            result += _number.to_string(base);

            return result;
        }

        const Whole_Number& Integer::get_Whole_Number() const {
            return _number;
        }

        void Integer::invert_sign() {

            switch (_sign) {

            case (SIGN::positive):
                _sign = SIGN::negative;
                break;

            case (SIGN::negative):
                _sign = SIGN::positive;
                break;

            case (SIGN::pos_infinity):
                _sign = SIGN::neg_infinity;
                break;

            case (SIGN::neg_infinity):
                _sign = SIGN::pos_infinity;
                break;

            default:
                _sign = SIGN::undef;
            }
        }

        void Integer::check_for_zero() {

            if (!_number.is()) {
                _sign = SIGN::zero;
            }
        }
    }
} 
