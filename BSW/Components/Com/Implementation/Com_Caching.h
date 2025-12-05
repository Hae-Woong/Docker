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
/*!        \file  Com_Caching.h
 *         \unit  Caching
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_CACHING_H)
# define COM_CACHING_H

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
  Com_Caching_InitStackBasedCache
**********************************************************************************************************************/
/*! \brief              This function initializes the passed cache.
    \details            Caches that are not contained in the gendata are initialized using this function.
                        Passing a cacheMaxSize of 0 marks the cache as invalid.
    \pre                -
    \param[in,out]      cacheStrctPtr       Pointer to Struct that describes the cache (cache location/size/index/overflow/numberOfElementsRead)
                                            that should be processed.
    \param[in]          ptrToCache          Location of the cache in memory.
    \param[in]          cacheMaxSize        The number of elements that can be stored in the cache.
    \context            TASK|ISR2
    \synchronous        TRUE
    \reentrant          TRUE, for different cacheStrctPtr
    \spec
      requires cacheStrctPtr != NULL_PTR;
      requires ptrToCache != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Caching_InitStackBasedCache(CONSTP2VAR(ComCacheType, AUTOMATIC, AUTOMATIC) cacheStrctPtr, CONSTP2VAR(uint32, AUTOMATIC, AUTOMATIC) ptrToCache, uint32 cacheMaxSize);

/**********************************************************************************************************************
  Com_Caching_Push
**********************************************************************************************************************/
/*! \brief              This function puts the passed value on top of the passed cache.
    \details            -
    \pre                -
    \param[in,out]      cacheStrctPtr       Pointer to Struct that describes the cache (cache location/size/index/overflow/numberOfElementsRead)
                                            that should be processed.
    \param[in]          value               Value to be stored in the cache.
    \context            TASK|ISR2
    \synchronous        TRUE
    \reentrant          TRUE, for different cacheStrctPtr
    \spec
      requires cacheStrctPtr != NULL_PTR;
      requires cacheStrctPtr->ptrToCache != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Caching_Push(CONSTP2VAR(ComCacheType, AUTOMATIC, AUTOMATIC) cacheStrctPtr, uint32 value);

/**********************************************************************************************************************
  Com_Caching_Pop
**********************************************************************************************************************/
/*! \brief              This function removes and returns the top most element from the passed cache.
    \details            -
    \pre                Com_Caching_IsEmpty() must return FALSE
    \param[in,out]      cacheStrctPtr       Pointer to Struct that describes the cache (cache location/size/index/overflow/numberOfElementsRead)
                                            that should be processed.
    \return             uint32              The element that was retrieved from the cache.
    \context            TASK|ISR2
    \synchronous        TRUE
    \reentrant          TRUE, for different cacheStrctPtr
    \spec
      requires cacheStrctPtr != NULL_PTR;
      requires cacheStrctPtr->ptrToCache != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(uint32, COM_CODE) Com_Caching_Pop(CONSTP2VAR(ComCacheType, AUTOMATIC, AUTOMATIC) cacheStrctPtr);

/**********************************************************************************************************************
  Com_Caching_IsEmpty
**********************************************************************************************************************/
/*! \brief              This function checks if the passed cache contains any elements.
    \details            -
    \pre                -
    \param[in]          cacheStrctPtr       Pointer to Struct that describes the cache (cache location/size/index/overflow/numberOfElementsRead)
                                            that should be processed.
    \return             boolean
                        TRUE                There are no elements in the cache.
                        FALSE               There are elements in the cache.
    \context            TASK|ISR2
    \synchronous        TRUE
    \reentrant          TRUE, for different cacheStrctPtr
    \spec
      requires cacheStrctPtr != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_Caching_IsEmpty(CONSTP2CONST(ComCacheType, AUTOMATIC, AUTOMATIC) cacheStrctPtr);

/**********************************************************************************************************************
  Com_Caching_IsOverflowOccurred
**********************************************************************************************************************/
/*! \brief              This function returns and resets the overflowOccurred flag of the passed cache.
    \details            -
    \pre                -
    \param[in,out]      cacheStrctPtr       Pointer to Struct that describes the cache (cache location/size/index/overflow/numberOfElementsRead)
                                            that should be processed.
    \return             boolean
                        TRUE                An overflow has occurred.
                        FALSE               No overflow has occurred.
    \context            TASK|ISR2
    \synchronous        TRUE
    \reentrant          TRUE, for different cacheStrctPtr
    \spec
      requires cacheStrctPtr != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_Caching_IsOverflowOccurred(CONSTP2VAR(ComCacheType, AUTOMATIC, AUTOMATIC) cacheStrctPtr);

/**********************************************************************************************************************
  Com_Caching_IsReadThresholdReached
**********************************************************************************************************************/
/*! \brief              This function checks if the number of read operations on the cache has reached the threshold.
    \details            Since the cache can be fed with new elements during processing of its contents,
                        a finite execution time can not be guaranteed.
                        The read threshold therefore acts as an upper limit to the iterations when processing the cache.
                        The threshold is the number of elements the cache can store.
    \pre                -
    \param[in,out]      cacheStrctPtr       Pointer to Struct that describes the cache (cache location/size/index/overflow/numberOfElementsRead)
                                            that should be processed.
    \return             boolean
                        TRUE                Read Threshold was reached
                        FALSE               Read Threshold was not reached
    \context            TASK|ISR2
    \synchronous        TRUE
    \reentrant          TRUE, for different cacheStrctPtr
    \spec
      requires cacheStrctPtr != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_Caching_IsReadThresholdReached(CONSTP2CONST(ComCacheType, AUTOMATIC, AUTOMATIC) cacheStrctPtr);
#ifdef __PRQA__
#pragma PRQA_NO_SIDE_EFFECTS Com_Caching_IsReadThresholdReached
#endif

/**********************************************************************************************************************
  Com_Caching_ResetNumberOfElementsRead
**********************************************************************************************************************/
/*! \brief              This function reset the numberOfElementsRead.
    \details            -
    \pre                -
    \param[in,out]      cacheStrctPtr       Pointer to Struct that describes the cache (cache location/size/index/overflow/numberOfElementsRead)
    \context            TASK|ISR2
    \synchronous        TRUE
    \reentrant          TRUE, for different cacheStrctPtr
    \spec
      requires cacheStrctPtr != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Caching_ResetNumberOfElementsRead(CONSTP2VAR(ComCacheType, AUTOMATIC, AUTOMATIC) cacheStrctPtr);

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_CACHING_H */
/**********************************************************************************************************************
  END OF FILE: Com_Caching.h
**********************************************************************************************************************/
