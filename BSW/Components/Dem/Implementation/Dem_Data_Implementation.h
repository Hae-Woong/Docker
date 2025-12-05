/* ********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 */
/*! \addtogroup Dem_Data
 *  \{
 *  \file       Dem_Data_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    
 *  \entity     Data
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_DATA_IMPLEMENTATION_H)
#define DEM_DATA_IMPLEMENTATION_H

/* ********************************************************************************************************************
 *  GLOBAL MISRA JUSTIFICATIONS
 *********************************************************************************************************************/
/* PRQA S 2880 EOF */ /* MD_DEM_2.1_UNREACHABLE_STATEMENT */
/* PRQA S 2982 EOF */ /* MD_DEM_2982 */
/* PRQA S 2983 EOF */ /* MD_DEM_2983 */
/* PRQA S 2985 EOF */ /* MD_DEM_2985 */
/* PRQA S 2986 EOF */ /* MD_DEM_2986 */
/* PRQA S 2987 EOF */ /* MD_DEM_2987 */
/* PRQA S 2991 EOF */ /* MD_DEM_2991 */
/* PRQA S 2992 EOF */ /* MD_DEM_2992 */
/* PRQA S 2995 EOF */ /* MD_DEM_2995 */
/* PRQA S 2996 EOF */ /* MD_DEM_2996 */
/* PRQA S 0759 EOF */ /* MD_MSR_Union */

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_Data_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_Infrastructure_Interface.h"
#include "Dem_FaultMemory_Interface.h"
#include "Dem_Core_Interface.h"
#include "Dem_RecordReader_Interface.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_DATA_IMPLEMENTATION_FILENAME "Dem_Data_Implementation.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DEFINITIONS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /*MD_MSR_MemMap */
/*!
 * \defgroup Dem_Data_PrivateProperties Private Properties
 * \{
 */

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

/*!
* \}
*/
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /*MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \addtogroup Dem_Data_Public
 * \{
 */

/* ****************************************************************************
 % Dem_Data_GetPtrToWriteIndexOfDestinationBuffer
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_SharedDataPtrType, DEM_CODE)
Dem_Data_GetPtrToWriteIndexOfDestinationBuffer(                                                                                  /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONST(Dem_Data_ConstDestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
  )
{
  return &(DestinationBuffer->Buffer[DestinationBuffer->WriteIndex]);
}

/* ****************************************************************************
 % Dem_Data_GetRemainingSizeOfDestinationBuffer
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(uint16, DEM_CODE)
Dem_Data_GetRemainingSizeOfDestinationBuffer(                                                                                    /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONST(Dem_Data_ConstDestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
  )
{
  uint16 lRemainingSize = 0U;

  if (DestinationBuffer->WriteIndex < DestinationBuffer->BufferSize)
  {
    lRemainingSize = (DestinationBuffer->BufferSize - DestinationBuffer->WriteIndex);
  }

  return lRemainingSize;
}

                                                         /* Permanent Memory */
/* ------------------------------------------------------------------------- */


/* ****************************************************************************
 % Dem_Data_InitDestinationBuffer
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Data_InitDestinationBuffer(                                                                                                  /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(Dem_SharedDataPtrType, AUTOMATIC) Buffer,
  CONST(uint16, AUTOMATIC) Size
  )
{
  DestinationBuffer->Buffer = Buffer;                                                                                            /* SBSW_DEM_CALL_DESTINATIONBUFFER_INIT */
  DestinationBuffer->BufferSize = Size;                                                                                          /* SBSW_DEM_CALL_DESTINATIONBUFFER_INIT */
  DestinationBuffer->WriteIndex = 0u;                                                                                            /* SBSW_DEM_CALL_DESTINATIONBUFFER_INIT */
}

/* ****************************************************************************
 % Dem_Data_ResetWriteIndexDestinationBuffer
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Data_ResetWriteIndexDestinationBuffer(                                                                                       /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
  )
{
  DestinationBuffer->WriteIndex = 0u;                                                                                            /* SBSW_DEM_DESTINATIONBUFFER_WRITEINDEX */
}

/* ****************************************************************************
 % Dem_Data_CheckAvailableSizeDestinationBuffer
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Data_CheckAvailableSizeDestinationBuffer(                                                                                    /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  CONST(Dem_Data_ConstDestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(uint16, AUTOMATIC) RequestedSize
  )
{
  Std_ReturnType lReturnValue;
  lReturnValue = E_OK;

# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (Dem_Data_GetRemainingSizeOfDestinationBuffer(DestinationBuffer) < RequestedSize)                                           /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  {
    Dem_Error_RunTimeCheckFailed(DEM_DATA_IMPLEMENTATION_FILENAME, __LINE__);                                                    /* SBSW_DEM_POINTER_RUNTIME_CHECK */
    lReturnValue = E_NOT_OK;
  }
