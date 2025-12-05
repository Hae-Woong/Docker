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
 *         File:  LinIf_Slave.h
 *    Component:  AUTOSAR LIN INTERFACE
 *       Module:  If_Asr4IfLin
 *    Generator:  If_Asr4IfLin
 *
 *  Description:  Header file of LIN Slave specific internal interface.
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

#if !defined (LINIF_SLAVE_H)
# define LINIF_SLAVE_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "LinIf_Cfg.h"

# if ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#  define LINIF_SLAVE_MAJOR_VERSION                 (12u)
#  define LINIF_SLAVE_MINOR_VERSION                 (0u)
#  define LINIF_SLAVE_PATCH_VERSION                 (0u)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/*! Access to static or dynamic LIN product identification, depending on configuration */
#  if ( LINIF_NC_PRODUCT_ID_CONFIGURABLE_SUPPORTED == STD_ON )
#   define LinIf_GetSupplierId(SlaveChannelIndex)   LinIf_GetSlaveChannelData(SlaveChannelIndex).LinIf_SupplierId /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#   define LinIf_GetFunctionId(SlaveChannelIndex)   LinIf_GetSlaveChannelData(SlaveChannelIndex).LinIf_FunctionId /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#   define LinIf_GetVariantId(SlaveChannelIndex)    LinIf_GetSlaveChannelData(SlaveChannelIndex).LinIf_VariantId /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  else
#   define LinIf_GetSupplierId(SlaveChannelIndex)   LinIf_GetSupplierIdOfSlaveChannelConfig(SlaveChannelIndex) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#   define LinIf_GetFunctionId(SlaveChannelIndex)   LinIf_GetFunctionIdOfSlaveChannelConfig(SlaveChannelIndex) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#   define LinIf_GetVariantId(SlaveChannelIndex)    LinIf_GetVariantIdOfSlaveChannelConfig(SlaveChannelIndex) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  endif

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
 *  LinIf_SlaveChannelMainFunction()
 *********************************************************************************************************************/
/*! \brief       Performs channel specific main function handling.
 *  \details     Performs timeout observation for LIN slave channels.
 *  \param[in]   LinIfChannel        Local LinIf channel identifier. Must reference a LIN slave channel.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_NODETYPE_SLAVE_SUPPORTED
 *********************************************************************************************************************/
FUNC(void, LINIF_CODE) LinIf_SlaveChannelMainFunction
(
  VAR(NetworkHandleType, AUTOMATIC)                                     LinIfChannel
);

/**********************************************************************************************************************
 *  LinIf_SlaveWakeup()
 *********************************************************************************************************************/
/*! \brief       Requests the transition into operational state for slave nodes.
 *  \details     Initiates the wake up process on the selected channel and transmits a wakeup frame.
 *  \param[in]   Channel                 Network channel handle. Must reference a LIN slave channel.
 *  \param[in]   LinIfChannel            Internal LinIf channel handle. Must reference a LIN slave channel.
 *  \param[in]   ChannelDataPtr          Pointer to (general) channel data
 *  \return      E_OK                    Request to wake up was accepted.
 *  \return      E_NOT_OK                Request to wake up was not accepted.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE for different channels
 *  \synchronous FALSE
 *  \config      LINIF_NODETYPE_SLAVE_SUPPORTED
 *********************************************************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinIf_SlaveWakeup
(
  VAR(NetworkHandleType, AUTOMATIC)                                     Channel,
  VAR(NetworkHandleType, AUTOMATIC)                                     LinIfChannel,
  CONSTP2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)        ChannelDataPtr
);

#  define LINIF_STOP_SEC_CODE
#  include "LinIf_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

# endif /* LINIF_NODETYPE_SLAVE_SUPPORTED */
#endif /* LINIF_SLAVE_H */

/**********************************************************************************************************************
 *  END OF FILE: LinIf_Slave.h
 *********************************************************************************************************************/

