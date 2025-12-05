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
/*!       \file  Can_30_Core_DiagnosticHandler.h
 *        \brief  Can_30_Core_DiagnosticHandler header file
 *        \details  Header of the Diagnostic handler unit. It provides the API declaration for this unit used by other units.
 *        \unit Can_30_Core_DiagnosticHandler
 *********************************************************************************************************************/
#if !defined(CAN_30_CORE_DIAGNOSTICHANDLER_H)
# define CAN_30_CORE_DIAGNOSTICHANDLER_H

/**********************************************************************************************************************
 * LOCAL MISRA JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA  S 2880, 2991, 2992, 2994, 2995, 2996 EOF */ /* MD_Can_30_Core_ConstValue */
/* PRQA  S 2741, 2742 EOF */ /* MD_CSL_ReducedToDefine */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Can_30_Core_PrivateTypes.h"
# include "Can_30_Core_Types.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
/* Even Functions intended to be inlined shall be declared and defined within a MEMMAP Section */
# define CAN_30_CORE_START_SEC_CODE
# include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 * Can_30_Core_IsControllerValid()
 ***********************************************************************************************************************/
/*! \brief        Returns if the passed controller is valid.
 *  \details      -
 *  \param[in]    Controller   The requested controller.
 *  \return       FALSE        Controller is NOT valid.
 *  \return       TRUE         Controller is valid.
 *  \pre          -
 *  \context      ANY
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 ***********************************************************************************************************************/
CAN_30_CORE_LOCAL_INLINE FUNC(boolean, CAN_30_CORE_STATIC_CODE) Can_30_Core_IsControllerValid(uint8 Controller);

# define CAN_30_CORE_STOP_SEC_CODE
# include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 * IMPLEMENTATION
 ***********************************************************************************************************************/
/* Even Functions intended to be inlined shall be declared and defined within a MEMMAP Section */
# define CAN_30_CORE_START_SEC_CODE
# include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# ifndef CAN_NOUNIT_CAN_30_CORE_DIAGNOSTICHANDLER     /* COV_CAN_30_CORE_UNUSED_PREPROCESSOR */
/***********************************************************************************************************************
 * Can_30_Core_IsControllerValid()
 ***********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_IsControllerValid
  Relation_Context:
  Relation:
DevErrorDetect, ChannelAmount, Variant
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 */
CAN_30_CORE_LOCAL_INLINE FUNC(boolean, CAN_30_CORE_STATIC_CODE) Can_30_Core_IsControllerValid( /* PRQA S 3219 */ /* MD_Can_30_Core_NotUsedInUnit */
  uint8 Controller)
{
  boolean retval = TRUE;
  /* ----- Implementation ----------------------------------------------- */
  if (Controller >= Can_30_Core_GetSizeOfControllerConfig())
  { /* #100 Check for valid controller parameter */
    retval = FALSE;
  }
  else if (Can_30_Core_IsPostBuildSelectableOfGeneralFeature(0) && (Can_30_Core_GetCanIfChannelId(Controller) == CAN_30_CORE_NO_CANIFCHANNELID)) /* PRQA S 2004 */ /* MD_Can_30_Core_NoElseAfterIf */
  { /* #110 Check if the controller is inactive */
    retval = FALSE;
  }
  return retval;
}
# endif /* CAN_NOUNIT_CAN_30_CORE_DIAGNOSTICHANDLER */

# define CAN_30_CORE_STOP_SEC_CODE
# include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* CAN_30_CORE_DIAGNOSTICHANDLER_H */

/**********************************************************************************************************************
 *  END OF FILE: Can_30_Core_DiagnosticHandler.h
 *********************************************************************************************************************/
