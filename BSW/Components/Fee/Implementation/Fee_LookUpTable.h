/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------
 *        \file  Fee_LookUpTable.h
 *        \brief  If_AsrIfFee Lookup Table module header file
 *
 *      \details  The module Fee provides an abstraction from the device specific addressing scheme and
 *                segmentation. It also offers a virtual addressing scheme and segmentation as well as
 *                a virtually unlimited number of erase cycles to the upper layer.
 *
 *********************************************************************************************************************/

#if !defined (FEE_LOOKUPTABLE_H)
# define FEE_LOOKUPTABLE_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "Fee_Int.h" /* PRQA S 3313 */ /*MD_FEE_18.1_MissingStructDefinition */
#if(FEE_LOOKUPTABLE_MODE == STD_ON)
#include "Fee_LookUpTableDefs.h"


/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/


# define FEE_LUT_PARTITION_INVALID_INDEX                                 (0xFFu)
# define FEE_LUT_BLOCK_INVALID_INDEX                                     (0xFFFFu)
/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define FEE_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 * Fee_Lut_InitContextStruct
 *********************************************************************************************************************/
/*!
 * \brief         Initializes LookUpTable context for further processing
 * \details       Initializes LookUpTable context for further processing
 * \param[in,out] self
 * \param[in]     partitionRef
 * \param[in]     processDataRef
 * \param[in]     blockConfigRef
 * \config        FEE_LOOKUPTABLE_MODE is enabled via pre-compile switch
 * \pre           -
 * \context       TASK
 * \reentrant     FALSE
 * \synchronous   TRUE
 */
FEE_INTERN_FUNC(void) Fee_Lut_InitContextStruct(Fee_LookUpTableContextRefType self, Fee_PartitionRefType partitionRef, Fee_ProcessDataConstRefType processDataRef, Fee_BlockConfigRefType blockConfigRef);

/**********************************************************************************************************************
 * Fee_Lut_GetChunkAddressFromLut
 *********************************************************************************************************************/
/*!
 * \brief      Returns ChunkHeaderAddress of current LookUpTable entry
 * \details    Returns ChunkHeaderAddress of current LookUpTable entry
 * \param[in]  self
 * \return     address of the chunk header for the block that shal be searched
 * \pre        -
 * \config        FEE_LOOKUPTABLE_MODE is enabled via pre-compile switch
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FEE_INTERN_FUNC(Fee_LengthType) Fee_Lut_GetChunkAddressFromLut(Fee_LookUpTableContextConstRefType self);


/**********************************************************************************************************************
 * Fee_Lut_GetSectorByAddress
 *********************************************************************************************************************/
/*!
 * \brief         Returns reference to sector in which address is contained
 * \details       Returns reference to sector in which address is contained
 * \param[in,out] jobPartition_pt
 * \param[in]     address
 * \return        Reference to sector: if input address is contained in any sector's valid area.
 * \return        NULL_PTR: if input address is not contained in any sector's valid area
 * \config        FEE_LOOKUPTABLE_MODE is enabled via pre-compile switch
 * \pre           -
 * \context       TASK
 * \reentrant     FALSE
 * \synchronous   TRUE
 */
FEE_INTERN_FUNC(Fee_SectorRefType) Fee_Lut_GetSectorByAddress(Fee_PartitionConstRefType jobPartition_pt,Fee_LengthType address);


/**********************************************************************************************************************
 * Fee_Lut_UpdateLut
 *********************************************************************************************************************/
/*!
 * \brief         Updates LookUpTable entry for current block during FEE jobs or block copy
 * \details       Updates ChunkHeaderAddress of current block in LookUpTable during Read/Write- and
 *                InvalidateBlock/EraseImmediateBlock jobs and sector switch
 * \param[in,out] self
 * \param[in]     chunk
 * \config        FEE_LOOKUPTABLE_MODE is enabled via pre-compile switch
 * \pre           -
 * \context       TASK
 * \reentrant     FALSE
 * \synchronous   TRUE
 */
