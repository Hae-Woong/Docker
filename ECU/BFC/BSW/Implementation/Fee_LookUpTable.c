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
 *  -----------------------------------------------------------------------------------------------------------------
 *        \file  Fee_LookUpTable.c
 *        \brief  If_AsrIfFee Lookup Table module source file
 *
 *      \details  The module Fee provides an abstraction from the device specific addressing scheme and
 *                segmentation. It also offers a virtual addressing scheme and segmentation as well as
 *                a virtually unlimited number of erase cycles to the upper layer.
 *
 ******************************************************************************************************************** */

/* macro identifying this compilation unit */
#define FEE_LOOKUPTABLE_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 ******************************************************************************************************************** */

#include "Fee_LookUpTable.h" /* PRQA S 3313  */ /*MD_FEE_18.1_MissingStructDefinition  */
#if(FEE_LOOKUPTABLE_MODE == STD_ON)
#include "Fee_PartitionDefs.h"
#include "Fee_ChunkInfo.h"

#define FEE_START_SEC_CODE
#include "MemMap.h"      /* PRQA S 5087  */ /* MD_MSR_19.1  */


/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 ******************************************************************************************************************** */

/**********************************************************************************************************************
 * Fee_Lut_UpdateLutEntry
 ******************************************************************************************************************** */
/*!
 * \brief      Updates LookUpTable entry of current block with input parameters
 * \details    Updates LookUpTable entry of current block with input parameters
 * \param[in,out]  self
 * \param[in]  chunkHeaderAddress
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
  */
FEE_LOCAL_FUNC(void) Fee_Lut_UpdateLutEntry(Fee_LookUpTableContextRefType self,Fee_LengthType chunkHeaderAddress);

/**********************************************************************************************************************
 * Fee_Lut_GetEntryIndex
 ******************************************************************************************************************** */
/*!
 * \brief      Returns entry index for current context in lut ram data
 * \details    Returns entry index for current context in lut ram data
 * \param[in]  self
 * \return     entry index
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
  */
FEE_LOCAL_FUNC(uint16) Fee_Lut_GetEntryIndex(Fee_LookUpTableContextConstRefType self);



/**********************************************************************************************************************
 * Fee_Lut_SetModifiedFlag
 ******************************************************************************************************************** */
/*!
 * \brief      Marks the the corresponding nv lut block as modified
 * \details    Marks the the corresponding nv lut block as modified
 * \param[in]  self
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
  */
FEE_LOCAL_FUNC(void) Fee_Lut_SetModifiedFlag(Fee_LookUpTableContextConstRefType self);


/**********************************************************************************************************************
 * Fee_Lut_GetEntry
 ******************************************************************************************************************** */
/*!
 * \brief      Returns reference to current entry
 * \details    Returns reference to current entry
 * \param[in]  self
 * \return     entryRef - pointer to LUT entry type
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
  */
FEE_LOCAL_FUNC(Fee_LookUpTableEntryPtrType) Fee_Lut_GetEntry(Fee_LookUpTableContextConstRefType self);


/**********************************************************************************************************************
 * Fee_Lut_GetLutBlocksStartIndexInRam
 ******************************************************************************************************************** */
/*!
 * \brief      Returns index of first entry in RAM that belongs to currently read in lut block
 * \details    Returns index of first entry in RAM that belongs to currently read in lut block
 * \param[in]  blockIndex
 * \param[in]  lutCfgIdx
 * \return     index of lut block's first entry in ram
 * \pre        Caller has to assure that blockIndex is valid!
 * \context    TASK
 * \reentrant  FALSE
  */
FEE_LOCAL_FUNC(uint16) Fee_Lut_GetLutBlocksStartIndexInRam(uint16 blockIndex,uint8 lutCfgIdx);

/**********************************************************************************************************************
 * Fee_Lut_ResetLutBlocksRamEntries
 ******************************************************************************************************************** */
/*!
 * \brief       Resets all ram entries for currently read in lut block (its block index is stored in sync context)
 * \details     Resets all ram entries for currently read in lut block (its block index is stored in sync context)
 * \param[in]   syncContext
 * \param[in]   lutCfgIdx
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
  */
FEE_LOCAL_FUNC(void) Fee_Lut_ResetLutBlocksRamEntries(Fee_LookUpTableSyncDataConstRefType syncContext,uint8 lutCfgIdx);



/**********************************************************************************************************************
 * Fee_Lut_GetNextSyncBlockIndex
 ******************************************************************************************************************** */
