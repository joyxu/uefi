/*-----------------------------------------------------------------------*/
/*!!Warning: Huawei key information asset. No spread without permission. */
/*CODEMARK:EG4uRhTwMmgcVFBsBnYHCEm2UPcyllv4D4NOje6cFLSYglw6LvPA978sGAr3yTchgOI0M46H
HZIZCDLcNqR1rbxHHGWmLNp+CRsGfVaxSWSrseVn/+wcdCo+wKA6WCQWkJcEHPNQYTvBaY4Z
0I9NvndOLnTjhWN+I7pzTCElbHAYg6rUfFI4Kei+8cKrLuKzTPRC2uz3hII/v1CB8RTYun9L
3I+RLNUi9qgOg8kbb4f/zE9zWGcXd2CBBjDJeG71qa4NUIX+ix1CvCHcYtfuGQ==#*/
/*--!!Warning: Deleting or modifying the preceding information is prohibited.--*/


/******************************************************************************

                  版权所有 (C), 2012-2016, 华为技术有限公司

 ******************************************************************************
  文 件 名   : 
  版 本 号   : 
  作    者   : 
  生成日期   : 
  最近修改   :
  功能描述   : 
  函数列表   :
  修改历史   :
  1.日    期   : 
    作    者   : 
    修改内容   : 

//DTS2015081106432 w00216241 20150813 【iWare uniBIOS V200R001C00B990 Pangea LLT测试】按照安全能力中心要求对代码中存在的敏感词汇进行整改
//DTS2015110700120 1610 PCIe 代码上库
//DTS2015110700120 1610 PCIe 代码上库 去掉调试时的打印
******************************************************************************/
#include "PcieInitLib.h"
#include <Library/DebugLib.h>
#include <Library/ArmLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/
//uniBIOS__l00228991_start DTS2015112607868 2015-12-4 15:15:47
//Description:修改从片PCIE子系统地址为0xB0000000
static PCIE_INIT_CFG mPcieIntCfg;
UINT64 pcie_subctrl_base[2] = {0xb0000000, BASE_4TB + 0xb0000000};
UINT64 pcie_subctrl_base_1610[2] = {0xa0000000, 0xb0000000};
UINT64 io_sub0_base = 0xa0000000;
UINT64 PCIE_APB_SLVAE_BASE[2] = {0xb0070000, BASE_4TB + 0xb0070000};
#define PCIE_REG_BASE(HostBridgeNum,port)              (PCIE_APB_SLVAE_BASE[HostBridgeNum] + (UINT32)(port * 0x10000))
UINT64 PCIE_APB_SLVAE_BASE_1610[2][4] = {{0xa0090000, 0xa0200000, 0xa00a0000, 0xa00b0000},
                                         {0xb0090000, 0xb0200000, 0xb00a0000, 0xb00b0000}};
UINT64 PCIE_PHY_BASE_1610[2][4] = {{0xa00c0000, 0xa00d0000, 0xa00e0000, 0xa00f0000},
                                    {0xb00c0000,0xb00d0000, 0xb00e0000, 0xb00f0000}};
UINT32 loop_test_flag[4] = {0,0,0,0};
UINT64 pcie_dma_des_base = PCIE_ADDR_BASE_HOST_ADDR;
//uniBIOS__l00228991_end   2015-12-4 15:15:47

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

extern PCIE_DRIVER_CFG gastr_pcie_driver_cfg;
extern PCIE_IATU gastr_pcie_iatu_cfg;
extern PCIE_IATU_VA mPcieIatuTable;

/*****************************************************************************
 函 数 名  : PcieRegWrite
 功能描述  : pcie寄存器读写（apb口）
 输入参数  : UINT32 Port
             UINTN Offset
             UINT32 value
 输出参数  : 无
 返 回 值  : static VOID
*****************************************************************************/
VOID PcieRegWrite(UINT32 Port, UINTN Offset, UINT32 Value)
{
    RegWrite((UINT64)mPcieIntCfg.RegResource[Port] + Offset, Value);
    //DEBUG((EFI_D_ERROR,"Write Port %d: 0x%lx Value is 0x%lx:\n",Port, mPcieIntCfg.RegResource[Port], Value));
   
}

/*****************************************************************************
 函 数 名  : PcieRegRead
 功能描述  : pcie 读寄存器（apb口）
 输入参数  : UINT32 Port
             UINTN Offset
 输出参数  : 无
 返 回 值  : UINT32
*****************************************************************************/
UINT32 PcieRegRead(UINT32 Port, UINTN Offset)
{
    UINT32 Value = 0;
   
    RegRead((UINT64)mPcieIntCfg.RegResource[Port] + Offset, Value);
    //DEBUG((EFI_D_ERROR,"Read Port %d: 0x%lx Value is 0x%lx:\n",Port, mPcieIntCfg.RegResource[Port], Value));
    return Value;
}

/*****************************************************************************
 函 数 名  : PcieMmioWrite
 功能描述  : pcie寄存器读写（axi口）
 输入参数  : UINT32 Port
             UINT32 Offset
             UINT32 value
 输出参数  : 无
 返 回 值  : static VOID
*****************************************************************************/
VOID PcieMmioWrite(UINT32 Port, UINTN Offset0, UINTN Offset1, UINT32 Value)
{
    RegWrite((UINT64)mPcieIntCfg.CfgResource[Port] + Offset0 + Offset1, Value);
}

/*****************************************************************************
 函 数 名  : PcieMmioRead
 功能描述  : pcie 读寄存器（axi口）
 输入参数  : UINT32 Port
             UINT32 Offset
 输出参数  : 无
 返 回 值  : UINT32
*****************************************************************************/
UINT32 PcieMmioRead(UINT32 Port, UINTN Offset0, UINTN Offset1)
{
    UINT32 Value = 0;
    RegRead((UINT64)mPcieIntCfg.CfgResource[Port] + Offset0 + Offset1, Value);
    return Value;
}

/*****************************************************************************
 函 数 名  : PcieChangeRwMode
 功能描述  : 读写模式改变
 输入参数  : UINT32 Port
             pcie_rw_mode_e mode
 输出参数  : 无
 返 回 值  : static UINT32
*****************************************************************************/
VOID PcieChangeRwMode(UINT32 HostBridgeNum, UINT32 Port, PCIE_RW_MODE Mode)
{
    u_sc_pcie0_clkreq pcie0;
    u_sc_pcie1_clkreq pcie1;
    u_sc_pcie2_clkreq pcie2;
    u_sc_pcie3_clkreq pcie3;

    switch(Port)
    {
        case 0:
            RegRead(pcie_subctrl_base[HostBridgeNum] + PCIE_SUBCTRL_SC_PCIE0_CLKREQ_REG, pcie0.UInt32);
            pcie0.Bits.pcie0_apb_cfg_sel = Mode;
            RegWrite(pcie_subctrl_base[HostBridgeNum] + PCIE_SUBCTRL_SC_PCIE0_CLKREQ_REG, pcie0.UInt32);
            break;
        case 1:
            RegRead(pcie_subctrl_base[HostBridgeNum] + PCIE_SUBCTRL_SC_PCIE1_CLKREQ_REG, pcie1.UInt32);
            pcie1.Bits.pcie1_apb_cfg_sel = Mode;
            RegWrite(pcie_subctrl_base[HostBridgeNum] + PCIE_SUBCTRL_SC_PCIE1_CLKREQ_REG, pcie1.UInt32);
            break;
        case 2:
            RegRead(pcie_subctrl_base[HostBridgeNum] + PCIE_SUBCTRL_SC_PCIE2_CLKREQ_REG, pcie2.UInt32);
            pcie2.Bits.pcie2_apb_cfg_sel = Mode;
            RegWrite(pcie_subctrl_base[HostBridgeNum] + PCIE_SUBCTRL_SC_PCIE2_CLKREQ_REG, pcie2.UInt32);
            break;
        case 3:
            RegRead(pcie_subctrl_base[HostBridgeNum] + PCIE_SUBCTRL_SC_PCIE3_CLKREQ_REG, pcie3.UInt32);
            pcie3.Bits.pcie3_apb_cfg_sel = Mode;
            RegWrite(pcie_subctrl_base[HostBridgeNum] + PCIE_SUBCTRL_SC_PCIE3_CLKREQ_REG, pcie3.UInt32);
            break;
        default:
            break;
    }
}



