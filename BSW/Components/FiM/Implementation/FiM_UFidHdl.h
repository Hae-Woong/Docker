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
/*!        \file     FiM_UFidHdl.h
 *         \unit     UFidHdl
 *         \brief    Internal Header for Autosar 4 module FiM - Unit FID Handling
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

#if !defined(FIM_UFIDHDL_H)
#define FIM_UFIDHDL_H

/* *********************************************************************************************************************
 *  GLOBAL MISRA Justifications
 **********************************************************************************************************************/


/***********************************************************************************************************************
*  INCLUDES
***********************************************************************************************************************/


/***********************************************************************************************************************
 *  UNIT EXTERNAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define FIM_START_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  public FiM_UFidHdl_Init()
 **********************************************************************************************************************/
 /*! \brief                   Initialize unit FiM_UFidHdl.
 *
 *  \details                  The initialization is done on a per word basis, not on a per bit basis.
 *                            The manipulation of the arrays within this function is not synchronized with other
 *                            bitfield operations on these arrays within the unit. Therefore, call this function before
 *                            the unit gets operational.
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \synchronous              TRUE
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FUNC(void, FIM_CODE)  FiM_UFidHdl_Init(void);


/***********************************************************************************************************************
 *  public FiM_UFidHdl_CalcPermissionStates()
 **********************************************************************************************************************/
/*! \brief                    Calculate permission status of FIDs.
 *
 *  \details                  Calculate permission status of FIDs and store result for later quick retrieval.
 *                            Adhere to the maximum number of FIDs to be calculated per cycle.
 *  \pre                      -
 *  \config                   FIM_CYCLIC_FID_CALCULATION == STD_ON
 *  \context                  TASK|ISR2
 *  \synchronous              TRUE
 *  \reentrant                FALSE
 **********************************************************************************************************************/
FUNC(void, FIM_CODE)  FiM_UFidHdl_CalcPermissionStates(void);


/***********************************************************************************************************************
 *  public FiM_UFidHdl_SetFidEventChanged()
 **********************************************************************************************************************/
/*! \brief                            Set tag that a connected event of FIDs has changed.
 *
 *  \details                          See brief.
 *  \param[in]  evTableIdx            Index into EventToFid index table. Index corresponds to index into eventId table.
 *  \pre                              -
 *  \config                           FIM_AFFECTED_FIDS_IDENTIFICATION == STD_ON
 *  \context                          TASK|ISR2
 *  \synchronous                      TRUE
 *  \reentrant                        TRUE
 **********************************************************************************************************************/
FUNC(void, FIM_CODE) FiM_UFidHdl_SetFidEventChanged(FiM_EventIdTableIndexType evTableIdx);


#define FIM_STOP_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/**********************************************************************************************************************
 *  UNIT EXTERNAL INLINE FUNCTIONS
 **********************************************************************************************************************/

#if (!defined (FIM_NOUNIT_UFIDHDL))                                                            /* COV_FIM_UNITTEST TX */


#endif /* FIM_NOUNIT_UFIDHDL */


#endif /* FIM_UFIDHDL_H */

/***********************************************************************************************************************
*  END OF FILE: FiM_UFidHdl.h
***********************************************************************************************************************/
