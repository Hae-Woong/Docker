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
 *         \file  IpduM_ContainerRx.h
 *         \unit  ContainerRx
 *        \brief  Microsar IpduM Container Rx header file
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
#ifndef IPDUM_CONTAINERRX_H
# define IPDUM_CONTAINERRX_H

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
*  IpduM_ContainerRx_RxIndication()
*******************************************************************************************/
/*! \brief        Indicate the reception of a ContainerRx PDU.
 *  \details      -
 *  \param[in]    rxContainerId
 *  \param[out]   PduInfoPtr         Pointer to buffer containing PDU
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different handle IDs
 *  \synchronous  TRUE
 *  \trace        SPEC-2068566, SPEC-2068569, SPEC-2068570, SPEC-2068572, SPEC-2068576
 *  \spec
 *    requires rxContainerId < IpduM_GetSizeOfRxContainerPdu();
 *  \endspec
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_ContainerRx_RxIndication(IpduM_RxPathwayIndIdxOfRxInfoType rxContainerId, P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr);

/*******************************************************************************************
 *  IpduM_ContainerRx_MainFunction()
*******************************************************************************************/
/*! \brief        Process deferred containerRx Pdus.
 *  \details      -
 *  \param[in]    mainFunctionRxIndirectionIdx index to know the execution context of this mainfunction.
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires mainFunctionRxIndirectionIdx < IpduM_GetSizeOfMainFunctionRxIndirection();
 *  \endspec
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_ContainerRx_MainFunction(IpduM_MainFunctionRxIndirectionIterType mainFunctionRxIndirectionIdx);

# define IPDUM_STOP_SEC_CODE
# include "IpduM_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* IPDUM_CONTAINERRX_H */

/**********************************************************************************************************************
 *  END OF FILE: IpduM_ContainerRx.h
 *********************************************************************************************************************/
