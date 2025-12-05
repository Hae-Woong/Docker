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
/*!        \file  SoAd_BestMatch.c
 *        \brief  Socket Adaptor source file
 *
 *      \details  Vector static code implementation for AUTOSAR Socket Adaptor sub-component BestMatch.
 *
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

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "SoAd_Priv.h"
#include "SoAd_SoCon.h"
#include "SoAd_BestMatch_Int.h"
#include "SoAd_RemoteAddr_Int.h"
#include "SoAd_Util.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#if !defined (SOAD_LOCAL) /* COV_SOAD_COMPATIBILITY */
# define SOAD_LOCAL static
#endif /* !defined (SOAD_LOCAL) */

#if !defined (SOAD_LOCAL_INLINE) /* COV_SOAD_COMPATIBILITY */
# define SOAD_LOCAL_INLINE LOCAL_INLINE
#endif /* !defined (SOAD_LOCAL_INLINE) */

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define SOAD_START_SEC_CODE
#include "SoAd_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  SoAd_BestMatch_GetPrio()
 *********************************************************************************************************************/
/*! \brief      Returns the best match algorithm priority of a socket connection.
 *  \details    -
 *  \param[in]  SoConIdx          Socket connection index.
 *                                [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]  PartitionIdx      Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]  SockAddrPtr       Pointer to socket address used to perform best match algorithm.
 *                                [Points to one of the following structs depending on configured IP address
 *                                version of parameter SoConIdx:
 *                                  - SoAd_SockAddrInetType for IPv4
 *                                  - SoAd_SockAddrInet6Type for IPv6]
 *  \return     Priority          Best match algorithm priority of socket connection.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE for different socket connection identifiers, FALSE for same socket connection identifier.
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(SoAd_BestMatchPrioType, SOAD_CODE) SoAd_BestMatch_GetPrio(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) SockAddrPtr);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  SoAd_BestMatch_GetPrio()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
SOAD_LOCAL_INLINE FUNC(SoAd_BestMatchPrioType, SOAD_CODE) SoAd_BestMatch_GetPrio(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) SockAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
#if ( SOAD_IPV6 == STD_ON )
  SoAd_SockAddrInetXType currentRemAddr = { 0u, 0u, { 0u, 0u, 0u, 0u } };
  SoAd_SockAddrInetXType ipWildcard = { 0u, 0u, { 0u, 0u, 0u, 0u } };
  SoAd_DomainType        domain = (SoAd_IsRemAddrIpV6UsedOfSoCon(SoConIdx, PartitionIdx)) ?
    SOAD_AF_INET6 : SOAD_AF_INET;
#else
  SoAd_SockAddrInetXType currentRemAddr = { 0u, 0u, { 0u } };
  SoAd_SockAddrInetXType ipWildcard = { 0u, 0u, { 0u } };
  SoAd_DomainType        domain = SOAD_AF_INET;
#endif /* SOAD_IPV6 == STD_ON */
  /* PRQA S 310, 3305 1 */ /* MD_SoAd_PointerCastOnStruct */
  const uint16           dynPort = (SOAD_A_P2CONST(SoAd_SockAddrInetXType)SockAddrPtr)->port;
  boolean                ipIsEqual = TRUE;
  boolean                ipIsWildcard = TRUE;
  SoAd_BestMatchPrioType bestMachPrio = SOAD_BEST_MATCH_NONE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if remote address has no "not set" values. */
  if ( (SoAd_RemoteAddr_GetState(SoConIdx, PartitionIdx) & SOAD_SOCON_MASK_NOT) == 0u )
  {
    /* #20 Retrieve remote address. */
    /* PRQA S 310 1 */ /* MD_SoAd_PointerCastOnStruct */
    SoAd_RemoteAddr_Get(SoConIdx, PartitionIdx, SOAD_A_P2VAR(SoAd_SockAddrType)&currentRemAddr);                       /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */

    /* #30 Check if IP address is equal */
    /* PRQA S 310, 3305 2 */ /* MD_SoAd_PointerCastOnStruct */
    ipIsEqual = (SoAd_Util_CompareIpAddr(domain, (SOAD_A_P2CONST(SoAd_SockAddrInetXType)SockAddrPtr)->addr,            /* SBSW_SOAD_SOCK_ADDR_CAST_POINTER_STRUCT_ELEMENT_FORWARD */
      (&currentRemAddr)->addr ) == E_OK ) ? TRUE : FALSE;

    /* #40 Check if IP address is wildcard. */
    ipIsWildcard = (SoAd_Util_CompareIpAddr(domain, (&ipWildcard)->addr, (&currentRemAddr)->addr) == E_OK ) ?          /* SBSW_SOAD_CONST_POINTER_FORWARD */
      TRUE : FALSE;

    /* #50 Calculate best match priority. */
    if ( (ipIsEqual == TRUE) && (currentRemAddr.port == dynPort) )
    {
      bestMachPrio = SOAD_BEST_MATCH_IP_MATCH_PORT_MATCH;
    }
    else if ( (ipIsEqual == TRUE) && (currentRemAddr.port == SOAD_PORT_ANY) )
    {
      bestMachPrio = SOAD_BEST_MATCH_IP_MATCH_PORT_ANY;
    }
    else if ( (ipIsWildcard == TRUE) && (currentRemAddr.port == dynPort) )
    {
      bestMachPrio = SOAD_BEST_MATCH_IP_ANY_PORT_MATCH;
    }
    else if ( (ipIsWildcard == TRUE) && (currentRemAddr.port == SOAD_PORT_ANY) )
    {
      bestMachPrio = SOAD_BEST_MATCH_IP_ANY_PORT_ANY;
    }
    else
    {
      /* Nothing to do. */
    }
  }

  return bestMachPrio;
} /* SoAd_BestMatch_GetPrio() */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/* ----- Functions ----------------------------------------------------- */

