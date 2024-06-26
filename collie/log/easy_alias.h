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

#include <collie/log/logging.h>

/**
 * 1. replace the default logger `collie::log::default_logger_raw()` with your own logger
 * 2. replace the `ABC_` prefix with your own prefix
 * 3. enjoy the easy alias for example `set up a logger for rpc named rpc_logger`
 * Example:
 *   1. copy easy_alias.h to your project named rpc_logger.h
 *   2. modify the rpc_logger.h, add the following code to the header file
 *   @code {.cpp}
 *   #include <collie/log/logger.h>
 *   #include <collie/log/clog.h>
 *   #include "rpc_logger.h"
 *   static std::shared_ptr<collie::log::logger> rpc_logger;
 *   void setup_rpc_logger() {
 *      rpc_logger = collie::log::daily_logger_mt("daily_logger", "logs/daily.txt", 2, 30);
 *      collie::log::initialize_logger(rpc_logger);
 *   }
 *   @endcode
 *   3. modify the rpc_logger.h, replace the `ABC_` prefix with `RPC_`
 *   4. modify the rpc_logger.h, replace the `collie::log::default_logger_raw()` with `rpc_logger`
 *   5. write a test code in main.cc
 *   @code {.cpp}
 *   #include "rpc_logger.h"
 *   int main() {
 *      setup_rpc_logger();
 *      RPC_LOG(INFO) << "Hello, World!";
 *      return 0;
 *   }
 *   @endcode
 *  6. compile and run the test code
 *  7. enjoy the easy alias for your own logger
 *  8. more information about the logger, please refer to the examples/log/stream_log.cc and examples/log/log_example.cc
 */
/// default logger
#define ABC_LOG(SEVERITY) LOG_LOGGER(SEVERITY, collie::log::default_logger_raw())
#define ABC_LOG_IF(SEVERITY, condition) LOG_LOGGER_IF(SEVERITY, (condition), collie::log::default_logger_raw())
#define ABC_LOG_EVERY_N(SEVERITY, N) LOG_LOGGER_EVERY_N(SEVERITY, N, collie::log::default_logger_raw())
#define ABC_LOG_IF_EVERY_N(SEVERITY, N, condition) LOG_LOGGER_IF_EVERY_N(SEVERITY, (N), (condition), collie::log::default_logger_raw())
#define ABC_LOG_FIRST_N(SEVERITY, N) LOG_LOGGER_FIRST_N(SEVERITY, (N), collie::log::default_logger_raw())
#define ABC_LOG_IF_FIRST_N(SEVERITY, N, condition) LOG_LOGGER_IF_FIRST_N(SEVERITY, (N), (condition), collie::log::default_logger_raw())
#define ABC_LOG_EVERY_T(SEVERITY, seconds) LOG_LOGGER_EVERY_T(SEVERITY, (seconds), collie::log::default_logger_raw())
#define ABC_LOG_IF_EVERY_T(SEVERITY, seconds, condition) LOG_LOGGER_IF_EVERY_T(SEVERITY, (seconds), (condition), collie::log::default_logger_raw())
#define ABC_LOG_ONCE(SEVERITY) LOG_LOGGER_ONCE(SEVERITY, collie::log::default_logger_raw())
#define ABC_LOG_IF_ONCE(SEVERITY, condition) LOG_LOGGER_IF_ONCE(SEVERITY, (condition), collie::log::default_logger_raw())

#define ABC_CHECK(condition) CHECK_LOGGER(condition, collie::log::default_logger_raw())
#define ABC_CHECK_NOTNULL(val) CHECK_LOGGER(collie::ptr(val) != nullptr,collie::log::default_logger_raw())
#define ABC_CHECK_EQ(val1, val2) CHECK_OP_LOGGER(_EQ, ==, val1, val2, collie::log::default_logger_raw())
#define ABC_CHECK_NE(val1, val2) CHECK_OP_LOGGER(_NE, !=, val1, val2, collie::log::default_logger_raw())
#define ABC_CHECK_LE(val1, val2) CHECK_OP_LOGGER(_LE, <=, val1, val2, collie::log::default_logger_raw())
#define ABC_CHECK_LT(val1, val2) CHECK_OP_LOGGER(_LT, <, val1, val2, collie::log::default_logger_raw())
#define ABC_CHECK_GE(val1, val2) CHECK_OP_LOGGER(_GE, >=, val1, val2, collie::log::default_logger_raw())
#define ABC_CHECK_GT(val1, val2) CHECK_OP_LOGGER(_GT, >, val1, val2, collie::log::default_logger_raw())
#define ABC_CHECK_DOUBLE_EQ(val1, val2) CHECK_DOUBLE_EQ_LOGGER(val1, val2, collie::log::default_logger_raw())
#define ABC_CHECK_NEAR(val1, val2, margin) CHECK_NEAR_LOGGER(val1, val2, margin, collie::log::default_logger_raw())
#define ABC_CHECK_INDEX(I, A) CHECK_LOGGER(I < (sizeof(A) / sizeof(A[0])), collie::log::default_logger_raw())
#define ABC_CHECK_BOUND(B, A) CHECK_LOGGER(B <= (sizeof(A) / sizeof(A[0])), collie::log::default_logger_raw())

