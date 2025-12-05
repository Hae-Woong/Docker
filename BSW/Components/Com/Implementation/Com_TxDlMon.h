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
/*!        \file  Com_TxDlMon.h
 *         \unit  TxDlMon
 *        \brief  MICROSAR Communication header file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_TXDLMON_H)
# define COM_TXDLMON_H
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
  Com_TxDlMon_MainFunctionTx
**********************************************************************************************************************/
/*! \brief        This function performs the Tx Deadline Monitoring.
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
FUNC(void, COM_CODE) Com_TxDlMon_MainFunctionTx(Com_MainFunctionTxStructIterType mainFunctionId);

/**********************************************************************************************************************
  Com_TxDlMon_TxConfirmation
**********************************************************************************************************************/
/*! \brief        This function stops the tx deadline monitoring after a successful transmission.
    \details      -
    \pre          -
    \param[in]    TxPduId Handle of the I-PDU.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires TxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxDlMon_TxConfirmation(Com_TxPduInfoIterType TxPduId);

/**********************************************************************************************************************
  Com_TxDlMon_IPduGroupStart
**********************************************************************************************************************/
/*! \brief        This function initializes the tx timeout values of the passed tx I-PDU
    \details      -
    \pre          -
    \param[in]    TxPduId Handle of the I-PDU.
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires TxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxDlMon_IPduGroupStart(Com_TxPduInfoIterType TxPduId);

/**********************************************************************************************************************
  Com_TxDlMon_InitTxMode
**********************************************************************************************************************/
/*! \brief        This function reset the tx timeout counter of the passed tx I-PDU
    \details      -
    \pre          -
    \param[in]    TxPduId Handle of the I-PDU.
    \trace        SPEC-2736879
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires TxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxDlMon_InitTxMode(Com_TxPduInfoIterType TxPduId);

/**********************************************************************************************************************
  Com_TxDlMon_Event
**********************************************************************************************************************/
/*! \brief        This function is called when a Tx Timeout occurs.
    \details      -
    \pre          -
    \param[in]    idxTxTOutInfo Tx Timeout Info idx.
    \trace        SPEC-2736820
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxTxTOutInfo < Com_GetSizeOfTxTOutInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxDlMon_Event(Com_TxTOutInfoIterType idxTxTOutInfo);

/**********************************************************************************************************************
  Com_TxDlMon_StartTxDlMonCounter
**********************************************************************************************************************/
/*! \brief        This method reloads the timer for normal Tx Deadline Monitoring if the timer is not already running.
    \details      -
    \pre          Function shall only be called in Normal Mode of Tx Deadline monitoring.
    \param[in]    ComTxPduId                ID of Tx I-PDU.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different Handles
    \trace        SPEC-5131191
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxDlMon_StartTxDlMonCounter(Com_TxPduInfoIterType ComTxPduId);

/**********************************************************************************************************************
  Com_TxDlMon_ReloadTxDlMonCounter
**********************************************************************************************************************/
/*! \brief        This method reloads the timer for normal Tx Deadline Monitoring.
    \details      -
    \pre          Function shall only be called in Normal Mode of Tx Deadline monitoring.
    \param[in]    ComTxPduId                ID of Tx I-PDU.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different Handles
    \trace        SPEC-2736884
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxDlMon_ReloadTxDlMonCounter(Com_TxPduInfoIterType ComTxPduId);

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_TXDLMON_H */
/**********************************************************************************************************************
  END OF FILE: Com_TxDlMon.h
**********************************************************************************************************************/
