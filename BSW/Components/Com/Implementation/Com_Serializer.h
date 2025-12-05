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
/*!        \file  Com_Serializer.h
 *         \unit  Serializer
 *        \brief  MICROSAR Communication header file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_SERIALIZER_H)
# define COM_SERIALIZER_H

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
  Com_Serializer_WriteSignal
**********************************************************************************************************************/
/*! \brief        This method writes a signal to the TxIPdu buffer.
    \details      -
    \pre          -
    \param[in]    SignalId          ID of the signal signal
    \param[in]    comDataPtr        Pointer to application memory where the signal is located
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires SignalId < Com_GetSizeOfTxSigInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Serializer_WriteSignal(Com_TxSigInfoIterType SignalId, CONSTP2CONST(ComDataTypeConst, AUTOMATIC, AUTOMATIC) comDataPtr);

/**********************************************************************************************************************
  Com_Serializer_WriteGroupSignal
**********************************************************************************************************************/
/*! \brief        This method writes a group signal to the shadow buffer.
    \details      -
    \pre          -
    \param[in]    groupSignalId     ID of the group signal
    \param[in]    comDataPtr        Pointer to application memory where the signal is located
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires groupSignalId < Com_GetSizeOfTxGrpSigInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Serializer_WriteGroupSignal(Com_TxGrpSigInfoIterType groupSignalId, CONSTP2CONST(ComDataTypeConst, AUTOMATIC, AUTOMATIC) comDataPtr);

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_SERIALIZER_H */

/**********************************************************************************************************************
  END OF FILE: Com_Serializer.h
**********************************************************************************************************************/
