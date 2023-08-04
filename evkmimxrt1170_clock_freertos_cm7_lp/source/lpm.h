/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _LPM_H_
#define _LPM_H_

#include <stdint.h>
#include "fsl_gpc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if __CORTEX_M == 7
#define GPC_CPU_MODE_CTRL GPC_CPU_MODE_CTRL_0
#elif __CORTEX_M == 4
#define GPC_CPU_MODE_CTRL GPC_CPU_MODE_CTRL_1
#endif
#define GET_CPU_MODE_NAME(mode) (g_cpuModeNames[(uint8_t)mode])

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

void SSARC_InitConfig(void);
void SSARC_TriggerSoftwareRestore(void);
void SSARC_TriggerSoftwareSave(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _LPM_H_ */