/*!
 * \brief       Returns index of the next block that shall be synchronized
 * \details     Returns invalid block index  if there is no further block to be synced in current partition
 * \param[in]   syncContext
 * \param[in]   config_pt
 * \return      next block index in case there is another block to be synchronized
 * \return      invalid block index in case there is no further block to be synchronized
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
  */
FEE_LOCAL_FUNC(uint16) Fee_Lut_GetNextSyncBlockIndex(Fee_LookUpTableSyncDataConstRefType syncContext,Fee_ConfigRefTypeInternal config_pt);


/**********************************************************************************************************************
 * Fee_Lut_GetBlockModifiedFlag
 ******************************************************************************************************************** */
/*!
 * \brief       Returns whether  lut block needs synchronization
 * \details     Returns whether  lut block needs synchronization
 * \param[in]   lutCfgIdx
 * \param[in]   lutBlockNumber
 * \return      TRUE: Block has been modified
 * \return      FALSE: Block has not been modfied
 * \pre         lutBlockNumber is less or equal 31
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
  */
FEE_LOCAL_FUNC(Fee_Bool) Fee_Lut_GetBlockModifiedFlag(uint8 lutCfgIdx,uint8 lutBlockNumber);

/**********************************************************************************************************************
 * Fee_Lut_UpdateLUTEntry
 ******************************************************************************************************************** */
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
  */
/* PRQA S 3673 1  */ /* MD_FEE_8.13_PointerParam_IS_modified  */
FEE_LOCAL_FUNC(void) Fee_Lut_UpdateLutEntry(Fee_LookUpTableContextRefType self,Fee_LengthType chunkHeaderAddress)
{
  /* #10 Get entry from lut */
  Fee_LookUpTableEntryPtrType lutEntryRef = Fee_Lut_GetEntry(self);  /* SBSW_FEE_ConstPointer_refParam */

  /* #20 Check if input value is different to stored value  */
  if (lutEntryRef->ChunkHeaderAddress != chunkHeaderAddress)
  {
    /* #30 Update LookUpTable value for current entry  */
    lutEntryRef->ChunkHeaderAddress = chunkHeaderAddress; /* SBSW_FEE_Lut_Entry_ref */
    /* #31 Mark corresponding nv lut block as modified */
    Fee_Lut_SetModifiedFlag(self); /* SBSW_FEE_PointerWrite_refParam */
  }
}

/**********************************************************************************************************************
 * Fee_Lut_GetLUTEntryIndex
 ******************************************************************************************************************** */
/*!
 * Internal comment removed.
 *
 *
  */
FEE_LOCAL_FUNC(uint16) Fee_Lut_GetEntryIndex(Fee_LookUpTableContextConstRefType self)
{
  /* #10 LookUpTable specific calculations: blockIndex and entryIndex are used to locate the correct position of current block  */
  uint8 lutCfgIndex = self->Partition_pt->cfg_pt->lutCfgIdx;
  uint16 lutBaseIndex = self->BlockConfig_pt->LookUpTableIndex;
  return (uint16) (Fee_LookUpTableConfig[lutCfgIndex].LutEntriesStartIdx + lutBaseIndex + self->datasetIndex);
}


/**********************************************************************************************************************
 * Fee_Lut_SetModifiedFlag
 ******************************************************************************************************************** */
/*!
 *
 * Internal comment removed.
 *
 *
 *
  */
FEE_LOCAL_FUNC(void) Fee_Lut_SetModifiedFlag(Fee_LookUpTableContextConstRefType self)
{
  /*#10 Compute number of lut block that shall be set to modified. This equals the bit number in flags */
  uint8 lutCfgIndex = self->Partition_pt->cfg_pt->lutCfgIdx;
  uint8 modifiedFlagIdx = Fee_LookUpTableConfig[lutCfgIndex].LutModifedFlagIdx;
  /*Cast and later shift are safe. It is assured by validator that there are max 32 lut blocks per partition */
  uint16 lutBlockNumber = (uint16)((self->BlockConfig_pt->LookUpTableIndex + self->datasetIndex) / Fee_LookUpTableConfig[lutCfgIndex].NrEntries);

  /*#20 Set the corresponding flag */
  uint32 mask = (1uL << lutBlockNumber);

  ModifiedFlags[modifiedFlagIdx] |= mask; /* SBSW_FEE_Lut_ModifiedFlags_Access */
}


