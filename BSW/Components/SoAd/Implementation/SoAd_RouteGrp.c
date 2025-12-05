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
/*!        \file  SoAd_RouteGrp.c
 *        \brief  Socket Adaptor source file
 *
 *      \details  Vector static code implementation for AUTOSAR Socket Adaptor sub-component RouteGrp.
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

/* PRQA S 1881 EOF */ /* MD_MSR_AutosarBoolean */
/* PRQA S 4304 EOF */ /* MD_MSR_AutosarBoolean */

#define SOAD_ROUTE_GRP_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "SoAd_EventQueue.h"
#include "SoAd_RouteGrp.h"
#include "SoAd_Tx.h"
#include "SoAd_Priv.h"

#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
# include "Det.h"
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

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
#endif

#if !defined (SOAD_LOCAL_INLINE) /* COV_SOAD_COMPATIBILITY */
# define SOAD_LOCAL_INLINE LOCAL_INLINE
#endif

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
 *  SoAd_RouteGrp_CheckIfAllowedForMultiInstance()
 *********************************************************************************************************************/
/*! \brief        Checks if a routing group is allowed to be enabled considering the multi-instance use case.
 *  \details      The multi-instance use case requires that the routing group is not enabled on another socket
 *                connection of the socket connection group. Additionally, no other routing group must be enabled on
 *                the same socket connection if it is related to the socket route by another socket route destination.
 *  \param[in]    RouteGrpIdx     Routing group index.
 *                                [range: RouteGrpIdx < SoAd_GetSizeOfRouteGrp()]
 *  \param[in]    SoConIdx        Socket connection index.
 *                                [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \return       E_OK            Routing group can be enabled.
 *  \return       E_NOT_OK        Routing group must not be enabled.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_RouteGrp_CheckIfAllowedForMultiInstance(
  SoAd_SizeOfRouteGrpType RouteGrpIdx,
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_RouteGrp_CheckIfDisabledForOtherSoCons()
 *********************************************************************************************************************/
/*! \brief        Checks if a routing group is disabled on all socket connections of the requested socket connection
 *                related socket connection group.
 *  \details      -
 *  \param[in]    RouteGrpIdx     Routing group index.
 *                                [range: RouteGrpIdx < SoAd_GetSizeOfRouteGrp()]
 *  \param[in]    SoConIdx        Socket connection index.
 *                                [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \return       E_OK            Routing group is disabled on all socket connections.
 *  \return       E_NOT_OK        Routing group is enabled on at least one socket connection.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_RouteGrp_CheckIfDisabledForOtherSoCons(
  SoAd_SizeOfRouteGrpType RouteGrpIdx,
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_RouteGrp_CheckIfNoOtherRelatedIsEnabledOnSoCon()
 *********************************************************************************************************************/
/*! \brief        Checks if no other routing group is enabled on the requested socket connection if it is related to
 *                the socket route by another socket route destination.
 *  \details      -
 *  \param[in]    RouteGrpIdx     Routing group index.
 *                                [range: RouteGrpIdx < SoAd_GetSizeOfRouteGrp()]
 *  \param[in]    SoConIdx        Socket connection index.
 *                                [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \return       E_OK            No other related routing group is enabled.
 *  \return       E_NOT_OK        At least one related routing group is enabled on the socket connection.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_RouteGrp_CheckIfNoOtherRelatedIsEnabledOnSoCon(
  SoAd_SizeOfRouteGrpType RouteGrpIdx,
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_RouteGrp_IfTransmit()
 *********************************************************************************************************************/
/*! \brief        Transmits a routing group on all related PduRouteDests.
 *  \details      -
 *  \param[in]    RouteGrpIdx     Routing group index.
 *                                [range: RouteGrpIdx < SoAd_GetSizeOfRouteGrp()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[out]   ErrorIdPtr      Pointer to error identifier (set if error occurred).
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_RouteGrp_IfTransmit(
  SoAd_SizeOfRouteGrpType RouteGrpIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) ErrorIdPtr);

/**********************************************************************************************************************
 *  SoAd_RouteGrp_CheckTrigger()
 *********************************************************************************************************************/
/*! \brief        Checks if routing group on PduRouteDest is set to be triggered.
 *  \details      -
 *  \param[in]    RouteGrpIdx           Routing group index.
 *                                      [range: RouteGrpIdx < SoAd_GetSizeOfRouteGrp()]
 *  \param[in]    PduRouteDestIdx       PDU route destination index.
 *                                      [range: PduRouteDestIdx < SoAd_GetSizeOfPduRouteDest()]
 *  \param[in]    PartitionIdx          Partition index.
 *                                      [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[out]   RouteGrpSoConIdxPtr   Pointer to routing group on socket connection index.
 *                                      [range: *RouteGrpSoConIdxPtr < SoAd_GetSizeOfRouteGrpSoCon()]
 *  \return       TRUE                  Routing group shall be triggered and RouteGrpSoConIdxPtr is set.
 *  \return       FALSE                 Routing group shall not be triggered and RouteGrpSoConIdxPtr has not been
 *                                      modified.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(boolean, SOAD_CODE) SoAd_RouteGrp_CheckTrigger(
  SoAd_SizeOfRouteGrpType RouteGrpIdx,
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(SoAd_SizeOfRouteGrpSoConType, AUTOMATIC, SOAD_APPL_VAR) RouteGrpSoConIdxPtr);

/**********************************************************************************************************************
 *  SoAd_RouteGrp_IfTransmitGetNextPduData()
 *********************************************************************************************************************/
