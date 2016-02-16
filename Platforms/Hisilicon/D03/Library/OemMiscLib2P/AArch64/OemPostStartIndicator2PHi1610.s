/*************************************************

Copyright (C), 1988-2010, Huawei Tech. Co., Ltd.

File name: HwProductsPkg\Pv660Evb\Library\OemMiscLibEvb\AArch64\OemPostStartIndicatorEvb.s

Author: uniBIOS-g00179230 (Heyi Guo)    Date: 2014.10.10

Description:

*************************************************/

#include <Uefi.h>
#include <Library/PcdLib.h>

.text
.align 2

GCC_ASM_EXPORT(OemPostStartIndicator)
GCC_ASM_EXPORT(OemDawConfigAsm)
GCC_ASM_EXPORT(OemA57Config)
GCC_ASM_EXPORT(GICD_secondary_init_asm)

ASM_PFX(OemPostStartIndicator):
    ret

//uniBIOS__l00228991_start DTS2015102903077 2015-11-10 10:15:41
//Description:SEC阶段代码平台化
ASM_PFX(OemDawConfigAsm):
    ret

ASM_PFX(OemA57Config):

    /* 待第一次DSB指令后，再清L2ACTLR[8] */  
    MRS x0, s3_1_c15_c0_0
    bic x0, x0, (1<<8)
    MSR s3_1_c15_c0_0, x0
    //l00228991 参考demo BIOS增加操作，解决VFP寄存器访问异常的问题      

    MRS x0, CPTR_EL3
    MOV w1, #0x400
    BIC w0, w0, w1
    MSR CPTR_EL3, x0


    //uniBIOS_DTS2014120901178_start 2014-12-9 10:19:52
    //Issue ID:l00228991
    //Description:合入性能优化代码
    mrs x0, s3_1_c15_c0_0    //L2ACTLR_EL1
    orr x0, x0, #(1<<14)
    bic x0, x0, #(1<<3)
    msr s3_1_c15_c0_0, x0
    //uniBIOS_DTS2014120901178_end   2014-12-9 10:19:52

    //uniBIOS__l00228991_start DTS2015041505804 2015-4-21 11:57:01
    //Description:按照海思提供配置，将CPUCTRL-EL1 bit31设置为1，规避原子锁失锁后挂死问题
    //uniBIOS__l00228991_start DTS2015121201403 2015-12-17 10:07:56
    //Description:将CPUACTRL_EL1 bit30配置为1，[30] Force main clock enable active
    mrs x0, s3_1_c15_c2_0
    orr x0, x0, #(1<<31)
    orr x0, x0, #(1<<30)
    msr s3_1_c15_c2_0, x0  
    //uniBIOS__l00228991_end   2015-12-17 10:07:56
    //uniBIOS__l00228991_start   2015-4-21 11:57:01

    //uniBIOS_y00216284_024_start DTS2015032407838 2015-3-26 11:15:59
    //Description:L1/L2 Cache ECC使能 
    mrs  x0,S3_1_c11_c0_2
    orr  x0,x0,#(1<<21)
    msr  S3_1_c11_c0_2,x0
    //uniBIOS_y00216284_024_end 2015-3-26 11:15:59

    mrs x0,CNTHCTL_EL2
    orr x0,x0,#(1<<0)  
    msr CNTHCTL_EL2,x0

    mrs x0, S3_1_c15_c2_1
    orr x0, x0, (1<<6)
    msr S3_1_c15_c2_1, x0

    //uniBIOS__l00228991_start DTS2016010805504 2016-1-8 16:03:59
    //Description:1610芯片合入50M/66M晶振兼容修改
    ldr x2, =0xD000E014
    ldr w1, [x2]
    ldr w0, =66000000
    cmp w1, wzr
    b.eq 1f    
    ldr w0, =50000000
1:        
    msr cntfrq_el0, x0    
    //uniBIOS__l00228991_end   2016-1-8 16:03:59

    ldr x0, =0x602E0050
    ldr w1, [x0]
    ldr w2, =0x3C00
    and w1, w1, w2
    ldr w2, =0xC00
    cmp w1, w2
    b.eq 1f

    mrs x0, s3_1_c15_c0_0    //L2ACTLR_EL1
    bic x0, x0, #(1<<4)
    msr s3_1_c15_c0_0, x0
1:    

    ret

  
ASM_PFX(GICD_secondary_init_asm):          //GIC初始化
    mov x19, x30
    bl    ASM_PFX(ArmReadMpidr) /*读取核号*/
    ldr   x1, =0xff0000
    and   x1, x1, x0
    lsr   x1, x1, #16
    cmp   x1, #1
    b.eq  1f
    cmp   x1, #6
    b.eq  2f
    cmp   x1, #5
    b.eq  3f

    ldr w1, =0x6d000000
    add x1,x1,#0x80         ;// GICD_IGROUPR, Only local interrupts for secondary CPUs
    mov w0, #~0             ;// Grp1 interrupts
    str w0, [x1], #4
    mov x30, x19
    ret 
1:  
    ldr w1, =0x4d000000
    add x1,x1,#0x80         ;// GICD_IGROUPR, Only local interrupts for secondary CPUs
    mov w0, #~0             ;// Grp1 interrupts
    str w0, [x1], #4
    mov x30, x19
    ret 
2:  
    ldr x1, =0x4006d000000
    add x1,x1,#0x80         ;// GICD_IGROUPR, Only local interrupts for secondary CPUs
    mov w0, #~0             ;// Grp1 interrupts
    str w0, [x1], #4
    mov x30, x19
    ret 
3:  
    ldr x1, =0x4004d000000
    add x1,x1,#0x80         ;// GICD_IGROUPR, Only local interrupts for secondary CPUs
    mov w0, #~0             ;// Grp1 interrupts
    str w0, [x1], #4
    mov x30, x19
    ret
//uniBIOS__l00228991_end 2015-11-10 10:15:41

