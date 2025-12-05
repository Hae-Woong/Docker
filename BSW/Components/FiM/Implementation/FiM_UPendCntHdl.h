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
/*!        \file     FiM_UPendCntHdl.h
 *         \unit     UPendCntHdl
 *         \brief    Internal Header for Autosar 4 module FiM - Unit Pending Counter Handling
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
#if !defined(FIM_UPENDCNTHDL_H)
#define FIM_UPENDCNTHDL_H


/* *********************************************************************************************************************
 *  GLOBAL MISRA Justifications
 **********************************************************************************************************************/


/***********************************************************************************************************************
*  INCLUDES
***********************************************************************************************************************/

#include "FiM_Int.h"

#if (FIM_FEATURE_OBD == STD_ON)

#include "FiM_Cfg_PendCntHdl.h"


/***********************************************************************************************************************
 *  UNIT EXTERNAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define FIM_START_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/***********************************************************************************************************************
 *  public FiM_UPendCntHdl_GetPendingCounter()
 **********************************************************************************************************************/
 /*!
 *  \brief                      Returns pending counter.
 *  \details                    See brief.
 *  \param[in]  fid             FID for which pending counter is returned
 *  \pre                        -
 *  \config                     FIM_FEATURE_OBD == STD_ON
 *  \context                    TASK|ISR2
 *  \synchronous                TRUE
 *  \reentrant                  TRUE
 *******************************************************************************/
FIM_LOCAL_INLINE FUNC(FiM_FidCounterType, FIM_CODE) FiM_UPendCntHdl_GetPendingCounter(FiM_FunctionIdType fid);


/***********************************************************************************************************************
 *  public FiM_UPendCntHdl_ClearPendingCounterAll()
 **********************************************************************************************************************/
 /*!
 *  \brief                      Clears all pending counters.
 *  \details                    Sets all pending counters to 0.
 *  \pre                        -
 *  \config                     FIM_FEATURE_OBD == STD_ON
 *  \context                    TASK|ISR2
 *  \synchronous                TRUE
 *  \reentrant                  TRUE
 *******************************************************************************/
FUNC(void, FIM_CODE) FiM_UPendCntHdl_ClearPendingCounterAll(void);


/***********************************************************************************************************************
 *  public FiM_UnitPendCntHdl_ModPendingCounter_Checked()
 **********************************************************************************************************************/
/*!
 *  \brief                      Modifies pending counter.
 *  \details                    Increment, decrement or leave the counters unchanged depending on passed modification
 *                              code.
 *                              Checks that write access is not done beyond array bounds
 *                              if FIM_DEV_RUNTIME_CHECKS is on.
 *  \param[in]  fid             FID for which pending counter is modified
 *  \param[in]  modCodePending  code how to modify FID Pending counter
 *                              possible codes:
 *                              FIM_FID_NOCHANGE: FID Pending shall not be changed due to status change
 *                              FIM_FID_INC:      FID Pending shall be incremented due to status change
 *                              FIM_FID_DEC:      FID Pending shall be decremented due to status change
 *  \pre                        -
 *  \config                     FIM_FEATURE_OBD == STD_ON
 *  \context                    TASK|ISR2
 *  \synchronous                TRUE
 *  \reentrant                  TRUE
 *******************************************************************************/
FUNC(void, FIM_CODE) FiM_UPendCntHdl_ModPendingCounter_Checked(FiM_FunctionIdType fid,
                                                                                   uint8 modCodePending);


#define FIM_STOP_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/**********************************************************************************************************************
 *  UNIT EXTERNAL INLINE FUNCTIONS
 **********************************************************************************************************************/

#if (!defined (FIM_NOUNIT_UPENDCNTHDL))                                                        /* COV_FIM_UNITTEST TX */

#define FIM_START_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/***********************************************************************************************************************
 *  public FiM_UPendCntHdl_GetPendingCounter()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FIM_LOCAL_INLINE FUNC(FiM_FidCounterType, FIM_CODE) FiM_UPendCntHdl_GetPendingCounter(FiM_FunctionIdType fid) /* PRQA S 3219 */ /* MD_FiM_3219 */
{
  return FiM_GetFidPendingCounter(fid);
}


#define FIM_STOP_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

#endif /* FIM_NOUNIT_UPENDCNTHDL */

#endif /* FIM_FEATURE_OBD == STD_ON */

#endif /* FIM_UPENDCNTHDL_H */

/***********************************************************************************************************************
*  END OF FILE: FiM_UPendCntHdl.h
***********************************************************************************************************************/