/*****************************************************************************
 函 数 名  : PcieEnableItssm
 功能描述  : 使能ltssm状态机
 输入参数  : UINT32 Port
 输出参数  : 无
 返 回 值  : UINT32
*****************************************************************************/
EFI_STATUS PcieEnableItssm(UINT32 soctype, UINT32 HostBridgeNum, UINT32 Port)
{
    PCIE_CTRL_7_U pcie_ctrl7;
    UINT32 Value = 0;

    if(Port >= PCIE_MAX_PORT_NUM)
    {
        return EFI_INVALID_PARAMETER;
    }

    if (0x1610 == soctype)
    {
        RegRead(PCIE_APB_SLVAE_BASE_1610[HostBridgeNum][Port] + 0x1114, Value);
        Value |= BIT11|BIT30|BIT31;
        RegWrite(PCIE_APB_SLVAE_BASE_1610[HostBridgeNum][Port] + 0x1114, Value);
        return EFI_SUCCESS;
    }
    else
    {
        PcieChangeRwMode(HostBridgeNum, Port, PCIE_SYS_CONTROL);

        pcie_ctrl7.UInt32 = PcieRegRead(Port, PCIE_CTRL_7_REG);
        pcie_ctrl7.Bits.pcie_linkdown_auto_rstn_enable = 0x1; //自动复位 关闭，防止自动复位挂死
        pcie_ctrl7.Bits.pcie2_app_ltssm_enable = 0x1;
        PcieRegWrite(Port, PCIE_CTRL_7_REG, pcie_ctrl7.UInt32);

        PcieChangeRwMode(HostBridgeNum, Port, PCIE_CONFIG_REG);

        return EFI_SUCCESS;
    }
    
}

/*****************************************************************************
 函 数 名  : PcieDisableItssm
 功能描述  : 使能ltssm状态机
 输入参数  : UINT32 Port
 输出参数  : 无
 返 回 值  : UINT32
*****************************************************************************/
EFI_STATUS PcieDisableItssm(UINT32 soctype, UINT32 HostBridgeNum, UINT32 Port)
{
    PCIE_CTRL_7_U pcie_ctrl7;
    UINT32 Value = 0;

    if(Port >= PCIE_MAX_PORT_NUM)
    {
        return PCIE_ERR_PARAM_INVALID;
    }

    if (0x1610 == soctype)
    {
        RegRead(PCIE_APB_SLVAE_BASE_1610[HostBridgeNum][Port] + 0x1114, Value);
        Value &= ~(BIT11);
        RegWrite(PCIE_APB_SLVAE_BASE_1610[HostBridgeNum][Port] + 0x1114, Value);
        return EFI_SUCCESS;
    }
    else
    {
        PcieChangeRwMode(HostBridgeNum, Port, PCIE_SYS_CONTROL);

        pcie_ctrl7.UInt32 = PcieRegRead(Port, PCIE_CTRL_7_REG);
        pcie_ctrl7.Bits.pcie2_app_ltssm_enable = 0x0;
        PcieRegWrite(Port,PCIE_CTRL_7_REG, pcie_ctrl7.UInt32);

        PcieChangeRwMode(HostBridgeNum, Port, PCIE_CONFIG_REG);

        return EFI_SUCCESS;
    }
    
}


/*****************************************************************************
 函 数 名  : PcieLinkSpeedSet
 功能描述  : 设置PCIE的link速率,GEN1.0,GEN2.0或GEN3.0
 输入参数  : UINT32 Port
             pcie_port_gen_e speed
 输出参数  : 无
 返 回 值  : UINT32
*****************************************************************************/
EFI_STATUS PcieLinkSpeedSet(UINT32 Port,PCIE_PORT_GEN Speed)
{
    PCIE_EP_PCIE_CAP12_U pcie_cap12;

    if(Port >= PCIE_MAX_PORT_NUM)
    {
        return EFI_INVALID_PARAMETER;
    }
    
    pcie_cap12.UInt32 = PcieRegRead(Port, PCIE_EP_PCIE_CAP12_REG);
    pcie_cap12.Bits.targetlinkspeed = Speed;
    PcieRegWrite(Port, PCIE_EP_PCIE_CAP12_REG, pcie_cap12.UInt32);

    /*如果是ntb模式的话 ，还需要配置eep*/
    if(mPcieIntCfg.Dev[Port].PcieDevice.PortInfo.PortType == PCIE_NTB_TO_NTB ||
        mPcieIntCfg.Dev[Port].PcieDevice.PortInfo.PortType == PCIE_NTB_TO_RP)
    {
        pcie_cap12.UInt32 = PcieMmioRead(Port, PCIE_MMIO_EEP_CFG, PCIE_EP_PCIE_CAP12_REG);
        pcie_cap12.Bits.targetlinkspeed = Speed;
        PcieMmioWrite(Port, PCIE_MMIO_EEP_CFG, PCIE_EP_PCIE_CAP12_REG, pcie_cap12.UInt32);
    }
    return EFI_SUCCESS;
}

/*****************************************************************************
 函 数 名  : PcieLinkWidthSet
 功能描述  : 宽度设置
 输入参数  : UINT32 Port
             PCIE_PORT_WIDTH Width
 输出参数  : 无
 返 回 值  : UINT32
*****************************************************************************/
EFI_STATUS PcieLinkWidthSet(UINT32 Port, PCIE_PORT_WIDTH Width)
{
    PCIE_EP_PORT_LOGIC4_U pcie_logic4;
    PCIE_EP_PORT_LOGIC22_U logic22;

    if(Port >= PCIE_MAX_PORT_NUM)
    {
        return PCIE_ERR_PARAM_INVALID;
    }

    pcie_logic4.UInt32 = PcieRegRead(Port, PCIE_EP_PORT_LOGIC4_REG);
    pcie_logic4.Bits.linkmodeenable = Width;
    pcie_logic4.Bits.crosslinkenable = 0;
    pcie_logic4.Bits.fastlinkmode = 1;
    PcieRegWrite(Port, PCIE_EP_PORT_LOGIC4_REG, pcie_logic4.UInt32);

    logic22.UInt32 = PcieRegRead(Port, PCIE_EP_PORT_LOGIC22_REG);
    logic22.Bits.n_fts = 0xff;//推荐配置
    if(Width == PCIE_WITDH_X1)
    {
        logic22.Bits.pre_determ_num_of_lane = 1;
    }
    else if(Width == PCIE_WITDH_X2)
    {
        logic22.Bits.pre_determ_num_of_lane = 2;
    }
    else
    {
        logic22.Bits.pre_determ_num_of_lane = 3;
    }
    PcieRegWrite(Port, PCIE_EP_PORT_LOGIC22_REG, logic22.UInt32);

    /*如果是ntb模式的话 ，还需要配置eep*/
    if(mPcieIntCfg.Dev[Port].PcieDevice.PortInfo.PortType == PCIE_NTB_TO_NTB ||
        mPcieIntCfg.Dev[Port].PcieDevice.PortInfo.PortType == PCIE_NTB_TO_RP)
    {
        pcie_logic4.UInt32 = PcieMmioRead(Port, PCIE_MMIO_EEP_CFG, PCIE_EP_PORT_LOGIC4_REG);
        pcie_logic4.Bits.linkmodeenable = Width;
        pcie_logic4.Bits.crosslinkenable = 0;
        pcie_logic4.Bits.fastlinkmode = 1;
        PcieMmioWrite(Port, PCIE_MMIO_EEP_CFG, PCIE_EP_PORT_LOGIC4_REG, pcie_logic4.UInt32);

        logic22.UInt32 = PcieMmioRead(Port, PCIE_MMIO_EEP_CFG, PCIE_EP_PORT_LOGIC22_REG);
        logic22.Bits.n_fts = 0xff;//推荐配置
        if(Width == PCIE_WITDH_X1)
        {
            logic22.Bits.pre_determ_num_of_lane = 1;
        }
        else if(Width == PCIE_WITDH_X2)
        {
            logic22.Bits.pre_determ_num_of_lane = 2;
        }
        else
        {
            logic22.Bits.pre_determ_num_of_lane = 3;
        }
        PcieMmioWrite(Port,PCIE_MMIO_EEP_CFG, PCIE_EP_PORT_LOGIC22_REG, logic22.UInt32);
    }
    return EFI_SUCCESS;
}

