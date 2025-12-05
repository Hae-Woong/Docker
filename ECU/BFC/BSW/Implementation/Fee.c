/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH.                                                  All rights reserved.
 * 
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  Fee.c
 *    Component:  If_AsrIfFee
 *       Module:  Fee
 *    Generator:  -
 *
 *  Description:  The module Fee provides an abstraction from the device specific addressing scheme and 
 *                segmentation. It also offers a virtual addressing scheme and segmentation as well as 
 *                a virtually unlimited number of erase cycles to the upper layer.
 *  
 *********************************************************************************************************************/

/* macro identifying this compilation unit */
#define FEE_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Fee.h"
#include "Fee_InitEx.h"
#include "Fee_Int.h" /* PRQA S 3313 */ /*MD_FEE_18.1_MissingStructDefinition */
#include "Fee_Cbk.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* vendor specific version information is BCD coded */
#if (  (FEE_SW_MAJOR_VERSION != (10u)) \
    || (FEE_SW_MINOR_VERSION != (0u)) )
# error "Vendor specific version numbers of Fee.c and Fee.h are inconsistent"
#endif

/* versions of all included header files shall be checked */
#if (  (FEE_CFG_MAJOR_VERSION != (10u)) \
    || (FEE_CFG_MINOR_VERSION != (0u)) )
# error "Version numbers of Fee.c and Fee_Cfg.h are inconsistent!"
#endif

#if (  (FEE_PRIVATE_CFG_MAJOR_VERSION != (10u)) \
    || (FEE_PRIVATE_CFG_MINOR_VERSION != (0u)) )
# error "Version numbers of Fee.c and Fee_PrivateCfg.h are inconsistent!"
#endif



/**********************************************************************************************************************
 *  LOCAL TYPE DEFINITIONS
 *********************************************************************************************************************/
/* PRQA S 2995, 2991 uncoveredCode */ /* MD_FEE_2.2_Condition_False_In_Other_Configs, MD_FEE_14.3_Condition_False_in_Other_Configs */
typedef P2VAR(struct Fee_tagBlockCfgDatasetPair, AUTOMATIC, AUTOMATIC) Fee_BlockCfgDatasetPairRefType;

#if (STD_ON == FEE_DEV_ERROR_DETECT)

    typedef enum {
        FEE_MOD_UNINITIALIZED = 0,
        FEE_MOD_INITIALIZED
    } Fee_InitStatusType;

#  define FEE_START_SEC_VAR_INIT_UNSPECIFIED
#  include "MemMap.h"      /* PRQA S 5087 */ /* MD_MSR_19.1 */

    FEE_LOCAL VAR(Fee_InitStatusType, FEE_VAR) Fee_ModuleStatus_t = FEE_MOD_UNINITIALIZED;

#  define FEE_STOP_SEC_VAR_INIT_UNSPECIFIED
#  include "MemMap.h"      /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif /* (FEE_DEV_ERROR_DETECT) */


/**********************************************************************************************************************
 *  GLOBAL DATA VARIABLE
 *********************************************************************************************************************/
#define FEE_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"      /* PRQA S 5087 */ /* MD_MSR_19.1 */

VAR(Fee_GlobalVariableType, FEE_VAR_NOINIT) Fee_Globals_t;

#define FEE_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"      /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define FEE_START_SEC_CODE
#include "MemMap.h"      /* PRQA S 5087 */ /* MD_MSR_19.1 */



#if (FEE_DEV_ERROR_DETECT == STD_ON)
/**********************************************************************************************************************
 * Fee_DetReportError
 *********************************************************************************************************************/
/*! \brief      This function reports an error to the development error tracer, when reporting is enabled.
 *  \details    When reporting is disabled, nothing will be done.
 *  \param[in]  serviceId           The id of the service that found the error.
 *  \param[in]  errorCode           The id of the error that shall be reported.
 *  \config     Error detection is enabled via pre-compile switch
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
FEE_INLINE_FUNC(void) Fee_ReportDetError(uint8 serviceId, Fee_DetErrorCodeType errCode);
#endif

/**********************************************************************************************************************
 * Fee_ErrorCode2ReturnType
 *********************************************************************************************************************/
/*! \brief      This function 'converts' the given error code to std_return type
 *  \details    This function 'converts' the given error code to std_return type
 *  \param[in]  errorCode           The id of the error that shall be reported.
 *  \param[in]  serviceId           The id of the service that found the error.
 *  \return     E_OK                Error code was FEE_E_NO_ERROR
 *  \return     E_NOT_OK            Error code was not FEE_NO_ERROR
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
FEE_INLINE_FUNC(Std_ReturnType) Fee_ErrorCode2ReturnType(Fee_DetErrorCodeType errCode, uint8 serviceId);


/**********************************************************************************************************************
 *  Fee_ChkInit
 *********************************************************************************************************************/
/*! \brief      This function checks whether the module has been initialized when error detection is enabled.
 *  \details    This function checks whether the module has been initialized when error detection is enabled.
 *  \return     FEE_E_NO_ERROR                             If module has been initialized while API call.
 *  \return     FEE_E_NO_INIT                              If module has not been initialized while API call.
 *  \pre        -
 *  \context    TASK
 *  \note       -
 *********************************************************************************************************************/
FEE_INLINE_FUNC(Fee_DetErrorCodeType) Fee_ChkInit(void);


/**********************************************************************************************************************
 *  Fee_DetChkNotBusy
 *********************************************************************************************************************/
/*! \brief      This function checks whether the module is currently BUSY (a user job is already pending) when error
 *              detection is enabled
 *  \details    This function checks whether the module is currently BUSY (a user job is already pending) when error
 *              detection is enabled
 *  \return     FEE_E_NO_ERROR                             If module has been initialized while API call.
 *  \return     FEE_E_BUSY                                 If module has not been initialized while API call.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
FEE_INLINE_FUNC(Fee_DetErrorCodeType) Fee_ChkNotBusy(void);



/**********************************************************************************************************************
 * Fee_DetChkBlockCfg
 *********************************************************************************************************************/
/*! \brief      This function checks if the block config really exist and whether dataIndex is within valid range when
 *              error detection is enabled
 *  \details    This function checks if the block config really exist and whether dataIndex is within valid range when
 *              error detection is enabled
 *  \param[in]  block                   The "pointer to blockConfig" / DataIndex pair to be checked.
 *  \return     FEE_E_NO_ERROR            The BlockNumber check succeeded.
 *  \return     FEE_E_INVALID_BLOCK_NO  The BlockNumber check failed.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
FEE_INLINE_FUNC(Fee_DetErrorCodeType) Fee_DetChkBlockCfg(Fee_BlockCfgDatasetPair block);


/**********************************************************************************************************************
 * Fee_DetChkParamPointer
 *********************************************************************************************************************/
/*! \brief      This function checks a pointer parameter for being valid (NON-NULL) when error detection is enabled
 *  \details    This function checks a pointer parameter's value for being valid (NON-NULL) when error detection is enabled
 *  \param[in]  ptr                            The "pointer" to be checked.
 *  \return     FEE_E_NO_ERROR                 The parameter pointer check succeeded
 *  \return     FEE_E_PARAM_DATABUFFERPTR      The parameter pointer check failed
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
FEE_INLINE_FUNC (Fee_DetErrorCodeType) Fee_DetChkParamPointer(Fee_UserDataVoidConstPtrType ptr);



/**********************************************************************************************************************
 * Fee_DetChkParamsOffsetLength
 *********************************************************************************************************************/
/*! \brief      This function checks a (read request's) offset and length values for addressing a valid range when
 *              error detection is enabled
 *  \details    This function checks a (read request's) offset and length values for addressing a valid range when
 *              error detection is enabled
 *  \param[in]  pCfg                pointer to BlockConfiguration to be checked against (must be a valid pointer).
 *  \param[in]  ofs                 requested offset
 *  \param[in]  len                 requested length
 *  \return     FEE_E_NO_ERROR                    The parameter offset length check succeeded
 *  \return     FEE_E_PARAM_LENGTH_OFFSET        The parameter offset length check failed
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
FEE_INLINE_FUNC (Fee_DetErrorCodeType) Fee_DetChkParamsOffsetLength(Fee_BlockConfigRefType pCfg, uint16 ofs, uint16 len);


/**********************************************************************************************************************
* Fee_getBlockCfgFromBlockNumber
*********************************************************************************************************************/
/*! \brief      This function computes blockId out of blocknumber and returns it together with blocks configuration.
 *  \details    This function computes blockId out of blocknumber and returns it together with blocks configuration.
 *  \param[in]  BlockNumber_u16     Contains the block number and the selected dataset.
 *  \return     Pair block ID and block configuration.
 *  \pre        Module is initialized
 *  \context    TASK
 *  \reentrant  FALSE
*********************************************************************************************************************/
FEE_INLINE_FUNC(Fee_BlockCfgDatasetPair) Fee_getBlockCfgFromBlockNumber(uint16 blockNumber);



/**********************************************************************************************************************
* Fee_getBlockCfgFromBlockNumber_Checked
*********************************************************************************************************************/
/*! \brief     This function performs common checks in order to safely compute blockId out of blocknumber and store
 *             it together with blocks configuration in given out parameter.
 *  \details   This function performs common checks in order to safely compute blockId out of blocknumber and store
 *             it together with blocks configuration in given out parameter.
 *  \param[in] BlockNumber_u16     Contains the block number and the selected dataset.
 *  \param[in,out]pBlock              Parameter to store the block ID
 *  \return    error Code
 *  \pre       Module is initialized
 *  \context   TASK
 *  \reentrant FALSE
*********************************************************************************************************************/
FEE_INLINE_FUNC(Fee_DetErrorCodeType) Fee_getBlockCfgFromBlockNumber_Checked(Fee_BlockCfgDatasetPairRefType pBlock,
                                                                                      uint16 blockNumber);



/**********************************************************************************************************************
* Fee_InternalEnableFss
*********************************************************************************************************************/
/*! \brief      This function provides the central way to enable Fss.
 *  \details    This function provides the central way to enable Fss.
 *  \param[in]  doEnable
 *  \pre        Module is initialized
 *  \context    TASK
 *  \reentrant  FALSE
*********************************************************************************************************************/
FEE_INLINE_FUNC(void) Fee_InternalControlFssEnable(Fee_Bool doEnable);




