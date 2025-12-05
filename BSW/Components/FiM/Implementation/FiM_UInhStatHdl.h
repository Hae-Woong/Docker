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
/*!        \file     FiM_UInhStatHdl.h
 *         \unit     UInhStatHdl
 *         \brief    Internal Header for Autosar 4 module FiM - Unit Inhibition Status Handling
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
#if !defined(FIM_UINHSTATHDL_H)
#define FIM_UINHSTATHDL_H


/* *********************************************************************************************************************
 *  GLOBAL MISRA Justifications
 **********************************************************************************************************************/


/***********************************************************************************************************************
*  INCLUDES
***********************************************************************************************************************/

#include "Std_Types.h"
#include "Dem_Types.h"
#include "FiM_Cfg.h"
#include "FiM_Cfg_InhStatHdl.h"
#include "FiM_Types.h"
#include "FiM_Lcfg.h"
#include "FiM_Int.h"

#include "FiM_LHelper.h"


/***********************************************************************************************************************
*  UNIT LOCAL FUNCTION MACROS
***********************************************************************************************************************/

/*! Return pointer to inhibition code table for given satellite id (macro is easier to read).                         */
#define FiM_UInhStatHdl_GetInhCodeTableBasePtr_SatelliteInfoTable  FiM_GetInhCfgCodeBasePtrOfSatelliteInfoTablePB
/*! Return pointer to inhibition status table for given satellite id (macro is easier to read).                       */
#define FiM_UInhStatHdl_GetInhStatusBasePtr_SatelliteInfoTable     FiM_GetInhStatusBasePtrOfSatelliteInfoTable
/*! Return length of inhibition status table for given satellite id (macro is easier to read).                        */
#define FiM_UInhStatHdl_GetInhStatusLength_SatelliteInfoTable      FiM_GetInhStatusSizeOfSatelliteInfoTable


/***********************************************************************************************************************
 *  UNIT EXTERNAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define FIM_START_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  public FiM_UInhStatHdl_GetInhibitionStatus()
 **********************************************************************************************************************/
/*! \brief                    Returns the inhibition status.
 *
 *  \details                  For one satellite, for one inhibition index.
 *  \param[in]  inhCodeIdx    Index in satellite specific array for which inhibition status should be returned.
 *  \param[in]  satId         Satellite Id that inhibition status should be set on.
 *                            Must be smaller than size of satellite info table!
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \synchronous              TRUE
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(boolean, FIM_CODE) FiM_UInhStatHdl_GetInhibitionStatus(uint16 inhCodeIdx,
                                                                             FiM_SatelliteIdType satId);


/***********************************************************************************************************************
 *  public FiM_UInhStatHdl_CalcInhibitionStatusFromMonitor()
 **********************************************************************************************************************/
/*! \brief                    Calculates and modifies the inhibition status accordingly for one inhibition code.
 *
 *  \details                  Calculates and modifies the inhibition status accordingly for
 *                            - one monitor status
 *                            - one inhibition code
 *  \param[in]  monStatus     monitor status byte of event
 *  \param[in]  inhCodeIdx    Index in satellite specific array for which inhibition status should be set.
 *  \param[in]  satId         Satellite Id that inhibition status should be set on.
 *                            Must be smaller than size of satellite info table!
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \synchronous              TRUE
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FUNC(void, FIM_CODE) FiM_UInhStatHdl_CalcInhibitionStatusFromMonitor(Dem_MonitorStatusType monStatus,
                                                                     uint16 inhCodeIdx,
                                                                     FiM_SatelliteIdType satId);

/***********************************************************************************************************************
 *  public FiM_UInhStatHdl_ClearInhibitionStatus_Checked()
 **********************************************************************************************************************/
/*! \brief                    Sets the inhibition status for one inhibition code to FALSE.
 *
 *  \details                  This function resets the inhibition status of the specified inhibition configuration on a
 *                            specified satellite.
 *                            If runtime checks are enabled:
 *                            It is ensured that satellite id is smaller than number of satellite info elements.
 *  \param[in]  inhCodeIdx    Index in satellite specific array for which inhibition status should be set.
 *  \param[in]  satId         Satellite Id that inhibition status should be set on.
 *  \return                   E_OK if inhibition status could be set
 *                            E_NOT_OK otherwise
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \synchronous              TRUE
 *  \reentrant                TRUE
**********************************************************************************************************************/
FUNC(Std_ReturnType, FIM_CODE) FiM_UInhStatHdl_ClearInhibitionStatus_Checked(uint16 inhCodeIdx,
                                                                                  FiM_SatelliteIdType satId);

/***********************************************************************************************************************
 *  public FiM_UInhStatHdl_ClearInhibitionStatusAll_Checked()
 **********************************************************************************************************************/
/*! \brief                    Sets all inhibition states on satellite to FALSE.
 *
 *  \details                  This function sets all inhibition states of a specified satellite to "0" (not inhibited).
 *                            If runtime checks are enabled:
 *                            It is ensured that satellite id is smaller than number of satellite info elements.
 *  \param[in]  satId         Satellite Id that inhibition states should be set on.
 *  \return                   E_OK if inhibition states could be set
 *                            E_NOT_OK otherwise
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \synchronous              TRUE
 *  \reentrant                TRUE
**********************************************************************************************************************/
FUNC(Std_ReturnType, FIM_CODE) FiM_UInhStatHdl_ClearInhibitionStatusAll_Checked(FiM_SatelliteIdType satId);

#define FIM_STOP_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/**********************************************************************************************************************
 *  UNIT EXTERNAL INLINE FUNCTIONS
 **********************************************************************************************************************/

#if (!defined (FIM_NOUNIT_UINHSTATHDL))                                                        /* COV_FIM_UNITTEST TX */

#define FIM_START_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/***********************************************************************************************************************
*  public FiM_UInhStatHdl_GetInhibitionStatus()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FIM_LOCAL_INLINE FUNC(boolean, FIM_CODE)
                      FiM_UInhStatHdl_GetInhibitionStatus(uint16 inhCodeIdx,         /* PRQA S 3219 */ /* MD_FiM_3219 */
                                                          FiM_SatelliteIdType satId)
{
  boolean   isInhibited;

  FIM_DUMMY_STATEMENT(satId); /* column InitStatusPtr in SatelliteInfoTable is optimized to a simple define */

  {
    /* caller should ensure that satId < FiM_GetSizeOfSatelliteInfoTable()                                              */
    /* no write access is done later on relying on that fact -> not silence critical -> no runtime check necessary      */
    const FiM_ProcessorWordType *inhStatusBasePtr = FiM_UInhStatHdl_GetInhStatusBasePtr_SatelliteInfoTable(satId);
    isInhibited = FiM_UBitfieldHdl_TestBitInBitfield(inhStatusBasePtr, inhCodeIdx);                /* SBSW_FIM_BITTEST_CALL */
  }

  return isInhibited;
}



#define FIM_STOP_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

#endif /* FIM_NOUNIT_UINHSTATHDL */


#endif /* FIM_UINHSTATHDL_H */

/***********************************************************************************************************************
*  END OF FILE: FiM_UInhStatHdl.h
***********************************************************************************************************************/