/*****************************************************************************
 函 数 名  : PcieSetupRC
 功能描述  : 配置Pcie RC
 输入参数  : UINT32 Port
             Lan、Width
 输出参数  : 无
 返 回 值  : UINT32
*****************************************************************************/
EFI_STATUS PcieSetupRC(UINT32 Port, PCIE_PORT_WIDTH Width)
{
    //PCIE_EP_PORT_LOGIC4_U pcie_logic4;
    PCIE_EP_PORT_LOGIC22_U logic22;
    PCIE_EEP_PCI_CFG_HDR15_U hdr15;
    UINT32 Value = 0;
    if(Port >= PCIE_MAX_PORT_NUM)
    {
        return EFI_INVALID_PARAMETER;
    }
    
    Value = PcieRegRead(Port, PCIE_EP_PORT_LOGIC4_REG);        
    Value &= ~(0x3f<<16);
    
    if(Width == PCIE_WITDH_X1)
    {
        Value |= (0x1 << 16);
    }
    else if(Width == PCIE_WITDH_X2)
    {
        Value |= (0x3 << 16);
    }
    else if(Width == PCIE_WITDH_X4)
    {
        Value |= (0x7 << 16);
    }
    else if(Width == PCIE_WITDH_X8)
    {
        Value |= (0xf << 16);
    }
    else
    {
        DEBUG((EFI_D_ERROR,"Width is not invalid\n")); 
    }
    
    PcieRegWrite(Port, PCIE_EP_PORT_LOGIC4_REG, Value);

    logic22.UInt32 = PcieRegRead(Port, PCIE_EP_PORT_LOGIC22_REG);
    //logic22.UInt32 &= ~(0x1ff<<8);
    
    if(Width == PCIE_WITDH_X1)
    {
        logic22.Bits.pre_determ_num_of_lane = 1;
    }
    else if(Width == PCIE_WITDH_X2)
    {
        logic22.Bits.pre_determ_num_of_lane = 2;
    }
    else if(Width == PCIE_WITDH_X4)
    {
        logic22.Bits.pre_determ_num_of_lane = 4;
    }
    else if(Width == PCIE_WITDH_X8)
    {
        logic22.Bits.pre_determ_num_of_lane = 8;
    }
    else
    {
        DEBUG((EFI_D_ERROR,"Width is not invalid\n")); 
    }
	//DTS2015111710758 PCIe建链保持和内核一致
    logic22.UInt32 |= (0x100<<8);
    logic22.UInt32 |= (0x1<<17);
    PcieRegWrite(Port, PCIE_EP_PORT_LOGIC22_REG, logic22.UInt32);
    
    /* setup RC BARs */
    PcieRegWrite(Port, PCIE_EP_PCI_CFG_HDR4_REG, 0x00000004);
    PcieRegWrite(Port, PCIE_EP_PCI_CFG_HDR5_REG, 0x00000000);

    /* setup interrupt pins */
    hdr15.UInt32 = PcieRegRead(Port, PCIE_EP_PCI_CFG_HDR15_REG);
    hdr15.UInt32 &= 0xffff00ff;
    hdr15.UInt32 |= 0x00000100;
    PcieRegWrite(Port, PCIE_EP_PCI_CFG_HDR15_REG, hdr15.UInt32);

    /* setup bus numbers */
    Value = PcieRegRead(Port, PCIE_EP_PCI_CFG_HDR6_REG);
    Value &= 0xff000000;
    Value |= 0x00010100;
    PcieRegWrite(Port, PCIE_EP_PCI_CFG_HDR6_REG, Value);

    /* setup command register */
    Value = PcieRegRead(Port, PCIE_EP_PCI_CFG_HDR1_REG);
    Value &= 0xffff0000;
    Value |= 0x1|0x2|0x4|0x100;
    PcieRegWrite(Port, PCIE_EP_PCI_CFG_HDR1_REG, Value);

    /*如果是ntb模式的话 ，还需要配置eep*/
    /*
    if(mPcieIntCfg.Dev[Port].PcieDevice.PortInfo.PortType == PCIE_NTB_TO_NTB ||
        mPcieIntCfg.Dev[Port].PcieDevice.PortInfo.PortType == PCIE_NTB_TO_RP)
    {
        pcie_logic4.UInt32 = PcieMmioRead(Port, PCIE_MMIO_EEP_CFG, PCIE_EP_PORT_LOGIC4_REG);
        pcie_logic4.Bits.linkmodeenable = Width;
        pcie_logic4.Bits.crosslinkenable = 0;
        pcie_logic4.Bits.fastlinkmode = 1;
        PcieMmioWrite(Port, PCIE_MMIO_EEP_CFG, PCIE_EP_PORT_LOGIC4_REG, pcie_logic4.UInt32);

        logic22.UInt32 = PcieMmioRead(Port, PCIE_MMIO_EEP_CFG, PCIE_EP_PORT_LOGIC22_REG);
        logic22.Bits.n_fts = 0xff;//推荐配置
        if(Width == PCIE_WITDH_X1)
        {
            logic22.Bits.pre_determ_num_of_lane = 1;
        }
        else if(Width == PCIE_WITDH_X2)
        {
            logic22.Bits.pre_determ_num_of_lane = 2;
        }
        else
        {
            logic22.Bits.pre_determ_num_of_lane = 3;
        }
        PcieMmioWrite(Port,PCIE_MMIO_EEP_CFG, PCIE_EP_PORT_LOGIC22_REG, logic22.UInt32);
    }
    */
    return EFI_SUCCESS;
}