/**********************************************************************************************************************
* Fee_InternalInitWriteRequest
*********************************************************************************************************************/
/*! \brief     This function initializes a write request by getting the configuration of the needed block.
*    \details   It additionally checks block configuration and sets write job specific parameter
*  \param[in]      job                 Type of write job, that shall be requested.
*  \param[in]  blockNumber           Number of the block that shall be written
*  \param[in]  dataBuf               Pointer to the data that shall be written
*  \return     errorCode
*  \pre        Module is initialized
*  \context    TASK
*  \reentrant  FALSE
*********************************************************************************************************************/
FEE_INLINE_FUNC(Fee_DetErrorCodeType) Fee_InternalInitWriteRequest(enum Fee_tagReqestedJob job, uint16 blockNumber, Fee_UserDataConstPtrType dataBuf);



/**********************************************************************************************************************
* Fee_InternalInitRequest
*********************************************************************************************************************/
/*! \brief      This function performs a busy check and initializes job request.
 *  \details    This function performs a busy check and initializes job request.
 *  \param[in]  job                 Type of job, that shall be requested.
 *  \param[in]  partitionIndex       Index of the partition
 *  \param[in]  specParams           Parameters of the job that shall be requested
 *  \return     errorCode
 *  \pre        Module is initialized
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
FEE_LOCAL_FUNC(Fee_DetErrorCodeType) Fee_InternalInitRequest(enum Fee_tagReqestedJob job, sint8 partitionIndex, union Fee_tagJobSpecificParams specParams);



#if(FEE_GET_ERASE_CYCLE_API == STD_ON)
/**********************************************************************************************************************
 * Fee_DetChkInitThenParamSectorNumber
 *********************************************************************************************************************/
/*! \brief      This function checks a requested sector number for being valid.
 *  \details    Checks also for initialization to ensure that config is accessible
 *  \param[in,out]  secId               requested pair<secId,partition>
 *  \return     FEE_E_NO_ERROR                The BlockNumber check succeeded.
 *  \return     FEE_E_PARAM_SECTOR_NUMBER     The BlockNumber check failed.
 *  \config     FEE_GET_ERASE_CYCLE_API is enabled via pre-compile switch
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
FEE_INLINE_FUNC (Fee_DetErrorCodeType) Fee_DetChkInitThenParamSectorNumber(struct Fee_tagSectorIdentification secId);





/**********************************************************************************************************************
* Fee_GetSectorNumber
*********************************************************************************************************************/
/*! \brief     This function extracts partition index and sector index out of the given parameter.
 *  \details   This function extracts partition index and sector index out of the given parameter.
 *  \param[in] parmSecId           Identifies partition and logical sector.
 *  \return    struct that holds partition index and sector index.
 *  \config    FEE_GET_ERASE_CYCLE_API is enabled via pre-compile switch
 *  \pre       -
 *  \context   TASK
 *  \reentrant FALSE
*********************************************************************************************************************/
FEE_INLINE_FUNC (struct Fee_tagSectorIdentification) Fee_GetSectorNumber(uint8 parmSecId);
#endif



#if((FEE_DATA_CONVERSION_API == STD_ON) || (FEE_FORCE_SECTOR_SWITCH_API == STD_ON))
/**********************************************************************************************************************
* Fee_InternalForceSectorSwitch
*********************************************************************************************************************/
/*! \brief     This function initializes a force sector switch request.
 *  \details   This function initializes a force sector switch request.
 *  \param[in] forceFblMode        Specifies if sector switch must be executed in FBL mode or not
 *  \config    FEE_DATA_CONVERSION_API or FEE_FORCE_SECTOR_SWITCH_API is enabled via pre-compile switch
 *  \pre       -
 *  \context   TASK
 *  \reentrant FALSE
*********************************************************************************************************************/
FEE_LOCAL_FUNC(void) Fee_InternalForceSectorSwitch(Fee_Bool forceFblMode);
#endif


#if (FEE_LOOKUPTABLE_MODE == STD_ON)
/**********************************************************************************************************************
 * Fee_InternalSyncLookUpTable
 *********************************************************************************************************************/
/*!
 * \brief      Sets flag to indicate that a SyncLookUp table job was requested
 * \details    Sets flag to indicate that a SyncLookUp table job was requested
 * \config     FEE_LOOKUPTABLE_MODE is enabled via pre-compile switch
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_InternalSyncLookUpTable(void);
#endif

/******************************************************************************
 *  INTERNAL FUNCTIONS
 *****************************************************************************/


#if (FEE_DEV_ERROR_DETECT == STD_ON)
/**********************************************************************************************************************
 *  Fee_ReportDetError
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */

FEE_INLINE_FUNC(void) Fee_ReportDetError(uint8 serviceId, Fee_DetErrorCodeType errCode) /* PRQA S 3206*/ /* MD_FEE_2.7_Parameter_Used_For_Other_Configs */ /* COV_FEE_ERR_DETECT_OFF_REPORTING */
{
#if(FEE_DEBUG_REPORTING == STD_ON)
        (void)Fee_Errorhook((uint8)serviceId, (uint8)errCode);
#else
        FEE_DUMMY_STATEMENT(serviceId);
        FEE_DUMMY_STATEMENT(errCode);
#endif
}
#endif


/**********************************************************************************************************************
 *  Fee_ErrorCode2ReturnType
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FEE_INLINE_FUNC(Std_ReturnType) Fee_ErrorCode2ReturnType(Fee_DetErrorCodeType errCode, uint8 serviceId) /* PRQA S 3206*/ /* MD_FEE_2.7_Parameter_Used_For_Other_Configs */
{
    Std_ReturnType retVal = E_NOT_OK;
    if(errCode == FEE_E_NO_ERROR) /* COV_FEE_ERR_DETECT_OFF */
    {
        retVal = E_OK;
    }
#if (FEE_DEV_ERROR_DETECT == STD_ON)
    else
    {
        Fee_ReportDetError(serviceId, errCode);
    }
#else
    FEE_DUMMY_STATEMENT(serviceId);
    FEE_DUMMY_STATEMENT(errCode);
#endif
    return retVal;
}



/**********************************************************************************************************************
 *  Fee_ChkInit
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FEE_INLINE_FUNC(Fee_DetErrorCodeType) Fee_ChkInit(void)
{
#if (FEE_DEV_ERROR_DETECT == STD_ON)
    return (Fee_ModuleStatus_t == FEE_MOD_INITIALIZED) ? FEE_E_NO_ERROR : FEE_E_NO_INIT;
#else
    return FEE_E_NO_ERROR;
#endif
}

/**********************************************************************************************************************
 *  Fee_ChkNotBusy
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FEE_INLINE_FUNC(Fee_DetErrorCodeType) Fee_ChkNotBusy(void)
{
#if (FEE_DEV_ERROR_DETECT == STD_ON)
    return (Fee_Globals_t.FeeModuleStatus_t != MEMIF_BUSY) ? FEE_E_NO_ERROR : FEE_E_BUSY;
#else
    return FEE_E_NO_ERROR;
#endif
}

/**********************************************************************************************************************
 * Fee_DetChkBlockCfg
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FEE_INLINE_FUNC(Fee_DetErrorCodeType) Fee_DetChkBlockCfg(Fee_BlockCfgDatasetPair block) /* PRQA S 3206 */ /* MD_FEE_2.7_Parameter_Used_For_Other_Configs*/
{
#if (FEE_DEV_ERROR_DETECT == STD_ON)
    return ((block.config == NULL_PTR) || (block.dataIndex >= block.config->NumberOfDatasets_u8))
        ? FEE_E_INVALID_BLOCK_NO : FEE_E_NO_ERROR;
#else
    FEE_DUMMY_STATEMENT(block);
    return FEE_E_NO_ERROR;
#endif
}
  
/**********************************************************************************************************************
 * Fee_DetChkParamPointer
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FEE_INLINE_FUNC(Fee_DetErrorCodeType) Fee_DetChkParamPointer(Fee_UserDataVoidConstPtrType ptr) /* PRQA S 3206 */ /* MD_FEE_2.7_Parameter_Used_For_Other_Configs*/
{
#if (FEE_DEV_ERROR_DETECT == STD_ON)
    return (ptr == NULL_PTR) ? FEE_E_PARAM_DATABUFFERPTR : FEE_E_NO_ERROR;
#else
    FEE_DUMMY_STATEMENT(ptr);
    return FEE_E_NO_ERROR;
#endif
}



/**********************************************************************************************************************
 * Fee_DetChkParamsOffsetLength
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FEE_INLINE_FUNC(Fee_DetErrorCodeType) Fee_DetChkParamsOffsetLength(Fee_BlockConfigRefType pCfg, uint16 ofs, uint16 len) /* PRQA S 3206 */ /* MD_FEE_2.7_Parameter_Used_For_Other_Configs*/
{
#if (FEE_DEV_ERROR_DETECT == STD_ON)
    Fee_DetErrorCodeType retVal = FEE_E_PARAM_LENGTH_OFFSET;

    if((len != 0u) && (((uint32)ofs + len) <= (uint32)pCfg->BlockPayloadSize_u16))
    {
        retVal = FEE_E_NO_ERROR;
    }
    return retVal;
#else
    FEE_DUMMY_STATEMENT(pCfg);
    FEE_DUMMY_STATEMENT(ofs);
    FEE_DUMMY_STATEMENT(len);
    return FEE_E_NO_ERROR;
#endif
}



 /**********************************************************************************************************************
 * Fee_getBlockCfgFromBlockNumber
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FEE_INLINE_FUNC(Fee_BlockCfgDatasetPair) Fee_getBlockCfgFromBlockNumber(uint16 blockNumber)
{
    /*#10 Prepare bitmask to extract index out of block number*/
    const uint16 DatasetSelectionBitMask = (((uint16)1u << Fee_Globals_t.Config_pt->Fee_DatasetSelectionBits_u8) - 1u);

    /*#20 Get index within config array out of block number*/
    const uint32 idx = (((uint32)blockNumber >> Fee_Globals_t.Config_pt->Fee_DatasetSelectionBits_u8) - 1u);
    struct Fee_tagBlockCfgDatasetPair retVal;
    
    /*#30 Get blocks config  when index is in range, otherwise null pointer */
    if((idx < Fee_Globals_t.Config_pt->Fee_NumberOfBlocks_u16)) /* COV_FEE_ERR_DETECT_OFF */
    {
        retVal.config = &Fee_Globals_t.Config_pt->BlockConfig_at[idx];
    }
    else
    {
        retVal.config = NULL_PTR;
    }
    
    /*#40 Get dataIndex out of blockNumber*/
    retVal.dataIndex = (uint8)(blockNumber & DatasetSelectionBitMask);

    return retVal;
}

