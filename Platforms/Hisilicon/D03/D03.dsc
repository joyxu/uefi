#
#  Copyright (c) 2011-2012, ARM Limited. All rights reserved.
#
#  This program and the accompanying materials
#  are licensed and made available under the terms and conditions of the BSD License
#  which accompanies this distribution.  The full text of the license may be found at
#  http://opensource.org/licenses/bsd-license.php
#
#  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
#  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
#
#

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  PLATFORM_NAME                  = D03
  PLATFORM_GUID                  = e5003abd-8809-6194-ac3d-a6a99ff52478
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/$(PLATFORM_NAME)
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = OpenPlatformPkg/Platforms/Hisilicon/$(PLATFORM_NAME)/$(PLATFORM_NAME).fdf
  DEFINE EDK2_SKIP_PEICORE=0
  DEFINE INCLUDE_TFTP_COMMAND=1

!include OpenPlatformPkg/Chips/Hisilicon/Pv660/Pv660.dsc.inc

[LibraryClasses.common]
  ArmLib|ArmPkg/Library/ArmLib/AArch64/AArch64Lib.inf
  ArmPlatformLib|OpenPlatformPkg/Chips/Hisilicon/Library/ArmPlatformLibPv660/ArmPlatformLib.inf

  ArmPlatformSysConfigLib|ArmPlatformPkg/ArmVExpressPkg/Library/ArmVExpressSysConfigLib/ArmVExpressSysConfigLib.inf
  NorFlashPlatformLib|ArmPlatformPkg/ArmVExpressPkg/Library/NorFlashArmVExpressLib/NorFlashArmVExpressLib.inf
  LcdPlatformLib|ArmPlatformPkg/ArmVExpressPkg/Library/PL111LcdArmVExpressLib/PL111LcdArmVExpressLib.inf

  #DebugAgentTimerLib|ArmPlatformPkg/ArmVExpressPkg/Library/DebugAgentTimerLib/DebugAgentTimerLib.inf

 
  I2CLib|OpenPlatformPkg/Chips/Hisilicon/Library/I2CLib/I2CLib.inf
  TimerLib|ArmPkg/Library/ArmArchTimerLib/ArmArchTimerLib.inf
 
  IpmiCmdLib|OpenPlatformPkg/Chips/Hisilicon/Binary/Hi1610/Library/IpmiCmdLib/IpmiCmdLib.inf

  NetLib|MdeModulePkg/Library/DxeNetLib/DxeNetLib.inf
  DpcLib|MdeModulePkg/Library/DxeDpcLib/DxeDpcLib.inf
  HiiLib|MdeModulePkg/Library/UefiHiiLib/UefiHiiLib.inf
  UefiHiiServicesLib|MdeModulePkg/Library/UefiHiiServicesLib/UefiHiiServicesLib.inf
  UdpIoLib|MdeModulePkg/Library/DxeUdpIoLib/DxeUdpIoLib.inf
  IpIoLib|MdeModulePkg/Library/DxeIpIoLib/DxeIpIoLib.inf



  #FDTUpdateLib
  FdtUpdateLib|OpenPlatformPkg/Platforms/Hisilicon/D03/Library/FdtUpdateLib/FdtUpdateLib.inf


  CpldIoLib|OpenPlatformPkg/Chips/Hisilicon/Library/CpldIoLib/CpldIoLib.inf
  
  SerdesLib|OpenPlatformPkg/Chips/Hisilicon/Binary/Hi1610/Library/Hi1610Serdes/Hi1610SerdesLib.inf
  
  # ARM PL031 RTC Driver
  
  RealTimeClockLib|OpenPlatformPkg/Platforms/Hisilicon/D03/Library/DS3231RealTimeClockLib/DS3231RealTimeClockLib.inf
 
  OemMiscLib|OpenPlatformPkg/Platforms/Hisilicon/D03/Library/OemMiscLib2P/OemMiscLib2PHi1610.inf
  
  BootLineLib|OpenPlatformPkg/Platforms/Hisilicon/D02/Library/BootLineLibD02/BootLineLibD02.inf
 
  
  OemAddressMapLib|OpenPlatformPkg/Platforms/Hisilicon/Binary/D03/Library/OemAddressMap2P/OemAddressMap2PHi1610.inf
  PlatformSysCtrlLib|OpenPlatformPkg/Chips/Hisilicon/Binary/Hi1610/Library/PlatformSysCtrlLibHi1610/PlatformSysCtrlLibHi1610.inf

  CapsuleLib|MdeModulePkg/Library/DxeCapsuleLibNull/DxeCapsuleLibNull.inf
  GenericBdsLib|IntelFrameworkModulePkg/Library/GenericBdsLib/GenericBdsLib.inf
  #PlatformBdsLib|ArmPlatformPkg/ArmVirtualizationPkg/Library/PlatformIntelBdsLib/PlatformIntelBdsLib.inf
  PlatformBdsLib|OpenPlatformPkg/Chips/Hisilicon/Override/ArmVirtPkg/Library/PlatformIntelBdsLib/PlatformIntelBdsLib.inf
  CustomizedDisplayLib|MdeModulePkg/Library/CustomizedDisplayLib/CustomizedDisplayLib.inf
  #QemuBootOrderLib|OvmfPkg/Library/QemuBootOrderLib/QemuBootOrderLib.inf

  # USB Requirements
  UefiUsbLib|MdePkg/Library/UefiUsbLib/UefiUsbLib.inf
  
  LpcLib|OpenPlatformPkg/Chips/Hisilicon/Binary/Hi1610/Library/LpcLib/LpcLib.inf
  SerialPortLib|OpenPlatformPkg/Chips/Hisilicon/Binary/Hi1610/Library/Uart/LpcSerialPortLib/LpcSerialPortLib.inf