/*****************************************************************************
 函 数 名  : PcieModeSet
 功能描述  : pcie模式设置
 输入参数  : UINT32 Port
             PCIE_END_POINT mode
 输出参数  : 无
 返 回 值  : static UINT32
*****************************************************************************/
EFI_STATUS PcieModeSet(UINT32 soctype, UINT32 HostBridgeNum, UINT32 Port, PCIE_PORT_TYPE PcieType)
{
    PCIE_CTRL_0_U str_pcie_ctrl_0;
    //UINT32 Value = 0;

    if(Port >= PCIE_MAX_PORT_NUM)
    {
        return EFI_INVALID_PARAMETER;
    }

    if (0x1610 == soctype)
    {
        RegWrite(PCIE_APB_SLVAE_BASE_1610[HostBridgeNum][Port] + 0x1000 + 0xf8, 0x4 << 28);
        //DEBUG((EFI_D_ERROR,"mfill 0x%lx 0x%lx\n",PCIE_APB_SLVAE_BASE_1610[HostBridgeNum][Port] + 0x1000 + 0xf8,0x4 << 28));
		//DTS2015111710758 关掉smmu bypass
        //RegRead(PCIE_APB_SLVAE_BASE_1610[HostBridgeNum][Port] + 0x1000 + 0x4, Value);
        //Value |= BIT10 | BIT12 | BIT13;
        //RegWrite(PCIE_APB_SLVAE_BASE_1610[HostBridgeNum][Port] + 0x1000 + 0x4, Value);
        // DEBUG((EFI_D_ERROR,"mfill 0x%lx 0x%lx\n",PCIE_APB_SLVAE_BASE_1610[HostBridgeNum][Port] + 0x1000 + 0x4,Value));
    }
    else 
    {
        PcieChangeRwMode(HostBridgeNum, Port, PCIE_SYS_CONTROL);
        
        str_pcie_ctrl_0.UInt32 = PcieRegRead(Port, PCIE_CTRL_0_REG);
        if(PcieType == PCIE_END_POINT)
        {
            str_pcie_ctrl_0.Bits.pcie2_slv_device_type = PCIE_EP_DEVICE;
        }
        else
        {
            str_pcie_ctrl_0.Bits.pcie2_slv_device_type = RP_OF_PCIE_RC;
        }
        PcieRegWrite(Port, PCIE_CTRL_0_REG, str_pcie_ctrl_0.UInt32);
        
        PcieChangeRwMode(HostBridgeNum, Port, PCIE_CONFIG_REG);
    }
    return EFI_SUCCESS;
}

/*****************************************************************************
 函 数 名  : PciePcsInit
 功能描述  : 对接的时候pcs初始化
 输入参数  : UINT32 Port
 输出参数  : 无
 返 回 值  : VOID
*****************************************************************************/
VOID PciePcsInit(UINT32 soctype, UINT32 HostBridgeNum, UINT32 Port)
{
    UINT8 i = 0;
    UINT32 Value = 0;
    if (0x1610 == soctype)
    {
        if (Port <= 2)
        {
            for (i = 0; i < 8; i++)
            {
                RegRead(PCIE_PHY_BASE_1610[HostBridgeNum][Port] + 0x204 + i*0x4, Value);
                Value |= (1 << 20);
                RegWrite(PCIE_PHY_BASE_1610[HostBridgeNum][Port] + 0x204 + i*0x4, Value);
                //DEBUG((EFI_D_ERROR, "PCS Init Value is: 0x%x\n",Value));
            }
            RegWrite(PCIE_PHY_BASE_1610[HostBridgeNum][Port] + 0x264, 0x4e20);
        }
        else
        {
            for (i = 0; i < 4; i++)
            {
                RegRead(PCIE_PHY_BASE_1610[HostBridgeNum][Port] + 0x204 + i*0x4, Value);
                Value |= (1 << 20);
                RegWrite(PCIE_PHY_BASE_1610[HostBridgeNum][Port] + 0x204 + i*0x4, Value);
            }
            RegWrite(PCIE_PHY_BASE_1610[HostBridgeNum][Port] + 0x264, 0x4e20);
        }
    }
    else
    {
        if(Port<=2)
        {
            RegWrite(pcie_subctrl_base[HostBridgeNum] + 0xc0000 + (UINT32)(Port * 0x10000) + 0x8020, 0x2026044);
            RegWrite(pcie_subctrl_base[HostBridgeNum] + 0xc0000 + (UINT32)(Port * 0x10000) + 0x8060, 0x2126044);
            RegWrite(pcie_subctrl_base[HostBridgeNum] + 0xc0000 + (UINT32)(Port * 0x10000) + 0x80c4, 0x2126044);
            RegWrite(pcie_subctrl_base[HostBridgeNum] + 0xc0000 + (UINT32)(Port * 0x10000) + 0x80e4, 0x2026044);

            RegWrite(pcie_subctrl_base[HostBridgeNum] + 0xc0000 + (UINT32)(Port * 0x10000) + 0x80a0, 0x4018);
            RegWrite(pcie_subctrl_base[HostBridgeNum] + 0xc0000 + (UINT32)(Port * 0x10000) + 0x80a4, 0x804018);
            RegWrite(pcie_subctrl_base[HostBridgeNum] + 0xc0000 + (UINT32)(Port * 0x10000) + 0x80c0, 0x11201100);
            RegWrite(pcie_subctrl_base[HostBridgeNum] + 0xc0000 + (UINT32)(Port * 0x10000) + 0x15c, 0x3);
            RegWrite(pcie_subctrl_base[HostBridgeNum] + 0xc0000 + (UINT32)(Port * 0x10000) + 0x158, 0);
        }
        else
        
        {
            RegWrite(pcie_subctrl_base[HostBridgeNum] + 0xc0000 + (UINT32)(Port * 0x10000) + 0x74, 0x46e000);
            RegWrite(pcie_subctrl_base[HostBridgeNum] + 0xc0000 + (UINT32)(Port * 0x10000) + 0x78, 0x46e000);
            RegWrite(pcie_subctrl_base[HostBridgeNum] + 0xc0000 + (UINT32)(Port * 0x10000) + 0x7c, 0x46e000);
            RegWrite(pcie_subctrl_base[HostBridgeNum] + 0xc0000 + (UINT32)(Port * 0x10000) + 0x80, 0x46e000);
            RegWrite(pcie_subctrl_base[HostBridgeNum] + 0xc0000 + (UINT32)(Port * 0x10000) + 0x84, 0x46e000);
            RegWrite(pcie_subctrl_base[HostBridgeNum] + 0xc0000 + (UINT32)(Port * 0x10000) + 0x88, 0x46e000);
            RegWrite(pcie_subctrl_base[HostBridgeNum] + 0xc0000 + (UINT32)(Port * 0x10000) + 0x8c, 0x46e000);
            RegWrite(pcie_subctrl_base[HostBridgeNum] + 0xc0000 + (UINT32)(Port * 0x10000) + 0x90, 0x46e000);

            RegWrite(pcie_subctrl_base[HostBridgeNum] + 0xc0000 + (UINT32)(Port * 0x10000) + 0x34, 0x1001);
            RegWrite(pcie_subctrl_base[HostBridgeNum] + 0xc0000 + (UINT32)(Port * 0x10000) + 0x38, 0x1001);
            RegWrite(pcie_subctrl_base[HostBridgeNum] + 0xc0000 + (UINT32)(Port * 0x10000) + 0x3c, 0x1001);
            RegWrite(pcie_subctrl_base[HostBridgeNum] + 0xc0000 + (UINT32)(Port * 0x10000) + 0x40, 0x1001);
            RegWrite(pcie_subctrl_base[HostBridgeNum] + 0xc0000 + (UINT32)(Port * 0x10000) + 0x44, 0x1001);
            RegWrite(pcie_subctrl_base[HostBridgeNum] + 0xc0000 + (UINT32)(Port * 0x10000) + 0x48, 0x1001);
            RegWrite(pcie_subctrl_base[HostBridgeNum] + 0xc0000 + (UINT32)(Port * 0x10000) + 0x4c, 0x1001);
            RegWrite(pcie_subctrl_base[HostBridgeNum] + 0xc0000 + (UINT32)(Port * 0x10000) + 0x50, 0x1001);

            RegWrite(pcie_subctrl_base[HostBridgeNum] + 0xc0000 + (UINT32)(Port * 0x10000) + 0xe4, 0xffff);
        }
    }
    return;
}

