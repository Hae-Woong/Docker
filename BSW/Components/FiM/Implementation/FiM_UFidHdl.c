/***********************************************************************************************************************
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  --------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/*!        \file     FiM_UFidHdl.c
 *         \unit     UFidHdl
 *         \brief    Source for Autosar 4 module FiM - Unit FID Handling
 *
 *         \details  Static source file for FiM. May not be changed by customer / generator
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  
 *  see FiM.h
 *  
 **********************************************************************************************************************/

#define FIM_SOURCE

/* *********************************************************************************************************************
 *  GLOBAL MISRA Justifications
 **********************************************************************************************************************/


/***********************************************************************************************************************
*  INCLUDES
***********************************************************************************************************************/

#include "Dem.h"
#include "FiM.h"
#include "FiM_Int.h"

#include "FiM_ErrorCheck.h"
#include "FiM_LHelper.h"
#include "FiM_UFidHdl.h"
#include "FiM_UGeneral.h"
#include "FiM_UInhStatHdl.h"
#include "FiM_UPendCntHdl.h"

#include "FiM_Cfg_FidHdl.h"


/***********************************************************************************************************************
*  LOCAL CONSTANT MACROS
***********************************************************************************************************************/


/***********************************************************************************************************************
*  LOCAL FUNCTION MACROS
***********************************************************************************************************************/


/*! Return number of configured FIDs. One less than size of FID table (last element is dummy element).                */
#define FiM_UFidHdl_GetNrOfFids()                                  (FiM_GetSizeOfFidTable() - 1U)
/*! Return index into inhibition code table at particular position in fid list (macro is easier to read).             */
#define FiM_UFidHdl_GetInhCodeIdx_FidToInhCodeTable                FiM_GetInhCodeTableIdxOfFidToInhCodeTable
/*! Return sat id for inhibition code table at particular position in fid list (macro is easier to read).             */
#define FiM_UFidHdl_GetSatId_FidToInhSatIdTable                    FiM_GetSatelliteInfoTableIdxOfFidToInhSatIdTable


/***********************************************************************************************************************
*  LOCAL DATA TYPES AND STRUCTURES
***********************************************************************************************************************/


/***********************************************************************************************************************
*  LOCAL DATA PROTOTYPES
***********************************************************************************************************************/



/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define FIM_START_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


#define FIM_STOP_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/


/**********************************************************************************************************************/
/* FiM_UFidHdl Start *******************************************************************************************/
/**********************************************************************************************************************/

#define FIM_START_SEC_VAR_NO_INIT_16
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (FIM_CYCLIC_FID_CALCULATION == STD_ON)
/*! For cyclic FID calculation where not all FIDs are scheduled per cycle,
this variable holds the FID between two cycles. */
FIM_LOCAL VAR(FiM_FunctionIdType, FIM_VAR_NO_INIT)         FiM_UFidHdl_NextFidToSchedule;
#endif

#define FIM_STOP_SEC_VAR_NO_INIT_16
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


#define FIM_START_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  private FiM_UFidHdl_IsFidInhibited()
 **********************************************************************************************************************/
/*! \brief                    Internal helper function to calculate whether a specific FID is inhibited.
 *
 *  \details                  Checks all inhibitions states connected to FID.
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(boolean, FIM_CODE)  FiM_UFidHdl_IsFidInhibited(FiM_FunctionIdType FID);


#if (FIM_FEATURE_OBD == STD_ON)
/***********************************************************************************************************************
 *  private FiM_UFidHdl_IsFidPendingLocked()
 **********************************************************************************************************************/