/**********************************************************************************************************************
 *  SoAd_BestMatch_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_BestMatch_Init(
  SoAd_SizeOfInstanceType InstanceIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reset best match list size. */
  SoAd_SetBestMatchListSizeOfInstanceDyn(InstanceIdx, 0u, PartitionIdx);                                               /* SBSW_SOAD_PARAMETER_IDX */

} /* SoAd_BestMatch_Init() */

/**********************************************************************************************************************
 *  SoAd_BestMatch_DeriveList()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, SOAD_CODE) SoAd_BestMatch_DeriveList(
  SoAd_BestMatch_SoConStartEndIdxType SoConStartEndIdxStruct,
  boolean ConsiderReconnectOnly,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) SockAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SoConIterType                   soConIter; /* Not initialized: For-loop counter. */
  SoAd_SizeOfSoConType                 soConIdxListSize = 0u;
  SoAd_SizeOfBestMatchSoConIdxListType bestMatchSoConIdxListStartIdx = SoAd_GetBestMatchSoConIdxListStartIdxOfInstance(
    SoConStartEndIdxStruct.InstanceIdx, SoConStartEndIdxStruct.PartitionIdx);
  SoAd_SizeOfBestMatchSoConIdxListType bestMatchSoConIdxListEndIdx = SoAd_GetBestMatchSoConIdxListEndIdxOfInstance(
    SoConStartEndIdxStruct.InstanceIdx, SoConStartEndIdxStruct.PartitionIdx);
  SoAd_BestMatchPrioType               bestMatchPrio = SOAD_BEST_MATCH_NONE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reset best match list size. */
  SoAd_SetBestMatchListSizeOfInstanceDyn(SoConStartEndIdxStruct.InstanceIdx, 0u, SoConStartEndIdxStruct.PartitionIdx); /* SBSW_SOAD_PARAMETER_IDX */

  /* #20 Check if the best match socket connection index list is valid. */
  if ( (SoConStartEndIdxStruct.SoConEndIdx - SoConStartEndIdxStruct.SoConStartIdx) <=
    (bestMatchSoConIdxListEndIdx - bestMatchSoConIdxListStartIdx) )
  {
    /* #30 Iterate over all socket connections. */
    for ( soConIter = SoConStartEndIdxStruct.SoConStartIdx; soConIter < SoConStartEndIdxStruct.SoConEndIdx;
      soConIter++ )
    {
      const SoAd_SizeOfSoConType soConIdxTmp = (SoAd_SizeOfSoConType)soConIter;

      /* #40 Check if socket connection mode reconnect is considered only. */
#if ( SOAD_TCP == STD_ON )
      /* PRQA S 3415 2 */ /* MD_SoAd_RightHandOperandWithoutSideEffects */
      if ( (ConsiderReconnectOnly == FALSE) ||
        (SoAd_SoCon_GetMode(soConIdxTmp, SoConStartEndIdxStruct.PartitionIdx) == SOAD_SOCON_RECONNECT) )
#else
      SOAD_DUMMY_STATEMENT(ConsiderReconnectOnly); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* SOAD_TCP == STD_ON */
      {
        /* #50 Get best match priority of current socket connection. */
        const SoAd_BestMatchPrioType bestMatchPrioTmp = SoAd_BestMatch_GetPrio(soConIdxTmp,
          SoConStartEndIdxStruct.PartitionIdx, SockAddrPtr);                                                           /* SBSW_SOAD_POINTER_FORWARD */

        /* #60 Store current socket connection if best match priority is higher than the highest priority up to now. */
        if ( bestMatchPrioTmp > bestMatchPrio )
        {
          bestMatchPrio = bestMatchPrioTmp;
          SoAd_SetSoConIdxOfBestMatchSoConIdxList(bestMatchSoConIdxListStartIdx, soConIdxTmp,                          /* SBSW_SOAD_CSL02_CSL05 */
            SoConStartEndIdxStruct.PartitionIdx);
          soConIdxListSize = 1u;

          /* #600 Stop iteration if best match with highest priority has been found (i.e. IP and port match). */
          if ( bestMatchPrio == SOAD_BEST_MATCH_IP_MATCH_PORT_MATCH )
          {
            break;
          }
        }
        /* #61 Store current socket connection additionally if best match priority is equal to the highest and not
         *     none. */
        else if ( (bestMatchPrioTmp == bestMatchPrio) && (bestMatchPrioTmp != SOAD_BEST_MATCH_NONE) )
        {
          SoAd_SetSoConIdxOfBestMatchSoConIdxList(bestMatchSoConIdxListStartIdx + soConIdxListSize, soConIdxTmp,       /* SBSW_SOAD_CALCULATED_CHECKED_CSL03_CSL05 */
            SoConStartEndIdxStruct.PartitionIdx);
          soConIdxListSize++;
        }
        /* #62 Continue without any action if best match priority is lower than the highest priority up to now. */
        else
        {
          /* Nothing to do. */
        }
      }
    }
  }

  /* #70 Set the length of the socket connection index list with the highest priorities. */
  SoAd_SetBestMatchListSizeOfInstanceDyn(SoConStartEndIdxStruct.InstanceIdx, soConIdxListSize,                         /* SBSW_SOAD_PARAMETER_IDX */
    SoConStartEndIdxStruct.PartitionIdx);
} /* SoAd_BestMatch_DeriveList() */