/**********************************************************************************************************************
* Fee_getBlockCfgFromBlockNumber_Checked
*********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FEE_INLINE_FUNC(Fee_DetErrorCodeType) Fee_getBlockCfgFromBlockNumber_Checked(
            Fee_BlockCfgDatasetPairRefType pBlock, uint16 blockNumber)
{
    /*#10 Perform initialization check*/
    Fee_DetErrorCodeType retVal = Fee_ChkInit();
    if(retVal == FEE_E_NO_ERROR) /* COV_FEE_ERR_DETECT_OFF */
    {
        /*#20 Get block config*/
        *pBlock = Fee_getBlockCfgFromBlockNumber(blockNumber); /* SBSW_FEE_PointerWrite_refParam */

        /*#30 Check block config*/
        retVal = Fee_DetChkBlockCfg(*pBlock); /* SBSW_FEE_getBlockCfgFromBlockNumber */
    }

    return retVal;
}


/**********************************************************************************************************************
* Fee_InternalEnableFss
*********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FEE_INLINE_FUNC(void) Fee_InternalControlFssEnable(Fee_Bool doEnable)
{
    if((!Fee_Globals_t.ApiFlags.enableFss) == doEnable) /* PRQA S 1881, 4558, 4116 */ /*MD_FEE_10.4_compare_essentially_boolean_to_FeeBool, MD_FEE_10.1_Essentially_boolean_in_logical_expressions, MD_FEE_10.1_Essentially_boolean_in_logical_expressions*/
    {
        Fee_GlobalSuspend();
        Fee_Globals_t.ApiFlags.enableFss = (doEnable == FEE_TRUE); /* PRQA S 4404, 1881 */ /* MD_FEE_10.3_essentially_boolean_type, MD_FEE_10.4_compare_FeeBool_to_enum */
        Fee_GlobalRestore();
    }
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Fee_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
 FUNC(void, FEE_API_CODE) Fee_Init(void)
 {
    /*#10 Call Fee_InitEx with current configuration as parameter*/
     Fee_InitEx(&Fee_Config); /* SBSW_FEE_Func_passParamByReference */
 }

/**********************************************************************************************************************
 * Fee_InitEx
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
 /* PRQA S 1505 2 */ /* MD_MSR_8.10 */
FUNC(void, FEE_API_CODE) Fee_InitEx(Fee_ConfigRefType ConfigPtr)
{
    Fee_DetErrorCodeType errCode = Fee_DetChkParamPointer(ConfigPtr); /* PRQA S 0315 */ /* MD_FEE_1.1_Cast_pointer_to_void */ /* SBSW_FEE_GlobalConfigPointer */

    if (errCode == FEE_E_NO_ERROR) /* COV_FEE_ERR_DETECT_OFF */
    {
        uint8 partIndex;

        for(partIndex = 0; (partIndex < ConfigPtr->Fee_NumberOfPartitions_u8); partIndex++)
        {
            Fee_Partition_init(ConfigPtr, partIndex); /* SBSW_FEE_ConstPointer_refParam */
        }

        Fee_Globals_t.Config_pt = ConfigPtr;

        Fee_Globals_t.JobParams.requestedJob = FEE_JOB_NONE; 
        Fee_Globals_t.JobParams.partitionId = 0; /* init it, although it should not be used without having a job */

        Fee_Globals_t.FlsJobResult_t = MEMIF_JOB_OK;
        Fee_Globals_t.FeeModuleStatus_t = MEMIF_BUSY_INTERNAL;
        Fee_Globals_t.FeeJobResult_t = MEMIF_JOB_OK;
        Fee_Globals_t.FeeLocalJobResult_t = FEE_LOCAL_JOB_RESULT_OK; 
        Fee_Globals_t.InternalFlags_t.brokenLink = 0u;
        Fee_Globals_t.InternalFlags_t.forceSectorSwitch = 0;
        Fee_Globals_t.InternalFlags_t.leaveMain = 0u;
        Fee_Globals_t.InternalFlags_t.jobDeferred = 0;

        Fee_Globals_t.ApiFlags.cancelReq = 0u;
        Fee_Globals_t.ApiFlags.enableBss = 0u;
        Fee_Globals_t.ApiFlags.enableFss = 0u;
        Fee_Globals_t.ApiFlags.forceFblMode = 0u;
        Fee_Globals_t.ApiFlags.forceSectorSwitch = 0u;

        Fee_Globals_t.ApiFlags.needProcessing = 1u;
        Fee_Globals_t.Fee_InternalErrorFlags = 0u;


        /* we start without sector switch, i.e. partition index is out of bounds     *
         * There's no need to initialize remaining SectorSwtichData_t members:       *
         * They will be set if a sector switch actually becomes necessary.           *
         * This, in turn, can only happen when a job completes                       */
        Fee_Globals_t.SectorSwitchData_t.partitionId_s8 = -1;

        Fee_Globals_t.WritesSuspended_bl = FALSE;

        Fee_FsmReset(&Fee_Globals_t); /* SBSW_FEE_Func_passParamByReference */

#if (FEE_DEV_ERROR_DETECT == STD_ON)
        Fee_ModuleStatus_t = FEE_MOD_INITIALIZED;
#endif
    }

    (void)Fee_ErrorCode2ReturnType(errCode, FEE_SID_INIT);
} /* End of Fee_InitEx() */

/**********************************************************************************************************************
 * Fee_SetMode
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, FEE_API_CODE) Fee_SetMode(MemIf_ModeType Mode)
{
    Fee_DetErrorCodeType errCode = Fee_ChkInit();

    if (errCode == FEE_E_NO_ERROR) /* COV_FEE_ERR_DETECT_OFF */
    {
        Fee_GlobalSuspend();

        /* A Sectorswitch will be done only, if it is necessary in order to process a pending job */
        Fee_Globals_t.ApiFlags.enableBss = (Mode == MEMIF_MODE_SLOW) ? TRUE : FALSE;

        if(Fee_Globals_t.FeeModuleStatus_t == MEMIF_IDLE)
        {

            Fee_Globals_t.FeeModuleStatus_t = MEMIF_BUSY_INTERNAL;
        }

        Fee_GlobalRestore();

        Fee_SignalNeedProcessing();
    }

    (void)Fee_ErrorCode2ReturnType(errCode, FEE_SID_SETMODE);
} /* End of Fee_SetMode() */



/**********************************************************************************************************************
 * Fee_Read
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, FEE_API_CODE) Fee_Read(uint16 BlockNumber, uint16 BlockOffset,
                                            Fee_UserDataPtrType DataBufferPtr, uint16 Length)
{
    Fee_DetErrorCodeType errCode = Fee_DetChkParamPointer(DataBufferPtr); /* PRQA S 0315 */ /* MD_FEE_1.1_Cast_pointer_to_void */ /* SBSW_FEE_UserDataBuffer */
    if(errCode == FEE_E_NO_ERROR) /* COV_FEE_ERR_DETECT_OFF */
    {
        Fee_BlockCfgDatasetPair block;
        errCode = Fee_getBlockCfgFromBlockNumber_Checked(&block, BlockNumber); /* SBSW_FEE_PointerWrite_refParam */
        if(errCode == FEE_E_NO_ERROR) /* COV_FEE_ERR_DETECT_OFF */
        {
            errCode = Fee_DetChkParamsOffsetLength(block.config, BlockOffset, Length); /* SBSW_FEE_getBlockCfgFromBlockNumber */
            if(errCode == FEE_E_NO_ERROR) /* COV_FEE_ERR_DETECT_OFF */
            {
                union Fee_tagJobSpecificParams p; /* PRQA S 0759 */ /* MD_MSR_Union */
                p.blockJob.add.read.readBuffer = DataBufferPtr;
                p.blockJob.add.read.offset = BlockOffset;
                p.blockJob.add.read.length = Length;
                p.blockJob.blockRef = block;
                errCode = Fee_InternalInitRequest(FEE_JOB_READ, (sint8)(block.config->PartitionIndex), p); /* SBSW_FEE_getBlockCfgFromBlockNumber */
            }
        }
    }
    return Fee_ErrorCode2ReturnType(errCode, FEE_SID_READ);
} /* End of Fee_Read() */


/**********************************************************************************************************************
 * Fee_Write
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 1503,3673 1 */ /* MD_MSR_14.1, MD_FEE_8.13_API */
FUNC(Std_ReturnType, FEE_API_CODE) Fee_Write(uint16 BlockNumber, Fee_UserDataPtrType DataBufferPtr)
{
    Fee_DetErrorCodeType errCode = Fee_DetChkParamPointer(DataBufferPtr); /* PRQA S 0315 */ /* MD_FEE_1.1_Cast_pointer_to_void */ /* SBSW_FEE_ConstUserDataBuffer */
    if(errCode == FEE_E_NO_ERROR) /* COV_FEE_ERR_DETECT_OFF */
    {
            errCode = Fee_InternalInitWriteRequest(FEE_JOB_WRITE, BlockNumber, DataBufferPtr); /* SBSW_FEE_ConstUserDataBuffer */
    }
    return Fee_ErrorCode2ReturnType(errCode, FEE_SID_WRITE);
} /* End of Fee_Write() */



