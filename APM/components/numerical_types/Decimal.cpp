
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

#include "Decimal.h"

namespace Olly {
    namespace APM {

        const Integer Decimal::ONE = Integer(1);
        const Integer Decimal::TWO = Integer(2);
        const Integer Decimal::TEN = Integer(10);

        sys_int Decimal::scale() {
            return decimal_scale();
        }

        void Decimal::scale(const sys_int& scale) {

            sys_int scl = (scale >= MIN_SCALE) ? scale : MIN_SCALE;
            scl = (scl <= MAX_SCALE) ? scl : MAX_SCALE;

            if (scale_not_configured()) {

                scale_not_configured() = false;
                decimal_scale() = scl;
                denominator() = Decimal::TEN.pow(scl);

                integer_e();
                integer_pi();
                integer_ln2();
            }
        }

        Text Decimal::rounding_mode() {

            switch (round_mode()) {

            case(ROUNDING_MODE::half_up):
                return "half_up";

            case(ROUNDING_MODE::half_down):
                return "half_down";

            case(ROUNDING_MODE::half_even):
                return "half_even";

            case(ROUNDING_MODE::half_odd):
                return "half_odd";

            case(ROUNDING_MODE::ceil):
                return "ceil";

            case(ROUNDING_MODE::floor):
                return "floor";

            case(ROUNDING_MODE::away_from_zero):
                return "away_from_zero";
            }

            return "toward_zero";
        }

        void Decimal::rounding_mode(const Text& mode) {

            if (mode == "half_up") {
                round_mode() = ROUNDING_MODE::half_up;
            }

            else if (mode == "half_down") {
                round_mode() = ROUNDING_MODE::half_down;
            }

            else if (mode == "half_even") {
                round_mode() = ROUNDING_MODE::half_even;
            }

            else if (mode == "half_odd") {
                round_mode() = ROUNDING_MODE::half_odd;
            }

            else if (mode == "ceil") {
                round_mode() = ROUNDING_MODE::ceil;
            }

            else if (mode == "floor") {
                round_mode() = ROUNDING_MODE::floor;
            }

            else if (mode == "away_from_zero") {
                round_mode() = ROUNDING_MODE::away_from_zero;
            }

            else {
                ROUNDING_MODE::toward_zero;
            }
        }

        Integer Decimal::decimal_denominator() {
            return denominator();
        }

        Decimal Decimal::e() {
            return decimal_e();
        }

        Decimal Decimal::pi() {
            return decimal_pi();
        }

        Decimal Decimal::ln2() {
            return decimal_ln2();
        }

        Boolean& Decimal::scale_not_configured() {

            static Boolean truth = true;

            return truth;
        }

        Size& Decimal::decimal_scale() {

            static Size scale = DEF_SCALE;

            return scale;
        }

        Integer& Decimal::denominator() {

            static Integer denom = Decimal::TEN;

            return denom;
        }

        Integer& Decimal::integer_pi() {

            static Integer pi(PI_STRING.substr(0, decimal_scale() + 1));

            return pi;
        }

        Integer& Decimal::integer_e() {

            static Integer e(E_STRING.substr(0, decimal_scale() + 1));

            return e;
        }

        Integer& Decimal::integer_ln2() {

            static Integer ln2(LN2_STRING.substr(0, decimal_scale()));

            return ln2;
        }

        Decimal& Decimal::decimal_e() {

            static Decimal e;

            e._number = integer_e();

            return e;
        }

        Decimal& Decimal::decimal_pi() {

            static Decimal pi;

            pi._number = integer_pi();

            return pi;
        }

        Decimal& Decimal::decimal_ln2() {

            static Decimal ln2;

            ln2._number = integer_ln2();

            return ln2;
        }

        Decimal::ROUNDING_MODE& Decimal::round_mode() {

            ROUNDING_MODE mode = ROUNDING_MODE::half_even;

            return mode;
        }

        Decimal::Decimal() : _number(), _scale(scale()) {
        }

        Decimal::Decimal(sys_int value) : _number(value), _scale(scale()) {
            _number *= denominator();
        }

        Decimal::Decimal(const Integer& value) : _number(value), _scale(scale()) {
            _number *= denominator();
        }

        Decimal::Decimal(const Whole_Number& value) : _number(value), _scale(scale()) {
            _number *= denominator();
        }

        Decimal::Decimal(Text value) : _number(), _scale(scale()) {
            if (scale_not_configured()) {
                scale(Decimal::DEF_SCALE);
            }

            value = to_lower(value);

            set_decimal_text(value);
        }

        Decimal::~Decimal() {
        }

        Boolean Decimal::is() const {
            return _number.is();
        }

        Boolean Decimal::is_odd() const {
            return _number.is_odd();
        }

        Boolean Decimal::is_even() const {
            return _number.is_even();
        }

        Boolean Decimal::is_positive() const {
            return _number.is_positive();
        }

