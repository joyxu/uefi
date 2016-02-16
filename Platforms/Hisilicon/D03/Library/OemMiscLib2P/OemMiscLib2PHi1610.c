/** @file
*
*  Copyright (c) 2015, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2015, Linaro Limited. All rights reserved.
*
*  This program and the accompanying materials
*  are licensed and made available under the terms and conditions of the BSD License
*  which accompanies this distribution.  The full text of the license may be found at
*  http://opensource.org/licenses/bsd-license.php
*
*  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
*  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
*
**/

#include <Uefi.h>
#include <PiPei.h>

#include <Library/OemMiscLib.h>
#include <Library/DebugLib.h>
#include <Library/SerialPortLib.h>
#include <PlatformArch.h>
#include <Library/PlatformSysCtrlLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/TimerLib.h>
#include <Library/OemAddressMapLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/PeiServicesLib.h>
#include <Guid/BootLine.h>
#include <BootLine.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Library/PrintLib.h>
#include <Library/TimerLib.h>

//
// FRB Common Structures
//
typedef enum {
    EfiReserved,
    EfiBiosFrb2,
    EfiBiosPost,
    EfiOsLoad,
    EfiSmsOs,
    EfiOem,
    EfiFrbReserved1,
    EfiFrbReserved2
} EFI_WDT_USER_TYPE;

EFI_STATUS IpmiCmdStopWatchdogTimer (IN EFI_WDT_USER_TYPE  UserType);

UINT16 OemUsbNum = 0;
UINT16 OemUsbDesSize = 0; 
UINT16 OemSataNum = 0;
UINT16 OemSataDesSize = 0;
UINT16 OemPXENum = 0;
UINT16 OemPXEDesSize = 0;
UINT8 OemFirstBootId = 0;

SATADES* OemSataDes = NULL;
PXEDES* OemPxeDes = NULL;
USBDES* OemUsbDes = NULL;

CHAR8 *sTokenList[] = {
  "FLASH",
  "PXE",
  NULL
};

REPORT_PCIEDIDVID2BMC  PcieDeviceToReport[PCIEDEVICE_REPORT_MAX] = {
      {67,0,0,0},   //槽位1
      {225,0,0,3},   //槽位4
      {0xFFFF,0xFFFF,0xFFFF,0xFFFF},
      {0xFFFF,0xFFFF,0xFFFF,0xFFFF}
};

// Right now we only support 1P
BOOLEAN OemIsSocketPresent (UINTN Socket)
{
  if (0 == Socket)
  {
    return TRUE;
  }

  if(1 == Socket)
  {
    return TRUE;
  }

  return FALSE;
}


UINTN OemGetSocketNumber (VOID)
{
    
    if(!OemIsMpBoot())
    {
        return 1;
    }
    
    return 2;
   
}


UINTN OemGetDdrChannel (VOID)
{
    return 4;
}


UINTN OemGetDimmSlot(UINTN Socket, UINTN Channel)
{
    return 2;
}


// Nothing to do for EVB
VOID OemPostEndIndicator (VOID)
{    
    
     DEBUG((EFI_D_ERROR,"M3 release reset CONFIG........."));    
    
     MmioWrite32(0xd0002180, 0x3);
     MmioWrite32(0xd0002194, 0xa4);
     MmioWrite32(0xd0000a54, 0x1);
    
     MicroSecondDelay(10000);
    
     MmioWrite32(0xd0002108, 0x1);
     MmioWrite32(0xd0002114, 0x1);
     MmioWrite32(0xd0002120, 0x1);
     MmioWrite32(0xd0003108, 0x1);
    
     MicroSecondDelay(500000);
     DEBUG((EFI_D_ERROR,"Done\n"));
    
}


EFI_STATUS OemSelfTestReport(IN E_SELF_TEST_ITEM Item, IN U_SELF_TEST_PARA Para)
{
    return EFI_SUCCESS;
}

VOID OemSetSelfTestFailFlagAndAlarm()
{
    return;
}

UINT32 OemGetCurrentBiosChannel(VOID)
{
    return 0;
}


EFI_STATUS OemCheckCpld(VOID)
{
    return EFI_SUCCESS;
}

EFI_STATUS OemCheckClock(VOID)
{
    return EFI_SUCCESS;
}

E_RESET_CAUSE OemGetCurrentResetCause(VOID)
{
    return RESET_CAUSE_CPLD_BUTT;
}

E_RESET_CAUSE OemGetPer2ResetCause(VOID)
{
    return RESET_CAUSE_CPLD_BUTT;
}

E_RESET_CAUSE OemGetPer3ResetCause(VOID)
{
    return RESET_CAUSE_CPLD_BUTT;
}

UINT32 OemIsWarmBoot(VOID)
{
    return 0;
}


VOID CoreSelectBoot(VOID)
{
    if (!PcdGet64 (PcdTrustedFirmwareEnable))
    {
        StartupAp ();
    }

    return;
}