/**********************************************************************************************************************
 * Fee_InvalidateBlock
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, FEE_API_CODE) Fee_InvalidateBlock(uint16 BlockNumber)
{
    return Fee_ErrorCode2ReturnType(
                Fee_InternalInitWriteRequest(FEE_JOB_INVALIDATE_BLOCK, BlockNumber, NULL_PTR), FEE_SID_INVALIDATEBLOCK); /* SBSW_FEE_InternalInitWriteRequest */
} /* End of Fee_InvalidateBlock() */


/**********************************************************************************************************************
 * Fee_EraseImmediateBlock
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, FEE_API_CODE) Fee_EraseImmediateBlock(uint16 BlockNumber)
{
    /**** EraseImmediateBlock is exactly the same as Fee_InvalidateBlock, except the "immediate block check"!
     * However, this check is not implemented, as it doesn't affect functionality.
     * Both functions could be combined, but there still are different API Ids.
     *  */

    return Fee_ErrorCode2ReturnType(
            Fee_InternalInitWriteRequest(FEE_JOB_ERASE_IMMEDIATE_BLOCK, BlockNumber, NULL_PTR), FEE_SID_ERASEIMMEDIATEBLOCK); /* SBSW_FEE_InternalInitWriteRequest */
} /* End of Fee_EraseImmediateBlock() */



/**********************************************************************************************************************
 * Fee_Cancel
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, FEE_API_CODE) Fee_Cancel(void)
{
    Fee_DetErrorCodeType errCode = Fee_ChkInit();
    if (errCode == FEE_E_NO_ERROR) /* COV_FEE_ERR_DETECT_OFF */
    {
        Fee_GlobalSuspend();

        if(Fee_Globals_t.FeeModuleStatus_t == MEMIF_BUSY)
        {
            Fee_Globals_t.FeeModuleStatus_t = MEMIF_IDLE;
            Fee_Globals_t.FeeJobResult_t = MEMIF_JOB_CANCELLED;
        }

        Fee_Globals_t.JobParams.requestedJob = FEE_JOB_NONE; 
        Fee_Globals_t.ApiFlags.cancelReq = TRUE;

        Fee_GlobalRestore();

        Fee_SignalNeedProcessing();
    }

    (void)Fee_ErrorCode2ReturnType(errCode, FEE_SID_CANCEL);
} /* End of Fee_Cancel() */


/**********************************************************************************************************************
 * Fee_GetStatus
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(MemIf_StatusType, FEE_API_CODE) Fee_GetStatus(void)
{
    return (Fee_Globals_t.FeeModuleStatus_t);
} /* End of Fee_GetStatus() */


/**********************************************************************************************************************
 * Fee_GetJobResult
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(MemIf_JobResultType, FEE_API_CODE) Fee_GetJobResult(void)
{
    Fee_DetErrorCodeType errCode = Fee_ChkInit();
    MemIf_JobResultType retVal = MEMIF_JOB_FAILED; /* PRQA S 2981 */ /* MD_MSR_RetVal  */

    if(errCode == FEE_E_NO_ERROR) /* COV_FEE_ERR_DETECT_OFF */
    {
        retVal = Fee_Globals_t.FeeJobResult_t;
    }
#if (FEE_DEV_ERROR_DETECT == STD_ON)
    else
    {
        Fee_ReportDetError(FEE_SID_GETJOBRESULT, errCode);
    }
#endif

    return retVal;
} /* End of Fee_GetJobResulst() */


/**********************************************************************************************************************
 * Fee_GetVersionInfo
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
#if (FEE_VERSION_INFO_API == STD_ON)
FUNC(void, FEE_API_CODE) Fee_GetVersionInfo(Fee_StdVersionInfoRefType versioninfo)
{
    Fee_DetErrorCodeType errCode = Fee_DetChkParamPointer(versioninfo); /* PRQA S 0315 */ /* MD_FEE_1.1_Cast_pointer_to_void */ /* SBSW_FEE_UserDataBuffer */
    if(errCode == FEE_E_NO_ERROR) /* COV_FEE_ERR_DETECT_OFF */
    {
        versioninfo->vendorID         = FEE_VENDOR_ID; /* SBSW_FEE_PointerWrite_refParam */
        versioninfo->moduleID         = FEE_MODULE_ID; /* SBSW_FEE_PointerWrite_refParam */
        versioninfo->sw_major_version = FEE_SW_MAJOR_VERSION; /* SBSW_FEE_PointerWrite_refParam */
        versioninfo->sw_minor_version = FEE_SW_MINOR_VERSION; /* SBSW_FEE_PointerWrite_refParam */
        versioninfo->sw_patch_version = FEE_SW_PATCH_VERSION; /* SBSW_FEE_PointerWrite_refParam */
    }
    (void)Fee_ErrorCode2ReturnType(errCode, FEE_SID_GETVERSIONINFO);
} /* End of Fee_GetVersionInfo() */
#endif /* (STD_ON == FEE_VERSION_INFO_API) */


/**********************************************************************************************************************
 * Fee_MainFunction
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, FEE_API_CODE) Fee_MainFunction(void)
{

    if(Fee_Globals_t.ApiFlags.needProcessing) /* PRQA S 3344 1 */ /* MD_FEE_13.2_boolean */
    {
        if (Fee_ChkInit() == FEE_E_NO_ERROR) /* COV_FEE_ERR_DETECT_OFF */
        {
            Fee_GlobalSuspend();
            Fee_Globals_t.ApiFlags.needProcessing = 0;
            Fee_GlobalRestore();

            Fee_DoProcessing(&Fee_Globals_t); /* SBSW_FEE_Func_passParamByReference */
        }
    }
}


/**********************************************************************************************************************
 * Fee_JobEndNotification
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
#if (FEE_POLLING_MODE == STD_OFF)
FUNC(void, FEE_API_CODE) Fee_JobEndNotification(void)  /* PRQA S 1505 */ /* MD_MSR_8.10 */
{
     /*#10 Set processing flag*/
    Fee_SignalNeedProcessing();
}



/**********************************************************************************************************************
 * Fee_JobErrorNotification
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, FEE_API_CODE) Fee_JobErrorNotification(void)
{
    Fee_JobEndNotification();
}
#endif /* (FEE_POLLING_MODE) */



#if (FEE_GET_ERASE_CYCLE_API == STD_ON)
/**********************************************************************************************************************
 * Fee_GetEraseCycle
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, FEE_API_CODE) Fee_GetEraseCycle(uint8 SectorNumber, Fee_UserData32PtrType DataPtr)
{
    Fee_DetErrorCodeType errCode = Fee_DetChkParamPointer(DataPtr); /* PRQA S 0315 */ /* MD_FEE_1.1_Cast_pointer_to_void */ /* SBSW_FEE_UserDataBuffer */

    if(errCode == FEE_E_NO_ERROR) /* COV_FEE_ERR_DETECT_OFF */
    {
        struct Fee_tagSectorIdentification sector;
        sector = Fee_GetSectorNumber(SectorNumber); /* must be a separate statement due to ISO C90 constraints */

        errCode = Fee_DetChkInitThenParamSectorNumber(sector);
        if(errCode == FEE_E_NO_ERROR) /* COV_FEE_ERR_DETECT_OFF */
        {
            union Fee_tagJobSpecificParams p; /* PRQA S 0759 */ /* MD_MSR_Union */

            p.eraseCc.resultBuffer = DataPtr;
            p.eraseCc.sectorIdent = sector;
            errCode = Fee_InternalInitRequest(FEE_JOB_GET_ERASE_CYCLE, (sint8)sector.partitionIdx, p); /* SBSW_FEE_UserDataBuffer */
        }
    }

    return Fee_ErrorCode2ReturnType(errCode, FEE_SID_GET_ERASE_CYCLE);
}

/**********************************************************************************************************************
 * Fee_DetChkInitThenParamSectorNumber
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FEE_INLINE_FUNC(Fee_DetErrorCodeType) Fee_DetChkInitThenParamSectorNumber(struct Fee_tagSectorIdentification secId) /* PRQA S 3206 */ /* MD_FEE_2.7_Parameter_Used_For_Other_Configs*/
{
#if (FEE_DEV_ERROR_DETECT == STD_ON)
{
    Fee_DetErrorCodeType retVal = Fee_ChkInit();

    if((retVal == FEE_E_NO_ERROR) /* COV_FEE_ERR_DETECT_OFF */
        && ((secId.partitionIdx) >= Fee_Globals_t.Config_pt->Fee_NumberOfPartitions_u8))
    {
        retVal = FEE_E_PARAM_SECTOR_NUMBER;
    }
    return retVal;
}
#else
    FEE_DUMMY_STATEMENT(secId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
    return FEE_E_NO_ERROR;
#endif
}

/**********************************************************************************************************************
 * Fee_GetSectorNumber
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FEE_INLINE_FUNC(struct Fee_tagSectorIdentification) Fee_GetSectorNumber(uint8 parmSecId)
{
    struct Fee_tagSectorIdentification retVal;
    retVal.sectorIdx = parmSecId & 1u;
    retVal.partitionIdx = (parmSecId >> 1);
    return retVal;
}
#endif /* (STD_ON == FEE_GET_ERASE_CYCLE_API) */

#if (FEE_GET_WRITE_CYCLE_API == STD_ON)
/**********************************************************************************************************************
 * Fee_GetWriteCycle
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, FEE_API_CODE) Fee_GetWriteCycle(uint16 BlockNumber, Fee_UserData32PtrType DataPtr)
{
    Fee_DetErrorCodeType errCode = Fee_DetChkParamPointer(DataPtr); /* PRQA S 0315 */ /* MD_FEE_1.1_Cast_pointer_to_void */ /* SBSW_FEE_UserDataBuffer */
    if(errCode == FEE_E_NO_ERROR) /* COV_FEE_ERR_DETECT_OFF */
    {
        struct Fee_tagBlockCfgDatasetPair block;

        errCode = Fee_getBlockCfgFromBlockNumber_Checked(&block, BlockNumber); /* SBSW_FEE_Func_passParamByReference */
        if(errCode == FEE_E_NO_ERROR) /* COV_FEE_ERR_DETECT_OFF */
        {
            union Fee_tagJobSpecificParams p; /* PRQA S 0759 */ /* MD_MSR_Union */

            p.blockJob.add.writeCc.resultBuffer = DataPtr;
            p.blockJob.blockRef = block;

            errCode = Fee_InternalInitRequest(FEE_JOB_GET_WRITE_CYCLE, (sint8)(block.config->PartitionIndex), p); /* SBSW_FEE_UserDataBuffer */ /* SBSW_FEE_getBlockCfgFromBlockNumber */
        }
    }

    return Fee_ErrorCode2ReturnType(errCode, FEE_SID_GET_WRITE_CYCLE);
}
#endif /* (FEE_GET_WRITE_CYCLE_API == STD_ON) */