/*! \brief        Retrieves PDU data via trigger transmit to a trigger transmit buffer. If PDU data for PduRoute has
 *                been retrieved before (indicated by PduInfoPtr), trigger transmit is not called anymore.
 *  \details      -
 *  \param[in]    PduRouteIdx       PDU route index.
 *                                  [range: PduRouteDestIdx < SoAd_GetSizeOfPduRoute()]
 *  \param[in]    PartitionIdx      Partition index.
 *                                  [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[out]   PduInfoPtr        Pointer to retrieved PDU.
 *                                  [range: PduInfoPtr != NULL_PTR,
 *                                  PduInfoPtr->SduDataPtr == NULL_PTR to indicate PDU data has not been retrieved,
 *                                  PduInfoPtr->SduDataPtr != NULL_PTR to indicate PDU data has been retrieved already]
 *  \return       E_OK              PDU data has been retrieved successfully (or has been retrieved before).
 *  \return       E_NOT_OK          PDU data has not been retrieved successfully.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_RouteGrp_IfTransmitGetNextPduData(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr);

/**********************************************************************************************************************
 *  SoAd_RouteGrp_IfTransmitReleasePduData()
 *********************************************************************************************************************/
/*! \brief        Releases a reserved trigger transmit buffer of the PduRoute.
 *  \details      -
 *  \param[in]    PduRouteIdx       PDU route index.
 *  \param[in]    PartitionIdx      Partition index.
 *                                  [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_RouteGrp_IfTransmitReleasePduData(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  SoAd_RouteGrp_CheckIfAllowedForMultiInstance()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_RouteGrp_CheckIfAllowedForMultiInstance(
  SoAd_SizeOfRouteGrpType RouteGrpIdx,
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if routing group is disabled on all socket connections of the same socket connection group. */
  if ( SoAd_RouteGrp_CheckIfDisabledForOtherSoCons(RouteGrpIdx, SoConIdx, PartitionIdx) == E_OK )
  {
    /* #20 Check if no other socket route related routing group is enabled on the requested socket connection. */
    if ( SoAd_RouteGrp_CheckIfNoOtherRelatedIsEnabledOnSoCon(RouteGrpIdx, SoConIdx, PartitionIdx) == E_OK )
    {
      /* #30 Indicate that request is valid. */
      retVal = E_OK;
    }
  }

  return retVal;
} /* SoAd_RouteGrp_CheckIfAllowedForMultiInstance() */

/**********************************************************************************************************************
 *  SoAd_RouteGrp_CheckIfDisabledForOtherSoCons()
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
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_RouteGrp_CheckIfDisabledForOtherSoCons(
  SoAd_SizeOfRouteGrpType RouteGrpIdx,
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_RouteGrpSoConIterType  routeGrpSoConIter;
  SoAd_SizeOfSoConGrpType     soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);
  Std_ReturnType              retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all routing group related socket connections. */
  for ( routeGrpSoConIter = SoAd_GetRouteGrpSoConStartIdxOfRouteGrp(RouteGrpIdx, PartitionIdx);
    routeGrpSoConIter < SoAd_GetRouteGrpSoConEndIdxOfRouteGrp(RouteGrpIdx, PartitionIdx);
    routeGrpSoConIter++ )
  {
    SoAd_SizeOfSoConType soConIdxTmp = SoAd_GetSoConIdxOfRouteGrpSoCon(routeGrpSoConIter, PartitionIdx);

    /* #20 Check if socket connection is not the requested one. */
    if ( soConIdxTmp != SoConIdx )
    {
      /* #30 Check if socket connection belongs to the same socket connection group like the requested one. */
      if ( SoAd_GetSoConGrpIdxOfSoCon(soConIdxTmp, PartitionIdx) == soConGrpIdx )
      {
        /* #40 Check if routing group is enabled on the socket connection. */
        if ( SoAd_IsEnabledOfRouteGrpSoConDyn(routeGrpSoConIter, PartitionIdx) )
        {
          /* #50 Indicate that routing group is enabled on at least one other socket connection and stop iteration. */
          retVal = E_NOT_OK;
          break;
        }
      }
    }
  }

  return retVal;
} /* SoAd_RouteGrp_CheckIfDisabledForOtherSoCons() */

