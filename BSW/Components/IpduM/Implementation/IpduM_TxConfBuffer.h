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
 *         \file  IpduM_TxConfBuffer.h
 *         \unit  TxConfBuffer
 *        \brief  Microsar IpduM Tx confirmation buffer header file
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
#ifndef IPDUM_TXCONFBUFFER_H
# define IPDUM_TXCONFBUFFER_H

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
*  IpduM_TxConfBuffer_Init()
*******************************************************************************************/
/*! \brief        Initializes all variables related to the Tx Confirmation Buffer
 *  \details      -
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different handle IDs
 *  \synchronous  TRUE
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_TxConfBuffer_Init(void);

/*******************************************************************************************
*  IpduM_TxConfBuffer_Put()
*******************************************************************************************/
/*! \brief        Puts the txConfId into the Tx Confirmation Buffer
 *  \details      -
 *  \param[in]    txContainerId
 *  \param[in]    txConfId      Id to put into the buffer
 *  \return       E_OK          Was put into the queue successfully.
 *  \return       E_NOT_OK      Wasn't put into the queue. Buffer is full.
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different handle IDs
 *  \synchronous  TRUE
 *  \spec
 *    requires txContainerId < IpduM_GetSizeOfCTxContainerPdu();
 *  \endspec
*******************************************************************************************/
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_TxConfBuffer_Put(IpduM_CTxContainerPduIterType txContainerId, PduIdType txConfId);

/*******************************************************************************************
*  IpduM_TxConfBuffer_Get()
*******************************************************************************************/
/*! \brief        Gets the next Id from the Tx Confirmation Buffer
 *  \details      -
 *  \param[in]    txContainerId
 *  \param[out]   txConfId      Pdu Id which is returned
 *  \return       E_OK          Id was read successfully.
 *  \return       E_NOT_OK      Id was not read. Buffer is empty.
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different handle IDs
 *  \synchronous  TRUE
 *  \spec
 *    requires txContainerId < IpduM_GetSizeOfCTxContainerPdu();
 *  \endspec
*******************************************************************************************/
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_TxConfBuffer_Get(IpduM_CTxContainerPduIterType txContainerId, P2VAR(PduIdType, AUTOMATIC, IPDUM_APPL_DATA) txConfId);

/*******************************************************************************************
*  IpduM_TxConfBuffer_GetSize()
*******************************************************************************************/
/*! \brief        Returns the size of the TxConfirmation buffer for this TxContainer.
 *  \details      -
 *  \param[in]    txContainerId
 *  \return       Size of the TxConfirmation Buffer for this txContainerId.
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different handle IDs
 *  \synchronous  TRUE
 *  \spec
 *    requires txContainerId < IpduM_GetSizeOfCTxContainerPdu();
 *  \endspec
*******************************************************************************************/
FUNC(IpduM_ContainerTxConfirmationBufferLengthOfContainerTxConfirmationBufferInfoType, IPDUM_CODE) IpduM_TxConfBuffer_GetSize(IpduM_CTxContainerPduIterType txContainerId);

/*******************************************************************************************
*  IpduM_TxConfBuffer_Clear()
*******************************************************************************************/
/*! \brief        Resets the TxConfirmationBuffer of the container to its init values.
 *  \details      -
 *  \param[in]    txContainerId
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different handle IDs
 *  \synchronous  TRUE
 *  \spec
 *    requires txContainerId < IpduM_GetSizeOfCTxContainerPdu();
 *  \endspec
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_TxConfBuffer_Clear(IpduM_CTxContainerPduIterType txContainerId);

# define IPDUM_STOP_SEC_CODE
# include "IpduM_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* IPDUM_TXCONFBUFFER_H */

/**********************************************************************************************************************
 *  END OF FILE: IpduM_TxConfBuffer.h
 *********************************************************************************************************************/
