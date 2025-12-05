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
/*!        \file  Com_RxDlMon.h
 *         \unit  RxDlMon
 *        \brief  MICROSAR Communication header file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_RXDLMON_H)
# define COM_RXDLMON_H
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
  Com_RxDlMon_HasTimeoutOccurred
**********************************************************************************************************************/
/*! \brief        This function evaluates if a Timeout has occurred.
    \details      -
    \pre          -
    \param[in]    ComRxToutObjtId  ID of Rx Timeout object.
    \return       boolean
                    TRUE     A timeout has occurred.
                    FALSE    No timeout has occurred.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different Handles
    \spec
      requires ComRxToutObjtId < Com_GetSizeOfRxTOutInfo();
    \endspec
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_RxDlMon_HasTimeoutOccurred(Com_RxTOutInfoIterType ComRxToutObjtId);

/**********************************************************************************************************************
  Com_RxDlMon_RemoveOccurredFlag
**********************************************************************************************************************/
/*! \brief        This function removes the OCCURRED flag form the Rx Deadline Monitoring State.
    \details      -
    \pre          -
    \param[in]    ComRxToutObjtId  ID of Rx Timeout object.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different Handles
    \spec
      requires ComRxToutObjtId < Com_GetSizeOfRxTOutInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxDlMon_RemoveOccurredFlag(Com_RxTOutInfoIterType ComRxToutObjtId);

/**********************************************************************************************************************
  Com_RxDlMon_MainFunctionRx
**********************************************************************************************************************/
/*! \brief        This function handles the decrementing of all active rx deadline monitoring counters.
    \details      -
    \pre          -
    \param[in]    mainFunctionId    ID of the calling rx main function.
    \trace        SPEC-2736949
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different Handles
    \spec
      requires mainFunctionId < Com_GetSizeOfMainFunctionRxStruct();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxDlMon_MainFunctionRx(Com_MainFunctionRxStructIterType mainFunctionId);

/**********************************************************************************************************************
  Com_RxDlMon_TimeOutEvent
**********************************************************************************************************************/
/*! \brief        This function is called when a timeout occurs.
    \details      -
    \pre          -
    \param[in]    idxRxTOutInfo Handle of the Rx Timeout Info.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxRxTOutInfo < Com_GetSizeOfRxTOutInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxDlMon_TimeOutEvent(Com_RxTOutInfoIterType idxRxTOutInfo);

/**********************************************************************************************************************
  Com_RxDlMon_InitRxIpdu
**********************************************************************************************************************/
/*! \brief        This function initializes the Rx deadline monitoring parameters of the passed Rx ComIPdu
    \details      -
    \pre          -
    \param[in]    rxPduId Handle of the I-PDU.
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxDlMon_InitRxIpdu(Com_RxPduInfoIterType rxPduId);

/**********************************************************************************************************************
  Com_RxDlMon_SigEvent
**********************************************************************************************************************/
/*! \brief        This function reloads the timeout counter value of the passed Rx ComSignal
    \details      -
    \pre          -
    \param[in]    idxRxSigInfo Handle Id of Rx ComSignal
    \trace        SPEC-2736869
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxRxSigInfo < Com_GetSizeOfRxSigInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxDlMon_SigEvent(Com_RxSigInfoIterType idxRxSigInfo);

/**********************************************************************************************************************
  Com_RxDlMon_SigGrpEvent
**********************************************************************************************************************/
/*! \brief        This function reloads the timeout counter value of the passed Rx ComSignalGroup
    \details      -
    \pre          -
    \param[in]    idxRxSigGrpInfo Handle Id of Rx ComSignalGroup
    \trace        SPEC-2736869
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxRxSigGrpInfo < Com_GetSizeOfRxSigGrpInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxDlMon_SigGrpEvent(Com_RxSigGrpInfoIterType idxRxSigGrpInfo);

/**********************************************************************************************************************
  Com_RxDlMon_ActivateRxDlMon
**********************************************************************************************************************/
/*! \brief        This function activates the Rx Deadline Monitoring
    \details      -
    \pre          -
    \param[in]    idxRxTOutInfo Handle Id of RxToutInfo
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxDlMon_ActivateRxDlMon(Com_RxTOutInfoIterType idxRxTOutInfo);

/**********************************************************************************************************************
  Com_RxDlMon_DeactivateRxIpduDm
**********************************************************************************************************************/
/*! \brief        This function disables the Deadline Monitoring for the Rx I-Pdu.
    \details      -
    \pre          -
    \param[in]    ComRxToutObjtId  ID of Rx Timeout object.
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxDlMon_DeactivateRxIpduDm(Com_RxTOutInfoIterType ComRxToutObjtId);

/**********************************************************************************************************************
  Com_RxDlMon_ActivateRxIpduDm
**********************************************************************************************************************/
/*! \brief        This function enables the Deadline Monitoring for the Rx I-Pdu.
    \details      -
    \pre          -
    \param[in]    ComRxToutObjtId  ID of Rx Timeout object.
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
FUNC(void, COM_CODE)  Com_RxDlMon_ActivateRxIpduDm(Com_RxTOutInfoIterType ComRxToutObjtId);

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_RXDLMON_H */
/**********************************************************************************************************************
  END OF FILE: Com_RxDlMon.h
**********************************************************************************************************************/