[LibraryClasses.common.SEC]
  ArmLib|ArmPkg/Library/ArmLib/AArch64/AArch64LibSec.inf
  #ArmPlatformSecLib|OpenPlatformPkg/Platforms/Hisilicon/D03/Library/HwSecPlatformLib/ArmVExpressSecLib.inf
  #ArmPlatformLib|ArmPlatformPkg/ArmVExpressPkg/Library/ArmVExpressLibRTSM/ArmVExpressLibSec.inf
  ArmPlatformLib|OpenPlatformPkg/Chips/Hisilicon/Library/ArmPlatformLibPv660/ArmPlatformLibSec.inf
  EfiResetSystemLib|OpenPlatformPkg/Platforms/Hisilicon/D03/Library/ResetSystemLibCpld/ResetSystemLibSec.inf


[LibraryClasses.common.DXE_RUNTIME_DRIVER]
  #EfiResetSystemLib|HwProductsPkg/2PServer/Library/ResetSystemLibCpld/ResetSystemLib.inf
  I2CLib|OpenPlatformPkg/Chips/Hisilicon/Library/I2CLib/I2CLibRuntime.inf
  SerialPortLib|OpenPlatformPkg/Chips/Hisilicon/Binary/Hi1610/Library/Uart/LpcSerialPortLib/LpcSerialPortLib.inf

[BuildOptions]
  GCC:*_*_AARCH64_ARCHCC_FLAGS  = -DARM_CPU_AARCH64 -mstrict-align
  GCC:*_*_AARCH64_PP_FLAGS  = -DARM_CPU_AARCH64
 
  GCC:*_*_AARCH64_PLATFORM_FLAGS == -I$(WORKSPACE)/ArmPlatformPkg/ArmVExpressPkg/Include -I$(WORKSPACE)/ArmPlatformPkg/ArmVExpressPkg/Include/Platform/RTSM -I$(WORKSPACE)/OpenPlatformPkg/Chips/Hisilicon/Hi1610/Include

[BuildOptions.AARCH64.EDKII.UEFI_APPLICATION]
  GCC:*_*_*_CC_FLAGS = -mcmodel=small -falign-functions=16
  GCC:*_*_*_DLINK_FLAGS = -z common-page-size=0x1000 --sort-section=alignment


################################################################################
#
# Pcd Section - list of all EDK II PCD Entries defined by this Platform
#
################################################################################

[PcdsFeatureFlag.common]

!if $(EDK2_SKIP_PEICORE) == 1
  gArmPlatformTokenSpaceGuid.PcdSystemMemoryInitializeInSec|TRUE
  gArmPlatformTokenSpaceGuid.PcdSendSgiToBringUpSecondaryCores|TRUE
!endif

  ## If TRUE, Graphics Output Protocol will be installed on virtual handle created by ConsplitterDxe.
  #  It could be set FALSE to save size.
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutGopSupport|TRUE
  
