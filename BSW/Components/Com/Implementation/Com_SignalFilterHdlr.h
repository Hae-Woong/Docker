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
/*!        \file  Com_SignalFilterHdlr.h
 *         \unit  SignalFilterHdlr
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_SIGNALFILTERHDLR_H)
# define COM_SIGNALFILTERHDLR_H

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
# include "Com_Lcfg.h"

/**********************************************************************************************************************
  GLOBAL CONSTANT MACROS
**********************************************************************************************************************/

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
 *  Com_SignalFilterHdlr_EvaluateFilter
 *********************************************************************************************************************/
/*!
    \brief        This function calls the apply type specific filter evaluation function of the SignalFilterHdlrHelper unit.
    \details      -
    \pre          the function is called for tx filter which is not ALWAYS or NEVER filter or for rx filter which is not ALWAYS filter
                  or for tx signals with triggered on change transfer property
    \param[in]    idxFilterInfo    index of related filter info
    \param[in]    oldValuePtr      pointer to the old value
    \param[in]    comDataPtr       pointer to pointer to the new value and length of the to be evaluated signal
    \return       boolean
                    TRUE  if filter condition is passed
                    FALSE otherwise
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxFilterInfo < Com_GetSizeOfFilterInfo();
    \endspec
*********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_SignalFilterHdlr_EvaluateFilter(Com_FilterInfoIterType idxFilterInfo, P2CONST(void, AUTOMATIC, AUTOMATIC) oldValuePtr, CONSTP2CONST(ComDataTypeConst, AUTOMATIC, AUTOMATIC) comDataPtr);

/**********************************************************************************************************************
  Com_SignalFilterHdlr_EvaluateSigGrpArrFilter
**********************************************************************************************************************/
/*! \brief        This function calls the SigGrpArray filter evaluation function of the SignalFilterHdlrHelper unit, if
                  the given idxFilterInfo belongs to a SIGGRPARRAYFILTER.
    \details      -
    \pre          the function is called for tx group signals where the signal group has array access enabled and the tx
                  group signal has a MASKED_NEW_DIFFERS_X or MASKED_NEW_EQUALS_X filter
    \param[in]    idxFilterInfo               Index of related filter info
    \param[in]    SignalGroupArrayPtr         Reference to the signal group array.
    \return       boolean
                    TRUE  if filter condition is passed
                    FALSE otherwise
    \context      TASK|ISR2
    \reentrant    TRUE, for different handles
    \synchronous  TRUE
    \spec
      requires SignalGroupArrayPtr != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_SignalFilterHdlr_EvaluateSigGrpArrFilter(Com_FilterInfoIterType idxFilterInfo, P2CONST(uint8, AUTOMATIC, AUTOMATIC) SignalGroupArrayPtr);

/**********************************************************************************************************************
  Com_SignalFilterHdlr_SetFilterOldValueToInitValue
**********************************************************************************************************************/
/*!
    \brief        This function calls the SetFilterOldValueToInitValue function of the SignalFilterHdlrHelper unit
    \details      -
    \pre          idxFilterInfo refers to a tx filter or a tx signal with triggered on change transfer property
    \param[in]    idxFilterInfo    index of related filter info
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxFilterInfo < Com_GetSizeOfFilterInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_SignalFilterHdlr_SetFilterOldValueToInitValue(Com_FilterInfoIterType idxFilterInfo);

/**********************************************************************************************************************
  Com_SignalFilterHdlr_SetFilterOldValueX
**********************************************************************************************************************/
/*! \brief        This function calls the SetFilterOldValueX function of the SignalFilterHdlrHelper unit
    \details      -
    \pre          idxFilterInfo refers to a tx filter or a tx signal with triggered on change transfer property
    \param[in]    idxFilterInfo     Index of FilterInfo
    \param[in]    SignalDataPtr     Reference to the signal data to be transmitted.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxFilterInfo < Com_GetSizeOfFilterInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_SignalFilterHdlr_SetFilterOldValueX(Com_FilterInfoIterType idxFilterInfo, P2CONST(void, AUTOMATIC, AUTOMATIC) SignalDataPtr);

/**********************************************************************************************************************
  Com_SignalFilterHdlr_GetOldValuePtr
**********************************************************************************************************************/
/*! \brief        This function calls the GetOldValuePtr function of the SignalFilterHdlrHelper unit
    \details      -
    \pre          idxFilterInfo refers to a tx filter or a tx signal with triggered on change transfer property
    \param[in]    idxFilterInfo      Index of FilterInfo
    \return       P2CONST(void, AUTOMATIC, AUTOMATIC)  pointer to the old filter value
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxFilterInfo < Com_GetSizeOfFilterInfo();
    \endspec
**********************************************************************************************************************/
FUNC(P2CONST(void, AUTOMATIC, AUTOMATIC), COM_CODE) Com_SignalFilterHdlr_GetOldValuePtr(Com_FilterInfoIterType idxFilterInfo);

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_SIGNALFILTERHDLR_H */
/**********************************************************************************************************************
  END OF FILE: Com_SignalFilterHdlr.h
**********************************************************************************************************************/
