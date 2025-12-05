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
/*!        \file  Com_TxMinDelay.h
 *         \unit  TxMinDelay
 *        \brief  MICROSAR Communication header file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_TXMINDELAY_H)
# define COM_TXMINDELAY_H
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
  Com_TxMinDelay_ResetCounter
**********************************************************************************************************************/
/*! \brief        This function set the Minimum Delay Counter to its inital value.
    \details      -
    \pre          -
    \param[in]    TxPduId Handle of the I-PDU.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxMinDelay_ResetCounter(Com_TxPduInfoIterType ComTxPduId);

/**********************************************************************************************************************
  Com_TxMinDelay_IsMDTExpired
**********************************************************************************************************************/
/*! \brief        This function checks if the Minimum Delay Counter is expired.
    \details      -
    \pre          -
    \param[in]    TxPduId Handle of the I-PDU.
    \return       boolean
                    TRUE   if the MDT expired.
                    FALSE  if the MDT has not expired jet.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_TxMinDelay_IsMDTExpired(Com_TxPduInfoIterType ComTxPduId);
#ifdef __PRQA__
#pragma PRQA_NO_SIDE_EFFECTS Com_TxMinDelay_IsMDTExpired
#endif

/**********************************************************************************************************************
  Com_TxMinDelay_GetCurrentMDTValue
**********************************************************************************************************************/
/*! \brief        This function returns the current Minimum Delay Counter value.
    \details      -
    \pre          -
    \param[in]    TxPduId Handle of the I-PDU.
    \return       The current MDT value.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(Com_TimerCounterValueType, COM_CODE) Com_TxMinDelay_GetCurrentMDTValue(Com_TxPduInfoIterType ComTxPduId);

/**********************************************************************************************************************
  Com_TxMinDelay_DecCounter
**********************************************************************************************************************/
/*! \brief        This function decrements the minimum delay time counter.
    \details      -
    \pre          -
    \param[in]    TxPduId Handle of the I-PDU.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxMinDelay_DecCounter(Com_TxPduInfoIterType ComTxPduId);

/**********************************************************************************************************************
  Com_TxMinDelay_StartCounter
**********************************************************************************************************************/
/*! \brief        This function starts the minimum delay time counter.
    \details      -
    \pre          -
    \param[in]    TxPduId Handle of the I-PDU.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxMinDelay_StartCounter(Com_TxPduInfoIterType ComTxPduId);

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_TXMINDELAY_H */
/**********************************************************************************************************************
  END OF FILE: Com_TxMinDelay.h
**********************************************************************************************************************/
