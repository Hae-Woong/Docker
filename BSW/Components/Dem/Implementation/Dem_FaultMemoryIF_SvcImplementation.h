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
/*!
 *  \defgroup   Dem_FaultMemoryIF Dem FaultMemoryIF Functionality
 *  \{
 *  \file       Dem_FaultMemoryIF_SvcImplementation.h
 *  \brief      Svc of unit FaultMemoryIF.
 *  \details    Realizes the public interfaces of its parent logical unit, i.e. both Dem external service APIs and Dem internal APIs.
 *  \entity     FaultMemoryIF
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_FAULTMEMORYIF_SVCIMPLEMENTATION_H)
#define DEM_FAULTMEMORYIF_SVCIMPLEMENTATION_H

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
/* PRQA S 3415 EOF */ /* MD_DEM_13.5_cf */

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                    /* Used subcomponent API */
/* ------------------------------------------------------------------------- */
#include "Dem_PrestorageMemory_Interface.h"
#include "Dem_Memories_Interface.h"
#include "Dem_MemoryEntry_Interface.h"

#include "Dem_Infrastructure_Interface.h"
#include "Dem_Core_Interface.h"
                                                   /* Subcomponents callbacks*/
/* ------------------------------------------------------------------------- */

/* ********************************************************************************************************************
 *  FILE SPECIFIC MISRA VIOLATIONS
 *********************************************************************************************************************/
/* PRQA S 3406 DEM_API_IMPL */ /* MD_DEM_8.6 */


/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

  /*! Filename declaration */
#define DEM_FAULTMEMORYIF_SVCIMPLEMENTATION_FILENAME "Dem_FaultMemoryIF_SvcImplementation.h"

 /* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \addtogroup Dem_FaultMemoryIF_Public
 * \{
 */

/* ****************************************************************************
 % Dem_PrestoreFreezeFrame
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_PrestoreFreezeFrame(
  Dem_EventIdType  EventId                                                                                                       /* PRQA S 1330 */ /* MD_DEM_8.3_rte */
  )
{
  Std_ReturnType lReturnValue;
  Dem_Det_ErrorRegisterDefine()

  lReturnValue = E_NOT_OK;

  Dem_Det_ErrorRegisterSet(DEM_E_NO_ERROR)

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dem_APIChecks_GlobalConfigPointerInState(DEM_CONFIGVALIDATION_CONFIGPTR_INITIALIZED) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_UNINIT)
  }
  else
  if (Dem_APIChecks_EventId(EventId) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_DATA)
  }
  else
  if (Dem_APIChecks_VerifyNotSlaveEvent(EventId) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_DATA)
  }
  else
  {
  /* ----- Implementation -------------------------------------------------- */
    lReturnValue = Dem_Memories_PrestorageMemory_EventPrestoreFreezeFrame(EventId);

    Dem_Memories_PrestorageMemory_MultiEvent_PrestoreFreezeFrame(EventId);
  }

  /* ----- Development Error Report ---------------------------------------- */
  if (Dem_Det_ErrorRegisterGet() != DEM_E_NO_ERROR)
  {
    Dem_Error_ReportError(DEM_PRESTOREFREEZEFRAME_APIID, Dem_Det_ErrorRegisterGet());
  }
  return lReturnValue;
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */

/* ****************************************************************************
 % Dem_GetFreezeFramePrestored
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_GetFreezeFramePrestored(
  Dem_EventIdType  EventId,
  P2VAR(boolean, AUTOMATIC, DEM_APPL_DATA) FreezeFramePrestored
)
{
  Std_ReturnType lReturnValue;
  Dem_Det_ErrorRegisterDefine()

  lReturnValue = E_NOT_OK;

  Dem_Det_ErrorRegisterSet(DEM_E_NO_ERROR)

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dem_APIChecks_GlobalConfigPointerInState(DEM_CONFIGVALIDATION_CONFIGPTR_INITIALIZED) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_UNINIT)
  }
  else
  if (Dem_APIChecks_MasterInState(DEM_INITSTATE_INITIALIZED) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_UNINIT)
  }
  else
  if (Dem_APIChecks_EventId(EventId) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_DATA)
  }
  else
  if (Dem_APIChecks_NullPointer(FreezeFramePrestored) == FALSE)                                                                  /* SBSW_DEM_POINTER_NULLPTR_CHECKING */  /* PRQA S 0315 */ /* MD_DEM_Dir1.1 */
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_POINTER)
  }
  else
  if (Dem_APIChecks_EventApplicationIdEqualsMaster(EventId) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_DATA)
  }
  else
  if (Dem_APIChecks_IsMasterPartition() == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  /* ----- Global Precondition Checks -------------------------------------- */
  if (Dem_Cfg_EventPrestorageIndex(EventId) == DEM_PRESTORAGEMEMORY_NOT_CONFIGURED_FOR_EVENT)
  {
    lReturnValue = E_NOT_OK;
  }
  else
  if (Dem_Event_TestEventAvailable(EventId) == FALSE)
  {
    lReturnValue = E_NOT_OK;
  }
  else
  {
  /* ----- Implementation -------------------------------------------------- */
    *FreezeFramePrestored = Dem_Memories_PrestorageMemory_TestEventFreezeFramePrestored(EventId);                                /* SBSW_DEM_POINTER_WRITE_API */
    lReturnValue = E_OK;
  }

  /* ----- Development Error Report ---------------------------------------- */
  if (Dem_Det_ErrorRegisterGet() != DEM_E_NO_ERROR)
  {
    Dem_Error_ReportError(DEM_GETFREEZEFRAMEPRESTORED_APIID, Dem_Det_ErrorRegisterGet());
  }
  return lReturnValue;
}                                                                                                                                /* PRQA S 6030, 6050, 6080 */ /* MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

/* ****************************************************************************
 % Dem_ClearPrestoredFreezeFrame
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_ClearPrestoredFreezeFrame(
  Dem_EventIdType  EventId                                                                                                       /* PRQA S 1330 */ /* MD_DEM_8.3_rte */
  )
{
  Std_ReturnType lReturnValue;
  Dem_Det_ErrorRegisterDefine()

  lReturnValue = E_NOT_OK;

  Dem_Det_ErrorRegisterSet(DEM_E_NO_ERROR)

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dem_APIChecks_GlobalConfigPointerInState(DEM_CONFIGVALIDATION_CONFIGPTR_INITIALIZED) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_UNINIT)
  }
  else
  if (Dem_APIChecks_EventId(EventId) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_DATA)
  }
  else
  if (Dem_APIChecks_VerifyNotSlaveEvent(EventId) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_DATA)
  }
  else
  {
  /* ----- Implementation -------------------------------------------------- */
    lReturnValue = Dem_Memories_PrestorageMemory_EventClearPrestoredFreezeFrame(EventId);

    Dem_Memories_PrestorageMemory_MultiEvent_ClearPrestoredFreezeFrame(EventId);
  }

  /* ----- Development Error Report ---------------------------------------- */
  if (Dem_Det_ErrorRegisterGet() != DEM_E_NO_ERROR)
  {
    Dem_Error_ReportError(DEM_CLEARPRESTOREDFREEZEFRAME_APIID, Dem_Det_ErrorRegisterGet());
  }
  return lReturnValue;
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */

                                /* Service Interface EvMemOverflowIndication */
/* ------------------------------------------------------------------------- */

