#include <PiDxe.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Guid/EventGroup.h>
#include <Library/UefiRuntimeLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/CpldIoLib.h>
#include <Library/TimerLib.h>
#include <Library/PcdLib.h>
#include <PlatformArch.h>
#include <Library/OemAddressMapLib.h>
#include <Library/OemMiscLib.h>

UINTN           mCpldRegAddr;
EFI_EVENT       mCpldVirtualAddressChangeEvent;


VOID
EFIAPI
CpldVirtualAddressChange (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  EfiConvertPointer (0, (VOID **) &mCpldRegAddr);

  return;
}

RETURN_STATUS
EFIAPI
CpldRuntimeLibConstructor (
    VOID  
)
{
    EFI_STATUS              Status;
    EFI_GCD_MEMORY_SPACE_DESCRIPTOR desp = {0}; 

    mCpldRegAddr = PcdGet64(PcdCpldBaseAddress);
    DEBUG((EFI_D_ERROR,"Creat event for rtc. \r\n"));
    Status = gDS->GetMemorySpaceDescriptor(mCpldRegAddr,&desp);
    if(EFI_ERROR(Status)){
        DEBUG ((EFI_D_ERROR, "[%a]:[%dL] GetMemorySpaceDescriptor failed: %r\n", __FUNCTION__, __LINE__, Status));
        return Status;
    } 
    desp.Attributes |= EFI_MEMORY_RUNTIME;
    Status = gDS->SetMemorySpaceAttributes(mCpldRegAddr,0x10000, desp.Attributes);
    if(EFI_ERROR(Status)){
			  DEBUG ((EFI_D_ERROR, "[%a]:[%dL] SetMemorySpaceAttributes failed: %r\n", __FUNCTION__, __LINE__, Status));
        return Status;
    } 
    //
    // Register notify function for EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE
    // 
    Status = gBS->CreateEventEx (
                    EVT_NOTIFY_SIGNAL,
                    TPL_NOTIFY,
                    CpldVirtualAddressChange,
                    NULL,
                    &gEfiEventVirtualAddressChangeGuid,
                    &mCpldVirtualAddressChangeEvent
                    );
    DEBUG((EFI_D_ERROR,"Creat event for cpld %r . \r\n",Status));
    ASSERT_EFI_ERROR (Status);
    return Status;
}

EFI_STATUS
EFIAPI
CpldRuntimeLibDestructor (
    VOID  
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  
  if(!mCpldVirtualAddressChangeEvent ){
     return Status;
  }
  
  Status = gBS->CloseEvent(mCpldVirtualAddressChangeEvent);
  return Status;
}

/*****************************************************************************
  �� �� ��  : WriteCpldReg
  ��������  : CPLD�Ĵ���д
  �������  : UINT32 ulAddr
            UINT32 ulValue
  �������  : ��
  �� �� ֵ  : VOID
*****************************************************************************/
VOID WriteCpldReg(UINTN ulRegAddr, UINT8 ulValue)
{
    *(volatile UINT8 *)(ulRegAddr + mCpldRegAddr) = ulValue;
}

/*****************************************************************************
  �� �� ��  : ReadCpldReg
  ��������  : CPLD�Ĵ�����
  �������  : UINTN ulRegAddr    
  �������  : ��
  �� �� ֵ  : UINT32
*****************************************************************************/
UINT8 ReadCpldReg(UINTN ulRegAddr)
{    
    volatile UINT8 ulValue = 0;
    
    ulValue = *(volatile UINT8 *)(ulRegAddr + mCpldRegAddr);
    
    return (ulValue); 
}


