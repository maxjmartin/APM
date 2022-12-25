
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
                decimal_scale()        = scl;
                denominator()          = Decimal::TEN.pow(scl);

                integer_e();
                integer_pi();
                integer_ln2();

                decimal_360();
                decimal_180();
                decimal_45();
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

        Decimal& Decimal::decimal_360() {
            
            static Decimal d(360);

            return d;
        }

        Decimal& Decimal::decimal_180() {
            
            static Decimal d(180);

            return d;
        }

        Decimal& Decimal::decimal_45() {
            
            static Decimal d(45);

            return d;
        }

        Decimal::ROUNDING_MODE& Decimal::round_mode() {

            ROUNDING_MODE mode = ROUNDING_MODE::half_even;

            return mode;
        }
    }
}
