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
/*!        \file  EthTSyn_Init.h
 *         \unit  Init
 *        \brief  Public header of the Init unit
 *      \details  Contains all public function declarations and type definitions with respect to the EthTSyn Init unit
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

#if !defined(ETHTSYN_INIT_H)
# define ETHTSYN_INIT_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
# include "EthTSyn_Lcfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define ETHTSYN_START_SEC_CODE
# include "EthTSyn_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  EthTSyn_Init
 *********************************************************************************************************************/
/*! \brief        Initialization of the EthTSyn module.
 *  \details      Function initializes the module EthTSyn. It initializes all variables and sets the module state to
 *                initialized.
 *  \param[in]    CfgPtr   Pointer to configuration
 *  \pre          The function EthTSyn_InitMemory() must be called first
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \spec
 *    requires CfgPtr == &EthTSyn_PCConfig.Config;
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_Init(
  P2CONST(EthTSyn_ConfigType, AUTOMATIC, ETHTSYN_INIT_DATA) CfgPtr);

/**********************************************************************************************************************
 *  EthTSyn_InitMemory
 *********************************************************************************************************************/
/*! \brief       Memory initialization of the EthTSyn module.
 *  \details     Service to initialize module global variables at power up. This function initializes the variables
 *               in *_INIT_* sections. Used in case they are not initialized by the startup code.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_InitMemory(void);

# define ETHTSYN_STOP_SEC_CODE
# include "EthTSyn_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ETHTSYN_INIT_H */
/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_Init.h
 *********************************************************************************************************************/
