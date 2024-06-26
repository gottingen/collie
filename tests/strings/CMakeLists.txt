#
# Copyright 2023 The titan-search Authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

find_package(Threads REQUIRED)

carbin_cc_test(
        NAME inlined_string_test
        MODULE base
        SOURCES inlined_string_test.cc
        CXXOPTS ${USER_CXX_FLAGS}
)

carbin_cc_test(
        NAME case_conv_test
        MODULE base
        SOURCES case_conv_test.cc
        CXXOPTS ${USER_CXX_FLAGS}
)

carbin_cc_test(
        NAME replace_test
        MODULE base
        SOURCES replace_test.cc
        CXXOPTS ${USER_CXX_FLAGS}
)

carbin_cc_test(
        NAME trim_test
        MODULE base
        SOURCES trim_test.cc
        CXXOPTS ${USER_CXX_FLAGS}
)