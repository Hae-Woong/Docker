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
/*!        \file  Sd_Priv.h
 *        \brief  Service Discovery source file
 *
 *      \details  Types definitions of Service Discovery (SD)
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined (SD_PRIV_H)
# define SD_PRIV_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Sd.h"
# if (STD_ON == SD_DEM_EVENTS_CONFIGURED)
#  include "Dem.h"
# endif /* SD_DEM_EVENTS_CONFIGURED */
# include "SchM_Sd.h"

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/
# define SD_START_SEC_VAR_CLEARED_UNSPECIFIED
# include "Sd_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

extern P2CONST(Sd_ConfigType, SD_VAR_CLEARED, SD_PBCFG) Sd_ConfigDataPtr;

# define SD_STOP_SEC_VAR_CLEARED_UNSPECIFIED
# include "Sd_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
*  GLOBAL FUNCTION MACROS
*********************************************************************************************************************/
# define SD_BEGIN_CRITICAL_SECTION() SchM_Enter_Sd_SD_EXCLUSIVE_AREA_0()
# define SD_END_CRITICAL_SECTION() SchM_Exit_Sd_SD_EXCLUSIVE_AREA_0()

# if ( SD_MULTI_PARTITION == STD_ON ) /* COV_SD_QM_FEATURE_MULTI_PARTITION */
#  define SD_BEGIN_CRITICAL_SECTION_MEASUREMENT() SchM_Enter_Sd_SD_EXCLUSIVE_AREA_MULTI_PARTITION()
#  define SD_END_CRITICAL_SECTION_MEASUREMENT()   SchM_Exit_Sd_SD_EXCLUSIVE_AREA_MULTI_PARTITION()
# else
#  define SD_BEGIN_CRITICAL_SECTION_MEASUREMENT() SchM_Enter_Sd_SD_EXCLUSIVE_AREA_0()
#  define SD_END_CRITICAL_SECTION_MEASUREMENT()   SchM_Exit_Sd_SD_EXCLUSIVE_AREA_0()
# endif /* SD_MULTI_PARTITION == STD_ON */

/* PRQA S 3453 FCT_MACROS */ /* MD_MSR_FctLikeMacroDir4.9 */

/* GET macros */
# define SD_GET_UINT8(PduInfoPtr, Offset, Variable)      (Variable) =            ((uint8)((PduInfoPtr)->SduDataPtr)[(Offset)])

# define SD_GET_UINT16(PduInfoPtr, Offset, Variable)     (Variable) = ((uint16)(((uint16)(((PduInfoPtr)->SduDataPtr)[(Offset)     ]) << 8u) | \
                                                                                ((uint16)(((PduInfoPtr)->SduDataPtr)[(Offset) + 1u]))))

# define SD_GET_UINT24(PduInfoPtr, Offset, Variable)     ((Variable) = (uint32)(((uint32)(((PduInfoPtr)->SduDataPtr)[(Offset)     ]) << 16u) | \
                                                                                ((uint32)(((PduInfoPtr)->SduDataPtr)[(Offset) + 1u]) <<  8u) | \
                                                                                ((uint32)(((PduInfoPtr)->SduDataPtr)[(Offset) + 2u]))))

# define SD_GET_UINT32(PduInfoPtr, Offset, Variable)     (Variable) = ((uint32)(((uint32)(((PduInfoPtr)->SduDataPtr)[(Offset)     ]) << 24u) | \
                                                                                ((uint32)(((PduInfoPtr)->SduDataPtr)[(Offset) + 1u]) << 16u) | \
                                                                                ((uint32)(((PduInfoPtr)->SduDataPtr)[(Offset) + 2u]) <<  8u) | \
                                                                                ((uint32)(((PduInfoPtr)->SduDataPtr)[(Offset) + 3u]))))

# define SD_GET_OPTION_TYPE(PduInfoPtr, OptionOffset, Variable)  ((Variable) = (Sd_OptionType)(PduInfoPtr)->SduDataPtr[(OptionOffset) + SD_OPTION_LENGTH_SIZE])

# define SD_GET_ENTRY_TYPE(PduInfoPtr, EntryOffset, Variable)  ((Variable) = (Sd_EntryType)((PduInfoPtr)->SduDataPtr[(EntryOffset)]))

# define SD_GET_L4PROTOCOL_IPV4_TYPE(PduInfoPtr, EntryOffset, Variable)  ((Variable) = (Sd_L4ProtocolType)(PduInfoPtr)->SduDataPtr[(EntryOffset) + SD_OPTION_IPV4_L4PROTO_OFFSET])