/*****************************************************************************
 函 数 名  : PcieEqualization
 功能描述  : Pcie均衡操作
 输入参数  : UINT32 Port
 输出参数  : 无
 返 回 值  : VOID
*****************************************************************************/
VOID PcieEqualization(UINT32 soctype, UINT32 HostBridgeNum, UINT32 Port)
{
    UINT32  Value;
    
    if (0x1610 == soctype)
    {
       PcieRegWrite(Port, 0x890, 0x1c00); 
    }
    else
    PcieRegWrite(Port, 0x890, 0x1400);
    PcieRegWrite(Port, 0x894, 0xfd7);
	//DTS2015111710758 PCIe建链保持和内核一致
    //Value = PcieRegRead(Port, 0x80);
    //Value |= 0x80;
    //PcieRegWrite(Port, 0x80, Value);

    PcieRegWrite(Port, 0x89c, 0x0);
    PcieRegWrite(Port, 0x898, 0xfc00);
    PcieRegWrite(Port, 0x89c, 0x1);
    PcieRegWrite(Port, 0x898, 0xbd00);
    PcieRegWrite(Port, 0x89c, 0x2);
    PcieRegWrite(Port, 0x898, 0xccc0);
    PcieRegWrite(Port, 0x89c, 0x3);
    PcieRegWrite(Port, 0x898, 0x8dc0);
    PcieRegWrite(Port, 0x89c, 0x4);
    PcieRegWrite(Port, 0x898, 0xfc0);
    PcieRegWrite(Port, 0x89c, 0x5);
    PcieRegWrite(Port, 0x898, 0xe46);
    PcieRegWrite(Port, 0x89c, 0x6);
    PcieRegWrite(Port, 0x898, 0xdc8);
    PcieRegWrite(Port, 0x89c, 0x7);
    PcieRegWrite(Port, 0x898, 0xcb46);
    PcieRegWrite(Port, 0x89c, 0x8);
    PcieRegWrite(Port, 0x898, 0x8c07);
    PcieRegWrite(Port, 0x89c, 0x9);
    PcieRegWrite(Port, 0x898, 0xd0b);
    PcieRegWrite(Port, 0x8a8, 0x103ff21);
    if (0x1610 == soctype)
    {
        PcieRegWrite(Port, 0x164, 0x44444444);
        PcieRegWrite(Port, 0x168, 0x44444444);
        PcieRegWrite(Port, 0x16c, 0x44444444);
        PcieRegWrite(Port, 0x170, 0x44444444);
        RegRead(PCIE_APB_SLVAE_BASE_1610[HostBridgeNum][Port] + 0x1000 + 0x2d0, Value);
        Value &= (~0x3f);
        Value |= 0x5;
        RegWrite(PCIE_APB_SLVAE_BASE_1610[HostBridgeNum][Port] + 0x1000 + 0x2d0, Value);
        
    }
    else
    {
        Value = PcieRegRead(Port, 0x80);
        Value |= 0x80;
        PcieRegWrite(Port, 0x80, Value);

        PcieRegWrite(Port, 0x184, 0x44444444);
        PcieRegWrite(Port, 0x188, 0x44444444);
        PcieRegWrite(Port, 0x18c, 0x44444444);
        PcieRegWrite(Port, 0x190, 0x44444444);
    }
}


/*****************************************************************************
 函 数 名  : PciePortReset
 功能描述  : PCIE端口复位，复位此PCIE端口下的所有设备，包括端口本身
 输入参数  : UINT32 Port
 输出参数  : 无
 返 回 值  : UINT32
*****************************************************************************/
EFI_STATUS PciePortReset(UINT32 soctype, UINT32 HostBridgeNum, UINT32 Port)
{
    if(Port >= PCIE_MAX_PORT_NUM)
    {
        return EFI_INVALID_PARAMETER;
    }
    
    //如果link up ,需要先停止状态机
    //if(!PcieIsLinkDown(Port) && (mPcieIntCfg.PciePort[Port] == TRUE))
    //{
    //    PcieDisableItssm(Port);
    //}
    
    if(PcieIsLinkUp(soctype, HostBridgeNum, Port) && mPcieIntCfg.PortIsInitilized[Port])
    {
        (VOID)PcieDisableItssm(soctype, HostBridgeNum, Port);
    }
    
    mPcieIntCfg.PortIsInitilized[Port] = FALSE;
    /*注意资源释放*/
    mPcieIntCfg.DmaResource[Port] = (VOID *)NULL;
    mPcieIntCfg.DmaChannel[Port][PCIE_DMA_CHANLE_READ] = 0;
    mPcieIntCfg.DmaChannel[Port][PCIE_DMA_CHANLE_WRITE] = 0;
    ZeroMem(&mPcieIntCfg.Dev[Port], sizeof(DRIVER_CFG_U));

    if(Port <= 2)
    {
        /*写subctrl寄存器进行复位操作*/
        RegWrite(pcie_subctrl_base[HostBridgeNum]+ PCIE_SUBCTRL_SC_PCIE0_RESET_REQ_REG + (UINT32)(8 * Port), 0x1);
        MicroSecondDelay(0x1000);
        
        /*写subctrl寄存器进行去复位操作*/
        RegWrite(pcie_subctrl_base[HostBridgeNum] + PCIE_SUBCTRL_SC_PCIE0_RESET_DREQ_REG + (UINT32)(8 * Port), 0x1);
        MicroSecondDelay(0x1000);
    }
    else
    {
        /*写subctrl寄存器进行复位操作*/
        RegWrite(pcie_subctrl_base[HostBridgeNum] + PCIE_SUBCTRL_SC_PCIE3_RESET_REQ_REG,0x1);
        MicroSecondDelay(0x1000);
        
        /*写subctrl寄存器进行去复位操作*/
        RegWrite(pcie_subctrl_base[HostBridgeNum] + PCIE_SUBCTRL_SC_PCIE3_RESET_DREQ_REG,0x1);
        MicroSecondDelay(0x1000);
    }
    return EFI_SUCCESS;
}

/*****************************************************************************
 函 数 名  : AssertPcieCoreReset
 功能描述  : PCIE端口复位，复位此PCIE端口下的所有设备，包括端口本身
 输入参数  : UINT8 soctype,UINT32 Port, UINT32 HostBridgeNum
 输出参数  : 无
 作者      : s00296804
 返 回 值  : UINT32
*****************************************************************************/
EFI_STATUS AssertPcieCoreReset(UINT32 soctype, UINT32 HostBridgeNum, UINT32 Port)
{
    if(Port >= PCIE_MAX_PORT_NUM)
    {
        return EFI_INVALID_PARAMETER;
    }
    
    //如果link up ,需要先停止状态机
    
    if(PcieIsLinkUp(soctype, HostBridgeNum, Port))
    {
        (VOID)PcieDisableItssm(soctype, HostBridgeNum, Port);
    }
 
    if (0x1610 == soctype)
    {
        if(Port <= 2)
        {
            /*写subctrl寄存器进行复位操作*/
            RegWrite(pcie_subctrl_base_1610[HostBridgeNum] + PCIE_SUBCTRL_SC_PCIE0_RESET_REQ_REG + (UINT32)(8 * Port), 0x3);
            MicroSecondDelay(0x1000);
        }
        else
        {
            /*写subctrl寄存器进行复位操作*/
            RegWrite(pcie_subctrl_base_1610[HostBridgeNum] + PCIE_SUBCTRL_SC_PCIE3_RESET_REQ_REG, 0x3);
            MicroSecondDelay(0x1000);
        }
    }
    else
    {
        if(Port <= 2)
        {
            /*写subctrl寄存器进行复位操作*/
            RegWrite(pcie_subctrl_base[HostBridgeNum] + PCIE_SUBCTRL_SC_PCIE0_RESET_REQ_REG + (UINT32)(8 * Port), 0x1);
            MicroSecondDelay(0x1000);
        }
        else
        {
            /*写subctrl寄存器进行复位操作*/
            RegWrite(pcie_subctrl_base[HostBridgeNum] + PCIE_SUBCTRL_SC_PCIE3_RESET_REQ_REG,0x1);
            MicroSecondDelay(0x1000);
        }
    }
    
    return EFI_SUCCESS;
}