[PcdsFixedAtBuild.common]
  gArmPlatformTokenSpaceGuid.PcdFirmwareVendor|"ARM Versatile Express"
  gEmbeddedTokenSpaceGuid.PcdEmbeddedPrompt|"D03"

  gArmPlatformTokenSpaceGuid.PcdCoreCount|8

  #
  # NV Storage PCDs. Use base of 0x0C000000 for NOR1
  #
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageVariableBase|0xa49D0000
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageVariableSize|0x0000E000
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwWorkingBase|0xa49DE000
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwWorkingSize|0x00002000
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwSpareBase|0xa49E0000
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwSpareSize|0x00010000

  
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxVariableSize|0x2000

  # Stacks for MPCores in Secure World
  #gArmPlatformTokenSpaceGuid.PcdCPUCoresSecStackBase|0x21000000
  #gArmPlatformTokenSpaceGuid.PcdCPUCoreSecPrimaryStackSize|0x10000
  gArmPlatformTokenSpaceGuid.PcdCPUCoresSecStackBase|0x81000000
  gArmPlatformTokenSpaceGuid.PcdCPUCoreSecPrimaryStackSize|0x10000

  # Stacks for MPCores in Monitor Mode
  #gArmPlatformTokenSpaceGuid.PcdCPUCoresSecMonStackBase|0x2E008000
  #gArmPlatformTokenSpaceGuid.PcdCPUCoreSecMonStackSize|0x100
  gArmPlatformTokenSpaceGuid.PcdCPUCoresSecMonStackBase|0x8100FF00
  gArmPlatformTokenSpaceGuid.PcdCPUCoreSecMonStackSize|0x100

  # Stacks for MPCores in Normal World
  #gArmPlatformTokenSpaceGuid.PcdCPUCoresStackBase|0x2E000000
  #gArmPlatformTokenSpaceGuid.PcdCPUCorePrimaryStackSize|0x4000
  gArmPlatformTokenSpaceGuid.PcdCPUCoresStackBase|0x81000000
  gArmPlatformTokenSpaceGuid.PcdCPUCorePrimaryStackSize|0xFF00
  #UEFI BIOS_t00216239_end   2014-10-29 10:59:46

  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x00000000
  
  
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x3FC00000 
 

  # Size of the region used by UEFI in permanent memory (Reserved 64MB)
  gArmPlatformTokenSpaceGuid.PcdSystemMemoryUefiRegionSize|0x10000000

  gHwTokenSpaceGuid.PcdSasCtrlId|1  
 
  gHwTokenSpaceGuid.PcdSerDesFlowCtrlFlag|1
  
  
  #
  # ARM Pcds
  #
  gArmTokenSpaceGuid.PcdArmUncachedMemoryMask|0x0000000040000000

  #
  # ARM PrimeCell
  #

  
  gHwTokenSpaceGuid.PcdSlotPerChannelNum|0x2

  
  gHwTokenSpaceGuid.PcdPcieRootBridgeMask|0x7 # bit0:HB0RB0,bit1:HB0RB1,bit2:HB0RB2,bit3:HB0RB3,bit4:HB1RB0,bit5:HB1RB1,bit6:HB1RB2,bit7:HB1RB3
  
  ## SP805 Watchdog - Motherboard Watchdog
  gArmPlatformTokenSpaceGuid.PcdSP805WatchdogBase|0x601e0000
  
  ## Serial Terminal
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialRegisterBase|0x2F8
  #gEfiMdeModulePkgTokenSpaceGuid.PcdSerialRegisterBase|0x60300000
  gEfiMdePkgTokenSpaceGuid.PcdUartDefaultBaudRate|115200

  gHwTokenSpaceGuid.PcdUartClkInHz|1846100
  #gHwTokenSpaceGuid.PcdUartClkInHz|200000000

  gHwTokenSpaceGuid.PcdSerialPortSendDelay|10000000

  gEfiMdePkgTokenSpaceGuid.PcdUartDefaultDataBits|8
  gEfiMdePkgTokenSpaceGuid.PcdUartDefaultParity|1
  gEfiMdePkgTokenSpaceGuid.PcdUartDefaultStopBits|1

  
  gHwTokenSpaceGuid.PcdM3SmmuBaseAddress|0xa0040000
  gHwTokenSpaceGuid.PcdPcieSmmuBaseAddress|0xb0040000
  gHwTokenSpaceGuid.PcdDsaSmmuBaseAddress|0xc0040000
  gHwTokenSpaceGuid.PcdAlgSmmuBaseAddress|0xd0040000

 
  gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwareVersionString|L"Estuary v2.2 D03 UEFI"
 
  gHwTokenSpaceGuid.PcdBiosVersionString|L"10.01.01T18"
  
  gHwTokenSpaceGuid.PcdBiosVersionForBmc|L"1.12"

  gHwTokenSpaceGuid.PcdSystemProductName|L"D03"
  gHwTokenSpaceGuid.PcdSystemVersion|L"Estuary"
  gHwTokenSpaceGuid.PcdBaseBoardProductName|L"D03"
  gHwTokenSpaceGuid.PcdBaseBoardVersion|L"Estuary"
 
  gHwTokenSpaceGuid.PcdCPUInfo|L"Hi1612"
  
  #
  # ARM PL390 General Interrupt Controller
  #

  gArmTokenSpaceGuid.PcdGicDistributorBase|0x4D000000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0xFE000000

  #
  # ARM OS Loader
  #
  # Versatile Express machine type (ARM VERSATILE EXPRESS = 2272) required for ARM Linux:
  gArmPlatformTokenSpaceGuid.PcdDefaultBootDescription|L"Linux from SATA"
  gArmPlatformTokenSpaceGuid.PcdDefaultBootDevicePath|L"EFI\GRUB2\grubaa64.efi"
  gArmPlatformTokenSpaceGuid.PcdDefaultBootArgument|""

  # Use the serial console (ConIn & ConOut) and the Graphic driver (ConOut)
  gArmPlatformTokenSpaceGuid.PcdDefaultConOutPaths|L"VenHw(D3987D4B-971A-435F-8CAF-4967EB627241)/Uart(115200,8,N,1)/VenPcAnsi();VenHw(407B4008-BF5B-11DF-9547-CF16E0D72085)"
  gArmPlatformTokenSpaceGuid.PcdDefaultConInPaths|L"VenHw(D3987D4B-971A-435F-8CAF-4967EB627241)/Uart(115200,8,N,1)/VenPcAnsi()"

  #
  # ARM L2x0 PCDs
  #
  gArmTokenSpaceGuid.PcdL2x0ControllerBase|0x1E00A000

  #
  # ARM Architectual Timer Frequency
  #
  # Set model tick to 120Mhz. This depends a lot on workstation performance.
  #gArmTokenSpaceGuid.PcdArmArchTimerFreqInHz|120000000

  gArmTokenSpaceGuid.PcdArmArchTimerFreqInHz|66000000

 
  gEfiMdeModulePkgTokenSpaceGuid.PcdResetOnMemoryTypeInformationChange|FALSE
  gEfiIntelFrameworkModulePkgTokenSpaceGuid.PcdShellFile|{ 0x83, 0xA5, 0x04, 0x7C, 0x3E, 0x9E, 0x1C, 0x4F, 0xAD, 0x65, 0xE0, 0x52, 0x68, 0xD0, 0xB4, 0xD1 }

  gHwTokenSpaceGuid.PcdSysControlBaseAddress|0x40010000
  gHwTokenSpaceGuid.PcdMailBoxAddress|0x0000FFF8

  gHwTokenSpaceGuid.PcdCpldBaseAddress|0x78000000
 
  gHwTokenSpaceGuid.PcdSFCCFGBaseAddress|0xA6000000      
  gHwTokenSpaceGuid.PcdSFCMEM0BaseAddress|0xA4000000      

  
  gHwTokenSpaceGuid.PcdRamDiskMaxSize|128

  
  gHwTokenSpaceGuid.PcdPeriSubctrlAddress|0x40000000

  
  gHwTokenSpaceGuid.PcdMdioSubctrlAddress|0x60000000

  gHwTokenSpaceGuid.PcdNORFlashBase|0x70000000
  gHwTokenSpaceGuid.PcdNORFlashCachableSize|0x8000000

  gHwTokenSpaceGuid.FdtFileAddress|0xA47C0000
  
  gHwTokenSpaceGuid.PcdPlatformDefaultPackageType|0x1

  gHwTokenSpaceGuid.PcdArmPrimaryCoreTemp|0x80010000

  gHwTokenSpaceGuid.PcdTopOfLowMemory|0x40000000
  
  gHwTokenSpaceGuid.PcdBottomOfHighMemory|0x2000000000

  ## Reserved Memory for NVRAM BaseAddress and Size
  gHwTokenSpaceGuid.PcdReservedNvramBase|0x0
  gHwTokenSpaceGuid.PcdReservedNvramSize|0x0
  
  gHwTokenSpaceGuid.PcdTrustedFirmwareEnable|0x1


  gHwTokenSpaceGuid.PcdIsMPBoot|0

  gHwTokenSpaceGuid.PcdNumaEnable|0
  
  gHwTokenSpaceGuid.PcdMacAddress|0xA47E0000

  gHwTokenSpaceGuid.PcdHb1BaseAddress|0x40000000000


  gHwTokenSpaceGuid.PcdHb0Rb0PciConfigurationSpaceBaseAddress|0xB0000000
  gHwTokenSpaceGuid.PcdHb0Rb0PciConfigurationSpaceSize|0x8000000

  gHwTokenSpaceGuid.PcdHb0Rb1PciConfigurationSpaceBaseAddress|0xB0000000
  gHwTokenSpaceGuid.PcdHb0Rb1PciConfigurationSpaceSize|0x10000000

  gHwTokenSpaceGuid.PcdHb0Rb2PciConfigurationSpaceBaseAddress|0xA0000000
  gHwTokenSpaceGuid.PcdHb0Rb2PciConfigurationSpaceSize|0x10000000

  gHwTokenSpaceGuid.PcdHb0Rb3PciConfigurationSpaceBaseAddress|0xAC000000
  gHwTokenSpaceGuid.PcdHb0Rb3PciConfigurationSpaceSize|0x4000000

  gHwTokenSpaceGuid.PciHb0Rb0Base|0xa0090000
  gHwTokenSpaceGuid.PciHb0Rb1Base|0xa0200000
  gHwTokenSpaceGuid.PciHb0Rb2Base|0xa00a0000
  gHwTokenSpaceGuid.PciHb0Rb3Base|0xa00b0000

  gHwTokenSpaceGuid.PcdHb0Rb0PciRegionBaseAddress|0xb2000000
  gHwTokenSpaceGuid.PcdHb0Rb0PciRegionSize|0x5feffff

  gHwTokenSpaceGuid.PcdHb0Rb1PciRegionBaseAddress|0xb8000000
  gHwTokenSpaceGuid.PcdHb0Rb1PciRegionSize|0x5feffff

  gHwTokenSpaceGuid.PcdHb0Rb2PciRegionBaseAddress|0xaa000000
  gHwTokenSpaceGuid.PcdHb0Rb2PciRegionSize|0x5feffff

  gHwTokenSpaceGuid.PcdHb0Rb0CpuMemRegionBase|0xB2000000
  gHwTokenSpaceGuid.PcdHb0Rb1CpuMemRegionBase|0xB8000000
  gHwTokenSpaceGuid.PcdHb0Rb2CpuMemRegionBase|0xAA000000

  gHwTokenSpaceGuid.PcdHb0Rb0CpuIoRegionBase|0xb7ff0000
  gHwTokenSpaceGuid.PcdHb0Rb1CpuIoRegionBase|0xbdff0000
  gHwTokenSpaceGuid.PcdHb0Rb2CpuIoRegionBase|0xAfff0000

  gHwTokenSpaceGuid.PcdHb0Rb0IoBase|0
  gHwTokenSpaceGuid.PcdHb0Rb0IoSize|0xffff #64K

  gHwTokenSpaceGuid.PcdHb0Rb1IoBase|0
  gHwTokenSpaceGuid.PcdHb0Rb1IoSize|0xffff #64K

  gHwTokenSpaceGuid.PcdHb0Rb2IoBase|0
  gHwTokenSpaceGuid.PcdHb0Rb2IoSize|0xffff #64K

  gHwTokenSpaceGuid.Pcdsoctype|0x1610
  gHwTokenSpaceGuid.PcdBoardType|1

  ## SP804 DualTimer
  gArmPlatformTokenSpaceGuid.PcdSP804TimerFrequencyInMHz|200
  gArmPlatformTokenSpaceGuid.PcdSP804TimerPeriodicInterruptNum|48
  gArmPlatformTokenSpaceGuid.PcdSP804TimerPeriodicBase|0x40060000
  ## TODO: need to confirm the base for Performance and Metronome base for PV660
  gArmPlatformTokenSpaceGuid.PcdSP804TimerPerformanceBase|0x40060000
  gArmPlatformTokenSpaceGuid.PcdSP804TimerMetronomeBase|0x40060000

  gHwTokenSpaceGuid.PcdPcieMsiTargetaddress|0xc6010040