#define ABC_PLOG(SEVERITY) PLOG_LOGGER(SEVERITY, collie::log::default_logger_raw())
#define ABC_PLOG_IF(SEVERITY, condition) PLOG_LOGGER_IF(SEVERITY, (condition), collie::log::default_logger_raw())
#define ABC_PLOG_EVERY_N(SEVERITY, N) PLOG_LOGGER_EVERY_N(SEVERITY, N, collie::log::default_logger_raw())
#define ABC_PLOG_IF_EVERY_N(SEVERITY, N, condition) PLOG_LOGGER_IF_EVERY_N(SEVERITY, (N), (condition), collie::log::default_logger_raw())
#define ABC_PLOG_FIRST_N(SEVERITY, N) PLOG_LOGGER_FIRST_N(SEVERITY, (N), true, collie::log::default_logger_raw())
#define ABC_PLOG_IF_FIRST_N(SEVERITY, N, condition) PLOG_LOGGER_IF_FIRST_N(SEVERITY, (N), (condition), collie::log::default_logger_raw())
#define ABC_PLOG_EVERY_T(SEVERITY, seconds) PLOG_LOGGER_EVERY_T(SEVERITY, (seconds), collie::log::default_logger_raw())
#define ABC_PLOG_IF_EVERY_T(SEVERITY, seconds, condition) PLOG_LOGGER_IF_EVERY_T(SEVERITY, (seconds), (condition), collie::log::default_logger_raw())
#define ABC_PLOG_ONCE(SEVERITY) PLOG_LOGGER_ONCE(SEVERITY, collie::log::default_logger_raw())
#define ABC_PLOG_IF_ONCE(SEVERITY, condition) PLOG_LOGGER_IF_ONCE(SEVERITY, (condition), collie::log::default_logger_raw())

#define ABC_PCHECK(condition) PCHECK_LOGGER(condition, collie::log::default_logger_raw())
#define ABC_PCHECK_PTREQ(val1, val2) PCHECK_PTREQ_LOGGER(val1, val2, collie::log::default_logger_raw())
#define ABC_PCHECK_NOTNUL(val) PCHECK_NOTNULL_LOGGER(collie::ptr(val) != nullptr, log::default_logger_raw())
#define ABC_PCHECK_EQ(val1, val2) PCHECK_EQ_LOGGER(val1, val2, collie::log::default_logger_raw())
#define ABC_PCHECK_NE(val1, val2) PCHECK_NE_LOGGER(val1, val2, collie::log::default_logger_raw())
#define ABC_PCHECK_LE(val1, val2) PCHECK_LE_LOGGER(val1, val2, collie::log::default_logger_raw())
#define ABC_PCHECK_LT(val1, val2) PCHECK_LT_LOGGER(val1, val2, collie::log::default_logger_raw())
#define ABC_PCHECK_GE(val1, val2) PCHECK_GE_LOGGER(val1, val2, collie::log::default_logger_raw())
#define ABC_PCHECK_GT(val1, val2) PCHECK_GT_LOGGER(val1, val2, collie::log::default_logger_raw())

