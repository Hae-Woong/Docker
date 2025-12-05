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
 *         \file  IpduM_ContainerTxTrigger.h
 *         \unit  ContainerTxTrigger
 *        \brief  Microsar IpduM ContainerTxTrigger header file
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
#ifndef IPDUM_CONTAINERTXTRIGGER_H
# define IPDUM_CONTAINERTXTRIGGER_H

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
*  IpduM_ContainerTxTrigger_Init()
*******************************************************************************************/
/*! \brief        Initializes the variables related to container trigger.
 *  \details      -
 *  \pre          Interrupts are disabled
 *  \pre          Module is uninitialized
 *  \pre          IpduM_InitMemory has been called unless IpduM_ModuleInitialized is initialized by start-up code
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_ContainerTxTrigger_Init(void);

/*******************************************************************************************
*  IpduM_ContainerTxTrigger_IsTriggered()
*******************************************************************************************/
/*! \brief        Checks trigger conditions and returns the result.
 *  \details      -
 *  \param[in]    txContainedId
 *  \param[in]    containerLength
 *  \return       TRUE   Container is triggered.
 *                FALSE  Contianer is not triggered.
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different handle IDs
 *  \synchronous  TRUE
 *  \trace        SPEC-2068537, SPEC-6231909, SPEC-2068539, SPEC-2068543, SPEC-2068545, SPEC-2068549, SPEC-2068551, SPEC-2068552
 *  \spec
 *    requires txContainedId < IpduM_GetSizeOfTxContainedPdu();
 *  \endspec
*******************************************************************************************/
FUNC(boolean, IPDUM_CODE) IpduM_ContainerTxTrigger_IsTriggered(IpduM_TxUpIndIdxOfTxUpInfoType txContainedId, uint32 containerLength);

/*******************************************************************************************
*  IpduM_ContainerTxTrigger_SetFirstContainedPduFlag()
*******************************************************************************************/
/*! \brief        Sets the first contained Pdu flag of the container to the corresponding value.
 *  \details      -
 *  \param[in]    txContainerIdx
 *  \param[in]    value the first contained Pdu flag is set to this value
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different handle IDs
 *  \synchronous  TRUE
 *  \spec
 *    requires txContainerIdx < IpduM_GetSizeOfCTxContainerPdu();
 *  \endspec
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_ContainerTxTrigger_SetFirstContainedPduFlag(IpduM_CTxContainerPduIterType txContainerIdx, boolean value);

/*******************************************************************************************
*  IpduM_ContainerTxTrigger_IsFirstContainedPdu()
*******************************************************************************************/
/*! \brief        Is the last added Pdu the first contained Pdu?
 *  \details      -
 *  \param[in]    txContainerIdx
 *  \return       Is the last added Pdu the first contained Pdu?
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different handle IDs
 *  \synchronous  TRUE
 *  \spec
 *    requires txContainerIdx < IpduM_GetSizeOfCTxContainerPdu();
 *  \endspec
*******************************************************************************************/
FUNC(boolean, IPDUM_CODE) IpduM_ContainerTxTrigger_IsFirstContainedPdu(IpduM_CTxContainerPduIterType txContainerIdx);

# define IPDUM_STOP_SEC_CODE
# include "IpduM_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* IPDUM_CONTAINERTXTRIGGER_H */

/**********************************************************************************************************************
 *  END OF FILE: IpduM_ContainerTxTrigger.h
 *********************************************************************************************************************/