/**********************************************************************************************************************
 * Fee_GetSectorSwitchStatus
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(Fee_SectorSwitchStatusType, FEE_API_CODE) Fee_GetSectorSwitchStatus(void)
{
    Fee_DetErrorCodeType errCode = Fee_ChkInit();
    Fee_SectorSwitchStatusType retVal = FEE_SECTOR_SWITCH_UNINIT; /* PRQA S 2981 */ /* MD_MSR_RetVal  */
    
    if (errCode == FEE_E_NO_ERROR) /* COV_FEE_ERR_DETECT_OFF */
    {
        retVal = FEE_SECTOR_SWITCH_IDLE;
        Fee_GlobalSuspend();

        if(Fee_FsmIsSectorSwitchFsmRunning(&Fee_Globals_t)) /* PRQA S 3344 1 */ /* MD_FEE_13.2_boolean */ /* SBSW_FEE_Func_passParamByReference */
        {
            retVal = (Fee_Partition_getFillLevel(
                                Fee_Globals_t.CurrentProcessData.Partition_pt) == FEE_PART_FILL_LEVEL_SINGLE_SECTOR) ?
                                                                    FEE_SECTOR_SWITCH_ERASE : FEE_SECTOR_SWITCH_BLOCK_COPY; /* SBSW_FEE_InitializedProcessData */
        }

        Fee_GlobalRestore();
    }
#if (FEE_DEV_ERROR_DETECT == STD_ON)
    else
    {
        Fee_ReportDetError(FEE_SID_GET_SECTOR_SWITCH_STATUS, errCode);
    }
#endif
    return retVal;
}



/**********************************************************************************************************************
 * Fee_SuspendWrites
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, FEE_API_CODE) Fee_SuspendWrites(void)
{
    Fee_DetErrorCodeType errCode = Fee_ChkInit();
    if (errCode == FEE_E_NO_ERROR) /* COV_FEE_ERR_DETECT_OFF */
    {
        Fee_GlobalSuspend();
        Fee_Globals_t.WritesSuspended_bl = TRUE;
        Fee_GlobalRestore();
    }
#if (FEE_DEV_ERROR_DETECT == STD_ON)
    else
    {
        Fee_ReportDetError(FEE_SID_SUSPEND_WRITE, errCode);
    }
#endif
}


/**********************************************************************************************************************
 * Fee_ResumeWrites
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(void, FEE_API_CODE) Fee_ResumeWrites(void)
{
    Fee_DetErrorCodeType errCode = Fee_ChkInit();
    if (errCode == FEE_E_NO_ERROR) /* COV_FEE_ERR_DETECT_OFF */
    {
        Fee_GlobalSuspend();
        Fee_Globals_t.WritesSuspended_bl = FALSE;
        Fee_GlobalRestore();
        Fee_SignalNeedProcessing();
    }
#if (FEE_DEV_ERROR_DETECT == STD_ON)
    else
    {
        Fee_ReportDetError(FEE_SID_RESUME_WRITE, errCode);
    }
#endif
}



#if (FEE_FSS_CONTROL_API == STD_ON)
/**********************************************************************************************************************
 * Fee_DisableFss
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
    FUNC(void, FEE_API_CODE) Fee_DisableFss(void)
    {
        Fee_DetErrorCodeType errCode = Fee_ChkInit();

        if (errCode == FEE_E_NO_ERROR) /* COV_FEE_ERR_DETECT_OFF */
        {
            Fee_InternalControlFssEnable(FEE_FALSE); /* PRQA S 4423 */ /* MD_FEE_10.3_essentially_enum_type */
        }
#if (FEE_DEV_ERROR_DETECT == STD_ON)
        else
        {
            Fee_ReportDetError(FEE_SID_DISABLE_FSS, errCode);
        }
#endif
    }

/**********************************************************************************************************************
 * Fee_EnableFss
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
    FUNC(void, FEE_API_CODE) Fee_EnableFss(void)
    {
        Fee_DetErrorCodeType errCode = Fee_ChkInit();
        if (errCode == FEE_E_NO_ERROR) /* COV_FEE_ERR_DETECT_OFF */
        {
            Fee_InternalControlFssEnable(FEE_TRUE); /* PRQA S 4423 */ /* MD_FEE_10.3_essentially_enum_type */
        }
#if (FEE_DEV_ERROR_DETECT == STD_ON)
        else
        {
            Fee_ReportDetError(FEE_SID_ENABLE_FSS, errCode);
        }
#endif
    }

#endif



#if (FEE_FORCE_SECTOR_SWITCH_API == STD_ON)
/**********************************************************************************************************************
 * Fee_ForceSectorSwitch
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, FEE_API_CODE) Fee_ForceSectorSwitch(void)
{
    Fee_DetErrorCodeType errCode = Fee_ChkInit();

    if (errCode == FEE_E_NO_ERROR) /* COV_FEE_ERR_DETECT_OFF */
    {
        Fee_GlobalSuspend();

        if(MEMIF_IDLE == Fee_Globals_t.FeeModuleStatus_t)
        {
            Fee_Globals_t.FeeModuleStatus_t = MEMIF_BUSY_INTERNAL;
        }

        Fee_GlobalRestore();

        Fee_InternalForceSectorSwitch(FEE_FALSE);  /* PRQA S 4423 */ /* MD_FEE_10.3_essentially_enum_type */
    }

    return Fee_ErrorCode2ReturnType(errCode, FEE_SID_FORCE_SECTOR_SWITCH);
}
#endif




#if (FEE_DATA_CONVERSION_API == STD_ON)
/**********************************************************************************************************************
 * Fee_ConvertBlockConfig
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
    FUNC(Std_ReturnType, FEE_API_CODE) Fee_ConvertBlockConfig(Fee_ConversionOptionsConstRefType options_pt)
    {
        Fee_DetErrorCodeType errCode = Fee_DetChkParamPointer(options_pt); /* PRQA S 0315 */ /* MD_FEE_1.1_Cast_pointer_to_void */ /* SBSW_FEE_ConstPointer_refParam */

#if (FEE_DEV_ERROR_DETECT == STD_ON)

        if((errCode == FEE_E_NO_ERROR) && (options_pt->notificationPtr == 0)) /* PRQA S 3004 */ /* MD_FEE_11.9_true_null_pointer */
        {
            errCode = FEE_E_PARAM_DATABUFFERPTR;
        }
#endif
        if(errCode == FEE_E_NO_ERROR) /* COV_FEE_ERR_DETECT_OFF */
        {
            errCode = Fee_DetChkParamPointer(options_pt->userBuffer); /* PRQA S 0315 */ /* MD_FEE_1.1_Cast_pointer_to_void */  /* SBSW_FEE_ConstPointer_refParam */

            if(errCode == FEE_E_NO_ERROR) /* COV_FEE_ERR_DETECT_OFF */
            {
                errCode = Fee_ChkInit();

                if(errCode == FEE_E_NO_ERROR) /* COV_FEE_ERR_DETECT_OFF */
                {
                    union Fee_tagJobSpecificParams p; /* PRQA S 0759 */ /* MD_MSR_Union */

                    p.conv = *options_pt;
                    Fee_Globals_t.JobParams.partitionId = 0; /* must be valid, but is actually a "don't care" */

                    errCode = Fee_InternalInitRequest(FEE_JOB_CONVERT_DATA_BLOCKS, 0, p); /* SBSW_FEE_Conversion_OptionsPointer */
                    if(errCode == FEE_E_NO_ERROR) /* COV_FEE_ERR_DETECT_OFF */
                    {
                        Fee_InternalForceSectorSwitch(FEE_TRUE); /* PRQA S 4423 */ /* MD_FEE_10.3_essentially_enum_type */
                    }
                }
            }
        }

        return Fee_ErrorCode2ReturnType(errCode, FEE_SID_CONVERT_BLOCK_CONFIG);
    }
#endif /*  (FEE_DATA_CONVERSION_API == STD_ON) */



#if (FEE_LOOKUPTABLE_MODE == STD_ON)
/**********************************************************************************************************************
 * Fee_SyncLookUpTable
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, FEE_API_CODE) Fee_SyncLookUpTable(void)
{
  /*#10 Perform initialization check*/
  Fee_DetErrorCodeType errCode = Fee_ChkInit();
  if (errCode == FEE_E_NO_ERROR) /* COV_FEE_ERR_DETECT_OFF */
  {
    Fee_GlobalSuspend();

    /*#20 Set module status to busy internal when status is idle*/
    if (MEMIF_IDLE == Fee_Globals_t.FeeModuleStatus_t)
    {
      Fee_Globals_t.FeeModuleStatus_t = MEMIF_BUSY_INTERNAL;
    }

    Fee_GlobalRestore();

    /*#30 Signal a sync LookUpTable job to be processed by main function*/
    Fee_InternalSyncLookUpTable();
  }

  return Fee_ErrorCode2ReturnType(errCode, FEE_SID_SYNC_LOOKUP_TABLE);

} /* End of Fee_SyncLookUpTable() */



/**********************************************************************************************************************
 * Fee_InternalSyncLookUpTable
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_InternalSyncLookUpTable(void)
{
  Fee_GlobalSuspend();
  /*#10 Set syncLookUpTable flag */
  Fee_Globals_t.ApiFlags.syncLookUpTable = TRUE;

  Fee_GlobalRestore();

  /*#20 Set enableFss flag when FSS_CONTROL_API is off*/
#if(FEE_FSS_CONTROL_API == STD_OFF)
  Fee_InternalControlFssEnable(FEE_TRUE); /* PRQA S 4423 */ /* MD_FEE_10.3_essentially_enum_type */