/*****************************************************************************
 函 数 名  : DeassertPcieCoreReset
 功能描述  : PCIE端口复位，复位此PCIE端口下的所有设备，包括端口本身
 输入参数  : UINT32 Port
 输出参数  : 无
 作者      : s00296804
 返 回 值  : UINT32
*****************************************************************************/
EFI_STATUS DeassertPcieCoreReset(UINT32 soctype, UINT32 HostBridgeNum, UINT32 Port)
{
    if(Port >= PCIE_MAX_PORT_NUM)
    {
        return EFI_INVALID_PARAMETER;
    }
    
    //如果link up ,需要先停止状态机
    if(PcieIsLinkUp(soctype, HostBridgeNum, Port))
    {
        (VOID)PcieDisableItssm(soctype, HostBridgeNum, Port);
    }
    
    if (0x1610 == soctype)
    {
        if(Port <= 2)
        {
            /*写subctrl寄存器进行去复位操作*/
            RegWrite(pcie_subctrl_base_1610[HostBridgeNum] + PCIE_SUBCTRL_SC_PCIE0_RESET_DREQ_REG + (UINT32)(8 * Port), 0x3);
            MicroSecondDelay(0x1000);
        }
        else
        {
            /*写subctrl寄存器进行去复位操作*/
            RegWrite(pcie_subctrl_base_1610[HostBridgeNum] + PCIE_SUBCTRL_SC_PCIE3_RESET_DREQ_REG,0x3);
            MicroSecondDelay(0x1000);
        }
    }
    else 
    {
        if(Port <= 2)
        {
            /*写subctrl寄存器进行去复位操作*/
            RegWrite(pcie_subctrl_base[HostBridgeNum] + PCIE_SUBCTRL_SC_PCIE0_RESET_DREQ_REG + (UINT32)(8 * Port), 0x1);
            MicroSecondDelay(0x1000);
        }
        else
        {
            /*写subctrl寄存器进行去复位操作*/
            RegWrite(pcie_subctrl_base[HostBridgeNum] + PCIE_SUBCTRL_SC_PCIE3_RESET_DREQ_REG,0x1);
            MicroSecondDelay(0x1000);
        }
    }
    
    return EFI_SUCCESS;
}

/*****************************************************************************
 函 数 名  : AssertPcieReset
 功能描述  : pcie serdes复位函数
 输出参数  : 无
 返 回 值  : static UINT32
 作者      : s00296804
*****************************************************************************/
EFI_STATUS AssertPciePcsReset(UINT32 soctype, UINT32 HostBridgeNum, UINT32 Port)
{
    u_sc_pcie_hilink_pcs_reset_req reset_req;
    if (0x1610 == soctype)
    {
        if(Port <= 3)
        {
            reset_req.UInt32 = 0;
            reset_req.UInt32 = reset_req.UInt32 | (0x1 << Port);
            RegWrite(pcie_subctrl_base_1610[HostBridgeNum] + PCIE_SUBCTRL_SC_PCS_LOCAL_RESET_REQ_REG, reset_req.UInt32);
            RegWrite(pcie_subctrl_base_1610[HostBridgeNum] + PCIE_SUBCTRL_SC_PCS_APB_RESET_REQ_REG, reset_req.UInt32);

            reset_req.UInt32 = 0;
            reset_req.UInt32 = reset_req.UInt32 | (0xFF << (8 * Port));
            RegWrite(pcie_subctrl_base_1610[HostBridgeNum] + PCIE_SUBCTRL_SC_PCIE_HILINK_PCS_RESET_REQ_REG, reset_req.UInt32);
            MicroSecondDelay(0x1000);        
        }
    }
    else
    {
        if(Port <= 3)
        {
            reset_req.UInt32 = 0;
            reset_req.UInt32 = reset_req.UInt32 | (0x1 << Port);
            RegWrite(pcie_subctrl_base[HostBridgeNum] + PCIE_SUBCTRL_SC_PCS_LOCAL_RESET_REQ_REG, reset_req.UInt32);

            reset_req.UInt32 = 0;
            reset_req.UInt32 = reset_req.UInt32 | (0xFF << (8 * Port));
            RegWrite(pcie_subctrl_base[HostBridgeNum] + PCIE_SUBCTRL_SC_PCIE_HILINK_PCS_RESET_REQ_REG, reset_req.UInt32);
            MicroSecondDelay(0x1000);        
        }
    }
    return EFI_SUCCESS;
}

/*****************************************************************************
 函 数 名  : DeassertPcieReset
 功能描述  : pcie serdes复位函数
 输出参数  : 无
 返 回 值  : static UINT32
 作者      : s00296804
*****************************************************************************/
EFI_STATUS DeassertPciePcsReset(UINT32 soctype, UINT32 HostBridgeNum, UINT32 Port)
{
    u_sc_pcie_hilink_pcs_reset_req reset_req;
    if (0x1610 == soctype)
    {
        if(Port <= 3)
        {
            reset_req.UInt32 = 0;
            reset_req.UInt32 = reset_req.UInt32 | (0x1 << Port);
            RegWrite(pcie_subctrl_base_1610[HostBridgeNum] + 0xacc, reset_req.UInt32);
            RegWrite(pcie_subctrl_base_1610[HostBridgeNum] + PCIE_SUBCTRL_SC_PCS_LOCAL_RESET_DREQ_REG, reset_req.UInt32);

            reset_req.UInt32 = 0;
            reset_req.UInt32 = reset_req.UInt32 | (0xFF << (8 * Port));
            RegWrite(pcie_subctrl_base_1610[HostBridgeNum] + PCIE_SUBCTRL_SC_PCIE_HILINK_PCS_RESET_DREQ_REG, reset_req.UInt32);
            MicroSecondDelay(0x1000);       
        }
    }
    else
    {
        if(Port <= 3)
        {
            reset_req.UInt32 = 0;
            reset_req.UInt32 = reset_req.UInt32 | (0x1 << Port);
            RegWrite(pcie_subctrl_base[HostBridgeNum] + PCIE_SUBCTRL_SC_PCS_LOCAL_RESET_DREQ_REG, reset_req.UInt32);

            reset_req.UInt32 = 0;
            reset_req.UInt32 = reset_req.UInt32 | (0xFF << (8 * Port));
            RegWrite(pcie_subctrl_base[HostBridgeNum] + PCIE_SUBCTRL_SC_PCIE_HILINK_PCS_RESET_DREQ_REG, reset_req.UInt32);
            MicroSecondDelay(0x1000);       
        }
    }
    
    return EFI_SUCCESS;
}