#if (DEM_CFG_SUPPORT_OVFLIND_API == STD_ON)
/* ****************************************************************************
 % Dem_GetEventMemoryOverflow
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_GetEventMemoryOverflow(
  uint8  ClientId,                                                                                                               /* PRQA S 1330 2 */ /* MD_DEM_8.3_rte */
  Dem_DTCOriginType  DTCOrigin,
  P2VAR(boolean, AUTOMATIC, DEM_APPL_DATA)  OverflowIndication                                                                   /* PRQA S 3673 */ /* MD_DEM_8.13_fp */
  )
{
  Std_ReturnType lReturnValue;
  Dem_Det_ErrorRegisterDefine()

  lReturnValue = E_NOT_OK;

  Dem_Det_ErrorRegisterSet(DEM_E_NO_ERROR)

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dem_APIChecks_MasterInState(DEM_INITSTATE_INITIALIZED) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_UNINIT)
  }
  else
  if (Dem_APIChecks_ClientId(ClientId) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_DATA)
  }
  else
  if (Dem_APIChecks_DTCOrigin(DTCOrigin) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_DATA)
  }
  else
  if (Dem_APIChecks_NullPointer(OverflowIndication) == FALSE)                                                                    /* SBSW_DEM_POINTER_NULLPTR_CHECKING */  /* PRQA S 0315 */ /* MD_DEM_Dir1.1 */
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_POINTER)
  }
  else
  if (Dem_APIChecks_IsMasterPartition() == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    lReturnValue = Dem_FaultMemory_Memories_GetOverflowOfDTCOrigin(DTCOrigin, OverflowIndication);                               /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }

  /* ----- Development Error Report ---------------------------------------- */
  if (Dem_Det_ErrorRegisterGet() != DEM_E_NO_ERROR)
  {
    Dem_Error_ReportError(DEM_GETEVENTMEMORYOVERFLOW_APIID, Dem_Det_ErrorRegisterGet());
  }
  return lReturnValue;
}                                                                                                                                /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
#endif /* (DEM_CFG_SUPPORT_OVFLIND_API == STD_ON) */

#if (DEM_CFG_SUPPORT_OVFLIND_API == STD_ON)
/* ****************************************************************************
 % Dem_GetNumberOfEventMemoryEntries
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_GetNumberOfEventMemoryEntries(
  uint8  ClientId,                                                                                                               /* PRQA S 1330 2 */ /* MD_DEM_8.3_rte */
  Dem_DTCOriginType  DTCOrigin,
  P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA)  NumberOfEventMemoryEntries
  )
{
  Std_ReturnType lReturnValue;
  Dem_Det_ErrorRegisterDefine()

  lReturnValue = E_NOT_OK;
  Dem_Det_ErrorRegisterSet(DEM_E_NO_ERROR)

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dem_APIChecks_MasterInState(DEM_INITSTATE_INITIALIZED) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_UNINIT)
  }
  else
  if (Dem_APIChecks_ClientId(ClientId) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_DATA)
  }
  else
  if (Dem_APIChecks_DTCOrigin(DTCOrigin) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_DATA)
  }
  else
  if (Dem_APIChecks_NullPointer(NumberOfEventMemoryEntries) == FALSE)                                                            /* SBSW_DEM_POINTER_NULLPTR_CHECKING */  /* PRQA S 0315 */ /* MD_DEM_Dir1.1 */
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_POINTER)
  }
  else
  if (Dem_APIChecks_IsMasterPartition() == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    uint16 lTranslatedMemoryId;

    if (Dem_FaultMemory_Memories_TranslateDTCOrigin(DTCOrigin, &lTranslatedMemoryId) == E_OK)                                    /* SBSW_DEM_POINTER_FORWARD_STACK */
    {
      *NumberOfEventMemoryEntries = Dem_FaultMemory_Memories_GetNumAllocatedMemoryEntry(lTranslatedMemoryId);                    /* SBSW_DEM_POINTER_WRITE_API */
      lReturnValue = E_OK;
    }
  }

  /* ----- Development Error Report ---------------------------------------- */
  if (Dem_Det_ErrorRegisterGet() != DEM_E_NO_ERROR)
  {
    Dem_Error_ReportError(DEM_GETNUMBEROFEVENTMEMORYENTRIES_APIID, Dem_Det_ErrorRegisterGet());
  }
  return lReturnValue;
}                                                                                                                                /* PRQA S 6030, 6050, 6080 */ /* MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
#endif /* (DEM_CFG_SUPPORT_OVFLIND_API == STD_ON) */


                                             /* Permanent Storage Activation */
