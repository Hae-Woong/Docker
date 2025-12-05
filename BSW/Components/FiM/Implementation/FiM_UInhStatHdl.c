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
/*!        \file     FiM_UInhStatHdl.c
 *         \unit     UInhStatHdl
 *         \brief    Source for Autosar 4 module FiM - Unit Inhibition Status Handling
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
#include "FiM_UInhStatHdl.h"


/***********************************************************************************************************************
*  LOCAL CONSTANT MACROS
***********************************************************************************************************************/


/***********************************************************************************************************************
*  LOCAL FUNCTION MACROS
***********************************************************************************************************************/


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
/* FiM_UInhStatHdl Start *******************************************************************************************/
/**********************************************************************************************************************/

#define FIM_START_SEC_CONST_UNSPECIFIED
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Inhibition Mode Table: used in calculation of effects of event status changes. */
FIM_LOCAL CONST(FiM_InhibitionModeType, FIM_CONST)
  FiM_UInhStatHdl_InhibitionModeTable[FIM_INHCODE_NUMBER] =                        /* PRQA S 3218 */ /* MD_FiM_8.9 */
{
  { /* invalid: pattern can never match "anded" mask */
    /* FIM_INHCODE_INVALID */
    (Dem_MonitorStatusType)0x00u,    
    (Dem_MonitorStatusType)0x01u,
    (boolean) FALSE
  },
  { /* FIM_LAST_FAILED */
    FIM_LAST_FAILED_MASK,
    FIM_LAST_FAILED_PATTERN,
    FIM_LAST_FAILED_INVERTRESULT
  },
  { /* FIM_NOT_TESTED */
    FIM_NOT_TESTED_MASK,                    
    FIM_NOT_TESTED_PATTERN,
    FIM_NOT_TESTED_INVERTRESULT
  },
  { /* FIM_TESTED */
    FIM_TESTED_MASK,                    
    FIM_TESTED_PATTERN,
    FIM_TESTED_INVERTRESULT
  },
  { /* FIM_TESTED_AND_FAILED */
    FIM_TESTED_AND_FAILED_MASK,            
    FIM_TESTED_AND_FAILED_PATTERN,
    FIM_TESTED_AND_FAILED_INVERTRESULT
  },
  { /* FIM_NOT_TESTED_OR_FAILED */
    FIM_NOT_TESTED_OR_FAILED_MASK,            
    FIM_NOT_TESTED_OR_FAILED_PATTERN,
    FIM_NOT_TESTED_OR_FAILED_INVERTRESULT
  },
  { /* invalid code for inhibition (only pending): pattern can never match "anded" mask */
    /* FIM_INHCODE_PENDING */
    (Dem_MonitorStatusType)0x00u,    
    (Dem_MonitorStatusType)0x01u,
    (boolean) FALSE
  }
};                   

#define FIM_STOP_SEC_CONST_UNSPECIFIED
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define FIM_START_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/***********************************************************************************************************************
 *  private FiM_UInhStatHdl_GetInhibitionStatusFromMonitor()
 **********************************************************************************************************************/
/*! \brief                    Internal helper function to obtain the inhibition status
 *                            for one monitor status / inhibition code.
 *
 *  \details                  Obtain the inhibition status for
 *                            - one monitor status
 *                            - one inhibition code
 *  \param[in]  monStatus     monitor status byte of event
 *  \param[in]  inhCode       code of inhibition configuration, i.e. index into FiM_InhibitionModeTable;
 *  \return                   TRUE:  combination inhibits FID.
 *                            FALSE: combination does not inhibit FID.
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FIM_LOCAL FUNC(boolean, FIM_CODE)
  FiM_UInhStatHdl_GetInhibitionStatusFromMonitor(Dem_MonitorStatusType monStatus, uint8 inhCode);


/***********************************************************************************************************************
 * private FiM_UInhStatHdl_GetInhCode_BySatId
 **********************************************************************************************************************/
/*! \brief                        Return inhibition code at index position for a given satellite id.
 *
 *  \details                      see brief 
 *  \param[in]  index             Index in table to obtain inhibition code from.
 *  \param[in]  satId             Satellite Id for which code is stored.
 *                                Must be smaller than size of satellite info table!
 *  \return                       Inhibition code from the specified table.
 *  \pre                          -
 *  \context                      TASK|ISR2
 *  \reentrant                    TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(FiM_InhCodeTableType, FIM_CODE)
  FiM_UInhStatHdl_GetInhCode_BySatId(uint16 index, FiM_SatelliteIdType satId);


/***********************************************************************************************************************
 *  private FiM_UInhStatHdl_ModInhibitionStatus_Checked()
 **********************************************************************************************************************/
