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
/*!        \file  EthIf_Mode_Cbk.h
 *        \brief  EthIf Mode public callback API header
 *
 *      \details  Provides access to the public callback API of the sub-module Mode of EthIf.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/

#if !defined (ETHIF_MODE_CBK_H)
# define ETHIF_MODE_CBK_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/*********************************************************************************************************
 * Predefined file includes based on \trace DSGN-EthIfDiag1975
 *********************************************************************************************************/
# include "EthIf_Mode_Types.h"

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

# define ETHIF_START_SEC_CODE
# include "EthIf_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# if (ETHIF_ENABLE_AUTOSAR_FORWARD_COMPATIBILITY == STD_ON)
/**********************************************************************************************************************
 *  EthIf_CtrlModeIndication
 *********************************************************************************************************************/
/*! \brief          Notify the caller of Eth_SetControllerMode that the controller mode has been set successfully
 *  \details        Called asynchronously when mode has been read out. Triggered by previous Eth_SetControllerMode 
 *                  call. Can directly be called within the trigger functions.
 *  \param[in]      CtrlIdx   Index of the physical Ethernet controller within the context of the Ethernet Interface
 *  \param[in]      CtrlMode  Notified Ethernet controller mode
 *  \pre            ETHIF_ENABLE_AUTOSAR_FORWARD_COMPATIBILITY == STD_ON
 *  \context        ANY
 *  \reentrant      FALSE for the same CtrlIdx, TRUE for different CtrlIdx
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, ETHIF_CODE) EthIf_CtrlModeIndication(
    uint8        CtrlIdx,
    Eth_ModeType CtrlMode);
# endif /* ETHIF_ENABLE_AUTOSAR_FORWARD_COMPATIBILITY */

# define ETHIF_STOP_SEC_CODE
# include "EthIf_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */

#endif /* ETHIF_MODE_CBK_H */

/**********************************************************************************************************************
 *  END OF FILE: EthIf_Mode_Cbk.h
 *********************************************************************************************************************/

