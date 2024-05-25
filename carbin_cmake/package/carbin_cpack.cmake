#
# Copyright 2023-2024 The EA Authors.
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

message(STATUS "CPACK FOR ${PRETTY_NAME}")
if (${PRETTY_NAME} MATCHES "Ubuntu")
    MESSAGE(STATUS "Linux dist: ubuntu, build deb package")
    include(carbin_deb)
elseif (${PRETTY_NAME} MATCHES "darwin")
    MESSAGE(STATUS "Linux dist: macos build dmg package")
elseif (${PRETTY_NAME} MATCHES "centos" OR ${PRETTY_NAME} MATCHES "CentOS")
    MESSAGE(STATUS "Linux dist: centos build rpm package")
    include(carbin_rpm)
endif ()