/* ------------------------------------------------------------------------- */
#if (DEM_CFG_SUPPORT_PERMANENTDTC_ACTIVATION_DISTANCE == STD_ON)
/* ****************************************************************************
 % Dem_EnablePermanentStorage
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_EnablePermanentStorage(
  void
  )
{
  Std_ReturnType lReturnValue;
  Dem_Det_ErrorRegisterDefine()

  lReturnValue = E_NOT_OK;
  Dem_Det_ErrorRegisterSet(DEM_E_NO_ERROR)

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dem_APIChecks_MasterInState(DEM_INITSTATE_INITIALIZED) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_UNINIT)
  }
  else
  if (Dem_APIChecks_IsObdIIOrObdOnUdsSupportedInVariant() == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  if (Dem_APIChecks_IsMasterPartition() == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  {
    lReturnValue = Dem_Memories_PermanentMemory_EnableStorage();
  }

  /* ----- Development Error Report ---------------------------------------- */
  if (Dem_Det_ErrorRegisterGet() != DEM_E_NO_ERROR)
  {
    Dem_Error_ReportError(DEM_ENABLEPERMANENTSTORAGE_APIID, Dem_Det_ErrorRegisterGet());
  }
  return lReturnValue;
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
#endif /* (DEM_CFG_SUPPORT_PERMANENTDTC_ACTIVATION_DISTANCE == STD_ON) */

#if (DEM_CFG_SUPPORT_PERMANENTDTC_ACTIVATION_DISTANCE == STD_ON)
/* ****************************************************************************
 % Dem_GetPermanentStorageState
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_GetPermanentStorageState(
  P2VAR(boolean, AUTOMATIC, DEM_APPL_DATA)  isEnabled                                                                            /* PRQA S 1330 */ /* MD_DEM_8.3_rte */
  )
{
  Std_ReturnType lReturnValue;
  Dem_Det_ErrorRegisterDefine()

  lReturnValue = E_NOT_OK;

  Dem_Det_ErrorRegisterSet(DEM_E_NO_ERROR)

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dem_APIChecks_MasterInState(DEM_INITSTATE_INITIALIZED) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_UNINIT)
  }
  else
  if (Dem_APIChecks_IsObdIIOrObdOnUdsSupportedInVariant() == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  if (Dem_APIChecks_NullPointer(isEnabled) == FALSE)                                                                             /* SBSW_DEM_POINTER_NULLPTR_CHECKING */  /* PRQA S 0315 */ /* MD_DEM_Dir1.1 */
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_POINTER)
  }
  else
  if (Dem_APIChecks_IsMasterPartition() == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    if (Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant())
    {
      *isEnabled = (boolean)(Dem_Memories_PermanentMemory_TestEnabled() == TRUE);                                                /* SBSW_DEM_POINTER_WRITE_API */
      lReturnValue = E_OK;
    }
  }

  /* ----- Development Error Report ---------------------------------------- */
  if (Dem_Det_ErrorRegisterGet() != DEM_E_NO_ERROR)
  {
    Dem_Error_ReportError(DEM_GETPERMANENTSTORAGESTATE_APIID, Dem_Det_ErrorRegisterGet());
  }
  return lReturnValue;
}                                                                                                                                /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif /* (DEM_CFG_SUPPORT_PERMANENTDTC_ACTIVATION_DISTANCE == STD_ON) */



