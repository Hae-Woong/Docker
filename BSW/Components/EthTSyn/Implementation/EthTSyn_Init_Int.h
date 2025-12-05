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
/*!        \file  EthTSyn_Init_Int.h
 *         \unit  Init
 *        \brief  EthTSyn internal header of the Init unit
 *      \details  Contains all internal function declarations and type definitions with respect to the Init unit
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

#if !defined(ETHTSYN_INIT_INT_H)
# define ETHTSYN_INIT_INT_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
# include "EthTSyn_Cbk.h"
# include "EthTSyn_Lcfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/* EthTSyn State Definitions */
typedef enum
{
  ETHTSYN_INIT_STATE_UNINIT = 0u,
  ETHTSYN_INIT_STATE_REINITIALIZING,
  ETHTSYN_INIT_STATE_INIT
} EthTSyn_Init_StateEnumType;


/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define ETHTSYN_START_SEC_CODE
# include "EthTSyn_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  EthTSyn_Init_IsInitialized
 *********************************************************************************************************************/
/*! \brief        Checks if the EthTSyn module is initialized.
 *  \details      -
 *  \pre          -
 *  \return       TRUE - Module is initialized
 *  \return       FALSE - Module is not initalized
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(boolean, ETHTSYN_CODE) EthTSyn_Init_IsInitialized(void);

# define ETHTSYN_STOP_SEC_CODE
# include "EthTSyn_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ETHTSYN_INIT_INT_H */
/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_Init_Int.h
 *********************************************************************************************************************/
