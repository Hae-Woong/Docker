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
/*!        \file  Com_Timer.h
 *         \unit  Timer
 *        \brief  MICROSAR Communication header file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_TIMER_H)
# define COM_TIMER_H

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
  Com_Timer_DecCounter
**********************************************************************************************************************/
/*! \brief        This function decrements the timer counter value and calls the event callout if the decrements goes from 1 to 0.
    \details      -
    \pre          -
    \param[in]    idxTimerInfo timer info handle
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxTimerInfo < Com_GetSizeOfComTimerInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Timer_DecCounter(Com_ComTimerInfoIterType idxTimerInfo);

/**********************************************************************************************************************
  Com_Timer_Init
**********************************************************************************************************************/
/*! \brief        This function initializes all timer counter values.
    \details      -
    \pre          -
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Timer_Init(void);

/**********************************************************************************************************************
  Com_Timer_StartCounter
**********************************************************************************************************************/
/*! \brief        This function loads the timer counter value with the start value.
    \details      -
    \pre          -
    \param[in]    idxTimerInfo timer info handle
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxTimerInfo < Com_GetSizeOfComTimerInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Timer_StartCounter(Com_ComTimerInfoIterType idxTimerInfo);

/**********************************************************************************************************************
  Com_Timer_InitCounter
**********************************************************************************************************************/
/*! \brief        This function loads the timer counter value with the init value.
    \details      -
    \pre          -
    \param[in]    idxTimerInfo timer info handle
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxTimerInfo < Com_GetSizeOfComTimerInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Timer_InitCounter(Com_ComTimerInfoIterType idxTimerInfo);

/**********************************************************************************************************************
  Com_Timer_StopCounter
**********************************************************************************************************************/
/*! \brief        This function stops the timer.
    \details      -
    \pre          -
    \param[in]    idxTimerInfo timer info handle
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxTimerInfo < Com_GetSizeOfComTimerInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Timer_StopCounter(Com_ComTimerInfoIterType idxTimerInfo);

/**********************************************************************************************************************
  Com_Timer_IsTimerRunning
**********************************************************************************************************************/
/*! \brief        This function is used to check if the timer is currently running.
    \details      -
    \pre          -
    \param[in]    idxTimerInfo timer info handle
    \return       boolean
                    TRUE   timer is running.
                    FALSE  timer is stopped.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxTimerInfo < Com_GetSizeOfComTimerInfo();
    \endspec
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_Timer_IsTimerRunning(Com_ComTimerInfoIterType idxTimerInfo);

/**********************************************************************************************************************
  Com_Timer_GetCurrentTimerValue
**********************************************************************************************************************/
/*! \brief        This function returns the current timer value.
    \details      -
    \pre          -
    \param[in]    idxTimerInfo timer info handle
    \return       current timer value.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxTimerInfo < Com_GetSizeOfComTimerInfo();
    \endspec
**********************************************************************************************************************/
FUNC(Com_TimerCounterValueType, COM_CODE) Com_Timer_GetCurrentTimerValue(Com_TimerCounterValueIterType idxTimerInfo);

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_TIMER_H */
/**********************************************************************************************************************
  END OF FILE: Com_Timer.h
**********************************************************************************************************************/