################################################################################
#
# Components Section - list of all EDK II Modules needed by this Platform
#
################################################################################
[Components.common]

  #
  # SEC
  #
  #HwPkg/Override/ArmPlatformPkg/Sec/Sec.inf {
  #  <LibraryClasses>
  #    # Use the implementation which set the Secure bits
  #    ArmGicLib|ArmPkg/Drivers/ArmGic/ArmGicSecLib.inf
  #}

  #
  # PEI Phase modules
  #
!if $(EDK2_SKIP_PEICORE) == 1
  ArmPlatformPkg/PrePi/PeiMPCore.inf {
    <LibraryClasses>
      ArmLib|ArmPkg/Library/ArmLib/AArch64/AArch64Lib.inf
      ArmPlatformLib|OpenPlatformPkg/Chips/Hisilicon/Library/ArmPlatformLibPv660/ArmPlatformLib.inf
      ArmPlatformGlobalVariableLib|ArmPlatformPkg/Library/ArmPlatformGlobalVariableLib/PrePi/PrePiArmPlatformGlobalVariableLib.inf
  }
  ArmPlatformPkg/PrePi/PeiUniCore.inf {
    <LibraryClasses>
      ArmLib|ArmPkg/Library/ArmLib/AArch64/AArch64Lib.inf
      ArmPlatformLib|OpenPlatformPkg/Chips/Hisilicon/Library/ArmPlatformLibPv660/ArmPlatformLib.inf
      ArmPlatformGlobalVariableLib|ArmPlatformPkg/Library/ArmPlatformGlobalVariableLib/PrePi/PrePiArmPlatformGlobalVariableLib.inf
  }
