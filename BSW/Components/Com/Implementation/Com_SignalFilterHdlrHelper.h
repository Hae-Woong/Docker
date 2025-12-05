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
/*!        \file  Com_SignalFilterHdlrHelper.h
 *         \unit  SignalFilterHdlrHelper
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_SIGNALFILTERHDLRHELPER_H)
# define COM_SIGNALFILTERHDLRHELPER_H

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
  Com_SignalFilterHdlrHelper_SetFilterOldValueToInitValue
**********************************************************************************************************************/
/*!
    \brief        This method sets the old filter value to the initial value, the buffer is ApplType specific.
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
FUNC(void, COM_CODE) Com_SignalFilterHdlrHelper_SetFilterOldValueToInitValue(Com_FilterInfoIterType idxFilterInfo);

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_SetFilterOldValueX
**********************************************************************************************************************/
/*! \brief        This method sets the old filter value.
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
FUNC(void, COM_CODE) Com_SignalFilterHdlrHelper_SetFilterOldValueX(Com_FilterInfoIterType idxFilterInfo, P2CONST(void, AUTOMATIC, AUTOMATIC) SignalDataPtr);

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_GetOldValuePtr
**********************************************************************************************************************/
/*! \brief        This method returns a pointer to the datatype dependent old filter value.
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
FUNC(P2CONST(void, AUTOMATIC, AUTOMATIC), COM_CODE) Com_SignalFilterHdlrHelper_GetOldValuePtr(Com_FilterInfoIterType idxFilterInfo);

/**********************************************************************************************************************
 *  Com_SignalFilterHdlrHelper_UInt8_EvaluateFilter
 *********************************************************************************************************************/
/*!
    \brief        This method evaluates the filter state for a uint8 signal.
    \details      -
    \pre          Multi Indirection check required: COM_FILTERINFO_UINT8_APPLTYPSPECIFICFILTERINFOOFFILTERINFO
    \param[in]    idxFilterInfo    index of related filter info
    \param[in]    newValue         new value
    \param[in]    oldValue         old value
    \return       boolean
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxFilterInfo < Com_GetSizeOfFilterInfo();
    \endspec
*********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_UInt8_EvaluateFilter(Com_FilterInfoIterType idxFilterInfo, uint8 newValue, uint8 oldValue);

/**********************************************************************************************************************
 *  Com_SignalFilterHdlrHelper_SInt8_EvaluateFilter
 *********************************************************************************************************************/
/*!
    \brief        This method evaluates the filter state for a sint8 signal.
    \details      -
    \pre          Multi Indirection check required: COM_FILTERINFO_SINT8_APPLTYPSPECIFICFILTERINFOOFFILTERINFO
    \param[in]    idxFilterInfo    index of related filter info
    \param[in]    newValue         new value
    \param[in]    oldValue         old value
    \return       boolean
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxFilterInfo < Com_GetSizeOfFilterInfo();
    \endspec
*********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_SInt8_EvaluateFilter(Com_FilterInfoIterType idxFilterInfo, sint8 newValue, sint8 oldValue);

/**********************************************************************************************************************
 *  Com_SignalFilterHdlrHelper_UInt16_EvaluateFilter
 *********************************************************************************************************************/
/*!
    \brief        This method evaluates the filter state for a uint16 signal.
    \details      -
    \pre          Multi Indirection check required: COM_FILTERINFO_UINT16_APPLTYPSPECIFICFILTERINFOOFFILTERINFO
    \param[in]    idxFilterInfo    index of related filter info
    \param[in]    newValue         new value
    \param[in]    oldValue         old value
    \return       boolean
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxFilterInfo < Com_GetSizeOfFilterInfo();
    \endspec
*********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_UInt16_EvaluateFilter(Com_FilterInfoIterType idxFilterInfo, uint16 newValue, uint16 oldValue);

/**********************************************************************************************************************
 *  Com_SignalFilterHdlrHelper_SInt16_EvaluateFilter
 *********************************************************************************************************************/
/*!
    \brief        This method evaluates the filter state for a sint16 signal.
    \details      -
    \pre          Multi Indirection check required: COM_FILTERINFO_SINT16_APPLTYPSPECIFICFILTERINFOOFFILTERINFO
    \param[in]    idxFilterInfo    index of related filter info
    \param[in]    newValue         new value
    \param[in]    oldValue         old value
    \return       boolean
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxFilterInfo < Com_GetSizeOfFilterInfo();
    \endspec
*********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_SInt16_EvaluateFilter(Com_FilterInfoIterType idxFilterInfo, sint16 newValue, sint16 oldValue);

/**********************************************************************************************************************
 *  Com_SignalFilterHdlrHelper_UInt32_EvaluateFilter
 *********************************************************************************************************************/
