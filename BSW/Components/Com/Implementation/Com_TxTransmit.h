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
/*!        \file  Com_TxTransmit.h
 *         \unit  TxTransmit
 *        \brief  MICROSAR Communication header file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_TXTRANSMIT_H)
# define COM_TXTRANSMIT_H

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
  Com_TxTransmit_MainFunctionTx
**********************************************************************************************************************/
/*! \brief        This function iterates over all Tx ComIPdus and process the related transmit requests.
    \details      -
    \pre          -
    \param[in]    mainFunctionId    ID of the calling tx main function.
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires mainFunctionId < Com_GetSizeOfMainFunctionTxStruct();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxTransmit_MainFunctionTx(Com_MainFunctionTxStructIterType mainFunctionId);

/**********************************************************************************************************************
  Com_TxTransmit_SetTransmitRequest
**********************************************************************************************************************/
/*! \brief        This method sets the transmit request.
    \details      -
    \pre          -
    \param[in]    ComTxPduId    ID of Tx I-PDU.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxTransmit_SetTransmitRequest(Com_TxPduInfoIterType ComTxPduId);

/**********************************************************************************************************************
  Com_TxTransmit_ClearTransmitRequest
**********************************************************************************************************************/
/*! \brief        This method clears the transmit request.
    \details      -
    \pre          -
    \param[in]    ComTxPduId    ID of Tx I-PDU.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxTransmit_ClearTransmitRequest(Com_TxPduInfoIterType ComTxPduId);

/**********************************************************************************************************************
  Com_TxTransmit_ClearInitialTransmitFlag
**********************************************************************************************************************/
/*! \brief        This method clears the initial transmit request flag.
    \details      -
    \pre          -
    \param[in]    ComTxPduId    ID of Tx I-PDU.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxTransmit_ClearInitialTransmitFlag(Com_TxPduInfoIterType ComTxPduId);

/**********************************************************************************************************************
  Com_TxTransmit_IsInitialTransmitFlag
**********************************************************************************************************************/
/*! \brief        This method returns the current state of the initial transmit request flag.
    \details      -
    \pre          -
    \param[in]    ComTxPduId    ID of Tx I-PDU.
    \return       current state of initial transmit request flag.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_TxTransmit_IsInitialTransmitFlag(Com_TxPduInfoIterType ComTxPduId);

# if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
/**********************************************************************************************************************
  Com_TxTransmit_DesGwTransmit
**********************************************************************************************************************/
/*! \brief        This function initiates the transmission of txPdu on gateway description event
    \details      -
    \pre          -
    \param[in]    ComTxPduId                Valid ID of TxPdu that should be sent
    \param[in]    isImmediate               true, if TxPdu should be sent immediately, otherwise transmission will be triggered
                                            in next main function cycle
    \param[in]    TriggerWithoutRepetition  true, if no repetitions are required
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec    
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxTransmit_DesGwTransmit(Com_TxPduInfoIterType ComTxPduId, boolean isImmediate, boolean TriggerWithoutRepetition);
# endif

/**********************************************************************************************************************
  Com_TxTransmit_ClearDirectTrigger
**********************************************************************************************************************/
/*! \brief        This method clears the direct trigger flag.
    \details      -
    \pre          -
    \param[in]    ComTxPduId    ID of Tx I-PDU.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxTransmit_ClearDirectTrigger(Com_TxPduInfoIterType ComTxPduId);

/**********************************************************************************************************************
  Com_TxTransmit_IsDirectTrigger
**********************************************************************************************************************/
/*! \brief        This method returns the current state of the direct trigger flag.
    \details      -
    \pre          -
    \param[in]    ComTxPduId    ID of Tx I-PDU.
    \return       current state of initial transmit request flag.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_TxTransmit_IsDirectTrigger(Com_TxPduInfoIterType ComTxPduId);

/**********************************************************************************************************************
  Com_TxTransmit_Confirmation
**********************************************************************************************************************/
/*! \brief        This method serves to start the delay time counter of the dependent I-PDU.
    \details      -
    \pre          -
    \param[in]    ComTxPduId    ID of Tx I-PDU.
    \trace        SPEC-2736859
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxTransmit_Confirmation(Com_TxPduInfoIterType ComTxPduId);

/**********************************************************************************************************************
  Com_TxTransmit_TriggerDeferredTransmitAndSetTimer
**********************************************************************************************************************/
/*! \brief        This method sets the internal transmit request flag and the I-PDU is transmitted once or multiple.
                  Further, the timer for normal Tx Deadline Monitoring is reloaded.
                  The I-PDU Transmit request is performed with the next call of Com_TxModeHdlr_MainFunction.
    \details      -
    \pre          -
    \param[in]    ComTxPduId                ID of Tx I-PDU.
    \param[in]    TriggerWithoutRepetition  Decide whether configured repetitions are taken into account.
                                        TRUE:  I-PDU is triggered just once without a repetition.
                                        FALSE: Configured repetitions are taken into account.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different Handles
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxTransmit_TriggerDeferredTransmitAndSetTimer(Com_TxPduInfoIterType ComTxPduId, boolean TriggerWithoutRepetition);

/**********************************************************************************************************************
  Com_TxTransmit_TriggerIpduSend
**********************************************************************************************************************/
/*! \brief        This method sets the internal transmit request flag.
                  The I-PDU Transmit request is performed with the next call of Com_TxModeHdlr_MainFunction.
    \details      -
    \pre          -
    \param[in]    ComTxPduId  ID of Tx I-PDU.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxTransmit_TriggerIpduSend(Com_TxPduInfoIterType ComTxPduId);

/**********************************************************************************************************************
  Com_TxTransmit_TriggerIpduSendWithMetaData
**********************************************************************************************************************/
/*! \brief        This method sets the internal transmit request flag.
                  The I-PDU Transmit request is performed with the next call of Com_TxModeHdlr_MainFunction.
    \details      -
    \pre          -
    \param[in]    ComTxPduId  ID of Tx I-PDU.
    \param[in]    MetaData    Pointer to the MetaData.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
      requires MetaData != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxTransmit_TriggerIpduSendWithMetaData(Com_TxPduInfoIterType ComTxPduId, P2CONST(uint8, AUTOMATIC, AUTOMATIC) MetaData);

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_TXTRANSMIT_H */
/**********************************************************************************************************************
  END OF FILE: Com_TxTransmit.h
**********************************************************************************************************************/
