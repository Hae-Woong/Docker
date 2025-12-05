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
/*!        \file  Com_EventCache.h
 *         \unit  EventCache
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/

#if !defined (COM_EVENTCACHE_H)
# define COM_EVENTCACHE_H

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
  Com_EventCache_Init
**********************************************************************************************************************/
/*! \brief              This function initializes all configured event caches.
    \details            -
    \pre                -
    \context            TASK
    \synchronous        TRUE
    \reentrant          FALSE
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_EventCache_Init(void);

/**********************************************************************************************************************
  Com_EventCache_Put
**********************************************************************************************************************/
/*! \brief              This function puts an element at the end of passed cache (FIFO behavior).
    \details            -
    \pre                This function has to be called inside of an exclusive area context.
    \param[in]          eventCacheIdx       Index of the event cache
    \param[in]          rxPduInfoIdx        Index of the PduInfo to be stored in the event cache
    \return             Std_ReturnType
                        E_OK                The action to put an element into the event cache was successful.
                        E_NOT_OK            The action to put an element into the event cache was not successful.
    \context            TASK|ISR2
    \synchronous        TRUE
    \reentrant          TRUE, for different cacheStrctPtr
**********************************************************************************************************************/
FUNC(Std_ReturnType, COM_CODE) Com_EventCache_Put(Com_CEventCacheIterType eventCacheIdx, Com_RxPduInfoIterType rxPduInfoIdx);

/**********************************************************************************************************************
  Com_EventCache_Get
**********************************************************************************************************************/
/*! \brief              This function returns the next element from the passed cache (FIFO behavior).
    \details            -
    \pre                This function has to be called inside of an exclusive area context.
    \param[in]          eventCacheIdx       Index of the event cache
    \param[in,out]      rxPduInfoIdxPtr     Pointer to be filled with the value of the PduInfo index stored in the event cache
    \return             Std_ReturnType
                        E_OK                The action to get an element from the event cache was successful.
                        E_NOT_OK            The action to get an element from the event cache was not successful.
    \context            TASK|ISR2
    \synchronous        TRUE
    \reentrant          TRUE, for different event cache indices
**********************************************************************************************************************/
FUNC(Std_ReturnType, COM_CODE) Com_EventCache_Get(Com_CEventCacheIterType eventCacheIdx, CONSTP2VAR(Com_RxPduInfoIterType, AUTOMATIC, AUTOMATIC) rxPduInfoIdxPtr);

/**********************************************************************************************************************
  Com_EventCache_IsFull
**********************************************************************************************************************/
/*! \brief              This function checks if the passed event cache is full.
    \details            -
    \pre                This function has to be called inside of an exclusive area context.
    \param[in]          eventCacheIdx       Index of the event cache
    \return             boolean
                        TRUE                Event cache is full.
                        FALSE               Event cache is not full.
    \context            TASK|ISR2
    \synchronous        TRUE
    \reentrant          TRUE, for different event cache indices
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_EventCache_IsFull(Com_CEventCacheIterType eventCacheIdx);

/**********************************************************************************************************************
  Com_EventCache_Flush
**********************************************************************************************************************/
/*! \brief              This function flushes the event cache.
    \details            -
    \pre                -
    \param[in]          eventCacheIdx       Index of the event cache
    \context            TASK|ISR2
    \synchronous        TRUE
    \reentrant          TRUE, for different event cache indices
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_EventCache_Flush(Com_CEventCacheIterType eventCacheIdx);

/**********************************************************************************************************************
  Com_EventCache_GetReadLimit
**********************************************************************************************************************/
/*! \brief              This function returns the read limit of the the passed cache.
    \details            -
    \pre                -
    \param[in]          eventCacheIdx                                   Index of the event cache
    \return             Com_EventCacheArrayLengthOfEventCacheType       Value of the read limit
    \context            TASK|ISR2
    \synchronous        TRUE
    \reentrant          TRUE, for different event cache indices
**********************************************************************************************************************/
FUNC(Com_EventCacheArrayLengthOfEventCacheType, COM_CODE) Com_EventCache_GetReadLimit(Com_CEventCacheIterType eventCacheIdx);

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_EVENTCACHE_H */
/**********************************************************************************************************************
  END OF FILE: Com_EventCache.h
**********************************************************************************************************************/
