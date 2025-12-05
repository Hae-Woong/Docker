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
/*!        \file     FiM_UPendCntHdl.c
 *         \unit     UPendCntHdl
 *         \brief    Source for Autosar 4 module FiM - Unit Pending Counter Handling
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

#include "FiM_UPendCntHdl.h"

#include "Dem.h"
#include "FiM.h"

#include "FiM_ErrorCheck.h"


/***********************************************************************************************************************
*  LOCAL CONSTANT MACROS
***********************************************************************************************************************/


/***********************************************************************************************************************
*  LOCAL FUNCTION MACROS
***********************************************************************************************************************/



/***********************************************************************************************************************
*  LOCAL DATA TYPES AND STRUCTURES
***********************************************************************************************************************/

/* If the file ends up empty after preprocessor is done with the file, some compilers might complain that this is an
   empty translation unit which they do not like. Therefore, we add add a dummy typedef here.                         */
typedef uint16 FiM_UPendCntHdl_FimCAvoidEmptyTranslationUnit;                         /* PRQA S 3205 */ /* MD_FiM_2.3 */


/***********************************************************************************************************************
*  LOCAL DATA PROTOTYPES
***********************************************************************************************************************/


#if (FIM_FEATURE_OBD == STD_ON)


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
/* FiM_UPendCntHdl Start *******************************************************************************************/
/**********************************************************************************************************************/

#define FIM_START_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  private FiM_UPendCntHdl_IncPendingCounter_Checked()
 **********************************************************************************************************************/
/*! \brief                    Internal helper function to increment FID pending counter.
 *
 *  \details                  This function increments the pending counter for the passed FID.
 *                            If runtime checks are enabled:
 *                              It is ensured that FID is smaller than number of elements in FiM_FidPendingCounter
 *                              before write access is done.
 *  \param[in]  fid           Function Id that pending counter should be incremented for.
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 *  \config                   FIM_FEATURE_OBD == STD_ON
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(void, FIM_CODE) FiM_UPendCntHdl_IncPendingCounter_Checked(FiM_FunctionIdType fid);

/***********************************************************************************************************************
 *  private FiM_UPendCntHdl_DecPendingCounter_Checked()
 **********************************************************************************************************************/
 /*! \brief                   Internal helper function to decrement FID pending counter if counter is not already 0.
 *
 *  \details                  This function decrements the pending counter for the passed FID. It includes a prior
 *                            check for 0.
 *                            If runtime checks are enabled:
 *                              It is ensured that FID is smaller than number of elements in FiM_FidPendingCounter
 *                              before write access is done.
 *  \param[in]  fid           Function Id that pending counter should be decremented for.
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 *  \config                   FIM_FEATURE_OBD == STD_ON
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(void, FIM_CODE) FiM_UPendCntHdl_DecPendingCounter_Checked(FiM_FunctionIdType fid);


/***********************************************************************************************************************
 *  public FiM_UPendCntHdl_ClearPendingCounterAll()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, FIM_CODE) FiM_UPendCntHdl_ClearPendingCounterAll(void)
{
  FiM_FidPendingCounterIterType            fidIter = 0;
  /* reset all pending counters to 0 */
  FiM_EnterCritical_LocalCore();                                                   /* PRQA S 3138 */ /* MD_MSR_14.3 */
  for (fidIter = 0; fidIter < FiM_GetSizeOfFidPendingCounter(); fidIter++)
  {
    FiM_SetFidPendingCounter(fidIter, 0);                    /* SBSW_FIM_ACCESS_FIDPENDINGCOUNTER_CALCINHPENDSTATES */
  }
  FiM_LeaveCritical_LocalCore();                                                   /* PRQA S 3138 */ /* MD_MSR_14.3 */
}


/***********************************************************************************************************************
 *  public FiM_UPendCntHdl_ModPendingCounter_Checked()
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
 *
 */