/**********************************************************************************************************************
 * Fee_Lut_GetLUTEntry
 ******************************************************************************************************************** */
/*!
 * Internal comment removed.
 *
 *
  */
FEE_LOCAL_FUNC(Fee_LookUpTableEntryPtrType) Fee_Lut_GetEntry(Fee_LookUpTableContextConstRefType self)
{
  return &(Entries[Fee_Lut_GetEntryIndex(self)]); /* SBSW_FEE_Lut_Entry_Index */
}


/**********************************************************************************************************************
 * Fee_Lut_GetLutBlocksStartIndexInRam
 ******************************************************************************************************************** */
/*!
 * Internal comment removed.
 *
 *
  */
FEE_LOCAL_FUNC(uint16) Fee_Lut_GetLutBlocksStartIndexInRam(uint16 blockIndex,uint8 lutCfgIdx)
{
  /*#10 Compute start index, every lutBlock has NrEntries entries, which can be used for calculation */
  return (uint16)(Fee_LookUpTableConfig[lutCfgIdx].LutEntriesStartIdx + (blockIndex * Fee_LookUpTableConfig[lutCfgIdx].NrEntries));
}


/**********************************************************************************************************************
 * Fee_Lut_ResetLutBlocksRamEntries
 ******************************************************************************************************************** */
/*!
 *
 * Internal comment removed.
 *
 *
 *
  */
FEE_LOCAL_FUNC(void) Fee_Lut_ResetLutBlocksRamEntries(Fee_LookUpTableSyncDataConstRefType syncContext, uint8 lutCfgIdx)
{
  /* #10 Get current Lut blocks start and end index in RAM */
  uint16 indexInRam = Fee_Lut_GetLutBlocksStartIndexInRam(syncContext->LookUpTableSyncBlockIndex, lutCfgIdx);
  uint16 endIndex = (uint16)(indexInRam + ((Fee_LookUpTableConfig[lutCfgIdx].NrEntries) - 1u));
  uint16 loopIndex;

  /*#20 Loop over all entries specified by indexes and reset them */
  for (loopIndex = indexInRam; loopIndex <= endIndex; loopIndex++)
  {
    Entries[loopIndex].ChunkHeaderAddress = 0; /* SBSW_FEE_Lut_Block_Ram_Entries */
  }

}


/**********************************************************************************************************************
 * Fee_Lut_GetNextSyncBlockIndex
 ******************************************************************************************************************** */
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
  */
FEE_LOCAL_FUNC(uint16) Fee_Lut_GetNextSyncBlockIndex(Fee_LookUpTableSyncDataConstRefType syncContext,
                                                     Fee_ConfigRefTypeInternal config_pt)
{

  uint16 retValue = FEE_LUT_BLOCK_INVALID_INDEX;
  /* #10 Start search for next partition with next partition that follows (LookUpTablePartitionIndex points to last checked partition)  */
  /*Cast is safe. It is assured by validator that max number of lut blocks is 32 per partition */
  sint16 LoopCounter = (sint16)(((sint16) syncContext->LookUpTableSyncBlockIndex) - 1);
  Fee_PartitionConstRefType partition_pt = Fee_Partition_getFromConfigById(config_pt,syncContext->LookUpTablePartitionIndex); /* SBSW_FEE_PartitionConfig  */

  /*#20 Check in descending order if any of partitions lut block has been modified and must be synchronized therefore */
  for (; LoopCounter >= 0; LoopCounter--)
  {

      /*#30 If block is modified, return loopCounter (equals its sync number) */
      if (Fee_Lut_GetBlockModifiedFlag(partition_pt->cfg_pt->lutCfgIdx, (uint8)LoopCounter) == FEE_TRUE) /* PRQA S 1881 */  /* MD_FEE_10.4_compare_FeeBool_to_enum */
      {
        retValue = (uint16)LoopCounter;
        break;
      }
  }

  /*#40 Return invalid block index if no block to be synced was found */
  return retValue;

}


/**********************************************************************************************************************
 * Fee_Lut_GetBlockModifiedFlag
 ******************************************************************************************************************** */
/*!
 * Internal comment removed.
 *
 *
 *
  */
