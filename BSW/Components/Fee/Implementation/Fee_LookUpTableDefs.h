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
 *         File:  Fee_LookUpTableDefs.h
 *    Component:  If_AsrIfFee
 *       Module:  Fee
 *    Generator:  -
 *
 *  Description:  The module Fee provides an abstraction from the device specific addressing scheme and
 *                segmentation. It also offers a virtual addressing scheme and segmentation as well as
 *                a virtually unlimited number of erase cycles to the upper layer.
 *
 *********************************************************************************************************************/

#ifndef FEE_LOOKUPTABLEDEF_H_
# define FEE_LOOKUPTABLEDEF_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Fee_IntBase.h"


/**********************************************************************************************************************
 *  TYPE DEFINITIONS
 *********************************************************************************************************************/


struct Fee_LookUpTableEntryType
{
  Fee_LengthType ChunkHeaderAddress;
};

typedef P2VAR(struct Fee_LookUpTableEntryType, AUTOMATIC, FEE_VAR_NOINIT) Fee_LookUpTableEntryPtrType;
typedef P2CONST(struct Fee_LookUpTableEntryType, AUTOMATIC, FEE_VAR_NOINIT) Fee_LookUpTableEntryConstPtrType;

typedef struct
{
  uint8  LutModifedFlagIdx; /*!< Index of ModifiedFlag in the corresponding Array*/
  uint16 LutEntriesStartIdx; /*!< Index of first entry belonging to this lut cfg within entries array*/
  uint16 LutRomBlockStartIdx; /*!< Index of first NvBlock (in order to store lut in flash) in Block Config array*/
  uint16 NrBlocks; /*!< Number of needed lookup table blocks*/
  uint16 NrEntries; /*!< Number of entries contained in one lookup table block*/
}Fee_LookUpTableConfigType;

typedef P2CONST(Fee_LookUpTableConfigType, AUTOMATIC, FEE_APPL_CONFIG) Fee_LookUpTableConfigPtrType;


#endif /* FEE_LOOKUPTABLEDEF_H_ */

/**********************************************************************************************************************
 *  END OF FILE: Fee_LookUpTableDefs.h
 *********************************************************************************************************************/