/*! \brief                    Internal helper function to calculate whether a specific FID is pending locked.
 *
 *  \details                  Checks all pending states connected to FID.
 *  \param[in]  FID           Function identifier.
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(boolean, FIM_CODE)  FiM_UFidHdl_IsFidPendingLocked(FiM_FunctionIdType FID);
#endif


/***********************************************************************************************************************
 *  external public FiM_UFidHdl::FiM_GetFunctionPermission()
 **********************************************************************************************************************/
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
FUNC(Std_ReturnType, FIM_CODE)  FiM_GetFunctionPermission                             /* PRQA S 0624 */ /* MD_FiM_8.3 */
                                 (FiM_FunctionIdType FID,                             /* PRQA S 1330 */ /* MD_FiM_8.3 */
                                  P2VAR(boolean, AUTOMATIC, FIM_APPL_DATA) Permission
                                 )                                                                     /*lint -e{529} */
{
  uint8             errorId = FIM_E_NO_ERROR;                                         /* PRQA S 2981 */ /* MD_FiM_2.2 */
  Std_ReturnType    retCode = E_NOT_OK;                                               /* PRQA S 2981 */ /* MD_FiM_2.2 */
  uint16            fid16   = FID; /* avoid warning when exactly 256 FIDs are configured */

  /* #010 if out parameter Permission is NULL_PTR set corresponding DET code */
  if (FiM_ErrorCheck_IsDevErrorDetected(Permission == NULL_PTR) == TRUE) /* PRQA S 2992, 2996, 4404 */ /* MD_FiM_ConfCheck, MD_FiM_ConfCheck, MD_MSR_AutosarBoolean */
  {
    /* (FIM_DEV_ERROR_DETECT == STD_ON) && (Permission == NULL_PTR) */
    errorId = FIM_E_PARAM_POINTER;                                                /* PRQA S 2983 */ /* MD_FiM_2.2conf */
  }
  /* #020 otherwise if FIM satellites are not initialized set corresponding DET code (SPEC-31915) */
  else if (FiM_ErrorCheck_IsDevErrorDetected(FiM_UGeneral_GetInitializationStatusSatCumulated() != FIM_INITIALIZED) == TRUE) /* PRQA S 2992, 2996, 4404 */ /* MD_FiM_ConfCheck, MD_FiM_ConfCheck, MD_MSR_AutosarBoolean */
  {
    /* (FIM_DEV_ERROR_DETECT == STD_ON) && (FiM_UGeneral_GetInitializationStatusSatCumulated() != FIM_INITIALIZED) */
    errorId = FIM_E_UNINIT;                                                       /* PRQA S 2983 */ /* MD_FiM_2.2conf */
    *Permission = FALSE;                                                                   /* SBSW_FIM_PERMISSION_PTR */
  }
  /* #030 otherwise if FID is out of range set corresponding DET code (SPEC-31912) */
  else if (FiM_ErrorCheck_IsDevErrorDetected(fid16 >= FiM_UFidHdl_GetNrOfFids()) == TRUE) /* PRQA S 2992, 2996, 4404 */ /* MD_FiM_ConfCheck, MD_FiM_ConfCheck, MD_MSR_AutosarBoolean */
  {
    /* (FIM_DEV_ERROR_DETECT == STD_ON) && (fid16 >= FiM_UFidHdl_GetNrOfFids()) */
    errorId = FIM_E_FID_OUT_OF_RANGE;                                             /* PRQA S 2983 */ /* MD_FiM_2.2conf */
    *Permission = FALSE;                                                                   /* SBSW_FIM_PERMISSION_PTR */
  }
  /* #040 otherwise return permission status of FID (SPEC-31914) */
  else    /* (errorId == FIM_E_NO_ERROR) */
  {
#if (FIM_CYCLIC_FID_CALCULATION == STD_ON)
    {
      /* #050 if cyclic FID calculation is configured return pre-calculated (in FiM_MainFunction) permission status */
      boolean permStat = FiM_UBitfieldHdl_TestBitInBitfield(FiM_GetAddrPermStatusTable(0), FID);               /* SBSW_FIM_BITTEST_CALL */
      *Permission = permStat;                                                                /* SBSW_FIM_PERMISSION_PTR */
    }
#else
    {
      /* #060 otherwise calculate permission status from inhibition states */
      boolean inhibStat = FiM_UFidHdl_IsFidInhibited(FID);
      *Permission = (boolean)(!inhibStat); /* PRQA S 4116, 4304, 4558 */ /* MD_MSR_AutosarBoolean */ /* SBSW_FIM_PERMISSION_PTR */
    }
#endif
    retCode = E_OK;
  }

  FiM_ErrorCheck_ReportError(FIM_SID_FIM_GETFUNCTIONPERMISSION, errorId);

  return retCode;
}


