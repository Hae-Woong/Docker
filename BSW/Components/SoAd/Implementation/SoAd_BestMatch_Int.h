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
/*!        \file  SoAd_BestMatch_Int.h
 *         \unit  BestMatch
 *        \brief  Internal header file of the BestMatch unit for AUTOSAR Socket Adaptor module.
 *      \details  Interface and type definitions of the BestMatch unit for module internal usage.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's main source file.
 *********************************************************************************************************************/

#if !defined (SOAD_BESTMATCH_INT_H)
# define SOAD_BESTMATCH_INT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "SoAd.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/* type used for best match algorithm */
typedef struct sSoAd_BestMatch_SoConStartEndIdxType
{
  SoAd_SizeOfInstanceType InstanceIdx;
  SoAd_PartitionConfigIdxType PartitionIdx;
  SoAd_SoConStartIdxOfSoConGrpType SoConStartIdx;
  SoAd_SoConEndIdxOfSoConGrpType SoConEndIdx;
} SoAd_BestMatch_SoConStartEndIdxType;

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define SOAD_START_SEC_CODE
# include "SoAd_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_MemMap */

/**********************************************************************************************************************
  SoAd_BestMatch_Init()
 *********************************************************************************************************************/
/*! \brief       Initializes best match list size.
 *  \details     -
 *  \param[in]   InstanceIdx  Socket connection index.
 *                            [range: InstanceIdx < SoAd_GetSizeOfInstance()]
 *  \param[in]   PartitionIdx Partition index.
 *                            [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   FALSE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_BestMatch_Init(
  SoAd_SizeOfInstanceType InstanceIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_BestMatch_DeriveList()
 *********************************************************************************************************************/
/*! \brief        Fills a global list of socket connection indexes with highest best match algorithm priority on a
 *                socket connection group.
 *  \details      SoConStartIdx and SoConEndIdx are summarized in a struct to identify all socket connections within a
 *                socket connection group.
 *  \param[in]    SoConStartEndIdxStruct Combines parameters to identify socket connection group and instance.
 *                                       [range: InstanceIdx < SoAd_GetSizeOfInstance(),
 *                                               PartitionIdx < size of SoAd_PCPartitionConfig,
 *                                               SoConStartIdx < SoAd_GetSizeOfSoCon(),
 *                                               SoConEndIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]    ConsiderReconnectOnly  Indicates if socket connections with state SOAD_SOCON_RECONNECT shall be
 *                                       considered only.
 *  \param[in]    SockAddrPtr            Pointer to socket address used to perform best match algorithm.
 *                                       [Points to one of the following structs depending on configured IP address
 *                                       version of parameter SoConIdx:
 *                                         - SoAd_SockAddrInetType for IPv4
 *                                         - SoAd_SockAddrInet6Type for IPv6]
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different socket connection identifiers, FALSE for same socket connection identifier.
 *  \note         This function must be called in a critical section.
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_BestMatch_DeriveList(
  SoAd_BestMatch_SoConStartEndIdxType SoConStartEndIdxStruct,
  boolean ConsiderReconnectOnly,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) SockAddrPtr);

/**********************************************************************************************************************
 *  SoAd_BestMatch_GetNextSoCon()
 *********************************************************************************************************************/
/*! \brief        Returns next socket connection index of the best match list using the ListOffset.
 *  \details      -
 *  \param[in]    InstanceIdx            Instance index.
 *                                       [range: InstanceIdx < SoAd_GetSizeOfInstance()]
 *  \param[in]    PartitionIdx           Partition index.
 *                                       [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    ListOffset             Offset that is applied to the best match list to return the corresponding
 *                                       connection.
 *  \param[out]   SoConIdxPtr            Pointer to socket connection index of next element in list.
 *  \return       E_OK                   Matching socket connection found and SoConIdxPtr is valid.
 *  \return       E_NOT_OK               No matching socket connection found and SoConIdxPtr is not set.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different socket connection indexes, FALSE for same socket connection index.
 *  \note         Is always called after SoAd_BestMatch_DeriveList() and within the same critical section.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SOAD_CODE) SoAd_BestMatch_GetNextSoCon(
  SoAd_SizeOfInstanceType InstanceIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_SizeOfSoConType ListOffset,
  P2VAR(SoAd_SizeOfSoConType, AUTOMATIC, SOAD_APPL_VAR) SoConIdxPtr);

# define SOAD_STOP_SEC_CODE
# include "SoAd_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */

#endif /* SOAD_BESTMATCH_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: SoAd_BestMatch_Int.h
 *********************************************************************************************************************/