#define ABC_VLOG(verboselevel) VLOG_LOGGER(verboselevel, collie::log::default_logger_raw())
#define ABC_VLOG_IF(verboselevel, condition) VLOG_IF_LOGGER(verboselevel, (condition), collie::log::default_logger_raw())
#define ABC_VLOG_EVERY_N(verboselevel, N) VLOG_EVERY_N_LOGGER(verboselevel, N, collie::log::default_logger_raw())
#define ABC_VLOG_IF_EVERY_N(verboselevel, N, condition) VLOG_IF_EVERY_N_LOGGER(verboselevel, (N), (condition), collie::log::default_logger_raw())
#define ABC_VLOG_FIRST_N(verboselevel, N) VLOG_FIRST_N_LOGGER(verboselevel, (N), collie::log::default_logger_raw())
#define ABC_VLOG_IF_FIRST_N(verboselevel, N, condition) VLOG_IF_FIRST_N_LOGGER(verboselevel, (N), (condition), collie::log::default_logger_raw())
#define ABC_VLOG_EVERY_T(verboselevel, seconds) VLOG_EVERY_T_LOGGER(verboselevel, (seconds), collie::log::default_logger_raw())
#define ABC_VLOG_IF_EVERY_T(verboselevel, seconds, condition) VLOG_IF_EVERY_T_LOGGER(verboselevel, (seconds), (condition), collie::log::default_logger_raw())
#define ABC_VLOG_ONCE(verboselevel) VLOG_ONCE_LOGGER(verboselevel, collie::log::default_logger_raw())
#define ABC_VLOG_IF_ONCE(verboselevel, condition) VLOG_IF_ONCE_LOGGER(verboselevel, (condition), collie::log::default_logger_raw())


#if CLOG_DCHECK_IS_ON()

#define ABC_DLOG(SEVERITY)                                LOG(SEVERITY)
#define ABC_DLOG_IF(SEVERITY, condition)                  LOG_LOGGER_IF(SEVERITY, (condition), collie::log::default_logger_raw())
#define ABC_DLOG_EVERY_N(SEVERITY, N)                     LOG_LOGGER_EVERY_N(SEVERITY, N, collie::log::default_logger_raw())
#define ABC_DLOG_IF_EVERY_N(SEVERITY, N, condition)       LOG_LOGGER_IF_EVERY_N(SEVERITY, (N), (condition), collie::log::default_logger_raw())
#define ABC_DLOG_FIRST_N(SEVERITY, N)                     LOG_LOGGER_FIRST_N(SEVERITY, (N), true, collie::log::default_logger_raw())
#define ABC_DLOG_IF_FIRST_N(SEVERITY, N, condition)       LOG_LOGGER_IF_FIRST_N(SEVERITY, (N), (condition), collie::log::default_logger_raw())
#define ABC_DLOG_EVERY_T(SEVERITY, seconds)               LOG_LOGGER_EVERY_T(SEVERITY, (seconds), collie::log::default_logger_raw())
#define ABC_DLOG_IF_EVERY_T(SEVERITY, seconds, condition) LOG_LOGGER_IF_EVERY_T(SEVERITY, (seconds), (condition), collie::log::default_logger_raw())
#define ABC_DLOG_ONCE(SEVERITY)                           LOG_LOGGER_ONCE(SEVERITY, collie::log::default_logger_raw())
#define ABC_DLOG_IF_ONCE(SEVERITY, condition)             LOG_LOGGER_IF_ONCE(SEVERITY, (condition), collie::log::default_logger_raw())

#define ABC_DCHECK(condition) DCHECK_LOGGER(condition, collie::log::default_logger_raw())
#define ABC_DCHECK_NOTNULL(val) DCHECK_NOTNULL_LOGGER(val, collie::log::default_logger_raw())
#define ABC_DCHECK_PTREQ(val1, val2) DCHECK_PTREQ_LOGGER(val1, val2, collie::log::default_logger_raw())
#define ABC_DCHECK_EQ(val1, val2) CHECK_EQ_LOGGER(val1, val2, collie::log::default_logger_raw())
#define ABC_DCHECK_NE(val1, val2) CHECK_NE_LOGGER(val1, val2, collie::log::default_logger_raw())
#define ABC_DCHECK_LE(val1, val2) CHECK_LE_LOGGER(val1, val2, collie::log::default_logger_raw())
#define ABC_DCHECK_LT(val1, val2) CHECK_LT_LOGGER(val1, val2, collie::log::default_logger_raw())
#define ABC_DCHECK_GE(val1, val2) CHECK_GE_LOGGER(val1, val2, collie::log::default_logger_raw())
#define ABC_DCHECK_GT(val1, val2) CHECK_GT_LOGGER(val1, val2, collie::log::default_logger_raw())

