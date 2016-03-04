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
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcdLib.h>

#include <PlatformArch.h>
#include <Library/HwMemInitLib.h>
#include <Library/OemMiscLib.h>
#include <Library/SerdesLib.h>
#include <Library/I2CLib.h>
//#include <Resource.h>

#include <Protocol/Smbios.h>
#include <IndustryStandard/SmBios.h>

#include <Library/HiiLib.h>

I2C_SLAVE_ADDR  DimmSpdAddr[MAX_SOCKET][MAX_CHANNEL][MAX_DIMM] = {
    {{{I2C_PORT1,0x54},{I2C_PORT1,0x55},{I2C_INVALIDPORT,0xFF}},//Socket 0 TC DDRC0
    {{I2C_PORT0,0x54},{I2C_PORT0,0x55},{I2C_INVALIDPORT,0xFF}}, //Socket 0 TC DDRC1
    {{I2C_PORT1,0x50},{I2C_PORT1,0x51},{I2C_INVALIDPORT,0xFF}},      //Socket 0 TA DDRC0
    {{I2C_PORT0,0x50},{I2C_PORT0,0x51},{I2C_INVALIDPORT,0xFF}}},      //Socket 0 TA DDRC1
#if MAX_SOCKET > 1
    {{{I2C_PORT1,0x54},{I2C_PORT1,0x55},{I2C_INVALIDPORT,0xFF}},//Socket 0 TC DDRC0
    {{I2C_PORT0,0x54},{I2C_PORT0,0x55},{I2C_INVALIDPORT,0xFF}}, //Socket 0 TC DDRC1
    {{I2C_PORT1,0x50},{I2C_PORT1,0x51},{I2C_INVALIDPORT,0xFF}},      //Socket 0 TA DDRC0
    {{I2C_PORT0,0x50},{I2C_PORT0,0x51},{I2C_INVALIDPORT,0xFF}}},      //Socket 0 TA DDRC1
#endif

}; 

I2C_DEVICE gDS3231RtcDevice = {
    .Socket = 0,
    .Port = 6,
    .DeviceType = DEVICE_TYPE_SPD,
    .SlaveDeviceAddress = 0x68
};


// Set Tx output polarity. Not inverting data is default. For Phosphor660 16 core EVB Board
//if(((3 == ulMacroId) && (3 == ulDsNum)) || ((4 == ulMacroId) && ((0 == ulDsNum) || (2 == ulDsNum))))
SERDES_POLARITY_INVERT gSerdesPolarityTxDesc[] =
{
    //{3, 2},
    //{4, 1},
    //{4, 2},
    //{4, 3},
    {SERDES_INVALID_MACRO_ID, SERDES_INVALID_LANE_NUM}
};

// Set Rx data polarity. Not inverting data is default. For Phosphor660 32core EVB Board
//if((4 == ulMacroId) && ((0 == ulDsNum) || (2 == ulDsNum)))
SERDES_POLARITY_INVERT gSerdesPolarityRxDesc[] =
{
    //{4, 0},
    //{4, 2},
    {SERDES_INVALID_MACRO_ID, SERDES_INVALID_LANE_NUM}
};

serdes_param_t gSerdesParam = {
    .hilink0_mode = EM_HILINK0_PCIE1_8LANE,
    .hilink1_mode = EM_HILINK1_PCIE0_8LANE,
    .hilink2_mode = EM_HILINK2_PCIE2_8LANE,
    .hilink3_mode = 0x0,
    .hilink4_mode = 0xF,
    .hilink5_mode = EM_HILINK5_SAS1_4LANE,
    .hilink6_mode = 0x0,
	.use_ssc      = 0,
    };

serdes_param_t gSerdesParam0 = {
    .hilink0_mode = EM_HILINK0_HCCS1_8LANE_16,
    .hilink1_mode = EM_HILINK1_HCCS0_8LANE_16,
    .hilink2_mode = EM_HILINK2_PCIE2_8LANE,
    .hilink3_mode = 0x0,
    .hilink4_mode = 0xF,
    .hilink5_mode = EM_HILINK5_SAS1_4LANE,
    .hilink6_mode = 0x0,
	.use_ssc      = 0,
    };

