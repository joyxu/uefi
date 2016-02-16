/*-----------------------------------------------------------------------*/
/*!!Warning: Huawei key information asset. No spread without permission. */
/*CODEMARK:EG4uRhTwMmgcVFBsBnYHCEm2UPcyllv4D4NOje6cFLSYglw6LvPA978sGAr3yTchgOI0M46H
HZIZCDLcNqR1rbxHHGWmLNp+CRsGfVaxSWTI2/8f/R+w4XjDnh/Xjw50z2xsARLBKfMM32Ii
FZ5sQI9ZmBxpd21rgb2PI9uz7AIDzmT0qvtY5iGyKeCHsryw27LC3TA+6yQs5rTkQOs8LZvz
hCCVt23ExetCTZWLrS2e16r7DQkl9XUvSXAsazO6/Iev7AJYR2tt8DjjMCVT6Q==#*/
/*--!!Warning: Deleting or modifying the preceding information is prohibited.--*/

/*************************************************

Copyright (C), 1988-2010, Huawei Tech. Co., Ltd.

File name: HwProductsPkg\Pangea\Pv660Pangea\OemNicConfigPangea\OemNicConfigPangea.c

Author: uniBIOS-g00179230 (Heyi Guo)    Date: 2014.11.17


*************************************************/


#include <Uefi.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/OemNicProtocol.h>
#include <OemNicConfig.h>
#include <Library/CpldIoLib.h>

#include <Library/I2CLib.h>

#define EEPROM_I2C_PORT 6

EFI_STATUS
EFIAPI OemGetMac2P (IN OUT EFI_MAC_ADDRESS *Mac, IN UINTN Port);

EFI_STATUS
EFIAPI OemSetMac2P (IN EFI_MAC_ADDRESS *Mac, IN UINTN Port);

EFI_STATUS OemGetMacE2prom(IN UINT32 Port, OUT UINT8 *pucAddr);
EFI_STATUS OemSetMacE2prom(IN UINT32 Port, IN UINT8 *pucAddr);

volatile unsigned char g_2pserveraddr[4][6] = 
{
    {0x00, 0x18, 0x16, 0x29, 0x11, 0x00},
    {0x00, 0x18, 0x16, 0x29, 0x11, 0x01},
    {0x00, 0x18, 0x16, 0x29, 0x11, 0x02},
    {0x00, 0x18, 0x16, 0x29, 0x11, 0x03}
};

//CRC 数值
UINT16 crc_tab[256] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
    0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
    0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
    0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
    0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
    0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
    0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
    0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
    0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
    0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
    0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
    0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
    0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
    0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
    0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
    0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
    0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
    0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
    0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
    0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
    0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
    0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
    0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0,
};

UINT16 make_crc_checksum(UINT8 *buf, UINT32 len)
{
    UINT16 StartCRC = 0;

    /*避免长度错误导致复位，最长允许计算512K的数据*/
    if (len > (512 * 1024))
    {
        return 0;
    }

    if (NULL == buf)
    {
        return 0;
    }

    while (len)
    {
        StartCRC = crc_tab[((UINT8)((StartCRC >> 8) & 0xff)) ^ *(buf++)] ^ ((UINT16)(StartCRC << 8));
        len--;
    }

    return StartCRC;
}
//uniBIOS__l00228991_end   2015-7-16 9:27:16