#define ABC_DPLOG(SEVERITY)                                  PLOG_LOGGER(SEVERITY, collie::log::default_logger_raw())
#define ABC_DPLOG_IF(SEVERITY, condition)                    PLOG_LOGGER_IF(SEVERITY, (condition), collie::log::default_logger_raw())
#define ABC_DPLOG_EVERY_N(SEVERITY, N)                       PLOG_LOGGER_EVERY_N(SEVERITY, N, collie::log::default_logger_raw())
#define ABC_DPLOG_IF_EVERY_N(SEVERITY, N, condition)         PLOG_LOGGER_IF_EVERY_N(SEVERITY, (N), (condition), collie::log::default_logger_raw())
#define ABC_DPLOG_FIRST_N(SEVERITY, N)                       PLOG_LOGGER_FIRST_N(SEVERITY, (N), true, collie::log::default_logger_raw())
#define ABC_DPLOG_IF_FIRST_N(SEVERITY, N, condition)         PLOG_LOGGER_IF_FIRST_N(SEVERITY, (N), (condition), collie::log::default_logger_raw())
#define ABC_DPLOG_EVERY_T(SEVERITY, seconds)                 PLOG_LOGGER_EVERY_T(SEVERITY, (seconds), collie::log::default_logger_raw())
#define ABC_DPLOG_IF_EVERY_T(SEVERITY, seconds, condition)   PLOG_LOGGER_IF_EVERY_T(SEVERITY, (seconds), (condition), collie::log::default_logger_raw())
#define ABC_DPLOG_ONCE(SEVERITY)                             PLOG_LOGGER_ONCE(SEVERITY, collie::log::default_logger_raw())
#define ABC_DPLOG_IF_ONCE(SEVERITY, condition)               PLOG_LOGGER_IF_ONCE(SEVERITY, (condition), collie::log::default_logger_raw())

#define ABC_DPCHECK(condition) DPCHECK_LOGGER(condition, collie::log::default_logger_raw())
#define ABC_DPCHECK_NOTNULL(val) DPCHECK_NOTNULL_LOGGER(val, collie::log::default_logger_raw())
#define ABC_DPCHECK_PTREQ(val1, val2) DPCHECK_PTREQ_LOGGER(val1, val2, collie::log::default_logger_raw())
#define ABC_DPCHECK_EQ(val1, val2) PCHECK_EQ_LOGGER(val1, val2, collie::log::default_logger_raw())
#define ABC_DPCHECK_NE(val1, val2) PCHECK_NE_LOGGER(val1, val2, collie::log::default_logger_raw())
#define ABC_DPCHECK_LE(val1, val2) PCHECK_LE_LOGGER(val1, val2, collie::log::default_logger_raw())
#define ABC_DPCHECK_LT(val1, val2) PCHECK_LT_LOGGER(val1, val2, collie::log::default_logger_raw())
#define ABC_DPCHECK_GE(val1, val2) PCHECK_GE_LOGGER(val1, val2, collie::log::default_logger_raw())
#define ABC_DPCHECK_GT(val1, val2) PCHECK_GT_LOGGER(val1, val2, collie::log::default_logger_raw())

#define ABC_DVLOG(verboselevel) DVLOG_LOGGER(verboselevel, collie::log::default_logger_raw())
#define ABC_DVLOG_IF(verboselevel, condition) DVLOG_IF_LOGGER(verboselevel, (condition), collie::log::default_logger_raw())
#define ABC_DVLOG_EVERY_N(verboselevel, N) DVLOG_EVERY_N_LOGGER(verboselevel, N, collie::log::default_logger_raw())
#define ABC_DVLOG_IF_EVERY_N(verboselevel, N, condition) DVLOG_IF_EVERY_N_LOGGER(verboselevel, (N), (condition), collie::log::default_logger_raw())
#define ABC_DVLOG_FIRST_N(verboselevel, N) DVLOG_FIRST_N_LOGGER(verboselevel, (N), collie::log::default_logger_raw())
#define ABC_DVLOG_IF_FIRST_N(verboselevel, N, condition) DVLOG_IF_FIRST_N_LOGGER(verboselevel, (N), (condition), collie::log::default_logger_raw())
#define ABC_DVLOG_EVERY_T(verboselevel, seconds) DVLOG_EVERY_T_LOGGER(verboselevel, (seconds), collie::log::default_logger_raw())
#define ABC_DVLOG_IF_EVERY_T(verboselevel, seconds, condition) DVLOG_IF_EVERY_T_LOGGER(verboselevel, (seconds), (condition), collie::log::default_logger_raw())
#define ABC_DVLOG_ONCE(verboselevel) DVLOG_ONCE_LOGGER(verboselevel, collie::log::default_logger_raw())
#define ABC_DVLOG_IF_ONCE(verboselevel, condition) DVLOG_IF_ONCE_LOGGER(verboselevel, (condition), collie::log::default_logger_raw())

