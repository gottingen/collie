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


#include <collie/simd/simd.h>
#ifndef COLLIE_SIMD_NO_SUPPORTED_ARCHITECTURE

namespace collie::simd
{

    template class batch<char>;
    template class batch<unsigned char>;
    template class batch<signed char>;
    template class batch<unsigned short>;
    template class batch<signed short>;
    template class batch<unsigned int>;
    template class batch<signed int>;
    template class batch<unsigned long>;
    template class batch<signed long>;
    template class batch<float>;
#if !COLLIE_SIMD_WITH_NEON || COLLIE_SIMD_WITH_NEON64
    template class batch<double>;
#endif
}
#endif
