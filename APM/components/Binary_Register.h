#pragma once

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

#include <bitset>
#include "sys/config.h"
#include "sys/string_support_functions.h"

namespace Olly {

    namespace APM {

        /********************************************************************************************/
        //
        //                                 'Binary_Register' class
        //
        //        The Binary_Register class implements a series of binary registers sized to
        //        the integral type passed during to the template at definition.
        //
        //        Support for all of the binary operation is provides, along with binary
        //        based mathematical operations.  The implimentation is little endian.
        //
        /********************************************************************************************/

        template<typename N>
        class Binary_Register {

            static_assert(std::numeric_limits<N>::is_integer, "The Binary_Register template argument T must be an unsigned integral.");
            static_assert(std::numeric_limits<N>::is_signed ? false : true, "The Binary_Register template argument T must be an unsigned integral.");
            // static_assert(std::numeric_limits<N>::digits >= 16 ? true : false, "The Binary_Register template argument T must be a minimum of 16 bits wide.");

        public:
            typedef N              Word;
            typedef std::vector<N> Register;

            static const N    MASK = ~N(0);
            static const Size BITS = std::numeric_limits<N>::digits;

            Binary_Register();
            Binary_Register(const Text& value, Text base = "10");

            Binary_Register(const Size& size, Word value = 0);
            virtual ~Binary_Register();

            Binary_Register(Binary_Register&& obj)                 = default;
            Binary_Register(const Binary_Register& obj)            = default;
            Binary_Register& operator=(const Binary_Register& obj) = default;
            Binary_Register& operator=(Binary_Register&& obj)      = default;

            Boolean    is() const;                             // Boolean conversion.
            Boolean   all() const;                             // Boolean test for all bits being set to 1.
            Size    count() const;                             // The count of bits set to 1.
            Size lead_bit() const;                             // Return the lead bit.
            Size last_bit() const;                             // Return the last bit.
            Word lead_reg() const;                             // Return the leading register.
            Word last_reg() const;                             // Return the last register.

            Boolean at_bit(Size index) const;                  // Return the value of a bit at the index.

            Word& at_reg(Size index);                          // Return the word at the indexed register.
            Word  at_reg(Size index) const;

            Text to_string()       const;                      // Return a string representation at radix 10.
            Text to_string(N base) const;                      // Return a string representation at radix 'base'.
            void to_string(Text_Stream& stream) const;         // Send a string representation to a stream_type.

            Size size_bits() const;                            // Get the total size in bits of the register.
            Size size_regs() const;                            // Get the total size of words in the register.

            Binary_Register& set();                            // Set all bits to true.
            Binary_Register& set(Size index);                  // Set a bit at 'index' to true.

            Binary_Register& reset();                          // Set all bits to false.
            Binary_Register& reset(Size index);                // Set a bit at 'index' to false.

            Binary_Register& flip();                           // Flip the truth of every bit in the register.
            Binary_Register& flip(Size index);                 // Flip the truth of a bit at 'index'.

            Boolean operator==(const Binary_Register& b) const;
            Boolean operator!=(const Binary_Register& b) const;
            Boolean operator< (const Binary_Register& b) const;
            Boolean operator> (const Binary_Register& b) const;
            Boolean operator<=(const Binary_Register& b) const;
            Boolean operator>=(const Binary_Register& b) const;

            Binary_Register& operator&=(const Binary_Register& other);
            Binary_Register& operator|=(const Binary_Register& other);
            Binary_Register& operator^=(const Binary_Register& other);

            Binary_Register& operator<<=(Size index);
            Binary_Register& operator>>=(Size index);

            Binary_Register operator&(const Binary_Register& b) const;
            Binary_Register operator|(const Binary_Register& b) const;
            Binary_Register operator^(const Binary_Register& b) const;
            Binary_Register operator~() const;

            Binary_Register operator<<(Size index) const;
            Binary_Register operator>>(Size index) const;

            Binary_Register& operator+=(const Binary_Register& other);
            Binary_Register& operator-=(const Binary_Register& other);
            Binary_Register& operator*=(const Binary_Register& other);
            Binary_Register& operator/=(const Binary_Register& other);
            Binary_Register& operator%=(const Binary_Register& other);