# else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(DestinationBuffer)                                                                            /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(RequestedSize)                                                                                /* PRQA S 3112 */ /* MD_DEM_14.2 */
# endif
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_Data_IncrementWriteIndexDestinationBuffer
 *****************************************************************************/
/*!
* Internal comment removed.
 *
 *
*/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Data_IncrementWriteIndexDestinationBuffer(                                                                                   /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(uint16, AUTOMATIC) Size
)
{
  DestinationBuffer->WriteIndex += Size;                                                                                         /* SBSW_DEM_DESTINATIONBUFFER_WRITEINDEX */
}

/* ****************************************************************************
 % Dem_Data_WriteDestinationBuffer
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Data_WriteDestinationBuffer(                                                                                                 /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(Dem_ConstSharedDataPtrType, AUTOMATIC) SourceBuffer,
  CONST(uint16, AUTOMATIC) Size
  )
{
  if (Dem_Data_CheckAvailableSizeDestinationBuffer(DestinationBuffer, Size) == E_OK)                                             /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  {
    Dem_SharedDataPtrType lDestinationBufferPtr = Dem_Data_GetPtrToWriteIndexOfDestinationBuffer(DestinationBuffer);             /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    Dem_MemCpy(lDestinationBufferPtr, SourceBuffer, Size);                                                                       /* PRQA S 0311, 0315, 0602 */ /* MD_DEM_11.8, MD_MSR_VStdLibCopy, MD_DEM_20.2 */ /* SBSW_DEM_WRITE_DESTINATIONBUFFER */
    Dem_Data_IncrementWriteIndexDestinationBuffer(DestinationBuffer, Size);                                                      /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
}

/* ****************************************************************************
 % Dem_Data_WriteDestinationBufferAtIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Data_WriteDestinationBufferAtIndex(                                                                                          /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,                                                        /* PRQA S 3673 */ /* MD_DEM_8.13_fp */
  CONST(uint16, AUTOMATIC) WriteIndex,
  CONST(uint8, AUTOMATIC) Data
  )
{
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (WriteIndex >= DestinationBuffer->BufferSize)
  {
    Dem_Error_RunTimeCheckFailed(DEM_DATA_IMPLEMENTATION_FILENAME, __LINE__);                                                    /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    DestinationBuffer->Buffer[WriteIndex] = Data;                                                                                /* SBSW_DEM_WRITE_DESTINATIONBUFFER_AT_INDEX */
  }
}

/* ****************************************************************************
 % Dem_Data_WriteDestinationBufferUint8
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Data_WriteDestinationBufferUint8(                                                                                            /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(uint8, AUTOMATIC) Data
  )
{
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (Dem_Data_GetRemainingSizeOfDestinationBuffer(DestinationBuffer) < 1U)                                                      /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  {
    Dem_Error_RunTimeCheckFailed(DEM_DATA_IMPLEMENTATION_FILENAME, __LINE__);                                                    /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    DestinationBuffer->Buffer[DestinationBuffer->WriteIndex] = Data;                                                             /* SBSW_DEM_ARRAYWRITE_DESTINATIONBUFFER */
    Dem_Data_IncrementWriteIndexDestinationBuffer(DestinationBuffer, 1u);                                                        /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
}

/* ****************************************************************************
 % Dem_Data_WriteDestinationBufferUint16
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Data_WriteDestinationBufferUint16(                                                                                           /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(uint16, AUTOMATIC) Data
  )
{
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (Dem_Data_GetRemainingSizeOfDestinationBuffer(DestinationBuffer) < 2U)                                                      /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  {
    Dem_Error_RunTimeCheckFailed(DEM_DATA_IMPLEMENTATION_FILENAME, __LINE__);                                                    /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    DestinationBuffer->Buffer[DestinationBuffer->WriteIndex] = Dem_GetHiByte(Data);                                              /* SBSW_DEM_ARRAYWRITE_DESTINATIONBUFFER */
    DestinationBuffer->Buffer[DestinationBuffer->WriteIndex + 1u] = Dem_GetLoByte(Data);                                         /* SBSW_DEM_ARRAYWRITE_DESTINATIONBUFFER */
    Dem_Data_IncrementWriteIndexDestinationBuffer(DestinationBuffer, 2u);                                                        /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
}

