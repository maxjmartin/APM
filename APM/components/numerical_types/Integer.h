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

namespace Olly {
    namespace APM {

        /********************************************************************************************/
        //
        //                              'Integer' Class Declaration
        //
        /********************************************************************************************/

        class Integer {
            const enum class SIGN {
                nan = 0, undef, neg_infinity, negative, zero, positive, pos_infinity
                /*
                    The category of number able to be defined within the class, with the exception
                    of a complex number.
                */
            };

        public:

            static const Integer   NaN;
            static const Integer   UNDEF;
            static const Integer   POS_INFINITY;
            static const Integer   NEG_INFINITY;

            Integer();
            Integer(sys_int value);
            Integer(const Whole_Number& value);
            Integer(Text value, sys_int base = 10);
            virtual ~Integer();

            Integer(Integer&& obj)                 = default;
            Integer(const Integer& obj)            = default;
            Integer& operator=(const Integer& obj) = default;
            Integer& operator=(Integer&& obj)      = default;

            Boolean is() const;

            Boolean is_odd()       const;
            Boolean is_even()      const;
            Boolean is_positive()  const;
            Boolean is_negative()  const;
            Boolean is_zero()      const;
            Boolean is_undefined() const;
            Boolean is_defined()   const;
            Boolean is_nan()       const;
            Boolean is_finite()    const;
            Boolean is_infinite()  const;

            Boolean operator==(const Integer& b) const;
            Boolean operator!=(const Integer& b) const;
            Boolean operator< (const Integer& b) const;
            Boolean operator> (const Integer& b) const;
            Boolean operator<=(const Integer& b) const;
            Boolean operator>=(const Integer& b) const;

            sys_float compare(const Integer& other) const;

            Integer operator+() const;
            Integer operator-() const;

            Integer& operator+=(const Integer& b);
            Integer& operator-=(const Integer& b);
            Integer& operator*=(const Integer& b);
            Integer& operator/=(const Integer& b);
            Integer& operator%=(const Integer& b);

            Integer operator+(const Integer& b) const;
            Integer operator-(const Integer& b) const;
            Integer operator*(const Integer& b) const;
            Integer operator/(const Integer& b) const;
            Integer operator%(const Integer& b) const;

            Integer& operator++();
            Integer  operator++(int);

            Integer& operator--();
            Integer  operator--(int);

            void div_rem(const Integer& other, Integer& qot, Integer& rem) const;

            Integer pow( Size    b) const;
            Integer root(Size    b) const;
            Integer gcd (Integer b) const;

            Integer sqrt() const;
            Integer  abs() const;

            Text sign()                             const;
            Text to_string()			            const;
            Text to_string(Size base, sys_int sign) const;

            template<typename N>
            N to_integral() const;

            const Whole_Number& get_Whole_Number() const;

        private:

            Whole_Number _number;
            SIGN         _sign;

            Integer(SIGN sign);

            void invert_sign();
            void check_for_zero();
        };

        template<typename N>
        inline N Integer::to_integral() const {

            N a = _number.to_integral<N>();

            if (std::is_signed<N>::value && is_negative()) {

                a *= -1;
            }

            return a;
        }
    }
}