/**********************************************************************************************************************
 *  SoAd_RouteGrp_CheckIfNoOtherRelatedIsEnabledOnSoCon()
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
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_RouteGrp_CheckIfNoOtherRelatedIsEnabledOnSoCon(
  SoAd_SizeOfRouteGrpType RouteGrpIdx,
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SocketRouteIterType                        socketRouteIter;
  SoAd_SocketRouteDestIterType                    socketRouteDestIter;
  SoAd_RouteGrpSoConBySocketRouteDestIndIterType  routeGrpSoConBySocketRouteDestIndIter;
  SoAd_SizeOfSocketRouteType                      socketRouteIdx;
  SoAd_SizeOfRouteGrpSoConType                    routeGrpSoConIdx;
  Std_ReturnType                                  retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all routing group related socket routes. */
  for ( socketRouteIter = SoAd_GetSocketRouteIndStartIdxOfRouteGrp(RouteGrpIdx, PartitionIdx);
    socketRouteIter < SoAd_GetSocketRouteIndEndIdxOfRouteGrp(RouteGrpIdx, PartitionIdx);
    socketRouteIter++ )
  {
    socketRouteIdx = SoAd_GetSocketRouteInd(socketRouteIter, PartitionIdx);

    /* #20 Check if socket route is related to the requested socket connection. */
    if ( SoAd_GetSoConIdxOfSocketRoute(socketRouteIdx, PartitionIdx) == SoConIdx )
    {
      /* #30 Iterate over all socket route destinations of the socket route. */
      for ( socketRouteDestIter = SoAd_GetSocketRouteDestStartIdxOfSocketRoute(socketRouteIdx, PartitionIdx);
        socketRouteDestIter < SoAd_GetSocketRouteDestEndIdxOfSocketRoute(socketRouteIdx, PartitionIdx);
        socketRouteDestIter++ )
      {
        /* #40 Iterate over all routing groups of the socket route destination. */
        for ( routeGrpSoConBySocketRouteDestIndIter =
            SoAd_GetRouteGrpSoConBySocketRouteDestIndStartIdxOfSocketRouteDest(socketRouteDestIter, PartitionIdx);
          routeGrpSoConBySocketRouteDestIndIter <
            SoAd_GetRouteGrpSoConBySocketRouteDestIndEndIdxOfSocketRouteDest(socketRouteDestIter, PartitionIdx);
          routeGrpSoConBySocketRouteDestIndIter++ )
        {
          routeGrpSoConIdx = SoAd_GetRouteGrpSoConBySocketRouteDestInd(routeGrpSoConBySocketRouteDestIndIter,
            PartitionIdx);

          /* #50 Stop iteration and return failure if routing group is enabled but is not the requested one. */
          if ( SoAd_IsEnabledOfRouteGrpSoConDyn(routeGrpSoConIdx, PartitionIdx) &&
            (SoAd_GetRouteGrpIdxOfRouteGrpSoCon(routeGrpSoConIdx, PartitionIdx) != RouteGrpIdx) )
          {
            retVal = E_NOT_OK;
            break;
          }
        }
        if ( retVal != E_OK )
        {
          break;
        }
      }
    }
    if ( retVal != E_OK )
    {
      break;
    }
  }

  return retVal;
} /* SoAd_RouteGrp_CheckIfNoOtherRelatedIsEnabledOnSoCon() */