            Binary_Register operator+(const Binary_Register& b) const;
            Binary_Register operator-(const Binary_Register& b) const;
            Binary_Register operator*(const Binary_Register& b) const;
            Binary_Register operator/(const Binary_Register& b) const;
            Binary_Register operator%(const Binary_Register& b) const;

            Binary_Register& operator++();
            Binary_Register  operator++(int);

            Binary_Register& operator--();
            Binary_Register  operator--(int);

            template<typename I>
            N to_integral() const;                // Cast the register to an integral of type T.

            Binary_Register  bin_comp() const;    // Return the binary compliment of the register.

            // Get both the qotient and the remainder of the regester divided by 'other'.
            void div_rem(Binary_Register& other, Binary_Register& qot, Binary_Register& rem) const;

            sys_float compare(const Binary_Register& other) const;  // Compare two registers.
                                                                    //  0.0 = equality.
                                                                    //  1.0 = greater than.
                                                                    // -1.0 = less than.

            Binary_Register& trim();    // Trim all words of 0 from the end of the register, until
                                        // a set word is encounter, or the last word is encountered.

        private:
            typedef std::bitset<BITS>           single_prc_bitset;
            typedef std::bitset<BITS + BITS>    double_prc_bitset;

            static const N ONE = 1;

            Register _reg;

            void get_shift_index(Size& index, Size& reg_index, Size& bit_index) const;

            void divide_remainder(const Binary_Register& x, Binary_Register y, Binary_Register& q, Binary_Register& r) const;

            Text get_string(N base) const;

            void  left_shift_bits(Size& word_index, Size& bit_index);
            void right_shift_bits(Size& word_index, Size& bit_index);
        };

        /********************************************************************************************/
        //
        //                              'Binary_Register' implimentation
        //
        /********************************************************************************************/

        template<typename N>
        inline Binary_Register<N>::Binary_Register() : _reg(1, 0) {
        }

        template<typename N>
        inline Binary_Register<N>::Binary_Register(const Size& size, Word value) : _reg((size > 0 ? size : 1), value) {
        }

        template<typename N>
        inline Binary_Register<N>::Binary_Register(const Text& value, Text base) : _reg(1, 0) {

            N base_radix = to<N>(base);                // Get the base radix to use.

            Binary_Register<N> radix(1, base_radix);   // Define a Binary_Register to act as the radix.

            for (auto i : value) {                     // Loop through each digit and add it to the Binary_Register.

                Text digit_str = "";

                digit_str.push_back(i);

                N n = to<N>(digit_str);

                if (n < base_radix) {
                    Binary_Register<N> digit(1, n);

                    operator*=(radix);
                    operator+=(digit);
                }
            }
        }

        template<typename N>
        inline Binary_Register<N>::~Binary_Register() {
        }

        template<typename N>
        inline Boolean Binary_Register<N>::is() const {

            for (auto i : _reg) {

                if (i) {
                    return true;
                }
            }
            return false;
        }

        template<typename N>
        inline Boolean Binary_Register<N>::all() const {

            for (auto i : _reg) {

                if (i != MASK) {
                    return false;
                }
            }
            return true;
        }

        template<typename N>
        inline Size Binary_Register<N>::count() const {

            Size count = 0;

            for (const auto i : _reg) {

                auto n = i;

                while (n > 0) {

                    if (n & 1) {
                        count += 1;
                    }
                    n >>= 1;
                }
            }

            return count;
        }

        template<typename N>
        inline Size Binary_Register<N>::lead_bit() const {

            Size word_index = _reg.size();

            Word mask = (ONE << (BITS - ONE));

            for (auto i = _reg.crbegin(); i != _reg.crend(); ++i) {
                word_index -= 1;

                auto a = *i;

                Size bit_index = BITS;

                while (a) {

                    if (a & mask) {
                        return bit_index + (word_index * BITS);
                    }
                    a <<= 1;
                    bit_index -= 1;
                }
            }

            return 0;
        }

        template<typename N>
        inline Size Binary_Register<N>::last_bit() const {

            Size word_index = 0;

            Word mask = 1;

            for (auto i = _reg.cbegin(); i != _reg.cend(); ++i) {

                auto a = *i;

                Size bit_index = 1;

                while (a) {

                    if (a & mask) {
                        return bit_index + (word_index * BITS);
                    }
                    a >>= 1;
                    bit_index += 1;
                }
                word_index += 1;
            }

            return 0;
        }

