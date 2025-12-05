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
 *         \file  IpduM_ContainerRxProcessing.h
 *         \unit  ContainerRxProcessing
 *        \brief  Microsar IpduM ContainerRxProcessing header file
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
#ifndef IPDUM_CONTAINERRXPROCESSING_H
# define IPDUM_CONTAINERRXPROCESSING_H

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
 *  IpduM_ContainerRxProcessing_UnpackContainerPdu()
*******************************************************************************************/
/*! \brief        Retrieves all contained PDUs from the container PDU and sends RxIndications where configured.
 *  \details      -
 *  \param[in]    rxContainerId
 *  \param[in]    dataPtr        Pointer to the received PDU data.
 *  \param[in]    pduLength      Length of received rx container
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different container IDs
 *  \synchronous  TRUE
 *  \spec
 *    requires rxContainerId < IpduM_GetSizeOfRxContainerPdu();
 *  \endspec
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_ContainerRxProcessing_UnpackContainerPdu(IpduM_RxContainerPduIterType rxContainerId, P2VAR(uint8, AUTOMATIC, IPDUM_APPL_DATA) dataPtr, PduLengthType pduLength);

# define IPDUM_STOP_SEC_CODE
# include "IpduM_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* IPDUM_CONTAINERRXPROCESSING_H */

/**********************************************************************************************************************
 *  END OF FILE: IpduM_ContainerRxProcessing.h
 *********************************************************************************************************************/
