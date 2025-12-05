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
/*!        \file  Com_DesGwBc.c
 *         \unit  DesGwBc
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#define COM_DESGWBC_SOURCE

/**********************************************************************************************************************
 * MISRA JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA  S 2983, 2984, 2985, 2986, 2987, 2991, 2992, 2993, 2994, 2995, 2996, 2997, 2998 EOF */ /* MD_MSR_ConstantCondition */
/* PRQA  S 883, 3406 EOF */ /* MD_Com_UnityBuild */

/**********************************************************************************************************************
 * INCLUDES
 **********************************************************************************************************************/
#include "Com.h"

#include "Com_DesGwBc.h"
#include "Com_Cfg_DescriptionGatewayData.h"
#include "Com_Cfg_ReceptionProcessingData.h"
#include "Com_Cfg_TransmissionProcessingData.h"

#include "Com_Transmission.h"
#include "Com_Reporting.h"

#include "vstdlib.h"

/**********************************************************************************************************************
 * LOCAL CONSTANT MACROS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * LOCAL FUNCTION MACROS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * LOCAL DATA PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"    /* PRQA S 5087 */        /* MD_MSR_MemMap */

/**********************************************************************************************************************
 * LOCAL FUNCTIONS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * GLOBAL FUNCTIONS
 **********************************************************************************************************************/

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON) /* COV_COM_GATEWAY_MAINFUNCTION_ROUTESIGNALSAPI */
 /**********************************************************************************************************************
  Com_DesGwBc_CopyBuffer
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 ********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_DesGwBc_CopyBuffer(P2CONST(uint8, AUTOMATIC, AUTOMATIC) sourceBuffer, Com_GwDescriptionAccessInfoIterType gwDescrAccessIdx)
{
  const Com_BitSizeOfGwDescriptionAccessInfoType bcNumberOfBits = Com_GetBitSizeOfGwDescriptionAccessInfo(gwDescrAccessIdx);

  /* Information regrading the TxBuffer of the current description gateway*/
  const Com_TxBufferInfoIterType txBufferInfoIdx = Com_GetTxBufferInfoIdxOfGwDescriptionAccessInfo(gwDescrAccessIdx); /*@ assert gwDescrAccessIdx < Com_GetSizeOfGwDescriptionAccessInfo(); */ /* VCA_COM_CSL03 */
  const Com_TxBufferLengthOfTxBufferInfoType bcDstByteLength = Com_Transmission_GetTxBufferLength(txBufferInfoIdx);
  CONSTP2VAR(Com_TxBufferType, AUTOMATIC, AUTOMATIC) bcDst = Com_Transmission_GetAddrTxBufferFirstElement(txBufferInfoIdx);

  /* In the gateway case, the index relative to the first byte is used, since the TxBuffers are prepared to point to the starting byte.
     Due to this behavior bcDst can be used to copy to tempStorage later on for a comparison */
  const Com_GwRelativeDestinationStartBitIndexOfGwDescriptionAccessInfoType bcDstBitOffsetSaw = Com_GetGwRelativeDestinationStartBitIndexOfGwDescriptionAccessInfo(gwDescrAccessIdx);

  /* In the gateway case, the source buffer starts on the minimum start position of the mapping in the PDU. This is enforced in the DesGwSp and DesGwCp.
     Due to this the source start bit index used for bitcopy needs to be corrected based on the minimum start byte position. */
  const Com_SourceStartBitIndexBasedonMinSourceStartBytePositionOfGwDescriptionAccessInfoType bcSrcBitOffsetSaw = Com_GetSourceStartBitIndexBasedonMinSourceStartBytePositionOfGwDescriptionAccessInfo(gwDescrAccessIdx);

  /* The temporary storage will be used to assess if the content has changed after the bitcopy operation
     Here, the total bytes are copied and are not directly comparable to the value copied in the description routing */
  uint8 tempStorage[COM_DESGWBCTEMPSTORAGESIZE] = {0};
  if(COM_DESGWBCTEMPSTORAGESIZE >= bcDstByteLength)
  {
    VStdLib_MemCpy_s(tempStorage, COM_DESGWBCTEMPSTORAGESIZE, bcDst, bcDstByteLength);
  }

  Std_ReturnType bitCpyRet = E_OK;
  if (Com_GetEndiannessOfGwDescriptionAccessInfo(gwDescrAccessIdx) == COM_BIG_ENDIAN_ENDIANNESSOFGWDESCRIPTIONACCESSINFO) /* big endian */
  {
    bitCpyRet = VStdLib_BitCpySawBigToBigEndian_s(bcDst, bcDstByteLength, bcDstBitOffsetSaw, sourceBuffer, bcSrcBitOffsetSaw, bcNumberOfBits, E_NOT_OK); /* VCA_COM_BITCPY_CSL_DEST */
  }
  else /* little endian */
  {
    bitCpyRet = VStdLib_BitCpySawLittleToLittleEndian_s(bcDst, bcDstByteLength, bcDstBitOffsetSaw, sourceBuffer, bcSrcBitOffsetSaw, bcNumberOfBits, E_NOT_OK); /* VCA_COM_BITCPY_CSL_DEST */
  }

  if(bitCpyRet == E_NOT_OK) /* copy operation failed */
  {
    Com_Reporting_CallDetReportError(COMServiceId_BitCopy, COM_E_BITCOPY); /* COV_COM_REPORT_ERROR */
  }

  boolean hasContentChanged = FALSE;
  if(COM_DESGWBCTEMPSTORAGESIZE >= bcDstByteLength)
  {
    hasContentChanged = (VStdLib_MemCmp(tempStorage, bcDst, bcDstByteLength) != 0);
  }

  return hasContentChanged;
}
#endif

#define COM_STOP_SEC_CODE
#include "Com_MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  MISRA JUSTIFICATIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  SILENTBSW JUSTIFICATIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  COVERAGE JUSTIFICATIONS
 **********************************************************************************************************************/
/* START_COVERAGE_JUSTIFICATION
    
END_COVERAGE_JUSTIFICATION */
/**********************************************************************************************************************
 *  END OF FILE: Com_DesGwBc.c
 *********************************************************************************************************************/
