
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

#include "Whole_Number.h"

namespace Olly {
    namespace APM {

        Whole_Number::Whole_Number() : _reg() {
        }

        Whole_Number::Whole_Number(Word value) : _reg(1, value) {
        }

        Whole_Number::Whole_Number(const Text& value, Word base) : _reg() {
            set_numeric_value(value, base);
        }

        Whole_Number::Whole_Number(const Text& value, Word base, Boolean& error) : _reg() {
            error = set_numeric_value(value, base);
        }

        Whole_Number::Whole_Number(const Reg& reg) : _reg(reg) {
        }

        Whole_Number::~Whole_Number() {
        }

        Boolean Whole_Number::is() const {
            return _reg.is();
        }

        Boolean Whole_Number::is_odd() const {
            return _reg.last_reg() & 1;
        }

        Boolean Whole_Number::is_even() const {
            return !is_odd();
        }

        Boolean Whole_Number::operator==(const Whole_Number& b) const {
            return compare(b) == 0;
        }

        Boolean Whole_Number::operator!=(const Whole_Number& b) const {
            return compare(b) != 0;
        }

        Boolean Whole_Number::operator>=(const Whole_Number& b) const {
            return compare(b) >= 0;
        }

        Boolean Whole_Number::operator<=(const Whole_Number& b) const {
            return compare(b) <= 0;
        }

        Boolean Whole_Number::operator>(const Whole_Number& b) const {
            return compare(b) > 0;
        }

        Boolean Whole_Number::operator<(const Whole_Number& b) const {
            return compare(b) < 0;
        }

        sys_float Whole_Number::compare(const Whole_Number& other) const {
            return _reg.compare(other._reg);
        }

        Whole_Number& Whole_Number::operator&=(const Whole_Number& other) {
            _reg &= other._reg;

            trim();

            return *this;
        }

        Whole_Number& Whole_Number::operator|=(const Whole_Number& other) {
            _reg |= other._reg;

            trim();

            return *this;
        }

        Whole_Number& Whole_Number::operator^=(const Whole_Number& other) {
            _reg ^= other._reg;

            trim();

            return *this;
        }

        Whole_Number& Whole_Number::operator<<=(Size index) {
            _reg <<= index;

            trim();

            return *this;
        }

        Whole_Number& Whole_Number::operator>>=(Size index) {
            _reg >>= index;

            trim();

            return *this;
        }

        Whole_Number Whole_Number::bin_comp() const {

            Whole_Number n;

            n._reg = _reg.bin_comp();

            return n;
        }

        Whole_Number Whole_Number::operator&(const Whole_Number& b) const {

            Whole_Number a = *this;

            a &= b;

            return a;
        }

        Whole_Number Whole_Number::operator|(const Whole_Number& b) const {

            Whole_Number a = *this;

            a |= b;

            return a;
        }

        Whole_Number Whole_Number::operator^(const Whole_Number& b) const {

            Whole_Number a = *this;

            a ^= b;

            return a;
        }

        Whole_Number Whole_Number::operator~() const {

            Whole_Number a;

            a._reg = ~_reg;

            return a;
        }

        Whole_Number Whole_Number::operator<<(Size index) const {

            Whole_Number a = *this;

            a <<= index;

            return a;
        }

        Whole_Number Whole_Number::operator>>(Size index) const {

            Whole_Number a = *this;

            a >>= index;

            return a;
        }

        Whole_Number& Whole_Number::operator+=(const Whole_Number& other) {

            Size limit = _reg.size_regs() > other._reg.size_regs() ? _reg.size_regs() : other._reg.size_regs();

            Double_Word n = 0;

            for (Size i = 0; i < limit; i += 1) {

                n = n + _reg.at_reg(i) + other._reg.at_reg(i);

                _reg.at_reg(i) = static_cast<Word>(n);

                n >>= _reg.BITS;
            }

            if (n != 0) {

                _reg.at_reg(limit) = static_cast<Word>(n);
            }

            trim();

            return *this;
        }

        Whole_Number& Whole_Number::operator-=(const Whole_Number& other) {

            if (other > *this) {
                *this = Whole_Number();
                return *this;
            }

            Size limit = _reg.size_regs() > other._reg.size_regs() ? _reg.size_regs() : other._reg.size_regs();

            Double_Word n = 0;

            for (Size i = 0; i < limit; i += 1) {

                n = n + _reg.at_reg(i) - other._reg.at_reg(i);

                _reg.at_reg(i) = static_cast<Word>(n);

                n = ((n >> _reg.BITS) ? -1 : 0);
            }

            trim();

            return *this;
        }

