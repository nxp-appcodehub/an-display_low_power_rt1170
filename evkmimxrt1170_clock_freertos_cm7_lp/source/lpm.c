/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "lpm.h"
#include "fsl_pgmc.h"
#include "fsl_clock.h"
#include "fsl_dcdc.h"
#include "fsl_soc_src.h"
#include "fsl_pmu.h"
#include "fsl_debug_console.h"
#include "fsl_ssarc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
   
#define DISPLAYMIX_GROUP_ID             0  
/* clang-format off */
#define DISPLAYMIX_DESCRIPTOR_NUM       20
#define DISPLAYMIX_DESCRIPTOR_TABLE \
{/*address                                      ,data ,size                             ,opreation                      ,type                         ,index */\
{ (uint32_t)&VIDEO_MUX->VID_MUX_CTRL.RW         ,0 ,kSSARC_DescriptorRegister32bitWidth ,kSSARC_SaveEnableRestoreEnable ,kSSARC_ReadValueWriteBack}, /*  0   */\
{ (uint32_t)&LCDIFV2->DISP_SIZE                 ,0 ,kSSARC_DescriptorRegister32bitWidth ,kSSARC_SaveEnableRestoreEnable ,kSSARC_ReadValueWriteBack}, /*  1   */\
{ (uint32_t)&LCDIFV2->HSYN_PARA	                ,0 ,kSSARC_DescriptorRegister32bitWidth ,kSSARC_SaveEnableRestoreEnable ,kSSARC_ReadValueWriteBack}, /*  2   */\
{ (uint32_t)&LCDIFV2->VSYN_PARA                 ,0 ,kSSARC_DescriptorRegister32bitWidth ,kSSARC_SaveEnableRestoreEnable ,kSSARC_ReadValueWriteBack}, /*  3   */\
{ (uint32_t)&LCDIFV2->CTRL                      ,0 ,kSSARC_DescriptorRegister32bitWidth ,kSSARC_SaveEnableRestoreEnable ,kSSARC_ReadValueWriteBack}, /*  4   */\
{ (uint32_t)&LCDIFV2->INT[0].INT_ENABLE         ,0 ,kSSARC_DescriptorRegister32bitWidth ,kSSARC_SaveEnableRestoreEnable ,kSSARC_ReadValueWriteBack}, /*  5   */\
{ (uint32_t)&LCDIFV2->INT[1].INT_ENABLE         ,0 ,kSSARC_DescriptorRegister32bitWidth ,kSSARC_SaveEnableRestoreEnable ,kSSARC_ReadValueWriteBack}, /*  6   */\
{ (uint32_t)&LCDIFV2->LAYER[0].CTRLDESCL1       ,0 ,kSSARC_DescriptorRegister32bitWidth ,kSSARC_SaveEnableRestoreEnable ,kSSARC_ReadValueWriteBack}, /*  7  */\
{ (uint32_t)&LCDIFV2->LAYER[0].CTRLDESCL2       ,0 ,kSSARC_DescriptorRegister32bitWidth ,kSSARC_SaveEnableRestoreEnable ,kSSARC_ReadValueWriteBack}, /*  8  */\
{ (uint32_t)&LCDIFV2->LAYER[0].CTRLDESCL3       ,0 ,kSSARC_DescriptorRegister32bitWidth ,kSSARC_SaveEnableRestoreEnable ,kSSARC_ReadValueWriteBack}, /*  9  */\
{ (uint32_t)&LCDIFV2->LAYER[0].CTRLDESCL4       ,0 ,kSSARC_DescriptorRegister32bitWidth ,kSSARC_SaveEnableRestoreEnable ,kSSARC_ReadValueWriteBack}, /*  10  */\
{ (uint32_t)&LCDIFV2->LAYER[0].CTRLDESCL6       ,0 ,kSSARC_DescriptorRegister32bitWidth ,kSSARC_SaveEnableRestoreEnable ,kSSARC_ReadValueWriteBack}, /*  11  */\
{ (uint32_t)&LCDIFV2->LAYER[0].CSC_COEF0        ,0 ,kSSARC_DescriptorRegister32bitWidth ,kSSARC_SaveEnableRestoreEnable ,kSSARC_ReadValueWriteBack}, /*  12  */\
{ (uint32_t)&LCDIFV2->LAYER[0].CSC_COEF1        ,0 ,kSSARC_DescriptorRegister32bitWidth ,kSSARC_SaveEnableRestoreEnable ,kSSARC_ReadValueWriteBack}, /*  13  */\
{ (uint32_t)&LCDIFV2->LAYER[0].CSC_COEF2        ,0 ,kSSARC_DescriptorRegister32bitWidth ,kSSARC_SaveEnableRestoreEnable ,kSSARC_ReadValueWriteBack}, /*  14  */\
{ (uint32_t)&LCDIFV2->DISP_PARA                 ,0 ,kSSARC_DescriptorRegister32bitWidth ,kSSARC_SaveEnableRestoreEnable ,kSSARC_ReadValueWriteBack}, /*  15   */\
{ (uint32_t)&LCDIFV2->LAYER[0].CTRLDESCL5       ,0 ,kSSARC_DescriptorRegister32bitWidth ,kSSARC_SaveEnableRestoreEnable ,kSSARC_ReadValueWriteBack}, /*  16  */\
{ (uint32_t)&LCDIFV2->LAYER[1].CSC_COEF0        ,0 ,kSSARC_DescriptorRegister32bitWidth ,kSSARC_SaveEnableRestoreEnable ,kSSARC_ReadValueWriteBack}, /*  17  */\
{ (uint32_t)&LCDIFV2->LAYER[1].CSC_COEF1        ,0 ,kSSARC_DescriptorRegister32bitWidth ,kSSARC_SaveEnableRestoreEnable ,kSSARC_ReadValueWriteBack}, /*  18  */\
{ (uint32_t)&LCDIFV2->LAYER[1].CSC_COEF2        ,0 ,kSSARC_DescriptorRegister32bitWidth ,kSSARC_SaveEnableRestoreEnable ,kSSARC_ReadValueWriteBack}} /*  19  */
  
  
void SSARC_InitConfig(void)
{
    uint16_t descriptorIndex = 0;

    ssarc_group_config_t groupConfig;
    static const ssarc_descriptor_config_t dispmixDescriptor[DISPLAYMIX_DESCRIPTOR_NUM]   = DISPLAYMIX_DESCRIPTOR_TABLE;
    
    groupConfig.startIndex = descriptorIndex;
    for (descriptorIndex = 0; descriptorIndex < DISPLAYMIX_DESCRIPTOR_NUM; descriptorIndex++)
    {
        SSARC_SetDescriptorConfig(SSARC_HP, descriptorIndex, &dispmixDescriptor[descriptorIndex]);
    }
    groupConfig.endIndex        = descriptorIndex - 1;
    groupConfig.highestAddress  = 0xFFFFFFFFU;
    groupConfig.lowestAddress   = 0x00000000U;
    groupConfig.saveOrder       = kSSARC_ProcessFromStartToEnd;
    groupConfig.savePriority    = 0U;
    groupConfig.restoreOrder    = kSSARC_ProcessFromStartToEnd;
    groupConfig.restorePriority = 0U;
    groupConfig.powerDomain     = kSSARC_DISPLAYMIXPowerDomain;
    groupConfig.cpuDomain       = kSSARC_CM7Core;
    SSARC_GroupInit(SSARC_LP, DISPLAYMIX_GROUP_ID, &groupConfig);
}

    
void SSARC_TriggerSoftwareRestore(void)
{
     SSARC_TriggerSoftwareRequest(SSARC_LP,DISPLAYMIX_GROUP_ID,kSSARC_TriggerRestoreRequest);
}

void SSARC_TriggerSoftwareSave(void)
{
     SSARC_TriggerSoftwareRequest(SSARC_LP,DISPLAYMIX_GROUP_ID,kSSARC_TriggerSaveRequest);
}    