serdes_param_t gSerdesParam1 = {
    .hilink0_mode = EM_HILINK0_HCCS1_8LANE_16,
    .hilink1_mode = EM_HILINK1_HCCS0_8LANE_16,
    .hilink2_mode = EM_HILINK2_PCIE2_8LANE,
    .hilink3_mode = 0x0,
    .hilink4_mode = 0xF,
    .hilink5_mode = EM_HILINK5_PCIE3_4LANE,
    .hilink6_mode = 0xF,
	.use_ssc      = 0,
    };

EFI_STATUS OemGetSerdesParam (serdes_param_t *Param)
{
  if (NULL == Param)
  {
    DEBUG((EFI_D_ERROR, "[%a]:[%dL] Param == NULL!\n", __FUNCTION__, __LINE__));
    return EFI_INVALID_PARAMETER;
  }
  (VOID) CopyMem(Param, &gSerdesParam, sizeof(*Param));
  return EFI_SUCCESS;
}

VOID OemPcieResetAndOffReset(void)
  {
    return;
  }

VOID BoardInformation(void)
{
    return;
}

ETH_PRODUCT_DESC gEthPdtDesc[ETH_MAX_PORT] = 
{   
    {TRUE,   ETH_SPEED_10KM,  ETH_FULL_DUPLEX, ETH_INVALID, ETH_INVALID},
    {TRUE,   ETH_SPEED_10KM,  ETH_FULL_DUPLEX, ETH_INVALID, ETH_INVALID},
    {FALSE,  ETH_INVALID,     ETH_INVALID,     ETH_INVALID, ETH_INVALID},
    {FALSE,  ETH_INVALID,     ETH_INVALID,     ETH_INVALID, ETH_INVALID},
    {TRUE,   ETH_SPEED_1000M, ETH_FULL_DUPLEX, ETH_PHY_MVL88E1512_ID, 0},
    {TRUE,   ETH_SPEED_1000M, ETH_FULL_DUPLEX, ETH_PHY_MVL88E1512_ID, 1},
    {FALSE,  ETH_INVALID,     ETH_INVALID,     ETH_INVALID, ETH_INVALID},
    {FALSE,  ETH_INVALID,     ETH_INVALID,     ETH_INVALID, ETH_INVALID}
};


UINT8   gEthSetupDesc[4] = {4,5,0,1};


/*****************************************************************************
 函 数 名  : OemEthFindFirstSP
 功能描述  : 查找产品第一个业务网口号
 输入参数  : 无
 输出参数  : 业务网口号
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年4月17日
    作    者   : ZhuGeSheng
    修改内容   : 新生成函数

*****************************************************************************/
UINT32 OemEthFindFirstSP()
{
    UINT32 i;

    for (i = 0; i < ETH_DEBUG_PORT0; i++)
    {
        if (TRUE == gEthPdtDesc[i].Valid)
        {
            return i;
        }
    }
    
    return ETH_INVALID;
}

/*****************************************************************************
 函 数 名  : OemEthInit
 功能描述  : 产品网卡相关信息初始化
 输入参数  : UINT port  
 输出参数  : 无
 返 回 值  : ETH_PRODUCT_DESC
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月23日
    作    者   : ZhuGeSheng
    修改内容   : 新生成函数

*****************************************************************************/
ETH_PRODUCT_DESC *OemEthInit(UINT32 port)
{
    return (ETH_PRODUCT_DESC *)(&(gEthPdtDesc[port]));
}