# define SD_GET_L4PROTOCOL_IPV6_TYPE(PduInfoPtr, EntryOffset, Variable)  ((Variable) = (Sd_L4ProtocolType)(PduInfoPtr)->SduDataPtr[(EntryOffset) + SD_OPTION_IPV6_L4PROTO_OFFSET])

# define SD_GET_NROFOPT(PduInfoPtr, EntryOffset, Nr1stOptions, Nr2ndOptions) \
          { \
            uint8 nrOfOpt; \
            SD_GET_UINT8((PduInfoPtr), (EntryOffset) + SD_ENTRY_NROFOPTS_OFFSET, nrOfOpt); \
            (Nr1stOptions) = (uint8)(nrOfOpt >> 4u); \
            (Nr2ndOptions) = (uint8)(nrOfOpt & 0x0Fu); \
          }

# define Sd_CheckIfTimerIsInvalid(TIMER)                     ((SD_INV_TIMER_VALUE_32BIT == (TIMER).Seconds) && (SD_INV_TIMER_VALUE_16BIT == (TIMER).Milliseconds))
# define Sd_CheckIfTimerIsZero(TIMER)                        ((0u == (TIMER).Seconds) && (0u == (TIMER).Milliseconds))

# define Sd_EventHandlerUsesUdp(EVENTHANDLERIDX, PARTITIONIDX)             Sd_IsEventHandlerUdpUsedOfEventHandler(EVENTHANDLERIDX, PARTITIONIDX)
# define Sd_EventHandlerUsesTcp(EVENTHANDLERIDX, PARTITIONIDX)             Sd_IsEventHandlerTcpUsedOfEventHandler(EVENTHANDLERIDX, PARTITIONIDX)
# define Sd_EventHandlerUsesMulticast(EVENTHANDLERIDX, PARTITIONIDX)       Sd_IsEventHandlerMulticastUsedOfEventHandler(EVENTHANDLERIDX, PARTITIONIDX)

# define Sd_ConsumedEventGroupUsesUdp(EVENTGROUPIDX, PARTITIONIDX)         Sd_IsRoutingGroupUdpActRefUsedOfConsumedEventgroup(EVENTGROUPIDX, PARTITIONIDX)
# define Sd_ConsumedEventGroupUsesTcp(EVENTGROUPIDX, PARTITIONIDX)         Sd_IsRoutingGroupTcpActRefUsedOfConsumedEventgroup(EVENTGROUPIDX, PARTITIONIDX)
# define Sd_ConsumedEventGroupUsesMulticast(EVENTGROUPIDX, PARTITIONIDX)   Sd_IsRoutingGroupMulticastActRefUsedOfConsumedEventgroup(EVENTGROUPIDX, PARTITIONIDX)

# define Sd_ServerTcpLocalPortUnusedOrValid(SERVERIDX, PARTITIONIDX)       ((!Sd_IsTcpSoConIdUsedOfServer(SERVERIDX, PARTITIONIDX)) || \
                                                                            (Sd_GetLocalPortOfSoConGrpDyn(Sd_GetSoConGrpDynTcpIdxOfServer(SERVERIDX, PARTITIONIDX), PARTITIONIDX) != 0u))
# define Sd_ServerUdpLocalPortUnusedOrValid(SERVERIDX, PARTITIONIDX)       ((!Sd_IsUdpSoConUsedOfServer(SERVERIDX, PARTITIONIDX)) || \
                                                                            (Sd_GetLocalPortOfSoConGrpDyn(Sd_GetSoConGrpDynUdpIdxOfServer(SERVERIDX, PARTITIONIDX), PARTITIONIDX) != 0u))

/* PRQA L:FCT_MACROS */
/* PRQA L:BRACED_MACROS */

/**********************************************************************************************************************
*  LOCAL DATA TYPES AND STRUCTURES
*********************************************************************************************************************/
/* This type defines the general function API which can be used to access values. */
typedef P2FUNC(uint16, SD_CODE, Sd_ServiceIdGetterFctType)  ( \
  uint16 Idx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define SD_START_SEC_CODE
# include "Sd_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Sd_MainFunctionPartition()
 *********************************************************************************************************************/
/*! \brief       Periodic processes for one partition.
 *  \details     -
 *  \param[in]   PartitionIdentifierIdx Partition identification index.
 *                                      [range: PartitionIdentifierIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, SD_CODE) Sd_MainFunctionPartition(
  Sd_SizeOfPartitionIdentifiersType PartitionIdentifierIdx);

# define SD_STOP_SEC_CODE
# include "Sd_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_MemMap */

#endif /* SD_PRIV_H */

/**********************************************************************************************************************
 *  END OF FILE: Sd_Priv.h
 *********************************************************************************************************************/