#if (FIM_FEATURE_OBD == STD_ON)
/***********************************************************************************************************************
 *  external public FiM_UFidHdl::FiM_GetFunctionPendingStatus()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, FIM_CODE)  FiM_GetFunctionPendingStatus
                                 (FiM_FunctionIdType FID,
                                  P2VAR(boolean, AUTOMATIC, FIM_DEM_DATA) pendingStatus
                                 )                                                                     /*lint -e{529} */
{
  uint8             errorId = FIM_E_NO_ERROR;                   /* PRQA S 2981, 2983 */ /* MD_FiM_2.2, MD_FiM_2.2conf */
  Std_ReturnType    retCode = E_NOT_OK;                                               /* PRQA S 2981 */ /* MD_FiM_2.2 */

  /* avoid warning "comparison is always false due to limited range of data type" if there are exactly 256 FIDs       */
  uint16            fid16   = FID;
  /* #10 if out parameter pendingStatus is NULL_PTR set corresponding DET code */
  if (FiM_ErrorCheck_IsDevErrorDetected(pendingStatus == NULL_PTR) == TRUE) /* PRQA S 2992, 2996, 4404 */ /* MD_FiM_ConfCheck, MD_FiM_ConfCheck, MD_MSR_AutosarBoolean */
  {
    /* (FIM_DEV_ERROR_DETECT == STD_ON) && (pendingStatus == NULL_PTR) */
    errorId = FIM_E_PARAM_POINTER;                                                /* PRQA S 2983 */ /* MD_FiM_2.2conf */
  }
  /* #20 otherwise if FIM is not initialized set corresponding DET code */
  else if (FiM_ErrorCheck_IsDevErrorDetected(FiM_UGeneral_GetInitializationStatusMaster() != FIM_INITIALIZED) == TRUE) /* PRQA S 2992, 2996, 4404 */ /* MD_FiM_ConfCheck, MD_FiM_ConfCheck, MD_MSR_AutosarBoolean */
  {
    /* (FIM_DEV_ERROR_DETECT == STD_ON) && (FiM_UGeneral_GetInitializationStatusMaster() != FIM_INITIALIZED) */
    errorId = FIM_E_UNINIT;                                                       /* PRQA S 2983 */ /* MD_FiM_2.2conf */
  }
  /* #30 otherwise if FID is out of range set corresponding DET code */
  else if (FiM_ErrorCheck_IsDevErrorDetected(fid16 >= FiM_GetSizeOfFidPendingCounter()) == TRUE) /* PRQA S 2992, 2996, 4404 */ /* MD_FiM_ConfCheck, MD_FiM_ConfCheck, MD_MSR_AutosarBoolean */
  {
    /* (FIM_DEV_ERROR_DETECT == STD_ON) && (fid16 >= FiM_GetSizeOfFidPendingCounter()) */
    errorId = FIM_E_FID_OUT_OF_RANGE;                                             /* PRQA S 2983 */ /* MD_FiM_2.2conf */
  }
  /* #40 otherwise return pending status of FID */
  else    /* (errorId == FIM_E_NO_ERROR) */
  {
    *pendingStatus = FiM_UFidHdl_IsFidPendingLocked(FID);                            /* SBSW_FIM_PENDINGSTATUS_PTR */
    retCode = E_OK;
  }

  FiM_ErrorCheck_ReportError(FIM_SID_FIM_GETFUNCTIONPENDINGSTATUS, errorId);

  return retCode;
}
#endif


/**********************************************************************************************************************
 *  public FiM_UFidHdl_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, FIM_CODE)  FiM_UFidHdl_Init(void)
{
# if (FIM_CYCLIC_FID_CALCULATION == STD_ON)
  {
    FiM_PermStatusTableIterType  permStatIter  = 0;
    for (permStatIter = 0; permStatIter < FiM_GetSizeOfPermStatusTable(); permStatIter++)
    {
      /* default is "not permitted" until first calculation                       */
      /* reasoning: before completion of initialization it's also "not permitted" */
      FiM_SetPermStatusTable(permStatIter, 0);                                          /* SBSW_FIM_PERMSTATUS_ACCESS */
    }
#if (FIM_AFFECTED_FIDS_IDENTIFICATION == STD_ON)
    {
      FiM_FidEventChangedTableIterType  evChgdIter  = 0;
      for (evChgdIter = 0; evChgdIter < FiM_GetSizeOfFidEventChangedTable(); evChgdIter++)
      {
        /* default is "changed" until first calculation                             */
        /* reasoning: the FIDs need calculation                                     */
        FiM_SetFidEventChangedTable(evChgdIter, ~(FiM_ProcessorWordType)0);              /* SBSW_FIM_EVCHANGED_ACCESS */
      }
    }
