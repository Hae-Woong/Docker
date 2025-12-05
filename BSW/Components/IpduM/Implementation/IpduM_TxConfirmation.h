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
 *  -------------------------------------------------------------------------------------------------------------------
 *         \file  IpduM_TxConfirmation.h
 *         \unit  TxConfirmation
 *        \brief  Microsar IpduM TxConfirmation header file
 *      \details  This is the implementation of the MICROSAR IpduM module.
 *                The basic software module is based on the AUTOSAR IpduM specification.
 *
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
#ifndef IPDUM_TXCONFIRMATION_H
# define IPDUM_TXCONFIRMATION_H

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
# include "IpduM_Cfg.h"
# include "IpduM_Lcfg.h"
# include "IpduM_PBcfg.h"
# include "ComStack_Types.h"

/**********************************************************************************************************************
  GLOBAL CONSTANT MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL FUNCTION MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL DATA PROTOTYPES
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL FUNCTION PROTOTYPES
**********************************************************************************************************************/
# define IPDUM_START_SEC_CODE
# include "IpduM_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

/*******************************************************************************************
 *  IpduM_TxConfirmation_Init()
*******************************************************************************************/
/*! \brief        Initialize the unit
 *  \details      -
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different container IDs
 *  \synchronous  TRUE
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_TxConfirmation_Init(void);

/*******************************************************************************************
 *  IpduM_TxConfirmation_StartTimeout()
*******************************************************************************************/
/*! \brief        Load TxConfirmation timeout counter
 *  \details      -
 *  \param[in]    txLoInfoWithInvalidIndexesIdx
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different container IDs
 *  \synchronous  TRUE
 *  \spec
 *    requires txLoInfoWithInvalidIndexesIdx < IpduM_GetSizeOfTxLoInfoWithInvalidIndexes();
 *  \endspec
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_TxConfirmation_StartTimeout(IpduM_TxLoInfoWithInvalidIndexesIterType txLoInfoWithInvalidIndexesIdx);

/*******************************************************************************************
 *  IpduM_TxConfirmation_StopTimeout()
*******************************************************************************************/
/*! \brief        Reset TxConfirmation timeout counter
 *  \details      -
 *  \param[in]    txLoInfoWithInvalidIndexesIdx
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different container IDs
 *  \synchronous  TRUE
 *  \spec
 *    requires txLoInfoWithInvalidIndexesIdx < IpduM_GetSizeOfTxLoInfoWithInvalidIndexes();
 *  \endspec
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_TxConfirmation_StopTimeout(IpduM_TxLoInfoWithInvalidIndexesIterType txLoInfoWithInvalidIndexesIdx);

/*******************************************************************************************
 *  IpduM_TxConfirmation_IsTransmissionAllowed
*******************************************************************************************/
/*! \brief        Is a transmission to the destination allowed?
 *  \details      -
 *  \param[in]    txLoInfoWithInvalidIndexesIdx
 *  \return       TRUE  transmission is allowed
 *                FALSE transmission is not allowed
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different container IDs
 *  \synchronous  TRUE
 *  \spec
 *    requires txLoInfoWithInvalidIndexesIdx < IpduM_GetSizeOfTxLoInfoWithInvalidIndexes();
 *  \endspec
*******************************************************************************************/
FUNC(boolean, IPDUM_CODE) IpduM_TxConfirmation_IsTransmissionAllowed(IpduM_TxLoInfoWithInvalidIndexesIterType txLoInfoWithInvalidIndexesIdx);

/*******************************************************************************************
*  IpduM_TxConfirmation_TxConfirmation()
*******************************************************************************************/
/*! \brief        Indicates complete transmission of a PDU
 *  \details      -
 *  \param[in]    txLoInfoWithInvalidIndexesIdx
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different handle IDs
 *  \synchronous  TRUE
 *  \spec
 *    requires txLoInfoWithInvalidIndexesIdx < IpduM_GetSizeOfTxLoInfoWithInvalidIndexes();
 *  \endspec
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_TxConfirmation_TxConfirmation(IpduM_TxLoInfoWithInvalidIndexesIterType txLoInfoWithInvalidIndexesIdx);

/*******************************************************************************************
*  IpduM_TxConfirmation_MainFunction()
*******************************************************************************************/
/*! \brief        Process the TxConfirmation Timeout.
 *  \details      -
 *  \param[in]    mainFunctionTxIndirectionIdx
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different handle IDs
 *  \synchronous  TRUE
 *  \spec
 *    requires mainFunctionTxIndirectionIdx < IpduM_GetSizeOfMainFunctionTxIndirection();
 *  \endspec
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_TxConfirmation_MainFunction(IpduM_MainFunctionTxIndirectionIterType mainFunctionTxIndirectionIdx);

# define IPDUM_STOP_SEC_CODE
# include "IpduM_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* IPDUM_TXCONFIRMATION_H */

/**********************************************************************************************************************
 *  END OF FILE: IpduM_TxConfirmation.h
 *********************************************************************************************************************/
