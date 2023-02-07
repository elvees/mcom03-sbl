// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#ifndef __MCOM03_ERRORS_H__
#define __MCOM03_ERRORS_H__

#include <stdint.h>

typedef uint32_t mcom_err_t;

#define MCOM03_ERROR_BASE_NUM (0x0)

/** @brief Base number of driver error codes. */
#define MCOM03_ERROR_DRIVERS_BASE_NUM (MCOM03_ERROR_BASE_NUM + 0x10000)
#define MCOM03_ERROR_RTC_BASE_NUM     (MCOM03_ERROR_BASE_NUM + 0x20000)

/** @brief Enumerated type for error codes. */

#define MCOM03_SUCCESS		   (MCOM03_ERROR_BASE_NUM + 0) ///< Operation performed successfully.
#define MCOM03_ERROR_INTERNAL	   (MCOM03_ERROR_BASE_NUM + 1) ///< Internal error.
#define MCOM03_ERROR_NO_MEM	   (MCOM03_ERROR_BASE_NUM + 2) ///< No memory for operation.
#define MCOM03_ERROR_NOT_SUPPORTED (MCOM03_ERROR_BASE_NUM + 3) ///< Not supported.
#define MCOM03_ERROR_INVALID_PARAM (MCOM03_ERROR_BASE_NUM + 4) ///< Invalid parameter.
#define MCOM03_ERROR_INVALID_STATE \
	(MCOM03_ERROR_BASE_NUM + 5) ///< Invalid state, operation disallowed in this state.
#define MCOM03_ERROR_INVALID_LENGTH (MCOM03_ERROR_BASE_NUM + 6) ///< Invalid length.
#define MCOM03_ERROR_INVALID_FLAGS  (MCOM03_ERROR_BASE_NUM + 7) ///< Invalid Flags
#define MCOM03_ERROR_INVALID_DATA   (MCOM03_ERROR_BASE_NUM + 8) ///< Invalid Data
#define MCOM03_ERROR_DATA_SIZE	    (MCOM03_ERROR_BASE_NUM + 9) ///< Data size exceeds limit

#define MCOM03_ERROR_TIMEOUT	  (MCOM03_ERROR_BASE_NUM + 10) ///< Operation timed out.
#define MCOM03_ERROR_FORBIDDEN	  (MCOM03_ERROR_BASE_NUM + 11) ///< Operation is forbidden.
#define MCOM03_ERROR_NULL	  (MCOM03_ERROR_BASE_NUM + 12) ///< Null pointer.
#define MCOM03_ERROR_INVALID_ADDR (MCOM03_ERROR_BASE_NUM + 13) ///< Bad memory address.
#define MCOM03_ERROR_BUSY	  (MCOM03_ERROR_BASE_NUM + 14) ///< Busy.
#define MCOM03_ERROR_ALREADY_INITIALIZED \
	(MCOM03_ERROR_BASE_NUM + 15) ///< Module already initialized.

#define MCOM03_ERROR_UART_OVERFLOW (MCOM03_ERROR_DRIVERS_BASE_NUM + 0) ///< UART error
#define MCOM03_ERROR_UART_PARITY   (MCOM03_ERROR_DRIVERS_BASE_NUM + 1) ///< UART error
#define MCOM03_ERROR_UART_FRAME	   (MCOM03_ERROR_DRIVERS_BASE_NUM + 2) ///< UART error
#define MCOM03_ERROR_UART_UNKNOWN  (MCOM03_ERROR_DRIVERS_BASE_NUM + 3) ///< UART error

#define MCOM03_ERROR_RTC_ERR_MASK (MCOM03_ERROR_RTC_BASE_NUM + 0) ///<

#endif // __MCOM03_ERRORS_H__
