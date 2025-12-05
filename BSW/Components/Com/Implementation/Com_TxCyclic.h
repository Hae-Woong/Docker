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
/*!        \file  Com_TxCyclic.h
 *         \unit  TxCyclic
 *        \brief  MICROSAR Communication header file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_TXCYCLIC_H)
# define COM_TXCYCLIC_H

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

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
/**********************************************************************************************************************
Com_TxCyclic_StartCyclicTransmission
**********************************************************************************************************************/
/*! \brief        This method starts the cyclic transmission of an I-PDU.
    \details      -
    \pre          -
    \param[in]    ComTxPduId    ID of Tx I-PDU.
    \context      TASK|ISR
    \synchronous  TRUE
    \reentrant    TRUE, for different Handles
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxCyclic_StartCyclicTransmission(Com_TxPduInfoIterType ComTxPduId);
#endif

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
/**********************************************************************************************************************
Com_TxCyclic_StopCyclicTransmission
**********************************************************************************************************************/
/*! \brief        This method stops the cyclic transmission of an I-PDU, cancels outstanding transmission requests of
                  an I-PDU and reloads the time offset counter.
    \details      -
    \pre          -
    \param[in]    ComTxPduId    ID of Tx I-PDU.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different Handles
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxCyclic_StopCyclicTransmission(Com_TxPduInfoIterType ComTxPduId);
#endif

/**********************************************************************************************************************
  Com_TxCyclic_StopCyclicSendRequest
**********************************************************************************************************************/
/*! \brief        This method stops the cyclic send request.
    \details      -
    \pre          -
    \param[in]    ComTxPduId    ID of Tx I-PDU.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different Handles
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxCyclic_StopCyclicSendRequest(Com_TxPduInfoIterType ComTxPduId);

/**********************************************************************************************************************
  Com_TxCyclic_ProcessCyclicPdu
**********************************************************************************************************************/
/*! \brief        This function processes the cyclic transmission for the passed ComTxPdu.
    \details      -
    \pre          -
    \param[in]    ComTxPduId ID of Tx I-PDU.
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different Handles
    \trace        SPEC-2736819
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxCyclic_ProcessCyclicPdu(Com_TxPduInfoIterType ComTxPduId);

/**********************************************************************************************************************
  Com_TxCyclic_IsCyclicSendRequest
**********************************************************************************************************************/
/*! \brief        This method checks if there is a Cyclic Send Request.
    \details      -
    \pre          -
    \param[in]    ComTxPduId    ID of Tx I-PDU.
    \return       boolean
                    TRUE    if cyclic send request.
                    FALSE   if no cyclic send request.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different Handles
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_TxCyclic_IsCyclicSendRequest(Com_TxPduInfoIterType ComTxPduId);

/**********************************************************************************************************************
  Com_TxCyclic_SetCycleTimeToCurrentTimeOffset
**********************************************************************************************************************/
/*! \brief        This function sets the cycle time to the current time offset.
    \details      -
    \pre          -
    \param[in]    ComTxPduId ID of Tx I-PDU.
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different Handles
    \trace        SPEC-2736819
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxCyclic_SetCycleTimeToCurrentTimeOffset(Com_TxPduInfoIterType ComTxPduId);

/**********************************************************************************************************************
  Com_TxCyclic_InitCyclicTxMode
**********************************************************************************************************************/
/*! \brief        This function sets the cycle time to the current time period and adds a minimum delay time value.
    \details      -
    \pre          -
    \param[in]    ComTxPduId ID of Tx I-PDU.
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different Handles
    \trace        SPEC-2736819, SPEC-2736812
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxCyclic_InitCyclicTxMode(Com_TxPduInfoIterType ComTxPduId);

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_TXCYCLIC_H */

/**********************************************************************************************************************
  END OF FILE: Com_TxCyclic.h
**********************************************************************************************************************/
