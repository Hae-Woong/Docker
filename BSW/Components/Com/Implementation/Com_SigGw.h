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
/*!        \file  Com_SigGw.h
 *         \unit  SigGw
 *        \brief  MICROSAR Communication header file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_SIGGW_H)
# define COM_SIGGW_H

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
  Com_SigGw_InitRxIpdu
**********************************************************************************************************************/
/*! \brief        This function initializes a rx I-PDU in Com_SigGw.
    \details      -
    \pre          -
    \param[in]    rxPduId Handle of the I-PDU.
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires rxPduId < Com_GetSizeOfRxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_SigGw_InitRxIpdu(Com_RxPduInfoIterType rxPduId);

/**********************************************************************************************************************
  Com_SigGw_SigEvent
**********************************************************************************************************************/
/*! \brief        This function sets a signal routing event flag for the passed ComSignal.
    \details      -
    \pre          -
    \param[in]    idxRxSigInfo Index of RxSigInfo
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxRxSigInfo < Com_GetSizeOfRxSigInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_SigGw_SigEvent(Com_RxSigInfoIterType idxRxSigInfo);

/**********************************************************************************************************************
  Com_SigGw_SigGrpEvent
**********************************************************************************************************************/
/*! \brief        This function sets a signal routing event flag for the passed ComSignalGroup.
    \details      -
    \pre          -
    \param[in]    idxRxSigGrpInfo Index of RxSigGrpInfo
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxRxSigGrpInfo < Com_GetSizeOfRxSigGrpInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_SigGw_SigGrpEvent(Com_RxSigGrpInfoIterType idxRxSigGrpInfo);

/**********************************************************************************************************************
  Com_SigGw_SetGwEvent
**********************************************************************************************************************/
/*! \brief        This function sets a signal routing event flag for the passed Signal or GroupSignal.
    \details      -
    \pre          -
    \param[in]    idxRxAccessInfo Index of RxAccessInfo
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxRxAccessInfo < Com_GetSizeOfRxAccessInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_SigGw_SetGwEvent(Com_RxAccessInfoIterType idxRxAccessInfo);

/**********************************************************************************************************************
  Com_SigGw_MainFunction
**********************************************************************************************************************/
/*! \brief        This function processes signal gateway events.
    \details      -
    \pre          -
    \param[in]    mainFunctionId    ID of the calling route signals main function.
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires mainFunctionId < Com_GetSizeOfMainFunctionRouteSignalsStruct();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_SigGw_MainFunction(Com_MainFunctionRouteSignalsStructIterType mainFunctionId);
# endif

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_SIGGW_H */

/**********************************************************************************************************************
  END OF FILE: Com_SigGw.h
**********************************************************************************************************************/
