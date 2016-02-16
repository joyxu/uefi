/*-----------------------------------------------------------------------*/
/*!!Warning: Huawei key information asset. No spread without permission. */
/*CODEMARK:EG4uRhTwMmgcVFBsBnYHCEm2UPcyllv4D4NOje6cFLSYglw6LvPA978sGAr3yTchgOI0M46H
HZIZCDLcNqR1rbxHHGWmLNp+CRsGfVaxSWSrseVn/+wcdCo+wKA6WCQWkJcEHPNQYTvBaY4Z
0I9NvmXks5/3xrf2SgrV77Jcoh3D6a4Ku5zaEnecGVEyMCIuQU9k9ArKHTov5q53mYXTsJcD
zoLvSiVq+83n5eB6LmVjbzKXb4s+NW0I6u9xD8JofNdvbpFaj/s0buPy7dR24g==#*/
/*--!!Warning: Deleting or modifying the preceding information is prohibited.--*/


/** @file
  This file implements ATA_PASSTHRU_PROCTOCOL and EXT_SCSI_PASSTHRU_PROTOCOL interfaces
  for managed ATA controllers.

  Copyright (c) 2010 - 2013, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "PcieInit.h"
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PcdLib.h>
#include <Library/OemMiscLib.h>

/*
PCIE_ROOT_BRIDGE_PROTOCOL gPcieRootBridge = {
    PcieChangeRwMode,  //PcieChangeReadWriteMode
    PcieSetDBICS2Enable //PcieSetDBICS2EnableDisable
};
*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/
    
extern VOID PcieRegWrite(UINT32 Port, UINTN Offset, UINT32 Value);
extern EFI_STATUS PciePortReset(UINT32 HostBridgeNum, UINT32 Port);
extern EFI_STATUS PciePortInit (UINT32 soctype, UINT32 HostBridgeNum, PCIE_DRIVER_CFG *PcieCfg);
//EFI_EVENT         EfiExitBootServicesEvent = (EFI_EVENT)NULL;  // uniBIOS_y00216284_081_start DTS2015081408681  2015-8-14 >>>

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/
PCIE_DRIVER_CFG gastr_pcie_driver_cfg[PCIE_MAX_PORT_NUM] =
{
    //Port 0
    {
        0x0,                        //Portindex
                                    
        {
            PCIE_ROOT_COMPLEX,      //PortType
            PCIE_WITDH_X8,          //PortWidth    
            PCIE_GEN3_0,            //PortGen
        }, //PortInfo
        
    },

    //Port 1
    {
        0x1,                        //Portindex
        {
            PCIE_ROOT_COMPLEX,      //PortType
            PCIE_WITDH_X8,          //PortWidth    
            PCIE_GEN3_0,            //PortGen
        },

    },

    //Port 2
    {
        0x2,                        //Portindex
        {
            PCIE_ROOT_COMPLEX,      //PortType
            PCIE_WITDH_X8,          //PortWidth    
            PCIE_GEN3_0,            //PortGen
        },

    },

    //Port 3
    {
        0x3,                        //Portindex
        {
            PCIE_ROOT_COMPLEX,      //PortType
            PCIE_WITDH_X8,          //PortWidth    
            PCIE_GEN3_0,            //PortGen
        },

    },
};

/*****************************************************************************
 函 数 名   : PcieInitEntry
 功能描述  : pcie初始化
 输入参数  : 无
 输出参数  : 无
 返 回 值   : EFI_STATUS
*****************************************************************************/
EFI_STATUS
PcieInitEntry (
  IN EFI_HANDLE                 ImageHandle,
  IN EFI_SYSTEM_TABLE           *SystemTable
  )

{
    UINT32             Port;
    EFI_STATUS         Status = EFI_SUCCESS;
   // EFI_HANDLE         Handle;
    UINT32             HostBridgeNum = 0;
    UINT32             soctype = 0;
    UINT32       PcieRootBridgeMask;

  
    //DTS2015110700120 1610 PCIe 代码上库,格式对齐
    //uniBIOS__l00228991_start DTS2016010404352 2016-1-4 16:06:06
    //Description:LemonC10 1P代码上库
    if (!OemIsMpBoot())
    {
        PcieRootBridgeMask = PcdGet32(PcdPcieRootBridgeMask);
        PcieRootBridgeMask &= 0xf;
    }
    else
    {
        PcieRootBridgeMask = PcdGet32(PcdPcieRootBridgeMask2P);
    }
    //uniBIOS__l00228991_end   2016-1-4 16:06:06
    
    //DTS2015073101571 PCIe代码检视意见修改:将多host考虑进去
    soctype = PcdGet32(Pcdsoctype);
    for (HostBridgeNum = 0; HostBridgeNum < PCIE_HOST_BRIDGE_NUM; HostBridgeNum++)
    {
        for (Port = 0; Port < PCIE_MAX_PORT_NUM; Port++)
        {
            if (!(((( PcieRootBridgeMask >> (4 * HostBridgeNum))) >> Port) & 0x1))
            {
                continue;
            }
            
            Status = PciePortInit(soctype, HostBridgeNum, &gastr_pcie_driver_cfg[Port]);
            if(EFI_ERROR(Status))
            {
                DEBUG((EFI_D_ERROR, "HostBridge %d, Pcie Port %d Init Failed! \n", HostBridgeNum, Port));  
            }
        
        }
    }

    //
    // Make a new handle and install the protocol
    //
   // Handle = NULL;
   // Status = gBS->InstallProtocolInterface (
   //               &Handle,
   //               &gEfiPcieRootBridgeProtocolGuid,
   //               EFI_NATIVE_INTERFACE,
   //               &gPcieRootBridge
   //               );
    //DTS2015072900796 PCLint消除
   // if (EFI_ERROR(Status))
   // {        
   //     DEBUG((EFI_D_ERROR, " Install PcieRootBridge protocol fail\n "));
   // }
 
    return EFI_SUCCESS;
    
}

        