/*! \brief                    Internal helper function to set or reset inhibition status on satellite.
 *
 *  \details                  This function (re-)sets the inhibition status of the specified inhibition configuration
 *                            on a specified satellite.
 *                            If runtime checks are enabled:
 *                            It is ensured that satellite id is smaller than number of satellite info elements.
 *  \param[in]  inhibStat     Inhibition status to be set.
 *  \param[in]  inhCodeIdx    Index in satellite specific array for which inhibition status should be set.
 *  \param[in]  satId         Satellite Id that inhibition status should be set on.
 *  \return                   E_OK if inhibition status could be set
 *                            E_NOT_OK otherwise
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(Std_ReturnType, FIM_CODE) FiM_UInhStatHdl_ModInhibitionStatus_Checked(boolean inhibStat,
                                                                                            uint16 inhCodeIdx,
                                                                                            FiM_SatelliteIdType satId);


/***********************************************************************************************************************
 *  private FiM_UInhStatHdl_GetInhibitionStatusFromMonitor()
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
FIM_LOCAL FUNC(boolean, FIM_CODE)
                      FiM_UInhStatHdl_GetInhibitionStatusFromMonitor(Dem_MonitorStatusType monStatus, uint8 inhCode)
{
  boolean inhibStat = FALSE;;
  if (FiM_ErrorCheck_IsRuntimeErrorDetected(inhCode >= FIM_INHCODE_NUMBER) == TRUE) /* PRQA S 2992, 2996, 4404 */ /* MD_FiM_ConfCheck, MD_FiM_ConfCheck, MD_MSR_AutosarBoolean */
  /* #20 if runtime checks are enabled and inhibition code is not valid                             */
  /*     Invalid inhibition codes are ignored                                                       */
  /*     only relevant if there is a problem with the generator                                     */
  /*     would result otherwise in an out of bounds read access and an undefined inhibition status  */
  /*       -> avoid out of bound access and define status to permitted (i.e. ignore code)           */
  {
    /* (FIM_DEV_RUNTIME_CHECKS == STD_OFF) || (inhCode < FIM_INHCODE_NUMBER) */
    /* #30 define status to permitted (i.e. ignore code)           */
    inhibStat = FALSE;
  }
  /* #40 otherwise */
  else
  {
    /* #100 mask the passed monStatus with the mask that's relevant for the passed inhibition code                    */
    /* #110 this yields the bits that are relevant for the passed inhibition code                                     */
    /* #120 if these bits match the bit pattern relevant for the passed inhibition code then combination inhibits FID */
    inhibStat = (boolean)((monStatus & FiM_UInhStatHdl_InhibitionModeTable[inhCode].Mask) /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
                        == FiM_UInhStatHdl_InhibitionModeTable[inhCode].Pattern);
    /* #130 check whether inhibition code requests an inversion of the result                                         */
    if (FiM_UInhStatHdl_InhibitionModeTable[inhCode].InvertResult == TRUE)
    {
      inhibStat = (boolean)(!inhibStat);                             /* PRQA S 4116, 4304, 4558 */ /* MD_MSR_AutosarBoolean */
    }
  }
  return inhibStat;
}


/***********************************************************************************************************************
*  public FiM_UInhStatHdl_CalcInhibitionStatusFromMonitor()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, FIM_CODE) FiM_UInhStatHdl_CalcInhibitionStatusFromMonitor(Dem_MonitorStatusType monStatus,
                                                                     uint16 inhCodeIdx,
                                                                     FiM_SatelliteIdType satId)
{
  /* #10 get code for index and satellite                                                                             */
  FiM_InhCodeTableType  inhCode   = FiM_UInhStatHdl_GetInhCode_BySatId(inhCodeIdx, satId);
  /* #20 get inhibition status for combination of code / monitor status                                               */
  boolean               inhibStat = FiM_UInhStatHdl_GetInhibitionStatusFromMonitor(monStatus, inhCode);
  /* #30 set the calculated status                                                                                    */
  (void)FiM_UInhStatHdl_ModInhibitionStatus_Checked(inhibStat, inhCodeIdx, satId);
}


