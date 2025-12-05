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
/*!        \file  Com_RxSignalIf.h
 *         \unit  RxSignalIf
 *        \brief  MICROSAR Communication header file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_RXSIGNALIF_H)
# define COM_RXSIGNALIF_H

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
  Com_RxSignalIf_ReceiveSignalProcessing
**********************************************************************************************************************/
/*! \brief          This function copies the content of the associated signal buffer to the passed SignalDataPtr.
    \details        -
    \pre            -
    \param[in]      SignalId       ID of signal to be received.
    \param[out]     SignalDataPtr  Pointer to the buffer where the received signal shall be stored.
    \return         uint8
                    E_OK                        service has been accepted
                    COM_SERVICE_NOT_AVAILABLE   corresponding I-PDU group was stopped
                                                (or service failed due to development error)
    \context        TASK|ISR2
    \synchronous    TRUE
    \reentrant      TRUE, for different handles
    \spec
      requires SignalId < Com_GetSizeOfRxAccessInfo();
      requires SignalDataPtr != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(uint8, COM_CODE) Com_RxSignalIf_ReceiveSignalProcessing(Com_RxAccessInfoIterType SignalId, P2VAR(void, AUTOMATIC, AUTOMATIC) SignalDataPtr);

/**********************************************************************************************************************
  Com_RxSignalIf_ReceiveDynSignalProcessing
**********************************************************************************************************************/
/*! \brief          This function copies the content of the associated signal buffer to the passed SignalDataPtr for
                    a UINT8_DYN signal.
    \details        -
    \pre            Signal must be a UINT8_DYN signal.
    \param[in]      SignalId       ID of signal to be received.
    \param[out]     SignalDataPtr  Pointer to the buffer where the received signal shall be stored.
    \param[in,out]  Length         in: maximum length that could be received, out: length of the dynamic length signal
    \return         uint8
                    E_OK                        service has been accepted
                    COM_SERVICE_NOT_AVAILABLE   corresponding I-PDU group was stopped
                                                (or service failed due to development error)
                    E_NOT_OK                    the passed buffer is too small to store the received signal
    \context        TASK|ISR2
    \synchronous    TRUE
    \reentrant      TRUE, for different handles
    \spec
      requires SignalId < Com_GetSizeOfRxAccessInfo();
      requires SignalDataPtr != NULL_PTR;
      requires Length != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(uint8, COM_CODE) Com_RxSignalIf_ReceiveDynSignalProcessing(Com_RxAccessInfoIterType SignalId, P2VAR(void, AUTOMATIC, AUTOMATIC) SignalDataPtr, P2VAR(uint16, AUTOMATIC, AUTOMATIC) Length);

/**********************************************************************************************************************
  Com_RxSignalIf_ReceiveSignalGroupProcessing
**********************************************************************************************************************/
/*! \brief        This function copies the content of the associated shadow buffer to the associated signal buffer.
                  After this call, all group signals can be read by a call of Com_ReceiveSignal.
    \details      -
    \pre          -
    \param[in]    SignalGroupId       ID of signal group to be received.
    \return       uint8
                  E_OK                        service has been accepted
                  COM_SERVICE_NOT_AVAILABLE   corresponding I-PDU group was stopped
                                             (or service failed due to development error)
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires SignalGroupId < Com_GetSizeOfRxSigGrpInfo();
    \endspec
**********************************************************************************************************************/
FUNC(uint8, COM_CODE) Com_RxSignalIf_ReceiveSignalGroupProcessing(Com_RxSigGrpInfoIterType SignalGroupId);

/**********************************************************************************************************************
  Com_RxSignalIf_ReceiveSignalGroupArrayProcessing
**********************************************************************************************************************/
/*! \brief        This function copies the content of the associated signal group array to the SignalGroupArrayPtr.
    \details      -
    \pre          Signal Group Array Access must be enabled for the Signal Group.
    \param[in]    SignalGroupId       ID of signal group to be received.
    \param[out]   SignalGroupArrayPtr Pointer to the buffer where the received signal group array shall be stored.
    \return       uint8
                  E_OK                        service has been accepted
                  COM_SERVICE_NOT_AVAILABLE   corresponding I-PDU group was stopped
                                              (or service failed due to development error)
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires SignalGroupId < Com_GetSizeOfRxSigGrpInfo();
      requires SignalGroupArrayPtr != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(uint8, COM_CODE) Com_RxSignalIf_ReceiveSignalGroupArrayProcessing(Com_RxSigGrpInfoIterType SignalGroupId, P2VAR(uint8, AUTOMATIC, AUTOMATIC) SignalGroupArrayPtr);

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_RXSIGNALIF_H */
/**********************************************************************************************************************
  END OF FILE: Com_RxSignalIf.h
**********************************************************************************************************************/
