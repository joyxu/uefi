/*-----------------------------------------------------------------------*/
/*!!Warning: Huawei key information asset. No spread without permission. */
/*CODEMARK:EG4uRhTwMmgcVFBsBnYHCEm2UPcyllv4D4NOje6cFLSYglw6LvPA978sGAr3yTchgOI0M46H
HZIZCDLcNqR1rbxHHGWmLNp+CRsGfVaxSWTI2/8f/R+w4XjDnh/Xjw50z2xsARLBKfMM32Ii
FZ5sQE0IeMhKej1ExM6VVVDK25hl09jwepQPVwgSdEWjMAjHX6VpvjQClsfH5u4rIViiH2Ii
un14K3AmzP3mN606ykBUKzsFi2OP4sLp/8Edp02yg6t4XsvP+xBzPRskQUxsAA==#*/
/*--!!Warning: Deleting or modifying the preceding information is prohibited.--*/

/** @file
  Template library implementation to support ResetSystem Runtime call.

  Fill in the templates with what ever makes you system reset.

  Copyright (c) 2008 - 2009, Apple Inc. All rights reserved.<BR>
  Copyright (c) 2013, ARM Ltd. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <PiDxe.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/EfiResetSystemLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>

//检视意见修改 z00201473   2014.12.26  DTS2014122401186


/**
  Resets the entire platform.

  @param  ResetType             The type of reset to perform.
  @param  ResetStatus           The status code for the reset.
  @param  DataSize              The size, in bytes, of WatchdogData.
  @param  ResetData             For a ResetType of EfiResetCold, EfiResetWarm, or
                                EfiResetShutdown the data buffer starts with a Null-terminated
                                Unicode string, optionally followed by additional binary data.

**/
EFI_STATUS
EFIAPI
LibResetSystem (
  IN EFI_RESET_TYPE   ResetType,
  IN EFI_STATUS       ResetStatus,
  IN UINTN            DataSize,
  IN CHAR16           *ResetData OPTIONAL
  )
{
    UINT64    mResetRegBaseAddr;

    mResetRegBaseAddr = PcdGet64(PcdCpldBaseAddress);
    switch (ResetType)
    {
        case EfiResetPlatformSpecific:
            // Map the platform specific reset as reboot
        case EfiResetWarm:
            // Map a warm reset into a cold reset
        case EfiResetCold:
        case EfiResetShutdown:

            MmioWrite32(mResetRegBaseAddr + (0x16 << 1), 0x55);
            break;
        default:
            return EFI_INVALID_PARAMETER;
    }


    return EFI_SUCCESS;
}
