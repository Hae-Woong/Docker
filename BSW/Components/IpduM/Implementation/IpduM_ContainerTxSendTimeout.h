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
 *         \file  IpduM_ContainerTxSendTimeout.h
 *         \unit  ContainerTxSendTimeout
 *        \brief  Microsar IpduM Container Tx Send Timeout header file
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
#ifndef IPDUM_CONTAINERTXSENDTIMEOUT_H
# define IPDUM_CONTAINERTXSENDTIMEOUT_H

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
*  IpduM_ContainerTx_SendTimeout_Init()
*******************************************************************************************/
/*! \brief        Initializes the Container send timeout
 *  \details      -
 *  \pre          Interrupts are disabled
 *  \pre          Module is uninitialized
 *  \pre          IpduM_InitMemory has been called unless IpduM_ModuleInitialized is initialized by start-up code
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_SendTimeout_Init(void);

/*******************************************************************************************
*  IpduM_ContainerTx_SendTimeout_MainFunction
*******************************************************************************************/
/*! \brief        Cyclicly decrements the SendTimeout
 *  \details      -
 *  \param[in]    txContainerId
 *  \return       TRUE   SendTimeout is expired
 *                FALSE  SendTimeout is still running
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different handle IDs
 *  \synchronous  TRUE
 *  \spec
 *    requires txContainerId < IpduM_GetSizeOfCTxContainerPdu();
 *  \endspec
*******************************************************************************************/
FUNC(boolean, IPDUM_CODE) IpduM_ContainerTx_SendTimeout_MainFunction(IpduM_CTxContainerPduIterType txContainerId);

/*******************************************************************************************
*  IpduM_ContainerTx_SendTimeout_IsRunning
*******************************************************************************************/
/*! \brief        Is the sendTimeout for the container Pdu running?
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
FUNC(boolean, IPDUM_CODE) IpduM_ContainerTx_SendTimeout_IsRunning(IpduM_CTxContainerPduIterType txContainerId);

/*******************************************************************************************
*  IpduM_ContainerTx_SendTimeout_Stop
*******************************************************************************************/
/*! \brief        Stop container Pdu send timeout
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
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_SendTimeout_Stop(IpduM_CTxContainerPduIterType txContainerId);

/*******************************************************************************************
*  IpduM_ContainerTx_SendTimeout_Start
*******************************************************************************************/
/*! \brief        Start send timeouts after adding a PDU to a container
 *  \details      -
 *  \param[in]    txContainedId
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different handle IDs
 *  \synchronous  TRUE
 *  \spec
 *    requires txContainedId < IpduM_GetSizeOfTxContainedPdu();
 *  \endspec
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_SendTimeout_Start(IpduM_TxContainedPduIterType txContainedId);

# define IPDUM_STOP_SEC_CODE
# include "IpduM_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* IPDUM_CONTAINERTXSENDTIMEOUT_H */

/**********************************************************************************************************************
 *  END OF FILE: IpduM_ContainerTxSendTimeout.h
 *********************************************************************************************************************/
