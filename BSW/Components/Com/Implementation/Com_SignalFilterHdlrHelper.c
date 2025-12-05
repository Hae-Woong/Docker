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
/*!        \file  Com_SignalFilterHdlrHelper.c
 *         \unit  SignalFilterHdlrHelper
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/

#define COM_SIGNALFILTERHDLRHELPER_SOURCE

/**********************************************************************************************************************
 * MISRA JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA  S 2741 EOF */ /* MD_CSL_ReducedToDefine */
/* PRQA  S 2983, 2984, 2985, 2986, 2987, 2991, 2992, 2993, 2994, 2995, 2996, 2997, 2998 EOF */ /* MD_MSR_ConstantCondition */
/* PRQA  S 6050 EOF */ /* MD_MSR_STCAL */
/* PRQA  S 883, 3406 EOF */ /* MD_Com_UnityBuild */

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
#include "Com_Cfg_FilterData.h"
#include "Com_SignalFilterHdlrHelper.h"
#include "Com_Cfg_TxSignalFiltering.h"

#include "Com_Util.h"
#include "Com_RxDlMon.h"

#include "vstdlib.h"

/**********************************************************************************************************************
  LOCAL CONSTANT MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL FUNCTION MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL DATA PROTOTYPES
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL DATA PROTOTYPES
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"    /* PRQA S 5087 */        /* MD_MSR_MemMap */

/**********************************************************************************************************************
*  Com_SignalFilterHdlrHelper_HasTimeoutOccurred
*********************************************************************************************************************/
/*!
    \brief        This method evaluates if a rx deadline monitoring timeout has occurred.
    \details      -
    \pre          -
    \param[in]    idxFilterInfo    index of related filter info
    \return       boolean
    \trace        SPEC-2736780
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
*********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_HasTimeoutOccurred(Com_FilterInfoIterType idxFilterInfo);

/**********************************************************************************************************************
*  Com_SignalFilterHdlrHelper_UInt8_MaskedNewDiffersX
*********************************************************************************************************************/
/*!
    \brief        Checks if the masked new value is different from the given value x for a uint8 value.
    \details      -
    \pre          -
    \param[in]    idxFilterInfo      index of filter info
    \param[in]    newValue           new value
    \return       filter result
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_UInt8_MaskedNewDiffersX(Com_FilterInfo_UInt8IterType idxFilterInfo, uint8 newValue);

/**********************************************************************************************************************
*  Com_SignalFilterHdlrHelper_UInt8_MaskedNewDiffersMaskedOld
*********************************************************************************************************************/
/*!
    \brief        Checks if the masked new value is different than the masked old value for a uint8 value.
    \details      -
    \pre          -
    \param[in]    idxFilterInfo      index of filter info
    \param[in]    idxUint8FilterInfo index of uint8 filter info
    \param[in]    newValue           new value
    \param[in]    oldValue           old value
    \return       filter result
    \trace        SPEC-2736780
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_UInt8_MaskedNewDiffersMaskedOld(Com_FilterInfoIterType idxFilterInfo, Com_FilterInfo_UInt8IterType idxUint8FilterInfo, uint8 newValue, uint8 oldValue);

/**********************************************************************************************************************
*  Com_SignalFilterHdlrHelper_UInt8_NewIsWithIn
*********************************************************************************************************************/
/*!
    \brief        Checks if the new value is within the configured range for a uint8 value.
    \details      -
    \pre          -
    \param[in]    idxFilterInfo      index of filter info
    \param[in]    newValue           new value
    \return       filter result
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_UInt8_NewIsWithIn(Com_FilterInfo_UInt8IterType idxFilterInfo, uint8 newValue);

/**********************************************************************************************************************
*  Com_SignalFilterHdlrHelper_SInt8_MaskedNewDiffersX
*********************************************************************************************************************/
/*!
    \brief        Checks if the masked new value is different from the given value x for a sint8 value.
    \details      -
    \pre          -
    \param[in]    idxFilterInfo      index of filter info
    \param[in]    newValue           new value
    \return       filter result
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_SInt8_MaskedNewDiffersX(Com_FilterInfo_SInt8IterType idxFilterInfo, sint8 newValue);

/**********************************************************************************************************************
*  Com_SignalFilterHdlrHelper_SInt8_MaskedNewDiffersMaskedOld
*********************************************************************************************************************/
/*!
    \brief        Checks if the masked new value is different than the masked old value for a sint8 value.
    \details      -
    \pre          -
    \param[in]    idxFilterInfo      index of filter info
    \param[in]    idxSint8FilterInfo index of sint8 filter info
    \param[in]    newValue           new value
    \param[in]    oldValue           old value
    \return       filter result
    \trace        SPEC-2736780
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_SInt8_MaskedNewDiffersMaskedOld(Com_FilterInfoIterType idxFilterInfo, Com_FilterInfo_SInt8IterType idxSint8FilterInfo, sint8 newValue, sint8 oldValue);

/**********************************************************************************************************************
*  Com_SignalFilterHdlrHelper_SInt8_NewIsWithIn
*********************************************************************************************************************/
/*!
    \brief        Checks if the new value is within the configured range for a sint8 value.
    \details      -
    \pre          -
    \param[in]    idxFilterInfo      index of filter info
    \param[in]    newValue           new value
    \return       filter result
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_SInt8_NewIsWithIn(Com_FilterInfo_SInt8IterType idxFilterInfo, sint8 newValue);

/**********************************************************************************************************************
*  Com_SignalFilterHdlrHelper_UInt16_MaskedNewDiffersX
*********************************************************************************************************************/
/*!
    \brief        Checks if the masked new value is different from the given value x for a uint16 value.
    \details      -
    \pre          -
    \param[in]    idxFilterInfo      index of filter info
    \param[in]    newValue           new value
    \return       filter result
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_UInt16_MaskedNewDiffersX(Com_FilterInfo_UInt16IterType idxFilterInfo, uint16 newValue);

/**********************************************************************************************************************
*  Com_SignalFilterHdlrHelper_UInt16_MaskedNewDiffersMaskedOld
*********************************************************************************************************************/
/*!
    \brief        Checks if the masked new value is different than the masked old value for a uint16 value.
    \details      -
    \pre          -
    \param[in]    idxFilterInfo       index of filter info
    \param[in]    idxUint16FilterInfo index of uint16 filter info
    \param[in]    newValue            new value
    \param[in]    oldValue            old value
    \return       filter result
    \trace        SPEC-2736780
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_UInt16_MaskedNewDiffersMaskedOld(Com_FilterInfoIterType idxFilterInfo, Com_FilterInfo_UInt16IterType idxUint16FilterInfo, uint16 newValue, uint16 oldValue);

/**********************************************************************************************************************
*  Com_SignalFilterHdlrHelper_UInt16_NewIsWithIn
*********************************************************************************************************************/
/*!
    \brief        Checks if the new value is within the configured range for a uint16 value.
    \details      -
    \pre          -
    \param[in]    idxFilterInfo      index of filter info
    \param[in]    newValue           new value
    \return       filter result
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_UInt16_NewIsWithIn(Com_FilterInfo_UInt16IterType idxFilterInfo, uint16 newValue);

/**********************************************************************************************************************
*  Com_SignalFilterHdlrHelper_SInt16_MaskedNewDiffersX
*********************************************************************************************************************/
/*!
    \brief        Checks if the masked new value is different from the given value x for a sint16 value.
    \details      -
    \pre          -
    \param[in]    idxFilterInfo      index of filter info
    \param[in]    newValue           new value
    \return       filter result
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_SInt16_MaskedNewDiffersX(Com_FilterInfo_SInt16IterType idxFilterInfo, sint16 newValue);

/**********************************************************************************************************************
*  Com_SignalFilterHdlrHelper_SInt16_MaskedNewDiffersMaskedOld
*********************************************************************************************************************/
/*!
    \brief        Checks if the masked new value is different than the masked old value for a sint16 value.
    \details      -
    \pre          -
    \param[in]    idxFilterInfo       index of filter info
    \param[in]    idxSint16FilterInfo index of sint16 filter info
    \param[in]    newValue            new value
    \param[in]    oldValue            old value
    \return       filter result
    \trace        SPEC-2736780
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_SInt16_MaskedNewDiffersMaskedOld(Com_FilterInfoIterType idxFilterInfo, Com_FilterInfo_SInt16IterType idxSint16FilterInfo, sint16 newValue, sint16 oldValue);

/**********************************************************************************************************************
*  Com_SignalFilterHdlrHelper_SInt16_NewIsWithIn
*********************************************************************************************************************/
/*!
    \brief        Checks if the new value is within the configured range for a sint16 value.
    \details      -
    \pre          -
    \param[in]    idxFilterInfo      index of filter info
    \param[in]    newValue           new value
    \return       filter result
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_SInt16_NewIsWithIn(Com_FilterInfo_SInt16IterType idxFilterInfo, sint16 newValue);

/**********************************************************************************************************************
*  Com_SignalFilterHdlrHelper_UInt32_MaskedNewDiffersX
*********************************************************************************************************************/
/*!
    \brief        Checks if the masked new value is different from the given value x for a uint32 value.
    \details      -
    \pre          -
    \param[in]    idxFilterInfo      index of filter info
    \param[in]    newValue           new value
    \return       filter result
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_UInt32_MaskedNewDiffersX(Com_FilterInfo_UInt32IterType idxFilterInfo, uint32 newValue);

/**********************************************************************************************************************
*  Com_SignalFilterHdlrHelper_UInt32_MaskedNewDiffersMaskedOld
*********************************************************************************************************************/
/*!
    \brief        Checks if the masked new value is different than the masked old value for a uint32 value.
    \details      -
    \pre          -
    \param[in]    idxFilterInfo       index of filter info
    \param[in]    idxUint32FilterInfo index of uint32 filter info
    \param[in]    newValue            new value
    \param[in]    oldValue            old value
    \return       filter result
    \trace        SPEC-2736780
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_UInt32_MaskedNewDiffersMaskedOld(Com_FilterInfoIterType idxFilterInfo, Com_FilterInfo_UInt32IterType idxUint32FilterInfo, uint32 newValue, uint32 oldValue);

/**********************************************************************************************************************
*  Com_SignalFilterHdlrHelper_UInt32_NewIsWithIn
*********************************************************************************************************************/
/*!
    \brief        Checks if the new value is within the configured range for a uint32 value.
    \details      -
    \pre          -
    \param[in]    idxFilterInfo      index of filter info
    \param[in]    newValue           new value
    \return       filter result
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_UInt32_NewIsWithIn(Com_FilterInfo_UInt32IterType idxFilterInfo, uint32 newValue);

/**********************************************************************************************************************
*  Com_SignalFilterHdlrHelper_SInt32_MaskedNewDiffersX
*********************************************************************************************************************/
/*!
    \brief        Checks if the masked new value is different from the given value x for a sint32 value.
    \details      -
    \pre          -
    \param[in]    idxFilterInfo      index of filter info
    \param[in]    newValue           new value
    \return       filter result
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_SInt32_MaskedNewDiffersX(Com_FilterInfo_SInt32IterType idxFilterInfo, sint32 newValue);

/**********************************************************************************************************************
*  Com_SignalFilterHdlrHelper_SInt32_MaskedNewDiffersMaskedOld
*********************************************************************************************************************/
/*!
    \brief        Checks if the masked new value is different than the masked old value for a sint32 value.
    \details      -
    \pre          -
    \param[in]    idxFilterInfo       index of filter info
    \param[in]    idxSint32FilterInfo index of sint32 filter info
    \param[in]    newValue            new value
    \param[in]    oldValue            old value
    \return       filter result
    \trace        SPEC-2736780
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_SInt32_MaskedNewDiffersMaskedOld(Com_FilterInfoIterType idxFilterInfo, Com_FilterInfo_SInt32IterType idxSint32FilterInfo, sint32 newValue, sint32 oldValue);

/**********************************************************************************************************************
*  Com_SignalFilterHdlrHelper_SInt32_NewIsWithIn
*********************************************************************************************************************/
/*!
    \brief        Checks if the new value is within the configured range for a sint32 value.
    \details      -
    \pre          -
    \param[in]    idxFilterInfo      index of filter info
    \param[in]    newValue           new value
    \return       filter result
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_SInt32_NewIsWithIn(Com_FilterInfo_SInt32IterType idxFilterInfo, sint32 newValue);

/**********************************************************************************************************************
*  Com_SignalFilterHdlrHelper_UInt64_MaskedNewDiffersX
*********************************************************************************************************************/
/*!
    \brief        Checks if the masked new value is different from the given value x for a uint64 value.
    \details      -
    \pre          -
    \param[in]    idxFilterInfo      index of filter info
    \param[in]    newValue           new value
    \return       filter result
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_UInt64_MaskedNewDiffersX(Com_FilterInfo_UInt64IterType idxFilterInfo, uint64 newValue);

/**********************************************************************************************************************
*  Com_SignalFilterHdlrHelper_UInt64_MaskedNewDiffersMaskedOld
*********************************************************************************************************************/
/*!
    \brief        Checks if the masked new value is different than the masked old value for a uint64 value.
    \details      -
    \pre          -
    \param[in]    idxFilterInfo       index of filter info
    \param[in]    idxUint64FilterInfo index of uint64 filter info
    \param[in]    newValue            new value
    \param[in]    oldValue            old value
    \return       filter result
    \trace        SPEC-2736780
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_UInt64_MaskedNewDiffersMaskedOld(Com_FilterInfoIterType idxFilterInfo, Com_FilterInfo_UInt64IterType idxUint64FilterInfo, uint64 newValue, uint64 oldValue);

/**********************************************************************************************************************
*  Com_SignalFilterHdlrHelper_UInt64_NewIsWithIn
*********************************************************************************************************************/
/*!
    \brief        Checks if the new value is within the configured range for a uint64 value.
    \details      -
    \pre          -
    \param[in]    idxFilterInfo      index of filter info
    \param[in]    newValue           new value
    \return       filter result
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_UInt64_NewIsWithIn(Com_FilterInfo_UInt64IterType idxFilterInfo, uint64 newValue);

/**********************************************************************************************************************
*  Com_SignalFilterHdlrHelper_SInt64_MaskedNewDiffersX
*********************************************************************************************************************/
/*!
    \brief        Checks if the masked new value is different from the given value x for a sint64 value.
    \details      -
    \pre          -
    \param[in]    idxFilterInfo      index of filter info
    \param[in]    newValue           new value
    \return       filter result
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_SInt64_MaskedNewDiffersX(Com_FilterInfo_SInt64IterType idxFilterInfo, sint64 newValue);

/**********************************************************************************************************************
*  Com_SignalFilterHdlrHelper_SInt64_MaskedNewDiffersMaskedOld
*********************************************************************************************************************/
/*!
    \brief        Checks if the masked new value is different than the masked old value for a sint64 value.
    \details      -
    \pre          -
    \param[in]    idxFilterInfo       index of filter info
    \param[in]    idxSint64FilterInfo index of sint64 filter info
    \param[in]    newValue            new value
    \param[in]    oldValue            old value
    \return       filter result
    \trace        SPEC-2736780
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_SInt64_MaskedNewDiffersMaskedOld(Com_FilterInfoIterType idxFilterInfo, Com_FilterInfo_SInt64IterType idxSint64FilterInfo, sint64 newValue, sint64 oldValue);

/**********************************************************************************************************************
*  Com_SignalFilterHdlrHelper_SInt64_NewIsWithIn
*********************************************************************************************************************/
/*!
    \brief        Checks if the new value is within the configured range for a sint64 value.
    \details      -
    \pre          -
    \param[in]    idxFilterInfo      index of filter info
    \param[in]    newValue           new value
    \return       filter result
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_SInt64_NewIsWithIn(Com_FilterInfo_SInt64IterType idxFilterInfo, sint64 newValue);

/**********************************************************************************************************************
*  Com_SignalFilterHdlrHelper_Float32_MaskedNewDiffersMaskedOld
*********************************************************************************************************************/
/*!
    \brief        Checks if the masked new value is different than the masked old value for a float32 value.
    \details      -
    \pre          -
    \param[in]    idxFilterInfo      index of filter info
    \param[in]    newValue           new value
    \param[in]    oldValue           old value
    \return       filter result
    \trace        SPEC-2736780
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_Float32_MaskedNewDiffersMaskedOld(Com_FilterInfoIterType idxFilterInfo, float32 newValue, float32 oldValue);

/**********************************************************************************************************************
*  Com_SignalFilterHdlrHelper_Float64_MaskedNewDiffersMaskedOld
*********************************************************************************************************************/
/*!
    \brief        Checks if the masked new value is different than the masked old value for a float64 value.
    \details      -
    \pre          -
    \param[in]    idxFilterInfo      index of filter info
    \param[in]    newValue           new value
    \param[in]    oldValue           old value
    \return       filter result
    \trace        SPEC-2736780
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_Float64_MaskedNewDiffersMaskedOld(Com_FilterInfoIterType idxFilterInfo, float64 newValue, float64 oldValue);

/**********************************************************************************************************************
*  Com_SignalFilterHdlrHelper_UInt8_N_MaskedNewDiffersMaskedOld
*********************************************************************************************************************/
/*!
    \brief        Checks if the masked new value is different than the masked old value for a uint8_n value.
    \details      -
    \pre          -
    \param[in]    idxFilterInfo    index of filter info
    \param[in]    oldValuePtr      pointer old value
    \param[in]    comDataPtr       pointer to pointer of new value and number bytes to be evaluated.
    \return       filter result
    \trace        SPEC-2736780
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_UInt8_N_MaskedNewDiffersMaskedOld(Com_FilterInfoIterType idxFilterInfo, CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) oldValuePtr, CONSTP2CONST(ComDataTypeConst, AUTOMATIC, AUTOMATIC) comDataPtr);

/**********************************************************************************************************************
*  Com_SignalFilterHdlrHelper_SigGrpArr_MaskedNewDiffersX
*********************************************************************************************************************/
/*!
    \brief        Bytewise evaluation of SignalGroup Filter MaskedNewDiffersX with ArrayAccess enabled. Checks if the masked new value is different from the given value x.
    \details      -
    \pre          -
    \param[in]    newValuePtr                  Reference to the signal group array.
    \param[in]    idxFilterInfoApplTypSpecific index of filter info
    \return       filter result
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_SigGrpArr_MaskedNewDiffersX(P2CONST(uint8, AUTOMATIC, AUTOMATIC) newValuePtr, Com_SigGrpArrayFilterInfoIterType idxFilterInfoApplTypSpecific);

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_HasTimeoutOccurred
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_HasTimeoutOccurred(Com_FilterInfoIterType idxFilterInfo)
{
  boolean hasToutOccurred = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if(Com_IsRxTOutInfoUsedOfFilterInfo(idxFilterInfo))
  {
    hasToutOccurred = Com_RxDlMon_HasTimeoutOccurred(Com_GetRxTOutInfoIdxOfFilterInfo(idxFilterInfo)); /* VCA_COM_OPTIONAL_INDIRECTION */
  }

  return hasToutOccurred;
}