#endif
    FiM_UFidHdl_NextFidToSchedule = 0;
  }
#endif
}


/**********************************************************************************************************************
 *  public FiM_UFidHdl_CalcPermissionStates()
 *********************************************************************************************************************/
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
*/
FUNC(void, FIM_CODE)  FiM_UFidHdl_CalcPermissionStates(void)
{
#if (FIM_CYCLIC_FID_CALCULATION == STD_ON)
  {
  /* there is at least one FID */
  /* #010 set starting point of iteration to where we stopped last time */
  const uint32  cFidStart = FiM_UFidHdl_NextFidToSchedule;
        uint32  fidIter   = FiM_UFidHdl_NextFidToSchedule;
        uint32  fidCount  = 0;
  /* #020 loop maximum through the number of FIDs that are configured per scheduling slice */
  for (fidCount = 0; fidCount < FiM_GetMaxHandledFidsPerCycle(); fidCount++)
  {
#if (FIM_AFFECTED_FIDS_IDENTIFICATION == STD_ON)
    /* #030 if option to identify FIDs affected by an event status change is on, then step ahead to next affected FID */
    /* Scan FidEventChanged bitfield range between current FID (including) and FID that we started from (excluding)   */
    /* for the next FID that is affected by an event status change. Wraps around at end of FID range.                 */
    boolean found =
                 FiM_UBitfieldHdl_GetNextSetBit(FiM_GetAddrFidEventChangedTable(0), /* SBSW_FIM_GETNEXTSETBIT_CALL */
                                                (uint16)FiM_UFidHdl_GetNrOfFids(), &fidIter, cFidStart);
    if (found == FALSE)
    {
      /* #040 stop loop if no affected FID was found */
      fidIter = cFidStart;
    }
    else
    {
      /* #050 otherwise reset the affected FID bit                                                                    */
      /*      -> event status changes arriving during our calculation will set it again  and trigger another          */
      /*         calculation in the next cycle                                                                        */
      FiM_UBitfieldHdl_ResetBitInBitfield(FiM_GetAddrFidEventChangedTable(0),            /* SBSW_FIM_BITMANIP_CALL */
                                          FiM_GetSizeOfFidEventChangedTable(), fidIter);
#else
    {
#endif
      /* #070 check if FID is inhibited and set bit in bitfield to permission status (which is !inhibition status)    */
      if (FiM_UFidHdl_IsFidInhibited((FiM_FunctionIdType)fidIter) == TRUE)
      {
        FiM_UBitfieldHdl_ResetBitInBitfield(FiM_GetAddrPermStatusTable(0),               /* SBSW_FIM_BITMANIP_CALL */
                                            FiM_GetSizeOfPermStatusTable(), fidIter);
      }
      else
      {
        FiM_UBitfieldHdl_SetBitInBitfield(FiM_GetAddrPermStatusTable(0),                 /* SBSW_FIM_BITMANIP_CALL */
                                          FiM_GetSizeOfPermStatusTable(), fidIter);
      }
      /* #080 select next FID to schedule and roll over if we reached end of FID list */
      fidIter++;
      /* check whether we need to roll over to the start of the FID list */
      if (fidIter >= (uint16)FiM_UFidHdl_GetNrOfFids())
      {
        fidIter = 0;
      }
    }
    /* #090 stop if we reached our starting point (each FID is calculated at most once per call to the main function) */
    if (fidIter == cFidStart)
    {
      break;
    }
  }
  /* #100 remember FID to continue with in next call */
  FiM_UFidHdl_NextFidToSchedule = (FiM_FunctionIdType)fidIter;
  }
#endif
}


/***********************************************************************************************************************
 * public FiM_UFidHdl_SetFidEventChanged
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, FIM_CODE) FiM_UFidHdl_SetFidEventChanged(FiM_EventIdTableIndexType evTableIdx)
{
#if ((FIM_CYCLIC_FID_CALCULATION == STD_ON) && (FIM_AFFECTED_FIDS_IDENTIFICATION == STD_ON))
  {
    /* #10 identify the parts of the complete event to FID config tables that are relevant for this event             */
    /*     do so by identifying start index and stop index (last relevant element plus 1) in the complete array       */
    uint16  evToFidTableIdx_Start = FiM_GetEventToFidIdxTable(evTableIdx);
    uint16  evToFidTableIdx_Stop  = FiM_GetEventToFidIdxTable(evTableIdx + 1U);

    uint16 evToFidTableIter;
    for (evToFidTableIter = evToFidTableIdx_Start; evToFidTableIter < evToFidTableIdx_Stop; evToFidTableIter++)
    {
      /* #20 set event changed flag of connected FIDs */
      FiM_UBitfieldHdl_SetBitInBitfield(FiM_GetAddrFidEventChangedTable(0),                 /* SBSW_FIM_BITMANIP_CALL */
                                        FiM_GetSizeOfFidEventChangedTable(),
                                        FiM_GetEventToFidTable(evToFidTableIter));
    }
  }
