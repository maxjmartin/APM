
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

        Decimal::Decimal() : _number() {
        }

        Decimal::Decimal(sys_int value) : _number(value) {
            _number *= denominator();
        }

        Decimal::Decimal(const Integer& value) : _number(value) {
            _number *= denominator();
        }

        Decimal::Decimal(const Whole_Number& value) : _number(value) {
            _number *= denominator();
        }

        Decimal::Decimal(Text value) : _number() {
            if (scale_not_configured()) {
                scale(Decimal::DEF_SCALE);
            }

            value = to_lower(value);

            if (value.size() > 1) {

                if (value.find('.') != Text::npos) {
                    set_decimal(value);
                }

                else if (value.find('/') != Text::npos) {
                    set_rational(value);
                }

                else if (value[0] == '0' && std::isalpha(value[1])) {

                    if (value[0] == '0' && value[1] == 'u') {
                        value[0] = ' ';
                        value[1] = ' ';
                        set_whole(value);
                    }

                    else if (value[0] == '0' && value[1] == 'b') {
                        value[0] = ' ';
                        value[1] = ' ';
                        set_binary(value);
                    }

                    else if (value[0] == '0' && value[1] == 'x') {
                        value[0] = ' ';
                        value[1] = ' ';
                        set_heximal(value);
                    }

                    else if (value[0] == '0' && value[1] == 'o') {
                        value[0] = ' ';
                        value[1] = ' ';
                        set_octal(value);
                    }

                    else {
                        set_integer(value);
                    }
                }

                else {
                    set_integer(value);
                }
            }

            else {
                set_integer(value);
            }
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

        Boolean Decimal::is_undefined() const {
            return _number.is_undefined();
        }

        Boolean Decimal::is_defined() const {
            return _number.is_defined();
        }

        Boolean Decimal::is_nan() const {
            return _number.is_nan();
        }

        Boolean Decimal::is_finite() const {
            return _number.is_finite();
        }

        Boolean Decimal::is_infinite() const {
            return _number.is_infinite();
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

            return _number.compare(b._number);
        }

        Decimal& Decimal::operator+=(const Decimal& b) {

            _number += b._number;

            return *this;
        }

        Decimal& Decimal::operator-=(const Decimal& b) {
            return operator+=(-b);
        }

        Decimal& Decimal::operator*=(const Decimal& b) {

            _number *= b._number;

            _number /= denominator();

            return *this;
        }

        Decimal& Decimal::operator/=(const Decimal& b) {

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

            return a;
        }

        void Decimal::div_rem(const Decimal& b, Decimal& qot, Decimal& rem) const {
            (_number * denominator()).div_rem(b._number, qot._number, rem._number);
        }

        Decimal Decimal::abs() const {

            Decimal a;

            a._number = _number.abs();

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

        Decimal Decimal::gcd(const Decimal& b) const {
            
            if (is_defined() && b.is_defined()) {

                Decimal a(*this);

                a._number = a._number.gcd(b._number);

                return a;
            }

            return Integer::UNDEF;
        }

        Decimal Decimal::pow(const Decimal& b) const {

            if (is_defined() && b.is_defined()) {

                Size n = b.to_integral<Size>();

                if (n == 2) {
                    Decimal a = *this;
                    return a * a;
                }

                if (n == 1) {
                    return *this;
                }

                if (b == 0) {
                    return 1;
                }

                Decimal a = *this;
                Decimal res = 1;

                while (n) {

                    if (n & 1) {

                        res *= a;
                    }

                    n >>= 1;

                    if (n) {
                        a *= a;
                    }
                }

                return res;
            }

            return Integer::UNDEF;
        }

        Decimal Decimal::root(const Decimal& b) const {

            // e^(ln(*this)/b) - Will need to see if this is faster, in the future.

            if (is_defined() && b.is_defined()) {

                Size n = b.to_integral<Size>();

                if (n > MAX_SCALE || is_negative()) {
                    return Integer::UNDEF;
                }

                Decimal a = *this * denominator().pow(n - 1);

                a._number = a._number.root(n);

                return a;
            }

            return Integer::UNDEF;
        }

        Decimal Decimal::hypot(const Decimal& b) const {

            return (operator*(*this) + (b * b)).root(2);
        }

        Decimal Decimal::hypot(const Decimal& b, const Decimal& c) const {

            return (operator*(*this) + (b * b) + (c * c)).root(2);
        }

        Decimal Decimal::ln() const {

            if ((_number == denominator())) {
                return Decimal();
            }
            else if ((_number == integer_e())) {
                return Decimal(1);
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

        Decimal Decimal::log2() const {
            return log(Decimal(2));
        }

        Decimal Decimal::log10() const {
            return log(Decimal(10));
        }

        Decimal Decimal::log(const Decimal& b) const {
            
            if (is_defined() && is_positive()) {

                Decimal a = ln() / b.ln();

                Decimal c(a);

                if (b.pow(c) == *this) {
                    return c;
                }

                return a;
            }

            return Integer::UNDEF;
        }

        Decimal Decimal::sin() const {
            
            Decimal sin_x = abs();

            Boolean neg = is_negative() ? true : false;

            /*
                First reduce value closure to 0.5
                to improve speed of convergance.  
            */

            if (sin_x > decimal_360()) {
                sin_x %= decimal_360();
            }

            if (sin_x > decimal_180()) {
                sin_x %= decimal_180();
                neg = !neg;
            }

            sin_x *= (decimal_pi() / decimal_180());  // Convert to radians.  

            sin_x = sin_x.get_sin();

            return neg ? -sin_x : sin_x;
        }

        Decimal Decimal::cos() const {

            Decimal sin_x = sin();
            Decimal one(1);

            return (one - (sin_x * sin_x)).root(2); 
        }

        Decimal Decimal::tan() const {

            Decimal one(1);
            Decimal sin_x = sin();
            Decimal cos_x = (one - (sin_x * sin_x)).root(2);

            return sin_x / cos_x;
        }


        Decimal Decimal::asin() const {

            Decimal asin_x = abs();

            Boolean neg = is_negative() ? true : false;

            /*
                First reduce value closure to 0.5
                to improve speed of convergance.
            */

            sys_int power_of_2 = 0;

            Decimal limit("0.5");
            Decimal one(ONE);
            Decimal two(TWO);

            while (asin_x > limit) {

                asin_x = asin_x / (one + (one + asin_x * asin_x).root(two));

                power_of_2 += 1;
            }

            asin_x = asin_x.get_asin() * two.pow(power_of_2);

            return neg ? -asin_x : asin_x;
        }

        Decimal Decimal::acos() const {
            
            return (decimal_pi() / 2) - asin();
        }

        Decimal Decimal::atan() const {

            Decimal sinh_x = abs();

            Boolean neg = is_negative() ? true : false;

            /*
                First reduce value closure to 0.5
                to improve speed of convergance.
            */

            sys_int power_of_2 = 0;

            Decimal limit("0.1");
            Decimal one(ONE);
            Decimal two(TWO);

            while (sinh_x > limit) {

                sinh_x = sinh_x / (one + (one + sinh_x * sinh_x).root(two));

                power_of_2 += 1;
            }

            sinh_x = sinh_x.get_atan() * two.pow(power_of_2);

            return neg ? -sinh_x : sinh_x;
        }

        Decimal Decimal::sinh() const {
            
            Decimal sinh_x = abs();

            Boolean neg = is_negative() ? true : false;

            sinh_x = sinh_x.get_sinh();

            return neg ? -sinh_x : sinh_x;
        }

        Decimal Decimal::cosh() const {

            Decimal cosh_x = abs();

            Boolean neg = is_negative() ? true : false;

            cosh_x = cosh_x.get_cosh();

            return neg ? -cosh_x : cosh_x;
        }

        Decimal Decimal::tanh() const {
            return sinh() / cosh();
        }

        Decimal Decimal::asinh() const {
            return (*this + ( *this * *this + 1).root(2)).ln();
        }

        Decimal Decimal::acosh() const {
            return (*this + (*this * *this - 1).root(2)).ln();
        }

        Decimal Decimal::atanh() const {
            return Decimal("0.5") * ((Decimal(1) + *this) / (Decimal(1) - *this)).ln();
        }

        Text Decimal::sign() const {
            return _number.sign();
        }

        Text Decimal::to_string() const {
            return to_string(10, -1);
        }

        Text Decimal::to_string(Size base, sys_int sign) const {

            Integer qot, rem;
            _number.div_rem(denominator(), qot, rem);

            Text result = qot.to_string(0, -1);

            Text decimal_string = rem.to_string(0, -1);

            if (decimal_string.size() || decimal_string.size() < decimal_scale()) {

                decimal_string.insert(0, decimal_scale() - decimal_string.size(), '0');
            }

            if (rem.is()) {
                result += "." + decimal_string;
            }
            else {
                result += ".0";
            }

            return result;
        }

        const Integer& Decimal::get_Integer() const {
            return _number;
        }

        Decimal Decimal::get_ln() const {
            /*
                Perform a Taylor Series estimation of
                the natural logorithm of this number.
            */

            Decimal one(ONE);
            Decimal two(TWO);

            Decimal x = (*this - one) / (*this + one); 

            Decimal ln_x_iter = x;
            Decimal ln_x      = x;

            Decimal denominator(3);

            Decimal last_iter;

            // Perform the calculation until the last 'last_iter' Decimal
            // is the same as the current calculation 'ln_x'.  

            while (last_iter != ln_x) {
                last_iter = ln_x;

                ln_x_iter *= x;
                ln_x_iter *= x;

                ln_x += (one / denominator) * ln_x_iter;

                denominator += two;
            }

            return ln_x * two;
        }

        Decimal Decimal::get_sin() const {

            Decimal one   = 1;
            Decimal x     = *this;
            Decimal sin_x = x;

            Decimal sin_x_iter = x;

            Decimal sin_power(3);
            Decimal sin_factorial(6);

            Decimal last;

            // Perform the calculation until the last 'last_iter' Decimal
            // is the same as the current calculation 'term'.  

            while (last.get_Integer().get_Whole_Number() != sin_x.get_Integer().get_Whole_Number()) {

                last = sin_x;

                sin_x = -sin_x;

                sin_x_iter *= x;
                sin_x_iter *= x;

                sin_x += (one / sin_factorial) * sin_x_iter;

                sin_power     += one;
                sin_factorial *= sin_power;

                sin_power     += one;
                sin_factorial *= sin_power;
            }

            return sin_x;
        }

        Decimal Decimal::get_asin() const {
            
            /*
                Perform a Taylor Series estimation of
                the natural logorithm of this number.
            */

            Decimal one(ONE);
            Decimal two(TWO);

            Decimal x = *this;
            Decimal asin_x = x;

            Decimal asin_x_iter = x;

            Decimal asin_power(3);

            Decimal num_coef(1);
            Decimal den_coef(2);

            Decimal last;

            // Perform the calculation until the last 'last_iter' Decimal
            // is the same as the current calculation 'term'.  

            while (last.get_Integer().get_Whole_Number() != asin_x.get_Integer().get_Whole_Number()) {

                last = asin_x;

                // asin_x = -asin_x;

                asin_x_iter *= x;
                asin_x_iter *= x;

                asin_x += (one / (den_coef * asin_power)) * (num_coef * asin_x_iter);

                num_coef   *= asin_power;
                asin_power += one;
                den_coef   *= asin_power;
                asin_power += one;
            }

            return asin_x;
        }

        Decimal Decimal::get_atan() const {

            /*
                Perform a Taylor Series estimation of
                the natural logorithm of this number.
            */

            Decimal one(ONE);
            Decimal two(TWO);

            Decimal x = *this;
            Decimal sinh_x = x;

            Decimal sinh_x_iter = x;

            Decimal atan_power(3);

            Decimal last;

            // Perform the calculation until the last 'last_iter' Decimal
            // is the same as the current calculation 'term'.  

            while (last.get_Integer().get_Whole_Number() != sinh_x.get_Integer().get_Whole_Number()) {

                last = sinh_x;

                sinh_x = -sinh_x;

                sinh_x_iter *= x;
                sinh_x_iter *= x;

                sinh_x += (one / atan_power) * sinh_x_iter;

                atan_power += two;
            }

            return sinh_x;
        }

        Decimal Decimal::get_sinh() const {

            Decimal one = 1;
            Decimal x = *this;
            Decimal sinh_x = x;

            Decimal sinh_x_iter = x;

            Decimal sin_power(3);
            Decimal sin_factorial(6);

            Decimal last;

            // Perform the calculation until the last 'last_iter' Decimal
            // is the same as the current calculation 'term'.  

            while (last.get_Integer().get_Whole_Number() != sinh_x.get_Integer().get_Whole_Number()) {

                last = sinh_x;

                sinh_x_iter *= x;
                sinh_x_iter *= x;

                sinh_x += (one / sin_factorial) * sinh_x_iter;

                sin_power += one;
                sin_factorial *= sin_power;

                sin_power += one;
                sin_factorial *= sin_power;
            }

            return sinh_x;
        }

        Decimal Decimal::get_cosh() const {

            Decimal one = 1;
            Decimal x = *this;
            Decimal cosh_x = one;

            Decimal cosh_x_iter = one;

            Decimal cos_power(2);
            Decimal cos_factorial(2);

            Decimal last;

            // Perform the calculation until the last 'last_iter' Decimal
            // is the same as the current calculation 'term'.  

            while (last.get_Integer().get_Whole_Number() != cosh_x.get_Integer().get_Whole_Number()) {

                last = cosh_x;

                cosh_x_iter *= x;
                cosh_x_iter *= x;

                cosh_x += (one / cos_factorial) * cosh_x_iter;

                cos_power += one;
                cos_factorial *= cos_power;

                cos_power += one;
                cos_factorial *= cos_power;
            }

            return cosh_x;
        }

        void Decimal::set_integer(Text& value) {
            _number = Integer(value) * denominator();
        }

        void Decimal::set_decimal(Text& value) {

            lrtrim(value);

            if (!value.empty()) {

                Integer exponent = get_sub_text_value(value, "e");

                Size scale = find_and_set_scale(value);

                _number = Integer(value);

                if (scale < decimal_scale()) {
                    _number = _number * (denominator() / TEN.pow(scale));
                }

                set_decimal_exponent(exponent);
            }
        }

        void Decimal::set_rational(Text& value) {

            // Locate and set the denominator.
            Integer den(get_sub_text_value(value, "/"));

            if (!den.is()) {
                _number = Integer::UNDEF;
                return;
            }

            lrtrim(value);

            // Look if a leading integer is present and get it.
            Text lead_val_str = "";

            auto found = value.find_last_of(' ');

            if (found != std::string::npos) {

                lead_val_str = value.substr(0, found);

                value.erase(0, found);
            }

            // Define the leading integer.
            Integer lead_value(lead_val_str);

            _number = Integer(value);

            // Add any leading value to the number.
            if (lead_value.is()) {

                _number = _number + (lead_value * den);
            }

            _number *= denominator();
            _number /= den;
        }

        void Decimal::set_whole(Text& value) {
            _number = Integer(value, 10);
        }

        void Decimal::set_binary(Text& value) {
            _number = Integer(value, 2);
        }

        void Decimal::set_octal(Text& value) {
            _number = Integer(value, 8);
        }

        void Decimal::set_heximal(Text& value) {
            _number = Integer(value, 16);
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
    }
}  // end Olly