FEE_INTERN_FUNC(void) Fee_Lut_UpdateLut(Fee_LookUpTableContextRefType self,Fee_ChunkInfoConstRefType chunk);




/**********************************************************************************************************************
 * Fee_Lut_ResetLut
 *********************************************************************************************************************/
/*!
 * \brief         Resets content of current LookUpTable entry
 * \details       Resets content of current LookUpTable entry
 * \param[in,out] self
 * \config        FEE_LOOKUPTABLE_MODE is enabled via pre-compile switch
 * \pre           -
 * \context       TASK
 * \reentrant     FALSE
 * \synchronous   TRUE
 */
FEE_INTERN_FUNC(void) Fee_Lut_ResetLut(Fee_LookUpTableContextRefType self);



/**********************************************************************************************************************
 * Fee_Lut_IsChunkAddressPlausible
 *********************************************************************************************************************/
/*!
 * \brief      Checks if ChunkHeaderAddress entry of current block is within sector range and aligned
 * \details    Checks if ChunkHeaderAddress entry of current block is within sector range and aligned
 * \param[in]  self
 * \param[in]  chunkAddress
 * \return     TRUE: ChunkHeaderAddress of current block is plausible
 * \return     FALSE: ChunkHeaderAddress of current block is out of range
 * \config        FEE_LOOKUPTABLE_MODE is enabled via pre-compile switch
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FEE_INTERN_FUNC(Fee_Bool) Fee_Lut_IsChunkAddressPlausible(Fee_LookUpTableContextConstRefType self,Fee_LengthType chunkAddress);


/**********************************************************************************************************************
 * Fee_Lut_InitLRam
 *********************************************************************************************************************/
/*!
 * \brief      Initializes the ram lookup table
 * \details    Sets all entries and modified flags to 0
 * \param[in]  partition whose lut in ram shall be initialized
 * \config        FEE_LOOKUPTABLE_MODE is enabled via pre-compile switch
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FEE_INTERN_FUNC(void) Fee_Lut_InitRam(Fee_PartitionConfigRefType partition);


/**********************************************************************************************************************
 * Fee_Lut_IsLutEnabled
 *********************************************************************************************************************/
/*!
 * \brief      Checks if LookUpTable feature is enabled for current partition
 * \details    Checks if LookUpTable feature is enabled for current partition
 * \param[in]  part
 * \return     TRUE: LookUpTable feature is enabled
 * \return     FALSE: LookUpTable feature is disabled
 * \config        FEE_LOOKUPTABLE_MODE is enabled via pre-compile switch
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FEE_INTERN_FUNC(Fee_Bool) Fee_Lut_IsLutEnabled(Fee_PartitionConstRefType part);



/**********************************************************************************************************************
 * Fee_Lut_IsAccessAllowed
 *********************************************************************************************************************/
/*!
 * \brief      Returns if LUT is allowed to be used for given context
 * \details    Allowed means lut feature is enabled and block config is not null
 * \param[in]  self
 * \return     TRUE: Access is allowed
 * \return     FALSE: Access is not allowed
 * \config        FEE_LOOKUPTABLE_MODE is enabled via pre-compile switch
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FEE_INTERN_FUNC(Fee_Bool) Fee_Lut_IsAccessAllowed(Fee_LookUpTableContextConstRefType self);


/**********************************************************************************************************************
 * Fee_Lut_PrepareProcessParameter
 *********************************************************************************************************************/
/*!
 * \brief         Sets all necessary parameters according to current job
 * \details       Sets all necessary parameters according to current job
 * \param[in]     jobPartition_pt
 * \param[in]     blockIndex
 * \param[in]     isWriteJob
 * \param[in,out] jobParams
 * \config        FEE_LOOKUPTABLE_MODE is enabled via pre-compile switch
 * \pre           -
 * \context       TASK
 * \reentrant     FALSE
 * \synchronous   TRUE
 */
FEE_INTERN_FUNC(void) Fee_Lut_PrepareProcessParameter(Fee_PartitionConstRefType jobPartition_pt, uint16 blockIndex,
                                                     Fee_Bool isWriteJob, struct Fee_tagBlockSpecificJob *jobParams);