#else
  FIM_DUMMY_STATEMENT(evTableIdx);                            /* PRQA S 1338, 2983 */ /*  MD_MSR_DummyStmt */
#endif
  return;
}


/**********************************************************************************************************************
 *  private FiM_UFidHdl_IsFidInhibited()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FIM_LOCAL_INLINE FUNC(boolean, FIM_CODE)  FiM_UFidHdl_IsFidInhibited(FiM_FunctionIdType FID)
{
  boolean       inhibStat = FALSE;

  /* #10 identify the parts of the complete "Fid-to-inhibition-code" table that are relevant for this FID             */
  /* #20 do so by identifying start index and stop index (last relevant element plus 1) in the complete array         */
  uint16_least  fidToInhTableIdx_Start  = (uint16_least)FiM_GetFidTable(FID);
  uint16_least  fidToInhTableIdx_Stop   = (uint16_least)FiM_GetFidTable(FID + 1U);

  /* #30 loop through all inhibition configurations that are connected to the FID                                     */
  uint16_least  fidToInhTableIdx        = fidToInhTableIdx_Start;
  for (fidToInhTableIdx = fidToInhTableIdx_Start; fidToInhTableIdx < fidToInhTableIdx_Stop; fidToInhTableIdx++)
  {
    /* inhibition configuration table is always generated, no manual calibration here                                 */
    /* -> it does not contain invalid inhibition configurations                                                       */
    /* even if it contained an invalid event id, first element of FiM_UInhStatHdl_InhibitionModeTable would        */
    /* neutralise it                                                                                                  */

    /* FiM_FidToInhSatIdTable contains only satellite ids smaller than size of satellite info table                   */
    /* satId < FiM_GetSizeOfSatelliteInfoTable() is always true                                                       */
    /* no write access is done later on relying on that fact -> not silence critical -> no runtime check necessary    */
    uint16  inhCodeTableIdx = FiM_UFidHdl_GetInhCodeIdx_FidToInhCodeTable(fidToInhTableIdx);
    uint8   satId           = FiM_UFidHdl_GetSatId_FidToInhSatIdTable(fidToInhTableIdx);

    /* #40 check if inhibition cfg inhibits FID, break if yes (FID is inhibited if at least one cfg inhibits)         */
    inhibStat = FiM_UInhStatHdl_GetInhibitionStatus(inhCodeTableIdx, satId);
    if (inhibStat == TRUE)
    {
      break;
    }
  }
  return inhibStat;
}


#if (FIM_FEATURE_OBD == STD_ON)
/**********************************************************************************************************************
*  private FiM_UFidHdl_IsFidPendingLocked()
*********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FIM_LOCAL_INLINE FUNC(boolean, FIM_CODE)  FiM_UFidHdl_IsFidPendingLocked(FiM_FunctionIdType FID)
{
  boolean pendingStat = FALSE;

  if (FiM_UPendCntHdl_GetPendingCounter(FID) != 0U)
  {
    pendingStat = TRUE;
  }
  else
  {
    pendingStat = FALSE;
  }

  return pendingStat;
}
#endif

#define FIM_STOP_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/**********************************************************************************************************************/
/* FiM_UFidHdl End *********************************************************************************************/
/**********************************************************************************************************************/



/***********************************************************************************************************************
*  END OF FILE: FiM_UFidHdl.c
***********************************************************************************************************************/