        Whole_Number& Whole_Number::operator*=(const Whole_Number& other) {

            *this = *this * other;

            return *this;
        }

        Whole_Number& Whole_Number::operator/=(const Whole_Number& other) {

            *this = *this / other;

            return *this;
        }

        Whole_Number& Whole_Number::operator%=(const Whole_Number& other) {

            *this = *this % other;

            return *this;
        }

        Whole_Number Whole_Number::operator+(const Whole_Number& b) const {

            Whole_Number a = *this;

            a += b;

            return a;
        }

        Whole_Number Whole_Number::operator-(const Whole_Number& b) const {

            Whole_Number a = *this;

            a -= b;

            return a;
        }

        Whole_Number Whole_Number::operator*(const Whole_Number& b) const {

            Size size_a = _reg.size_regs();
            Size size_b = b._reg.size_regs();

            Reg r((size_a + size_b + 1), 0);

            for (Size j = 0; j < size_b; j += 1) {

                Double_Word n = 0;

                for (Size i = 0; i < size_a; i += 1) {

                    Size k = i + j;

                    n += static_cast<Double_Word>(_reg.at_reg(i)) * b._reg.at_reg(j) + r.at_reg(k);

                    r.at_reg(k) = static_cast<Word>(n);

                    n >>= _reg.BITS;
                }
                r.at_reg(j + size_a) = static_cast<Word>(n);
            }

            r.trim();

            return r;
        }

        Whole_Number Whole_Number::operator/(const Whole_Number& b) const {

            Whole_Number q;
            Whole_Number r;

            div_rem(b, q, r);

            return q;
        }

        Whole_Number Whole_Number::operator%(const Whole_Number& b) const {

            Whole_Number q;
            Whole_Number r;

            div_rem(b, q, r);

            return r;
        }

        Whole_Number& Whole_Number::operator++() {

            ++_reg;

            trim();

            return *this;
        }

        Whole_Number Whole_Number::operator++(int) {

            Whole_Number a(*this);

            operator++();

            return a;
        }

        Whole_Number& Whole_Number::operator--() {

            --_reg;

            trim();

            return *this;
        }

        Whole_Number Whole_Number::operator--(int) {

            Whole_Number a(*this);

            operator--();

            return a;
        }

        void Whole_Number::div_rem(const Whole_Number& other, Whole_Number& qot, Whole_Number& rem) const {

            // Ensure both the qotient and remander are initalized to zero.
            qot = Whole_Number();
            rem = Whole_Number();

            if (!other.is()) {
                // Division by zero.
                return;
            }

            if (other > *this) {
                // Division by a greater value.
                qot = Whole_Number();
                rem = *this;
                return;
            }

            if (*this > other) {

                if (other._reg.size_regs() == 1) {

                    divide_remainder(other._reg.at_reg(0), qot, rem, 0);

                    return;
                }

                sys_int stop = static_cast<sys_int>(other._reg.size_regs() - 1);

                auto d = other._reg.at_reg(stop);

                for (sys_int i = stop - 1; i >= 0; i -= 1) {
                    // Add one to 'd' if any other digits are defined.
                    if (other._reg.at_reg(i) != 0) {
                        d += 1;
                        break;
                    }
                }

                // Perform long division.
                Whole_Number n = *this;

                Whole_Number q = Whole_Number();

                Whole_Number guard = Whole_Number();

                while (n >= other && n != guard) {

                    n.divide_remainder(d, q, rem, stop);

                    qot += q;

                    guard = n;

                    n -= (other * q);

                    q = Whole_Number();
                }

                // Confirm the qoutent is correct.
                q = other * qot;

                while (q < *this) {
                    qot += Whole_Number(1);
                    q += other;
                }

                while (q > *this) {
                    qot -= Whole_Number(1);
                    q -= other;
                }

                // Determine the remainder.
                rem = *this - q;
                return;
            }

            // Return  division by two equal values.
            qot = Whole_Number(1);
            rem = Whole_Number();
        }

        Whole_Number Whole_Number::pow(Size b) const {

            if (b == 1) {
                return *this;
            }
            else if (b == 0) {
                return 1;
            }

            Whole_Number a = *this;
            Whole_Number res = 1;

            while (b) {

                if (b & 1) {

                    res *= a;
                }

                b >>= 1;

                if (b) {
                    a *= a;
                }
            }

            return res;
        }

        Whole_Number Whole_Number::sqrt() const {
            return static_cast<Word>(_reg.lead_bit() - 1);
        }

