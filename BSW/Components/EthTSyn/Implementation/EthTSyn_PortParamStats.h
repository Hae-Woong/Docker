/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  EthTSyn_PortParamStats.h
 *         \unit  PortParamStats
 *        \brief  EthTSyn header file of the EthTSyn_PortParamStats unit.
 *      \details  Interface and type definitions of the EthTSyn_PortParamStats unit.
 *********************************************************************************************************************/

#if !defined (ETHTSYN_PORT_PARAM_STATS_H)
# define ETHTSYN_PORT_PARAM_STATS_H

# if (ETHTSYN_PORT_PARAM_STATS_SUPPORT== STD_ON) /* COV_MSR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#  include "EthTSyn_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#  define ETHTSYN_START_SEC_CODE
#  include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  EthTSyn_GetAndResetPortParameterStatistics
 *********************************************************************************************************************/
/*! \brief       Retrieves and optionally resets the port parameter statistics of an EthTSyn port.
 *  \details     -
 *  \param[in]   PortIdx               Index of the EthTSyn port.
 *  \param[in]   StatisticsResetNeeded Indicates if the port parameter counters shall be reset.
 *  \param[out]  PortParamStatsPtr     Pointer to an object where to store the port parameter statistics. Parameter
 *                                     must not be NULL.
 *  \return      E_OK - Successfully retrieved port parameter statistics.
 *  \return      E_NOT_OK - Retrieving port parameter statistics failed.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      ETHTSYN_PORT_PARAM_STATS_SUPPORT
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_GetAndResetPortParameterStatistics(
                uint8                       PortIdx,
                boolean                     StatisticsResetNeeded,
  ETHTSYN_P2VAR(EthTSyn_PortParamStatsType) PortParamStatsPtr);

#  define ETHTSYN_STOP_SEC_CODE
#  include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# endif /* (ETHTSYN_PORT_PARAM_STATS_SUPPORT == STD_ON) */
#endif /* ETHTSYN_PORT_PARAM_STATS_H */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_PortParamStats.h
 *********************************************************************************************************************/
