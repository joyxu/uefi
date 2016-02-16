/*-----------------------------------------------------------------------*/
/*!!Warning: Huawei key information asset. No spread without permission. */
/*CODEMARK:EG4uRhTwMmgcVFBsBnYHCEm2UPcyllv4D4NOje6cFLSYglw6LvPA978sGAr3yTchgOI0M46H
HZIZCDLcNqR1rbxHHGWmLNp+CRsGfVaxSWSW9PGkrN+FTCe9yN1IrSBPE0G6AkpKaoQRL2DK
39fCQLAi1+W/M9Rj7XK8Mns5dxjAWb1WErWRGoklQ+Hm66uFFsUjWHOrKLeibvrtd/AR5F/K
T6Qq3cMzdWJDSvsuCwXMSYmDQ8AbHHIXPN9y6Yk5dg3ocCSDhrz16k7nM10pKg==#*/
/*--!!Warning: Deleting or modifying the preceding information is prohibited.--*/


#include <Uefi.h>
#include <Library/BaseLib.h>
#include <libfdt.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/FdtUpdateLib.h>
#include <PlatformArch.h>
//#include <PlatformArchRestricted.h>
#include <Library/PcdLib.h>
#include <Library/PlatformSysCtrlLib.h>
//#include <Library/PlatformSysCtrlLibRestricted.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/OemNicProtocol.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/OemMiscLib.h>
#include <Guid/BootLine.h>
//#include <SetupVariable.h>