/* ****************************************************************************
 % Dem_NvM_InitObdFreezeFrameData
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_NvM_InitObdFreezeFrameData(
  void
  )
{
  Dem_Det_ErrorRegisterDefine()

  Dem_Det_ErrorRegisterSet(DEM_E_NO_ERROR)

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dem_APIChecks_MasterNotInState(DEM_INITSTATE_INITIALIZED) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  {
    uint8 lFreezeFrameIndex;

    /* ----- Implementation ------------------------------------------------ */
    lFreezeFrameIndex = Dem_Memories_ObdFreezeFrameMemory_GetSize();

    while (lFreezeFrameIndex != 0U)                                                                                              /* PRQA S 2994 */ /* MD_DEM_2994 */
    {
      lFreezeFrameIndex--;
      Dem_MemoryEntry_ObdFreezeFrameEntry_Free(lFreezeFrameIndex);
    }
  }

  /* ----- Development Error Report ---------------------------------------- */
  if (Dem_Det_ErrorRegisterGet() != DEM_E_NO_ERROR)
  {
    Dem_Error_ReportError(DEM_NVMINIT_APIID, Dem_Det_ErrorRegisterGet());
  }


  /* The DEM expects that the NVM by itself marks the NVRAM block as dirty */
  return E_OK;
}

#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/* ****************************************************************************
 % Dem_NvM_InitObdPermanentData
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_NvM_InitObdPermanentData(
  void
  )
{
  /* ----- Development Error Checks ---------------------------------------- */

  /* Do not check for preinitialization phase */

  /* ----- Implementation -------------------------------------------------- */
  uint8 lPermanentIndex;

  lPermanentIndex = Dem_Cfg_GlobalPermanentMaxCount();

  while(lPermanentIndex != 0u)
  {
    --lPermanentIndex;
    Dem_MemoryEntry_PermanentEntry_SetEventId(lPermanentIndex, DEM_EVENT_INVALID);
    Dem_MemoryEntry_PermanentEntry_SetStatus(lPermanentIndex, DEM_ESM_PERMANENT_NONE);
  }

  Dem_Memories_PermanentMemory_SetInitPattern(DEM_PERMANENTMEMORY_INIT_PATTERN);

  /* ----- Development Error Report ---------------------------------------- */

  /* The DEM expects that the NVM by itself marks the NVRAM block as dirty */
  return E_OK;
}
#endif /* (DEM_CFG_SUPPORT_PERMANENT == STD_ON) */


#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_OBDII_OR_OBDONUDS == STD_ON)
/* ****************************************************************************
 % Dem_DcmGetDTCOfOBDFreezeFrame
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_DcmGetDTCOfOBDFreezeFrame(
  uint8  FrameNumber,
  P2VAR(uint32, AUTOMATIC, DEM_DCM_DATA)  DTC,                                                                                   /* PRQA S 3206, 3673 */ /* MD_DEM_3206, MD_DEM_8.13 */
  Dem_DTCFormatType  DTCFormat
  )
{
  Std_ReturnType lReturnValue;
  Dem_Det_ErrorRegisterDefine()

  lReturnValue = E_NOT_OK;
  Dem_Det_ErrorRegisterSet(DEM_E_NO_ERROR)

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dem_APIChecks_MasterInState(DEM_INITSTATE_INITIALIZED) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_UNINIT)
  }
  else
  if (Dem_APIChecks_IsOBDIILegislationEnabledInVariant() == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  if (Dem_APIChecks_NullPointer(DTC) == FALSE)                                                                                   /* SBSW_DEM_POINTER_NULLPTR_CHECKING */  /* PRQA S 0315 */ /* MD_DEM_Dir1.1 */
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_POINTER)
  }
  else
  if (Dem_APIChecks_IsDTCFormatValid(DTCFormat, Dem_DTCFormat_Context_GetDTCOfOBDFF) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_DATA)
  }
  else
# if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (FrameNumber != 0x00u)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_DATA)
  }
  else  