/*!
    \brief        This method evaluates the filter state for a uint32 signal.
    \details      -
    \pre          Multi Indirection check required: COM_FILTERINFO_UINT32_APPLTYPSPECIFICFILTERINFOOFFILTERINFO
    \param[in]    idxFilterInfo    index of related filter info
    \param[in]    newValue         new value
    \param[in]    oldValue         old value
    \return       boolean
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxFilterInfo < Com_GetSizeOfFilterInfo();
    \endspec
*********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_UInt32_EvaluateFilter(Com_FilterInfoIterType idxFilterInfo, uint32 newValue, uint32 oldValue);

/**********************************************************************************************************************
 *  Com_SignalFilterHdlrHelper_SInt32_EvaluateFilter
 *********************************************************************************************************************/
/*!
    \brief        This method evaluates the filter state for a sint32 signal.
    \details      -
    \pre          Multi Indirection check required: COM_FILTERINFO_SINT32_APPLTYPSPECIFICFILTERINFOOFFILTERINFO
    \param[in]    idxFilterInfo    index of related filter info
    \param[in]    newValue         new value
    \param[in]    oldValue         old value
    \return       boolean
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxFilterInfo < Com_GetSizeOfFilterInfo();
    \endspec
*********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_SInt32_EvaluateFilter(Com_FilterInfoIterType idxFilterInfo, sint32 newValue, sint32 oldValue);

/**********************************************************************************************************************
 *  Com_SignalFilterHdlrHelper_UInt64_EvaluateFilter
 *********************************************************************************************************************/
/*!
    \brief        This method evaluates the filter state for a uint64 signal.
    \details      -
    \pre          Multi Indirection check required: COM_FILTERINFO_UINT64_APPLTYPSPECIFICFILTERINFOOFFILTERINFO
    \param[in]    idxFilterInfo    index of related filter info
    \param[in]    newValue         new value
    \param[in]    oldValue         old value
    \return       boolean
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxFilterInfo < Com_GetSizeOfFilterInfo();
    \endspec
*********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_UInt64_EvaluateFilter(Com_FilterInfoIterType idxFilterInfo, uint64 newValue, uint64 oldValue);

/**********************************************************************************************************************
 *  Com_SignalFilterHdlrHelper_SInt64_EvaluateFilter
 *********************************************************************************************************************/
/*!
    \brief        This method evaluates the filter state for a sint64 signal.
    \details      -
    \pre          Multi Indirection check required: COM_FILTERINFO_SINT64_APPLTYPSPECIFICFILTERINFOOFFILTERINFO
    \param[in]    idxFilterInfo    index of related filter info
    \param[in]    newValue         new value
    \param[in]    oldValue         old value
    \return       boolean
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxFilterInfo < Com_GetSizeOfFilterInfo();
    \endspec
*********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_SInt64_EvaluateFilter(Com_FilterInfoIterType idxFilterInfo, sint64 newValue, sint64 oldValue);

/**********************************************************************************************************************
 *  Com_SignalFilterHdlrHelper_Float32_EvaluateFilter
 *********************************************************************************************************************/
/*!
    \brief        This method evaluates the filter state for a float32 signal.
    \details      -
    \pre          Multi Indirection check required: COM_FILTERINFO_FLOAT32_APPLTYPSPECIFICFILTERINFOOFFILTERINFO
                  Only MASKED_NEW_DIFFERS_MASKED_OLD, ALWAYS and NEVER filters are supported
    \param[in]    idxFilterInfo    index of related filter info
    \param[in]    newValue         new value
    \param[in]    oldValue         old value
    \return       boolean
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxFilterInfo < Com_GetSizeOfFilterInfo();
    \endspec
*********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_Float32_EvaluateFilter(Com_FilterInfoIterType idxFilterInfo, float32 newValue, float32 oldValue);

/**********************************************************************************************************************
 *  Com_SignalFilterHdlrHelper_Float64_EvaluateFilter
 *********************************************************************************************************************/