EFI_STATUS HisiPcieClockCtrl(UINT32 soctype, UINT32 HostBridgeNum, UINT32 Port, BOOLEAN Clock)
{
	UINT32 reg_clock_disable;
	UINT32 reg_clock_enable;
	//UINT32 reg_clock_status;
    //UINT8  clock_ctrl_660 = 0x3;
    //UINT8  clock_ctrl_1610 = 0x7;
	
	if (Port == 3) {
		reg_clock_disable = PCIE_SUBCTRL_SC_PCIE3_CLK_DIS_REG;
		reg_clock_enable = PCIE_SUBCTRL_SC_PCIE3_CLK_EN_REG;
		//reg_clock_status = PCIE_SUBCTRL_SC_PCIE3_CLK_ST_REG;
	} else {
		reg_clock_disable = PCIE_SUBCTRL_SC_PCIE0_2_CLK_DIS_REG(Port);
		reg_clock_enable = PCIE_SUBCTRL_SC_PCIE0_2_CLK_EN_REG(Port);
		//reg_clock_status = PCIE_SUBCTRL_SC_PCIE0_2_CLK_ST_REG(Port);
	}

    if (0x1610 == soctype)
    {
    	if (Clock)
    		RegWrite(pcie_subctrl_base_1610[HostBridgeNum] + reg_clock_enable, 0x7);
    	else
    		RegWrite(pcie_subctrl_base_1610[HostBridgeNum] + reg_clock_disable, 0x7);
    }
    else 
    {
        if (Clock)
    		RegWrite(pcie_subctrl_base[HostBridgeNum] + reg_clock_enable, 0x3);
    	else
    		RegWrite(pcie_subctrl_base[HostBridgeNum] + reg_clock_disable, 0x3);
    }
    return EFI_SUCCESS;
}

VOID PciePortNumSet(UINT32 soctype, UINT32 HostBridgeNum, UINT32 Port, UINT8 Num)
{
    if (0x1610 == soctype)
    {
        UINT32 Value = 0;
        RegRead(PCIE_APB_SLVAE_BASE_1610[HostBridgeNum][Port] + 0x1000 + 0x1c, Value);
        Value &= ~(0xff);
        Value |= Num;
        RegWrite(PCIE_APB_SLVAE_BASE_1610[HostBridgeNum][Port] + 0x1000 + 0x1c, Value);
    }
    return;
}
/*****************************************************************************
 函 数 名  : PcieMaskLinkUpInit
 功能描述  : 
 输出参数  : 无
 返 回 值  : UINT32
*****************************************************************************/
EFI_STATUS PcieMaskLinkUpInit(UINT32 soctype, UINT32 HostBridgeNum, UINT32 Port)
{
    UINT32 Value = 0;
    if (0x1610 == soctype)
    {
        //DTS2015110700120 1610 PCIe,解决概率link失败的问题
        Value = PcieRegRead(Port, 0x120);
		Value |= 1 << 25;
		PcieRegWrite(Port,0x120, Value);
    }
    else
    {
        PcieChangeRwMode(HostBridgeNum, Port, PCIE_SYS_CONTROL);
        Value = PcieRegRead(Port, 0x1d0);
        Value |= 1 << 12;
        PcieRegWrite(Port,0x1d0, Value);
        PcieChangeRwMode(HostBridgeNum, Port, PCIE_CONFIG_REG);
    }
    return EFI_SUCCESS;
}

/*****************************************************************************
 函 数 名  : PcieIsLinkUp
 功能描述  : 判断是否link Up
 输入参数  : UINT32 Port,UINT32 HostBridgeNum
 输出参数  : 无
 返 回 值  : UINT32 : FALSE : linkdown TRUE : linkup
*****************************************************************************/
BOOLEAN PcieIsLinkUp(UINT32 soctype, UINT32 HostBridgeNum, UINT32 Port)
{
    UINT32                     Value = 0;
    U_SC_PCIE0_SYS_STATE4      PcieStat;
    if (0x1610 == soctype)
    {
        RegRead(PCIE_APB_SLVAE_BASE_1610[HostBridgeNum][Port] + 0x131c, PcieStat.UInt32);
        Value = PcieStat.UInt32;
        if ((Value & PCIE_LTSSM_STATE_MASK) == PCIE_LTSSM_LINKUP_STATE)
            return TRUE;
        return FALSE;
    }
    else
    {
        RegRead(pcie_subctrl_base[HostBridgeNum] + PCIE_SUBCTRL_SC_PCIE0_SYS_STATE4_REG + (UINT32)(0x100 * Port), PcieStat.UInt32);
        Value = PcieStat.UInt32;
        if ((Value & PCIE_LTSSM_STATE_MASK) == PCIE_LTSSM_LINKUP_STATE)
            return TRUE;
        return FALSE;
    }
}

BOOLEAN PcieClockIsLock(UINT32 soctype, UINT32 HostBridgeNum, UINT32 Port)
{
    UINT32 Value = 0;
    if (0x1610 == soctype)
    {
        RegRead( PCIE_PHY_BASE_1610[HostBridgeNum][Port] + 0x504, Value);
        return ((Value & 0x3) == 0x3);
    }
    else return TRUE;
    
}

VOID PcieSpdSet(UINT32 soctype, UINT32 HostBridgeNum, UINT32 Port, UINT8 Spd)
{
    UINT32 Value = 0;
    if (0x1610 == soctype)
    {
        RegRead(PCIE_APB_SLVAE_BASE_1610[HostBridgeNum][Port] + 0xa0, Value);
        Value &= ~(0xf);
    	Value |= Spd;
    	RegWrite(PCIE_APB_SLVAE_BASE_1610[HostBridgeNum][Port] + 0xa0, Value); 
        return;
    }
    return;
}
//DTS2015111710758 PCIe建链保持和内核一致
VOID PcieWriteOwnConfig(UINT32 HostBridgeNum, UINT32 Port, UINT32 Offset, UINT32 Data)
{
     UINT32 Value = 0;
     //if (0x1610 == soctype)
     {
         RegRead(PCIE_APB_SLVAE_BASE_1610[HostBridgeNum][Port] + (Offset & (~0x3)), Value);
         Value &= 0x0000ffff;
         Value |= 0x06040000;
         RegWrite(PCIE_APB_SLVAE_BASE_1610[HostBridgeNum][Port] + (Offset & (~0x3)), Value); 
         return;
     }
     //else
    // {

     //}
}

void PcieConfigContextHi1610(UINT32 soctype, UINT32 HostBridgeNum, UINT32 Port)
{
	UINT32 Value = 0;

    RegWrite(PCIE_APB_SLVAE_BASE_1610[HostBridgeNum][Port] + 0x11b4, 0xc6010040);
    RegWrite(PCIE_APB_SLVAE_BASE_1610[HostBridgeNum][Port] + 0x11c4, 0);
    RegRead(PCIE_APB_SLVAE_BASE_1610[HostBridgeNum][Port] + 0x11c8, Value);
    Value |= (1 << 12);
    RegWrite(PCIE_APB_SLVAE_BASE_1610[HostBridgeNum][Port] + 0x11c8, Value);

    return;
}

