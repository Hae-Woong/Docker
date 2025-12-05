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
/*!        \file  Com_Repetition.h
 *         \unit  Repetition
 *        \brief  MICROSAR Communication header file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_REPETITION_H)
# define COM_REPETITION_H

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
  Com_Repetition_Init
**********************************************************************************************************************/
/*! \brief        This function is used to initialize the repetition dependent values.
    \details      -
    \pre          -
    \synchronous  TRUE
    \reentrant    FALSE
    \context      TASK
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Repetition_Init(void);

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
/**********************************************************************************************************************
  Com_Repetition_Start
**********************************************************************************************************************/
/*! \brief        This function starts a transmission with repetitions with loading the repetition cycle counter.
    \details      -
    \pre          -
    \param[in]    repetitionInfoIdx ID of RepetitionInfo.
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \context      TASK|ISR2
    \spec
      requires repetitionInfoIdx < Com_GetSizeOfRepetitionInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Repetition_Start(Com_RepetitionInfoIterType repetitionInfoIdx);
#endif

/**********************************************************************************************************************
  Com_Repetition_StartFirstTransmission
**********************************************************************************************************************/
/*! \brief        This function is used to trigger the first transmission of a transmission with repetitions.
    \details      -
    \pre          -
    \param[in]    repetitionInfoIdx ID of RepetitionInfo.
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \context      TASK|ISR2
    \spec
      requires repetitionInfoIdx < Com_GetSizeOfRepetitionInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Repetition_StartFirstTransmission(Com_RepetitionInfoIterType repetitionInfoIdx);

/**********************************************************************************************************************
  Com_Repetition_Stop
**********************************************************************************************************************/
/*! \brief        This function stops a transmission with repetitions.
    \details      -
    \pre          -
    \param[in]    repetitionInfoIdx ID of RepetitionInfo.
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \context      TASK|ISR2
    \spec
      requires repetitionInfoIdx < Com_GetSizeOfRepetitionInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Repetition_Stop(Com_RepetitionInfoIterType repetitionInfoIdx);

#if (COM_STRICT_REPETITION_PERIOD == STD_ON)
/**********************************************************************************************************************
  Com_Repetition_ResetRepCycle
**********************************************************************************************************************/
/*! \brief        This function sets the repetition cycle counter to the current tx mode dependent value.
    \details      -
    \pre          -
    \param[in]    repetitionInfoIdx ID of RepetitionInfo.
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \context      TASK|ISR2
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Repetition_ResetRepCycle(Com_RepetitionInfoIterType repetitionInfoIdx);
#endif

/**********************************************************************************************************************
  Com_Repetition_IsInProgress
**********************************************************************************************************************/
/*! \brief        This function returns the current status of the Repetitions.
    \details      -
    \pre          -
    \param[in]    repetitionInfoIdx ID of RepetitionInfo.
    \return       TRUE  if a transmission with repetitions is in progress.
                  FALSE if no transmission with repetitions is in progress.
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \context      TASK|ISR2
    \spec
      requires repetitionInfoIdx < Com_GetSizeOfRepetitionInfo();
    \endspec
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_Repetition_IsInProgress(Com_RepetitionInfoIterType repetitionInfoIdx);

/**********************************************************************************************************************
  Com_Repetition_ProcessRepetitions
**********************************************************************************************************************/
/*! \brief        This function processes the repetitions for the passed index.
    \details      -
    \pre          -
    \param[in]    repetitionInfoIdx ID of RepetitionInfo.
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \context      TASK
    \trace        SPEC-2736819
    \spec
      requires repetitionInfoIdx < Com_GetSizeOfRepetitionInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Repetition_ProcessRepetitions(Com_RepetitionInfoIterType repetitionInfoIdx);

/**********************************************************************************************************************
  Com_Repetition_Confirmation
**********************************************************************************************************************/
/*! \brief        This function processes the Com_TxConfirmation for repetitions.
    \details      -
    \pre          -
    \param[in]    repetitionInfoIdx ID of RepetitionInfo.
    \return       TRUE  if all N-Times transmissions have been performed.
                  FALSE if not all N-Times transmissions have been performed.
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \context      TASK|ISR2
    \spec
      requires repetitionInfoIdx < Com_GetSizeOfRepetitionInfo();
    \endspec
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_Repetition_Confirmation(Com_RepetitionInfoIterType repetitionInfoIdx);

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_REPETITION_H */

/**********************************************************************************************************************
  END OF FILE: Com_Repetition.h
**********************************************************************************************************************/