/*!
    \brief        This method evaluates the filter state for a float64 signal.
    \details      -
    \pre          Multi Indirection check required: COM_FILTERINFO_FLOAT64_APPLTYPSPECIFICFILTERINFOOFFILTERINFO
                  Only MASKED_NEW_DIFFERS_MASKED_OLD, ALWAYS and NEVER filters are supported
    \param[in]    idxFilterInfo    index of related filter info
    \param[in]    newValue         new value
    \param[in]    oldValue         old value
    \return       boolean
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxFilterInfo < Com_GetSizeOfFilterInfo();
    \endspec
*********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_Float64_EvaluateFilter(Com_FilterInfoIterType idxFilterInfo, float64 newValue, float64 oldValue);

/**********************************************************************************************************************
 *  Com_SignalFilterHdlrHelper_UInt8_N_EvaluateFilter
 *********************************************************************************************************************/
/*! \brief        This method evaluates the filter state for a uint8_N signal.
    \details      -
    \pre          Multi Indirection check required: COM_FILTERINFO_UINT8_N_APPLTYPSPECIFICFILTERINFOOFFILTERINFO
                  Only MASKED_NEW_DIFFERS_MASKED_OLD, ALWAYS and NEVER filters are supported
    \param[in]    idxFilterInfo    index of related filter info
    \param[in]    oldValuePtr      pointer to old value
    \param[in]    comDataPtr       pointer to pointer to old value and number of bytes to be evaluated
    \return       boolean
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxFilterInfo < Com_GetSizeOfFilterInfo();
    \endspec
*********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_UInt8_N_EvaluateFilter(Com_FilterInfoIterType idxFilterInfo, CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) oldValuePtr, CONSTP2CONST(ComDataTypeConst, AUTOMATIC, AUTOMATIC) comDataPtr);

/**********************************************************************************************************************
 *  Com_SignalFilterHdlrHelper_UInt8_Dyn_EvaluateFilter
 *********************************************************************************************************************/
/*! \brief        This method evaluates the filter state for a uint8_Dyn signal.
    \details      -
    \pre          Multi Indirection check required: COM_FILTERINFO_UINT8_DYN_APPLTYPSPECIFICFILTERINFOOFFILTERINFO
                  Only ALWAYS and NEVER filters are supported
    \param[in]    idxFilterInfo    index of related filter info
    \return       boolean
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxFilterInfo < Com_GetSizeOfFilterInfo();
    \endspec
*********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_UInt8_Dyn_EvaluateFilter(Com_FilterInfoIterType idxFilterInfo);

/**********************************************************************************************************************
 *  Com_SignalFilterHdlrHelper_ZeroBit_EvaluateFilter
 *********************************************************************************************************************/
/*! \brief        This method evaluates the filter state for a ZeroBit signal.
    \details      -
    \pre          Multi Indirection check required: COM_FILTERINFO_ZEROBIT_APPLTYPSPECIFICFILTERINFOOFFILTERINFO
                  Only ALWAYS and NEVER filters are supported
    \param[in]    idxFilterInfo    index of related filter info
    \return       boolean
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxFilterInfo < Com_GetSizeOfFilterInfo();
    \endspec
*********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_ZeroBit_EvaluateFilter(Com_FilterInfoIterType idxFilterInfo);

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_SigGrpArr_EvaluateFilter
**********************************************************************************************************************/
/*! \brief        This function evaluates the filter bytewise for SignalGroups with ArrayAccess enabled.
    \details      -
    \pre          Multi Indirection check required: COM_SIGGRPARRAYFILTERINFO_APPLTYPSPECIFICFILTERINFOOFFILTERINFO
                  only MASKED_NEW_DIFFERS_X and MASKED_NEW_EQUALS_X filters are supported
    \param[in]    idxFilterInfoApplTypSpecific Index of related SignalGroup Array filter info
    \param[in]    newValuePtr                  Reference to the signal group array.
    \return       boolean                      TRUE, if filter condition is passed, FALSE otherwise.
    \context      TASK|ISR2
    \reentrant    TRUE, for different handles
    \synchronous  TRUE
    \spec
      requires idxFilterInfoApplTypSpecific < Com_GetSizeOfSigGrpArrayFilterInfo();
      requires newValuePtr != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_SigGrpArr_EvaluateFilter(Com_SigGrpArrayFilterInfoIterType idxFilterInfoApplTypSpecific, P2CONST(uint8, AUTOMATIC, AUTOMATIC) newValuePtr);

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_SIGNALFILTERHDLRHELPER_H */
/**********************************************************************************************************************
  END OF FILE: Com_SignalFilterHdlrHelper.h
**********************************************************************************************************************/
