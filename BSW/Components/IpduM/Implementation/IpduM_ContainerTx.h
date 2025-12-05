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
 *         \file  IpduM_ContainerTx.h
 *         \unit  ContainerTx
 *        \brief  Microsar IpduM Container header file
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
#ifndef IPDUM_CONTAINERTX_H
# define IPDUM_CONTAINERTX_H

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
*  IpduM_ContainerTx_Transmit()
*******************************************************************************************/
/*! \brief        Requests the transmission of a contained PDU
 *  \details      -
 *  \param[in]    txContainedId
 *  \param[in]    PduInfoPtr    Pointer to payload buffer
 *  \return       E_OK          Transmit request was accepted by IpduM
 *  \return       E_NOT_OK      Transmit request was not accepted
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different handle IDs
 *  \synchronous  TRUE
 *  \trace        SPEC-2068537, SPEC-6231909, SPEC-2068539, SPEC-2068543, SPEC-2068545, SPEC-2068549, SPEC-2068551, SPEC-2068552
 *  \spec
 *    requires txContainedId < IpduM_GetSizeOfTxContainedPdu();
 *  \endspec
*******************************************************************************************/
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_ContainerTx_Transmit(IpduM_TxUpIndIdxOfTxUpInfoType txContainedId, P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr);

/*******************************************************************************************
*  IpduM_ContainerTx_TriggerTransmit()
*******************************************************************************************/
/*! \brief          Requests the PDU data before transmission
 *  \details        -
 *  \param[in]      txContainerId
 *  \param[in,out]  PduInfoPtr    Pointer to lower layer buffer
 *  \return         E_OK          Data was copied correctly
 *  \return         E_NOT_OK      Data could not be copied correctly, PduInfoPtr does not contain valid data
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE for different handle IDs
 *  \synchronous    TRUE
 *  \spec
 *    requires txContainerId < IpduM_GetSizeOfCTxContainerPdu();
 *  \endspec
*******************************************************************************************/
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_ContainerTx_TriggerTransmit(IpduM_TxContainerPduIdxOfCTxLoInfoType txContainerId, P2VAR(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr);

/*******************************************************************************************
*  IpduM_ContainerTx_TxConfirmation()
*******************************************************************************************/
/*! \brief        Indicates complete transmission of a PDU
 *  \details      -
 *  \param[in]    containerPduId
 *  \param[in]    result of the transmission
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different handle IDs
 *  \synchronous  TRUE
 *  \spec
 *    requires containerPduId < IpduM_GetSizeOfCTxContainerPdu();
 *  \endspec
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_TxConfirmation(IpduM_CTxContainerPduIterType containerPduId, Std_ReturnType result);

/*******************************************************************************************
 *  IpduM_ContainerTx_MainFunction()
*******************************************************************************************/
/*! \brief        Performs actions not directly initiated by calls from the PduR.
 *  \details      -
 *  \param[in]    mainFunctionTxIndirectionIdx index to know the execution context of this mainfunction.
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires mainFunctionTxIndirectionIdx < IpduM_GetSizeOfMainFunctionTxIndirection();
 *  \endspec
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_MainFunction(IpduM_MainFunctionTxIndirectionIterType mainFunctionTxIndirectionIdx);

# define IPDUM_STOP_SEC_CODE
# include "IpduM_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* IPDUM_CONTAINERTX_H */

/**********************************************************************************************************************
 *  END OF FILE: IpduM_ContainerTx.h
 *********************************************************************************************************************/
