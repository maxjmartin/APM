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

#include "Whole_Number.h"
#include "Integer.h"

namespace Olly {
    namespace APM {

        /********************************************************************************************/
        //
        //                              'Rational' Class Declaration
        //
        /********************************************************************************************/

        class Rational {

        public:
            Rational();
            Rational(Text value, sys_int base = 10);
            virtual ~Rational();

            Rational(Rational&& obj)                 = default;
            Rational(const Rational& obj)            = default;
            Rational& operator=(const Rational& obj) = default;
            Rational& operator=(Rational&& obj)      = default;

            Boolean is() const;

            Boolean is_positive() const;
            Boolean is_negative() const;
            Boolean is_zero()     const;

            Boolean operator==(const Rational& b) const;
            Boolean operator!=(const Rational& b) const;
            Boolean operator< (const Rational& b) const;
            Boolean operator> (const Rational& b) const;
            Boolean operator<=(const Rational& b) const;
            Boolean operator>=(const Rational& b) const;

            sys_float compare(const Rational& other) const;

            Rational operator+() const;
            Rational operator-() const;

            Rational operator+(const Rational& b) const;
            Rational operator-(const Rational& b) const;
            Rational operator*(const Rational& b) const;
            Rational operator/(const Rational& b) const;
            Rational operator%(const Rational& b) const;

            Rational abs() const;
            Rational inv() const;

            Text sign()                             const;
            Text to_string()			            const;
            Text to_string(Size base, sys_int sign) const;

            const Integer& get_numerator()   const;
            const Whole_Number& get_denominator() const;

        private:

            Integer      _numerator;
            Whole_Number _denominator;

            Integer get_sub_text_value(Text& value, Text del) const;
        };
    }
}
