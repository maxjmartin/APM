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

#include "../Binary_Register.h"

namespace Olly {
    namespace APM {

        /********************************************************************************************/
        //
        //                              'Whole_Number' Class Declaration
        //
        /********************************************************************************************/

        class Whole_Number {

        public:
            typedef sys_uint              Word;
            typedef sys_ulong             Double_Word;
            typedef Binary_Register<Word> Reg;

            Whole_Number();
            Whole_Number(Word value);
            Whole_Number(const Text& value, Word base = 10);
            Whole_Number(const Text& value, Word base, Boolean& error);
            virtual ~Whole_Number();

            Whole_Number(Whole_Number&& obj)                 = default;
            Whole_Number(const Whole_Number& obj)            = default;
            Whole_Number& operator=(const Whole_Number& obj) = default;
            Whole_Number& operator=(Whole_Number&& obj)      = default;

            Boolean is() const;

            Boolean is_odd()  const;
            Boolean is_even() const;

            Boolean operator==(const Whole_Number& b) const;
            Boolean operator!=(const Whole_Number& b) const;
            Boolean operator< (const Whole_Number& b) const;
            Boolean operator> (const Whole_Number& b) const;
            Boolean operator<=(const Whole_Number& b) const;
            Boolean operator>=(const Whole_Number& b) const;

            sys_float compare(const Whole_Number& other) const;

            Whole_Number& operator&=(const Whole_Number& other);
            Whole_Number& operator|=(const Whole_Number& other);
            Whole_Number& operator^=(const Whole_Number& other);

            Whole_Number& operator<<=(Size index);
            Whole_Number& operator>>=(Size index);

            Whole_Number bin_comp() const;

            Whole_Number operator&(const Whole_Number& b) const;
            Whole_Number operator|(const Whole_Number& b) const;
            Whole_Number operator^(const Whole_Number& b) const;
            Whole_Number operator~() const;

            Whole_Number operator<<(Size index) const;
            Whole_Number operator>>(Size index) const;

            Whole_Number& operator+=(const Whole_Number& other);
            Whole_Number& operator-=(const Whole_Number& other);
            Whole_Number& operator*=(const Whole_Number& other);
            Whole_Number& operator/=(const Whole_Number& other);
            Whole_Number& operator%=(const Whole_Number& other);

            Whole_Number operator+(const Whole_Number& b) const;
            Whole_Number operator-(const Whole_Number& b) const;
            Whole_Number operator*(const Whole_Number& b) const;
            Whole_Number operator/(const Whole_Number& b) const;
            Whole_Number operator%(const Whole_Number& b) const;

            Whole_Number& operator++();
            Whole_Number  operator++(int);

            Whole_Number& operator--();
            Whole_Number  operator--(int);

            void div_rem(const Whole_Number& other, Whole_Number& qot, Whole_Number& rem) const;

            Whole_Number pow(Size b) const;

            Whole_Number sqrt() const;
            Whole_Number  root(const Whole_Number& b) const;

            Text to_string()			   const;
            Text to_string(Size base) const;

            template<typename N>
            N to_integral() const;

            const Reg& get_Binary_Register() const;

        private:

            Reg _reg;

            Whole_Number(const Reg& reg);

            void trim();

            void divide_remainder(Word d, Whole_Number& q, Whole_Number& r, sys_int stop) const;

            Boolean set_numeric_value(const Text& text, const Word& base);
        };

        template<typename N>
        inline N Whole_Number::to_integral() const {
            return _reg.to_integral<N>();
        }
    }
}