/*****************************************************************************
 函 数 名  : PciePortInit
 功能描述  : PCIE端口初始化
 输入参数  : PCIE_DRIVER_CFG        *PcieCfg,HostBridgeNum
 输出参数  : 无
 返 回 值  : EFI_STATUS

 修改历史      :
  1.日    期   : 
    作    者   :
    修改内容   : 

*****************************************************************************/
EFI_STATUS
EFIAPI
PciePortInit (
  IN UINT32                 soctype,
  IN UINT32                 HostBridgeNum,
  IN PCIE_DRIVER_CFG        *PcieCfg
  )
{
     UINT16              Count = 0;
     UINT32             PortIndex = PcieCfg->PortIndex;
     
     // uniBIOS_y00216284_081_start DTS2015081408681  2015-8-14 >>>
     // Description:统一在SmmuConfigDxe驱动中处理
     //*(volatile UINT32 *)(PCIE_SMMU_BASE + 0) = 0x00150101;
     // uniBIOS_y00216284_081_end 2015-8-14 <<<
     
     if(PortIndex >= PCIE_MAX_PORT_NUM)
     {
        return EFI_INVALID_PARAMETER;
     }

     if (0x1610 == soctype)
     {
         mPcieIntCfg.RegResource[PortIndex] = (VOID *)PCIE_APB_SLVAE_BASE_1610[HostBridgeNum][PortIndex];
         DEBUG((EFI_D_ERROR, "Soc type is 1610\n"));
     }
     else
     {        
         mPcieIntCfg.RegResource[PortIndex] = (VOID *)(UINTN)PCIE_REG_BASE(HostBridgeNum, PortIndex);
         DEBUG((EFI_D_ERROR, "Soc type is 660\n"));
     }
      
     /* assert reset signals */
     (VOID)AssertPcieCoreReset(soctype, HostBridgeNum, PortIndex);
     (VOID)AssertPciePcsReset(soctype, HostBridgeNum, PortIndex);
     (VOID)HisiPcieClockCtrl(soctype, HostBridgeNum, PortIndex, 0);
     (VOID)DeassertPcieCoreReset(soctype, HostBridgeNum, PortIndex);
     /* de-assert phy reset */
     (VOID)DeassertPciePcsReset(soctype, HostBridgeNum, PortIndex);

     /* de-assert core reset */    
     (VOID)HisiPcieClockCtrl(soctype, HostBridgeNum, PortIndex, 1);

     //看时钟是否锁住
     while (!PcieClockIsLock(soctype, HostBridgeNum, PortIndex)) {
         MicroSecondDelay(1000);
		 Count++;
		 if (Count >= 50) {
			DEBUG((EFI_D_ERROR, "HostBridge %d, Port %d PLL Lock failed\n", HostBridgeNum, PortIndex));
			return PCIE_ERR_LINK_OVER_TIME;
		 }
	 }
     //DEBUG((EFI_D_ERROR, "HostBridge %d, Port %d PLL Lock ok\n", HostBridgeNum, PortIndex));
     /* initialize phy */
     (VOID)PciePcsInit(soctype, HostBridgeNum, PortIndex);
     
     //DEBUG((EFI_D_ERROR, "HostBridge %d, Pcie Port 0x%d Port pcs init ok.\n",HostBridgeNum, PortIndex));

     //设置PCIE工作模式为相应的模式
     (VOID)PcieModeSet(soctype, HostBridgeNum, PortIndex,PcieCfg->PortInfo.PortType);
     //DEBUG((EFI_D_ERROR, "HostBridge %d, Pcie Port 0x%d ModeSet ok.\n",HostBridgeNum, PortIndex));
     //PcieDelay(50000);
     //1610新加
     (VOID)PcieSpdSet(soctype, HostBridgeNum, PortIndex, 3);
     // DEBUG((EFI_D_ERROR, "HostBridge %d, Pcie Port 0x%d Spd set ok.\n",HostBridgeNum, PortIndex));
     (VOID)PciePortNumSet(soctype, HostBridgeNum, PortIndex, 0);
     //DEBUG((EFI_D_ERROR, "HostBridge %d, Pcie Port 0x%d Port num set ok.\n",HostBridgeNum, PortIndex));
     /* setup root complex */
     (VOID)PcieSetupRC(PortIndex,PcieCfg->PortInfo.PortWidth);
     //DEBUG((EFI_D_ERROR, "HostBridge %d, Pcie Port 0x%d SetupRC ok.\n",HostBridgeNum, PortIndex));

     /* disable link up interrupt */
     (VOID)PcieMaskLinkUpInit(soctype, HostBridgeNum, PortIndex);

     /* Pcie Equalization*/
     (VOID)PcieEqualization(soctype ,HostBridgeNum, PortIndex);

     /* assert LTSSM enable */
     (VOID)PcieEnableItssm(soctype, HostBridgeNum, PortIndex);

     //uniBIOS__l00228991_start DTS2015072002840 2015-7-20 13:35:42
     //Description:Lemon新单板适配代码上库
     /* check if the link is up or not */
	 while (!PcieIsLinkUp(soctype, HostBridgeNum, PortIndex)) {
         MicroSecondDelay(1000);
		 Count++;
		 if (Count >= 1000) {//DTS2015072003703 设为1s
			DEBUG((EFI_D_ERROR, "HostBridge %d, Port %d link up failed\n", HostBridgeNum, PortIndex));
			return PCIE_ERR_LINK_OVER_TIME;
		 }
	 }
     //uniBIOS__l00228991_end   2015-7-20 13:35:42
     DEBUG((EFI_D_ERROR, "HostBridge %d, Port %d Link up ok\n", HostBridgeNum, PortIndex));
     
     PcieConfigContextHi1610(soctype, HostBridgeNum, PortIndex);
     PcieRegWrite(PortIndex, 0x10, 0);
	 //DTS2015111710758 PCIe建链保持和内核一致
     (VOID)PcieWriteOwnConfig(HostBridgeNum, PortIndex, 0xa, 0x0604);
     PcieRegWrite(PortIndex, 0x8BC, 0); //关掉RC寄存器属性为RO的可写功能
     //PcieRegWrite(PortIndex, 0x8, 0x6040000);
     //PcieRegWrite(PortIndex, 0x80c, 0x208FF);
     
     return EFI_SUCCESS;
}




/*****************************************************************************
 函 数 名  : PcieSetDBICS2Enable
 功能描述  : DBI CS2 Enable
 输入参数  : UINT32 Port
             UINT32 enable
 输出参数  : 无
 返 回 值  : UINT32
*****************************************************************************/
EFI_STATUS PcieSetDBICS2Enable(UINT32 HostBridgeNum, UINT32 Port, UINT32 Enable)
{
    PCIE_SYS_CTRL20_U dbi_ro_enable;

    if(Port >= PCIE_MAX_PORT_NUM)
    {
        return EFI_INVALID_PARAMETER;
    }

    PcieChangeRwMode(HostBridgeNum, Port, PCIE_SYS_CONTROL);
    dbi_ro_enable.UInt32 = PcieRegRead(Port, PCIE_SYS_CTRL20_REG);
    dbi_ro_enable.Bits.ro_sel = Enable;
    PcieRegWrite(Port, PCIE_SYS_CTRL20_REG, dbi_ro_enable.UInt32);
    PcieChangeRwMode(HostBridgeNum, Port, PCIE_CONFIG_REG);

    return EFI_SUCCESS;

}

/*****************************************************************************
 函 数 名  : PcieDelay
 功能描述  : pcie delay
 输入参数  : UINT32 cnt
 输出参数  : 无
 返 回 值  : static VOID
*****************************************************************************/
//DTS2015072900796 PCLint消除
VOID PcieDelay(UINT32 dCount)
{
    volatile UINT32 *uwCnt = &dCount;

    while(*uwCnt > 0)
    {
        *uwCnt = *uwCnt - 1;
    }

}