#endif

  /*#30 Set processing flag*/
  Fee_SignalNeedProcessing();
}



#endif

/**********************************************************************************************************************
 * Fee_InternalForceSectorSwitch
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
#if((FEE_DATA_CONVERSION_API == STD_ON) || (FEE_FORCE_SECTOR_SWITCH_API == STD_ON))
FEE_LOCAL_FUNC(void) Fee_InternalForceSectorSwitch(Fee_Bool forceFblMode) /* PRQA S 3206 */ /* MD_FEE_2.7_Parameter_Used_For_Other_Configs*/
{
    Fee_GlobalSuspend();
    Fee_Globals_t.ApiFlags.forceSectorSwitch = TRUE;
    Fee_Globals_t.ApiFlags.enableBss = TRUE;

#  if (FEE_DATA_CONVERSION_API == STD_ON)
    Fee_Globals_t.ApiFlags.forceFblMode = (forceFblMode == FEE_TRUE); /* PRQA S 4404, 4423, 1881 */ /* MD_FEE_10.3_essentially_boolean_type, MD_FEE_10.3_essentially_enum_type, MD_FEE_10.4_compare_FeeBool_to_enum */
#  else
    FEE_DUMMY_STATEMENT(forceFblMode);
#  endif

    Fee_GlobalRestore();

    Fee_InternalControlFssEnable(FEE_TRUE);  /* PRQA S 4423 */ /* MD_FEE_10.3_essentially_enum_type */
    Fee_SignalNeedProcessing();
}
#endif




/**********************************************************************************************************************
 * Fee_InternalInitRequest
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(Fee_DetErrorCodeType) Fee_InternalInitRequest(enum Fee_tagReqestedJob job, sint8 partitionIndex,
                                                   union Fee_tagJobSpecificParams specParams)                    /* PRQA S 0759 */ /* MD_MSR_Union */
{
    Fee_DetErrorCodeType err;

    Fee_GlobalSuspend();

    err = Fee_ChkNotBusy();

    if(err == FEE_E_NO_ERROR) /* COV_FEE_ERR_DETECT_OFF */
    {
        Fee_Globals_t.JobParams.params = specParams;

        Fee_Globals_t.JobParams.partitionId = partitionIndex;
        Fee_Globals_t.JobParams.requestedJob = (Fee_RequestedJobType)job;

        Fee_Globals_t.FeeModuleStatus_t = MEMIF_BUSY;
        Fee_Globals_t.FeeJobResult_t = MEMIF_JOB_PENDING;
    }

    Fee_GlobalRestore();

    Fee_SignalNeedProcessing();

    return err;
}


/**********************************************************************************************************************
 * Fee_InternalInitWriteRequest
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
FEE_INLINE_FUNC(Fee_DetErrorCodeType) Fee_InternalInitWriteRequest(enum Fee_tagReqestedJob job, uint16 blockNumber, Fee_UserDataConstPtrType dataBuf)
{
    Fee_BlockCfgDatasetPair block;
    Fee_DetErrorCodeType retVal = Fee_getBlockCfgFromBlockNumber_Checked(&block, blockNumber); /* SBSW_FEE_Func_passParamByReference */

    if(retVal == FEE_E_NO_ERROR) /* COV_FEE_ERR_DETECT_OFF */
    {
        union Fee_tagJobSpecificParams p; /* PRQA S 0759 */ /* MD_MSR_Union */

        p.blockJob.add.write.writeBuffer = dataBuf;
        p.blockJob.blockRef = block;

        retVal = Fee_InternalInitRequest(job, (sint8)(block.config->PartitionIndex), p); /* SBSW_FEE_getBlockCfgFromBlockNumber */

        #if(FEE_FSS_CONTROL_API == STD_OFF)
        if(retVal == FEE_E_NO_ERROR) /* COV_FEE_ERR_DETECT_OFF */
        {
            Fee_InternalControlFssEnable(FEE_TRUE);  /* PRQA S 4423 */ /* MD_FEE_10.3_essentially_enum_type */
        }
#endif
    }

    return retVal;
}

#define FEE_STOP_SEC_CODE
#include "MemMap.h"      /* PRQA S 5087 */ /* MD_MSR_19.1 */
/* PRQA L:uncoveredCode */

/* module specific MISRA deviations:
 *
 *
  MD_FEE_1.1_Cast_pointer_to_void:
  Reason: Avoid implementing more than one "pointer checking function" by using void pointer as parameter.
  Risk: None, a cast to a void pointer is always safe. It is dangerous to cast in the other direction. Nevertheless,
        Fee_DetChkParamPointer does no cast back, it just checks for null pointer. Hence, everything is safe here.
  Prevention: Review

   MD_FEE_2.2_Condition_False_In_Other_Configs:
   Reason: This conditions can not be false for some configs (DET off).
   Risk: None.
   Prevention: ECO would detect coverage leaks.


   MD_FEE_2.4_Parameter_Used_For_Other_Configs:
   Reason: This parameter is used for configuration specific code and might be not needed for some configurations.
   Risk: no risk.
   Prevention: n/a

   MD_FEE_2.4_CompileChecks:
   Reason: This structure is used for static compile checks in order to verify correct sizes of some arrays.
   Risk: None, since this code is only used for static compile tests and not for runtime test.
   Prevention: n/a

   MD_FEE_2.7_Parameter_Used_For_Other_Configs:
   Reason: This parameter is used for configuration specific code and might be not needed for some configurations.
   Risk: no risk.
   Prevention: n/a

   MD_FEE_4.1_Loop_Does_Terminate:
   Reason: Continue with next state machine state until main function must be left for flash request. The number of calls
           is not known before. So a flag within state machine actions is set which prohibits further execution of loop body.
   Risk: Possible endless loop, if setting the flag is forgotten for one path.
   Prevention: Review and tests. Use state machine tracer to verify that all crucial transitions have been tested.

   MD_FEE_5.1_identifiers:
     Reason: Identifiers' name length have been exceeded due to their naming scheme
     Risk: Compiler might issue warnings or errors. Currently no such compiler is known.
     Prevention: n/a

   MD_FEE_8.7_fsm_tables:
     Reason: State Machine Descriptor Tables are actually accessed by one function.
             However, usage of MemMap.h prevents from defining objects with static storage duration,
             as MemMap-Sections may not be nested.
     Risk: no risk
     Prevention: n/a


   MD_FEE_8.9_fee_config_struct:
     Reason: Offer a global container holding configuration values. Otherwise testing would not be possible
     Risk: no risk
     Prevention: n/a

   MD_FEE_8.9_invalid_block_number:
     Reason: All block numbers for accessing the sector array shall be defined at the same place, even when the invalid block number
             is only used in one function.
     Risk: no risk
     Prevention: n/a

     MD_FEE_8.13_not_const_in_other_configs:
     Reason: The parameter cannot be const in other configs (i.e. lut enabled).
     Risk: no risk
     Prevention: n/a


   MD_FEE_10.1_Action_Query_State_Compile_Checks:
     Reason: Check that correct number of actions, queries and states are implemented. Therefore, we use the last element of the enum and increment it
             by 1 to get the actual size. There are no drawbacks here for using an enumeration in an arithmetical operation.
     Risk: No risk, it wont compile in case of an error.
     Prevention: n/a

   MD_FEE_10.1_Essentially_boolean_in_logical_expressions:
     Reason: Avoid to use boolean type defined in stdbool.h
     Risk: No risk, values are either 1 or 0. There is no possibility of loosing information.
     Prevention: Review

   MD_FEE_10.3_instance_status_type:
     Reason: Enums are handled as uint32 values, but they have to be compared to content of uint8 buffer. But Instance header and trailer
             are by design 8bit.
     Risk: No risk, because every instance status type in enumeration has a fixed uint8 value assigned
     Prevention: Review.

   MD_FEE_10.3_essentially_boolean_type:
     Reason: Avoid to use boolean type defined in stdbool.h.
     Risk: No risk, values are either 1 or 0. There is no possibility of loosing information
     Prevention: Review.

   MD_FEE_10.3_essentially_enum_type:
     Reason: Use enumeration to store values for true and false, whereas Fee_Bool is a plain integer value. Otherwise it would not be
             possible to store the result of a boolean expression in a Fee_Bool. This seems to be a tool (i.e. QAC) problem, because MISRA
             itself suggest to implement booleans that way.
     Risk: No risk, values are either 1 or 0. There is no possibility of loosing information
     Prevention: Review.

   MD_FEE_10.4_compare_essentially_boolean_to_FeeBool:
     Reason: Avoid to use boolean type defined in stdbool.h.
     Risk: No risk, values are either 1 or 0. There is no possibility of loosing information
     Prevention: Review.

   MD_FEE_10.4_compare_FeeBool_to_enum:
     Reason: Avoid to use boolean type defined in stdbool.h. Actually this is the way that MISRA suggest. This rule seems to make no sense...
     Risk: No risk, values are either 1 or 0. There is no possibility of loosing information
     Prevention: Review.

   MD_FEE_10.4_compare_enums:
     Reason: Safe ROM and allow easy debugging at once. If DET is enabled, just use enum based types (size of 4 bytes in C90). If DET is not
             enabled use uint8. In this case MISRA complains, but this is the only way to support both goals.
     Risk: No risk.
     Prevention: Compile checks to verify correct size of action, query and state array.

   MD_FEE_10.5_cast essentially_boolean:
     Reason: Avoid to use boolean type defined in stdbool.h. This rule seems to make no sense...
     Risk: No risk, values are either 1 or 0. There is no possibility of loosing information
     Prevention: Review.

   MD_FEE_11.4_pointer_cast:
     Reason: Casts to less strict alignment (smaller pointed-to types). FEE's internal buffer is based on uint32
             In order to have a guaranteed alignment. However some accesses must be byte-wise, especially
             when instance data (header, trailer and user data/payload) are involved.
     Risk: no risk
     Prevention: n/a


   MD_FEE_11.4_lut_pointer_cast:
     Reason: Pointer to LookUpTable entries of type Fee_LookUpTableEntryPtrType need to be casted to Fee_UserDataPtrType because
             LookUpTable blocks are written to and read from Flash memory. The buffer for reading and writing needs to be of type
             Fee_UserDataPtrType, thus the pointer to the LookUpTable entries needs to be casted. Actually this is a cast from
             *uint32 to *uint8.
     Risk: no risk
     Prevention: n/a

   MD_FEE_11.9_true_null_pointer:
     Reason: NULL_PTR is specified to be (void*)0, which does not comply with ISO C!
             Compilers might issue diagnostics when comparing a pointer to function with (void*)0.
             Thus we use the true null pointer: 0
     Risk: no risk
     Prevention: n/a


   MD_FEE_13.5_queries:
     Reason: Functions at right hand of || or && are defined to have no side-effects.
             It intended and thus ok/intended to omit unnecessary calls.
             Avoiding this deviation would degrade code structure without benefits (nesting of if clauses)
     Risk: A function HAS side effects, leading to unexpected behavior, if call is omitted.
     Prevention: Review; make sure, that these functions do not change any global value,
                 and that they do not access "volatile" values.


   MD_FEE_13.2_boolean: 
     Reason: According to rule description this is no violation; it is criticized by QAC.
             Two types are used to represent boolean values, as suggested in Appendix E, "Boolean Expressions":
             Fee_Bool and bit-fields (of width 1). The first one, is used within normal code flow (parameters,
             return values, automatic/local variables) the latter one is used to store boolean values effectively
             in global variables.
     Risk: Type is misused, leading to unexpected behavior. Misuses are hard to detect, because QAC does not
           support declaring types for use as boolean.
     Prevention: Review; boolean values shall only be set by (effectively) boolean expressions.

   MD_FEE_14.3_CompileChecks:
   Reason: Ensure a minimum size of all internal buffers as additional compile check. In case of wrong sizes code won't compile.
           In case all sizes are correct result of evaluation is always true which is detected by QAC.
   Risk: None, since this code is only used for static compile tests and not for runtime test.
   Prevention: n/a

   MD_FEE_14.3_Condition_False_in_Other_Configs:
   Reason: This conditions can not be false for some configs (DET off.
   Risk: None.
   Prevention: ECO would detect coverage leaks.

   MD_FEE_8.13_Pointer2ConstParams
     Reason: Function implements a kind of API - the contract was defined by a Function Pointer.
             It states that the object may be modified. Implementations of that contract must
             follow that prototype to be compatible
     Risk: None.
     Prevention: n/a

   MD_FEE_8.13_API
     Reason: Function prototype is specified by AUTOSAR
     Risk: None.
     Prevention: n/a

   MD_FEE_8.13_PointerParam_IS_modified
     Reason: Apparently QAC refers to undefined behavior: neither ISO C90 nor ISOC99 specify the (type of the) result of
             taking the address of a const-qualified variable. Similarly there is no information about taking the address
             of a struct member, via a pointer to const (struct).
             Referring to const-correctness it should be assumed that the result of applying the address operator ('&')
             inherits qualifiers.
             Following program logic, the conclusion, the pointer-parameter is not modified, is also incorrect:
             A function is called, passing a pointer to structure member (pointer to struct was passed to caller) as argument.
             The callee _will_ modify data; hence the caller _will_ modify, too.

     Risk: None.
     Prevention: n/a

   MD_FEE_18.1_MissingStructDefinition
       Reason: Encapsulation. Within component as well as externally, the structure shall be used only as opaque data pointer.
               Users shall not worry about exact definition; they SHALL NOT access specific contents.
               (Pointer) type declaration is necessary for global structure definition (Fee_GlobalVariablesType; it contains pointer),
               which in turn cannot be hidden - it is used.
               Hiding definition ensures violations against abstraction will be detected by compiler.
       Risk: None.
       Prevention: Recent compilers detect incorrect usage.

   MD_FEE_18.6_CopyingToPointerWithGreaterLifetime
       Reason: Only pointers (within structures) that point to something within Fee_Globals_t (i.e. pointer to sector info, pointer to chunk hopping context, to partition and
       pointer to the block configuration) are copied. Since they are global, the source to which they are copied cannot have any greater lifetime. 
       Risk: None.
       Prevention: Review. Check that only pointers to global variables are copied.

*/