typedef union AA_DAW
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sysdaw_id           : 7  ; /* [6:0] */
        unsigned int    interleave_en       : 1  ; /* [7] */
        unsigned int    sysdaw_size         : 4  ; /* [11:8] */
        unsigned int    reserved            : 4  ; /* [15:12] */
        unsigned int    sysdaw_addr         : 16 ; /* [31:16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} AA_DAW_U;



MAC_ADDRESS gMacAddress[1];


CHAR8  *EthNameC10[8]=
{
 "ethernet@0","ethernet@1",
 "ethernet@4","ethernet@5",
 "ethernet@6","ethernet@7",
 "ethernet@2","ethernet@3"
};

UINT8 EthPortC10[4] = {0, 1, 4, 5};

CHAR8  *EthName[8]=
{
 "ethernet@0","ethernet@1",
 "ethernet@6","ethernet@7",
 "ethernet@4","ethernet@5",
 "ethernet@2","ethernet@3"
};

UINT8 EthPort[4] = {0, 1, 6, 7};

UINT8 DawNum[4] = {0, 0, 0, 0};
PHY_MEM_REGION    *NodemRegion[4] = {NULL, NULL, NULL, NULL};
UINTN  NumaPages[4] = {0, 0, 0, 0};

CHAR8  *NumaNodeName[4]=
{
 "p0-ta","p0-tc",
 "p1-ta","p1-tc",
};

STATIC
BOOLEAN
IsMemMapRegion (
  IN EFI_MEMORY_TYPE MemoryType
  )
{
  switch(MemoryType)
    {
        case EfiRuntimeServicesCode:
        case EfiRuntimeServicesData:
        case EfiConventionalMemory:
        case EfiACPIReclaimMemory:
        case EfiACPIMemoryNVS:
        case EfiLoaderCode:
        case EfiLoaderData: 
        case EfiBootServicesCode:
        case EfiBootServicesData:
        case EfiPalCode:
                return TRUE;
        default:
                return FALSE;
  }
}


EFI_STATUS
GetMacAddress (UINT32 Port)
{
    EFI_MAC_ADDRESS Mac;
    EFI_STATUS Status;
    OEM_NIC_PROTOCOL *OemNic = NULL;

    Status = gBS->LocateProtocol(&gOemNicProtocolGuid, NULL, (VOID **)&OemNic);
    if(EFI_ERROR(Status))
    {
        DEBUG((EFI_D_ERROR, "[%a]:[%dL] LocateProtocol failed %r\n", __FUNCTION__, __LINE__, Status));
        return Status;
    }

    Status = OemNic->GetMac(&Mac, Port);
    if(EFI_ERROR(Status))
    {
        DEBUG((EFI_D_ERROR, "[%a]:[%dL] GetMac failed %r\n", __FUNCTION__, __LINE__, Status));
        return Status;
    }
  
    gMacAddress[0].data0=Mac.Addr[0];
    gMacAddress[0].data1=Mac.Addr[1];
    gMacAddress[0].data2=Mac.Addr[2];
    gMacAddress[0].data3=Mac.Addr[3];
    gMacAddress[0].data4=Mac.Addr[4];
    gMacAddress[0].data5=Mac.Addr[5];
    DEBUG((EFI_D_ERROR, "Port%d:0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n",
        Port,gMacAddress[0].data0,gMacAddress[0].data1,gMacAddress[0].data2,
        gMacAddress[0].data3,gMacAddress[0].data4,gMacAddress[0].data5));
   
    return EFI_SUCCESS;
}

STATIC
EFI_STATUS
DelPhyhandleUpdateMacAddress(IN VOID* Fdt)
{
    UINT8               port;
    INTN                ethernetnode;
    INTN			    node;
    INTN                Error;
    struct              fdt_property *m_prop;
    int                 m_oldlen;
    EFI_STATUS          Status = EFI_SUCCESS;
    EFI_STATUS          GetMacStatus = EFI_SUCCESS;
    UINT8               BoardType = PcdGet8(PcdBoardType);
    
    node = fdt_subnode_offset(Fdt, 0, "soc");
    if (node < 0) 
    {
        DEBUG ((EFI_D_ERROR, "can not find soc root node\n"));
        return EFI_INVALID_PARAMETER;
    }
    else
    {
        //获得4个网口的MAC ADDRESS,并更新ethernet@xxxxxx 的local-mac-address
        for( port=0; port<4; port++ )
        {
            
            if(BoardType)
            {
                GetMacStatus= GetMacAddress(EthPortC10[port]);
                ethernetnode = fdt_subnode_offset(Fdt, node,EthNameC10[port]);
            }
            else
            {
                GetMacStatus= GetMacAddress(EthPort[port]);
                ethernetnode = fdt_subnode_offset(Fdt, node,EthName[port]); 
            }
            if(!EFI_ERROR(GetMacStatus))
            {
                
                if (ethernetnode < 0) 
                {
                    DEBUG ((EFI_D_ERROR, "can not find ethernet@%d node\n",port));
                }
                m_prop = fdt_get_property_w(Fdt, ethernetnode, "local-mac-address", &m_oldlen);
                if(m_prop)
                {
                    Error = fdt_delprop(Fdt, ethernetnode, "local-mac-address");
                    if (Error) 
                    {
                        DEBUG ((EFI_D_ERROR, "ERROR:fdt_delprop() Local-mac-address: %a\n", fdt_strerror (Error)));
                        Status = EFI_INVALID_PARAMETER;
                    } 
                    Error = fdt_setprop(Fdt, ethernetnode, "local-mac-address",gMacAddress,sizeof(MAC_ADDRESS));
                    if (Error) 
                    {
                        DEBUG ((EFI_D_ERROR, "ERROR:fdt_setprop():local-mac-address %a\n", fdt_strerror (Error)));
                        Status = EFI_INVALID_PARAMETER;
                    } 
                }
            }
        }
    }
    return Status;
}

INTN
GetMemoryNode(VOID* Fdt)
{
    INTN                node;
    int                 m_oldlen;
    struct              fdt_property *m_prop;
    INTN                Error = 0;

   
    node = fdt_subnode_offset(Fdt, 0, "memory");
    if (node < 0) 
    {
        // Create the memory node
        node = fdt_add_subnode(Fdt, 0, "memory");
        
        if(node < 0)
        {
          DEBUG((EFI_D_INFO, "[%a]:[%dL] fdt add subnode error\n", __FUNCTION__, __LINE__));
          
          return node;
        }
       
    }
    //find the memory node property  
    m_prop = fdt_get_property_w(Fdt, node, "memory", &m_oldlen);
    if(m_prop)
    {
        Error = fdt_delprop(Fdt, node, "reg");
       
        if (Error) 
        {
            DEBUG ((EFI_D_ERROR, "ERROR:fdt_delprop(): %a\n", fdt_strerror (Error)));
            node = -1;
            return node;
        } 
    }

    return node;
}


EFI_STATUS UpdateMemoryNode(VOID* Fdt)
{
    INTN                Error = 0;
    EFI_STATUS          Status = EFI_SUCCESS;
    UINT32              Index = 0;
    UINT32              MemIndex;
    INTN			    node;
    EFI_MEMORY_DESCRIPTOR *MemoryMap;
    EFI_MEMORY_DESCRIPTOR *MemoryMapPtr;
    EFI_MEMORY_DESCRIPTOR *MemoryMapPtrCurrent;
    UINTN                 MemoryMapSize;
    UINTN                 Pages0 = 0;
    UINTN                 Pages1 = 0;
    UINTN                 MapKey;
    UINTN                 DescriptorSize;
    UINT32                DescriptorVersion;
    PHY_MEM_REGION        *mRegion;
    UINTN                 MemoryMapLastEndAddress ;
    UINTN                 MemoryMapcontinuousStartAddress ;
    UINTN                 MemoryMapCurrentStartAddress;
    BOOLEAN               FindMemoryRegionFlag = FALSE;
   
    node = GetMemoryNode(Fdt);
    if (node < 0)
    {
        DEBUG((EFI_D_ERROR,"Can not find memory node\n"));
        return EFI_NOT_FOUND;
    }
    //2,得到当前的系统内存布局	
    MemoryMap = NULL;
    MemoryMapSize = 0;
    MemIndex = 0;
 
    Status = gBS->GetMemoryMap (&MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
    if (Status == EFI_BUFFER_TOO_SMALL)
    {
        // The UEFI specification advises to allocate more memory for the MemoryMap buffer between successive
        // calls to GetMemoryMap(), since allocation of the new buffer may potentially increase memory map size.
        //DEBUG ((EFI_D_ERROR, "MemoryMapsize: 0x%lx\n",MemoryMapSize));
        Pages0 = EFI_SIZE_TO_PAGES (MemoryMapSize) + 1;
        MemoryMap = AllocatePages (Pages0);
        if (MemoryMap == NULL)
        {
            Status = EFI_OUT_OF_RESOURCES;
            return Status;
        }
        Status = gBS->GetMemoryMap (&MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
        
        if (EFI_ERROR(Status))
        {
            DEBUG ((EFI_D_ERROR, "FdtUpdateLib GetMemoryMap Error\n"));
            FreePages (MemoryMap, Pages0);
            return Status;
        }      
    }
    else
    {
        DEBUG ((EFI_D_ERROR, "FdtUpdateLib GetmemoryMap Status: %r\n",Status));
        return EFI_ABORTED;
    }

    mRegion = NULL;
    Pages1 = EFI_SIZE_TO_PAGES (sizeof(PHY_MEM_REGION) *( MemoryMapSize / DescriptorSize));
    
    mRegion = (PHY_MEM_REGION*)AllocatePool(Pages1);
    if (mRegion == NULL)
    {
      Status = EFI_OUT_OF_RESOURCES;
      FreePages (MemoryMap, Pages0);
      return Status;
    }
	
    
    MemoryMapPtr = MemoryMap;
    MemoryMapPtrCurrent = MemoryMapPtr;
    MemoryMapLastEndAddress = 0;
    MemoryMapcontinuousStartAddress = 0;
    MemoryMapCurrentStartAddress = 0;
    for (Index = 0; Index < (MemoryMapSize / DescriptorSize); Index++)
    {
        MemoryMapPtrCurrent = (EFI_MEMORY_DESCRIPTOR*)((UINTN)MemoryMapPtr + Index*DescriptorSize);
        MemoryMapCurrentStartAddress = (UINTN)MemoryMapPtrCurrent->PhysicalStart;
       
        if (!IsMemMapRegion ((EFI_MEMORY_TYPE)MemoryMapPtrCurrent->Type))
        {
            continue;
        }
        else 
        {
            FindMemoryRegionFlag = TRUE;
            if(MemoryMapCurrentStartAddress != MemoryMapLastEndAddress)
            {
                mRegion[MemIndex].BaseHigh= cpu_to_fdt32(MemoryMapcontinuousStartAddress>>32);
	            mRegion[MemIndex].BaseLow=cpu_to_fdt32(MemoryMapcontinuousStartAddress);
                mRegion[MemIndex].LengthHigh= cpu_to_fdt32((MemoryMapLastEndAddress-MemoryMapcontinuousStartAddress)>>32);
	            mRegion[MemIndex].LengthLow=cpu_to_fdt32(MemoryMapLastEndAddress-MemoryMapcontinuousStartAddress);
                MemIndex+=1;
                MemoryMapcontinuousStartAddress=MemoryMapCurrentStartAddress;
            }
        }
        MemoryMapLastEndAddress = (UINTN)(MemoryMapPtrCurrent->PhysicalStart + MemoryMapPtrCurrent->NumberOfPages * EFI_PAGE_SIZE);
    }
    if (FindMemoryRegionFlag)
    {
        mRegion[MemIndex].BaseHigh = cpu_to_fdt32(MemoryMapcontinuousStartAddress>>32);
        mRegion[MemIndex].BaseLow = cpu_to_fdt32(MemoryMapcontinuousStartAddress);
        mRegion[MemIndex].LengthHigh = cpu_to_fdt32((MemoryMapLastEndAddress-MemoryMapcontinuousStartAddress)>>32);
	    mRegion[MemIndex].LengthLow = cpu_to_fdt32(MemoryMapLastEndAddress-MemoryMapcontinuousStartAddress);
    }
    
    Error = fdt_setprop(Fdt, node, "reg",mRegion,sizeof(PHY_MEM_REGION) *(MemIndex+1));
    
    FreePool (mRegion);
    FreePages (MemoryMap, Pages0);
    if (Error)
    {
        DEBUG ((EFI_D_ERROR, "ERROR:fdt_setprop(): %a\n", fdt_strerror (Error)));
        Status = EFI_INVALID_PARAMETER;
        return Status;
    }
 
  return Status;
}


EFI_STATUS
UpdateNumaNode(VOID* Fdt)
{
    //TODO: Need to update numa node
    return EFI_SUCCESS;
}
/*
 * Entry point for fdtupdate lib.
 */