/**********************************************************************************************************************
 *  SoAd_RouteGrp_IfTransmit()
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
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_RouteGrp_IfTransmit(
  SoAd_SizeOfRouteGrpType RouteGrpIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) ErrorIdPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduInfoType                   pduInfo = { NULL_PTR, 0u };
  SoAd_PduRouteDestIndIterType  pduRouteDestIndIter;
  SoAd_RouteGrpSoConIterType    routeGrpSoConIter;
  SoAd_SizeOfRouteGrpSoConType  routeGrpSoConIdx = 0u;
  SoAd_SizeOfPduRouteDestType   pduRouteDestIdx;
  SoAd_SizeOfPduRouteType       pduRouteIdx = SoAd_GetSizeOfPduRoute(PartitionIdx);
  SoAd_SizeOfPduRouteType       lastPduRouteIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all PduRouteDests related to the routing group. */
  for ( pduRouteDestIndIter = SoAd_GetPduRouteDestIndStartIdxOfRouteGrp(RouteGrpIdx, PartitionIdx);
    pduRouteDestIndIter < SoAd_GetPduRouteDestIndEndIdxOfRouteGrp(RouteGrpIdx, PartitionIdx);
    pduRouteDestIndIter++ )
  {
    pduRouteDestIdx = SoAd_GetPduRouteDestInd(pduRouteDestIndIter, PartitionIdx);
    lastPduRouteIdx = pduRouteIdx;
    pduRouteIdx = SoAd_GetPduRouteIdxOfPduRouteDest(pduRouteDestIdx, PartitionIdx);

    /* #20 Release PDU data which was retrieved in this context for the last PduRoute. */
    if ( lastPduRouteIdx != pduRouteIdx )
    {
      SoAd_RouteGrp_IfTransmitReleasePduData(lastPduRouteIdx, PartitionIdx);

      pduInfo.SduDataPtr = NULL_PTR;
      pduInfo.SduLength = 0u;
    }

    /* #30 Trigger transmission of PduRouteDest if routing group transmit trigger is set for this PduRouteDest. */
    if ( SoAd_RouteGrp_CheckTrigger(RouteGrpIdx, pduRouteDestIdx, PartitionIdx, &routeGrpSoConIdx) == TRUE )           /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
    {
#if ( SOAD_TX_DYN_LEN == STD_ON )
      /* #300 Trigger transmission in case PDU is configued to use trigger transmit. */
      if ( SoAd_GetIfTriggerTransmitModeOfPduRoute(pduRouteIdx, PartitionIdx) == SOAD_IF_TRIGGER_TRANSMIT_MODE_MULTI )
      {
        /* #3000 Prepare transmit with size of maximum configured PDU size and copy less data later if necessary. */
        pduInfo.SduDataPtr = NULL_PTR;
        pduInfo.SduLength = SoAd_GetMaxIfRouteGrpTransmitPduSize(PartitionIdx);

        /* #3001 Transmit PDU. */
        (void)SoAd_Tx_IfTransmitPduRouteDest(pduRouteDestIdx, PartitionIdx, &pduInfo, ErrorIdPtr);                     /* SBSW_SOAD_POINTER_AND_VARIABLE_POINTER_FORWARD */
      }
      else
#endif /* SOAD_TX_DYN_LEN == STD_ON */
      /* #301 Get PDU data via trigger transmit here otherwise. */
      if ( SoAd_RouteGrp_IfTransmitGetNextPduData(pduRouteIdx, PartitionIdx, &pduInfo) == E_OK )                       /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
      {
        /* #3010 Transmit PDU. */
        (void)SoAd_Tx_IfTransmitPduRouteDest(pduRouteDestIdx, PartitionIdx, &pduInfo, ErrorIdPtr);                     /* SBSW_SOAD_POINTER_AND_VARIABLE_POINTER_FORWARD */
      }
      /* #302 Skip PduRouteDest otherwise. */
      else
      {
        /* Nothing to do. */
      }

      /* #303 Mark PDU as sent and prevent write access via critical section. */
      SOAD_BEGIN_CRITICAL_SECTION();
      SoAd_NonAtomicSetSentFlagOfRouteGrpSoConDyn(SoAd_GetAddrFlagsOfRouteGrpSoConDyn(routeGrpSoConIdx, PartitionIdx));/* SBSW_SOAD_VARIABLE_POINTER_FORWARD_CSL02_CSL03_CSL05 */
      SOAD_END_CRITICAL_SECTION();
    }
  }

  /* #40 Release PDU data which was retrieved in this context. */
  SoAd_RouteGrp_IfTransmitReleasePduData(pduRouteIdx, PartitionIdx);

  /* #50 Iterate over all related socket connections of the routing group. */
  for ( routeGrpSoConIter = SoAd_GetRouteGrpSoConStartIdxOfRouteGrp(RouteGrpIdx, PartitionIdx);
    routeGrpSoConIter < SoAd_GetRouteGrpSoConEndIdxOfRouteGrp(RouteGrpIdx, PartitionIdx);
    routeGrpSoConIter++ )
  {
    SoAd_FlagsOfRouteGrpSoConDynPtrType flagsPtr = SoAd_GetAddrFlagsOfRouteGrpSoConDyn(
      routeGrpSoConIter, PartitionIdx);

    /* #60 Reset all flags if the related PDUs are sent and protect read access via critical section. */
    SOAD_BEGIN_CRITICAL_SECTION();
    if ( SoAd_NonAtomicIsSentFlagOfRouteGrpSoConDyn(flagsPtr) )
    {
      SoAd_AtomicSetFlagsOfRouteGrpSoConDyn(flagsPtr, SOAD_ROUTEGRPSOCON_MASK_NONE);                                   /* SBSW_SOAD_VARIABLE_POINTER_FORWARD_CSL02_CSL03_CSL05 */
    }
    SOAD_END_CRITICAL_SECTION();
  }
} /* SoAd_RouteGrp_IfTransmit() */