SMBIOS_TABLE_TYPE9 gPcieSlotInfo[] = {
    // PCIe0 Slot 1
    {
        {                                                     // Hdr
            EFI_SMBIOS_TYPE_SYSTEM_SLOTS,                         // Type,
            0,                                                    // Length,
            0                                                     // Handle
        },
        1,                                                    // SlotDesignation
        SlotTypePciExpressX8,     // SlotType
        SlotDataBusWidth8X,       // SlotDataBusWidth
        SlotUsageAvailable,       // SlotUsage
        SlotLengthOther,          // SlotLength
        0x0001,                   // SlotId
        {                         // SlotCharacteristics1
            0,                      // CharacteristicsUnknown  :1;
            0,                      // Provides50Volts         :1;
            0,                      // Provides33Volts         :1;
            0,                      // SharedSlot              :1;
            0,                      // PcCard16Supported       :1;
            0,                      // CardBusSupported        :1;
            0,                      // ZoomVideoSupported      :1;
            0                       // ModemRingResumeSupported:1;
        },
        {                         // SlotCharacteristics2
            0,                      // PmeSignalSupported      :1;
            0,                      // HotPlugDevicesSupported  :1;
            0,                      // SmbusSignalSupported    :1;
            0                       // Reserved                :5;
        },                        
        0x00,                     // SegmentGroupNum
        0x00,                     // BusNum
        0                         // DevFuncNum
    },
    
    // PCIe0 Slot 4
    {
        {                                                     // Hdr
            EFI_SMBIOS_TYPE_SYSTEM_SLOTS,                         // Type,
            0,                                                    // Length,
            0                                                     // Handle
        },
        1,                                                    // SlotDesignation
        SlotTypePciExpressX8,     // SlotType
        SlotDataBusWidth8X,       // SlotDataBusWidth
        SlotUsageAvailable,       // SlotUsage
        SlotLengthOther,          // SlotLength
        0x0004,                   // SlotId
        {                         // SlotCharacteristics1
            0,                      // CharacteristicsUnknown  :1;
            0,                      // Provides50Volts         :1;
            0,                      // Provides33Volts         :1;
            0,                      // SharedSlot              :1;
            0,                      // PcCard16Supported       :1;
            0,                      // CardBusSupported        :1;
            0,                      // ZoomVideoSupported      :1;
            0                       // ModemRingResumeSupported:1;
        },
        {                         // SlotCharacteristics2
            0,                      // PmeSignalSupported      :1;
            0,                      // HotPlugDevicesSupported  :1;
            0,                      // SmbusSignalSupported    :1;
            0                       // Reserved                :5;
        },                        
        0x00,                     // SegmentGroupNum
        0x00,                     // BusNum
        0                         // DevFuncNum
    }
};


UINT8 OemGetPcieSlotNumber ()
{
    return  sizeof (gPcieSlotInfo) / sizeof (SMBIOS_TABLE_TYPE9);
}

EFI_STRING_ID gDimmToDevLocator[MAX_SOCKET][MAX_CHANNEL][MAX_DIMM] = {
  {{STRING_TOKEN(STR_LEMON_C10_DIMM_000), STRING_TOKEN(STR_LEMON_C10_DIMM_001), STRING_TOKEN(STR_LEMON_C10_DIMM_002)},
   {STRING_TOKEN(STR_LEMON_C10_DIMM_010), STRING_TOKEN(STR_LEMON_C10_DIMM_011), STRING_TOKEN(STR_LEMON_C10_DIMM_012)},
   {STRING_TOKEN(STR_LEMON_C10_DIMM_020), STRING_TOKEN(STR_LEMON_C10_DIMM_021), STRING_TOKEN(STR_LEMON_C10_DIMM_022)},
   {STRING_TOKEN(STR_LEMON_C10_DIMM_030), STRING_TOKEN(STR_LEMON_C10_DIMM_031), STRING_TOKEN(STR_LEMON_C10_DIMM_032)}},

  {{STRING_TOKEN(STR_LEMON_C10_DIMM_100), STRING_TOKEN(STR_LEMON_C10_DIMM_101), STRING_TOKEN(STR_LEMON_C10_DIMM_102)},
   {STRING_TOKEN(STR_LEMON_C10_DIMM_110), STRING_TOKEN(STR_LEMON_C10_DIMM_111), STRING_TOKEN(STR_LEMON_C10_DIMM_112)},
   {STRING_TOKEN(STR_LEMON_C10_DIMM_120), STRING_TOKEN(STR_LEMON_C10_DIMM_121), STRING_TOKEN(STR_LEMON_C10_DIMM_122)},
   {STRING_TOKEN(STR_LEMON_C10_DIMM_130), STRING_TOKEN(STR_LEMON_C10_DIMM_131), STRING_TOKEN(STR_LEMON_C10_DIMM_132)}}
};

EFI_HII_HANDLE
EFIAPI
OemGetPackages (
  )
{
    return HiiAddPackages (
                            &gEfiCallerIdGuid,
                            NULL,
                            OemMiscLib2PStrings,
                            NULL,
                            NULL
                            );  
}


