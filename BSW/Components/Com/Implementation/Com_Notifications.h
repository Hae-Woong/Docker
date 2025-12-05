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
/*!        \file  Com_Notifications.h
 *         \unit  Notifications
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/

#if !defined (COM_NOTIFICATIONS_H)
# define COM_NOTIFICATIONS_H

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
  Com_Notifications_InitTxIpdu
**********************************************************************************************************************/
/*! \brief        This function initializes the Notification parameters.
    \details      -
    \pre          -
    \param[in]    ComPduId Handle of the I-PDU.
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ComPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Notifications_InitTxIpdu(Com_TxPduInfoIterType ComPduId);

/**********************************************************************************************************************
  Com_Notifications_MainFunctionTx
**********************************************************************************************************************/
/*! \brief        This function calls all deferred Tx notification functions.
    \details      -
    \pre          -
    \param[in]    mainFunctionId    ID of the calling tx main function.
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different Handles
    \spec
      requires mainFunctionId < Com_GetSizeOfMainFunctionTxStruct();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Notifications_MainFunctionTx(Com_MainFunctionTxStructIterType mainFunctionId);

/**********************************************************************************************************************
  Com_Notifications_Confirmation
**********************************************************************************************************************/
/*! \brief        This method sets the internal confirmation flag or calls the confirmation callback function.
    \details      -
    \pre          -
    \param[in]    ComPduId    ID of Tx I-PDU.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ComPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Notifications_Confirmation(Com_TxPduInfoIterType ComPduId);

/**********************************************************************************************************************
  Com_Notifications_ProcessRxFctPtrCache
**********************************************************************************************************************/
/*! \brief             This function calls all Rx cached function pointer.
    \details           -
    \pre               Should be called without locked interrupts.
    \param[in,out]     fctPtrCacheStrctPtr    Pointer to Struct that contains informations (cache location, cache size, cache index)
                                              about cache which should be processed.
    \context           TASK|ISR2
    \synchronous       TRUE
    \reentrant         TRUE, for different fctPtrCacheStrctPtr
    \spec
      requires fctPtrCacheStrctPtr != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Notifications_ProcessRxFctPtrCache(CONSTP2VAR(ComCacheType, AUTOMATIC, AUTOMATIC) fctPtrCacheStrctPtr);

/**********************************************************************************************************************
  Com_Notifications_CacheOrCallRxCbkFctPtr
**********************************************************************************************************************/
/*! \brief            This caches or calls the function pointer in immediate or deferred cache.
    \details          -
    \pre              -
    \param[in]        reportingIdx        Index corresponding to the  function that should be cached or called.
    \param[in]        comRxPduInfoIdx     Pdu Id
    \param[in,out]    fctPtrCacheStrctPtr contains all information of function pointer cache (location, size, index)
    \context          TASK|ISR2
    \synchronous      TRUE
    \reentrant        TRUE, for different Handles
    \spec
      requires reportingIdx < Com_GetSizeOfReporting();
      requires comRxPduInfoIdx < Com_GetSizeOfRxPduInfo();
      requires fctPtrCacheStrctPtr != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Notifications_CacheOrCallRxCbkFctPtr(Com_ReportingIterType reportingIdx, Com_RxPduInfoIterType comRxPduInfoIdx,
                                                              CONSTP2VAR(ComCacheType, AUTOMATIC, AUTOMATIC) fctPtrCacheStrctPtr);

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_NOTIFICATIONS_H */
/**********************************************************************************************************************
  END OF FILE: Com_Notifications.h
**********************************************************************************************************************/
