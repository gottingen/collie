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

#[[
carbin_cc_binary(
        NAME hashes_example
        SOURCES hashes_example.cc
        DEPS tedis::tedis ${CARBIN_DEPS_LINK}
        COPTS ${USER_CXX_FLAGS}
)
]]
#[[
carbin_cc_binary(
        NAME foo_ex
        SOURCES foo_ex.cc
        DEPS changeme::foo ${CARBIN_DEPS_LINK}
        COPTS ${USER_CXX_FLAGS}
)
]]
add_subdirectory(cli)
add_subdirectory(strings)
add_subdirectory(tc)
add_subdirectory(module)
add_subdirectory(table)
add_subdirectory(toml)
add_subdirectory(log)
add_subdirectory(container)
add_subdirectory(meta)
add_subdirectory(simd)
add_subdirectory(taskflow)
add_subdirectory(utility)