FEE_LOCAL_FUNC(Fee_Bool) Fee_Lut_GetBlockModifiedFlag(uint8 lutCfgIdx,uint8 lutBlockNumber)
{
  /*#10 Get modified flag idx */
  uint8 modifiedFlagIdx = Fee_LookUpTableConfig[lutCfgIdx].LutModifedFlagIdx;

  /*#20 Get single modified flag for current lut block */
  uint32 mask = (1uL << lutBlockNumber);

  return (ModifiedFlags[modifiedFlagIdx] & mask) != 0uL; /* PRQA S 4403 */ /* MD_FEE_10.3_essentially_boolean_type */
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 ******************************************************************************************************************** */

/**********************************************************************************************************************
 * Fee_Lut_InitContextStruct
 ******************************************************************************************************************** */
/*!
 * Internal comment removed.
 *
 *
  */
FEE_INTERN_FUNC(void) Fee_Lut_InitContextStruct(Fee_LookUpTableContextRefType self, Fee_PartitionRefType partitionRef,
                                                Fee_ProcessDataConstRefType processDataRef, Fee_BlockConfigRefType blockConfigRef)
{
  /* #10 Assign necessary information to LookUpTable context  */
  self->Partition_pt = partitionRef; /* SBSW_FEE_PointerWrite_refParam */
  self->datasetIndex = processDataRef->BlockId.DataIndex; /* SBSW_FEE_PointerWrite_refParam */
  self->BlockConfig_pt = blockConfigRef; /* SBSW_FEE_PointerWrite_refParam */
}


/**********************************************************************************************************************
 * Fee_Lut_GetChunkAddressFromLut
 ******************************************************************************************************************** */
/*!
 * Internal comment removed.
 *
 *
 *
  */
FEE_INTERN_FUNC(Fee_LengthType) Fee_Lut_GetChunkAddressFromLut(Fee_LookUpTableContextConstRefType self)
{
  /* #10 Get reference to current entry  */
  Fee_LookUpTableEntryConstPtrType entryRef = Fee_Lut_GetEntry(self); /* SBSW_FEE_ConstPointer_refParam */

  /* #20 Return entry's ChunkHeaderAddress  */
  return entryRef->ChunkHeaderAddress; /* SBSW_FEE_Lut_Entry_ref */
}

/**********************************************************************************************************************
 * Fee_Lut_GetSectorByAddress
 ******************************************************************************************************************** */
/*!
 * Internal comment removed.
 *
 *
 *
 *
  */
FEE_INTERN_FUNC(Fee_SectorRefType) Fee_Lut_GetSectorByAddress(Fee_PartitionConstRefType jobPartition_pt,Fee_LengthType address)
{
  Fee_SectorRefType retVal;

  Fee_SectorRefType newerSector = Fee_Partition_getNewestSector(jobPartition_pt); /* SBSW_FEE_ConstPointer_refParam */
  Fee_SectorRefType olderSector = Fee_Partition_getOldestSector(jobPartition_pt); /* SBSW_FEE_ConstPointer_refParam */


  /* #10 Check if input address is related to newer sector's valid area (between first free address and end address) */
  if (Fee_Sector_isValidLutAddress(newerSector,address) == FEE_TRUE) /* PRQA S 1881 */  /* MD_FEE_10.4_compare_FeeBool_to_enum */ /* SBSW_FEE_ConstPointer_refParam */
  {
    retVal = newerSector;
  }
  /* #20 Check if input address is related to older sector's valid area (between first free address and end address) */
  else if (Fee_Sector_isValidLutAddress(olderSector,address) == FEE_TRUE) /* PRQA S 1881 */  /* MD_FEE_10.4_compare_FeeBool_to_enum */ /* SBSW_FEE_ConstPointer_refParam */
  {
    retVal = olderSector;
  }
  /* #30 Return NULL_PTR in case the input address cannot be assigned to any sector */
  else
  {
    retVal = NULL_PTR;
  }

  return retVal;
}



/**********************************************************************************************************************
 * Fee_UpdateLUT
 ******************************************************************************************************************** */
/*!
 * Internal comment removed.
 *
 *
 *
 *
  */
FEE_INTERN_FUNC(void) Fee_Lut_UpdateLut(Fee_LookUpTableContextRefType self,Fee_ChunkInfoConstRefType chunk)
{

  /* #10 Check if update is allowed  */
  if (Fee_Lut_IsAccessAllowed(self) == FEE_TRUE) /* PRQA S 1881 */  /* MD_FEE_10.4_compare_FeeBool_to_enum */ /* SBSW_FEE_ConstPointer_refParam */
  {
    /* #20 Calculate absolute ChunkHeaderAddress  */
    Fee_LengthType chunkHeaderAddress = Fee_ChunkInfo_GetBaseAddress(chunk) + chunk->Sector_pt->cfg_pt->logicalRange_t.baseAddress_t; /* SBSW_FEE_ConstPointer_refParam */

    /* #25 Update LookUpTable with calculated value  */
    Fee_Lut_UpdateLutEntry(self, chunkHeaderAddress); /* SBSW_FEE_PointerWrite_refParam */
  }
}


/**********************************************************************************************************************
 * Fee_Lut_ResetLutEntry
 ******************************************************************************************************************** */
/*!
 * Internal comment removed.
 *
 *
 *
 *
  */
/* PRQA S 3673 1  */ /* MD_FEE_8.13_PointerParam_IS_modified  */
FEE_INTERN_FUNC(void) Fee_Lut_ResetLut(Fee_LookUpTableContextRefType self)
{
  /* #10 Check if update is allowed  */
  if (Fee_Lut_IsAccessAllowed(self) == FEE_TRUE) /* PRQA S 1881 */  /* MD_FEE_10.4_compare_FeeBool_to_enum */ /* SBSW_FEE_ConstPointer_refParam */
  {
    Fee_Lut_UpdateLutEntry(self, 0); /* SBSW_FEE_PointerWrite_refParam */

  }

}


/**********************************************************************************************************************
 * Fee_Lut_IsChunkAddressPlausible
 ******************************************************************************************************************** */
/*!
 *
 * Internal comment removed.
 *
 *
 *
  */
FEE_INTERN_FUNC(Fee_Bool) Fee_Lut_IsChunkAddressPlausible(Fee_LookUpTableContextConstRefType self,Fee_LengthType chunkAddress)
{
  /* #10 Check if ChunkAddress is aligned  */
  Fee_Bool isAligned = (chunkAddress %  ((Fee_LengthType)1u << self->Partition_pt->currentSector_pt->cfg_pt->alignments_t.address_u8)) == 0uL; /* PRQA S 4403 */ /* MD_FEE_10.3_essentially_boolean_type */
  Fee_Bool isContainedInSector = FEE_FALSE; /* PRQA S 4423 */ /* MD_FEE_10.3_essentially_enum_type */

  /* #20 Check if absolute ChunkAddress is within current partition's sector range  */
  if (Fee_Lut_GetSectorByAddress(self->Partition_pt, chunkAddress) != NULL_PTR) /* SBSW_FEE_ConstPointer_refParam */
  {
    isContainedInSector = FEE_TRUE; /* PRQA S 4423 */ /* MD_FEE_10.3_essentially_enum_type */
  }

  return (isContainedInSector && isAligned); /* PRQA S 4403, 4538, 4115 */ /* MD_FEE_10.3_essentially_boolean_type, MD_FEE_10.1_Essentially_boolean_in_logical_expressions,MD_FEE_10.1_Essentially_boolean_in_logical_expressions */
}


/**********************************************************************************************************************
 * Fee_Lut_InitRam
 ******************************************************************************************************************** */
/*!
 * Internal comment removed.
 *
 *
 *
 *
  */
FEE_INTERN_FUNC(void) Fee_Lut_InitRam(Fee_PartitionConfigRefType partition)
{
  /*#10 Only init ram if lookup table is enabled for the partition */
  if (partition->lutEnabled == TRUE) /* COV_FEE_PART_CONFIG  */ /*Might not be tested if only one partition is enabled and Lut is enabled */
  {
    /*#11 Loop over all entries and set them to 0 separately */
    uint8 lutCfgIndex = partition->lutCfgIdx;
    uint16 entriesInRamLut = (uint16) (Fee_LookUpTableConfig[lutCfgIndex].NrBlocks * Fee_LookUpTableConfig[lutCfgIndex].NrEntries);
    uint16 index;
    for (index = Fee_LookUpTableConfig[lutCfgIndex].LutEntriesStartIdx;
         index < (entriesInRamLut + Fee_LookUpTableConfig[lutCfgIndex].LutEntriesStartIdx); index++)
    {
      Entries[index].ChunkHeaderAddress = 0; /* SBSW_FEE_Lut_Block_Ram_Entries */
    }
    /*#12 Initialize modified flags */
    ModifiedFlags[Fee_LookUpTableConfig[lutCfgIndex].LutModifedFlagIdx] = 0; /* SBSW_FEE_Lut_ModifiedFlags_Access */
  }
}


/**********************************************************************************************************************
 * Fee_Lut_IsLutEnabled
 ******************************************************************************************************************** */
/*!
 * Internal comment removed.
 *
 *
  */
FEE_INTERN_FUNC(Fee_Bool) Fee_Lut_IsLutEnabled(Fee_PartitionConstRefType part)
{
    /* #10 If lut is not configured, return false */
    return  part->cfg_pt->lutEnabled == TRUE; /* PRQA S 4403 */ /* MD_FEE_10.3_essentially_boolean_type */
}


/**********************************************************************************************************************
 * Fee_Lut_IsAccessAllowed
 ******************************************************************************************************************** */
/*!
 * Internal comment removed.
 *
 *
  */
FEE_INTERN_FUNC(Fee_Bool) Fee_Lut_IsAccessAllowed(Fee_LookUpTableContextConstRefType self)
{
  /* #10 If LUT context is initialized, lut feature is enabled and block config is not null, update is allowed  */
  /* lut context is expected to be almost always initialized (exception: reading both sector headers fails) */
  return ((self->Partition_pt) != NULL_PTR) && (Fee_Lut_IsLutEnabled(self->Partition_pt)) /* SBSW_FEE_ConstPointer_refParam */  /* PRQA S 3415, 4403, 4558, 4115  */ /* MD_FEE_13.5_queries, MD_FEE_10.3_essentially_boolean_type, MD_FEE_10.1_Essentially_boolean_in_logical_expressions, MD_FEE_10.1_Essentially_boolean_in_logical_expressions  */
      && ((self->BlockConfig_pt) != NULL_PTR); /* COV_FEE_PART_CONFIG  */ /* PRQA S 3415  */ /* MD_FEE_13.5_queries  */
}



/**********************************************************************************************************************
 * Fee_Lut_PrepareProcessParameter
 ******************************************************************************************************************** */
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
  */
FEE_INTERN_FUNC(void) Fee_Lut_PrepareProcessParameter(Fee_PartitionConstRefType jobPartition_pt, uint16 blockIndex,
                                                     Fee_Bool isWriteJob, struct Fee_tagBlockSpecificJob *jobParams)
{
  /* #10 This function is used to prepare both reading and writing of LookUpTable blocks  */
  uint8 lutCfgIndex = jobPartition_pt->cfg_pt->lutCfgIdx;
  uint16 indexInRam = Fee_Lut_GetLutBlocksStartIndexInRam(blockIndex, lutCfgIndex);
  Fee_BlockCfgDatasetPair block;

  /* #20 Get block configuration related to input blockIndex  */
  block.config = &Fee_BlockConfig_at[Fee_LookUpTableConfig[lutCfgIndex].LutRomBlockStartIdx + blockIndex];
  block.dataIndex = 0u;

  /* #30 Distinguish between read and write jobs regarding job parameters i.e. buffer, length etc.  */
  if (isWriteJob == FEE_TRUE) /* PRQA S 1881 */  /* MD_FEE_10.4_compare_FeeBool_to_enum */
  {
    jobParams->add.write.writeBuffer = (Fee_UserDataPtrType) &Entries[indexInRam];/* SBSW_FEE_PointerWrite_refParam */ /* PRQA S 0310  */ /* MD_FEE_11.4_lut_pointer_cast  */
  }
  else
  {
    jobParams->add.read.readBuffer = (Fee_UserDataPtrType) &Entries[indexInRam]; /* SBSW_FEE_PointerWrite_refParam */ /* PRQA S 0310  */ /* MD_FEE_11.4_lut_pointer_cast  */
    jobParams->add.read.length = block.config->BlockPayloadSize_u16; /* SBSW_FEE_PointerWrite_refParam */
    jobParams->add.read.offset = 0; /* SBSW_FEE_PointerWrite_refParam */
  }
  /*#40 Assign block reference and set broken link flag to false */
  jobParams->blockRef = block; /* SBSW_FEE_PointerWrite_refParam */
  Fee_Globals_t.InternalFlags_t.brokenLink = FALSE;
}





/**********************************************************************************************************************
 * Fee_Lut_ValidateBlockContent
 ******************************************************************************************************************** */
/*!
 *
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
FEE_INTERN_FUNC(void) Fee_Lut_ValidateBlockContent(Fee_PartitionConstRefType jobPartition_pt, Fee_SectorConstRefType lutBlockSector,
                                                   Fee_LookUpTableSyncDataConstRefType syncContext,Fee_Bool validInstanceFound)
{
  uint8 lutCfgIndex = jobPartition_pt->cfg_pt->lutCfgIdx;
  /*#10 If no valid instance was found, there is noting else to do but to reset all entries of current lut block */
  if (!validInstanceFound) /* PRQA S 4538, 4116 */  /* MD_FEE_10.1_Essentially_boolean_in_logical_expressions, MD_FEE_10.1_Essentially_boolean_in_logical_expressions */
  {
    Fee_Lut_ResetLutBlocksRamEntries(syncContext, lutCfgIndex); /* SBSW_FEE_ConstPointer_refParam */
  }
  /*#11 Otherwise continue with validation */
  else
  {

    /*#20 If lut block was read from new sector there is nothing to check here  */
    /*#21 Otherwise loop over all entries of current lut block to validate them */
    if (lutBlockSector != jobPartition_pt->currentSector_pt)
    {
      /* #30 Get current Lut blocks start and end index in RAM */
      uint16 loopIndex;
      Fee_SectorConstRefType entrySector;
      Fee_LookUpTableEntryPtrType currentEntry;
      uint16 indexInRam = Fee_Lut_GetLutBlocksStartIndexInRam(syncContext->LookUpTableSyncBlockIndex, lutCfgIndex); /* SBSW_FEE_ConstPointer_refParam */
      uint16 endIndex = (uint16)(indexInRam + ((Fee_LookUpTableConfig[lutCfgIndex].NrEntries) - 1u));

      /*#40 Loop over all entries specified by indexes and reset them if they are pointing to the future */
      for (loopIndex = indexInRam; loopIndex <= endIndex; loopIndex++)
      {
        /*#41 Get lut entry */
        currentEntry = &Entries[loopIndex];

        /*#42 Reset lut entry if it points to new sector (i.e. entry sector and lutBlockSector differ */
        entrySector = Fee_Lut_GetSectorByAddress(jobPartition_pt,  currentEntry->ChunkHeaderAddress); /* SBSW_FEE_ConstPointer_refParam */
        if(entrySector != lutBlockSector)
        {
          currentEntry->ChunkHeaderAddress = 0; /* SBSW_FEE_Lut_Block_Ram_Entries */
        }
      }
    }
  }
}