/**********************************************************************************************************************
 *  SoAd_RouteGrp_CheckTrigger()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(boolean, SOAD_CODE) SoAd_RouteGrp_CheckTrigger(
  SoAd_SizeOfRouteGrpType RouteGrpIdx,
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(SoAd_SizeOfRouteGrpSoConType, AUTOMATIC, SOAD_APPL_VAR) RouteGrpSoConIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_RouteGrpSoConByPduRouteDestIndIterType routeGrpSoConByPduRouteDestIndIter;
  SoAd_SizeOfRouteGrpSoConType                routeGrpSoConIdx;
  boolean                                     trigger = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all PduRouteDest related socket connection routing group states. */
  for ( routeGrpSoConByPduRouteDestIndIter =
      SoAd_GetRouteGrpSoConByPduRouteDestIndStartIdxOfPduRouteDest(PduRouteDestIdx, PartitionIdx);
    routeGrpSoConByPduRouteDestIndIter <
      SoAd_GetRouteGrpSoConByPduRouteDestIndEndIdxOfPduRouteDest(PduRouteDestIdx, PartitionIdx);
    routeGrpSoConByPduRouteDestIndIter++ )
  {
    routeGrpSoConIdx = SoAd_GetRouteGrpSoConByPduRouteDestInd(routeGrpSoConByPduRouteDestIndIter, PartitionIdx);

    /* #20 Check if the routing group is the requested one. */
    if ( SoAd_GetRouteGrpIdxOfRouteGrpSoCon(routeGrpSoConIdx, PartitionIdx) == RouteGrpIdx )
    {
      /* #30 Check if trigger is set and protect read access via critical section. */
      SOAD_BEGIN_CRITICAL_SECTION();
      if ( SoAd_NonAtomicIsTriggeredFlagOfRouteGrpSoConDyn(SoAd_GetAddrFlagsOfRouteGrpSoConDyn(
            routeGrpSoConIdx, PartitionIdx)) )
      {
        *RouteGrpSoConIdxPtr = routeGrpSoConIdx;                                                                       /* SBSW_SOAD_POINTER_WRITE */
        trigger = TRUE;
      }
      SOAD_END_CRITICAL_SECTION();
      break;
    }
  }

  return trigger;
} /* SoAd_RouteGrp_CheckTrigger() */

/**********************************************************************************************************************
 *  SoAd_RouteGrp_IfTransmitGetNextPduData()
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
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_RouteGrp_IfTransmitGetNextPduData(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduLengthType   length = 0u;
  Std_ReturnType  retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if PDU data for PduRoute has not been retrieved yet. */
  if ( PduInfoPtr->SduDataPtr == NULL_PTR )
  {
    /* #20 Try to reserve trigger transmit buffer. */
    if ( SoAd_Tx_IfReserveTriggerBuf(PduRouteIdx, PartitionIdx, &length, &PduInfoPtr->SduDataPtr) == E_OK )            /* SBSW_SOAD_POINTER_AND_VARIABLE_POINTER_FORWARD */
    {
      PduInfoPtr->SduLength = length;                                                                                  /* SBSW_SOAD_POINTER_WRITE */

      /* #30 Call IfTriggerTransmit of upper layer and check if call succeeds. */
      if ( SoAd_GetIfTriggerTransmitCbkOfUpperLayer(SoAd_GetUpperLayerIdxOfPduRoute(PduRouteIdx, PartitionIdx))(       /* SBSW_SOAD_FUNCTION_PTR */
          SoAd_GetTxConfPduIdOfPduRoute(PduRouteIdx, PartitionIdx), PduInfoPtr) == E_OK )
      {
        /* #40 Check if returned parameters are valid. */
        if ( (PduInfoPtr->SduDataPtr != NULL_PTR) && (PduInfoPtr->SduLength != 0u) &&
          (PduInfoPtr->SduLength <= length) )
        {
          /* #50 Indicate that PDU has been retrieved. */
          retVal = E_OK;
        }
      }
    }

    /* #60 Indicate that call failed if one of the previous steps failed. */
    if ( retVal != E_OK )
    {
      PduInfoPtr->SduDataPtr = NULL_PTR;                                                                               /* SBSW_SOAD_POINTER_WRITE */
    }
  }
  /* #11 Indicate that PDU has been retrieved otherwise. */
  else
  {
    retVal = E_OK;
  }

  return retVal;
} /* SoAd_RouteGrp_IfTransmitGetNextPduData() */

/**********************************************************************************************************************
 *  SoAd_RouteGrp_IfTransmitReleasePduData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_RouteGrp_IfTransmitReleasePduData(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  if ( PduRouteIdx < SoAd_GetSizeOfPduRoute(PartitionIdx) )
  {
    SoAd_Tx_IfReleaseTriggerBuf(PduRouteIdx, PartitionIdx);
  }
} /* SoAd_RouteGrp_IfTransmitReleasePduData() */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  SoAd_RouteGrp_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_RouteGrp_Init(
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_RouteGrpIterType routeGrpIter;
  boolean               routeGrpEnabled;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all routing groups. */
  for ( routeGrpIter = 0u; routeGrpIter < SoAd_GetSizeOfRouteGrp(PartitionIdx); routeGrpIter++ )
  {
    SoAd_RouteGrpSoConIterType routeGrpSoConIter;

    /* #20 Get routing group state at initialization. */
    if ( SoAd_IsEnabledAtInitOfRouteGrp(routeGrpIter, PartitionIdx) )
    {
      routeGrpEnabled = TRUE;
    }
    else
    {
      routeGrpEnabled = FALSE;
    }

    /* #30 Initialize routing group state on all related socket connections. */
    for ( routeGrpSoConIter = SoAd_GetRouteGrpSoConStartIdxOfRouteGrp(routeGrpIter, PartitionIdx);
      routeGrpSoConIter < SoAd_GetRouteGrpSoConEndIdxOfRouteGrp(routeGrpIter, PartitionIdx);
      routeGrpSoConIter++ )
    {
      SoAd_SetEnabledOfRouteGrpSoConDyn(routeGrpSoConIter, routeGrpEnabled, PartitionIdx);                             /* SBSW_SOAD_CSL02_CSL03_CSL05 */
      SoAd_AtomicSetFlagsOfRouteGrpSoConDyn(SoAd_GetAddrFlagsOfRouteGrpSoConDyn(                                       /* SBSW_SOAD_VARIABLE_POINTER_FORWARD_CSL02_CSL03_CSL05 */
        routeGrpSoConIter, PartitionIdx), SOAD_ROUTEGRPSOCON_MASK_NONE);
    }
  }
} /* SoAd_RouteGrp_Init() */

