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
/*!        \file  Com_TxSignalFiltering.h
 *         \unit  TxSignalFiltering
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_TXSIGNALFILTERING_H)
# define COM_TXSIGNALFILTERING_H

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
  Com_TxSignalFiltering_InitTxFilter
**********************************************************************************************************************/
/*! \brief        Set the old filter value of all signal and groups signals to the initial value and set the tx filter
                  states to their initial state.
    \details      -
    \pre          -
    \param[in]    ComTxPduId ID of Tx I-PDU.
    \context      TASK
    \reentrant    TRUE, for different handles
    \synchronous  TRUE
    \trace        SPEC-2736775
    \spec
      requires ComPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxSignalFiltering_InitTxFilter(Com_TxPduInfoIterType ComPduId);

/**********************************************************************************************************************
  Com_TxSignalFiltering_SetTxSignalFilterOldValueX
**********************************************************************************************************************/
/*! \brief        This method sets the old filter value of a signal to the given value in SignalDataPtr.
    \details      -
    \pre          -
    \param[in]    idxTxSigInfo      Index of Tx Signal Info
    \param[in]    SignalDataPtr     Reference to the signal data
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxTxSigInfo < Com_GetSizeOfTxSigInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxSignalFiltering_SetTxSignalFilterOldValueX(Com_TxSigInfoIterType idxTxSigInfo, P2CONST(void, AUTOMATIC, AUTOMATIC) SignalDataPtr);

/**********************************************************************************************************************
  Com_TxSignalFiltering_SetTxGroupSignalFilterOldValueX
**********************************************************************************************************************/
/*! \brief        This method sets the old filter value of a group signal to the given value in SignalDataPtr.
    \details      -
    \pre          -
    \param[in]    idxTxGrpSigInfo   Index of Tx Group Signal Info
    \param[in]    SignalDataPtr     Reference to the group signal data
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxTxGrpSigInfo < Com_GetSizeOfTxGrpSigInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxSignalFiltering_SetTxGroupSignalFilterOldValueX(Com_TxGrpSigInfoIterType idxTxGrpSigInfo, P2CONST(void, AUTOMATIC, AUTOMATIC) SignalDataPtr);

/**********************************************************************************************************************
  Com_TxSignalFiltering_TransmissionModeSignalFilterCheck
**********************************************************************************************************************/
/*! \brief        This function evaluates the state of the signal filters for the transmission mode evaluation.
    \details      -
    \pre          -
    \param[in]    idxTxSigInfo        Index of TxSigInfo
    \param[in]    SignalDataPtr       Reference to the signal data
    \return       boolean
                    TRUE  if filter condition is passed
                    FALSE otherwise
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxTxSigInfo < Com_GetSizeOfTxSigInfo();
    \endspec
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_TxSignalFiltering_TransmissionModeSignalFilterCheck(Com_TxSigInfoIterType idxTxSigInfo, P2CONST(void, AUTOMATIC, AUTOMATIC) SignalDataPtr);

/**********************************************************************************************************************
  Com_TxSignalFiltering_SignalTransferPropertySignalFilterCheck
**********************************************************************************************************************/
/*! \brief        This function evaluates the state of the signal filters for the signal transfer property evaluation.
    \details      -
    \pre          -
    \param[in]    idxTxSigInfo        Index of TxSigInfo
    \param[in]    SignalDataPtr       Reference to the signal data
    \return       boolean
                    TRUE  if filter condition is passed
                    FALSE otherwise
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxTxSigInfo < Com_GetSizeOfTxSigInfo();
    \endspec
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_TxSignalFiltering_SignalTransferPropertySignalFilterCheck(Com_TxSigInfoIterType idxTxSigInfo, P2CONST(void, AUTOMATIC, AUTOMATIC) SignalDataPtr);

/**********************************************************************************************************************
  Com_TxSignalFiltering_TransmissionModeGroupSignalFilterCheck
**********************************************************************************************************************/
/*! \brief        This function evaluates the state of the group signal filters for the transmission mode evaluation.
    \details      -
    \pre          -
    \param[in]    idxTxGrpSigInfo     Index of TxGrpSigInfo
    \param[in]    SignalDataPtr       Reference to the signal data
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxTxGrpSigInfo < Com_GetSizeOfTxGrpSigInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxSignalFiltering_TransmissionModeGroupSignalFilterCheck(Com_TxGrpSigInfoIterType idxTxGrpSigInfo, P2CONST(void, AUTOMATIC, AUTOMATIC) SignalDataPtr);

/**********************************************************************************************************************
  Com_TxSignalFiltering_SignalTransferPropertyGroupSignalFilterCheck
**********************************************************************************************************************/
/*! \brief        This function evaluates the state of the group signal filters for the signal transfer property evaluation.
    \details      -
    \pre          -
    \param[in]    idxTxGrpSigInfo     Index of TxGrpSigInfo
    \param[in]    SignalDataPtr       Reference to the signal data
    \return       boolean
                    TRUE  if filter condition is passed
                    FALSE otherwise
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxTxGrpSigInfo < Com_GetSizeOfTxGrpSigInfo();
    \endspec
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_TxSignalFiltering_SignalTransferPropertyGroupSignalFilterCheck(Com_TxGrpSigInfoIterType idxTxGrpSigInfo, P2CONST(void, AUTOMATIC, AUTOMATIC) SignalDataPtr);

/**********************************************************************************************************************
  Com_TxSignalFiltering_SignalGroupArrayEvaluateFilter
**********************************************************************************************************************/
/*! \brief        This function evaluates the state of the signal group array filter.
    \details      -
    \pre          -
    \param[in]    idxTxSigGrpInfo     Index of TxSigGrpInfo
    \param[in]    SignalDataPtr       Reference to the signal data
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxTxSigGrpInfo < Com_GetSizeOfTxSigGrpInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxSignalFiltering_SignalGroupArrayEvaluateFilter(Com_TxSigGrpInfoIterType idxTxSigGrpInfo, P2CONST(uint8, AUTOMATIC, AUTOMATIC) SignalGroupArrayPtr);

/**********************************************************************************************************************
  Com_TxSignalFiltering_IsFilterStateTrue
**********************************************************************************************************************/
/*! \brief        Iterate over all related filter states. If at least one filter state is true, the overall filter state
                  of the Pdu is true.
    \details      -
    \pre          -
    \param[in]    idxTxPduInfo  ID of TxPduInfo.
    \return       boolean
                    TRUE  if at least one filter state is true
                    FALSE otherwise
    \context      TASK|ISR2
    \reentrant    TRUE, for different handles
    \synchronous  TRUE
    \spec
      requires idxTxPduInfo < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_TxSignalFiltering_IsFilterStateTrue(Com_TxPduInfoIterType idxTxPduInfo);

/**********************************************************************************************************************
  Com_TxSignalFiltering_HasTransmissionModeFilter
**********************************************************************************************************************/
/*! \brief        Check if at least one signal or group signal in the Pdu has a filter.
    \details      -
    \pre          -
    \param[in]    idxTxPduInfo  ID of TxPduInfo.
    \return       boolean       TRUE, if at least one filter is present, FALSE otherwise.
    \context      TASK|ISR2
    \reentrant    TRUE, for different handles
    \synchronous  TRUE
    \spec
      requires idxTxPduInfo < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_TxSignalFiltering_HasTransmissionModeFilter(Com_TxPduInfoIterType idxTxPduInfo);

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_TXSIGNALFILTERING_H */
/**********************************************************************************************************************
  END OF FILE: Com_TxSignalFiltering.h
**********************************************************************************************************************/