#else // NDEBUG

#define ABC_DLOG(SEVERITY)                                DLOG_LOGGER(SEVERITY, collie::log::default_logger_raw())
#define ABC_DLOG_IF(SEVERITY, condition)                  DLOG_LOGGER_IF(SEVERITY, (condition), collie::log::default_logger_raw())
#define ABC_DLOG_EVERY_N(SEVERITY, N)                     DLOG_LOGGER_EVERY_N(SEVERITY, N, collie::log::default_logger_raw())
#define ABC_DLOG_IF_EVERY_N(SEVERITY, N, condition)       DLOG_LOGGER_IF_EVERY_N(SEVERITY, (N), (condition), collie::log::default_logger_raw())
#define ABC_DLOG_FIRST_N(SEVERITY, N)                     DLOG_LOGGER_FIRST_N(SEVERITY, (N), true, collie::log::default_logger_raw())
#define ABC_DLOG_IF_FIRST_N(SEVERITY, N, condition)       DLOG_LOGGER_IF_FIRST_N(SEVERITY, (N), (condition), collie::log::default_logger_raw())
#define ABC_DLOG_EVERY_T(SEVERITY, seconds)               DLOG_LOGGER_EVERY_T(SEVERITY, (seconds), collie::log::default_logger_raw())
#define ABC_DLOG_IF_EVERY_T(SEVERITY, seconds, condition) DLOG_LOGGER_IF_EVERY_T(SEVERITY, (seconds), (condition), collie::log::default_logger_raw())
#define ABC_DLOG_ONCE(SEVERITY)                           DLOG_LOGGER_ONCE(SEVERITY, collie::log::default_logger_raw())
#define ABC_DLOG_IF_ONCE(SEVERITY, condition)             DLOG_LOGGER_IF_ONCE(SEVERITY, (condition), collie::log::default_logger_raw())

#define ABC_DCHECK(condition) DCHECK_LOGGER(condition, collie::log::default_logger_raw())
#define ABC_DCHECK_NOTNULL(val) DCHECK_NOTNULL_LOGGER(val, collie::log::default_logger_raw())
#define ABC_DCHECK_PTREQ(val1, val2) DCHECK_PTREQ_LOGGER(val1, val2, collie::log::default_logger_raw())
#define ABC_DCHECK_EQ(val1, val2) CHECK_EQ_LOGGER(val1, val2, collie::log::default_logger_raw())
#define ABC_DCHECK_NE(val1, val2) CHECK_NE_LOGGER(val1, val2, collie::log::default_logger_raw())
#define ABC_DCHECK_LE(val1, val2) CHECK_LE_LOGGER(val1, val2, collie::log::default_logger_raw())
#define ABC_DCHECK_LT(val1, val2) CHECK_LT_LOGGER(val1, val2, collie::log::default_logger_raw())
#define ABC_DCHECK_GE(val1, val2) CHECK_GE_LOGGER(val1, val2, collie::log::default_logger_raw())
#define ABC_DCHECK_GT(val1, val2) CHECK_GT_LOGGER(val1, val2, collie::log::default_logger_raw())