//Description:增加盘古单板MAC地址CRC校验功能
/*******************************************************************************
 函 数 名  : OemGetMacE2prom
 功能描述  : 从EEPROM中获取mac地址
 输入参数  : Port    
             *pucAddr  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年11月24日
    作    者   : ZhuGeSheng
    修改内容   : 新生成函数

*******************************************************************************/
EFI_STATUS OemGetMacE2prom(IN UINT32 Port, OUT UINT8 *pucAddr)
{
    I2C_DEVICE stI2cDev = {0};
    EFI_STATUS Status;
    UINT16     I2cOffset;
    UINT16     crc16;
    NIC_MAC_ADDRESS stMacDesc = {0};

    Status = I2CInit(0, EEPROM_I2C_PORT, Normal);
    if (EFI_ERROR(Status))
    {
        DEBUG((EFI_D_ERROR, "[%a]:[%dL] Call I2CInit failed! p1=0x%x.\n", __FUNCTION__, __LINE__, Status));
        return Status;
    } 

    I2cOffset = I2C_OFFSET_EEPROM_ETH0 + (Port * sizeof(NIC_MAC_ADDRESS));
    
    stI2cDev.DeviceType = DEVICE_TYPE_E2PROM;
    stI2cDev.Port = EEPROM_I2C_PORT;
    stI2cDev.SlaveDeviceAddress = I2C_SLAVEADDR_EEPROM;
    stI2cDev.Socket = 0;
    Status = I2CRead(&stI2cDev, I2cOffset, sizeof(NIC_MAC_ADDRESS), (UINT8 *)&stMacDesc);
    if (EFI_ERROR(Status))
    {
        DEBUG((EFI_D_ERROR, "[%a]:[%dL] Call I2cRead failed! p1=0x%x.\n", __FUNCTION__, __LINE__, Status));
        return Status;
    } 

    //通过crc判断mac地址是否合法
    crc16 = make_crc_checksum((UINT8 *)&(stMacDesc.MacLen), sizeof(stMacDesc.MacLen) + sizeof(stMacDesc.Mac));
    if ((crc16 != stMacDesc.Crc16) || (0 == crc16))
    {
        return EFI_NOT_FOUND;
    }
    
    gBS->CopyMem((VOID *)(pucAddr), (VOID *)(stMacDesc.Mac), MAC_ADDR_LEN);


    return EFI_SUCCESS;
}
//uniBIOS_l00306713_000_end   2015-8-25 11:34:50