# endif
  if (Dem_APIChecks_IsMasterPartition() == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  {
    if (Dem_Cfg_IsObdIIExclusivelySupportedInVariant() == TRUE)
    {
# if (DEM_CFG_SUPPORT_OBDII == STD_ON)
      uint8 lSelectedIndex;
      Dem_EventIdType lEventId;

      /* ----- Implementation ------------------------------------------------ */
      /* copy Mode02 index on the stack as it might change in the background */
      lSelectedIndex = Dem_Memories_ObdFreezeFrameMemory_GetFreezeFrameIndexMode02();

      if (lSelectedIndex < Dem_Memories_ObdFreezeFrameMemory_GetSize())
      {
        lEventId = Dem_MemoryEntry_ObdFreezeFrameEntry_GetEventId(lSelectedIndex);

        if ( (Dem_Event_TestValidHandle(lEventId) == TRUE)
          && (Dem_DTC_TestDtcSuppression(lEventId) == FALSE) )                                                                   /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
        {
          lReturnValue = Dem_DTC_GetFormatedDTCByEvent(lEventId, DTCFormat, DTC);                                                /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
        }
      }
# endif

      /* else No Freeze Frame stored, return value lReturnValue = E_NOT_OK initially set */
    }
  }

  /* ----- Development Error Report ---------------------------------------- */
  if (Dem_Det_ErrorRegisterGet() != DEM_E_NO_ERROR)
  {
    Dem_Error_ReportError(DEM_DCMGETDTCOFOBDFREEZEFRAME_APIID, Dem_Det_ErrorRegisterGet());
  }

  DEM_IGNORE_UNUSED_ARGUMENT(FrameNumber)                                                                                        /* PRQA S 1338, 3112 */ /*  MD_MSR_DummyStmt, MD_DEM_14.2 */
  return lReturnValue;
}                                                                                                                                /* PRQA S 6030, 6050, 6080 */ /* MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
#endif

#if (DEM_FEATURE_NEED_CUSTOMTRIGGERSTORAGE == STD_ON)
/* ****************************************************************************
 % Dem_StoreCustomTriggeredFreezeFrame
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_StoreCustomTriggeredFreezeFrame(
  Dem_EventIdType  EventId                                                                                                       /* PRQA S 1330 */ /* MD_DEM_8.3_rte */
  )
{
  Std_ReturnType lReturnValue;
  Dem_Det_ErrorRegisterDefine()

  lReturnValue = E_NOT_OK;

  Dem_Det_ErrorRegisterSet(DEM_E_NO_ERROR)

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dem_APIChecks_GlobalConfigPointerInState(DEM_CONFIGVALIDATION_CONFIGPTR_INITIALIZED) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_UNINIT)
  }
  else
  if (Dem_APIChecks_MasterInState(DEM_INITSTATE_INITIALIZED) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_UNINIT)
  }
  else
  if (Dem_APIChecks_EventId(EventId) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_DATA)
  }
  else
  if (Dem_APIChecks_SatelliteInState(Dem_Cfg_EventSatelliteId(EventId), DEM_INITSTATE_INITIALIZED) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_UNINIT)
  }
  else
  if (Dem_APIChecks_EventApplicationId(EventId) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_DATA)
  }
  else
  {
    /* ----- Global Precondition Checks For EventId ------------------------ */
    if ( (Dem_Cfg_EventUdsDtc(EventId) != DEM_CFG_DTC_UDS_INVALID)
      && (Dem_Event_TestEventAvailable(EventId) == TRUE)
      && (Dem_Core_TestCustomTriggerConfigured(EventId) == TRUE))
    {
      /* ----- Implementation -------------------------------------------------- */
      Dem_Memories_CustomTriggerMemory_StoreCustomTriggeredFreezeFrame(EventId);
      lReturnValue = E_OK;
    }
  }

  /* ----- Development Error Report ---------------------------------------- */
  if (Dem_Det_ErrorRegisterGet() != DEM_E_NO_ERROR)
  {
    Dem_Error_ReportError(DEM_STORECUSTOMTRIGGEREDFREEZEFRAME_APIID, Dem_Det_ErrorRegisterGet());
  }
  return lReturnValue;
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
#endif

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  FILE SPECIFIC MISRA VIOLATIONS
 *********************************************************************************************************************/
/* PRQA L:DEM_API_IMPL */


#endif /* DEM_FAULTMEMORYIF_SVCIMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_FaultMemoryIF_SvcImplementation.h
 *********************************************************************************************************************/