EFI_STATUS EFIFdtUpdate(UINTN FdtFileAddr)
{
    INTN                Error;
    VOID*               Fdt;      
    UINT32              Size;     
    UINTN               NewFdtBlobSize;   
    UINTN               NewFdtBlobBase; 
    EFI_STATUS          Status = EFI_SUCCESS;
    EFI_STATUS          UpdateNumaStatus = EFI_SUCCESS;
    UINT32              Index = 0;
    

    Error = fdt_check_header ((VOID*)(FdtFileAddr));
    if (0 != Error) 
    {
        DEBUG ((EFI_D_ERROR,"ERROR: Device Tree header not valid (%a)\n", fdt_strerror(Error)));
        return EFI_INVALID_PARAMETER;
    }   

    Size = (UINTN)fdt_totalsize ((VOID*)(UINTN)(FdtFileAddr));
    NewFdtBlobSize = Size + ADD_FILE_LENGTH;
    Fdt = (VOID*)(UINTN)FdtFileAddr;

    Status = gBS->AllocatePages (AllocateAnyPages, EfiBootServicesData, EFI_SIZE_TO_PAGES(NewFdtBlobSize), &NewFdtBlobBase);
    if (EFI_ERROR (Status)) 
    {
        return EFI_OUT_OF_RESOURCES;
    }


    Error = fdt_open_into(Fdt,(VOID*)(UINTN)(NewFdtBlobBase), (NewFdtBlobSize));
    if (Error) {
        DEBUG ((EFI_D_ERROR, "ERROR:fdt_open_into(): %a\n", fdt_strerror (Error)));
        Status = EFI_INVALID_PARAMETER;
        goto EXIT;
    }

    Fdt = (VOID*)(UINTN)NewFdtBlobBase;   

    // LengLow and LengthHigh同时为0时, 标识数组结束
    for(Index = 0; gExtMemRegion[Index].LengthHigh || gExtMemRegion[Index].LengLow; Index++)
    {
        gExtMemRegion[Index].BaseHigh = cpu_to_fdt32 (gExtMemRegion[Index].BaseHigh);        
        gExtMemRegion[Index].BaseLow = cpu_to_fdt32 (gExtMemRegion[Index].BaseLow);
        gExtMemRegion[Index].LengthHigh= cpu_to_fdt32 (gExtMemRegion[Index].LengthHigh);
        gExtMemRegion[Index].LengLow = cpu_to_fdt32 (gExtMemRegion[Index].LengLow);
        gExtMemRegion[Index].Type= cpu_to_fdt32 (gExtMemRegion[Index].Type);
    }

    // Index表示最后一个结束节点, 因此Index * sizeof(gExtMemRegion[0])表示有效数据长度
    Error = fdt_setprop(Fdt, 0, "mem-extends", &gExtMemRegion, Index * sizeof(gExtMemRegion[0]));
    if (Error) 
    {
        DEBUG ((EFI_D_ERROR, "ERROR:fdt_setprop(): %a\n", fdt_strerror (Error)));
        Status = EFI_INVALID_PARAMETER;
        goto EXIT;
    }     

    
    Status = DelPhyhandleUpdateMacAddress(Fdt);
    if (EFI_ERROR (Status))
    {
        DEBUG ((EFI_D_ERROR, "DelPhyhandleUpdateMacAddress fail:\n"));
        Status = EFI_SUCCESS;//网口MAC地址更新不成功，不要将整个函数退出
    }
    
    
    Status = UpdateMemoryNode(Fdt);
    if (EFI_ERROR (Status)) 
    {
        DEBUG ((EFI_D_ERROR, "UpdateMemoryNode Error\n"));
        goto EXIT;
    }       
    
    UpdateNumaStatus = UpdateNumaNode(Fdt);
    if (EFI_ERROR (UpdateNumaStatus)) 
    {
        DEBUG ((EFI_D_ERROR, "Update NumaNode fail\n"));
    }    
    
    //将内存拷贝到原先位置
    gBS->CopyMem(((VOID*)(UINTN)(FdtFileAddr)),((VOID*)(UINTN)(NewFdtBlobBase)),NewFdtBlobSize);
    
EXIT:    
    //释放内存
    gBS->FreePages(NewFdtBlobBase,EFI_SIZE_TO_PAGES(NewFdtBlobSize));

    return Status;

    
    
}