        Boolean Decimal::is_negative() const {
            return _number.is_negative();
        }

        Boolean Decimal::is_zero() const {
            return _number.is_zero();
        }

        Boolean Decimal::operator==(const Decimal& b) const {
            return compare(b) == 0;
        }

        Boolean Decimal::operator!=(const Decimal& b) const {
            return compare(b) != 0;
        }

        Boolean Decimal::operator>=(const Decimal& b) const {
            return compare(b) >= 0;
        }

        Boolean Decimal::operator<=(const Decimal& b) const {
            return compare(b) <= 0;
        }

        Boolean Decimal::operator>(const Decimal& b) const {
            return compare(b) > 0;
        }

        Boolean Decimal::operator<(const Decimal& b) const {
            return compare(b) < 0;
        }

        sys_float Decimal::compare(const Decimal& b) const {

            if (_scale != scale()) {

                return update_scale_copy().compare(b);
            }

            if (b._scale != scale()) {
                return compare(b.update_scale_copy());
            }

            return _number.compare(b._number);
        }

        Decimal& Decimal::operator+=(const Decimal& b) {

            if (b._scale != scale()) {
                return operator+=(b.update_scale_copy());
            }

            if (_scale != scale()) {

                update_scale();
            }

            _number += b._number;

            return *this;
        }

        Decimal& Decimal::operator-=(const Decimal& b) {
            return operator+=(-b);
        }

        Decimal& Decimal::operator*=(const Decimal& b) {

            if (b._scale != scale()) {
                return operator*=(b.update_scale_copy());
            }

            if (_scale != scale()) {

                update_scale();
            }

            _number *= b._number;

            _number /= denominator();

            return *this;
        }

        Decimal& Decimal::operator/=(const Decimal& b) {

            if (b._scale != scale()) {
                return operator/=(b.update_scale_copy());
            }

            if (_scale != scale()) {

                update_scale();
            }

            _number *= denominator();

            _number /= b._number;

            return *this;
        }

        Decimal& Decimal::operator%=(const Decimal& b) {

            operator/=(b);

            _number %= denominator();

            return *this;
        }

        Decimal Decimal::operator+(const Decimal& b) const {

            Decimal a(*this);

            a += b;

            return a;
        }

        Decimal Decimal::operator-(const Decimal& b) const {

            Decimal a(*this);

            a -= b;

            return a;
        }

        Decimal Decimal::operator*(const Decimal& b) const {

            Decimal a(*this);

            a *= b;

            return a;
        }

        Decimal Decimal::operator/(const Decimal& b) const {

            Decimal a(*this);

            a /= b;

            return a;
        }

        Decimal Decimal::operator%(const Decimal& b) const {

            Decimal a(*this);

            a %= b;

            return a;
        }

        Decimal Decimal::operator+() const {

            Decimal a(*this);

            return a;
        }

        Decimal Decimal::operator-() const {

            Decimal a(*this);

            a._number = -a._number;

            if (a._scale != scale()) {
                a.update_scale();
            }

            return a;
        }

        void Decimal::div_rem(const Decimal& b, Decimal& qot, Decimal& rem) const {

            if (b._scale != scale()) {
                return div_rem(b.update_scale_copy(), qot, rem);
            }

            if (_scale != scale()) {

                return update_scale_copy().div_rem(b, qot, rem);
            }

            (_number * denominator()).div_rem(b._number, qot._number, rem._number);
        }

        void Decimal::num_den(Decimal& num, Decimal& den) const {
            _number.div_rem(denominator(), num._number, den._number);
        }

        Decimal Decimal::pow(Size b) const {

            Decimal a;

            a._number = _number.pow(b);

            return a;
        }

        Decimal Decimal::abs() const {

            Decimal a;

            a._number = _number.abs();
            a._scale = _scale;

            return a;
        }

        Decimal Decimal::inverse() const {

            Decimal a(denominator());

            a._number /= _number;

            return a;
        }

        Decimal Decimal::ceil() const {

            if (decimal_scale() == 0) {
                return *this;
            }

            Decimal a(*this);

            Integer q, r;
            a._number.div_rem(denominator(), q, r);

            a._number = q * denominator();

            if (r.is()) {

                if (a.is_positive()) {
                    a._number += denominator();
                }
                else {
                    a._number -= denominator();
                }
            }

            return a;
        }

        Decimal Decimal::floor() const {

            if (decimal_scale() == 0) {
                return *this;
            }

            Decimal a(*this);

            Integer q, r;
            a._number.div_rem(denominator(), q, r);

            a._number = q * denominator();

            if (r.is()) {

                if (a.is_positive()) {
                    a._number -= denominator();
                }
                else {
                    a._number += denominator();
                }
            }

            return a;
        }