/**********************************************************************************************************************
 *  SoAd_RouteGrp_SetRoutingGroupState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_RouteGrp_SetRoutingGroupState(
  SoAd_SizeOfRouteGrpType RouteGrpIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean State)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_RouteGrpSoConIterType routeGrpSoConIter;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over routing group related socket connections. */
  for ( routeGrpSoConIter = SoAd_GetRouteGrpSoConStartIdxOfRouteGrp(RouteGrpIdx, PartitionIdx);
    routeGrpSoConIter < SoAd_GetRouteGrpSoConEndIdxOfRouteGrp(RouteGrpIdx, PartitionIdx);
    routeGrpSoConIter++ )
  {
    /* #20 Set routing group state. */
    SoAd_SetEnabledOfRouteGrpSoConDyn(routeGrpSoConIter, State, PartitionIdx);                                         /* SBSW_SOAD_CSL02_CSL05 */
  }
} /* SoAd_RouteGrp_SetRoutingGroupState() */

/**********************************************************************************************************************
 *  SoAd_RouteGrp_SetSpecificRoutingGroupState()
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
FUNC(Std_ReturnType, SOAD_CODE) SoAd_RouteGrp_SetSpecificRoutingGroupState(
  SoAd_SizeOfRouteGrpType RouteGrpIdx,
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean State)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_RouteGrpSoConIterType  routeGrpSoConIter;
  Std_ReturnType              retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over routing group related socket connections. */
  for ( routeGrpSoConIter = SoAd_GetRouteGrpSoConStartIdxOfRouteGrp(RouteGrpIdx, PartitionIdx);
    routeGrpSoConIter < SoAd_GetRouteGrpSoConEndIdxOfRouteGrp(RouteGrpIdx, PartitionIdx);
    routeGrpSoConIter++ )
  {
    /* #20 Check if socket connection is the requested one. */
    if ( SoAd_GetSoConIdxOfRouteGrpSoCon(routeGrpSoConIter, PartitionIdx) == SoConIdx )
    {
      /* #30 Enter critical section to prevent that a routing group is enabled on multiple socket connections (multi
       * service instance use case). */
      SOAD_BEGIN_CRITICAL_SECTION();

      /* #40 Check if routing group is used by a socket route destination which is part of a socket route with multiple
       *     socket route destinations (multi service instance use case) on a socket connection group in case routing
       *     group shall be enabled. */
      if ( (State == TRUE) && SoAd_IsMultiInstanceOnGrpOfRouteGrp(RouteGrpIdx, PartitionIdx) )
      {
        /* #400 Assume request is valid if routing group can be enabled considering multi-instance use case. */
        retVal = SoAd_RouteGrp_CheckIfAllowedForMultiInstance(RouteGrpIdx, SoConIdx, PartitionIdx);
      }
      /* #41 Assume request is valid otherwise. */
      else
      {
        retVal = E_OK;
      }

      /* #50 Set routing group state if request is valid. */
      if ( retVal == E_OK )
      {
        SoAd_SetEnabledOfRouteGrpSoConDyn(routeGrpSoConIter, State, PartitionIdx);                                     /* SBSW_SOAD_CSL02_CSL05 */
      }

      /* #60 Leave critical section. */
      SOAD_END_CRITICAL_SECTION();

      break;
    }
  }

  return retVal;
} /* SoAd_RouteGrp_SetSpecificRoutingGroupState() */