/***********************************************************************************************************************
 *  public FiM_UInhStatHdl_ClearInhibitionStatus_Checked()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, FIM_CODE) FiM_UInhStatHdl_ClearInhibitionStatus_Checked(uint16 inhCodeIdx,
                                                                             FiM_SatelliteIdType satId)
{
  return FiM_UInhStatHdl_ModInhibitionStatus_Checked(FALSE, inhCodeIdx, satId);
}


/***********************************************************************************************************************
 *  public FiM_UInhStatHdl_ClearInhibitionStatusAll_Checked()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, FIM_CODE) FiM_UInhStatHdl_ClearInhibitionStatusAll_Checked(FiM_SatelliteIdType satId)
{
  Std_ReturnType        retVal              = E_NOT_OK;                               /* PRQA S 2981 */ /* MD_FiM_2.2 */

  if ((FiM_ErrorCheck_IsRuntimeErrorDetected(satId >= FiM_GetSizeOfSatelliteInfoTable()) == FALSE)) /* PRQA S 2991, 2995, 4404 */ /* MD_FiM_ConfCheck, MD_FiM_ConfCheck, MD_MSR_AutosarBoolean */
  {
    /* ((FIM_DEV_RUNTIME_CHECKS == STD_OFF) ||
        (FIM_DEV_RUNTIME_CHECKS == STD_ON) && (satId < FiM_GetSizeOfSatelliteInfoTable())) */
    FiM_ProcessorWordType *inhStatusBasePtr = FiM_UInhStatHdl_GetInhStatusBasePtr_SatelliteInfoTable(satId);
    FiM_ProcessorWordType inhStatusLength   = FiM_UInhStatHdl_GetInhStatusLength_SatelliteInfoTable(satId);
    FiM_ProcessorWordType inhStatIter       = 0;

    FiM_EnterCritical_CrossCore();                                                   /* PRQA S 3138 */ /* MD_MSR_14.3 */
    for (inhStatIter = 0; inhStatIter < inhStatusLength; inhStatIter++)
    {
      inhStatusBasePtr[inhStatIter] = 0;                                              /* SBSW_FIM_ACCESS_INHSTATUSSAT */
    }
    FiM_LeaveCritical_CrossCore();                                                   /* PRQA S 3138 */ /* MD_MSR_14.3 */
    retVal = E_OK;
  }
  return retVal;
}


/***********************************************************************************************************************
 * private FiM_UInhStatHdl_GetInhCode_BySatId
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FIM_LOCAL_INLINE FUNC(FiM_InhCodeTableType, FIM_CODE)
  FiM_UInhStatHdl_GetInhCode_BySatId(uint16 index, FiM_SatelliteIdType satId)
{
  /* caller has to assert: satId < FiM_GetSizeOfSatelliteInfoTable() */
  const FiM_InhCodeTableType  *inhCodeTableArr  = FiM_UInhStatHdl_GetInhCodeTableBasePtr_SatelliteInfoTable(satId);
  const FiM_InhCodeTableType  inhCode           = inhCodeTableArr[index];

  {
    FIM_DUMMY_STATEMENT(satId); /* if column InhCodeBasePtr in SatelliteInfoTable is optimized to a simple define */
  }

  return inhCode;
}


/***********************************************************************************************************************
 *  private FiM_UInhStatHdl_ModInhibitionStatus_Checked()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FIM_LOCAL_INLINE FUNC(Std_ReturnType, FIM_CODE) FiM_UInhStatHdl_ModInhibitionStatus_Checked(boolean inhibStat,
  uint16 inhCodeIdx,
  FiM_SatelliteIdType satId)
{
  Std_ReturnType retVal = E_NOT_OK;                                                   /* PRQA S 2981 */ /* MD_FiM_2.2 */
  if ((FiM_ErrorCheck_IsRuntimeErrorDetected(satId >= FiM_GetSizeOfSatelliteInfoTable()) == FALSE)) /* PRQA S 2991, 2995, 4404 */ /* MD_FiM_ConfCheck, MD_FiM_ConfCheck, MD_MSR_AutosarBoolean */
  {
    /* ((FIM_DEV_RUNTIME_CHECKS == STD_OFF) ||
        (FIM_DEV_RUNTIME_CHECKS == STD_ON) && (satId < FiM_GetSizeOfSatelliteInfoTable())) */
    FiM_ProcessorWordType *inhStatusBasePtr = FiM_UInhStatHdl_GetInhStatusBasePtr_SatelliteInfoTable(satId);
    uint16                inhStatusLength   = FiM_UInhStatHdl_GetInhStatusLength_SatelliteInfoTable(satId);

    /* set bit in bitfield to inhibition status */
    if (inhibStat == TRUE)
    {
      FiM_UBitfieldHdl_SetBitInBitfield(inhStatusBasePtr, inhStatusLength, inhCodeIdx);     /* SBSW_FIM_BITMANIP_INHSTATUSSAT_CALL */
    }
    else
    {
      FiM_UBitfieldHdl_ResetBitInBitfield(inhStatusBasePtr, inhStatusLength, inhCodeIdx);   /* SBSW_FIM_BITMANIP_INHSTATUSSAT_CALL */
    }
    retVal = E_OK;
  }
  return retVal;
}

#define FIM_STOP_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/**********************************************************************************************************************/
/* FiM_UInhStatHdl End *********************************************************************************************/
/**********************************************************************************************************************/

/***********************************************************************************************************************
*  END OF FILE: FiM_UInhStatHdl.c
***********************************************************************************************************************/
