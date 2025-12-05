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
/*!        \file     FiM_UGeneral.h
 *         \unit     UGeneral
 *         \brief    Internal Header for Autosar 4 module FiM - Unit General
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

#if !defined(FIM_UGENERAL_H)
#define FIM_UGENERAL_H

/* *********************************************************************************************************************
 *  GLOBAL MISRA Justifications
 **********************************************************************************************************************/


/***********************************************************************************************************************
*  INCLUDES
***********************************************************************************************************************/

#include "FiM_Int.h"

#include "FiM_Cfg_General.h"


/***********************************************************************************************************************
*  UNIT LOCAL FUNCTION MACROS
***********************************************************************************************************************/

/*! Return pointer to initialization status for given satellite id (macro is easier to read).                         */
#define FiM_UGeneral_GetInitStatusPtr_SatelliteInfoTable           FiM_GetInitStatusPtrOfSatelliteInfoTable
/*! Return application id of given satellite id (macro is easier to read).                                            */
#define FiM_UGeneral_GetApplicationId_SatelliteInfoTable           FiM_GetApplicationIdOfSatelliteInfoTable


/***********************************************************************************************************************
*  UNIT LOCAL DATA PROTOTYPES
***********************************************************************************************************************/

#define FIM_START_SEC_VAR_INIT_8
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Is the FiM master initialized? Don't use directly. Call FiM_UGeneral_GetInitializationStatusMaster() instead. */
extern VAR(boolean, FIM_VAR_INIT)              FiM_UGeneral_MasterInitialized;

#define FIM_STOP_SEC_VAR_INIT_8
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


#define FIM_START_SEC_VAR_INIT_8_RESTRICTED
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Is the FiM at least pre-initialized? */
extern VAR(boolean, FIM_VAR_INIT)              FiM_UGeneral_PreInitialized;
/*! Cumulated initialization status of all FiM Satellites. */
extern VAR(FiM_InitializationStatusType, FIM_VAR_INIT)
                                               FiM_UGeneral_InitializationStatus_SatCumulated;

#define FIM_STOP_SEC_VAR_INIT_8_RESTRICTED
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/***********************************************************************************************************************
 *  UNIT EXTERNAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define FIM_START_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
*  public FiM_UGeneral_GetInitializationStatusMaster()
**********************************************************************************************************************/
/*! \brief                    Internal helper function to get initialization status of master.
 *
 *  \details                  This function gets the initialization status of the master depending on the stored
 *                            flags for (pre-)initialization.
 *  \return                   Initialization status
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \synchronous              TRUE
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(FiM_InitializationStatusType, FIM_CODE) FiM_UGeneral_GetInitializationStatusMaster(void);

/***********************************************************************************************************************
 *  public FiM_UGeneral_GetInitializationStatusSat()
 **********************************************************************************************************************/
/*! \brief                    Internal helper function to get initialization status of satellite.
 *
 *  \details                  This function gets the initialization status of a satellite depending on the stored
 *                            initialization status of the satellite and the pre-initialization flag.
 *  \param[in]  satId         Satellite Id for which initialization status is requested.
 *  \return                   Initialization status
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \synchronous              TRUE
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(FiM_InitializationStatusType, FIM_CODE)
  FiM_UGeneral_GetInitializationStatusSat(FiM_SatelliteIdType satId);

/***********************************************************************************************************************
 *  public FiM_UGeneral_GetInitializationStatusSatCumulated()
 **********************************************************************************************************************/
/*! \brief                    Internal helper function to access cumulated initialization status of satellites.
 *
 *  \details                  See brief.
 *  \return                   Cumulated initialization status
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \synchronous              TRUE
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(FiM_InitializationStatusType, FIM_CODE) FiM_UGeneral_GetInitializationStatusSatCumulated(void);


#define FIM_STOP_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/**********************************************************************************************************************
 *  UNIT EXTERNAL INLINE FUNCTIONS
 **********************************************************************************************************************/

#if (!defined (FIM_NOUNIT_UGENERAL))                                                           /* COV_FIM_UNITTEST TX */

#define FIM_START_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  public FiM_UGeneral_GetInitializationStatusMaster()
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
FIM_LOCAL_INLINE FUNC(FiM_InitializationStatusType, FIM_CODE) FiM_UGeneral_GetInitializationStatusMaster(void) /* PRQA S 3219 */ /* MD_FiM_3219 */
{
  FiM_InitializationStatusType initStatus = FIM_UNINITIALIZED;
  if (FiM_UGeneral_PreInitialized == TRUE)
  {
    initStatus = FIM_PREINITIALIZED;
    if (FiM_UGeneral_MasterInitialized == TRUE)
    {
      initStatus = FIM_INITIALIZED;
    }
  }
  return initStatus;
}


/***********************************************************************************************************************
 *  public FiM_UGeneral_GetInitializationStatusSat()
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
FIM_LOCAL_INLINE FUNC(FiM_InitializationStatusType, FIM_CODE)
  FiM_UGeneral_GetInitializationStatusSat(FiM_SatelliteIdType satId) /* PRQA S 3219 */ /* MD_FiM_3219 */
{
  FiM_InitializationStatusType initStatus = FIM_UNINITIALIZED;

  FIM_DUMMY_STATEMENT(satId); /* column InitStatusPtr in SatelliteInfoTable is optimized to a simple define */

  if (FiM_UGeneral_PreInitialized == TRUE)
  {
    initStatus = FIM_PREINITIALIZED;
    if (*FiM_UGeneral_GetInitStatusPtr_SatelliteInfoTable(satId) == FIM_INITIALIZED)
    {
      initStatus = FIM_INITIALIZED;
    }
  }
  return initStatus;
}


/***********************************************************************************************************************
 *  public FiM_UGeneral_GetInitializationStatusSatCumulated()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FIM_LOCAL_INLINE FUNC(FiM_InitializationStatusType, FIM_CODE) FiM_UGeneral_GetInitializationStatusSatCumulated(void) /* PRQA S 3219 */ /* MD_FiM_3219 */
{
  return FiM_UGeneral_InitializationStatus_SatCumulated;
}

#define FIM_STOP_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


#endif /* FIM_NOUNIT_UGENERAL */


#endif /* FIM_UGENERAL_H */

/***********************************************************************************************************************
*  END OF FILE: FiM_UGeneral.h
***********************************************************************************************************************/
