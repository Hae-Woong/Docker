/**********************************************************************************************************************
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
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  EthTSyn_VersionInfo.h
 *         \unit  VersionInfo
 *        \brief  Public header of the VersionInfo unit
 *      \details  Contains all public function declarations and type definitions with respect to the VersionInfo unit
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/


#if !defined(ETHTSYN_VERSION_INFO_H)
# define ETHTSYN_VERSION_INFO_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
/* Include external headers of the Units. */
# include "Std_Types.h"
# include "EthTSyn_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define ETHTSYN_START_SEC_CODE
# include "EthTSyn_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

# if (ETHTSYN_VERSION_INFO_API == STD_ON) /* COV_MSR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 *  EthTSyn_GetVersionInfo
 *********************************************************************************************************************/
/*! \brief       Returns the version information.
 *  \details     EthTSyn_GetVersionInfo() returns version information, vendor ID and AUTOSAR module ID of the component.
 *  \param[out]  VersionInfoPtr  Pointer to where to store the version information. Parameter must not be NULL.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      ETHTSYN_VERSION_INFO_API
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_GetVersionInfo(
  P2VAR(Std_VersionInfoType, AUTOMATIC, ETHTSYN_APPL_VAR) VersionInfoPtr);
# endif /* (ETHTSYN_VERSION_INFO_API == STD_ON) */

# define ETHTSYN_STOP_SEC_CODE
# include "EthTSyn_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ETHTSYN_VERSION_INFO_H */
/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_VersionInfo.h
 *********************************************************************************************************************/
