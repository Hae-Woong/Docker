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
/*!        \file  SoAd_State.h
 *         \unit  State
 *        \brief  Header file of the State unit for AUTOSAR Socket Adaptor module.
 *      \details  Interface and type definitions of the State unit.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's main source file.
 *********************************************************************************************************************/

#if !defined (SOAD_STATE_H)
# define SOAD_STATE_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "SoAd_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

 /**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define SOAD_START_SEC_CODE
# include "SoAd_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  SoAd_InitMemory()
 *********************************************************************************************************************/
/*! \brief       Initializes *_CLEARED_*-variables.
 *  \details     Service to initialize module global variables at power up. This function initializes the
 *               variables in *_CLEARED_* sections. Used in case they are not initialized by the startup code.
 *  \pre         Module is uninitialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_InitMemory(void);

/**********************************************************************************************************************
 *  SoAd_PreInit()
 *********************************************************************************************************************/
/*! \brief       Pre-Initializes module.
 *  \details     This function initializes the shared memory of SoAd module and sets the module to the pre-initialized
 *               state.
 *  \param[in]   SoAdConfigPtr            Configuration structure for initializing the module.
 *  \pre         Interrupts are disabled.
 *  \pre         SoAd_InitMemory has been called unless the *_CLEARED_*-variables are initialized by start-up code.
 *  \pre         Module is uninitialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        SoAd must not be initialized during runtime. The initialization is only allowed in uninit and shutdown
 *               state.
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_PreInit(
  P2CONST(SoAd_ConfigType, AUTOMATIC, SOAD_CONST) SoAdConfigPtr);

/**********************************************************************************************************************
 *  SoAd_Init()
 *********************************************************************************************************************/
/*! \brief       Initializes module.
 *  \details     This function initializes the partition-specific memory of SoAd module of the application context this
 *               function is called in and sets the partition-specific initialized state.
 *  \param[in]   SoAdConfigPtr            Configuration structure for initializing the module. This parameter is not
 *                                        used since it is already provided in context of SoAd_PreInit().
 *  \pre         Interrupts are disabled.
 *  \pre         SoAd_PreInit() has been called.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        SoAd must not be initialized during runtime. The initialization is only allowed in uninit and shutdown
 *               state. In multi-partition use-case: This function has to be called once in each partition context.
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_Init(
  P2CONST(SoAd_ConfigType, AUTOMATIC, SOAD_CONST) SoAdConfigPtr);

/**********************************************************************************************************************
 *  SoAd_PostInit()
 *********************************************************************************************************************/
/*! \brief       Post-Initializes module.
 *  \details     This function checks the initialization state of all partitions and sets the global initialized state.
 *  \pre         SoAd_Init() has been called on each partition.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        SoAd must not be initialized during runtime. The initialization is only allowed in uninit and shutdown
 *               state. This function must be called only once and it must be called on the main partition in case of
 *               multi-partition.
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_PostInit(void);

/**********************************************************************************************************************
 *  SoAd_Shutdown()
 *********************************************************************************************************************/
/*! \brief       Shuts down SoAd module.
 *  \details     Closes all open socket connections and disables transmission and reception.
 *  \return      E_OK               Shutdown request was accepted.
 *  \return      SOAD_E_INPROGRESS  Shutdown is in progress.
 *  \return      E_NOT_OK           Shutdown request was not accepted.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   FALSE
 *  \synchronous FALSE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SOAD_CODE) SoAd_Shutdown(void);

# define SOAD_STOP_SEC_CODE
# include "SoAd_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_MemMap */

#endif /* !defined(SOAD_STATE_H) */

/**********************************************************************************************************************
 *  END OF FILE: SoAd_State.h
 *********************************************************************************************************************/
