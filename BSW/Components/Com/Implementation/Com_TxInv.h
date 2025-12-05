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
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  Com_TxInv.h
 *         \unit  TxInv
 *        \brief  MICROSAR Communication header file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_TXINV_H)
# define COM_TXINV_H

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
# include "Com_Lcfg.h"

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
  GLOBAL DATA PROTOTYPES (Com)
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL FUNCTION PROTOTYPES
**********************************************************************************************************************/
# define COM_START_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

/**********************************************************************************************************************
  Com_TxInv_InvalidateSignal
**********************************************************************************************************************/
/*! \brief        This function calls Com_TxSignalIf_SendSignal_Processing with the invalid value.
    \details      -
    \pre          -
    \param[in]    txAccessInfo ID of the access info table.
    \return       uint8
                    E_OK                        service has been accepted
                    COM_SERVICE_NOT_AVAILABLE   corresponding I-PDU group was stopped
                                                (or service failed due to development error)
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires txAccessInfo < Com_GetSizeOfTxAccessInfo();
    \endspec
**********************************************************************************************************************/
FUNC(uint8, COM_CODE) Com_TxInv_InvalidateSignal(Com_TxAccessInfoIterType txAccessInfo);

/**********************************************************************************************************************
  Com_TxInv_InvalidateSignalGroup
**********************************************************************************************************************/
/*! \brief        This function calls Com_TxSignalIf_SendSignal_Processing with the invalid value for all groupSignals
                  within the SignalGroup and then calls Com_TxSignalIf_SendSignalGroup_Processing to send the SignalGroup.
    \details      -
    \pre          -
    \param[in]    SignalGroupId ID of the access info table.
    \return       uint8
                  E_OK                        service has been accepted
                  COM_SERVICE_NOT_AVAILABLE   corresponding I-PDU group was stopped
                                              (or service failed due to development error)
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires SignalGroupId < Com_GetSizeOfTxSigGrpInfo();
    \endspec
**********************************************************************************************************************/
FUNC(uint8, COM_CODE) Com_TxInv_InvalidateSignalGroup(Com_TxSigGrpInfoIterType SignalGroupId);

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_TXINV_H */
/**********************************************************************************************************************
  END OF FILE: Com_TxInv.h
**********************************************************************************************************************/
