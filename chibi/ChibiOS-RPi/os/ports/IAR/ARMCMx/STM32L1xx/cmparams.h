/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    IAR/ARMCMx/STM32L1xx/cmparams.h
 * @brief   ARM Cortex-M3 parameters for the STM32L1xx.
 *
 * @defgroup IAR_ARMCMx_STM32L1xx STM32L1xx Specific Parameters
 * @ingroup IAR_ARMCMx_SPECIFIC
 * @details This file contains the Cortex-M3 specific parameters for the
 *          STM32L1xx platform.
 * @{
 */

#ifndef _CMPARAMS_H_
#define _CMPARAMS_H_

/**
 * @brief   Cortex core model.
 */
#define CORTEX_MODEL            CORTEX_M3

/**
 * @brief   Systick unit presence.
 */
#define CORTEX_HAS_ST           TRUE

/**
 * @brief   Memory Protection unit presence.
 */
#define CORTEX_HAS_MPU          TRUE

/**
 * @brief   Floating Point unit presence.
 */
#define CORTEX_HAS_FPU          FALSE

/**
 * @brief   Number of bits in priority masks.
 */
#define CORTEX_PRIORITY_BITS    4

#endif /* _CMPARAMS_H_ */

/** @} */