        Whole_Number Whole_Number::root(const Whole_Number& b) const {

            Size n = b.to_integral<Size>();

            Whole_Number low = 0;
            Whole_Number high = 1;

            while (high.pow(n) <= *this) {
                low = high;
                high *= 2;
            }

            while (low != high - 1) {

                Whole_Number step = (high - low) / 2;

                Whole_Number candidate = low + step;

                auto value = candidate.pow(n);

                if (value == *this) {
                    return candidate;
                }
                if (value < *this) {
                    low = candidate;
                }
                else {
                    high = candidate;
                }
            }

            return low;
        }

        Text Whole_Number::to_string() const {
            return to_string(10);
        }

        Text Whole_Number::to_string(Size base) const {

            if (base == 10 || base == 0 || base == 2 || base == 8 || base == 16) {

                if (!is()) {
                    return "0";
                }

                Whole_Number radix = static_cast<Word>(base != 0 ? base : 10);
                Whole_Number n = *this;

                Text_Stream stream;

                int count = 0;

                while (n.is()) {

                    Whole_Number q;
                    Whole_Number r = n;

                    n.div_rem(radix, q, r);

                    n = q;

                    if (base == 16) {
                        stream << std::hex << r._reg.at_reg(0);
                    }
                    else if (base == 8) {
                        stream << std::oct << r._reg.at_reg(0);
                    }
                    else {
                        stream << r._reg.at_reg(0);
                    }

                    if (base == 10) {
                        count += 1;

                        if (count == 3) {
                            stream << ',';
                            count = 0;
                        }
                    }
                }
                Text res = stream.str();

                if (res.back() == ',') {
                    res.pop_back();
                }

                std::reverse(res.begin(), res.end());

                switch (base) {

                case (2):
                    res = "0b" + res;
                    break;

                case (8):
                    res = "0o" + res;
                    break;

                case (16):
                    res = "0x" + res;
                    break;
                }

                return res;
            }

            return "";
        }

        const Whole_Number::Reg& Whole_Number::get_Binary_Register() const {
            return _reg;
        }

        void Whole_Number::divide_remainder(Word d, Whole_Number& q, Whole_Number& r, sys_int stop) const {

            Double_Word n(0);

            for (sys_int i = static_cast<sys_int>(_reg.size_regs() - 1); i >= stop; i -= 1) {

                n += _reg.at_reg(i);

                q._reg.at_reg(static_cast<Size>(i) - stop) = static_cast<Word>(n / d);

                n %= d;
                n <<= _reg.BITS;
            }

            r._reg.at_reg(0) = n >> _reg.BITS;

            return;
        }

        Boolean Whole_Number::set_numeric_value(const Text& text, const Word& base) {

            if (base == 10) {  // Parse a decimal number.

                Word x = 0;

                for (const auto n : text) {

                    if (!std::isspace(n) && n != ',') {

                        x = n - '0';

                        if (x >= 0 && x < base) {

                            operator*=(base);
                            operator+=(x);
                        }
                        else {
                            _reg = Reg();
                            return true;
                        }
                    }
                }
            }

            else if (base == 16) {  // Parse a hexidecimal number.

                Word x;

                for (const auto n : text) {

                    if (!std::isspace(n)) {

                        switch (n) {

                        case('a'):
                        case('A'):
                            x = 10;
                            break;
                        case('b'):
                        case('B'):
                            x = 11;
                            break;
                        case('c'):
                        case('C'):
                            x = 12;
                            break;
                        case('d'):
                        case('D'):
                            x = 13;
                            break;
                        case('e'):
                        case('E'):
                            x = 14;
                            break;
                        case('f'):
                        case('F'):
                            x = 15;
                            break;
                        default:
                            x = (n - '0');
                        }

                        if (x >= 0 && x < base) {

                            operator*=(base);
                            operator+=(x);
                        }
                        else {
                            _reg = Reg();
                            return true;
                        }
                    }
                }
            }

            else if (base == 8) {  // Parse an octal number.

                Word x;

                for (const auto n : text) {

                    if (!std::isspace(n)) {

                        x = n - '0';

                        if (x >= 0 && x < base) {

                            operator*=(base);
                            operator+=(x);
                        }
                        else {
                            _reg = Reg();
                            return true;
                        }
                    }
                }
            }

            else if (base == 2) {  // Parse a binary number.

                for (const auto n : text) {

                    operator<<=(1);

                    if (n == '1') {
                        operator+=(1);
                    }
                    else if (n != '0') {
                        _reg = Reg();
                        return true;
                    }
                }
            }

            return false;
        }

        void Whole_Number::trim() {
            _reg.trim();
        }
    }
}
