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
/*!        \file  Com_CheckUpdateBit.h
 *        \unit   CheckUpdateBit
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_CHECKUPDATEBIT_H)
# define COM_CHECKUPDATEBIT_H

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
  Com_CheckUpdateBit_Signal
**********************************************************************************************************************/
/*! \brief        This function evaluates if the ComSignal was updated.
    \details      -
    \pre          -
    \param[in]    idxRxSigInfo       Index of RxSigInfo
    \param[in]    PduInfoPtr         contains the SDU buffer and the SDU length.
    \return       boolean
                    TRUE  if the ComSignal was updated
                    FALSE otherwise
    \reentrant    TRUE, for different handles
    \synchronous  TRUE
    \context      TASK|ISR2
    \spec
      requires idxRxSigInfo < Com_GetSizeOfRxSigInfo();
      requires PduInfoPtr != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_CheckUpdateBit_Signal(Com_RxSigInfoIterType idxRxSigInfo, CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr);

/**********************************************************************************************************************
  Com_CheckUpdateBit_SignalGroup
**********************************************************************************************************************/
/*! \brief        This function evaluates if the ComSignalGroup was updated.
    \details      -
    \pre          -
    \param[in]    PduInfoPtr        contains the SDU buffer and the SDU length.
    \param[in]    idxRxSigGrpInfo   Handle Id of ComSignalGroup
    \return       boolean
                    TRUE  if the ComSignalGroup was updated
                    FALSE otherwise
    \reentrant    TRUE, for different handles
    \synchronous  TRUE
    \context      TASK|ISR2
    \spec
      requires idxRxSigGrpInfo < Com_GetSizeOfRxSigGrpInfo();
      requires PduInfoPtr != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_CheckUpdateBit_SignalGroup(CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr, Com_RxSigGrpInfoIterType idxRxSigGrpInfo);

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
/**********************************************************************************************************************
  Com_CheckUpdateBit_GwDescription
**********************************************************************************************************************/
/*! \brief        This function evaluates if the gateway source description was updated.
    \details      -
    \pre          -
    \param[in]    gwDescrAccessIdx     index of GwDescriptionAccessInfo.
    \param[in]    PduInfoPtr           contains the SDU buffer and the SDU length.
    \return       boolean
                    TRUE  if the source description was updated
                    FALSE otherwise
    \reentrant    TRUE, for different handles
    \synchronous  TRUE
    \context      TASK|ISR2
    \spec
      requires gwDescrAccessIdx < Com_GetSizeOfGwDescriptionAccessInfo();
      requires PduInfoPtr != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_CheckUpdateBit_GwDescription(Com_GwDescriptionAccessInfoIterType gwDescrAccessIdx, CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr);
#endif

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_CHECKUPDATEBIT_H */
/**********************************************************************************************************************
  END OF FILE: Com_CheckUpdateBit.h
**********************************************************************************************************************/
