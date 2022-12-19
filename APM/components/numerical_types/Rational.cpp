
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

#include "Rational.h"

namespace Olly {
    namespace APM {

        Rational::Rational() : _numerator(), _denominator(1) {
        }

        Rational::Rational(Text value, sys_int base) : _numerator(), _denominator(1) {

            // Locate and set the denominator.
            _denominator = Integer(get_sub_text_value(value, "/")).get_Whole_Number();

            if (!_denominator.is()) {
                return;
            }

            value = trim(value);

            // Look if a leading integer is present and get it.
            Text lead_val_str = "";

            auto found = value.find_last_of(' ');

            if (found != std::string::npos) {

                lead_val_str = value.substr(0, found);

                value.erase(0, found);
            }

            // Define the leading integer.
            Integer lead_value = Integer(lead_val_str);

            _numerator = Integer(value);

            // Add any leading value to the number.
            if (lead_value.is()) {

                _numerator = _numerator + (lead_value * _denominator);
            }
        }

        Rational::~Rational() {
        }

        Boolean Rational::is() const {
            return _numerator.is() && _denominator.is();
        }

        Boolean Rational::is_positive() const {
            return _numerator.is_positive();
        }

        Boolean Rational::is_negative() const {
            return _numerator.is_negative();
        }

        Boolean Rational::is_zero() const {
            return _numerator.is_zero();
        }

        Boolean Rational::operator==(const Rational& b) const {
            return compare(b) == 0;
        }

        Boolean Rational::operator!=(const Rational& b) const {
            return compare(b) != 0;
        }

        Boolean Rational::operator>=(const Rational& b) const {
            return compare(b) >= 0;
        }

        Boolean Rational::operator<=(const Rational& b) const {
            return compare(b) <= 0;
        }

        Boolean Rational::operator>(const Rational& b) const {
            return compare(b) > 0;
        }

        Boolean Rational::operator<(const Rational& b) const {
            return compare(b) < 0;
        }

        sys_float Rational::compare(const Rational& b) const {

            if (_denominator != b._denominator) {

                Rational x, y;

                x._numerator = _numerator * b._denominator;
                y._numerator = b._numerator * _denominator;

                return x._numerator.compare(y._numerator);
            }

            return _numerator.compare(b._numerator);
        }

        Rational Rational::operator+(const Rational& b) const {

            Rational a;

            if (_denominator != b._denominator) {

                if (is_zero()) {
                    a = b;
                }
                else if (b.is()) {


                    a._numerator = _numerator * b._denominator;
                    a._numerator = a._numerator + (b._numerator * _denominator);
                    a._denominator = _denominator * b._denominator;
                }
            }
            else {
                a._numerator = _numerator + b._numerator;
                a._denominator = _denominator;
            }

            return a;
        }

        Rational Rational::operator-(const Rational& b) const {
            return operator+(-b);
        }

        Rational Rational::operator*(const Rational& b) const {

            Rational a;

            a._numerator = _numerator * b._numerator;
            a._denominator = _denominator * b._denominator;

            return a;
        }

        Rational Rational::operator/(const Rational& b) const {
            return operator*(b.inv());
        }

        Rational Rational::operator%(const Rational& b) const {

            Rational a = operator/(b);

            a._numerator = a._numerator / a._denominator;
            a._denominator = 1;

            return a;
        }

        Rational Rational::operator+() const {
            return *this;
        }

        Rational Rational::operator-() const {

            Rational a;

            a._numerator = -_numerator;
            a._denominator = _denominator;

            return a;
        }

        Rational Rational::abs() const {
            return is_negative() ? operator-() : *this;
        }

        Rational Rational::inv() const {

            Rational a;

            a._numerator = _denominator;
            a._denominator = _numerator.get_Whole_Number();

            if (is_negative()) {
                a._numerator = a._numerator * -1;
            }

            return a;
        }

        Text Rational::sign() const {
            return _numerator.sign();
        }

        Text Rational::to_string() const {
            return to_string(10, -1);
        }

        Text Rational::to_string(Size base, sys_int sign) const {

            Text result = _numerator.to_string(base, sign);

            result += "/";

            result += _denominator.to_string(base);

            return result;
        }

        const Integer& Rational::get_numerator() const {
            return _numerator;
        }

        const Whole_Number& Rational::get_denominator() const {
            return _denominator;
        }

        Integer Rational::get_sub_text_value(Text& value, Text del) const {
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
    }
}