/**********************************************************************************************************************
 *  SoAd_RouteGrp_SetIfTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_RouteGrp_SetIfTransmit(
  SoAd_SizeOfRouteGrpType RouteGrpIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_RouteGrpSoConIterType   routeGrpSoConIter;
  SoAd_SizeOfRouteGrpSoConType routeGrpSoConStartIdx = SoAd_GetRouteGrpSoConStartIdxOfRouteGrp(RouteGrpIdx,
    PartitionIdx);
  SoAd_SizeOfSoConType         soConIdx = SoAd_GetSoConIdxOfRouteGrpSoCon(routeGrpSoConStartIdx, PartitionIdx);
  SoAd_SizeOfSoConGrpType      soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(soConIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Store transmission request on all related socket connections. */
  for ( routeGrpSoConIter = routeGrpSoConStartIdx;
    routeGrpSoConIter < SoAd_GetRouteGrpSoConEndIdxOfRouteGrp(RouteGrpIdx, PartitionIdx);
    routeGrpSoConIter++ )
  {
    SoAd_AtomicSetFlagsOfRouteGrpSoConDyn(SoAd_GetAddrFlagsOfRouteGrpSoConDyn(                                         /* SBSW_SOAD_VARIABLE_POINTER_FORWARD_CSL02_CSL03_CSL05 */
      routeGrpSoConIter, PartitionIdx), SOAD_ROUTEGRPSOCON_MASK_TRIGGERED);
  }

  /* #20 Set event to handle routing group transmit request in main function. */
  SoAd_EventQueue_AddElement(SOAD_EVENT_QUEUE_IF_TX_ROUTE_GRP, RouteGrpIdx, SoAd_GetInstanceIdxOfSoConGrp(
    soConGrpIdx, PartitionIdx), PartitionIdx);
} /* SoAd_RouteGrp_SetIfTransmit() */

/**********************************************************************************************************************
 *  SoAd_RouteGrp_SetIfSpecificTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_RouteGrp_SetIfSpecificTransmit(
  SoAd_SizeOfRouteGrpType RouteGrpIdx,
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_RouteGrpSoConIterType  routeGrpSoConIter;
  SoAd_SizeOfSoConGrpType     soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);
  Std_ReturnType              retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Store transmission request for the requested socket connection. */
  for ( routeGrpSoConIter = SoAd_GetRouteGrpSoConStartIdxOfRouteGrp(RouteGrpIdx, PartitionIdx);
    routeGrpSoConIter < SoAd_GetRouteGrpSoConEndIdxOfRouteGrp(RouteGrpIdx, PartitionIdx);
    routeGrpSoConIter++ )
  {
    if ( SoAd_GetSoConIdxOfRouteGrpSoCon(routeGrpSoConIter, PartitionIdx) == SoConIdx )
    {
      SoAd_AtomicSetFlagsOfRouteGrpSoConDyn(SoAd_GetAddrFlagsOfRouteGrpSoConDyn(                                       /* SBSW_SOAD_VARIABLE_POINTER_FORWARD_CSL02_CSL03_CSL05 */
        routeGrpSoConIter, PartitionIdx), SOAD_ROUTEGRPSOCON_MASK_TRIGGERED);

      retVal = E_OK;
      break;
    }
  }

  /* #20 Set event to handle routing group transmit request in main function. */
  if ( retVal == E_OK )
  {
    SoAd_EventQueue_AddElement(SOAD_EVENT_QUEUE_IF_TX_ROUTE_GRP, RouteGrpIdx, SoAd_GetInstanceIdxOfSoConGrp(
      soConGrpIdx, PartitionIdx), PartitionIdx);
  }

  return retVal;
} /* SoAd_RouteGrp_SetIfSpecificTransmit() */