        /*Decimal Decimal::factorial() const {

            Integer q, r;
            _number.div_rem(denominator(), q, r);

            if (!r.is()) {
                Integer a = q;
                Integer b = 1;

                while (a > ONE) {
                    b *= a;
                    a -= ONE;
                }

                return Decimal(b);
            }

            Decimal num, den;
            num_den(num, den);

            q = num._number;
            r = den._number;

            Integer n = 1;
            Integer d = 1;

            while (q > ONE) {
                n *= q;
                d *= r;
                q -= den._number;
            }

            Decimal r;
            r._number = ((n * denominator()) / d) * integer_pi().sqrt();

            return r;
        }*/

        Text Decimal::sign() const {
            return _number.sign();
        }

        Text Decimal::to_string() const {
            return to_string(10, -1);
        }

        Text Decimal::to_string(Size base, sys_int sign) const {

            if (_scale != decimal_scale()) {
                return update_scale_copy().to_string();
            }

            Integer qot, rem;
            _number.div_rem(denominator(), qot, rem);

            Text result = qot.to_string(0, -1);

            if (rem.is()) {
                result += "." + rem.to_string(0, -1);
            }
            else {
                result += ".0";
            }

            return result;
        }

        Decimal Decimal::ln() const {

            if ((_number == denominator())) {
                return Decimal();
            }
            else if ((_number == integer_e())) {
                return Decimal(denominator());
            }

            if ((_number > denominator())) {
                /*
                    Factor out the power of 2.  Then get ln().
                    This greatly improves speed of convergance.
                */

                Size  exp = (_number / denominator()).get_Whole_Number().get_Binary_Register().lead_bit() - 1;
                Integer x = _number.get_Whole_Number() >> exp;

                Decimal r;
                r._number = x;

                // return ln(x) + ln(2) * exp
                return (r.get_ln() + decimal_ln2() * Decimal(exp));
            }

            return -get_ln();
        }

        const Integer& Decimal::get_Integer() const {
            return _number;
        }

        void Decimal::set_decimal_text(Text& text) {

            lrtrim(text);

            if (!text.empty()) {

                text = to_lower(text);

                Integer exponent = get_sub_text_value(text, "e");

                Size scale = find_and_set_scale(text);

                _number = Integer(text);

                if (scale < decimal_scale()) {
                    _number = _number * (denominator() / TEN.pow(scale));
                }

                set_decimal_exponent(exponent);
            }
        }

        void Decimal::set_decimal_exponent(Integer& exponent) {
            /*
                Test if an exponent was defined then apply the exponent.
                Then reset the _scale of the decimal number.
            */
            if (exponent.is()) {

                Size n = (exponent / denominator()).to_integral<Size>();

                Integer exp = TEN.pow(n);

                if (exponent.is_negative()) {

                    _number = _number / exp;
                }
                else if (exponent.is_positive()) {

                    _number = _number * exp;
                }
            }
        }

        Integer Decimal::get_sub_text_value(Text& value, Text del) const {
            /*
                See if a value is defined for a specific deliminator.  If found get and
                return the Ineger after the delimeter, preserving the remaingin text.
            */
            auto found = value.find(del);

            Text sub_string_value = "";

            Integer derived_value;

            if (found != std::string::npos) {

                if (value.begin() + found < value.end()) {
                    sub_string_value = value.substr(found + 1);
                }
                value.erase(found);

                derived_value = Integer(sub_string_value);
            }

            return derived_value;
        }

        Size Decimal::find_and_set_scale(Text& value) const {
            /*
                Find the decimal point which is required to for all
                Decimal number text input.
            */

            sys_int scale = 0;

            auto found = value.find(".");

            if (found != std::string::npos) {
                value.at(found) = ' ';

                scale = static_cast<sys_int>(value.size() - found) - 1;
                if (scale < 1) {
                    scale = 0;
                }
            }

            if (scale > decimal_scale()) {

                value.resize(value.size() - (scale - decimal_scale()));
            }

            return scale;
        }

        Decimal& Decimal::update_scale() {

            if (_scale < scale()) {

                _number *= TEN.pow(decimal_scale() - _scale);
            }

            else {
                _number /= TEN.pow(_scale - decimal_scale());
            }

            _scale = decimal_scale();

            return *this;
        }

        Decimal Decimal::update_scale_copy() const {

            Decimal a(*this);

            a.update_scale();

            return a;
        }

        Decimal Decimal::get_ln() const {
            /*
                Perform a Taylor Series estimation of
                the natural logorithm of this number.
            */

            Decimal one(ONE);
            Decimal two(TWO);

            Decimal x = (*this - one) / (*this + one);

            Decimal a = x;   // Derivative of x = a;
            Decimal term = x;

            Decimal power(3);

            Decimal last_iter;

            // Perform the calculation until the last 'last_iter' Decimal
            // is the same as the current calculation 'term'.  

            while (last_iter != term) {
                last_iter = term;

                a *= x;  // Update the derivative.
                a *= x;

                term += (one / power) * a;

                power += two;
            }

            return term * two;
        }
    }
}  // end Olly
