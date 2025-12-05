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
/*!        \file  Com_DesGw.h
 *         \unit  DesGw
 *        \brief  MICROSAR Communication header file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_DESGW_H)
# define COM_DESGW_H

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
# if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON) /* COV_COM_GATEWAY_MAINFUNCTION_ROUTESIGNALSAPI */

typedef enum
{
  COM_TRANSMIT_TRIGGER_NOTFULFILLED,
  COM_TRANSMIT_TRIGGER,
  COM_TRANSMIT_TRIGGERWITHOUTREP
} Com_DescriptionGwTriggerType;

# endif
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
  Com_DesGw_MainFunction
**********************************************************************************************************************/
/*! \brief        This function processes deferred gateway description events that are queued in the gateway Event Cache
 *                in reverse order.
    \details      -
    \pre          -
    \param[in]    mainFunctionId    ID of the calling route signals main function.
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_DesGw_MainFunction(Com_MainFunctionRouteSignalsStructIterType mainFunctionId);

/**********************************************************************************************************************
  Com_DesGw_ImmediateEvent
**********************************************************************************************************************/
/*! \brief        This function handles the immediate gateway description events.
    \details      -
    \pre          -
    \param[in]    rxComPduId          Handle of the rx I-PDU.
    \param[in]    PduInfoPtr          contains the SDU buffer and the SDU length
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires PduInfoPtr != NULL_PTR;
      requires rxComPduId < Com_GetSizeOfRxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_DesGw_ImmediateEvent(Com_RxPduInfoIterType rxComPduId, P2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr);

/**********************************************************************************************************************
  Com_DesGw_GetLengthOfHandleRxDeferredGwDescription
**********************************************************************************************************************/
/*! \brief        This function returns the length of the deferred Rx I-PDU which was set before.
    \details      -
    \pre          -
    \param[in]    comRxPduInfoIdx    Rx PduId
    \return       deferredPduLength  Length of Rx deferred I-PDU
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires comRxPduInfoIdx < Com_GetSizeOfRxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(PduLengthType, COM_CODE) Com_DesGw_GetLengthOfHandleRxDeferredGwDescription(Com_RxPduInfoIterType comRxPduInfoIdx);

/**********************************************************************************************************************
  Com_DesGw_SetLengthOfHandleRxDeferredGwDescription
**********************************************************************************************************************/
/*! \brief        This function sets the length of the deferred Rx I-PDU to the given length.
    \details      -
    \pre          -
    \param[in]    comRxPduInfoIdx    Rx PduId
    \param[in]    deferredPduLength  Length of Rx deferred I-PDU
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires comRxPduInfoIdx < Com_GetSizeOfRxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_DesGw_SetLengthOfHandleRxDeferredGwDescription(Com_RxPduInfoIterType comRxPduInfoIdx, PduLengthType deferredPduLength);

/**********************************************************************************************************************
  Com_DesGw_ProcessDestinationDescription
**********************************************************************************************************************/
/*! \brief        This function processes a gateway destination description. More precisely, it copies the configured bits
 *                from the source to destination buffer if the transfer property conditions are satisfied.
    \details      -
    \pre          -
    \param[in]    sourceBuffer         Buffer of rxPdu that is received on gateway description event.
    \param[in]    gwDescrAccessIdx     Index of Com_GwDescriptionAccessInfo, which contains all informations of the description
    \return       Com_DescriptionGwTriggerType
                      COM_TRANSMIT_TRIGGER_NOTFULFILLED  transfer property conditions are not fulfilled
                      COM_TRANSMIT_TRIGGER               transfer property conditions are fulfilled and repetitions
                                                         are required
                      COM_TRANSMIT_TRIGGERWITHOUTREP     transfer property conditions are fulfilled and repetitions
                                                         are not required
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
FUNC(Com_DescriptionGwTriggerType, COM_CODE) Com_DesGw_ProcessDestinationDescription(P2CONST(uint8, AUTOMATIC, AUTOMATIC) sourceBuffer, Com_GwDescriptionAccessInfoIterType gwDescrAccessIdx);

# endif

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_DESGW_H */
/**********************************************************************************************************************
  END OF FILE: Com_DesGw.h
**********************************************************************************************************************/