/**********************************************************************************************************************
 *  SoAd_RouteGrp_HandleIfTransmit()
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
FUNC(void, SOAD_CODE) SoAd_RouteGrp_HandleIfTransmit(
  SoAd_SizeOfInstanceMapType InstMapIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) ErrorIdPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_EventQueueIfTxRouteGrpIterType eventQueueIfTxRouteGrpIter;
  uint32                              handleIdx = 0u;
  uint32                              elementNumToHandle = SoAd_EventQueue_GetElementNumToHandle(
    SOAD_EVENT_QUEUE_IF_TX_ROUTE_GRP, InstMapIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all queue elements which have to be handled in this context considering configured limit. */
  for ( eventQueueIfTxRouteGrpIter = 0u;
    eventQueueIfTxRouteGrpIter < elementNumToHandle;
    eventQueueIfTxRouteGrpIter++ )
  {
    /* #20 Get next element. */
    if ( SoAd_EventQueue_GetNextElement(SOAD_EVENT_QUEUE_IF_TX_ROUTE_GRP, InstMapIdx, &handleIdx) == E_OK )            /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
    {
      /* #30 Enter critical section to prevent that other transmit requests interrupt this transmission. */
      SOAD_BEGIN_CRITICAL_SECTION_1(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* #40 Transmit routing group. */
      SoAd_RouteGrp_IfTransmit((SoAd_SizeOfRouteGrpType)handleIdx,
        SoAd_GetPartitionSpecificInstancePartitionIdxOfInstanceMap(InstMapIdx), ErrorIdPtr);                           /* SBSW_SOAD_POINTER_FORWARD */

      /* #50 Leave critical section. */
      SOAD_END_CRITICAL_SECTION_1(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
    /* #21 Stop iteration if no element is left. */
    else
    {
      break;
    }
  }
} /* SoAd_RouteGrp_HandleIfTransmit() */

/**********************************************************************************************************************
 *  SoAd_RouteGrp_CheckAnyRoutGrpOnPduRouteDestEnabled()
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
FUNC(boolean, SOAD_CODE) SoAd_RouteGrp_CheckAnyRoutGrpOnPduRouteDestEnabled(
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_RouteGrpSoConByPduRouteDestIndIterType routeGrpSoConByPduRouteDestIndIter;
  SoAd_SizeOfRouteGrpSoConType                routeGrpSoConIdx;
  boolean                                     retVal = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if any routing group is configured. */
  if ( SoAd_GetRouteGrpSoConByPduRouteDestIndStartIdxOfPduRouteDest(PduRouteDestIdx, PartitionIdx) !=
    SoAd_GetRouteGrpSoConByPduRouteDestIndEndIdxOfPduRouteDest(PduRouteDestIdx, PartitionIdx) )
  {
    /* #20 Iterate over all related routing groups. */
    for ( routeGrpSoConByPduRouteDestIndIter =
        SoAd_GetRouteGrpSoConByPduRouteDestIndStartIdxOfPduRouteDest(PduRouteDestIdx, PartitionIdx);
      routeGrpSoConByPduRouteDestIndIter <
        SoAd_GetRouteGrpSoConByPduRouteDestIndEndIdxOfPduRouteDest(PduRouteDestIdx, PartitionIdx);
      routeGrpSoConByPduRouteDestIndIter++ )
    {
      routeGrpSoConIdx = SoAd_GetRouteGrpSoConByPduRouteDestInd(routeGrpSoConByPduRouteDestIndIter, PartitionIdx);

      /* #30 Check if routing group is enabled. */
      if ( SoAd_IsEnabledOfRouteGrpSoConDyn(routeGrpSoConIdx, PartitionIdx) )
      {
        /* #40 Indicate to forward PDU. */
        retVal = TRUE;
        break;
      }
    }
  }
  /* #11 Indicate to forward PDU otherwise. */
  else
  {
    retVal = TRUE;
  }

  return retVal;
} /* SoAd_RouteGrp_CheckAnyRoutGrpOnPduRouteDestEnabled() */

/**********************************************************************************************************************
 *  SoAd_RouteGrp_CheckAnyRoutGrpOnSockRouteDestEnabled()
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
FUNC(boolean, SOAD_CODE) SoAd_RouteGrp_CheckAnyRoutGrpOnSockRouteDestEnabled(
  SoAd_SizeOfSocketRouteDestType SockRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_RouteGrpSoConBySocketRouteDestIndIterType  routeGrpSoConBySocketRouteDestIndIter;
  SoAd_SizeOfRouteGrpSoConType                    routeGrpSoConIdx;
  boolean                                         retVal = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if any routing group is configured. */
  if ( SoAd_GetRouteGrpSoConBySocketRouteDestIndStartIdxOfSocketRouteDest(SockRouteDestIdx, PartitionIdx) !=
    SoAd_GetRouteGrpSoConBySocketRouteDestIndEndIdxOfSocketRouteDest(SockRouteDestIdx, PartitionIdx))
  {
    /* #20 Iterate over all related routing groups. */
    for ( routeGrpSoConBySocketRouteDestIndIter =
        SoAd_GetRouteGrpSoConBySocketRouteDestIndStartIdxOfSocketRouteDest(SockRouteDestIdx, PartitionIdx);
      routeGrpSoConBySocketRouteDestIndIter <
        SoAd_GetRouteGrpSoConBySocketRouteDestIndEndIdxOfSocketRouteDest(SockRouteDestIdx, PartitionIdx);
      routeGrpSoConBySocketRouteDestIndIter++ )
    {
      routeGrpSoConIdx = SoAd_GetRouteGrpSoConBySocketRouteDestInd(routeGrpSoConBySocketRouteDestIndIter,
        PartitionIdx);

      /* #30 Check if routing group is enabled. */
      if ( SoAd_IsEnabledOfRouteGrpSoConDyn(routeGrpSoConIdx, PartitionIdx) )
      {
        /* #40 Indicate to forward PDU. */
        retVal = TRUE;
        break;
      }
    }
  }
  /* #11 Indicate to forward PDU otherwise. */
  else
  {
    retVal = TRUE;
  }

  return retVal;
} /* SoAd_RouteGrp_CheckAnyRoutGrpOnSockRouteDestEnabled() */

#define SOAD_STOP_SEC_CODE
#include "SoAd_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_MemMap */

/* Justification for module-specific MISRA deviations:
 * see file SoAd.c
 */

/**********************************************************************************************************************
 *  END OF FILE: SoAd_RouteGrp.c
 *********************************************************************************************************************/