!else
  ArmPlatformPkg/PrePeiCore/PrePeiCoreMPCore.inf
  MdeModulePkg/Core/Pei/PeiMain.inf
  MdeModulePkg/Universal/PCD/Pei/Pcd.inf
  
  OpenPlatformPkg/Platforms/Hisilicon/Binary/D03/Drivers/Ipmi/IpmiInterfacePei/IpmiInterfacePei.inf
 
  
  ArmPlatformPkg/PlatformPei/PlatformPeim.inf
  
  OpenPlatformPkg/Platforms/Hisilicon/Binary/D03/MemoryInitPei/MemoryInitPeim.inf
  ArmPkg/Drivers/CpuPei/CpuPei.inf
  IntelFrameworkModulePkg/Universal/StatusCode/Pei/StatusCodePei.inf
  MdeModulePkg/Universal/FaultTolerantWritePei/FaultTolerantWritePei.inf
  MdeModulePkg/Universal/Variable/Pei/VariablePei.inf
  
  OpenPlatformPkg/Platforms/Hisilicon/D03/Drivers/BootLinuxConfig/BootLinuxConfigPeimHi1610.inf

  MdeModulePkg/Core/DxeIplPeim/DxeIpl.inf {
    <LibraryClasses>
      NULL|IntelFrameworkModulePkg/Library/LzmaCustomDecompressLib/LzmaCustomDecompressLib.inf
  }