/**********************************************************************************************************************
 * Fee_Lut_PrepareSyncContextForNextBlock
 ******************************************************************************************************************** */
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
  */
FEE_INTERN_FUNC(Fee_Bool) Fee_Lut_PrepareSyncContextForNextBlock(Fee_LookUpTableSyncDataRefType syncContext,
                                                                 Fee_ConfigRefTypeInternal config_pt)
{
  Fee_Bool retValue = FEE_TRUE; /* PRQA S 4423 */ /* MD_FEE_10.3_essentially_enum_type */
  uint16 nextLutBlockIndex;
  uint8 nextLutPartitionIndex;

  /*#10 Continue following procedure until a valid block index was found */
  /*#20 If next sync block index is valid (i.e. it is not -1), assign it to context's block index and return true */
  /*#30 Otherwise get next sync partition index */
  /*#31 If next sync partition index is not valid (i.e. -1), return false and reset sync context,
   * because there is no further partition to be synced */
  nextLutBlockIndex = Fee_Lut_GetNextSyncBlockIndex(syncContext, /* SBSW_FEE_PointerWrite_refParam */
                                                     config_pt); /* SBSW_FEE_ConstPointer_refParam */
  while (nextLutBlockIndex == FEE_LUT_BLOCK_INVALID_INDEX)
  {
    nextLutPartitionIndex = Fee_Lut_GetNextSyncPartitionIndex(syncContext,/* SBSW_FEE_PointerWrite_refParam */
                                                              config_pt); /* SBSW_FEE_ConstPointer_refParam */
    if (nextLutPartitionIndex == FEE_LUT_PARTITION_INVALID_INDEX) /* COV_FEE_PART_CONFIG  */ /*Might not be tested if only one partition is enabled and Lut is enabled */
    {
      syncContext->LookUpTablePartitionIndex = FEE_LUT_PARTITION_INVALID_INDEX; /* SBSW_FEE_PointerWrite_refParam */
      syncContext->LookUpTableSyncBlockIndex = FEE_LUT_BLOCK_INVALID_INDEX; /* SBSW_FEE_PointerWrite_refParam */
      retValue = FEE_FALSE; /* PRQA S 4423 */ /* MD_FEE_10.3_essentially_enum_type */
      break;
    }

    syncContext->LookUpTablePartitionIndex = nextLutPartitionIndex; /* SBSW_FEE_PointerWrite_refParam */
    nextLutBlockIndex = Fee_Lut_GetNextSyncBlockIndex(syncContext, /* SBSW_FEE_PointerWrite_refParam */
                                                      config_pt); /* SBSW_FEE_ConstPointer_refParam */
  }
  if (retValue == FEE_TRUE) /* PRQA S 1881 */  /* MD_FEE_10.4_compare_FeeBool_to_enum */
  {
    syncContext->LookUpTableSyncBlockIndex = nextLutBlockIndex; /* SBSW_FEE_PointerWrite_refParam */
  }
  return retValue;
}