#define ABC_DPLOG(SEVERITY)                                  DPLOG_LOGGER(SEVERITY, collie::log::default_logger_raw())
#define ABC_DPLOG_IF(SEVERITY, condition)                    DPLOG_LOGGER_IF(SEVERITY, (condition), collie::log::default_logger_raw())
#define ABC_DPLOG_EVERY_N(SEVERITY, N)                       DPLOG_LOGGER_EVERY_N(SEVERITY, N, collie::log::default_logger_raw())
#define ABC_DPLOG_IF_EVERY_N(SEVERITY, N, condition)         DPLOG_LOGGER_IF_EVERY_N(SEVERITY, (N), (condition), collie::log::default_logger_raw())
#define ABC_DPLOG_FIRST_N(SEVERITY, N)                       DPLOG_LOGGER_FIRST_N(SEVERITY, (N), true, collie::log::default_logger_raw())
#define ABC_DPLOG_IF_FIRST_N(SEVERITY, N, condition)         DPLOG_LOGGER_IF_FIRST_N(SEVERITY, (N), (condition), collie::log::default_logger_raw())
#define ABC_DPLOG_EVERY_T(SEVERITY, seconds)                 DPLOG_LOGGER_EVERY_T(SEVERITY, (seconds), collie::log::default_logger_raw())
#define ABC_DPLOG_IF_EVERY_T(SEVERITY, seconds, condition)   DPLOG_LOGGER_IF_EVERY_T(SEVERITY, (seconds), (condition), collie::log::default_logger_raw())
#define ABC_DPLOG_ONCE(SEVERITY)                             DPLOG_LOGGER_ONCE(SEVERITY, collie::log::default_logger_raw())
#define ABC_DPLOG_IF_ONCE(SEVERITY, condition)               DPLOG_LOGGER_IF_ONCE(SEVERITY, (condition), collie::log::default_logger_raw())

#define ABC_DPCHECK(condition)        DPCHECK_LOGGER(condition, collie::log::default_logger_raw())
#define ABC_DPCHECK_NOTNULL(val)      DPCHECK_NOTNULL_LOGGER(val, collie::log::default_logger_raw())
#define ABC_DPCHECK_PTREQ(val1, val2) DPCHECK_PTREQ_LOGGER(val1, val2, collie::log::default_logger_raw())
#define ABC_DPCHECK_EQ(val1, val2) DPCHECK_EQ_LOGGER(val1, val2, collie::log::default_logger_raw())
#define ABC_DPCHECK_NE(val1, val2) DPCHECK_NE_LOGGER(val1, val2, collie::log::default_logger_raw())
#define ABC_DPCHECK_LE(val1, val2) DPCHECK_LE_LOGGER(val1, val2, collie::log::default_logger_raw())
#define ABC_DPCHECK_LT(val1, val2) DPCHECK_LT_LOGGER(val1, val2, collie::log::default_logger_raw())
#define ABC_DPCHECK_GE(val1, val2) DPCHECK_GE_LOGGER(val1, val2, collie::log::default_logger_raw())
#define ABC_DPCHECK_GT(val1, val2) DPCHECK_GT_LOGGER(val1, val2, collie::log::default_logger_raw())

#define ABC_DVLOG(verboselevel) DVLOG_LOGGER(verboselevel, collie::log::default_logger_raw())
#define ABC_DVLOG_IF(verboselevel, condition) DVLOG_IF_LOGGER(verboselevel, (condition), collie::log::default_logger_raw())
#define ABC_DVLOG_EVERY_N(verboselevel, N) DVLOG_EVERY_N_LOGGER(verboselevel, N, collie::log::default_logger_raw())
#define ABC_DVLOG_IF_EVERY_N(verboselevel, N, condition) DVLOG_IF_EVERY_N_LOGGER(verboselevel, (N), (condition), collie::log::default_logger_raw())
#define ABC_DVLOG_FIRST_N(verboselevel, N) DVLOG_FIRST_N_LOGGER(verboselevel, (N), collie::log::default_logger_raw())
#define ABC_DVLOG_IF_FIRST_N(verboselevel, N, condition) DVLOG_IF_FIRST_N_LOGGER(verboselevel, (N), (condition), collie::log::default_logger_raw())
#define ABC_DVLOG_EVERY_T(verboselevel, seconds) DVLOG_EVERY_T_LOGGER(verboselevel, (seconds), collie::log::default_logger_raw())
#define ABC_DVLOG_IF_EVERY_T(verboselevel, seconds, condition) DVLOG_IF_EVERY_T_LOGGER(verboselevel, (seconds), (condition), collie::log::default_logger_raw())
#define ABC_DVLOG_ONCE(verboselevel) DVLOG_ONCE_LOGGER(verboselevel, collie::log::default_logger_raw())
#define ABC_DVLOG_IF_ONCE(verboselevel, condition) DVLOG_IF_ONCE_LOGGER(verboselevel, (condition), collie::log::default_logger_raw())

#endif
