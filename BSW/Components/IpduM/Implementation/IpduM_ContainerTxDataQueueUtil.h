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
 *         \file  IpduM_ContainerTxDataQueueUtil.h
 *         \unit  ContainerTxDataQueueUtil
 *        \brief  Microsar IpduM Container Tx Data Queue Util header file
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
#ifndef IPDUM_CONTAINERTXDATAQUEUEUTIL_H
# define IPDUM_CONTAINERTXDATAQUEUEUTIL_H

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
 *  IpduM_ContainerTx_DataQueueUtil_TransmitContainerPdu_DirectDP()
*******************************************************************************************/
/*! \brief        Transmit the next container instance from the queue with direct data provision.
 *  \details      -
 *  \param[in]    txContainerId
 *  \return       E_OK            Transmission was successful, last queue item is removed.
 *  \return       E_NOT_OK        Transmission was unsuccessful.
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different container IDs.
 *  \synchronous  TRUE
 *  \spec
 *    requires txContainerId < IpduM_GetSizeOfCTxContainerPdu();
 *  \endspec
*******************************************************************************************/
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_ContainerTx_DataQueueUtil_TransmitContainerPdu_DirectDP(IpduM_CTxContainerPduIterType txContainerId);

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueueUtil_TriggerTransmissionOfCurrContainerPdu()
*******************************************************************************************/
/*! \brief        Transmits (if direct transmission configured) or queues current container PDU.
 *  \details      -
 *  \param[in]    txContainerId
 *  \param[in]    queueCurrentContainer shall the current container instance be queued before triggering its transmission.
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different container IDs
 *  \synchronous  TRUE
 *  \spec
 *    requires txContainerId < IpduM_GetSizeOfCTxContainerPdu();
 *  \endspec
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_DataQueueUtil_TriggerTransmissionOfCurrContainerPdu(IpduM_CTxContainerPduIterType txContainerId, boolean queueCurrentContainer);

# define IPDUM_STOP_SEC_CODE
# include "IpduM_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* IPDUM_CONTAINERTXDATAQUEUEUTIL_H */

/**********************************************************************************************************************
 *  END OF FILE: IpduM_ContainerTxDataQueueUtil.h
 *********************************************************************************************************************/