//Description:增加盘古单板MAC地址CRC校验功能
/*******************************************************************************
 函 数 名  : OemSetMacE2prom
 功能描述  : 向EEPROM中写入MAC地址
 输入参数  : Port    
             *pucAddr  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年11月24日
    作    者   : ZhuGeSheng
    修改内容   : 新生成函数

*******************************************************************************/
EFI_STATUS OemSetMacE2prom(IN UINT32 Port, IN UINT8 *pucAddr)
{
    I2C_DEVICE stI2cDev = {0};
    EFI_STATUS Status;
    UINT16     I2cOffset;
    NIC_MAC_ADDRESS stMacDesc = {0};


    stMacDesc.MacLen = MAC_ADDR_LEN;
    gBS->CopyMem((VOID *)(stMacDesc.Mac), (VOID *)pucAddr, MAC_ADDR_LEN);

    //计算长度和mac地址两部分的crc,
    stMacDesc.Crc16  = make_crc_checksum((UINT8 *)&(stMacDesc.MacLen), sizeof(stMacDesc.MacLen) + MAC_ADDR_LEN);

    Status = I2CInit(0, EEPROM_I2C_PORT, Normal);
    if (EFI_ERROR(Status))
    {
        DEBUG((EFI_D_ERROR, "[%a]:[%dL] Call I2CInit failed! p1=0x%x.\n", __FUNCTION__, __LINE__, Status));
        return Status;
    } 

    I2cOffset = I2C_OFFSET_EEPROM_ETH0 + (Port * sizeof(NIC_MAC_ADDRESS));
    
    stI2cDev.DeviceType = DEVICE_TYPE_E2PROM;
    stI2cDev.Port = EEPROM_I2C_PORT;
    stI2cDev.SlaveDeviceAddress = I2C_SLAVEADDR_EEPROM;
    stI2cDev.Socket = 0;
    Status = I2CWrite(&stI2cDev, I2cOffset, sizeof(NIC_MAC_ADDRESS), (UINT8 *)&stMacDesc);
    if (EFI_ERROR(Status))
    {
        DEBUG((EFI_D_ERROR, "[%a]:[%dL] Call I2cWrite failed! p1=0x%x.\n", __FUNCTION__, __LINE__, Status));
        return Status;
    } 
    return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI OemGetMac2P (
  IN OUT EFI_MAC_ADDRESS *Mac,
  IN UINTN Port
  )
{
    EFI_STATUS Status;
    
    if (NULL == Mac)
    {
      DEBUG((EFI_D_ERROR, "[%a]:[%dL] Mac buffer is null!\n", __FUNCTION__, __LINE__));
      return EFI_INVALID_PARAMETER;
    }
    
    //TODO: discard port number, only support one port
    // Only 6 bytes are used
    //uniBIOS_z00304930_000_start 2014-11-29 11:56:17
    
     //Description:pv660_pangea
    Status = OemGetMacE2prom(Port, Mac->Addr);
    if ((EFI_ERROR(Status)))
    {
        DEBUG((EFI_D_ERROR, "[%a]:[%dL] Get mac failed!\n", __FUNCTION__, __LINE__));
        
        //Description:Lemon获取Mac地址失败时，返回固定MAC地址
        Mac->Addr[0] = 0x00;
        Mac->Addr[1] = 0x18;
        Mac->Addr[2] = 0x82;
        Mac->Addr[3] = 0x2F;
        Mac->Addr[4] = 0x02;
        Mac->Addr[5] = Port;
        //uniBIOS__l00228991_end   2015-12-14 15:36:02
        return Status;
    }
    //uniBIOS_z00304930_000_end   2014-11-29 11:56:17
    
    return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI OemSetMac2P (
  IN EFI_MAC_ADDRESS *Mac,
  IN UINTN Port
  )
{
  EFI_STATUS Status;
  
  if (NULL == Mac)
  {
    DEBUG((EFI_D_ERROR, "[%a]:[%dL] Mac buffer is null!\n", __FUNCTION__, __LINE__));
    return EFI_INVALID_PARAMETER;
  }
  
  //uniBIOS_z00304930_000_start 2014-11-29 11:59:26
   
   //Description:pv660_pangea
  Status = OemSetMacE2prom(Port, Mac->Addr);
  if ((EFI_ERROR(Status)))
  {
      DEBUG((EFI_D_ERROR, "[%a]:[%dL] Set mac failed!\n", __FUNCTION__, __LINE__));
      return Status;
  }
  //uniBIOS_z00304930_000_end   2014-11-29 11:59:26
  
  return EFI_SUCCESS;
}

OEM_NIC_PROTOCOL mOemNicProtocol2P = {
  .GetMac = OemGetMac2P,
  .SetMac = OemSetMac2P,
};

VOID OemFeedbackXGeStatus(BOOLEAN IsLinkup, BOOLEAN IsActOK, UINT32 port)
{
    UINT8 CpldValue = 0;
    UINTN RegOffset = 0x10 + (UINTN)port * 4;
    
    if (port > 2)   //当前只支持port0、1
    {
        return;
    }
    
    if (IsLinkup)
    {
        CpldValue = ReadCpldReg(RegOffset);
        CpldValue |= BIT2;
        WriteCpldReg(RegOffset, CpldValue); 
    }
    else
    {
        CpldValue = ReadCpldReg(RegOffset);
        CpldValue &= ~((UINT8)BIT2);
        WriteCpldReg(RegOffset, CpldValue); 
    }

    if (IsActOK)
    {
        CpldValue = ReadCpldReg(RegOffset);
        CpldValue |= BIT4;
        WriteCpldReg(RegOffset, CpldValue); 
    }
    else
    {
        CpldValue = ReadCpldReg(RegOffset);
        CpldValue &= ~((UINT8)BIT4);
        WriteCpldReg(RegOffset, CpldValue); 
    }
}

OEM_XGE_STATUS_PROTOCOL mOemXgeStatusProtocol2p = {
  .FeedbackXgeStatus = OemFeedbackXGeStatus,
};


EFI_STATUS
EFIAPI
OemNicConfigEntry (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS Status;

  Status = gBS->InstallProtocolInterface(
    &ImageHandle,
    &gOemNicProtocolGuid,
    EFI_NATIVE_INTERFACE,
    &mOemNicProtocol2P
    );

  if(EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR, "[%a]:[%dL] Install Protocol failed %r\n", __FUNCTION__, __LINE__, Status));
    return Status;
  }

  Status = gBS->InstallProtocolInterface(
    &ImageHandle,
    &gOemXgeStatusProtocolGuid,
    EFI_NATIVE_INTERFACE,
    &mOemXgeStatusProtocol2p
    );

  if(EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR, "[%a]:[%dL] Install Protocol failed %r\n", __FUNCTION__, __LINE__, Status));
    return Status;
  }

  return EFI_SUCCESS;
}
//uniBIOS__l00228991_end   2015-12-1 15:41:59

