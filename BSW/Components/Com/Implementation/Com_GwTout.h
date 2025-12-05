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
/*!        \file  Com_GwTout.h
 *         \unit  GwTout
 *        \brief  MICROSAR Communication header file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_GWTOUT_H)
# define COM_GWTOUT_H

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
  Com_GwTout_MainFunctionTx
**********************************************************************************************************************/
/*! \brief        This function handles the decrementing of an active GW timeout counter.
    \details      -
    \pre          -
    \param[in]    mainFunctionId    ID of the calling tx main function.
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires mainFunctionId < Com_GetSizeOfMainFunctionTxStruct();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_GwTout_MainFunctionTx(Com_MainFunctionTxStructIterType mainFunctionId);

/**********************************************************************************************************************
  Com_GwTout_InitCounter
**********************************************************************************************************************/
/*! \brief        This function initializes the GW timeout counter for a given Pdu.
    \details      -
    \pre          -
    \param[in]    ComTxPduId    ComIPdu handle id
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_GwTout_InitCounter(Com_TxPduInfoIterType ComTxPduId);

/**********************************************************************************************************************
  Com_GwTout_Event
**********************************************************************************************************************/
/*! \brief          This function handles the reloading of the Gw timeout.
    \details        If a gateway timeout is configured, this function Starts cyclic transmission for periodic I-PDUs
                    with a configured GW Timeout and reloads the gw Timeout counter.
    \pre            -
    \param[in]      ComTxPduId
    \synchronous    TRUE
    \reentrant      TRUE, for different handles
    \context        TASK
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_GwTout_Event(Com_TxPduInfoIterType ComTxPduId);
# endif

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_GWTOUT_H */
/**********************************************************************************************************************
  END OF FILE: Com_GwTout.h
**********************************************************************************************************************/