/**********************************************************************************************************************
 * Fee_Lut_ValidateBlockContent
 *********************************************************************************************************************/
/*!
 * \brief         Checks every entry within recently read in lut block whether it "points to the future"
 * \details       Checks whether lut block from old sector points to new sector. Such entries will be reset. If no valid instance was found
 *                all entries are reset
 * \param[in]     jobPartition_pt
 * \param[in]     lutBlockSector
 * \param[in]     syncContext
 * \param[in]     validInstanceFound
 * \config        FEE_LOOKUPTABLE_MODE is enabled via pre-compile switch
 * \pre           -
 * \context       TASK
 * \reentrant     FALSE
 * \synchronous   TRUE
 */
FEE_INTERN_FUNC(void) Fee_Lut_ValidateBlockContent(Fee_PartitionConstRefType jobPartition_pt, Fee_SectorConstRefType lutBlockSector,
                                                   Fee_LookUpTableSyncDataConstRefType syncContext,Fee_Bool validInstanceFound);


/**********************************************************************************************************************
 * Fee_Lut_PrepareSyncContextForNextBlock
 *********************************************************************************************************************/
/*!
 * \brief         Prepares partition index and block index by setting it to next block that shall be synchronized. Returns if another sync is necessary.
 * \details       If current partition has no further blocks, partition index is decremented. If there is no further partition, false is returned.
 * \param[in,out] syncContext
 * \param[in]     config_pt
 * \return        TRUE: Search can be continued with valid sync context
 * \return        FALSE: Search is finished, all blocks have been processed. Don't continue.
 * \config        FEE_LOOKUPTABLE_MODE is enabled via pre-compile switch
 * \pre           -
 * \context       TASK
 * \reentrant     FALSE
 * \synchronous   TRUE
 */
FEE_INTERN_FUNC(Fee_Bool) Fee_Lut_PrepareSyncContextForNextBlock(Fee_LookUpTableSyncDataRefType syncContext,Fee_ConfigRefTypeInternal config_pt);


/**********************************************************************************************************************
 * Fee_Lut_GetNextSyncPartitionIndex
 *********************************************************************************************************************/
/*!
 * \brief         Returns index of next partition that shall be synchronized
 * \details       Returns invalid partition index if there is no further partition to be synced. Sets sync context block index
 *                to partitions number of lut blocks in order to start searching for block afterwards.
 * \param[in,out] syncContext
 * \param[in]     config_pt
 * \return        next partition index in case there is another partition to be synchronized
 * \return        invalid partition index in case there is no further partition to be synchronized
 * \config        FEE_LOOKUPTABLE_MODE is enabled via pre-compile switch
 * \pre           -
 * \context       TASK
 * \reentrant     FALSE
 * \synchronous   TRUE
 */
FEE_INTERN_FUNC(uint8) Fee_Lut_GetNextSyncPartitionIndex(Fee_LookUpTableSyncDataRefType syncContext,Fee_ConfigRefTypeInternal config_pt);


/**********************************************************************************************************************
 * Fee_Lut_ResetModifiedFlag
 *********************************************************************************************************************/
/*!
 * \brief      Resets modified flag for lut block specified in given syncContext
 * \details    Resets modified flag for lut block specified in given syncContext
 * \param[in]  partCfg
 * \param[in]  syncContext
 * \config        FEE_LOOKUPTABLE_MODE is enabled via pre-compile switch
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FEE_INTERN_FUNC(void) Fee_Lut_ResetModifiedFlag(Fee_PartitionConfigRefType partCfg,Fee_LookUpTableSyncDataConstRefType syncContext);
# define FEE_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

#endif /* FEE_LOOKUPTABLE_MODE == STD_ON */
#endif /*!defined (FEE_LOOKUPTABLE_H)*/
/**********************************************************************************************************************
 *  END OF FILE: Fee_LookUpTable.h
 *********************************************************************************************************************/
