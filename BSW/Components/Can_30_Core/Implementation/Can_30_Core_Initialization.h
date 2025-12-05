/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!       \file  Can_30_Core_Initialization.h
 *        \brief  Can_30_Core_Initialization header file
 *        \details  Header of the Initialization unit. It provides the API declaration for this unit used by other units.
 *        \unit Can_30_Core_Initialization
 *********************************************************************************************************************/
#if !defined(CAN_30_CORE_INITIALIZATION_H)
#define CAN_30_CORE_INITIALIZATION_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Can_30_Core_PrivateTypes.h"

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/
# define CAN_30_CORE_START_SEC_VAR_INIT_UNSPECIFIED
# include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Pointer to the configuration */
extern P2CONST(Can_30_Core_ConfigType, CAN_30_CORE_VAR_CLEARED, CAN_30_CORE_PBCFG) Can_30_Core_ConfigDataPtr; /* PRQA S 3449,3451 */ /* MD_Can_30_Core_ExternalScope */

/*! Initialization state of the module */
extern VAR(Can_30_Core_InitState, CAN_30_CORE_VAR_INIT) Can_30_Core_GlobalInitState; /* PRQA S 3449,3451 */ /* MD_Can_30_Core_ExternalScope */

# define CAN_30_CORE_STOP_SEC_VAR_INIT_UNSPECIFIED
# include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define CAN_30_CORE_START_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Can_30_Core_IsDriverInitialized()
 *********************************************************************************************************************/
/*! \brief       Checks if the driver is initialized
 *  \details     Checks if the controller is initialized using the global init status
 *  \param[in]   -
 *  \return      FALSE    The driver is not initialized
 *  \return      TRUE     The driver is initialized
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL_INLINE FUNC(boolean, CAN_30_CORE_CODE) Can_30_Core_IsDriverInitialized(void);


/***********************************************************************************************************************
 * Can_30_Core_IsControllerInitialized_Internal()
 ***********************************************************************************************************************/
/*! \brief        Returns if the passed controller is initialized.
 *  \details      -
 *  \param[in]    Controller   The requested controller.
 *  \return       FALSE        Controller is NOT initialized.
 *  \return       TRUE         Controller is initialized.
 *  \pre          Controller has to be valid
 *  \context      ANY
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 ***********************************************************************************************************************/
CAN_30_CORE_LOCAL_INLINE FUNC(boolean, CAN_30_CORE_STATIC_CODE) Can_30_Core_IsControllerInitialized_Internal(uint8 Controller);

#define CAN_30_CORE_STOP_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 * IMPLEMENTATION
 ***********************************************************************************************************************/
/* Even Functions intended to be inlined shall be declared and defined within a MEMMAP Section */
# define CAN_30_CORE_START_SEC_CODE
# include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# ifndef CAN_NOUNIT_CAN_30_CORE_INITIALIZATION     /* COV_CAN_30_CORE_UNUSED_PREPROCESSOR */


/**********************************************************************************************************************
 *  Can_30_Core_IsDriverInitialized()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_IsDriverInitialized
  Relation_Context:
  Relation:
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 */
CAN_30_CORE_LOCAL_INLINE FUNC(boolean, CAN_30_CORE_CODE) Can_30_Core_IsDriverInitialized(void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #100 Return TRUE if the driver is initialized */
  return (Can_30_Core_GlobalInitState == CAN_30_CORE_INITIALIZED);
}


/***********************************************************************************************************************
 * Can_30_Core_IsControllerInitialized_Internal()
 ***********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_IsControllerInitialized_Internal
  Relation_Context: DevErrorDetect, ChannelAmount, Variant
  Relation:
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 */
CAN_30_CORE_LOCAL_INLINE FUNC(boolean, CAN_30_CORE_STATIC_CODE) Can_30_Core_IsControllerInitialized_Internal(uint8 Controller) /* PRQA S 3219 */ /* MD_Can_30_Core_NotUsedInUnit */
{
  return ((Can_30_Core_GetLogStatusOfControllerData(Controller) & CAN_30_CORE_STATUS_INIT) == CAN_30_CORE_STATUS_INIT);
}
# endif /* CAN_NOUNIT_CAN_30_CORE_INITIALIZATION */

# define CAN_30_CORE_STOP_SEC_CODE
# include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* CAN_30_CORE_INITIALIZATION_H */

/**********************************************************************************************************************
 *  END OF FILE: Can_30_Core_Initialization.h
 *********************************************************************************************************************/
