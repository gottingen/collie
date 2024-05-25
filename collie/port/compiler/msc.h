//
// Copyright (C) 2024 EA group inc.
// Author: Jeff.li lijippy@163.com
// All rights reserved.
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//
//

#pragma once
///////////////////////////////////////////////////////
/// I DO not using msvc,but for extension in future, I just put here, and not compile and test yet.
////////////////////////////////////////
#if defined(COLLIE_COMPILER_MSVC)
#   error "can not define COLLIE_COMPILER_MSVC manully"
#elif defined(_MSC_VER)
#   define COLLIE_COMPILER_MSVC 1
#   define COLLIE_COMPILER_MICROSOFT 1
#   define COLLIE_COMPILER_VERSION _MSC_VER
#   define COLLIE_COMPILER_NAME "Microsoft Visual C++"

#   if defined(__clang__)
#define COLLIE_COMPILER_CLANG_CL 1
#   endif

#   define COLLIE_STANDARD_LIBRARY_MSVC 1
#   define COLLIE_STANDARD_LIBRARY_MICROSOFT 1

#   if (_MSC_VER <= 1200) // If VC6.x and earlier...
#       if (_MSC_VER < 1200)
#           define COLLIE_COMPILER_MSVCOLD 1
#       else
#           define COLLIE_COMPILER_MSVC6 1
#       endif

#       if (_MSC_VER < 1200) // If VC5.x or earlier...
#           define COLLIE_COMPILER_NO_TEMPLATE_SPECIALIZATION 1
#       endif
#       define COLLIE_COMPILER_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS 1     // The compiler compiles this OK, but executes it wrong. Fixed in VC7.0
#       define COLLIE_COMPILER_NO_VOID_RETURNS 1                             // The compiler fails to compile such cases. Fixed in VC7.0
#       define COLLIE_COMPILER_NO_EXCEPTION_STD_NAMESPACE 1                  // The compiler fails to compile such cases. Fixed in VC7.0
#       define COLLIE_COMPILER_NO_DEDUCED_TYPENAME 1                         // The compiler fails to compile such cases. Fixed in VC7.0
#       define COLLIE_COMPILER_NO_STATIC_CONSTANTS 1                         // The compiler fails to compile such cases. Fixed in VC7.0
#       define COLLIE_COMPILER_NO_COVARIANT_RETURN_TYPE 1                    // The compiler fails to compile such cases. Fixed in VC7.1
#       define COLLIE_COMPILER_NO_ARGUMENT_DEPENDENT_LOOKUP 1                // The compiler compiles this OK, but executes it wrong. Fixed in VC7.1
#       define COLLIE_COMPILER_NO_TEMPLATE_TEMPLATES 1                       // The compiler fails to compile such cases. Fixed in VC7.1
#       define COLLIE_COMPILER_NO_TEMPLATE_PARTIAL_SPECIALIZATION 1          // The compiler fails to compile such cases. Fixed in VC7.1
#       define COLLIE_COMPILER_NO_MEMBER_TEMPLATE_FRIENDS 1                  // The compiler fails to compile such cases. Fixed in VC7.1
//#define COLLIE_COMPILER_NO_MEMBER_TEMPLATES 1                       // VC6.x supports member templates properly 95% of the time. So do we flag the remaining 5%?
//#define COLLIE_COMPILER_NO_MEMBER_TEMPLATE_SPECIALIZATION 1         // VC6.x supports member templates properly 95% of the time. So do we flag the remaining 5%?

#   elif (_MSC_VER <= 1300) // If VC7.0 and earlier...
#       define COLLIE_COMPILER_MSVC7 1

#       define COLLIE_COMPILER_NO_COVARIANT_RETURN_TYPE 1                    // The compiler fails to compile such cases. Fixed in VC7.1
#       define COLLIE_COMPILER_NO_ARGUMENT_DEPENDENT_LOOKUP 1                // The compiler compiles this OK, but executes it wrong. Fixed in VC7.1
#       define COLLIE_COMPILER_NO_TEMPLATE_TEMPLATES 1                       // The compiler fails to compile such cases. Fixed in VC7.1
#       define COLLIE_COMPILER_NO_TEMPLATE_PARTIAL_SPECIALIZATION 1          // The compiler fails to compile such cases. Fixed in VC7.1
#       define COLLIE_COMPILER_NO_MEMBER_TEMPLATE_FRIENDS 1                  // The compiler fails to compile such cases. Fixed in VC7.1
#       define COLLIE_COMPILER_NO_MEMBER_FUNCTION_SPECIALIZATION 1           // This is the case only for VC7.0 and not VC6 or VC7.1+. Fixed in VC7.1
            //#define COLLIE_COMPILER_NO_MEMBER_TEMPLATES 1                       // VC7.0 supports member templates properly 95% of the time. So do we flag the remaining 5%?

#   elif (_MSC_VER < 1400) // VS2003       _MSC_VER of 1300 means VC7 (VS2003)
            // The VC7.1 and later compiler is fairly close to the C++ standard
            // and thus has no compiler limitations that we are concerned about.
#       define COLLIE_COMPILER_MSVC7_2003 1
#d      efine COLLIE_COMPILER_MSVC7_1    1

#   elif (_MSC_VER < 1500) // VS2005       _MSC_VER of 1400 means VC8 (VS2005)
#       define COLLIE_COMPILER_MSVC8_2005 1
#       define COLLIE_COMPILER_MSVC8_0    1

#   elif (_MSC_VER < 1600) // VS2008.      _MSC_VER of 1500 means VC9 (VS2008)
#       define COLLIE_COMPILER_MSVC9_2008 1
#       define COLLIE_COMPILER_MSVC9_0    1

#   elif (_MSC_VER < 1700) // VS2010       _MSC_VER of 1600 means VC10 (VS2010)
#       define COLLIE_COMPILER_MSVC_2010 1
#       define COLLIE_COMPILER_MSVC10_0  1

#  elif (_MSC_VER < 1800) // VS2012       _MSC_VER of 1700 means VS2011/VS2012
#       define COLLIE_COMPILER_MSVC_2011 1   // Microsoft changed the name to VS2012 before shipping, despite referring to it as VS2011 up to just a few weeks before shipping.
#       define COLLIE_COMPILER_MSVC11_0  1
#       define COLLIE_COMPILER_MSVC_2012 1
#       define COLLIE_COMPILER_MSVC12_0  1

#  elif (_MSC_VER < 1900) // VS2013       _MSC_VER of 1800 means VS2013
#       define COLLIE_COMPILER_MSVC_2013 1
#       define COLLIE_COMPILER_MSVC13_0  1

#   elif (_MSC_VER < 1910) // VS2015       _MSC_VER of 1900 means VS2015
#       define COLLIE_COMPILER_MSVC_2015 1
#       define COLLIE_COMPILER_MSVC14_0  1

#   elif (_MSC_VER < 1911) // VS2017       _MSC_VER of 1910 means VS2017
#       define COLLIE_COMPILER_MSVC_2017 1
#       define COLLIE_COMPILER_MSVC15_0  1

#   endif
#endif