/* SBSW_JUSTIFICATION_BEGIN
\ID SBSW_FEE_PointerWrite_refParam
\DESCRIPTION Pointer Write Access or forwarding a pointer argument to another (internal) function; function parameters, passed by reference.
\COUNTERMEASURE \N Caller has to ensure that pointer is valid, writable (i.e. points to non-const area), and of appropriate type.

\ID SBSW_FEE_ConstPointer_refParam
\DESCRIPTION Pointer is passed as function argument; parameter is "pointer-to-const"
\COUNTERMEASURE \N Called function promises to not write via such a pointer, neither directly nor indirectly.

\ID SBSW_FEE_UserDataBuffer
\DESCRIPTION Dereferencing a pointer to a user-supplied buffer, or passing it as as argument.
This buffer is used to read data into, i.e. it is written by FEE (or lower layers).
\COUNTERMEASURE \S Caller has to ensure that pointer is and remains valid (and of correct size), for a request's complete processing time.
\R Runtime-Check exists to verify it's not a NULL_PTR

\ID SBSW_FEE_ConstUserDataBuffer
\DESCRIPTION Dereferencing a pointer to a user-supplied buffer, or passing it as as argument.
    Such a buffer is used to write FROM; it is not written to.
    Additionally neither FEE nor Fls do interpret the data contents.
\COUNTERMEASURE \N This pointer is never written to; lower layer's (Fls's) API (according to AUTOSAR) also promises to read only.


\ID SBSW_FEE_InternalDataBuffer_writeAccess
\DESCRIPTION Writing via a pointer to an internally supplied buffer, implicitly assuming it has sufficient size.
    Caller (component itself) is responsible to pass correct buffer to accessing functions.
    Callee assumes that are large enough to hold at least 16bytes (size of chunk header - FEE's largest management info).
    Additionally it must also be large enough to hold one complete write page (the largest value of all configured sectors).
    Minimum size for write accesses is ensured by compile-time checks (Fee_Processing.c; struct Fee_tagBufferSizes).
\COUNTERMEASURE \S Verify that internal buffer size is equal to or larger than all configured partitions' FeeAddressAliment values. SMI-2553

\ID SBSW_FEE_InternalDataBuffer_passedTo
\DESCRIPTION Passing an internal buffer to a function (which shall write to it - i.e. non-const).
Within component we assume correct size: The buffer is statically allocated.
Minimum size for write accesses is ensured by compile-time checks (Fee_Processing.c; struct Fee_tagBufferSizes).
\COUNTERMEASURE \S Verify that internal buffer size is equal to or larger than all configured partitions' FeeAddressAliment values. SMI-2553, \S SMI-2553

\ID SBSW_FEE_Func_passParamByReference
\DESCRIPTION The address of an object is passed to a function. The object is "passed by reference".
This includes the address of a variable or a struct member, as well as the address of an element obtained via a pointer
(e.g. retrieving the address of a structure member via a pointer to this structure).
In the latter case the pointer is assumed valid (generally ensured by a function's caller)
\COUNTERMEASURE \N Since an Object (of a specific type) has storage allocated by compiler, it is always valid.

\ID SBSW_FEE_Func_writeObjectByReference
\DESCRIPTION The reference to an object (constant or variable) is used to write data or to pass it to other functions (which may or may not modify).
This reference was created locally (and const), just to get a short syntax, to increase readability.
\COUNTERMEASURE \N Since an Object (of a specific type) has storage allocated by compiler, it is always valid.

\ID SBSW_FEE_getBlockCfgFromBlockNumber
\DESCRIPTION Function Fee_getBlockCfgFromBlockNumber is responsible to deliver a valid BlockConfig pointer,
or NULL_PTR.
\COUNTERMEASURE \T TCASE-394215
\T TCASE-394216
\T TCASE-394217
\T TCASE-394218
\T TCASE-394219
\R ConfigPointer is used to perform additional runtime checks, especially offset and length (Fee_Read).

\ID SBSW_FEE_InternalInitWriteRequest
\DESCRIPTION NULL_PTR is a valid argument for parameter dataBuf, unless it is actually Fee_WriteBlock.
Furthermore they are only read, never written (=> Pointer to Const)
\COUNTERMEASURE \N Fee_EraseImmediateBlock and Fee_InvalidateBlock are actually write requests, but they don't have any user data.

\ID SBSW_FEE_PartitionConfig
\DESCRIPTION Fee_Config references PartitionConfig array, of size Fee_NumberOfPartitions_u8 (also stored in Config)
It is assumed that the FeeConfig pointer is valid (to be ensured by caller of Fee_InitEx).
A FeePartitionConfig element contains a pointer referencing partition's and its two sectors' RAM data.
This association is established at compile time; the referenced RAM data structure is also an array of same
number of elements.
\COUNTERMEASURE \N Fee_NumberOfPartitions_u8 matches the size of Fee_PartitionConfig_at (and Fee_PartitionRamData)
All are based on a common macro, FEE_NUMBER_OF_PARTITIONS; for "silence" it does not matter that its value actually
matches the configuration.

\ID SBSW_FEE_PointerWrite_FsmStack
\DESCRIPTION Indexed access to fsmStack->Stack_at via fsmStack->TopOfStack_u8, which might get out of bounds
(Bounds are [0..FEE_FSM_STACK_SIZE))
The maximum Stack size is determined by design, and thus it is fixed.
\COUNTERMEASURE \N The whole TestSuite and resulting coverage prove that FEE_FSM_STACK_SIZE is sufficient.

\ID SBSW_FEE_FunctionPointerCall_FsmTable
\DESCRIPTION Call a function via function pointer, by indexing into Fee_ActionTable_ap, Fee_QueryTable_ap
These tables are defined at compile time (in ROM).
The indexes into these table are defined by enumerators: Fee_StateEnumType, Fee_StateQueryEnumType, Fee_StateActionEnumType
Thus they are limited to their respective ranges.
\COUNTERMEASURE \N Additional compile time checks exist, to verify that each table has exactly the number of elements.
The correctness of mappings (enumerator vs. pointer) is ensured by whole test suite. In case of mismatch the component would not
behave as intended; tests would fail.

\ID SBSW_FEE_InitializedProcessData
\DESCRIPTION Accessing Members of CurrentProcessData (read, write, pass to functions).
All members (especially Partition_pt, which is a pointer) were initialized/updated (valid), whenever the module left IDLE state.
While in IDLE state, this structure is used.
\COUNTERMEASURE \T TCASE-19645
\T TCASE-23097
\T TCASE-22046
\T TCASE-19610
\T TCASE-19618

\ID SBSW_FEE_GlobalConfigPointer
\DESCRIPTION Fee_Globals_t.Config_pt is ALWAYS valid, once FEE was initialized.
There is no write access to this member except in Fee_InitEx.
\COUNTERMEASURE \S Verify that pointer passed to Fee_InitEx is valid. SMI-2554

\ID SBSW_FEE_ChunkHoppingContext_ref
\DESCRIPTION ChunkHoppingContext reference get initialized to the address of Fee_Globals_t.FsmLocalVars.[ChunkAllocContext_t or chunkSearchContext_t].super_t
Hence it is always valid within those operations.
\COUNTERMEASURE \N There is no write access to this member except in Fee_InitEx.

\ID SBSW_FEE_Partitions_OldestNewestSectorRef
\DESCRIPTION Fee_Partition_getNewestSector and Fee_Partition_getOldestSector always return a VALID reference (SectorInfoRefType) or NULL.
Once Partition Init SubFsm was run, none of both Functions returns even NULL.
Partitions cannot be de-initialized; during run-time the pointer toggles between pre-configured values (associatedSectors_at)
It is sufficient to ensure, that a partition was properly initialized.
\COUNTERMEASURE \T TCASE-26456, TCASE-348579, TCASE-19635, TCASE-19626, TCASE-19618, TCASE-19610, TCASE-22046

\ID SBSW_FEE_Partitions_GetSectorByPosition
\DESCRIPTION Fee returns a pointer to the wanted sector in the the sector array.
Since the values really exist, it is assured that the returned pointer is valid, as long as the given index is correct.
\COUNTERMEASURE \N The caller has to assure that a correct index is forwarded to the function

\ID SBSW_FEE_SectorFormatBuffer
\DESCRIPTION Fee uses a buffer in the sector format context to write the sector header and the erase start marker.
The buffer is allocated at compile time with at least one page size which is sufficient for both.
\COUNTERMEASURE \N If this would not be given, testsuite would crash anyway.

\ID SBSW_FEE_InstanceSearchContext
\DESCRIPTION InstanceSearchContext and its members are used, as the name suggests, during instance search.
It was initialized during Fee_ActInitInstanceSearchBinaryFsm, Fee_ActInitInstanceSearchBinaryFsm, Fee_ActInitInstanceSearchUpwards
Later on it does not change (instance search is limited to one single chunk)
\COUNTERMEASURE \N If this behavior was not given, Test would not succeed.


\ID SBSW_FEE_ChunkAllocContext
\DESCRIPTION ChunkAllocContext and its members are used, as the name suggests, during chunk allocation.
It was initialized during Fee_ActPreInitWriteProcessing, Fee_ActInitLinkTableAllocationFsm, Fee_InternalInitChunkAllocCopyFsm,
Fee_Proc_CreateChunkAllocContext, Fee_ActInitDCWriteProcSubFsm.
\COUNTERMEASURE \N If this behavior was not given, Test would not succeed.

\ID SBSW_FEE_ChunkSearchContext
\DESCRIPTION SBSW_FEE_ChunkSearchContext and its members are used, as the name suggests, during chunk chunk search.
It was initialized during Fee_Proc_InitReverseChunkSearch, Fee_ActInitSearchUpwardsFsm, Fee_ActContinueUpwardsChunkSearch,
Fee_ActSetupSectorSwitchSourceSearch, Fee_ActSwitchOperatingSector, Fee_ActSetupJobProcessing
During processing the pointers don't change.
\COUNTERMEASURE \N If this behavior was not given, Test would not succeed.


\ID SBSW_FEE_LutChunkSearchContext
\DESCRIPTION SBSW_FEE_LutChunkSearchContext and its members are used, as the name suggests, during lut chunk search.
It was initialized during Fee_ActInitChunkSearchFsm. During processing the pointers don't change.
\COUNTERMEASURE \N If this behavior was not given, Test would not succeed.


\ID SBSW_FEE_Chunks_Sector_ref
\DESCRIPTION A ChunkInfo is associated with a particular sector the corresponding chunk resides in,
or it shall be allocated in.
ChunkInfos are always initialized with a particular sector (from a given partition), either explicitly
or implicitly (Fee_ChunkInfo_clone)
\COUNTERMEASURE \N If this behavior was not given, Test would not succeed.

\ID SBSW_FEE_Conversion_OptionsPointer
\DESCRIPTION A user-supplied data buffer is used and eventually a user-supplied function is called via function pointer.
Both pointers were passed when requesting Fee_ConvertBlockConfig.
\COUNTERMEASURE \S Caller shall ensure that Function pointer is valid,
and User Data buffer is large enough (larger than largest block that might have been written by FEE)
\R Module's checks prevent from passing NULL_PTRs
\S User has to ensure that the data conversion feature is disabled for Microsar Safe. SMI-2595


\ID SBSW_FEE_Lut_Entry_ref
\DESCRIPTION A reference to a lut ram entry, which is used to store the chunk header address for a block.
This reference is computed by function Fee_Lut_GetEntry only.
\COUNTERMEASURE \N Fee_Lut_GetEntry guarantees only to return valid entry references (see own argumentation).

\ID SBSW_FEE_Lut_Entry_Index
\DESCRIPTION Index that is used to access elements in the generated Entries array. This index is computed by function
Fee_Lut_GetEntryIndex by adding entriesStartIdx, lutBaseIdx and datasetIndex..
\COUNTERMEASURE \M Result of computation must be within Entries array borders. Verify that: LutCfgIdx is correct, LutEntriesStartIdx + LookUpTableIndex
+ NumberOfDatasets_u8 - 1 < FEE_LUT_ENTRIES_ARRAY_SIZE for every block of a lut partition.


\ID SBSW_FEE_Lut_Block_Ram_Entries
\DESCRIPTION For one lut block all ram entries shall be iterated. Therefore a correct start and end index are necessary.
\COUNTERMEASURE \M End index is computed out of start index and NrOfEntries (from config, verfied via MSSV, see above counter measure). If
start index is correct, also end index is correct. Start index is computed via function Fee_Lut_GetLutBlocksStartIndexInRam. This function uses
lutCfgIdx and syncBlockIndex to compute the start index out of config values: LutEntriesStartIdx and NrEntries, which are both verified via MSSV (see below countermeasure)
Hence, it must be assured that lutCfgIdx and syncBlockIndex are correct. Caller has to assure that lutCfgIdx was "extracted" from a valid partition configuration.
SyncBlockIndex is initialized by function Fee_Lut_GetNextSyncPartitionIndex (set to NrBlocks, which is verfied by MSSV). It is only decremented by function Fee_Lut_PrepareSyncContextForNextBlock.
This function also checks, if a correct syncBlockIndex could be computed. In case it is invalid (i.e. no more block to sync) it returns false and the further sync handling is aborted (i.e. no further ram access).
Summarization: Verify, that for every partition with lut enabled NrEntries, NrBlocks and lutCfgIdx are correct.
Result of computations must be within Entries array borders. Verify that:lutCfgIdx is correct, LutEntriesStartIdx + (numberOfLutNvBlocks *
NumberOfEntriesPerLutBlock) < FEE_LUT_ENTRIES_ARRAY_SIZE for every lut partition.


\ID SBSW_FEE_Lut_ModifiedFlags_Access
\DESCRIPTION An uint32 array in connection with in index is used in order to determine if a nv lut block has been modified (in ram) and
must be rewritten.
\COUNTERMEASURE \M Index must be inside modified flags array borders. Verify that:lutCfgIdx is correct, LutModifedFlagIdx < FEE_NUMBER_OF_PARTITIONS_WITH_LUT


SBSW_JUSTIFICATION_END */