/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_UInt8_MaskedNewDiffersX
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_UInt8_MaskedNewDiffersX(Com_FilterInfo_UInt8IterType idxFilterInfo, uint8 newValue)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */

  const uint8 filterMask = Com_GetFilterMaskOfFilterInfo_UInt8(idxFilterInfo);
  const uint8 filterX = Com_GetFilterXOfFilterInfo_UInt8(idxFilterInfo);
  if((newValue & filterMask) != filterX)  /* PRQA S 2985 */ /* MD_Com_2985_2986 */
  {
    filterState = TRUE;
  }

  return filterState;
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_UInt8_MaskedNewDiffersMaskedOld
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_UInt8_MaskedNewDiffersMaskedOld(Com_FilterInfoIterType idxFilterInfo, Com_FilterInfo_UInt8IterType idxUint8FilterInfo, uint8 newValue, uint8 oldValue)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if(Com_SignalFilterHdlrHelper_HasTimeoutOccurred(idxFilterInfo))
  {
    filterState = TRUE;
  }
  else
  {
    const uint8 filterMask = Com_GetFilterMaskOfFilterInfo_UInt8(idxUint8FilterInfo);
    if((newValue & filterMask) != (oldValue & filterMask))  /* PRQA S 2985 */ /* MD_Com_2985_2986 */
    {
      filterState = TRUE;
    }
  }

  return filterState;
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_UInt8_NewIsWithIn
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_UInt8_NewIsWithIn(Com_FilterInfo_UInt8IterType idxFilterInfo, uint8 newValue)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */

  const uint8 filterMin = Com_GetFilterMinOfFilterInfo_UInt8(idxFilterInfo);
  const uint8 filterMax = Com_GetFilterMaxOfFilterInfo_UInt8(idxFilterInfo);

  if((filterMin <= newValue) && (newValue <= filterMax))
  {
    filterState = TRUE;
  }

  return filterState;
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_SInt8_MaskedNewDiffersX
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_SInt8_MaskedNewDiffersX(Com_FilterInfo_SInt8IterType idxFilterInfo, sint8 newValue)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */

  const uint8 filterMask = Com_GetFilterMaskOfFilterInfo_SInt8(idxFilterInfo);
  const sint8 filterX = Com_GetFilterXOfFilterInfo_SInt8(idxFilterInfo);

  if((sint8) (((uint8) newValue) & filterMask) != filterX) /* PRQA S 4394 */ /* MD_Com_4394 */
  {
    filterState = TRUE;
  }

  return filterState;
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_SInt8_MaskedNewDiffersMaskedOld
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_SInt8_MaskedNewDiffersMaskedOld(Com_FilterInfoIterType idxFilterInfo, Com_FilterInfo_SInt8IterType idxSint8FilterInfo, sint8 newValue, sint8 oldValue)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if(Com_SignalFilterHdlrHelper_HasTimeoutOccurred(idxFilterInfo))
  {
    filterState = TRUE;
  }
  else
  {
    const uint8 filterMask = Com_GetFilterMaskOfFilterInfo_SInt8(idxSint8FilterInfo);
    if((((uint8) newValue) & filterMask) != (((uint8) oldValue) & filterMask))
    {
      filterState = TRUE;
    }
  }

  return filterState;
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_SInt8_NewIsWithIn
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_SInt8_NewIsWithIn(Com_FilterInfo_SInt8IterType idxFilterInfo, sint8 newValue)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */

  const sint8 filterMin = Com_GetFilterMinOfFilterInfo_SInt8(idxFilterInfo);
  const sint8 filterMax = Com_GetFilterMaxOfFilterInfo_SInt8(idxFilterInfo);

  if((filterMin <= newValue) && (newValue <= filterMax))
  {
    filterState = TRUE;
  }

  return filterState;
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_UInt16_MaskedNewDiffersX
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_UInt16_MaskedNewDiffersX(Com_FilterInfo_UInt16IterType idxFilterInfo, uint16 newValue)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */

  const uint16 filterMask = Com_GetFilterMaskOfFilterInfo_UInt16(idxFilterInfo);
  const uint16 filterX = Com_GetFilterXOfFilterInfo_UInt16(idxFilterInfo);
  if((newValue & filterMask) != filterX)  /* PRQA S 2985 */ /* MD_Com_2985_2986 */
  {
    filterState = TRUE;
  }

  return filterState;
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_UInt16_MaskedNewDiffersMaskedOld
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_UInt16_MaskedNewDiffersMaskedOld(Com_FilterInfoIterType idxFilterInfo, Com_FilterInfo_UInt16IterType idxUint16FilterInfo, uint16 newValue, uint16 oldValue)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if(Com_SignalFilterHdlrHelper_HasTimeoutOccurred(idxFilterInfo))
  {
    filterState = TRUE;
  }
  else
  {
    const uint16 filterMask = Com_GetFilterMaskOfFilterInfo_UInt16(idxUint16FilterInfo);
    if((newValue & filterMask) != (oldValue & filterMask))  /* PRQA S 2985 */ /* MD_Com_2985_2986 */
    {
      filterState = TRUE;
    }
  }

  return filterState;
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_UInt16_NewIsWithIn
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_UInt16_NewIsWithIn(Com_FilterInfo_UInt16IterType idxFilterInfo, uint16 newValue)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */

  const uint16 filterMin = Com_GetFilterMinOfFilterInfo_UInt16(idxFilterInfo);
  const uint16 filterMax = Com_GetFilterMaxOfFilterInfo_UInt16(idxFilterInfo);

  if((filterMin <= newValue) && (newValue <= filterMax))
  {
    filterState = TRUE;
  }

  return filterState;
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_SInt16_MaskedNewDiffersX
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_SInt16_MaskedNewDiffersX(Com_FilterInfo_SInt16IterType idxFilterInfo, sint16 newValue)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */

  const uint16 filterMask = Com_GetFilterMaskOfFilterInfo_SInt16(idxFilterInfo);
  const sint16 filterX = Com_GetFilterXOfFilterInfo_SInt16(idxFilterInfo);
  if((sint16) (((uint16) newValue) & filterMask) != filterX) /* PRQA S 4394 */ /* MD_Com_4394 */
  {
    filterState = TRUE;
  }

  return filterState;
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_SInt16_MaskedNewDiffersMaskedOld
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_SInt16_MaskedNewDiffersMaskedOld(Com_FilterInfoIterType idxFilterInfo, Com_FilterInfo_SInt16IterType idxSint16FilterInfo, sint16 newValue, sint16 oldValue)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if(Com_SignalFilterHdlrHelper_HasTimeoutOccurred(idxFilterInfo))
  {
    filterState = TRUE;
  }
  else
  {
    const uint16 filterMask = Com_GetFilterMaskOfFilterInfo_SInt16(idxSint16FilterInfo);
    if((((uint16) newValue) & filterMask) != (((uint16) oldValue) & filterMask))
    {
      filterState = TRUE;
    }
  }

  return filterState;
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_SInt16_NewIsWithIn
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_SInt16_NewIsWithIn(Com_FilterInfo_SInt16IterType idxFilterInfo, sint16 newValue)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */

  const sint16 filterMin = Com_GetFilterMinOfFilterInfo_SInt16(idxFilterInfo);
  const sint16 filterMax = Com_GetFilterMaxOfFilterInfo_SInt16(idxFilterInfo);

  if((filterMin <= newValue) && (newValue <= filterMax))
  {
    filterState = TRUE;
  }

  return filterState;
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_UInt32_MaskedNewDiffersX
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_UInt32_MaskedNewDiffersX(Com_FilterInfo_UInt32IterType idxFilterInfo, uint32 newValue)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */

  const uint32 filterMask = Com_GetFilterMaskOfFilterInfo_UInt32(idxFilterInfo);
  const uint32 filterX = Com_GetFilterXOfFilterInfo_UInt32(idxFilterInfo);
  if((newValue & filterMask) != filterX)  /* PRQA S 2985 */ /* MD_Com_2985_2986 */
  {
    filterState = TRUE;
  }

  return filterState;
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_UInt32_MaskedNewDiffersMaskedOld
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_UInt32_MaskedNewDiffersMaskedOld(Com_FilterInfoIterType idxFilterInfo, Com_FilterInfo_UInt32IterType idxUint32FilterInfo, uint32 newValue, uint32 oldValue)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if(Com_SignalFilterHdlrHelper_HasTimeoutOccurred(idxFilterInfo))
  {
    filterState = TRUE;
  }
  else
  {
    const uint32 filterMask = Com_GetFilterMaskOfFilterInfo_UInt32(idxUint32FilterInfo);
    if((newValue & filterMask) != (oldValue & filterMask))  /* PRQA S 2985 */ /* MD_Com_2985_2986 */
    {
      filterState = TRUE;
    }
  }

  return filterState;
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_UInt32_NewIsWithIn
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_UInt32_NewIsWithIn(Com_FilterInfo_UInt32IterType idxFilterInfo, uint32 newValue)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */

  const uint32 filterMin = Com_GetFilterMinOfFilterInfo_UInt32(idxFilterInfo);
  const uint32 filterMax = Com_GetFilterMaxOfFilterInfo_UInt32(idxFilterInfo);

  if((filterMin <= newValue) && (newValue <= filterMax))
  {
    filterState = TRUE;
  }

  return filterState;
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_SInt32_MaskedNewDiffersX
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_SInt32_MaskedNewDiffersX(Com_FilterInfo_SInt32IterType idxFilterInfo, sint32 newValue)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */

  const uint32 filterMask = Com_GetFilterMaskOfFilterInfo_SInt32(idxFilterInfo);
  const sint32 filterX = Com_GetFilterXOfFilterInfo_SInt32(idxFilterInfo);
  if((sint32) (((uint32) newValue) & filterMask) != filterX)  /* PRQA S 2985, 4394 */ /* MD_Com_2985_2986, MD_Com_4394 */
  {
    filterState = TRUE;
  }

  return filterState;
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_SInt32_MaskedNewDiffersMaskedOld
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_SInt32_MaskedNewDiffersMaskedOld(Com_FilterInfoIterType idxFilterInfo, Com_FilterInfo_SInt32IterType idxSint32FilterInfo, sint32 newValue, sint32 oldValue)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if(Com_SignalFilterHdlrHelper_HasTimeoutOccurred(idxFilterInfo))
  {
    filterState = TRUE;
  }
  else
  {
    const uint32 filterMask = Com_GetFilterMaskOfFilterInfo_SInt32(idxSint32FilterInfo);
    if((((uint32) newValue) & filterMask) != (((uint32) oldValue) & filterMask))  /* PRQA S 2985 */ /* MD_Com_2985_2986 */
    {
      filterState = TRUE;
    }
  }

  return filterState;
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_SInt32_NewIsWithIn
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_SInt32_NewIsWithIn(Com_FilterInfo_SInt32IterType idxFilterInfo, sint32 newValue)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */

  const sint32 filterMin = Com_GetFilterMinOfFilterInfo_SInt32(idxFilterInfo);
  const sint32 filterMax = Com_GetFilterMaxOfFilterInfo_SInt32(idxFilterInfo);

  if((filterMin <= newValue) && (newValue <= filterMax))
  {
    filterState = TRUE;
  }

  return filterState;
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_UInt64_MaskedNewDiffersX
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_UInt64_MaskedNewDiffersX(Com_FilterInfo_UInt64IterType idxFilterInfo, uint64 newValue)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */

  const uint64 filterMask = Com_GetFilterMaskOfFilterInfo_UInt64(idxFilterInfo);
  const uint64 filterX = Com_GetFilterXOfFilterInfo_UInt64(idxFilterInfo);
  if((newValue & filterMask) != filterX)
  {
    filterState = TRUE;
  }

  return filterState;
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_UInt64_MaskedNewDiffersMaskedOld
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_UInt64_MaskedNewDiffersMaskedOld(Com_FilterInfoIterType idxFilterInfo, Com_FilterInfo_UInt64IterType idxUint64FilterInfo, uint64 newValue, uint64 oldValue)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if(Com_SignalFilterHdlrHelper_HasTimeoutOccurred(idxFilterInfo))
  {
    filterState = TRUE;
  }
  else
  {
    const uint64 filterMask = Com_GetFilterMaskOfFilterInfo_UInt64(idxUint64FilterInfo);
    if((newValue & filterMask) != (oldValue & filterMask))
    {
      filterState = TRUE;
    }
  }

  return filterState;
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_UInt64_NewIsWithIn
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_UInt64_NewIsWithIn(Com_FilterInfo_UInt64IterType idxFilterInfo, uint64 newValue)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */

  const uint64 filterMin = Com_GetFilterMinOfFilterInfo_UInt64(idxFilterInfo);
  const uint64 filterMax = Com_GetFilterMaxOfFilterInfo_UInt64(idxFilterInfo);

  if((filterMin <= newValue) && (newValue <= filterMax))
  {
    filterState = TRUE;
  }

  return filterState;
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_SInt64_MaskedNewDiffersX
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_SInt64_MaskedNewDiffersX(Com_FilterInfo_SInt64IterType idxFilterInfo, sint64 newValue)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */

  const uint64 filterMask = Com_GetFilterMaskOfFilterInfo_SInt64(idxFilterInfo);
  const sint64 filterX = Com_GetFilterXOfFilterInfo_SInt64(idxFilterInfo);
  if((sint64) (((uint64) newValue) & filterMask) != filterX) /* PRQA S 4394 */ /* MD_Com_4394 */
  {
    filterState = TRUE;
  }

  return filterState;
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_SInt64_MaskedNewDiffersMaskedOld
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_SInt64_MaskedNewDiffersMaskedOld(Com_FilterInfoIterType idxFilterInfo, Com_FilterInfo_SInt64IterType idxSint64FilterInfo, sint64 newValue, sint64 oldValue)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if(Com_SignalFilterHdlrHelper_HasTimeoutOccurred(idxFilterInfo))
  {
    filterState = TRUE;
  }
  else
  {
    const uint64 filterMask = Com_GetFilterMaskOfFilterInfo_SInt64(idxSint64FilterInfo);
    if((((uint64) newValue) & filterMask) != (((uint64) oldValue) & filterMask))
    {
      filterState = TRUE;
    }
  }

  return filterState;
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_SInt64_NewIsWithIn
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_SInt64_NewIsWithIn(Com_FilterInfo_SInt64IterType idxFilterInfo, sint64 newValue)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */

  const sint64 filterMin = Com_GetFilterMinOfFilterInfo_SInt64(idxFilterInfo);
  const sint64 filterMax = Com_GetFilterMaxOfFilterInfo_SInt64(idxFilterInfo);

  if((filterMin <= newValue) && (newValue <= filterMax))
  {
    filterState = TRUE;
  }

  return filterState;
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_Float32_MaskedNewDiffersMaskedOld
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_Float32_MaskedNewDiffersMaskedOld(Com_FilterInfoIterType idxFilterInfo, float32 newValue, float32 oldValue)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if(Com_SignalFilterHdlrHelper_HasTimeoutOccurred(idxFilterInfo))
  {
    filterState = TRUE;
  }
  else
  {
    filterState = !Com_Util_CompareFloat32Values(newValue, oldValue);
  }

  return filterState;
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_Float64_MaskedNewDiffersMaskedOld
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_Float64_MaskedNewDiffersMaskedOld(Com_FilterInfoIterType idxFilterInfo, float64 newValue, float64 oldValue)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if(Com_SignalFilterHdlrHelper_HasTimeoutOccurred(idxFilterInfo))
  {
    filterState = TRUE;
  }
  else
  {
    filterState = !Com_Util_CompareFloat64Values(newValue, oldValue);
  }

  return filterState;
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_UInt8_N_MaskedNewDiffersMaskedOld
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_UInt8_N_MaskedNewDiffersMaskedOld(Com_FilterInfoIterType idxFilterInfo, CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) oldValuePtr, CONSTP2CONST(ComDataTypeConst, AUTOMATIC, AUTOMATIC) comDataPtr)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if(Com_SignalFilterHdlrHelper_HasTimeoutOccurred(idxFilterInfo))
  {
    filterState = TRUE;
  }
  else
  {
    if((oldValuePtr != NULL_PTR) && (comDataPtr->dataPtr != NULL_PTR))
    {
      filterState = !Com_Util_CompareArrayBasedValues( (P2CONST(uint8, AUTOMATIC, AUTOMATIC)) comDataPtr->dataPtr, oldValuePtr, comDataPtr->dataLength); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
    }
  }

  return filterState;
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_SigGrpArr_MaskedNewDiffersX
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_SigGrpArr_MaskedNewDiffersX(P2CONST(uint8, AUTOMATIC, AUTOMATIC) newValuePtr, Com_SigGrpArrayFilterInfoIterType idxFilterInfoApplTypSpecific)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */

  const Com_ArrayBasedFilterMaskLengthOfSigGrpArrayFilterInfoType maskLength = Com_GetArrayBasedFilterMaskLengthOfSigGrpArrayFilterInfo(idxFilterInfoApplTypSpecific); /* PRQA S 2983 */ /* MD_Com_2982_2983 */
  const Com_ArrayBasedFilterMaskIterType idxFilterMask = Com_GetArrayBasedFilterMaskStartIdxOfSigGrpArrayFilterInfo(idxFilterInfoApplTypSpecific);
  const Com_ArrayBasedFilterValueXIterType idxFilterValueX = Com_GetArrayBasedFilterValueXStartIdxOfSigGrpArrayFilterInfo(idxFilterInfoApplTypSpecific);

  CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) maskPtr = Com_GetAddrArrayBasedFilterMask(idxFilterMask); /* PRQA S 2983 */ /* MD_Com_2982_2983 */
  CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) filterValueXPtr = Com_GetAddrArrayBasedFilterValueX(idxFilterValueX); /* PRQA S 2983 */ /* MD_Com_2982_2983 */
  
  for(Com_ArrayBasedFilterMaskLengthOfSigGrpArrayFilterInfoType i = 0u; i < maskLength; i++)  /* FETA_COM_01 */
  {
    /* Stop filter evaluation if at least one byte evaluates to TRUE */
    if((newValuePtr[i] & maskPtr[i]) != filterValueXPtr[i])
    {
      filterState = TRUE;
      break;
    }
  }

  return filterState;
}

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_SetFilterOldValueToInitValue
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
/* PRQA S 6030 1 */ /* MD_MSR_STCYC */
FUNC(void, COM_CODE) Com_SignalFilterHdlrHelper_SetFilterOldValueToInitValue(Com_FilterInfoIterType idxFilterInfo)
{
  switch (Com_GetFilterInitValueOfFilterInfo(idxFilterInfo))
  {
    case COM_TXFILTERINITVALUEUINT8_FILTERINITVALUEOFFILTERINFO:
    {
      const Com_TxFilterInitValueUInt8IterType idxTxFilterOldValue = (Com_TxFilterInitValueUInt8IterType) Com_GetFilterInitValueStartIdxOfFilterInfo(idxFilterInfo);
      /*@ assert idxTxFilterOldValue < Com_GetSizeOfTxFilterOldValueUInt8(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      Com_SetTxFilterOldValueUInt8(idxTxFilterOldValue, Com_GetTxFilterInitValueUInt8(idxTxFilterOldValue));
      break;
    }
    case COM_TXFILTERINITVALUESINT8_FILTERINITVALUEOFFILTERINFO:
    {
      const Com_TxFilterInitValueSInt8IterType idxTxFilterOldValue = (Com_TxFilterInitValueSInt8IterType) Com_GetFilterInitValueStartIdxOfFilterInfo(idxFilterInfo);
      /*@ assert idxTxFilterOldValue < Com_GetSizeOfTxFilterOldValueSInt8(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      Com_SetTxFilterOldValueSInt8(idxTxFilterOldValue, Com_GetTxFilterInitValueSInt8(idxTxFilterOldValue));
      break;
    }
    case COM_TXFILTERINITVALUEUINT16_FILTERINITVALUEOFFILTERINFO:
    {
      const Com_TxFilterInitValueUInt16IterType idxTxFilterOldValue = (Com_TxFilterInitValueUInt16IterType) Com_GetFilterInitValueStartIdxOfFilterInfo(idxFilterInfo);
      /*@ assert idxTxFilterOldValue < Com_GetSizeOfTxFilterOldValueUInt16(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      Com_SetTxFilterOldValueUInt16(idxTxFilterOldValue, Com_GetTxFilterInitValueUInt16(idxTxFilterOldValue));
      break;
    }
    case COM_TXFILTERINITVALUESINT16_FILTERINITVALUEOFFILTERINFO:
    {
      const Com_TxFilterInitValueSInt16IterType idxTxFilterOldValue = (Com_TxFilterInitValueSInt16IterType) Com_GetFilterInitValueStartIdxOfFilterInfo(idxFilterInfo);
      /*@ assert idxTxFilterOldValue < Com_GetSizeOfTxFilterOldValueSInt16(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      Com_SetTxFilterOldValueSInt16(idxTxFilterOldValue, Com_GetTxFilterInitValueSInt16(idxTxFilterOldValue));
      break;
    }
    case COM_TXFILTERINITVALUEUINT32_FILTERINITVALUEOFFILTERINFO:
    {
      const Com_TxFilterInitValueUInt32IterType idxTxFilterOldValue = (Com_TxFilterInitValueUInt32IterType) Com_GetFilterInitValueStartIdxOfFilterInfo(idxFilterInfo);
      /*@ assert idxTxFilterOldValue < Com_GetSizeOfTxFilterOldValueUInt32(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      Com_SetTxFilterOldValueUInt32(idxTxFilterOldValue, Com_GetTxFilterInitValueUInt32(idxTxFilterOldValue));
      break;
    }
    case COM_TXFILTERINITVALUESINT32_FILTERINITVALUEOFFILTERINFO:
    {
      const Com_TxFilterInitValueSInt32IterType idxTxFilterOldValue = (Com_TxFilterInitValueSInt32IterType) Com_GetFilterInitValueStartIdxOfFilterInfo(idxFilterInfo);
      /*@ assert idxTxFilterOldValue < Com_GetSizeOfTxFilterOldValueSInt32(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      Com_SetTxFilterOldValueSInt32(idxTxFilterOldValue, Com_GetTxFilterInitValueSInt32(idxTxFilterOldValue));
      break;
    }
    case COM_TXFILTERINITVALUEUINT64_FILTERINITVALUEOFFILTERINFO:
    {
      const Com_TxFilterInitValueUInt64IterType idxTxFilterOldValue = (Com_TxFilterInitValueUInt64IterType) Com_GetFilterInitValueStartIdxOfFilterInfo(idxFilterInfo);
      /*@ assert idxTxFilterOldValue < Com_GetSizeOfTxFilterOldValueUInt64(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      Com_SetTxFilterOldValueUInt64(idxTxFilterOldValue, Com_GetTxFilterInitValueUInt64(idxTxFilterOldValue));
      break;
    }
    case COM_TXFILTERINITVALUESINT64_FILTERINITVALUEOFFILTERINFO:
    {
      const Com_TxFilterInitValueSInt64IterType idxTxFilterOldValue = (Com_TxFilterInitValueSInt64IterType) Com_GetFilterInitValueStartIdxOfFilterInfo(idxFilterInfo);
      /*@ assert idxTxFilterOldValue < Com_GetSizeOfTxFilterOldValueSInt64(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      Com_SetTxFilterOldValueSInt64(idxTxFilterOldValue, Com_GetTxFilterInitValueSInt64(idxTxFilterOldValue));
      break;
    }
    case COM_TXFILTERINITVALUEFLOAT32_FILTERINITVALUEOFFILTERINFO:
    {
      const Com_TxFilterInitValueFloat32IterType idxTxFilterOldValue = (Com_TxFilterInitValueFloat32IterType) Com_GetFilterInitValueStartIdxOfFilterInfo(idxFilterInfo);
      /*@ assert idxTxFilterOldValue < Com_GetSizeOfTxFilterOldValueFloat32(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      Com_SetTxFilterOldValueFloat32(idxTxFilterOldValue, Com_GetTxFilterInitValueFloat32(idxTxFilterOldValue));
      break;
    }
    case COM_TXFILTERINITVALUEFLOAT64_FILTERINITVALUEOFFILTERINFO:
    {
      const Com_TxFilterInitValueFloat64IterType idxTxFilterOldValue = (Com_TxFilterInitValueFloat64IterType) Com_GetFilterInitValueStartIdxOfFilterInfo(idxFilterInfo);
      /*@ assert idxTxFilterOldValue < Com_GetSizeOfTxFilterOldValueFloat64(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      Com_SetTxFilterOldValueFloat64(idxTxFilterOldValue, Com_GetTxFilterInitValueFloat64(idxTxFilterOldValue));
      break;
    }
    case COM_TXFILTERINITVALUEARRAYBASED_FILTERINITVALUEOFFILTERINFO:
    {
      const Com_TxFilterInitValueArrayBasedIterType idxTxFilterOldValue = (Com_TxFilterInitValueArrayBasedIterType) Com_GetFilterInitValueStartIdxOfFilterInfo(idxFilterInfo);
      /*@ assert idxTxFilterOldValue < Com_GetSizeOfTxFilterOldValueArrayBased(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      VStdLib_MemCpy(Com_GetAddrTxFilterOldValueArrayBased(idxTxFilterOldValue), Com_GetAddrTxFilterInitValueArrayBased(idxTxFilterOldValue), Com_GetFilterInitValueLengthOfFilterInfo(idxFilterInfo)); /* VCA_COM_MEMCPY_CSL_DEST */  /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */
      break;
    }
    default:  /* case COM_NO_FILTERINITVALUEOFFILTERINFO */
      /* Intentionally Empty */
      break;
  }
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_SetFilterOldValueX
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
/* PRQA S 6030 1 */ /* MD_MSR_STCYC */
FUNC(void, COM_CODE) Com_SignalFilterHdlrHelper_SetFilterOldValueX(Com_FilterInfoIterType idxFilterInfo, P2CONST(void, AUTOMATIC, AUTOMATIC) SignalDataPtr)
{
  switch (Com_GetFilterInitValueOfFilterInfo(idxFilterInfo))
  {
    case COM_TXFILTERINITVALUEUINT8_FILTERINITVALUEOFFILTERINFO:
    {
      const Com_TxFilterOldValueUInt8IterType idxTxFilterOldValue = (Com_TxFilterOldValueUInt8IterType) Com_GetFilterInitValueStartIdxOfFilterInfo(idxFilterInfo);
      /*@ assert idxTxFilterOldValue < Com_GetSizeOfTxFilterOldValueUInt8(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      Com_SetTxFilterOldValueUInt8(idxTxFilterOldValue, *((P2CONST(uint8, AUTOMATIC, AUTOMATIC)) SignalDataPtr)); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TXFILTERINITVALUESINT8_FILTERINITVALUEOFFILTERINFO:
    {
      const Com_TxFilterOldValueSInt8IterType idxTxFilterOldValue = (Com_TxFilterOldValueSInt8IterType) Com_GetFilterInitValueStartIdxOfFilterInfo(idxFilterInfo);
      /*@ assert idxTxFilterOldValue < Com_GetSizeOfTxFilterOldValueSInt8(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      Com_SetTxFilterOldValueSInt8(idxTxFilterOldValue, *((P2CONST(sint8, AUTOMATIC, AUTOMATIC)) SignalDataPtr)); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TXFILTERINITVALUEUINT16_FILTERINITVALUEOFFILTERINFO:
    {
      const Com_TxFilterOldValueUInt16IterType idxTxFilterOldValue = (Com_TxFilterOldValueUInt16IterType) Com_GetFilterInitValueStartIdxOfFilterInfo(idxFilterInfo);
      /*@ assert idxTxFilterOldValue < Com_GetSizeOfTxFilterOldValueUInt16(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      Com_SetTxFilterOldValueUInt16(idxTxFilterOldValue, *((P2CONST(uint16, AUTOMATIC, AUTOMATIC)) SignalDataPtr)); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TXFILTERINITVALUESINT16_FILTERINITVALUEOFFILTERINFO:
    {
      const Com_TxFilterOldValueSInt16IterType idxTxFilterOldValue = (Com_TxFilterOldValueSInt16IterType) Com_GetFilterInitValueStartIdxOfFilterInfo(idxFilterInfo);
      /*@ assert idxTxFilterOldValue < Com_GetSizeOfTxFilterOldValueSInt16(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      Com_SetTxFilterOldValueSInt16(idxTxFilterOldValue, *((P2CONST(sint16, AUTOMATIC, AUTOMATIC)) SignalDataPtr)); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TXFILTERINITVALUEUINT32_FILTERINITVALUEOFFILTERINFO:
    {
      const Com_TxFilterOldValueUInt32IterType idxTxFilterOldValue = (Com_TxFilterOldValueUInt32IterType) Com_GetFilterInitValueStartIdxOfFilterInfo(idxFilterInfo);
      /*@ assert idxTxFilterOldValue < Com_GetSizeOfTxFilterOldValueUInt32(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      Com_SetTxFilterOldValueUInt32(idxTxFilterOldValue, *((P2CONST(uint32, AUTOMATIC, AUTOMATIC)) SignalDataPtr)); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TXFILTERINITVALUESINT32_FILTERINITVALUEOFFILTERINFO:
    {
      const Com_TxFilterOldValueSInt32IterType idxTxFilterOldValue = (Com_TxFilterOldValueSInt32IterType) Com_GetFilterInitValueStartIdxOfFilterInfo(idxFilterInfo);
      /*@ assert idxTxFilterOldValue < Com_GetSizeOfTxFilterOldValueSInt32(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      Com_SetTxFilterOldValueSInt32(idxTxFilterOldValue, *((P2CONST(sint32, AUTOMATIC, AUTOMATIC)) SignalDataPtr)); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TXFILTERINITVALUEUINT64_FILTERINITVALUEOFFILTERINFO:
    {
      const Com_TxFilterOldValueUInt64IterType idxTxFilterOldValue = (Com_TxFilterOldValueUInt64IterType) Com_GetFilterInitValueStartIdxOfFilterInfo(idxFilterInfo);
      /*@ assert idxTxFilterOldValue < Com_GetSizeOfTxFilterOldValueUInt64(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      Com_SetTxFilterOldValueUInt64(idxTxFilterOldValue, *((P2CONST(uint64, AUTOMATIC, AUTOMATIC)) SignalDataPtr)); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TXFILTERINITVALUESINT64_FILTERINITVALUEOFFILTERINFO:
    {
      const Com_TxFilterOldValueSInt64IterType idxTxFilterOldValue = (Com_TxFilterOldValueSInt64IterType) Com_GetFilterInitValueStartIdxOfFilterInfo(idxFilterInfo);
      /*@ assert idxTxFilterOldValue < Com_GetSizeOfTxFilterOldValueSInt64(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      Com_SetTxFilterOldValueSInt64(idxTxFilterOldValue, *((P2CONST(sint64, AUTOMATIC, AUTOMATIC)) SignalDataPtr)); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TXFILTERINITVALUEFLOAT32_FILTERINITVALUEOFFILTERINFO:
    {
      const Com_TxFilterOldValueFloat32IterType idxTxFilterOldValue = (Com_TxFilterOldValueFloat32IterType) Com_GetFilterInitValueStartIdxOfFilterInfo(idxFilterInfo);
      /*@ assert idxTxFilterOldValue < Com_GetSizeOfTxFilterOldValueFloat32(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      Com_SetTxFilterOldValueFloat32(idxTxFilterOldValue, *((P2CONST(float32, AUTOMATIC, AUTOMATIC)) SignalDataPtr)); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TXFILTERINITVALUEFLOAT64_FILTERINITVALUEOFFILTERINFO:
    {
      const Com_TxFilterOldValueFloat64IterType idxTxFilterOldValue = (Com_TxFilterOldValueFloat64IterType) Com_GetFilterInitValueStartIdxOfFilterInfo(idxFilterInfo);
     /*@ assert idxTxFilterOldValue < Com_GetSizeOfTxFilterOldValueFloat64(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      Com_SetTxFilterOldValueFloat64(idxTxFilterOldValue, *((P2CONST(float64, AUTOMATIC, AUTOMATIC)) SignalDataPtr)); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TXFILTERINITVALUEARRAYBASED_FILTERINITVALUEOFFILTERINFO:
    {
      const Com_TxFilterOldValueArrayBasedIterType idxTxFilterOldValue = (Com_TxFilterOldValueArrayBasedIterType) Com_GetFilterInitValueStartIdxOfFilterInfo(idxFilterInfo);
      /*@ assert idxTxFilterOldValue < Com_GetSizeOfTxFilterOldValueArrayBased(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      VStdLib_MemCpy(Com_GetAddrTxFilterOldValueArrayBased(idxTxFilterOldValue), SignalDataPtr, Com_GetFilterInitValueLengthOfFilterInfo(idxFilterInfo)); /* VCA_COM_VCA_ASSUMES_MEMCPY_OVERWRITES_MEMORY */    /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */
      break;
    }
    default:  /* case COM_NO_FILTERINITVALUEOFFILTERINFO */
      /* Intentionally Empty */
      break;
  }
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_GetOldValuePtr
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
/* PRQA S 6030 1 */ /* MD_MSR_STCYC */
FUNC(P2CONST(void, AUTOMATIC, AUTOMATIC), COM_CODE) Com_SignalFilterHdlrHelper_GetOldValuePtr(Com_FilterInfoIterType idxFilterInfo)
{
  P2CONST(void, AUTOMATIC, AUTOMATIC) oldValuePtr = NULL_PTR;

  switch (Com_GetFilterInitValueOfFilterInfo(idxFilterInfo))
  {
    case COM_TXFILTERINITVALUEUINT8_FILTERINITVALUEOFFILTERINFO:
    {
      const Com_TxFilterOldValueUInt8IterType idxTxFilterOldValue = (Com_TxFilterOldValueUInt8IterType) Com_GetFilterInitValueStartIdxOfFilterInfo(idxFilterInfo);
      oldValuePtr = (P2CONST(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTxFilterOldValueUInt8(idxTxFilterOldValue); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TXFILTERINITVALUESINT8_FILTERINITVALUEOFFILTERINFO:
    {
      const Com_TxFilterOldValueSInt8IterType idxTxFilterOldValue = (Com_TxFilterOldValueSInt8IterType) Com_GetFilterInitValueStartIdxOfFilterInfo(idxFilterInfo);
      oldValuePtr = (P2CONST(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTxFilterOldValueSInt8(idxTxFilterOldValue); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TXFILTERINITVALUEUINT16_FILTERINITVALUEOFFILTERINFO:
    {
      const Com_TxFilterOldValueUInt16IterType idxTxFilterOldValue = (Com_TxFilterOldValueUInt16IterType) Com_GetFilterInitValueStartIdxOfFilterInfo(idxFilterInfo);
      oldValuePtr = (P2CONST(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTxFilterOldValueUInt16(idxTxFilterOldValue); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TXFILTERINITVALUESINT16_FILTERINITVALUEOFFILTERINFO:
    {
      const Com_TxFilterOldValueSInt16IterType idxTxFilterOldValue = (Com_TxFilterOldValueSInt16IterType) Com_GetFilterInitValueStartIdxOfFilterInfo(idxFilterInfo);
      oldValuePtr = (P2CONST(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTxFilterOldValueSInt16(idxTxFilterOldValue); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TXFILTERINITVALUEUINT32_FILTERINITVALUEOFFILTERINFO:
    {
      const Com_TxFilterOldValueUInt32IterType idxTxFilterOldValue = (Com_TxFilterOldValueUInt32IterType) Com_GetFilterInitValueStartIdxOfFilterInfo(idxFilterInfo);
      oldValuePtr = (P2CONST(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTxFilterOldValueUInt32(idxTxFilterOldValue); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TXFILTERINITVALUESINT32_FILTERINITVALUEOFFILTERINFO:
    {
      const Com_TxFilterOldValueSInt32IterType idxTxFilterOldValue = (Com_TxFilterOldValueSInt32IterType) Com_GetFilterInitValueStartIdxOfFilterInfo(idxFilterInfo);
      oldValuePtr = (P2CONST(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTxFilterOldValueSInt32(idxTxFilterOldValue); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TXFILTERINITVALUEUINT64_FILTERINITVALUEOFFILTERINFO:
    {
      const Com_TxFilterOldValueUInt64IterType idxTxFilterOldValue = (Com_TxFilterOldValueUInt64IterType) Com_GetFilterInitValueStartIdxOfFilterInfo(idxFilterInfo);
      oldValuePtr = (P2CONST(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTxFilterOldValueUInt64(idxTxFilterOldValue); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TXFILTERINITVALUESINT64_FILTERINITVALUEOFFILTERINFO:
    {
      const Com_TxFilterOldValueSInt64IterType idxTxFilterOldValue = (Com_TxFilterOldValueSInt64IterType) Com_GetFilterInitValueStartIdxOfFilterInfo(idxFilterInfo);
      oldValuePtr = (P2CONST(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTxFilterOldValueSInt64(idxTxFilterOldValue); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TXFILTERINITVALUEFLOAT32_FILTERINITVALUEOFFILTERINFO:
    {
      const Com_TxFilterOldValueFloat32IterType idxTxFilterOldValue = (Com_TxFilterOldValueFloat32IterType) Com_GetFilterInitValueStartIdxOfFilterInfo(idxFilterInfo);
      oldValuePtr = (P2CONST(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTxFilterOldValueFloat32(idxTxFilterOldValue); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TXFILTERINITVALUEFLOAT64_FILTERINITVALUEOFFILTERINFO:
    {
      const Com_TxFilterOldValueFloat64IterType idxTxFilterOldValue = (Com_TxFilterOldValueFloat64IterType) Com_GetFilterInitValueStartIdxOfFilterInfo(idxFilterInfo);
      oldValuePtr = (P2CONST(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTxFilterOldValueFloat64(idxTxFilterOldValue); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TXFILTERINITVALUEARRAYBASED_FILTERINITVALUEOFFILTERINFO:
    {
      const Com_TxFilterOldValueArrayBasedIterType idxTxFilterOldValue = (Com_TxFilterOldValueArrayBasedIterType) Com_GetFilterInitValueStartIdxOfFilterInfo(idxFilterInfo);
      oldValuePtr = (P2CONST(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTxFilterOldValueArrayBased(idxTxFilterOldValue); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    default:  /* case COM_NO_FILTERINITVALUEOFFILTERINFO */
      /* Intentionally Empty */
      break;
  }

  return oldValuePtr;
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_UInt8_EvaluateFilter
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_UInt8_EvaluateFilter(Com_FilterInfoIterType idxFilterInfo, uint8 newValue, uint8 oldValue)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */
  const Com_FilterInfo_UInt8IterType idxFilterInfoApplTypSpecific = (Com_FilterInfo_UInt8IterType) Com_GetApplTypSpecificFilterInfoIdxOfFilterInfo(idxFilterInfo);

  switch (Com_GetFilterAlgoOfFilterInfo_UInt8(idxFilterInfoApplTypSpecific))
  {
    case COM_ALWAYS_FILTERALGOOFFILTERINFO_UINT8:
    {
      filterState = TRUE;
      break;
    }
    case COM_MASKED_NEW_DIFFERS_X_FILTERALGOOFFILTERINFO_UINT8:
    {
      filterState = Com_SignalFilterHdlrHelper_UInt8_MaskedNewDiffersX(idxFilterInfoApplTypSpecific, newValue);
      break;
    }
    case COM_MASKED_NEW_DIFFERS_MASKED_OLD_FILTERALGOOFFILTERINFO_UINT8:
    {
      filterState = Com_SignalFilterHdlrHelper_UInt8_MaskedNewDiffersMaskedOld(idxFilterInfo, idxFilterInfoApplTypSpecific, newValue, oldValue);
      break;
    }
    case COM_MASKED_NEW_EQUALS_X_FILTERALGOOFFILTERINFO_UINT8:
    {
      filterState = !Com_SignalFilterHdlrHelper_UInt8_MaskedNewDiffersX(idxFilterInfoApplTypSpecific, newValue);
      break;
    }
    case COM_NEW_IS_OUTSIDE_FILTERALGOOFFILTERINFO_UINT8:
    {
      filterState = !Com_SignalFilterHdlrHelper_UInt8_NewIsWithIn(idxFilterInfoApplTypSpecific, newValue);
      break;
    }
    case COM_NEW_IS_WITHIN_FILTERALGOOFFILTERINFO_UINT8:
    {
      filterState = Com_SignalFilterHdlrHelper_UInt8_NewIsWithIn(idxFilterInfoApplTypSpecific, newValue);
      break;
    }
    default: /* case COM_NEVER_FILTERALGOOFFILTERINFO_UINT8 */
      /* Intentionally Empty */
      break;
  }
  return filterState;
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_SInt8_EvaluateFilter
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_SInt8_EvaluateFilter(Com_FilterInfoIterType idxFilterInfo, sint8 newValue, sint8 oldValue)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */
  const Com_FilterInfo_SInt8IterType idxFilterInfoApplTypSpecific = (Com_FilterInfo_SInt8IterType) Com_GetApplTypSpecificFilterInfoIdxOfFilterInfo(idxFilterInfo);

  switch (Com_GetFilterAlgoOfFilterInfo_SInt8(idxFilterInfoApplTypSpecific))
  {
    case COM_ALWAYS_FILTERALGOOFFILTERINFO_SINT8:
    {
      filterState = TRUE;
      break;
    }
    case COM_MASKED_NEW_DIFFERS_X_FILTERALGOOFFILTERINFO_SINT8:
    {
      filterState = Com_SignalFilterHdlrHelper_SInt8_MaskedNewDiffersX(idxFilterInfoApplTypSpecific, newValue);
      break;
    }
    case COM_MASKED_NEW_DIFFERS_MASKED_OLD_FILTERALGOOFFILTERINFO_SINT8:
    {
      filterState = Com_SignalFilterHdlrHelper_SInt8_MaskedNewDiffersMaskedOld(idxFilterInfo, idxFilterInfoApplTypSpecific, newValue, oldValue);
      break;
    }
    case COM_MASKED_NEW_EQUALS_X_FILTERALGOOFFILTERINFO_SINT8:
    {
      filterState = !Com_SignalFilterHdlrHelper_SInt8_MaskedNewDiffersX(idxFilterInfoApplTypSpecific, newValue);
      break;
    }
    case COM_NEW_IS_OUTSIDE_FILTERALGOOFFILTERINFO_SINT8:
    {
      filterState = !Com_SignalFilterHdlrHelper_SInt8_NewIsWithIn(idxFilterInfoApplTypSpecific, newValue);
      break;
    }
    case COM_NEW_IS_WITHIN_FILTERALGOOFFILTERINFO_SINT8:
    {
      filterState = Com_SignalFilterHdlrHelper_SInt8_NewIsWithIn(idxFilterInfoApplTypSpecific, newValue);
      break;
    }
    default: /* case COM_NEVER_FILTERALGOOFFILTERINFO_SINT8 */
      /* Intentionally Empty */
      break;
  }
  return filterState;
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_UInt16_EvaluateFilter
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_UInt16_EvaluateFilter(Com_FilterInfoIterType idxFilterInfo, uint16 newValue, uint16 oldValue)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */
  const Com_FilterInfo_UInt16IterType idxFilterInfoApplTypSpecific = (Com_FilterInfo_UInt16IterType) Com_GetApplTypSpecificFilterInfoIdxOfFilterInfo(idxFilterInfo);

  switch (Com_GetFilterAlgoOfFilterInfo_UInt16(idxFilterInfoApplTypSpecific))
  {
    case COM_ALWAYS_FILTERALGOOFFILTERINFO_UINT16:
    {
      filterState = TRUE;
      break;
    }
    case COM_MASKED_NEW_DIFFERS_X_FILTERALGOOFFILTERINFO_UINT16:
    {
      filterState = Com_SignalFilterHdlrHelper_UInt16_MaskedNewDiffersX(idxFilterInfoApplTypSpecific, newValue);
      break;
    }
    case COM_MASKED_NEW_DIFFERS_MASKED_OLD_FILTERALGOOFFILTERINFO_UINT16:
    {
      filterState = Com_SignalFilterHdlrHelper_UInt16_MaskedNewDiffersMaskedOld(idxFilterInfo, idxFilterInfoApplTypSpecific, newValue, oldValue);
      break;
    }
    case COM_MASKED_NEW_EQUALS_X_FILTERALGOOFFILTERINFO_UINT16:
    {
      filterState = !Com_SignalFilterHdlrHelper_UInt16_MaskedNewDiffersX(idxFilterInfoApplTypSpecific, newValue);
      break;
    }
    case COM_NEW_IS_OUTSIDE_FILTERALGOOFFILTERINFO_UINT16:
    {
      filterState = !Com_SignalFilterHdlrHelper_UInt16_NewIsWithIn(idxFilterInfoApplTypSpecific, newValue);
      break;
    }
    case COM_NEW_IS_WITHIN_FILTERALGOOFFILTERINFO_UINT16:
    {
      filterState = Com_SignalFilterHdlrHelper_UInt16_NewIsWithIn(idxFilterInfoApplTypSpecific, newValue);
      break;
    }
    default: /* case COM_NEVER_FILTERALGOOFFILTERINFO_UINT16 */
      /* Intentionally Empty */
      break;
  }
  return filterState;
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_SInt16_EvaluateFilter
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_SInt16_EvaluateFilter(Com_FilterInfoIterType idxFilterInfo, sint16 newValue, sint16 oldValue)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */
  const Com_FilterInfo_SInt16IterType idxFilterInfoApplTypSpecific = (Com_FilterInfo_SInt16IterType) Com_GetApplTypSpecificFilterInfoIdxOfFilterInfo(idxFilterInfo);

  switch (Com_GetFilterAlgoOfFilterInfo_SInt16(idxFilterInfoApplTypSpecific))
  {
    case COM_ALWAYS_FILTERALGOOFFILTERINFO_SINT16:
    {
      filterState = TRUE;
      break;
    }
    case COM_MASKED_NEW_DIFFERS_X_FILTERALGOOFFILTERINFO_SINT16:
    {
      filterState = Com_SignalFilterHdlrHelper_SInt16_MaskedNewDiffersX(idxFilterInfoApplTypSpecific, newValue);
      break;
    }
    case COM_MASKED_NEW_DIFFERS_MASKED_OLD_FILTERALGOOFFILTERINFO_SINT16:
    {
      filterState = Com_SignalFilterHdlrHelper_SInt16_MaskedNewDiffersMaskedOld(idxFilterInfo, idxFilterInfoApplTypSpecific, newValue, oldValue);
      break;
    }
    case COM_MASKED_NEW_EQUALS_X_FILTERALGOOFFILTERINFO_SINT16:
    {
      filterState = !Com_SignalFilterHdlrHelper_SInt16_MaskedNewDiffersX(idxFilterInfoApplTypSpecific, newValue);
      break;
    }
    case COM_NEW_IS_OUTSIDE_FILTERALGOOFFILTERINFO_SINT16:
    {
      filterState = !Com_SignalFilterHdlrHelper_SInt16_NewIsWithIn(idxFilterInfoApplTypSpecific, newValue);
      break;
    }
    case COM_NEW_IS_WITHIN_FILTERALGOOFFILTERINFO_SINT16:
    {
      filterState = Com_SignalFilterHdlrHelper_SInt16_NewIsWithIn(idxFilterInfoApplTypSpecific, newValue);
      break;
    }
    default: /* case COM_NEVER_FILTERALGOOFFILTERINFO_SINT16 */
      /* Intentionally Empty */
      break;
  }
  return filterState;
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_UInt32_EvaluateFilter
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_UInt32_EvaluateFilter(Com_FilterInfoIterType idxFilterInfo, uint32 newValue, uint32 oldValue)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */
  const Com_FilterInfo_UInt32IterType idxFilterInfoApplTypSpecific = (Com_FilterInfo_UInt32IterType) Com_GetApplTypSpecificFilterInfoIdxOfFilterInfo(idxFilterInfo);

  switch (Com_GetFilterAlgoOfFilterInfo_UInt32(idxFilterInfoApplTypSpecific))
  {
    case COM_ALWAYS_FILTERALGOOFFILTERINFO_UINT32:
    {
      filterState = TRUE;
      break;
    }
    case COM_MASKED_NEW_DIFFERS_X_FILTERALGOOFFILTERINFO_UINT32:
    {
      filterState = Com_SignalFilterHdlrHelper_UInt32_MaskedNewDiffersX(idxFilterInfoApplTypSpecific, newValue);
      break;
    }
    case COM_MASKED_NEW_DIFFERS_MASKED_OLD_FILTERALGOOFFILTERINFO_UINT32:
    {
      filterState = Com_SignalFilterHdlrHelper_UInt32_MaskedNewDiffersMaskedOld(idxFilterInfo, idxFilterInfoApplTypSpecific, newValue, oldValue);
      break;
    }
    case COM_MASKED_NEW_EQUALS_X_FILTERALGOOFFILTERINFO_UINT32:
    {
      filterState = !Com_SignalFilterHdlrHelper_UInt32_MaskedNewDiffersX(idxFilterInfoApplTypSpecific, newValue);
      break;
    }
    case COM_NEW_IS_OUTSIDE_FILTERALGOOFFILTERINFO_UINT32:
    {
      filterState = !Com_SignalFilterHdlrHelper_UInt32_NewIsWithIn(idxFilterInfoApplTypSpecific, newValue);
      break;
    }
    case COM_NEW_IS_WITHIN_FILTERALGOOFFILTERINFO_UINT32:
    {
      filterState = Com_SignalFilterHdlrHelper_UInt32_NewIsWithIn(idxFilterInfoApplTypSpecific, newValue);
      break;
    }
    default: /* case COM_NEVER_FILTERALGOOFFILTERINFO_UINT32 */
      /* Intentionally Empty */
      break;
  }
  return filterState;
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_SInt32_EvaluateFilter
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_SInt32_EvaluateFilter(Com_FilterInfoIterType idxFilterInfo, sint32 newValue, sint32 oldValue)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */
  const Com_FilterInfo_SInt32IterType idxFilterInfoApplTypSpecific = (Com_FilterInfo_SInt32IterType) Com_GetApplTypSpecificFilterInfoIdxOfFilterInfo(idxFilterInfo);

  switch (Com_GetFilterAlgoOfFilterInfo_SInt32(idxFilterInfoApplTypSpecific))
  {
    case COM_ALWAYS_FILTERALGOOFFILTERINFO_SINT32:
    {
      filterState = TRUE;
      break;
    }
    case COM_MASKED_NEW_DIFFERS_X_FILTERALGOOFFILTERINFO_SINT32:
    {
      filterState = Com_SignalFilterHdlrHelper_SInt32_MaskedNewDiffersX(idxFilterInfoApplTypSpecific, newValue);
      break;
    }
    case COM_MASKED_NEW_DIFFERS_MASKED_OLD_FILTERALGOOFFILTERINFO_SINT32:
    {
      filterState = Com_SignalFilterHdlrHelper_SInt32_MaskedNewDiffersMaskedOld(idxFilterInfo, idxFilterInfoApplTypSpecific, newValue, oldValue);
      break;
    }
    case COM_MASKED_NEW_EQUALS_X_FILTERALGOOFFILTERINFO_SINT32:
    {
      filterState = !Com_SignalFilterHdlrHelper_SInt32_MaskedNewDiffersX(idxFilterInfoApplTypSpecific, newValue);
      break;
    }
    case COM_NEW_IS_OUTSIDE_FILTERALGOOFFILTERINFO_SINT32:
    {
      filterState = !Com_SignalFilterHdlrHelper_SInt32_NewIsWithIn(idxFilterInfoApplTypSpecific, newValue);
      break;
    }
    case COM_NEW_IS_WITHIN_FILTERALGOOFFILTERINFO_SINT32:
    {
      filterState = Com_SignalFilterHdlrHelper_SInt32_NewIsWithIn(idxFilterInfoApplTypSpecific, newValue);
      break;
    }
    default: /* case COM_NEVER_FILTERALGOOFFILTERINFO_SINT32 */
      /* Intentionally Empty */
      break;
  }
  return filterState;
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_UInt64_EvaluateFilter
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_UInt64_EvaluateFilter(Com_FilterInfoIterType idxFilterInfo, uint64 newValue,  uint64 oldValue)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */
  const Com_FilterInfo_UInt64IterType idxFilterInfoApplTypSpecific = (Com_FilterInfo_UInt64IterType) Com_GetApplTypSpecificFilterInfoIdxOfFilterInfo(idxFilterInfo);

  switch (Com_GetFilterAlgoOfFilterInfo_UInt64(idxFilterInfoApplTypSpecific))
  {
    case COM_ALWAYS_FILTERALGOOFFILTERINFO_UINT64:
    {
      filterState = TRUE;
      break;
    }
    case COM_MASKED_NEW_DIFFERS_X_FILTERALGOOFFILTERINFO_UINT64:
    {
      filterState = Com_SignalFilterHdlrHelper_UInt64_MaskedNewDiffersX(idxFilterInfoApplTypSpecific, newValue);
      break;
    }
    case COM_MASKED_NEW_DIFFERS_MASKED_OLD_FILTERALGOOFFILTERINFO_UINT64:
    {
      filterState = Com_SignalFilterHdlrHelper_UInt64_MaskedNewDiffersMaskedOld(idxFilterInfo, idxFilterInfoApplTypSpecific, newValue, oldValue);
      break;
    }
    case COM_MASKED_NEW_EQUALS_X_FILTERALGOOFFILTERINFO_UINT64:
    {
      filterState = !Com_SignalFilterHdlrHelper_UInt64_MaskedNewDiffersX(idxFilterInfoApplTypSpecific, newValue);
      break;
    }
    case COM_NEW_IS_OUTSIDE_FILTERALGOOFFILTERINFO_UINT64:
    {
      filterState = !Com_SignalFilterHdlrHelper_UInt64_NewIsWithIn(idxFilterInfoApplTypSpecific, newValue);
      break;
    }
    case COM_NEW_IS_WITHIN_FILTERALGOOFFILTERINFO_UINT64:
    {
      filterState = Com_SignalFilterHdlrHelper_UInt64_NewIsWithIn(idxFilterInfoApplTypSpecific, newValue);
      break;
    }
    default: /* case COM_NEVER_FILTERALGOOFFILTERINFO_UINT64 */
      /* Intentionally Empty */
      break;
  }
  return filterState;
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_SInt64_EvaluateFilter
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_SInt64_EvaluateFilter(Com_FilterInfoIterType idxFilterInfo, sint64 newValue, sint64 oldValue)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */
  const Com_FilterInfo_SInt64IterType idxFilterInfoApplTypSpecific = (Com_FilterInfo_SInt64IterType) Com_GetApplTypSpecificFilterInfoIdxOfFilterInfo(idxFilterInfo);

  switch (Com_GetFilterAlgoOfFilterInfo_SInt64(idxFilterInfoApplTypSpecific))
  {
    case COM_ALWAYS_FILTERALGOOFFILTERINFO_SINT64:
    {
      filterState = TRUE;
      break;
    }
    case COM_MASKED_NEW_DIFFERS_X_FILTERALGOOFFILTERINFO_SINT64:
    {
      filterState = Com_SignalFilterHdlrHelper_SInt64_MaskedNewDiffersX(idxFilterInfoApplTypSpecific, newValue);
      break;
    }
    case COM_MASKED_NEW_DIFFERS_MASKED_OLD_FILTERALGOOFFILTERINFO_SINT64:
    {
      filterState = Com_SignalFilterHdlrHelper_SInt64_MaskedNewDiffersMaskedOld(idxFilterInfo, idxFilterInfoApplTypSpecific, newValue, oldValue);
      break;
    }
    case COM_MASKED_NEW_EQUALS_X_FILTERALGOOFFILTERINFO_SINT64:
    {
      filterState = !Com_SignalFilterHdlrHelper_SInt64_MaskedNewDiffersX(idxFilterInfoApplTypSpecific, newValue);
      break;
    }
    case COM_NEW_IS_OUTSIDE_FILTERALGOOFFILTERINFO_SINT64:
    {
      filterState = !Com_SignalFilterHdlrHelper_SInt64_NewIsWithIn(idxFilterInfoApplTypSpecific, newValue);
      break;
    }
    case COM_NEW_IS_WITHIN_FILTERALGOOFFILTERINFO_SINT64:
    {
      filterState = Com_SignalFilterHdlrHelper_SInt64_NewIsWithIn(idxFilterInfoApplTypSpecific, newValue);
      break;
    }
    default: /* case COM_NEVER_FILTERALGOOFFILTERINFO_SINT64 */
      /* Intentionally Empty */
      break;
  }
  return filterState;
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_Float32_EvaluateFilter
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_Float32_EvaluateFilter(Com_FilterInfoIterType idxFilterInfo, float32 newValue, float32 oldValue)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */
  const Com_FilterInfo_Float32IterType idxFilterInfoApplTypSpecific = (Com_FilterInfo_Float32IterType) Com_GetApplTypSpecificFilterInfoIdxOfFilterInfo(idxFilterInfo);

  switch (Com_GetFilterAlgoOfFilterInfo_Float32(idxFilterInfoApplTypSpecific))
  {
    case COM_ALWAYS_FILTERALGOOFFILTERINFO_FLOAT32:
    {
      filterState = TRUE;
      break;
    }
    case COM_MASKED_NEW_DIFFERS_MASKED_OLD_FILTERALGOOFFILTERINFO_FLOAT32:
    {
      filterState = Com_SignalFilterHdlrHelper_Float32_MaskedNewDiffersMaskedOld(idxFilterInfo, newValue, oldValue);
      break;
    }
    default: /* case COM_NEVER_FILTERALGOOFFILTERINFO_FLOAT32 */
      /* Intentionally Empty */
      break;
  }
  return filterState;
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_Float64_EvaluateFilter
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_Float64_EvaluateFilter(Com_FilterInfoIterType idxFilterInfo, float64 newValue, float64 oldValue)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */
  const Com_FilterInfo_Float64IterType idxFilterInfoApplTypSpecific = (Com_FilterInfo_Float64IterType) Com_GetApplTypSpecificFilterInfoIdxOfFilterInfo(idxFilterInfo);

  switch (Com_GetFilterAlgoOfFilterInfo_Float64(idxFilterInfoApplTypSpecific))
  {
    case COM_ALWAYS_FILTERALGOOFFILTERINFO_FLOAT64:
    {
      filterState = TRUE;
      break;
    }
    case COM_MASKED_NEW_DIFFERS_MASKED_OLD_FILTERALGOOFFILTERINFO_FLOAT64:
    {
      filterState = Com_SignalFilterHdlrHelper_Float64_MaskedNewDiffersMaskedOld(idxFilterInfo, newValue, oldValue);
      break;
    }
    default: /* case COM_NEVER_FILTERALGOOFFILTERINFO_FLOAT64 */
      /* Intentionally Empty */
      break;
  }
  return filterState;
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_UInt8_N_EvaluateFilter
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_UInt8_N_EvaluateFilter(Com_FilterInfoIterType idxFilterInfo, CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) oldValuePtr, CONSTP2CONST(ComDataTypeConst, AUTOMATIC, AUTOMATIC) comDataPtr)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */
  const Com_FilterInfo_UInt8_NIterType idxFilterInfoApplTypSpecific = (Com_FilterInfo_UInt8_NIterType) Com_GetApplTypSpecificFilterInfoIdxOfFilterInfo(idxFilterInfo);

  switch (Com_GetFilterAlgoOfFilterInfo_UInt8_N(idxFilterInfoApplTypSpecific))
  {
    case COM_ALWAYS_FILTERALGOOFFILTERINFO_UINT8_N:
    {
      filterState = TRUE;
      break;
    }
    case COM_MASKED_NEW_DIFFERS_MASKED_OLD_FILTERALGOOFFILTERINFO_UINT8_N:
    {
      filterState = Com_SignalFilterHdlrHelper_UInt8_N_MaskedNewDiffersMaskedOld(idxFilterInfo, oldValuePtr, comDataPtr);
      break;
    }
    default: /* case COM_NEVER_FILTERALGOOFFILTERINFO_UINT8_N */
      /* Intentionally Empty */
      break;
  }

  return filterState;
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_UInt8_Dyn_EvaluateFilter
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_UInt8_Dyn_EvaluateFilter(Com_FilterInfoIterType idxFilterInfo)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */
  const Com_FilterInfo_UInt8_DynIterType idxFilterInfoApplTypSpecific = (Com_FilterInfo_UInt8_DynIterType) Com_GetApplTypSpecificFilterInfoIdxOfFilterInfo(idxFilterInfo);

  switch (Com_GetFilterAlgoOfFilterInfo_UInt8_Dyn(idxFilterInfoApplTypSpecific))
  {
    case COM_ALWAYS_FILTERALGOOFFILTERINFO_UINT8_DYN:
    {
      filterState = TRUE;
      break;
    }
    default: /* case COM_NEVER_FILTERALGOOFFILTERINFO_UINT8_DYN */
      /* Intentionally Empty */
      break;
  }

  return filterState;
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_ZeroBit_EvaluateFilter
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_ZeroBit_EvaluateFilter(Com_FilterInfoIterType idxFilterInfo)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */
  const Com_FilterInfo_ZeroBitIterType idxFilterInfoApplTypSpecific = (Com_FilterInfo_ZeroBitIterType) Com_GetApplTypSpecificFilterInfoIdxOfFilterInfo(idxFilterInfo);

  switch (Com_GetFilterAlgoOfFilterInfo_ZeroBit(idxFilterInfoApplTypSpecific))
  {
    case COM_ALWAYS_FILTERALGOOFFILTERINFO_ZEROBIT:
    {
      filterState = TRUE;
      break;
    }
    default: /* case COM_NEVER_FILTERALGOOFFILTERINFO_ZEROBIT */
      /* Intentionally Empty */
      break;
  }

  return filterState;
}

/**********************************************************************************************************************
  Com_SignalFilterHdlrHelper_SigGrpArr_EvaluateFilter
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_SignalFilterHdlrHelper_SigGrpArr_EvaluateFilter(Com_SigGrpArrayFilterInfoIterType idxFilterInfoApplTypSpecific, P2CONST(uint8, AUTOMATIC, AUTOMATIC) newValuePtr)
{
  boolean filterState = FALSE;  /* PRQA S 2981, 2983 */ /* MD_MSR_RetVal */

  if(idxFilterInfoApplTypSpecific < Com_GetSizeOfSigGrpArrayFilterInfo())
  {
    switch (Com_GetFilterAlgoOfSigGrpArrayFilterInfo(idxFilterInfoApplTypSpecific))
    {
      case COM_ALWAYS_FILTERALGOOFSIGGRPARRAYFILTERINFO:
      {
        filterState = TRUE;
        break;
      }
      case COM_MASKED_NEW_DIFFERS_X_FILTERALGOOFSIGGRPARRAYFILTERINFO:
      {
        if(Com_IsArrayBasedFilterMaskUsedOfSigGrpArrayFilterInfo(idxFilterInfoApplTypSpecific) && Com_IsArrayBasedFilterValueXUsedOfSigGrpArrayFilterInfo(idxFilterInfoApplTypSpecific))
        {
          filterState = Com_SignalFilterHdlrHelper_SigGrpArr_MaskedNewDiffersX(newValuePtr, idxFilterInfoApplTypSpecific);
        }
        break;
      }
      case COM_MASKED_NEW_EQUALS_X_FILTERALGOOFSIGGRPARRAYFILTERINFO:
      {
        if(Com_IsArrayBasedFilterMaskUsedOfSigGrpArrayFilterInfo(idxFilterInfoApplTypSpecific) && Com_IsArrayBasedFilterValueXUsedOfSigGrpArrayFilterInfo(idxFilterInfoApplTypSpecific))
        {
          filterState = !Com_SignalFilterHdlrHelper_SigGrpArr_MaskedNewDiffersX(newValuePtr, idxFilterInfoApplTypSpecific);
        }
        break;
      }
      default: /* case COM_NEVER_FILTERALGOOFSIGGRPARRAYFILTERINFO */
        /* Intentionally Empty */
        break;
    }
  }

  return filterState;
}


#define COM_STOP_SEC_CODE
#include "Com_MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  MISRA JUSTIFICATIONS
 **********************************************************************************************************************/
/* *INDENT-OFF* */
/* Justifications for module-specific MISRA deviations:
*/

/***********************************************************************************************************************
 *  SILENTBSW JUSTIFICATIONS
 **********************************************************************************************************************/
/* SBSW_JUSTIFICATION_BEGIN

SBSW_JUSTIFICATION_END */

/***********************************************************************************************************************
 *  COVERAGE JUSTIFICATIONS
 **********************************************************************************************************************/
/* START_COVERAGE_JUSTIFICATION

END_COVERAGE_JUSTIFICATION */
/* *INDENT-ON* */

/**********************************************************************************************************************
 *  END OF FILE: Com_SignalFilterHdlrHelper.c
 *********************************************************************************************************************/