        template<typename N>
        inline N Binary_Register<N>::lead_reg() const {

            if (_reg.empty()) {
                return N(0);
            }

            return _reg.back();
        }

        template<typename N>
        inline N Binary_Register<N>::last_reg() const {

            if (_reg.empty()) {
                return N(0);
            }

            return _reg.front();
        }

        template<typename N>
        inline Boolean Binary_Register<N>::at_bit(Size index) const {

            Size reg_index, bit_index;
            get_shift_index(index, reg_index, bit_index);

            if (reg_index < _reg.size()) {

                return _reg[reg_index] & (ONE << (bit_index - ONE));
            }

            return false;
        }

        template<typename N>
        inline N& Binary_Register<N>::at_reg(Size index) {

            while (index >= _reg.size()) {

                _reg.push_back(0);
            }

            return _reg[index];
        }

        template<typename N>
        inline N Binary_Register<N>::at_reg(Size index) const {

            if (index < _reg.size()) {

                return _reg[index];
            }

            return Word(0);
        }

        template<typename N>
        inline Text Binary_Register<N>::to_string() const {

            return to_string(10);
        }

        template<typename N>
        inline Text Binary_Register<N>::to_string(N base) const {

            if (base > 360) {
                return "Radix must be between 0 and 360.";
            }

            if (base == 0) {
                Text_Stream stream;

                to_string(stream);

                return stream.str();
            }

            if (!is()) {
                return "0";
            }

            return get_string(base);
        }

        template<typename N>
        inline void Binary_Register<N>::to_string(Text_Stream& stream) const {

            Size i = _reg.size();

            while (i-- > 1) {
                stream << "word[" << i << "] = " << single_prc_bitset(_reg[i]) << "\n";
            }
            stream << "word[" << 0 << "] = " << single_prc_bitset(_reg[i]);
        }

        template<typename N>
        inline Size Binary_Register<N>::size_bits() const {
            return _reg.size() * BITS;
        }

        template<typename N>
        inline Size Binary_Register<N>::size_regs() const {
            return _reg.size();
        }

        template<typename N>
        inline Binary_Register<N>& Binary_Register<N>::set() {

            for (auto i = _reg.begin(); i != _reg.end(); ++i) {
                *i = MASK;
            }

            return *this;
        }

        template<typename N>
        inline Binary_Register<N>& Binary_Register<N>::set(Size index) {

            Size reg_index, bit_index;
            get_shift_index(index, reg_index, bit_index);

            while (reg_index >= _reg.size()) {

                _reg.push_back(0);
            }

            _reg[reg_index] |= (ONE << (bit_index - ONE));

            return *this;
        }

        template<typename N>
        inline Binary_Register<N>& Binary_Register<N>::reset() {

            for (Size i = 0, end = _reg.size(); i < end; i += 1) {
                _reg[i] = Word(0);
            }

            return *this;
        }

        template<typename N>
        inline Binary_Register<N>& Binary_Register<N>::reset(Size index) {

            Size reg_index, bit_index;
            get_shift_index(index, reg_index, bit_index);

            while (reg_index >= _reg.size()) {

                _reg.push_back(0);
            }

            _reg[reg_index] &= ~(1 << (bit_index - 1));

            return *this;
        }

        template<typename N>
        inline Binary_Register<N>& Binary_Register<N>::flip() {

            for (Size i = 0, end = _reg.size(); i < end; i += 1) {
                _reg[i] = ~_reg[i];
            }

            return *this;
        }

        template<typename N>
        inline Binary_Register<N>& Binary_Register<N>::flip(Size index) {

            Size reg_index, bit_index;
            get_shift_index(index, reg_index, bit_index);

            while (reg_index >= _reg.size()) {

                _reg.push_back(0);
            }

            _reg[reg_index] ^= (1 << (bit_index - 1));

            return *this;
        }

        template<typename N>
        inline Boolean Binary_Register<N>::operator==(const Binary_Register<N>& b) const {
            return compare(b) == 0;
        }