/* ****************************************************************************
 % Dem_Data_WriteDestinationBufferUint16_Intel
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Data_WriteDestinationBufferUint16_Intel(                                                                                     /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(uint16, AUTOMATIC) Data
  )
{
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (Dem_Data_GetRemainingSizeOfDestinationBuffer(DestinationBuffer) < 2U)                                                      /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  {
    Dem_Error_RunTimeCheckFailed(DEM_DATA_IMPLEMENTATION_FILENAME, __LINE__);                                                    /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    DestinationBuffer->Buffer[DestinationBuffer->WriteIndex] = Dem_GetLoByte(Data);                                              /* SBSW_DEM_ARRAYWRITE_DESTINATIONBUFFER */
    DestinationBuffer->Buffer[DestinationBuffer->WriteIndex + 1u] = Dem_GetHiByte(Data);                                         /* SBSW_DEM_ARRAYWRITE_DESTINATIONBUFFER */
    Dem_Data_IncrementWriteIndexDestinationBuffer(DestinationBuffer, 2u);                                                        /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
}

/* ****************************************************************************
 % Dem_Data_WriteDestinationBufferUint32
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Data_WriteDestinationBufferUint32(                                                                                           /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(uint32, AUTOMATIC) Data
  )
{
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (Dem_Data_GetRemainingSizeOfDestinationBuffer(DestinationBuffer) < 4U)                                                      /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  {
    Dem_Error_RunTimeCheckFailed(DEM_DATA_IMPLEMENTATION_FILENAME, __LINE__);                                                    /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    DestinationBuffer->Buffer[DestinationBuffer->WriteIndex] = Dem_GetHiHiByte(Data);                                            /* SBSW_DEM_ARRAYWRITE_DESTINATIONBUFFER */
    DestinationBuffer->Buffer[DestinationBuffer->WriteIndex + 1u] = Dem_GetHiLoByte(Data);                                       /* SBSW_DEM_ARRAYWRITE_DESTINATIONBUFFER */
    DestinationBuffer->Buffer[DestinationBuffer->WriteIndex + 2u] = Dem_GetLoHiByte(Data);                                       /* SBSW_DEM_ARRAYWRITE_DESTINATIONBUFFER */
    DestinationBuffer->Buffer[DestinationBuffer->WriteIndex + 3u] = Dem_GetLoLoByte(Data);                                       /* SBSW_DEM_ARRAYWRITE_DESTINATIONBUFFER */
    Dem_Data_IncrementWriteIndexDestinationBuffer(DestinationBuffer, 4u);                                                        /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
}

/* ****************************************************************************
 % Dem_Data_WriteDestinationBufferUint32_Intel
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Data_WriteDestinationBufferUint32_Intel(                                                                                     /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(uint32, AUTOMATIC) Data
  )
{
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (Dem_Data_GetRemainingSizeOfDestinationBuffer(DestinationBuffer) < 4U)                                                      /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  {
    Dem_Error_RunTimeCheckFailed(DEM_DATA_IMPLEMENTATION_FILENAME, __LINE__);                                                    /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    DestinationBuffer->Buffer[DestinationBuffer->WriteIndex] = Dem_GetLoLoByte(Data);                                            /* SBSW_DEM_ARRAYWRITE_DESTINATIONBUFFER */
    DestinationBuffer->Buffer[DestinationBuffer->WriteIndex + 1u] = Dem_GetLoHiByte(Data);                                       /* SBSW_DEM_ARRAYWRITE_DESTINATIONBUFFER */
    DestinationBuffer->Buffer[DestinationBuffer->WriteIndex + 2u] = Dem_GetHiLoByte(Data);                                       /* SBSW_DEM_ARRAYWRITE_DESTINATIONBUFFER */
    DestinationBuffer->Buffer[DestinationBuffer->WriteIndex + 3u] = Dem_GetHiHiByte(Data);                                       /* SBSW_DEM_ARRAYWRITE_DESTINATIONBUFFER */
    Dem_Data_IncrementWriteIndexDestinationBuffer(DestinationBuffer, 4u);                                                        /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
}

/* ****************************************************************************
% Dem_Data_GetDestinationBufferWriteIndex
*****************************************************************************/
/*!
* Internal comment removed.
 *
 *
*/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Data_GetDestinationBufferWriteIndex(                                                                                         /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
CONST(Dem_Data_ConstDestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
)
{
  return DestinationBuffer->WriteIndex;
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_DATA_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_Data_Implementation.h
 *********************************************************************************************************************/
