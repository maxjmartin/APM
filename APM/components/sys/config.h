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

#include <iostream>
#include <sstream>
#include <vector>

#ifdef _MSC_VER
#include "Windows.h"
#endif

/*  Place holder for porting over to another operating system.
#ifdef _MSC_VER

#else

#endif

*/

namespace Olly {

#if _WIN32 || _WIN64
#if _WIN64
    using   sys_int = long int;
    using  sys_long = long long int;
    using  sys_uint = unsigned long int;
    using sys_ulong = unsigned long long int;
    using sys_float = double;
#else
    using   sys_int = int;
    using  sys_long = long int;
    using  sys_uint = unsigned int;
    using sys_ulong = unsigned long int;
    using sys_float = double;
#endif
#endif

#if __GNUC__
#if __x86_64__ || __ppc64__
    using   sys_int = long int;
    using  sys_long = long long int;
    using  sys_uint = unsigned long int;
    using sys_ulong = unsigned long long int;
    using sys_float = double;
#else
    using   sys_int = int;
    using  sys_long = long int;
    using  sys_uint = unsigned int;
    using sys_ulong = unsigned long int;
    using sys_float = double;
#endif
#endif

    /********************************************************************************************/
    //
    //                             Fundamental Type Declarations
    //
    //          The declarations below should be used throughout the entire project.
    //
    /********************************************************************************************/

    typedef     bool                 Boolean;
    typedef     std::size_t          Size;
    typedef     char                 Char;
    typedef     std::string          Text;
    typedef     std::stringstream    Text_Stream;
    typedef     std::vector<Text>    Text_Tokens;;

    static const sys_float           NOT_A_NUMBER = std::numeric_limits<sys_float>::quiet_NaN();
}