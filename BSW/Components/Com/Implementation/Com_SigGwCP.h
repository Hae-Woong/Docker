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
/*!        \file  Com_SigGwCP.h
 *         \unit  SigGwCP
 *        \brief  MICROSAR Communication header file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_SIGGWCP_H)
# define COM_SIGGWCP_H

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

# if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON) /* COV_COM_GATEWAY_MAINFUNCTION_ROUTESIGNALSAPI */
/**********************************************************************************************************************
  Com_SigGwCP_SignalGroupRoutingRx
**********************************************************************************************************************/
/*! \brief        This function processes cross partition signal group routings.
    \details      -
    \param[in]    idxGwSigGrpMapping   Index of GwSigGrpMapping.
    \pre          IsUsed check required: Com_IsCprQueueUsedOfGwSigGrpMapping()
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxGwSigGrpMapping < Com_GetSizeOfGwSigGrpMapping();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_SigGwCP_SignalGroupRoutingRx(Com_GwSigGrpMappingEndIdxOfGwInfoType idxGwSigGrpMapping);

/**********************************************************************************************************************
  Com_SigGwCP_SignalRoutingRx
**********************************************************************************************************************/
/*! \brief        This function processes cross partition signal routings.
    \details      -
    \param[in]    idxGwSigMapping     Index of GwSigMapping.
    \pre          IsUsed check required: Com_IsCprQueueUsedOfGwSigMapping()
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxGwSigMapping < Com_GetSizeOfGwSigMapping();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_SigGwCP_SignalRoutingRx(Com_GwSigMappingEndIdxOfGwInfoType idxGwSigMapping);

/**********************************************************************************************************************
  Com_SigGwCP_ProcessCrossPartitionQueuesTx
**********************************************************************************************************************/
/*! \brief        This function processes the cross partition routing queues leading to this partition.
    \details      -
    \pre          -
    \param[in]    mainFunctionId  Idx of the main function route signals
    \param[in]    gwProcessingISRlockCounterIdx index of the indirection from the main function route signals to the ISRLock counter
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_SigGwCP_ProcessCrossPartitionQueuesTx(Com_MainFunctionRouteSignalsStructIterType mainFunctionId,
                                                                                 Com_ISRThresholdInfoIterType gwProcessingISRlockCounterIdx);

# endif

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_SIGGWCP_H */

/**********************************************************************************************************************
  END OF FILE: Com_SigGwCP.h
**********************************************************************************************************************/