/**********************************************************************************************************************
 *  SoAd_BestMatch_GetNextSoCon()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_BestMatch_GetNextSoCon(
  SoAd_SizeOfInstanceType InstanceIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_SizeOfSoConType ListOffset,
  P2VAR(SoAd_SizeOfSoConType, AUTOMATIC, SOAD_APPL_VAR) SoConIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
   /* #10 Check if offset is in valid range. */
  if ( ListOffset < SoAd_GetBestMatchListSizeOfInstanceDyn(InstanceIdx, PartitionIdx) )
  {
    /* #20 Get next element. */
    const SoAd_SizeOfSoConType soConIdx = SoAd_GetSoConIdxOfBestMatchSoConIdxList(
        (SoAd_GetBestMatchSoConIdxListStartIdxOfInstance(InstanceIdx, PartitionIdx) + ListOffset), PartitionIdx);

    /* #30 Return the next socket connection index of the best match algorithm list. */
    *SoConIdxPtr = soConIdx;                                                                                           /* SBSW_SOAD_POINTER_WRITE */

    retVal = E_OK;
  }

  return retVal;
} /* SoAd_BestMatch_GetNextSoCon() */

#define SOAD_STOP_SEC_CODE
#include "SoAd_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_MemMap */

/* Justification for module-specific MISRA deviations:
 * see file SoAd.c
 */

 /**********************************************************************************************************************
 *  END OF FILE: SoAd_BestMatch.c
 *********************************************************************************************************************/