/* START_COVERAGE_JUSTIFICATION

\ID COV_FEE_COMPATIBILITY
  \ACCEPT XF
  \ACCEPT TX
  \REASON [COV_MSR_COMPATIBILITY]

\ID COV_FEE_PREEMPTION
  \ACCEPT TF tf tx
  \ACCEPT TX
  \REASON Check is necessary, because Fee (Fee_MainFunction) may run in an preemptive task; FEE API may be called from
  another task (higher priority). Preemption might occur anywhere - this cannot be tested.

\ID COV_FEE_FLS_CANCEL
  \ACCEPT TX
  \REASON Fls may indeed by busy when FEE handles a cancel request (e.g. a long lasting erase).
  TestSuite cannot create this situation; this it must be checked by review

\ID COV_FEE_PART_CONFIG
  \ACCEPT XF
  \ACCEPT TX
  \ACCEPT TF tx tf
  \ACCEPT TF tf tx tf
  \ACCEPT TF tx tf tf
  \ACCEPT TF tx
  \ACCEPT TF xf
  \ACCEPT TF xf tf
  \REASON FEE's partition config is part of run-time config. Test configurations intentionally cover a standard use case:
  one partition per config. In those configurations partition related checks cannot be fully covered by tests.
  They shall be checked by review.

\ID COV_FEE_DEFAULT_CASE
  \ACCEPT XX
  \REASON [COV_MSR_MISRA]

\ID COV_FEE_ERR_DETECT_OFF
  \ACCEPT TX
  \REASON [COV_MSR_UNSUPPORTED]

\ID COV_FEE_ERR_DETECT_OFF_REPORTING
  \ACCEPT XX
  \REASON [COV_MSR_UNSUPPORTED] - Function is not called, if ErrorDetection is OFF.

END_COVERAGE_JUSTIFICATION */

/**********************************************************************************************************************
 *  END OF FILE: Fee.c
 *********************************************************************************************************************/