FUNC(void, FIM_CODE) FiM_UPendCntHdl_ModPendingCounter_Checked(FiM_FunctionIdType fid, uint8 modCodePending)
{
  {
    /* enter critical section to protect each increment/decrement against interruption; */
    /* only one enter/leave over the whole if/else to reduce number of enter/leaves     */
    FiM_EnterCritical_LocalCore();                                                   /* PRQA S 3138 */ /* MD_MSR_14.3 */
    if (modCodePending == FIM_FID_DEC)
    {
      /* decrement FID Pending counter is necessary                              */
      FiM_UPendCntHdl_DecPendingCounter_Checked(fid);
    }
    if (modCodePending == FIM_FID_INC)
    {
      /* increment FID Pending counter is necessary                              */
      FiM_UPendCntHdl_IncPendingCounter_Checked(fid);
    }
    /* leave critical section */
    FiM_LeaveCritical_LocalCore();                                                   /* PRQA S 3138 */ /* MD_MSR_14.3 */
  }
  return;
}


/***********************************************************************************************************************
*  private FiM_UPendCntHdl_IncPendingCounter_Checked()
**********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
*/
FIM_LOCAL_INLINE FUNC(void, FIM_CODE) FiM_UPendCntHdl_IncPendingCounter_Checked(FiM_FunctionIdType fid)
{
  /* avoid warning "comparison is always false due to limited range of data type" if there are exactly 256 FIDs       */
  uint16            fid16   = fid;
  if ((FiM_ErrorCheck_IsRuntimeErrorDetected(fid16 >= FiM_GetSizeOfFidPendingCounter()) == FALSE)) /* PRQA S 2991, 2995, 2996, 4404 */ /* MD_FiM_ConfCheck, MD_FiM_ConfCheck, MD_FiM_ConfCheck, MD_MSR_AutosarBoolean */
  {
    /* ((FIM_DEV_RUNTIME_CHECKS == STD_OFF) ||
        (FIM_DEV_RUNTIME_CHECKS == STD_ON) && (fid16 < FiM_GetSizeOfFidPendingCounter())) */
    FiM_IncFidPendingCounter(fid16);                                     /* SBSW_FIM_ACCESS_FIDPENDINGCOUNTER_CHECKED */
  }
}


/***********************************************************************************************************************
 *  private FiM_UPendCntHdl_DecPendingCounter_Checked()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FIM_LOCAL_INLINE FUNC(void, FIM_CODE) FiM_UPendCntHdl_DecPendingCounter_Checked(FiM_FunctionIdType fid)
{
  /* avoid warning "comparison is always false due to limited range of data type" if there are exactly 256 FIDs       */
  uint16            fid16   = fid;
  if ((FiM_ErrorCheck_IsRuntimeErrorDetected(fid16 >= FiM_GetSizeOfFidPendingCounter()) == FALSE)) /* PRQA S 2991, 2995, 2996, 4404 */ /* MD_FiM_ConfCheck, MD_FiM_ConfCheck, MD_FiM_ConfCheck, MD_MSR_AutosarBoolean */
    /* ((FIM_DEV_RUNTIME_CHECKS == STD_OFF) ||
        (FIM_DEV_RUNTIME_CHECKS == STD_ON) && (fid16 < FiM_GetSizeOfFidPendingCounter())) */
  {
    if (FiM_GetFidPendingCounter(fid16) > 0U)
    {
      FiM_DecFidPendingCounter(fid16);                                   /* SBSW_FIM_ACCESS_FIDPENDINGCOUNTER_CHECKED */
    }
  }
}


#define FIM_STOP_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


#endif /* FIM_FEATURE_OBD == STD_ON */

/**********************************************************************************************************************/
/* FiM_UPendCntHdl End *********************************************************************************************/
/**********************************************************************************************************************/



/***********************************************************************************************************************
*  END OF FILE: FiM_UPendCntHdl.c
***********************************************************************************************************************/
