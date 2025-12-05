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
/*!        \file  Com_ISRThreshold.h
 *         \unit  ISRThreshold
 *        \brief  MICROSAR Communication header file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_ISRTHRESHOLD_H)
# define COM_ISRTHRESHOLD_H

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
  Com_ISRThreshold_Init
**********************************************************************************************************************/
/*! \brief          This function initializes the threshold counter values.
    \details        -
    \pre            -
    \trace          DSGN-COM_ISRTHRESHOLD_CONCEPT
    \synchronous    TRUE
    \reentrant      FALSE
    \context        TASK
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_ISRThreshold_Init(void);

/**********************************************************************************************************************
  Com_ISRThreshold_EnterExclusiveArea
**********************************************************************************************************************/
/*! \brief          This function enters the exclusive area.
    \details        -
    \pre            -
    \param[in]      isrThresholdInfoIdx
    \trace          DSGN-COM_ISRTHRESHOLD_CONCEPT
    \synchronous    TRUE
    \reentrant      TRUE, for different handles
    \context        TASK
    \spec
      requires isrThresholdInfoIdx < Com_GetSizeOfISRThresholdInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_ISRThreshold_EnterExclusiveArea(Com_ISRThresholdInfoIterType isrThresholdInfoIdx);

/**********************************************************************************************************************
  Com_ISRThreshold_ExitExclusiveArea
**********************************************************************************************************************/
/*! \brief          This function exits the exclusive area.
    \details        -
    \pre            -
    \param[in]      isrThresholdInfoIdx
    \trace          DSGN-COM_ISRTHRESHOLD_CONCEPT
    \synchronous    TRUE
    \reentrant      TRUE, for different handles
    \context        TASK
    \spec
      requires isrThresholdInfoIdx < Com_GetSizeOfISRThresholdInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_ISRThreshold_ExitExclusiveArea(Com_ISRThresholdInfoIterType isrThresholdInfoIdx);

/**********************************************************************************************************************
  Com_ISRThreshold_ThresholdCheck
**********************************************************************************************************************/
/*! \brief          This function checks if the ISR Lock must be opened.
    \details        -
    \pre            -
    \param[in]      isrThresholdInfoIdx
    \trace          DSGN-COM_ISRTHRESHOLD_CONCEPT
    \synchronous    TRUE
    \reentrant      TRUE, for different handles
    \context        TASK
    \spec
      requires isrThresholdInfoIdx < Com_GetSizeOfISRThresholdInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_ISRThreshold_ThresholdCheck(Com_ISRThresholdInfoIterType isrThresholdInfoIdx);

/**********************************************************************************************************************
  Com_ISRThreshold_DecThresholdCounter
**********************************************************************************************************************/
/*! \brief          This function decrements the threshold counter is possible.
    \details        -
    \pre            -
    \param[in]      isrThresholdInfoIdx
    \return         TRUE  threshold counter was decremented.
                    FALSE threshold counter was already reached.
    \trace          DSGN-COM_ISRTHRESHOLD_CONCEPT
    \synchronous    TRUE
    \reentrant      TRUE, for different handles
    \context        TASK
    \spec
      requires isrThresholdInfoIdx < Com_GetSizeOfISRThresholdInfo();
    \endspec
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_ISRThreshold_DecThresholdCounter(Com_ISRThresholdInfoIterType isrThresholdInfoIdx);

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_ISRTHRESHOLD_H */
/**********************************************************************************************************************
  END OF FILE: Com_ISRThreshold.h
**********************************************************************************************************************/