        template<typename N>
        inline Boolean Binary_Register<N>::operator!=(const Binary_Register<N>& b) const {
            return compare(b) != 0;
        }

        template<typename N>
        inline Boolean Binary_Register<N>::operator<(const Binary_Register<N>& b) const {
            return compare(b) < 0;
        }

        template<typename N>
        inline Boolean Binary_Register<N>::operator>(const Binary_Register<N>& b) const {
            return compare(b) > 0;
        }

        template<typename N>
        inline Boolean Binary_Register<N>::operator<=(const Binary_Register<N>& b) const {
            return compare(b) <= 0;
        }

        template<typename N>
        inline Boolean Binary_Register<N>::operator>=(const Binary_Register<N>& b) const {
            return compare(b) >= 0;
        }

        template<typename N>
        inline Binary_Register<N>& Binary_Register<N>::operator&=(const Binary_Register<N>& other) {

            while (_reg.size() < other._reg.size()) {

                _reg.push_back(0);
            }

            for (Size i = 0, end = _reg.size(); i < end; i += 1) {
                _reg[i] &= other.at_reg(i);
            }

            return *this;
        }

        template<typename N>
        inline Binary_Register<N>& Binary_Register<N>::operator|=(const Binary_Register<N>& other) {

            while (_reg.size() < other._reg.size()) {

                _reg.push_back(0);
            }

            for (Size i = 0, end = _reg.size(); i < end; i += 1) {
                _reg[i] |= other.at_reg(i);
            }

            return *this;
        }

        template<typename N>
        inline Binary_Register<N>& Binary_Register<N>::operator^=(const Binary_Register<N>& other) {

            while (_reg.size() < other._reg.size()) {

                _reg.push_back(0);
            }

            for (Size i = 0, end = _reg.size(); i < end; i += 1) {
                _reg[i] ^= other.at_reg(i);
            }

            return *this;
        }

        template<typename N>
        inline Binary_Register<N>& Binary_Register<N>::operator<<=(Size index) {

            Size word_index, bit_index;

            get_shift_index(index, word_index, bit_index);

            if (word_index) {
                _reg.insert(_reg.begin(), word_index, 0);
            }

            if (bit_index) {

                left_shift_bits(word_index, bit_index);
            }

            return *this;
        }

        template<typename N>
        inline Binary_Register<N>& Binary_Register<N>::operator>>=(Size index) {

            Size word_index, bit_index;

            get_shift_index(index, word_index, bit_index);

            if (word_index) {

                if (word_index < _reg.size()) {

                    _reg.erase(_reg.begin(), _reg.begin() + word_index);
                }
                else {
                    for (auto i = _reg.begin(), end = _reg.end(); i != end; ++i) {
                        *i = 0;
                    }

                    return *this;
                }
            }

            if (bit_index) {

                right_shift_bits(word_index, bit_index);
            }

            return *this;
        }

        template<typename N>
        inline Binary_Register<N> Binary_Register<N>::operator&(const Binary_Register<N>& b) const {

            Binary_Register<N> a(*this);

            a &= b;

            return a;
        }

        template<typename N>
        inline Binary_Register<N> Binary_Register<N>::operator|(const Binary_Register<N>& b) const {

            Binary_Register<N> a(*this);

            a |= b;

            return a;
        }

        template<typename N>
        inline Binary_Register<N> Binary_Register<N>::operator^(const Binary_Register<N>& b) const {

            Binary_Register<N> a(*this);

            a ^= b;

            return a;
        }

        template<typename N>
        inline Binary_Register<N> Binary_Register<N>::operator~() const {

            Binary_Register<N> a = *this;

            for (Size i = 0, end = a._reg.size(); i < end; i += 1) {
                a._reg[i] = ~a._reg[i];
            }

            return a;
        }

        template<typename N>
        inline Binary_Register<N> Binary_Register<N>::operator<<(Size index) const {

            Binary_Register<N> a(*this);

            a <<= index;

            return a;
        }

        template<typename N>
        inline Binary_Register<N> Binary_Register<N>::operator>>(Size index) const {

            Binary_Register<N> a(*this);

            a >>= index;

            return a;
        }

