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

#include "Integer.h"
#include "numerical_strings.h"

namespace Olly {
    namespace APM {

        /********************************************************************************************/
        //
        //                              'Decimal' Class Declaration
        //
        /********************************************************************************************/

        class Decimal {

            const enum class ROUNDING_MODE {
                toward_zero = 0, half_up, half_down, half_even, half_odd, ceil, floor, away_from_zero
            };

        public:

            static sys_int scale();
            static void    scale(const sys_int& scale);

            static Text rounding_mode();
            static void rounding_mode(const Text& mode);

            static Integer decimal_denominator();

            static Decimal e();
            static Decimal pi();
            static Decimal ln2();

            Decimal();
            Decimal(Text value);
            virtual ~Decimal();

            Decimal(Decimal&& obj)                 = default;
            Decimal(const Decimal& obj)            = default;
            Decimal& operator=(const Decimal& obj) = default;
            Decimal& operator=(Decimal&& obj)      = default;

            Boolean is() const;

            Boolean is_odd()      const;
            Boolean is_even()     const;
            Boolean is_positive() const;
            Boolean is_negative() const;
            Boolean is_zero()     const;

            Boolean operator==(const Decimal& b) const;
            Boolean operator!=(const Decimal& b) const;
            Boolean operator< (const Decimal& b) const;
            Boolean operator> (const Decimal& b) const;
            Boolean operator<=(const Decimal& b) const;
            Boolean operator>=(const Decimal& b) const;

            sys_float compare(const Decimal& other) const;

            Decimal operator+() const;
            Decimal operator-() const;

            Decimal& operator+=(const Decimal& b);
            Decimal& operator-=(const Decimal& b);
            Decimal& operator*=(const Decimal& b);
            Decimal& operator/=(const Decimal& b);
            Decimal& operator%=(const Decimal& b);

            Decimal operator+(const Decimal& b) const;
            Decimal operator-(const Decimal& b) const;
            Decimal operator*(const Decimal& b) const;
            Decimal operator/(const Decimal& b) const;
            Decimal operator%(const Decimal& b) const;

            void div_rem(const Decimal& b, Decimal& qot, Decimal& rem) const;
            void num_den(Decimal& num, Decimal& den)                   const;

            Decimal pow(Size b) const;

            Decimal abs()       const;
            Decimal inverse()   const;
            Decimal ceil()      const;
            Decimal floor()     const;
            // Decimal factorial() const;

            Text sign()                             const;
            Text to_string()                        const;
            Text to_string(Size base, sys_int sign) const;

            Decimal    ln()                 const;

            template<typename N>
            N to_integral() const;

            const Integer& get_Integer() const;

        private:

            static const Integer ONE;
            static const Integer TWO;
            static const Integer TEN;

            static const Size DEF_SCALE = 16;
            static const Size MIN_SCALE = 8;
            static const Size MAX_SCALE = 10000;

            static Boolean& scale_not_configured();
            static Size& decimal_scale();
            static Integer& denominator();

            static Integer& integer_pi();
            static Integer& integer_e();
            static Integer& integer_ln2();

            static Decimal& decimal_pi();
            static Decimal& decimal_e();
            static Decimal& decimal_ln2();

            static ROUNDING_MODE& round_mode();

            Integer _number;
            Size    _scale;

            Decimal(sys_int value);
            Decimal(const Integer& value);
            Decimal(const Whole_Number& value);

            void set_decimal_text(Text& text);
            void set_decimal_exponent(Integer& exponent);

            Integer get_sub_text_value(Text& value, Text del) const;

            Size find_and_set_scale(Text& value) const;

            Decimal& update_scale();
            Decimal  update_scale_copy() const;

            Decimal get_ln() const;
        };

        template<typename N>
        inline N Decimal::to_integral() const {

            N a = _number.to_integral<N>();

            if (std::is_signed<N>::value && is_negative()) {

                a *= -1;
            }

            return a;
        }
    }
}
