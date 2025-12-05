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
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  LinIf_Master.h
 *    Component:  AUTOSAR LIN INTERFACE
 *       Module:  If_Asr4IfLin
 *    Generator:  If_Asr4IfLin
 *
 *  Description:  Header file of LIN Master specific internal interface.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/

#if !defined (LINIF_MASTER_H)
# define LINIF_MASTER_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "LinIf_Cfg.h"

# if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON )

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#  define LINIF_MASTER_MAJOR_VERSION                  (12u)
#  define LINIF_MASTER_MINOR_VERSION                  (0u)
#  define LINIF_MASTER_PATCH_VERSION                  (0u)

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

#  define LINIF_START_SEC_CODE
#  include "LinIf_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LinIf_MasterChannelMainFunction()
 *********************************************************************************************************************/
/*! \brief       Performs channel specific main function handling.
 *  \details     Performs schedule and message processing for one channel.
 *  \param[in]   LinIfChannel           Internal LinIf channel identifier. Must reference a LIN master channel.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_NODETYPE_MASTER_SUPPORTED
 *********************************************************************************************************************/
FUNC(void, LINIF_CODE) LinIf_MasterChannelMainFunction
(
  VAR(NetworkHandleType, AUTOMATIC)                                   LinIfChannel
);

/**********************************************************************************************************************
 *  LinIf_MasterWakeup()
 *********************************************************************************************************************/
/*! \brief       Requests the transition into operational state for master nodes.
 *  \details     Initiates the wake up process on the selected controller and transmits a wakeup frame.
 *  \param[in]   Channel                Network channel handle. Must reference a LIN master channel.
 *  \param[in]   LinIfChannel           Internal LinIf channel handle. Must reference a LIN master channel.
 *  \param[in]   ChannelDataPtr         Pointer to (general) channel data.
 *  \return      E_OK                   Request to wake up is accepted.
 *  \return      E_NOT_OK               Request to wake up is not accepted.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE for different channels
 *  \synchronous FALSE
 *  \config      LINIF_NODETYPE_MASTER_SUPPORTED
 *********************************************************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinIf_MasterWakeup
(
  VAR(NetworkHandleType, AUTOMATIC)                                   Channel,
  VAR(NetworkHandleType, AUTOMATIC)                                   LinIfChannel,
  CONSTP2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)      ChannelDataPtr
);

/**********************************************************************************************************************
 *  LinIf_FlushAllSchedules()
 *********************************************************************************************************************/
/*! \brief       Resets the referenced schedule table data.
 *  \details     Sets the referenced schedule table data to initial default values. It also sets the NULL
 *               schedule table and sets the collision resolving state to idle.
 *  \param[in]   SchedTabDataPtr     Pointer to schedule table data structure.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_NODETYPE_MASTER_SUPPORTED
 *********************************************************************************************************************/
FUNC(void, LINIF_CODE) LinIf_FlushAllSchedules
(
  CONSTP2VAR(LinIf_SchedTabDataType, AUTOMATIC, LINIF_VAR_NOINIT)     SchedTabDataPtr
);


/**********************************************************************************************************************
 *  LinIf_ChannelMainFunctionPostProcessing()
 *********************************************************************************************************************/
/*! \brief       Notifies the upper layers after each main processing function cycle of the LIN interface.
 *  \details     Performs all notification and indication to upper layers. It may be used to decouple the
 *               main processing (frame and schedule handling) from the interactions with upper layer modules.
 *               By default this function is called directly from LinIf_ChannelMainFunction().
 *               The possibility to call this function from outside is optional and should only be enabled per user
 *               config file after clarification with Vector. In this case it is mandatory to call it once after each call
 *               of LinIf_ChannelMainFunction().
 *  \param[in]   LinIfChannel           Internal LinIf channel handle. Must reference a LIN master channel.
 *  \pre         This module must be initialized before this service function is called.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_NODETYPE_MASTER_SUPPORTED
 *********************************************************************************************************************/
FUNC(void, LINIF_CODE) LinIf_ChannelMainFunctionPostProcessing
(
  VAR(NetworkHandleType, AUTOMATIC)             LinIfChannel
);

#  define LINIF_STOP_SEC_CODE
#  include "LinIf_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

# endif /* LINIF_NODETYPE_MASTER_SUPPORTED */
#endif /* LINIF_MASTER_H */

/**********************************************************************************************************************
 *  END OF FILE: LinIf_Master.h
 *********************************************************************************************************************/