        template<typename N>
        inline Binary_Register<N>& Binary_Register<N>::operator+=(const Binary_Register<N>& other) {

            Binary_Register<N> b(other);
            Binary_Register<N> c;

            while (b.is()) {

                c = (*this & b) << 1;

                *this ^= b;

                b = c;
            }

            return *this;
        }

        template<typename N>
        inline Binary_Register<N>& Binary_Register<N>::operator-=(const Binary_Register<N>& other) {

            if (other >= *this) {
                return reset();
            }

            Binary_Register<N> b = other;

            while (b.size_regs() < size_regs()) {
                b._reg.push_back(0);
            }

            b = b.bin_comp();

            b._reg.push_back(0);  // Add a word to handle the two's compliment overflow.

            *this += b;

            _reg.pop_back(); // Get rid of the two's compliment overflow.

            return *this;
        }

        template<typename N>
        inline Binary_Register<N>& Binary_Register<N>::operator*=(const Binary_Register<N>& other) {

            *this = *this * other;

            return *this;
        }

        template<typename N>
        inline Binary_Register<N>& Binary_Register<N>::operator/=(const Binary_Register<N>& other) {

            *this = *this / other;

            return *this;
        }

        template<typename N>
        inline Binary_Register<N>& Binary_Register<N>::operator%=(const Binary_Register<N>& other) {

            *this = *this % other;

            return *this;
        }

        template<typename N>
        inline Binary_Register<N> Binary_Register<N>::operator+(const Binary_Register<N>& b) const {

            Binary_Register<N> a = *this;

            a += b;

            return a;
        }

        template<typename N>
        inline Binary_Register<N> Binary_Register<N>::operator-(const Binary_Register<N>& b) const {

            Binary_Register<N> a = *this;

            a -= b;

            return a;
        }

        template<typename N>
        inline Binary_Register<N> Binary_Register<N>::operator*(const Binary_Register<N>& b) const {

            Size count = 0;

            Binary_Register<N> x;
            Binary_Register<N> y = b;

            while (y.is()) {

                if (y.at_bit(1)) {
                    x += (*this << count);
                }

                count += 1;
                y >>= 1;
            }

            return x;
        }

        template<typename N>
        inline Binary_Register<N> Binary_Register<N>::operator/(const Binary_Register<N>& b) const {

            Binary_Register<N> q;
            Binary_Register<N> r = *this;

            divide_remainder(*this, b, q, r);

            return q;
        }

        template<typename N>
        inline Binary_Register<N> Binary_Register<N>::operator%(const Binary_Register<N>& b) const {

            Binary_Register<N> q;
            Binary_Register<N> r = *this;

            divide_remainder(*this, b, q, r);

            return r;
        }

        template<typename N>
        inline Binary_Register<N>& Binary_Register<N>::operator++() {

            Binary_Register<N> one(1, 1);

            operator+=(one);

            return *this;
        }

        template<typename N>
        inline Binary_Register<N> Binary_Register<N>::operator++(int) {

            Binary_Register<N> a(*this);

            operator++();

            return a;
        }

        template<typename N>
        inline Binary_Register<N>& Binary_Register<N>::operator--() {

            Binary_Register<N> one(1, 1);

            operator-=(one);

            return *this;
        }

        template<typename N>
        inline Binary_Register<N> Binary_Register<N>::operator--(int) {

            Binary_Register<N> a(*this);

            operator--();

            return a;
        }

        template<typename N>
        inline Binary_Register<N> Binary_Register<N>::bin_comp() const {

            Binary_Register<N> a = ~*this;
            Binary_Register<N> one(1, 1);

            a += one;

            return a;
        }

        template<typename N>
        inline void Binary_Register<N>::div_rem(Binary_Register& other, Binary_Register& qot, Binary_Register& rem) const {

            rem = *this;

            divide_remainder(*this, other, qot, rem);
        }

        template<typename N>
        inline sys_float Binary_Register<N>::compare(const Binary_Register<N>& other) const {

            Size i = size_regs() > other.size_regs() ? size_regs() : other.size_regs();

            while (i-- > 0) {

                auto x = at_reg(i);
                auto y = other.at_reg(i);

                if (x > y) {
                    return 1.0;
                }

                if (x < y) {
                    return -1.0;
                }
            }

            return 0;
        }