!endif

  #
  # DXE
  #
  MdeModulePkg/Core/Dxe/DxeMain.inf {
    <LibraryClasses>
      NULL|MdeModulePkg/Library/DxeCrc32GuidedSectionExtractLib/DxeCrc32GuidedSectionExtractLib.inf
  }
  MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  OpenPlatformPkg/Chips/Hisilicon/Hi1610/Drivers/IoInitDxe/IoInitDxe.inf

  #
  # Architectural Protocols
  #
  ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf

 
  OpenPlatformPkg/Platforms/Hisilicon/D03/Drivers/OemNicConfig2PHi1610/OemNicConfig2P.inf
  
  OpenPlatformPkg/Platforms/Hisilicon/Binary/D03/Drivers/SFC/SfcDxeDriver.inf

  MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf
  MdeModulePkg/Universal/Variable/EmuRuntimeDxe/EmuVariableRuntimeDxe.inf
  OpenPlatformPkg/Chips/Hisilicon/Drivers/FlashFvbDxe/FlashFvbDxe.inf
  MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf {
    <LibraryClasses>
      NULL|MdeModulePkg/Library/VarCheckUefiLib/VarCheckUefiLib.inf
  }
  MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  MdeModulePkg/Universal/FaultTolerantWriteDxe/FaultTolerantWriteDxe.inf
  EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf

  MdeModulePkg/Universal/MonotonicCounterRuntimeDxe/MonotonicCounterRuntimeDxe.inf
  EmbeddedPkg/ResetRuntimeDxe/ResetRuntimeDxe.inf
  EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf {
    <LibraryClasses>
      CpldIoLib|OpenPlatformPkg/Chips/Hisilicon/Library/CpldIoLib/CpldIoLibRuntime.inf
  }
  EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf

  MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
  MdeModulePkg/Universal/Console/TerminalDxe/TerminalDxe.inf
  MdeModulePkg/Universal/SerialDxe/SerialDxe.inf

  # Simple TextIn/TextOut for UEFI Terminal
  EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf

  MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  #ArmPkg/Drivers/PL390Gic/PL390GicDxe.inf
  OpenPlatformPkg/Chips/Hisilicon/Drivers/Hic/HicDxe.inf

  #ArmPkg/Drivers/TimerDxe/TimerDxe
  ArmPlatformPkg/Drivers/SP804TimerDxe/SP804TimerDxe.inf
  ArmPlatformPkg/Drivers/LcdGraphicsOutputDxe/PL111LcdGraphicsOutputDxe.inf

  ArmPlatformPkg/Drivers/SP805WatchdogDxe/SP805WatchdogDxe.inf
  IntelFrameworkModulePkg/Universal/StatusCode/RuntimeDxe/StatusCodeRuntimeDxe.inf
  #
  #ACPI
  #
  MdeModulePkg/Universal/Acpi/AcpiPlatformDxe/AcpiPlatformDxe.inf
  MdeModulePkg/Universal/Acpi/AcpiTableDxe/AcpiTableDxe.inf

  OpenPlatformPkg/Chips/Hisilicon/Pv660/Pv660AcpiTables/AcpiTablesHi1610.inf

  #
  # Usb Support
  #
  OpenPlatformPkg/Chips/Hisilicon/Drivers/EhciDxe/EhciDxe.inf
  OpenPlatformPkg/Platforms/Hisilicon/Binary/D03/Drivers/OhciDxe/OhciDxe.inf
  MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  MdeModulePkg/Bus/Usb/UsbMouseDxe/UsbMouseDxe.inf
  MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf
  
  
  OpenPlatformPkg/Platforms/Hisilicon/Binary/D03/Drivers/Ipmi/ipmiInterfaceDxe/IpmiInterfaceDxe.inf
  

  #
  #network
  #
  OpenPlatformPkg/Platforms/Hisilicon/Binary/D03/Drivers/Net/SnpPV660DxeMac0/SnpPV600DxeMac0.inf
  OpenPlatformPkg/Platforms/Hisilicon/Binary/D03/Drivers/Net/SnpPV660DxeMac1/SnpPV600DxeMac1.inf
  OpenPlatformPkg/Platforms/Hisilicon/Binary/D03/Drivers/Net/SnpPV660DxeMac4/SnpPV600DxeMac4.inf
  OpenPlatformPkg/Platforms/Hisilicon/Binary/D03/Drivers/Net/SnpPV660DxeMac5/SnpPV600DxeMac5.inf

  MdeModulePkg/Universal/Network/ArpDxe/ArpDxe.inf
  MdeModulePkg/Universal/Network/Dhcp4Dxe/Dhcp4Dxe.inf
  MdeModulePkg/Universal/Network/DpcDxe/DpcDxe.inf
  MdeModulePkg/Universal/Network/Ip4Dxe/Ip4Dxe.inf
  MdeModulePkg/Universal/Network/MnpDxe/MnpDxe.inf
  MdeModulePkg/Universal/Network/Mtftp4Dxe/Mtftp4Dxe.inf
  MdeModulePkg/Universal/Network/Tcp4Dxe/Tcp4Dxe.inf
  MdeModulePkg/Universal/Network/Udp4Dxe/Udp4Dxe.inf
  MdeModulePkg/Universal/Network/UefiPxeBcDxe/UefiPxeBcDxe.inf
  MdeModulePkg/Universal/Network/VlanConfigDxe/VlanConfigDxe.inf

  #
  # Semi-hosting filesystem
  #
  ArmPkg/Filesystem/SemihostFs/SemihostFs.inf

  #
  # Multimedia Card Interface
  #
  #EmbeddedPkg/Universal/MmcDxe/MmcDxe.inf
  #ArmPlatformPkg/Drivers/PL180MciDxe/PL180MciDxe.inf
  
  
  OpenPlatformPkg/Platforms/Hisilicon/Binary/D03/Drivers/Sas/SasDxeDriver.inf
  
  #
  # FAT filesystem + GPT/MBR partitioning
  #
 
  OpenPlatformPkg/Chips/Hisilicon/Drivers/ramdisk/ramdisk.inf
  MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  OpenPlatformPkg/Platforms/Hisilicon/Binary/D03/Ebl/Ebl.inf
  #c00213799 secure boot test
  MdeModulePkg/Application/HelloWorld/HelloWorld.inf
  #
  # Bds
  #
  MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf

  #OpenPlatformPkg/Chips/Hisilicon/Network/OemFtp/OemFtp.inf

  MdeModulePkg/Bus/Ata/AtaBusDxe/AtaBusDxe.inf

 
  OpenPlatformPkg/Platforms/Hisilicon/Binary/D03/Drivers/GetInfoFromBmc/GetInfoFromBmc.inf
  OpenPlatformPkg/Platforms/Hisilicon/Binary/D03/Drivers/TransferSmbiosInfo/TransSmbiosInfo.inf
  OpenPlatformPkg/Platforms/Hisilicon/Binary/D03/Drivers/IpmiMiscOpDxe/IpmiMiscOpDxe.inf
  
 
  OpenPlatformPkg/Platforms/Hisilicon/Binary/D03/Drivers/IpmiWatchdogDxe/IpmiWatchdogDxe.inf
  
  MdeModulePkg/Universal/SmbiosDxe/SmbiosDxe.inf
  OpenPlatformPkg/Chips/Hisilicon/Drivers/Smbios/SmbiosMiscDxe/SmbiosMiscDxe.inf

  OpenPlatformPkg/Chips/Hisilicon/Drivers/UpdateFdtDxe/UpdateFdtDxe.inf
  
 
  #PCIe Support
  OpenPlatformPkg/Chips/Hisilicon/Drivers/CpuIo2Dxe/CpuIo2Dxe.inf
  OpenPlatformPkg/Chips/Hisilicon/Hi1610/Drivers/PcieInit1610/PcieInitDxe.inf
  OpenPlatformPkg/Platforms/Hisilicon/D03/Drivers/PciPlatform/PciPlatform.inf
  OpenPlatformPkg/Chips/Hisilicon/Hi1610/Drivers/PciHostBridgeDxe1610/PciHostBridgeDxe.inf {
    <LibraryClasses>
      NULL|OpenPlatformPkg/Platforms/Hisilicon/D03/Library/PlatformPciLib/PlatformPciLib.inf
  }

  MdeModulePkg/Bus/Pci/PciBusDxe/PciBusDxe.inf
  
  OpenPlatformPkg/Platforms/Hisilicon/Binary/D03/Drivers/ReportPciePlugDidVidToBmc/ReportPciePlugDidVidToBmc.inf

  OpenPlatformPkg/Chips/Hisilicon/Drivers/Smbios/AddSmbiosType9/AddSmbiosType9.inf
  

  
  OpenPlatformPkg/Platforms/Hisilicon/Binary/D03/Drivers/Sm750Dxe/UefiSmi.inf
 
  OpenPlatformPkg/Chips/Hisilicon/Drivers/Smbios/MemorySubClassDxe/MemorySubClassDxe.inf

  
  OpenPlatformPkg/Chips/Hisilicon/Drivers/Smbios/ProcessorSubClassDxe/ProcessorSubClassDxe.inf

  #
  # Memory test
  #
  MdeModulePkg/Universal/MemoryTest/GenericMemoryTestDxe/GenericMemoryTestDxe.inf

  MdeModulePkg/Universal/DisplayEngineDxe/DisplayEngineDxe.inf
  MdeModulePkg/Universal/SetupBrowserDxe/SetupBrowserDxe.inf
  IntelFrameworkModulePkg/Universal/BdsDxe/BdsDxe.inf
  OpenPlatformPkg/Chips/Hisilicon/Application/EmbeddedLoader/EslStartOs.inf

  #
  # UEFI application (Shell Embedded Boot Loader)
  #
  ShellPkg/Application/Shell/Shell.inf {
    <LibraryClasses>
      ShellCommandLib|ShellPkg/Library/UefiShellCommandLib/UefiShellCommandLib.inf
      NULL|ShellPkg/Library/UefiShellLevel2CommandsLib/UefiShellLevel2CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellLevel1CommandsLib/UefiShellLevel1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellLevel3CommandsLib/UefiShellLevel3CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellDriver1CommandsLib/UefiShellDriver1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellDebug1CommandsLib/UefiShellDebug1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellInstall1CommandsLib/UefiShellInstall1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellNetwork1CommandsLib/UefiShellNetwork1CommandsLib.inf
      HandleParsingLib|ShellPkg/Library/UefiHandleParsingLib/UefiHandleParsingLib.inf
      PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
      BcfgCommandLib|ShellPkg/Library/UefiShellBcfgCommandLib/UefiShellBcfgCommandLib.inf
!ifdef $(INCLUDE_DP)
      NULL|ShellPkg/Library/UefiDpLib/UefiDpLib.inf
!endif #$(INCLUDE_DP)
!ifdef $(INCLUDE_TFTP_COMMAND)
      NULL|ShellPkg/Library/UefiShellTftpCommandLib/UefiShellTftpCommandLib.inf
!endif #$(INCLUDE_TFTP_COMMAND)

    <PcdsFixedAtBuild>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0xFF
      gEfiShellPkgTokenSpaceGuid.PcdShellLibAutoInitialize|FALSE
      gEfiMdePkgTokenSpaceGuid.PcdUefiLibMaxPrintBufferSize|8000
  }