/**********************************************************************************************************************
 * Fee_Lut_GetNextSyncPartitionIndex
 ******************************************************************************************************************** */
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
  */
FEE_INTERN_FUNC(uint8) Fee_Lut_GetNextSyncPartitionIndex(Fee_LookUpTableSyncDataRefType syncContext,Fee_ConfigRefTypeInternal config_pt)
{

  uint8 retValue = FEE_LUT_PARTITION_INVALID_INDEX;
  uint8 lutCfgIndex;
  Fee_PartitionConstRefType partition_pt;

  /* #10 Start search for next partition with next partition that follows (LookUpTablePartitionIndex points to last checked partition)  */
  sint8 currPartitionIndex = (sint8)(((sint8)syncContext->LookUpTablePartitionIndex) - 1);

  /*#20 Check in descending order if any of FEE's partitions  has lut enabled and is initialized and writable */

  for(;currPartitionIndex >= 0; currPartitionIndex--)
  { /*#21 Return partition index, if such partition was found */
    partition_pt = Fee_Partition_getFromConfigById(config_pt,(uint8)currPartitionIndex); /* SBSW_FEE_PartitionConfig  */
    lutCfgIndex = partition_pt->cfg_pt->lutCfgIdx;
    if((partition_pt->cfg_pt->lutEnabled == TRUE) &&          /* PRQA S 3415  */ /* MD_FEE_13.5_queries  */
       (Fee_Partition_getNewestSector(partition_pt) != NULL_PTR) && /* SBSW_FEE_ConstPointer_refParam */ /* PRQA S 3415, 4558, 4115  */ /* MD_FEE_13.5_queries, MD_FEE_10.1_Essentially_boolean_in_logical_expressions, MD_FEE_10.1_Essentially_boolean_in_logical_expressions  */
       (Fee_Partition_isWriteable(partition_pt)))                   /* SBSW_FEE_ConstPointer_refParam */ /* PRQA S 3415  */ /* MD_FEE_13.5_queries  */ /* COV_FEE_PART_CONFIG  */ /*Might not be tested if only one partition is enabled and Lut is enabled */
    {
      /*#22 Set contexts block index to its maximum + 1 in order to start block search with correct block */
      syncContext->LookUpTableSyncBlockIndex = Fee_LookUpTableConfig[lutCfgIndex].NrBlocks; /* SBSW_FEE_PointerWrite_refParam */
      retValue = (uint8)currPartitionIndex;
      break;
    }
  }

  /*#30 Return invalid index, if no partition was found */
  return retValue;
}


/**********************************************************************************************************************
 * Fee_Lut_ResetModifiedFlag
 ******************************************************************************************************************** */
/*!
 * Internal comment removed.
 *
 *
 *
  */
FEE_INTERN_FUNC(void) Fee_Lut_ResetModifiedFlag(Fee_PartitionConfigRefType partCfg, Fee_LookUpTableSyncDataConstRefType syncContext)
{
  /*#10 Get modified flags and lut block number which equals bit number */
  uint8 lutCfgIndex = partCfg->lutCfgIdx;
  uint16 lutBlockNumber = syncContext->LookUpTableSyncBlockIndex;

  /*#20 Reset the corresponding flag */
  /*Shift is safe. Validator assures that there are max 32 lut blocks, so max lut block number is 31 */
  uint32 mask = ~(1uL << lutBlockNumber);

  ModifiedFlags[Fee_LookUpTableConfig[lutCfgIndex].LutModifedFlagIdx] &= mask; /* SBSW_FEE_Lut_ModifiedFlags_Access */
}

#define FEE_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087  */ /* MD_MSR_19.1  */
#endif
/**********************************************************************************************************************
 *  END OF FILE: Fee_LookUpTable.c
 ******************************************************************************************************************** */