VOID OemBiosSwitch(UINT32 Master)
{
    (VOID)Master;
    return;
}



VOID OpenAlarmLed(VOID)
{
    return;
}
 
VOID OemWriteProtectSet(BOOLEAN val)
{
    return;
}
/*******************************************************************************
 函 数 名  : OemIsSpiFlashWriteProtected
 功能描述  : 是否本项目需要spi flash写保护功能，是请返回TRUE,否返回FALSE
 输入参数  : 无  
 输出参数  : TRUE/FALSE
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年12月17日
    作    者   : l00306713
    修改内容   : 新生成函数
*******************************************************************************/
BOOLEAN OemIsSpiFlashWriteProtected(VOID)
{
    return FALSE;
}


EFI_STATUS OemCk420Read(UINT16 InfoOffset, UINT32 ulLength, UINT8 *pBuf)
{
    return EFI_SUCCESS;
}

EFI_STATUS OemCk420Write(UINT16 InfoOffset, UINT32 ulLength, UINT8 *pBuf)
{
    return EFI_SUCCESS;
}

UINT8 OemAhciStartPort(VOID)
{
    return 0;
}


UINT32 OemGetDefaultSetupData(VOID *Setup)
{
    return 0;
}


EFI_STATUS OemGetUsbBootNum(USBDES* UsbDes, UINTN UsbDesSize)
{
    if(UsbDesSize/sizeof(USBDES) > 0)
    {
        OemUsbDesSize = UsbDesSize;
        OemUsbNum = OemUsbDesSize/sizeof(USBDES);
    }
    else
    {
        OemUsbNum = 0;
    }
    OemUsbDes = UsbDes;     
    return EFI_SUCCESS;
}

EFI_STATUS OemGetSataBootNum(SATADES* SataDes, UINTN SataDesSize)
{ 
    if( SataDesSize/sizeof(SATADES) > 0)
    {
        OemSataDesSize = SataDesSize;
        OemSataNum = OemSataDesSize/sizeof(SATADES);
    }
    else{    
        OemSataNum = 0;    
    }
    OemSataDes = SataDes; 
    return EFI_SUCCESS;
}

EFI_STATUS OemGetPXEBootNum(PXEDES* PxeDes, UINTN PXESize)
{
    if( PXESize/sizeof(PXEDES) > 0)
    {
        OemPXEDesSize = PXESize;
        OemPXENum = OemPXEDesSize/sizeof(PXEDES);
    }
    else
    {
        OemPXENum = 0;
    }
    OemPxeDes = PxeDes; 
    return EFI_SUCCESS;
}
//uniBIOS_l00306713_000_end   2015-5-21 10:23:51

EFI_STATUS OemPreStartBootOptionAction(UINT16 BootOptionId)
{
    return EFI_SUCCESS;
}

EFI_STATUS OemBootOrderSeting(IN OUT UINT16* BootOrderFinal, UINTN BootOrderSize, BOOLEAN *IsFlashBootFirst, BOOLEAN *FlashBootSupport)
{
    *FlashBootSupport = FALSE;
    if(BootOrderFinal == NULL || BootOrderSize == 0)
    {
        return EFI_INVALID_PARAMETER;
    }
    return EFI_SUCCESS;
}

VOID
OemRecordMarginResult(
    pGBL_DATA           pGblData,
    struct rankMargin   resultsRxDq[], 
    struct rankMargin   resultsTxDq[], 
    struct rankMargin   resultsRxVref[], 
    struct rankMargin   resultsTxVref[]
)
{
    return;
}


BOOLEAN IsNeedSkipVariableSpaceOperation()
{
    return TRUE;
}

BOOLEAN OemIsLoadDefault()
{
    UINT32  Tmp;

    Tmp = MmioRead32(0x602E0050);
    if(BIT9 == (Tmp & BIT9))
        return FALSE;
    else
        return TRUE;
}

BOOLEAN OemIsInitEth(UINT32 Port)
{
    return TRUE;
}


BOOLEAN OemIsMpBoot()
{
    UINT32 Tmp;
    
    Tmp = MmioRead32(0x602E0050);
    if ( ((Tmp >> 10) & 0xF) == 0x3)
        return TRUE;
    else
        return FALSE;
}

VOID OemStopBmcPostWatchdog(VOID){
    EFI_STATUS Status;
    
    DEBUG((EFI_D_ERROR,"Now Do Stop POST Watchdog Timer. \r\n"));
    
    Status = IpmiCmdStopWatchdogTimer(EfiBiosPost);
    if(EFI_ERROR(Status))
    {
        DEBUG ((EFI_D_ERROR, "Stop POST Watchdog Timer Failed. Status %r\n", Status));
    }

    return;
}
//end


VOID OemGpioConfig(VOID)
{
}

VOID OemI2CSdaConfig(VOID)
{
}

VOID GetOemSetupConfig(pGBL_DATA pGblData)
{
    return;
}


VOID OemLpcInit(VOID)
{
    LpcInit();
    return;
}

