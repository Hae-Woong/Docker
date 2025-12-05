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
/*!        \file  Com_Reporting.h
 *         \unit  Reporting
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/

#if !defined (COM_REPORTING_H)
# define COM_REPORTING_H

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
# include "Com_Lcfg.h"

/**********************************************************************************************************************
  GLOBAL CONSTANT MACROS
**********************************************************************************************************************/

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
Com_Reporting_CallDetReportError
**********************************************************************************************************************/
/*! \brief        This function is called by Com whenever a development error occurs.
  \details        This function does nothing if error reporting is not configured or the error code equals COM_E_NO_ERROR
  \pre            -
  \param[in]      API_ID              the service ID of the AUTOSAR COM modules API in which the error was detected.
  \param[in]      ERROR_CODE          the error ID
  \context        TASK|ISR2
  \synchronous    TRUE
  \reentrant      TRUE
  \trace          SPEC-2736947
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Reporting_CallDetReportError(uint8 apiId, uint8 errorCode);

#if(COM_USE_INIT_POINTER == STD_ON)
/**********************************************************************************************************************
 * Com_Reporting_CallBswErrorHook
 *********************************************************************************************************************/
/*! \brief       Calls EcuM_BswErrorHook with an errorId to indicate corrupted Postbuild configuration data.
 *  \details     This function does nothing if now BswErrorHook is configured.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Reporting_CallBswErrorHook(uint8 errorId);
#endif

/**********************************************************************************************************************
  Com_Reporting_CallTxIpduCallout
**********************************************************************************************************************/
/*! \brief            This function calls the TxIpduCallout and returns the result.
    \details          This function returns True if the tx Pdu does not have a TxIpduCallout.
    \pre              -
    \param[in]        txPduInfoIdx  ID of I-PDU to be transmitted.
    \param[in,out]    pduInfoPtr    Pointer to the SDU buffer and the SDU length.
    \return           boolean       Callout result
    \context          TASK|ISR2
    \synchronous      TRUE
    \reentrant        TRUE, for different Handles
    \trace            DSGN-Com23621
    \spec
      requires reportingIdx < Com_GetSizeOfReporting();
      requires txPduInfoIdx < Com_GetSizeOfTxPduInfo();
      requires pduInfoPtr != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_Reporting_CallTxIpduCallout(Com_ReportingIterType reportingIdx, Com_TxPduInfoIterType txPduInfoIdx, P2VAR(PduInfoType, AUTOMATIC, AUTOMATIC) pduInfoPtr);

/**********************************************************************************************************************
  Com_Reporting_CallRxIpduCallout
**********************************************************************************************************************/
/*! \brief            This function calls the RxIpduCallout and returns the result.
    \details          This function returns True if the tx Pdu does not have a RxIpduCallout.
    \pre              -
    \param[in]        rxPduInfoIdx  ID of the received I-PDU.
    \param[in,out]    pduInfoPtr    Pointer to the SDU buffer and the SDU length.
    \return           boolean       Callout result
    \context          TASK|ISR2
    \synchronous      TRUE
    \reentrant        TRUE, for different Handles
    \trace            DSGN-RX-IPDUCALLOUT
    \spec
      requires reportingIdx < Com_GetSizeOfReporting();
      requires rxPduInfoIdx < Com_GetSizeOfRxPduInfo();
      requires pduInfoPtr != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_Reporting_CallRxIpduCallout(Com_ReportingIterType reportingIdx, Com_RxPduInfoIterType rxPduInfoIdx, P2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) pduInfoPtr);

/**********************************************************************************************************************
  Com_Reporting_CallNotification
**********************************************************************************************************************/
/*! \brief        This function calls the notification corrsponding to the passed Index.
    \details      The table entry can either be a InvalidNotification or a IndicationNotification.
    \pre          Should be called without locked interrupts.
    \param[in]    reportingIdx  Handle of the notification.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different Handles
    \spec
      requires reportingIdx < Com_GetSizeOfReporting();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Reporting_CallNotification(Com_ReportingIterType reportingIdx);

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_REPORTING_H */
/**********************************************************************************************************************
  END OF FILE: Com_Reporting.h
**********************************************************************************************************************/