        template<typename N>
        inline Binary_Register<N>& Binary_Register<N>::trim() {

            while (!_reg.empty() && _reg.back() == 0) {

                _reg.pop_back();
            }

            if (_reg.empty()) {

                _reg.push_back(0);
            }

            return *this;
        }

        template<typename N>
        inline void Binary_Register<N>::get_shift_index(Size& index, Size& reg_index, Size& bit_index) const {

            if (index) {

                if (index >= BITS) {

                    reg_index = index / BITS;
                    bit_index = index % BITS;

                    if (!bit_index) {
                        --reg_index;
                        bit_index = BITS;
                    }

                    return;
                }

                reg_index = 0;
                bit_index = index;

                return;
            }

            reg_index = 0;
            bit_index = 0;
        }

        template<typename N>
        inline void Binary_Register<N>::divide_remainder(const Binary_Register<N>& x, Binary_Register<N> y, Binary_Register<N>& q, Binary_Register<N>& r) const {

            if (!y.is() || !x.is() || x < y) {
                return;
            }

            Size lead_x = x.lead_bit();
            Size lead_y = y.lead_bit();

            Size bit_dif = (lead_x - lead_y);

            y <<= bit_dif;

            bit_dif += 2;

            while (bit_dif-- > 1) {

                if (r >= y) {
                    q.set(bit_dif);
                    r -= y;
                }
                y >>= 1;
            }
        }

        template<typename N>
        inline Text Binary_Register<N>::get_string(N base) const {

            Binary_Register<N> radix(1, base);
            Binary_Register<N> n = *this;

            Text_Stream stream;

            while (n.is()) {

                Binary_Register<N> q;
                Binary_Register<N> r = n;

                divide_remainder(n, radix, q, r);

                n = q;

                stream << r.at_reg(0);
            }

            Text res = stream.str();
            std::reverse(res.begin(), res.end());

            return res;
        }

        template<typename N>
        inline void Binary_Register<N>::left_shift_bits(Size& word_index, Size& bit_index) {

            Size i = _reg.size();

            _reg.push_back(0);

            auto bit_mask = double_prc_bitset(MASK);

            while (i-- > 0) {

                auto buffer = double_prc_bitset();
                buffer |= double_prc_bitset(_reg[i]);

                buffer <<= bit_index;

                _reg[i] = static_cast<N>((buffer & bit_mask).to_ullong());

                buffer >>= BITS;
                buffer |= double_prc_bitset(_reg[i + 1]);

                _reg[i + 1] = static_cast<N>(buffer.to_ullong());
            }

            if (_reg.back() == 0) {
                _reg.pop_back();
            }
        }

        template<typename N>
        inline void Binary_Register<N>::right_shift_bits(Size& word_index, Size& bit_index) {

            Boolean pop_back = word_index ? false : true;

            if (word_index) {
                word_index -= 1;
            }
            _reg.push_back(0);

            auto inv_index = BITS - bit_index;

            Size end = (_reg.size() - 1);

            auto bit_mask = double_prc_bitset(MASK);

            for (Size i = 0; i < end; i += 1) {

                auto buffer = double_prc_bitset();
                buffer |= double_prc_bitset(_reg[i + 1]);

                buffer <<= inv_index;

                _reg[i] >>= bit_index;
                _reg[i] |= static_cast<N>((buffer & bit_mask).to_ullong());
            }
            _reg[end] >>= bit_index;


            while (word_index-- > 0) {
                _reg.push_back(0);
            }

            if (pop_back) {
                _reg.pop_back();
            }
        }

        template<typename N>
        template<typename I>
        inline N Binary_Register<N>::to_integral() const {
            static_assert(std::numeric_limits<I>::is_integer, "Integral required.");

            if (!_reg.empty()) {

                auto bits_of_I = std::numeric_limits<I>::digits;

                if (bits_of_I >= BITS && !_reg.empty()) {

                    return I(_reg.front());
                }

                I n = 0;

                for (sys_int i = BITS / bits_of_I; i >= 0; i -= 1) {

                    n <<= bits_of_I;
                    n += at_reg(i);
                }

                return static_cast<N>(n);
            }

            return I(0);
        }
    }
}