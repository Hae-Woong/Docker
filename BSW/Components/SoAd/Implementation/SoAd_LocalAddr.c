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
/*!        \file  SoAd_LocalAddr.c
 *        \brief  Socket Adaptor source file
 *
 *      \details  Vector static code implementation for AUTOSAR Socket Adaptor sub-component LocalAddr.
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

#define SOAD_LOCAL_ADDR_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "SoAd_LocalAddr_Int.h"
#include "SoAd_LocalAddr_Cbk.h"
#include "SoAd_SoCon.h"
#include "SoAd_State_Int.h"
#include "SoAd_Util.h"
#include "SoAd_GenTcpIpApi.h"
#include "SoAd_Cfg.h"
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

#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
/**********************************************************************************************************************
 *  SoAd_LocalAddr_CheckIpAddrAssignmentType()
 *********************************************************************************************************************/
/*! \brief      Checks value range of variables of type SoAd_IpAddrAssignmentType.
 *  \details    -
 *  \pre        -
 *  \param[in]  Type          IP address type.
 *  \return     E_OK          Type is in valid range.
 *  \return     E_NOT_OK      Type is invalid.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *  \config     SOAD_DEV_ERROR_DETECT
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_LocalAddr_CheckIpAddrAssignmentType(
  SoAd_IpAddrAssignmentType Type);
#endif /* SOAD_DEV_ERROR_DETECT == STO_ON */

/**********************************************************************************************************************
 *  SoAd_LocalAddr_RequestIpAddrAssignment()
 *********************************************************************************************************************/
/*! \brief        Requests IP address assignment on a local address identified by a socket connection.
 *  \details      -
 *  \param[in]    SoConIdx          Socket connection index.
 *                                  [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]    PartitionIdx      Partition index.
 *                                  [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    Type              IP address type.
 *                                  [range: SOAD_IPADDR_ASSIGNMENT_STATIC .. SOAD_IPADDR_ASSIGNMENT_IPV6_ROUTER]
 *  \param[in]    LocalIpAddrPtr    Pointer to IP address which shall be assigned.
 *                                  [Points to one of the following structs depending on configured IP address version
 *                                  of parameter SoConIdx:
 *                                    - SoAd_SockAddrInetType for IPv4
 *                                    - SoAd_SockAddrInet6Type for IPv6]
 *  \param[in]    Netmask           Netmask in CIDR.
 *  \param[in]    DefaultRouterPtr  Pointer to default router (gateway) address.
 *                                  [Type == SOAD_IPADDR_ASSIGNMENT_STATIC:
 *                                    ->  DefaultRouterPtr != NULL_PTR
 *                                        Points to one of the following structs depending on configured IP address
 *                                        version of parameter SoConIdx:
 *                                          - SoAd_SockAddrInetType for IPv4
 *                                          - SoAd_SockAddrInet6Type for IPv6
 *                                  Type != SOAD_IPADDR_ASSIGNMENT_STATIC:
 *                                    ->  DefaultRouterPtr == NULL_PTR]
 *  \return       E_OK              Assignment request was accepted.
 *  \return       E_NOT_OK          Assignment request was not accepted.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different local IP addresses, FALSE for same local IP address.
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_LocalAddr_RequestIpAddrAssignment(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_IpAddrAssignmentType Type,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) LocalIpAddrPtr,
  uint8 Netmask,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) DefaultRouterPtr);

/**********************************************************************************************************************
 *  SoAd_LocalAddr_IsRequestIpAddrAssignmentNeeded()
 *********************************************************************************************************************/
/*! \brief        Checks if a request of the IP address assignment is needed.
 *  \details      -
 *  \param[in]    SoConIdx          Socket connection index.
 *                                  [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]    PartitionIdx      Partition index.
 *                                  [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    LocalAddrIdx      Local address index.
 *                                  [range: LocalAddrIdx < SoAd_GetSizeOfLocalAddr()]
 *  \param[in]    Type              IP address type.
 *                                  [range: SOAD_IPADDR_ASSIGNMENT_STATIC .. SOAD_IPADDR_ASSIGNMENT_IPV6_ROUTER]
 *  \param[in]    LocalIpAddrPtr    Pointer to IP address which shall be assigned.
 *                                  [Points to one of the following structs depending on configured IP address version
 *                                  of parameter SoConIdx:
 *                                    - SoAd_SockAddrInetType for IPv4
 *                                    - SoAd_SockAddrInet6Type for IPv6]
 *  \return       TRUE              Assignment request is needed.
 *  \return       FALSE             Assignment request is not needed.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different local IP addresses, FALSE for same local IP address.
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(boolean, SOAD_CODE) SoAd_LocalAddr_IsRequestIpAddrAssignmentNeeded(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_SizeOfLocalAddrType LocalAddrIdx,
  SoAd_IpAddrAssignmentType Type,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) LocalIpAddrPtr);

/**********************************************************************************************************************
 *  SoAd_LocalAddr_GetLocalAddrIdxByTcpIpLocalAddrId()
 *********************************************************************************************************************/
/*! \brief      Checks if the IP address ID is used by SoAd and gets the partition specific index if available.
 *  \details    -
 *  \param[in]  IpAddrId        IP address identifier.
 *  \param[out] LocalAddrIdxPtr Pointer to the partition specific LocalAddrIdx.
 *  \param[out] PartitionIdxPtr Pointer to the partition index of the LocalAddr.
 *  \return     E_OK            IP address identifier is inside valid range.
 *  \return     E_NOT_OK        IP address identifier is not inside valid range.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  TRUE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_LocalAddr_GetLocalAddrIdxByTcpIpLocalAddrId(
  SoAd_LocalAddrIdType IpAddrId,
  P2VAR(SoAd_SizeOfLocalAddrType, AUTOMATIC, SOAD_APPL_VAR) LocalAddrIdxPtr,
  P2VAR(SoAd_PartitionConfigIdxType, AUTOMATIC, SOAD_APPL_VAR) PartitionIdxPtr);

/**********************************************************************************************************************
 *  SoAd_LocalAddr_LocalIpAddrAssignmentChg()
 *********************************************************************************************************************/
/*! \brief        Receives local IP address assignment state changes.
 *  \details      -
 *  \param[in]    LocalAddrIdx  Local IP address index.
 *                              [range: LocalAddrIdx < SoAd_GetSizeOfLocalAddr()]
 *  \param[in]    PartitionIdx  Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    State         State of IP address assignment.
 *                              [range: SOAD_IPADDR_STATE_ASSIGNED, SOAD_IPADDR_STATE_ONHOLD,
 *                              SOAD_IPADDR_STATE_UNASSIGNED]
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different IP address identifier, FALSE for same IP address identifier.
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_LocalAddr_LocalIpAddrAssignmentChg(
  SoAd_SizeOfLocalAddrType LocalAddrIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_IpAddrStateType State);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
/**********************************************************************************************************************
 *  SoAd_LocalAddr_CheckIpAddrAssignmentType()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_LocalAddr_CheckIpAddrAssignmentType(
  SoAd_IpAddrAssignmentType Type)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if IP address assignment type is in valid range. */
  if ( (Type == SOAD_IPADDR_ASSIGNMENT_STATIC) ||
    (Type == SOAD_IPADDR_ASSIGNMENT_LINKLOCAL_DOIP) ||
    (Type == SOAD_IPADDR_ASSIGNMENT_DHCP) ||
    (Type == SOAD_IPADDR_ASSIGNMENT_LINKLOCAL) ||  /* PRQA S 2996 */ /* MD_SoAd_MacroRedundantCheck */
    (Type == SOAD_IPADDR_ASSIGNMENT_IPV6_ROUTER) )
  {
    retVal = E_OK;
  }

  return retVal;
} /* SoAd_LocalAddr_CheckIpAddrAssignmentType() */
#endif /* SOAD_DEV_ERROR_DETECT == STO_ON */

/**********************************************************************************************************************
 *  SoAd_LocalAddr_RequestIpAddrAssignment()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6060 1 */ /* MD_MSR_STPAR */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_LocalAddr_RequestIpAddrAssignment(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_IpAddrAssignmentType Type,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) LocalIpAddrPtr,
  uint8 Netmask,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) DefaultRouterPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  const SoAd_SizeOfLocalAddrType  localAddrIdx = SoAd_SoCon_GetLocalAddrIdx(SoConIdx, PartitionIdx);
  Std_ReturnType                  retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if request of IP address assignment is needed. */
  if ( SoAd_LocalAddr_IsRequestIpAddrAssignmentNeeded(SoConIdx, PartitionIdx, localAddrIdx, Type, LocalIpAddrPtr)      /* SBSW_SOAD_POINTER_FORWARD */
    == TRUE )
  {
    /* #20 Forward request to TcpIp module. */
    retVal = SoAd_TcpIpApiRequestIpAddrAssignment(SoAd_GetAddrIdOfLocalAddr(localAddrIdx, PartitionIdx), Type,         /* SBSW_SOAD_POINTER_FORWARD */
      LocalIpAddrPtr, Netmask, DefaultRouterPtr);

    /* #30 Update local port of local static IP address if request was successful. */
    if ( retVal == E_OK )
    {
      if ( Type == SOAD_IPADDR_ASSIGNMENT_STATIC )
      {
        /* PRQA S 310, 3305 2 */ /* MD_SoAd_PointerCastOnStruct */
        SoAd_SoCon_UpdateLocalPort(SoConIdx, PartitionIdx,
          (SOAD_A_P2CONST(SoAd_SockAddrInetXType)LocalIpAddrPtr)->port);                                               /* SBSW_SOAD_SOCK_ADDR_CAST_POINTER_STRUCT_ELEMENT_FORWARD */
      }
    }
  }

  return retVal;
} /* SoAd_LocalAddr_RequestIpAddrAssignment() */

/**********************************************************************************************************************
 *  SoAd_LocalAddr_IsRequestIpAddrAssignmentNeeded()
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
/* PRQA S 6060 1 */ /* MD_MSR_STPAR */
SOAD_LOCAL FUNC(boolean, SOAD_CODE) SoAd_LocalAddr_IsRequestIpAddrAssignmentNeeded(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_SizeOfLocalAddrType LocalAddrIdx,
  SoAd_IpAddrAssignmentType Type,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) LocalIpAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isNeeded = TRUE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if IP address assignmnent type static is used. */
  if ( Type == SOAD_IPADDR_ASSIGNMENT_STATIC )
  {
    /* #20 Check if IP address is not unassigned. */
    if ( SoAd_GetIpAddrStateOfLocalAddrDyn(LocalAddrIdx, PartitionIdx) != SOAD_IPADDR_STATE_UNASSIGNED )
    {
#if ( SOAD_IPV6 == STD_ON )
      SoAd_SockAddrInetXType localIpAddr = { 0u, 0u, { 0u, 0u, 0u, 0u } };
      SoAd_SockAddrInetXType defaultRouter = { 0u, 0u, { 0u, 0u, 0u, 0u } };
#else
      SoAd_SockAddrInetXType localIpAddr = { 0u, 0u, { 0u } };
      SoAd_SockAddrInetXType defaultRouter = { 0u, 0u, { 0u } };
#endif /* SOAD_IPV6 == STD_ON */
      uint8                  dummyNetmask = 0u;

      /* #30 Retrieve local IP address. */
      /* PRQA S 310 4 */ /* MD_SoAd_PointerCastOnStruct */
      if ( SoAd_TcpIpApiGetIpAddr(SoAd_GetAddrIdOfLocalAddr(LocalAddrIdx, PartitionIdx),                               /* SBSW_SOAD_SOCK_ADDR_VARIABLE_POINTER_AND_VARIABLE_POINTER_FORWARD */
        SOAD_A_P2VAR(SoAd_SockAddrType)&localIpAddr,
        &dummyNetmask,
        SOAD_A_P2VAR(SoAd_SockAddrType)&defaultRouter) == E_OK )
      {
        /* #40 Check if requested IP address is already assigned. */
        /* PRQA S 310, 3305 2 */ /* MD_SoAd_PointerCastOnStruct */
        if ( SoAd_Util_CompareIpAddr(SoAd_GetDomainOfLocalAddr(LocalAddrIdx, PartitionIdx), localIpAddr.addr,          /* SBSW_SOAD_SOCK_ADDR_CAST_POINTER_STRUCT_ELEMENT_AND_CONST_POINTER_FORWARD */
            (SOAD_A_P2CONST(SoAd_SockAddrInetXType)LocalIpAddrPtr)->addr) == E_OK )
        {
          /* #50 Request IP address assignment is not required. */
          isNeeded = FALSE;

          /* #60 Update local port of local static IP address. */
          /* PRQA S 310, 3305 2 */ /* MD_SoAd_PointerCastOnStruct */
          SoAd_SoCon_UpdateLocalPort(SoConIdx, PartitionIdx,
            (SOAD_A_P2CONST(SoAd_SockAddrInetXType)LocalIpAddrPtr)->port);                                             /* SBSW_SOAD_SOCK_ADDR_CAST_POINTER_STRUCT_ELEMENT_FORWARD */
        }
      }
    }
  }

  return isNeeded;
} /* SoAd_LocalAddr_IsRequestIpAddrAssignmentNeeded() */

/**********************************************************************************************************************
 *  SoAd_LocalAddr_GetLocalAddrIdxByTcpIpLocalAddrId()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_LocalAddr_GetLocalAddrIdxByTcpIpLocalAddrId(
  SoAd_LocalAddrIdType IpAddrId,
  P2VAR(SoAd_SizeOfLocalAddrType, AUTOMATIC, SOAD_APPL_VAR) LocalAddrIdxPtr,
  P2VAR(SoAd_PartitionConfigIdxType, AUTOMATIC, SOAD_APPL_VAR) PartitionIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the local address is mapped to any partition. */
  if ( SoAd_IsPartitionSpecificLocalAddrUsedOfLocalAddrIdMap(IpAddrId) )
  {
    /* #20 Get the partition specific local address index and the corresponding partition index. */
    *LocalAddrIdxPtr = SoAd_GetPartitionSpecificLocalAddrIdxOfLocalAddrIdMap(IpAddrId);                                /* SBSW_SOAD_POINTER_WRITE */
    *PartitionIdxPtr = SoAd_GetPartitionSpecificLocalAddrPartitionIdxOfLocalAddrIdMap(IpAddrId);                       /* SBSW_SOAD_POINTER_WRITE */
    retVal = E_OK;
  }

  return retVal;
} /* SoAd_LocalAddr_GetLocalAddrIdxByTcpIpLocalAddrId() */

/**********************************************************************************************************************
 *  SoAd_LocalAddr_LocalIpAddrAssignmentChg()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_LocalAddr_LocalIpAddrAssignmentChg(
  SoAd_SizeOfLocalAddrType LocalAddrIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_IpAddrStateType State)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SoConGrpIterType soConGrpIter; /* Not initialized: For-loop counter. */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set local address state. */
  SoAd_SetIpAddrStateOfLocalAddrDyn(LocalAddrIdx, State, PartitionIdx);                                                /* SBSW_SOAD_PARAMETER_IDX */

  /* #20 Iterate over all socket connection groups which are related to the local address. */
  for ( soConGrpIter = SoAd_GetSoConGrpStartIdxOfLocalAddr(LocalAddrIdx, PartitionIdx); soConGrpIter <
    SoAd_GetSoConGrpEndIdxOfLocalAddr(LocalAddrIdx, PartitionIdx); soConGrpIter++ )
  {
    SoAd_SizeOfSoConGrpType soConGrpIdx = (SoAd_SizeOfSoConGrpType)soConGrpIter;

    /* #30 Notify user about IP address state change on socket connection. */
    SoAd_SoCon_NotifyLocalIpAddrAssignmentChg(soConGrpIdx, PartitionIdx, State);
  }
} /* SoAd_LocalAddr_LocalIpAddrAssignmentChg() */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/* ----- Service functions --------------------------------------------- */

/**********************************************************************************************************************
 *  SoAd_RequestIpAddrAssignment()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_RequestIpAddrAssignment(
  SoAd_SoConIdType SoConId,
  SoAd_IpAddrAssignmentType Type,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) LocalIpAddrPtr,
  uint8 Netmask,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) DefaultRouterPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConType        soConIdx; /* Not initialized: Variable is initialized by another function. */
  SoAd_PartitionConfigIdxType partitionIdx; /* Not initialized: Variable is initialized by another function. */
  Std_ReturnType              retVal = E_NOT_OK;
  uint8                       errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state. */
  if ( SoAd_State_GetState() < SOAD_STATE_INIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* #20 Check plausibility of input parameter 'SoConId'. */
  else if ( SoAd_SoCon_GetSoConIdxBySoConId(SoConId, &soConIdx, &partitionIdx) != E_OK )                               /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
  {
    errorId = SOAD_E_INV_ARG;
  }
  /* #30 Check plausibility of input parameter 'Type'. */
  else if ( SoAd_LocalAddr_CheckIpAddrAssignmentType(Type) != E_OK )
  {
    errorId = SOAD_E_INV_ARG;
  }
  /* #40 Check plausibility of input parameter 'LocalIpAddrPtr' dependent on 'Type'. */
  else if ( (Type == SOAD_IPADDR_ASSIGNMENT_STATIC) && (LocalIpAddrPtr == NULL_PTR) )
  {
    errorId = SOAD_E_PARAM_POINTER;
  }
  /* PRQA S 3415 1 */ /* MD_SoAd_RightHandOperandWithoutSideEffects */
  else if ( (Type == SOAD_IPADDR_ASSIGNMENT_STATIC) && (SoAd_GetDomainOfLocalAddr(SoAd_SoCon_GetLocalAddrIdx(soConIdx,
    partitionIdx), partitionIdx)!= LocalIpAddrPtr->domain) )
  {
    errorId = SOAD_E_INV_ARG;
  }
# if ( SOAD_MULTI_PARTITION == STD_ON ) /* COV_SOAD_QM_FEATURE_MULTI_PARTITION */
  /* #50 Check validity of application identifier and the mapped instance index (if required). */
  else if ( SoAd_Util_CheckApplicationIdValidity(partitionIdx) != E_OK )
  {
    errorId = SOAD_E_INV_APPLICATION_ID;
  }
# endif /* SOAD_MULTI_PARTITION == STD_ON */
  else
#else
  if ( SoAd_SoCon_GetSoConIdxBySoConId(SoConId, &soConIdx, &partitionIdx) == E_OK )                                    /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #60 Check module state. */
    if ( SoAd_State_GetState() == SOAD_STATE_INIT )
    {
      /* #70 Handle IP address assignment request. */
      retVal = SoAd_LocalAddr_RequestIpAddrAssignment(
        soConIdx,
        partitionIdx,
        Type,
        LocalIpAddrPtr,
        Netmask,
        DefaultRouterPtr);                                                                                             /* SBSW_SOAD_POINTER_FORWARD */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #80 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_REQ_IP_ADDR_ASSIGN, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* SoAd_RequestIpAddrAssignment() */

/**********************************************************************************************************************
 *  SoAd_ReleaseIpAddrAssignment()
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
FUNC(Std_ReturnType, SOAD_CODE) SoAd_ReleaseIpAddrAssignment(
  SoAd_SoConIdType SoConId)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConType        soConIdx; /* Not initialized: Variable is initialized by another function. */
  SoAd_PartitionConfigIdxType partitionIdx; /* Not initialized: Variable is initialized by another function. */
  Std_ReturnType              retVal = E_NOT_OK;
  uint8                       errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state. */
  if ( SoAd_State_GetState() < SOAD_STATE_INIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* #20 Check plausibility of input parameter. */
  else if ( SoAd_SoCon_GetSoConIdxBySoConId(SoConId, &soConIdx, &partitionIdx) != E_OK )                               /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
  {
    errorId = SOAD_E_INV_ARG;
  }
# if ( SOAD_MULTI_PARTITION == STD_ON ) /* COV_SOAD_QM_FEATURE_MULTI_PARTITION */
  /* Check validity of application identifier and the mapped instance index. */
  else if ( SoAd_Util_CheckApplicationIdValidity(partitionIdx) != E_OK )
  {
    errorId = SOAD_E_INV_APPLICATION_ID;
  }
# endif /* SOAD_MULTI_PARTITION == STD_ON */
  else
#else
  if ( SoAd_SoCon_GetSoConIdxBySoConId(SoConId, &soConIdx, &partitionIdx) == E_OK )                                    /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Check module state. */
    if ( SoAd_State_GetState() == SOAD_STATE_INIT )
    {
      /* #40 Handle IP address assignment release request. */
      retVal = SoAd_TcpIpApiReleaseIpAddrAssignment(SoAd_GetAddrIdOfLocalAddr(SoAd_SoCon_GetLocalAddrIdx(soConIdx,
        partitionIdx), partitionIdx));
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #50 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_RLS_IP_ADDR_ASSIGN, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* SoAd_ReleaseIpAddrAssignment() */

/**********************************************************************************************************************
 *  SoAd_GetLocalAddr()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_GetLocalAddr(
  SoAd_SoConIdType SoConId,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) LocalAddrPtr,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) NetmaskPtr,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) DefaultRouterPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConType        soConIdx; /* Not initialized: Variable is initialized by another function. */
  SoAd_PartitionConfigIdxType partitionIdx; /* Not initialized: Variable is initialized by another function. */
  Std_ReturnType              retVal = E_NOT_OK;
  uint8                       errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state. */
  if ( SoAd_State_GetState() < SOAD_STATE_INIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* #20 Check plausibility of all input parameters. */
  else if ( SoAd_SoCon_GetSoConIdxBySoConId(SoConId, &soConIdx, &partitionIdx) != E_OK )                               /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
  {
    errorId = SOAD_E_INV_ARG;
  }
  else if ( LocalAddrPtr == NULL_PTR )
  {
    errorId = SOAD_E_PARAM_POINTER;
  }
  else if ( NetmaskPtr == NULL_PTR )
  {
    errorId = SOAD_E_PARAM_POINTER;
  }
  else if ( DefaultRouterPtr == NULL_PTR )
  {
    errorId = SOAD_E_PARAM_POINTER;
  }
# if ( SOAD_MULTI_PARTITION == STD_ON ) /* COV_SOAD_QM_FEATURE_MULTI_PARTITION */
  /* Check validity of application identifier and the mapped instance index. */
  else if ( SoAd_Util_CheckApplicationIdValidity(partitionIdx) != E_OK )
  {
    errorId = SOAD_E_INV_APPLICATION_ID;
  }
# endif /* SOAD_MULTI_PARTITION == STD_ON */
  else
#else
  if ( SoAd_SoCon_GetSoConIdxBySoConId(SoConId, &soConIdx, &partitionIdx) == E_OK )                                    /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Check module state. */
    if ( SoAd_State_GetState() == SOAD_STATE_INIT )
    {
      const SoAd_SizeOfSocketType sockIdx = SoAd_SoCon_GetSocketIdx(soConIdx, partitionIdx);

      /* #40 Get local IP address. */
      SoAd_LocalAddr_GetIpAddr(soConIdx, partitionIdx, LocalAddrPtr, NetmaskPtr, DefaultRouterPtr);                    /* SBSW_SOAD_POINTER_FORWARD */

      /* #50 Get port from local data structure. */
      /* PRQA S 310, 3305 1 */ /* MD_SoAd_PointerCastOnStruct */
      ((SoAd_SockAddrInetXType*)LocalAddrPtr)->port = SoAd_SoCon_GetLocalPortDyn(sockIdx, partitionIdx);               /* SBSW_SOAD_POINTER_WRITE */

      retVal = E_OK;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #60 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_GET_LOCAL_ADDR, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* SoAd_GetLocalAddr() */

/**********************************************************************************************************************
 *  SoAd_ReadDhcpHostNameOption()
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
/* PRQA S 6080 2 */ /* MD_MSR_STMIF */
/* PRQA S 3673 4 */ /* MD_SoAd_ConstCausedByUnused */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_ReadDhcpHostNameOption(
  SoAd_SoConIdType SoConId,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) length,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) data)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConType        soConIdx; /* Not initialized: Variable is initialized by another function. */
  SoAd_PartitionConfigIdxType partitionIdx; /* Not initialized: Variable is initialized by another function. */
  Std_ReturnType              retVal = E_NOT_OK;
  uint8                       errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state. */
  if ( SoAd_State_GetState() < SOAD_STATE_INIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* #20 Check plausibility of all input parameters. */
  else if ( SoAd_SoCon_GetSoConIdxBySoConId(SoConId, &soConIdx, &partitionIdx) != E_OK )                               /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
  {
    errorId = SOAD_E_INV_ARG;
  }
  else if ( length == NULL_PTR )
  {
    errorId = SOAD_E_PARAM_POINTER;
  }
  else if ( data == NULL_PTR )
  {
    errorId = SOAD_E_PARAM_POINTER;
  }
# if ( SOAD_MULTI_PARTITION == STD_ON ) /* COV_SOAD_QM_FEATURE_MULTI_PARTITION */
  /* Check validity of application identifier and the mapped instance index. */
  else if ( SoAd_Util_CheckApplicationIdValidity(partitionIdx) != E_OK )
  {
    errorId = SOAD_E_INV_APPLICATION_ID;
  }
# endif /* SOAD_MULTI_PARTITION == STD_ON */
  else
#else
  if ( SoAd_SoCon_GetSoConIdxBySoConId(SoConId, &soConIdx, &partitionIdx) == E_OK )                                    /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Check if component is initialized. */
    if ( SoAd_State_GetState() == SOAD_STATE_INIT )
    {
#if ( (SOAD_DHCPV6_CLIENT_ENABLED == STD_ON) || (SOAD_DHCPV4_CLIENT_ENABLED == STD_ON) )
      const SoAd_SizeOfLocalAddrType localAddrIdx = SoAd_SoCon_GetLocalAddrIdx(soConIdx, partitionIdx);

# if ( SOAD_DHCPV6_CLIENT_ENABLED == STD_ON )
      /* #40 Forward request to TcpIp module for DHCPv6. */
      if ( SoAd_GetDomainOfLocalAddr(localAddrIdx, partitionIdx) == SOAD_AF_INET6 )
      {
        /* The hostname option has a maximum length of 255 but the generic TcpIp API (TcpIp_DhcpV6ReadOption) is used
         * which has a uint16 length. There is no risk using this cast. */
        uint16 tmpLength = *length;

        if ( SoAd_TcpIpApiDhcpV6ReadOption(SoAd_GetAddrIdOfLocalAddr(localAddrIdx, partitionIdx),                      /* SBSW_SOAD_POINTER_AND_VARIABLE_POINTER_FORWARD */
          SOAD_DHCP_OPT_V6_HOSTNAME, &tmpLength, data) == E_OK )
        {
          *length = (uint8)tmpLength;                                                                                  /* SBSW_SOAD_POINTER_WRITE */
          retVal = E_OK;
        }
      }
# endif /* SOAD_DHCPV6_CLIENT_ENABLED == STD_ON */
# if ( SOAD_DHCPV4_CLIENT_ENABLED == STD_ON )
      /* #50 Forward request to TcpIp module for DHCPv4. */
      if ( SoAd_GetDomainOfLocalAddr(localAddrIdx, partitionIdx) == SOAD_AF_INET )
      {
        retVal = SoAd_TcpIpApiDhcpReadOption(SoAd_GetAddrIdOfLocalAddr(localAddrIdx, partitionIdx),                    /* SBSW_SOAD_POINTER_FORWARD */
          SOAD_DHCP_OPT_V4_HOSTNAME, length, data);
      }
# endif /* SOAD_DHCPV4_CLIENT_ENABLED == STD_ON */
#else
      SOAD_DUMMY_STATEMENT(SoConId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
      SOAD_DUMMY_STATEMENT(length); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
      SOAD_DUMMY_STATEMENT(data); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
      SOAD_DUMMY_STATEMENT(soConIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
      SOAD_DUMMY_STATEMENT(partitionIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* (SOAD_DHCPV6_CLIENT_ENABLED == STD_ON) || (SOAD_DHCPV4_CLIENT_ENABLED == STD_ON) */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #60 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_READ_DHCP_HOST_NAME_OPT, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* SoAd_ReadDhcpHostNameOption() */

/**********************************************************************************************************************
 *  SoAd_WriteDhcpHostNameOption()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_WriteDhcpHostNameOption(
  SoAd_SoConIdType SoConId,
  uint8 length,
  P2CONST(uint8, AUTOMATIC, SOAD_APPL_VAR) data)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConType        soConIdx; /* Not initialized: Variable is initialized by another function. */
  SoAd_PartitionConfigIdxType partitionIdx; /* Not initialized: Variable is initialized by another function. */
  Std_ReturnType              retVal = E_NOT_OK;
  uint8                       errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state. */
  if ( SoAd_State_GetState() < SOAD_STATE_INIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* #20 Check plausibility of all input parameters. */
  else if ( SoAd_SoCon_GetSoConIdxBySoConId(SoConId, &soConIdx, &partitionIdx) != E_OK )                               /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
  {
    errorId = SOAD_E_INV_ARG;
  }
  else if ( data == NULL_PTR )
  {
    errorId = SOAD_E_PARAM_POINTER;
  }
# if ( SOAD_MULTI_PARTITION == STD_ON ) /* COV_SOAD_QM_FEATURE_MULTI_PARTITION */
  /* Check validity of application identifier and the mapped instance index. */
  else if ( SoAd_Util_CheckApplicationIdValidity(partitionIdx) != E_OK )
  {
    errorId = SOAD_E_INV_APPLICATION_ID;
  }
# endif /* SOAD_MULTI_PARTITION == STD_ON */
  else
#else
  if ( SoAd_SoCon_GetSoConIdxBySoConId(SoConId, &soConIdx, &partitionIdx) == E_OK )                                    /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Check if component is initialized. */
    if ( SoAd_State_GetState() == SOAD_STATE_INIT )
    {
#if ( (SOAD_DHCPV6_CLIENT_ENABLED == STD_ON) || (SOAD_DHCPV4_CLIENT_ENABLED == STD_ON) )
      const SoAd_SizeOfLocalAddrType localAddrIdx = SoAd_SoCon_GetLocalAddrIdx(soConIdx, partitionIdx);

# if ( SOAD_DHCPV6_CLIENT_ENABLED == STD_ON )
      /* #40 Forward request to TcpIp module for DHCPv6. */
      if ( SoAd_GetDomainOfLocalAddr(localAddrIdx, partitionIdx) == SOAD_AF_INET6 )
      {
        retVal = SoAd_TcpIpApiDhcpV6WriteOption(SoAd_GetAddrIdOfLocalAddr(localAddrIdx, partitionIdx),                 /* SBSW_SOAD_POINTER_FORWARD */
          SOAD_DHCP_OPT_V6_HOSTNAME, length, data);
      }
# endif /* SOAD_DHCPV6_CLIENT_ENABLED == STD_ON */
# if ( SOAD_DHCPV4_CLIENT_ENABLED == STD_ON )
      /* #50 Forward request to TcpIp module for DHCPv4. */
      if ( SoAd_GetDomainOfLocalAddr(localAddrIdx, partitionIdx) == SOAD_AF_INET )
      {
        retVal = SoAd_TcpIpApiDhcpWriteOption(SoAd_GetAddrIdOfLocalAddr(localAddrIdx, partitionIdx),                   /* SBSW_SOAD_POINTER_FORWARD */
          SOAD_DHCP_OPT_V4_HOSTNAME, length, data);
      }
# endif /* SOAD_DHCPV4_CLIENT_ENABLED == STD_ON */
#else
      SOAD_DUMMY_STATEMENT(SoConId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
      SOAD_DUMMY_STATEMENT(length); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
      SOAD_DUMMY_STATEMENT_CONST(data); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
      SOAD_DUMMY_STATEMENT(soConIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
      SOAD_DUMMY_STATEMENT(partitionIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* (SOAD_DHCPV6_CLIENT_ENABLED == STD_ON) || (SOAD_DHCPV4_CLIENT_ENABLED == STD_ON)) */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #60 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_WRITE_DHCP_HOST_NAME_OPT, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* SoAd_WriteDhcpHostNameOption() */

/**********************************************************************************************************************
 *  SoAd_ReadDhcpOption()
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
/* PRQA S 6030, 6080 2 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
/* PRQA S 3673 5 */ /* MD_SoAd_ConstCausedByUnused */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_ReadDhcpOption(
  SoAd_LocalAddrIdType IpAddrId,
  uint16 Option,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) DataLengthPtr,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) DataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfLocalAddrType    localAddrIdx; /* Not initialized: Variable is initialized by another function. */
  SoAd_PartitionConfigIdxType partitionIdx; /* Not initialized: Variable is initialized by another function. */
  Std_ReturnType              retVal = E_NOT_OK;
  uint8                       errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state. */
  if ( SoAd_State_GetState() < SOAD_STATE_INIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* #20 Check plausibility of all input parameters. */
  else if ( (IpAddrId >= SoAd_GetSizeOfLocalAddrIdMap())
# if ( SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
    || SoAd_IsInvalidHndOfLocalAddrIdMap(IpAddrId)
# endif /* SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE */
    )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  if ( SoAd_LocalAddr_GetLocalAddrIdxByTcpIpLocalAddrId(IpAddrId, &localAddrIdx, &partitionIdx) != E_OK )              /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
  {
    /* This is a valid use-case. No DETs are expected when the local address is not mapped to any partition since not
     * all TcpIp local addresses are used by SoAd. */
  }
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  else if (
# if ( SOAD_IPV6 == STD_ON )
    (SoAd_GetDomainOfLocalAddr(localAddrIdx, partitionIdx) == SOAD_AF_INET) &&
# endif /* SOAD_IPV6 == STD_ON */
    (Option > SOAD_MAX_UINT_8) )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else if ( DataLengthPtr == NULL_PTR )
  {
    errorId = SOAD_E_PARAM_POINTER;
  }
  else if (
# if ( SOAD_IPV6 == STD_ON )
    (SoAd_GetDomainOfLocalAddr(localAddrIdx, partitionIdx) == SOAD_AF_INET) &&
# endif /* SOAD_IPV6 == STD_ON */
    (*DataLengthPtr > SOAD_MAX_UINT_8) )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else if ( DataPtr == NULL_PTR )
  {
    errorId = SOAD_E_PARAM_POINTER;
  }
# if ( SOAD_MULTI_PARTITION == STD_ON ) /* COV_SOAD_QM_FEATURE_MULTI_PARTITION */
  /* Check validity of application identifier and the mapped instance index. */
  else if ( SoAd_Util_CheckApplicationIdValidity(partitionIdx) != E_OK )
  {
    errorId = SOAD_E_INV_APPLICATION_ID;
  }
# endif /* SOAD_MULTI_PARTITION == STD_ON */
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Check if component is initialized. */
    if ( SoAd_State_GetState() == SOAD_STATE_INIT )
    {
#if ( (SOAD_DHCPV6_CLIENT_ENABLED == STD_ON) || (SOAD_DHCPV4_CLIENT_ENABLED == STD_ON) )
# if ( SOAD_DHCPV6_CLIENT_ENABLED == STD_ON )
      /* #40 Forward request to TcpIp module for DHCPv6. */
      if ( SoAd_GetDomainOfLocalAddr(localAddrIdx, partitionIdx) == SOAD_AF_INET6 )
      {
        retVal = SoAd_TcpIpApiDhcpV6ReadOption(IpAddrId, Option, DataLengthPtr, DataPtr);                              /* SBSW_SOAD_POINTER_FORWARD */
      }
# endif /* SOAD_DHCPV6_CLIENT_ENABLED == STD_ON */
# if ( SOAD_DHCPV4_CLIENT_ENABLED == STD_ON )
      /* #50 Forward request to TcpIp module for DHCPv4. */
      if ( SoAd_GetDomainOfLocalAddr(localAddrIdx, partitionIdx) == SOAD_AF_INET )
      {
        uint8 tmpDataLength = (uint8)*DataLengthPtr;

        retVal = SoAd_TcpIpApiDhcpReadOption(IpAddrId, (uint8)Option, &tmpDataLength, DataPtr);                        /* SBSW_SOAD_POINTER_AND_VARIABLE_POINTER_FORWARD */

        *DataLengthPtr = tmpDataLength;                                                                                /* SBSW_SOAD_POINTER_WRITE */
      }
# endif /* SOAD_DHCPV4_CLIENT_ENABLED == STD_ON */
#else
      SOAD_DUMMY_STATEMENT(IpAddrId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
      SOAD_DUMMY_STATEMENT(Option); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
      SOAD_DUMMY_STATEMENT(DataLengthPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
      SOAD_DUMMY_STATEMENT(DataPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* (SOAD_DHCPV6_CLIENT_ENABLED == STD_ON) || (SOAD_DHCPV4_CLIENT_ENABLED == STD_ON) */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #60 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_READ_DHCP_OPT, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* SoAd_ReadDhcpOption() */

/**********************************************************************************************************************
 *  SoAd_WriteDhcpOption()
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
/* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_WriteDhcpOption(
  SoAd_LocalAddrIdType IpAddrId,
  uint16 Option,
  uint16 DataLength,
  P2CONST(uint8, AUTOMATIC, SOAD_APPL_VAR) DataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfLocalAddrType    localAddrIdx; /* Not initialized: Variable is initialized by another function. */
  SoAd_PartitionConfigIdxType partitionIdx; /* Not initialized: Variable is initialized by another function. */
  Std_ReturnType              retVal = E_NOT_OK;
  uint8                       errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state. */
  if ( SoAd_State_GetState() < SOAD_STATE_INIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* #20 Check plausibility of all input parameters. */
  else if ( (IpAddrId >= SoAd_GetSizeOfLocalAddrIdMap())
# if ( SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
    || SoAd_IsInvalidHndOfLocalAddrIdMap(IpAddrId)
# endif /* SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE */
    )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  if ( SoAd_LocalAddr_GetLocalAddrIdxByTcpIpLocalAddrId(IpAddrId, &localAddrIdx, &partitionIdx) != E_OK )              /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
  {
    /* This is a valid use-case. No DETs are expected when the local address is not mapped to any partition since not
     * all TcpIp local addresses are used by SoAd. */
  }
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  else if (
# if ( SOAD_IPV6 == STD_ON )
    (SoAd_GetDomainOfLocalAddr(localAddrIdx, partitionIdx) == SOAD_AF_INET) &&
# endif /* SOAD_IPV6 == STD_ON */
    (Option > SOAD_MAX_UINT_8) )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else if (
# if ( SOAD_IPV6 == STD_ON )
    (SoAd_GetDomainOfLocalAddr(localAddrIdx, partitionIdx) == SOAD_AF_INET) &&
# endif /* SOAD_IPV6 == STD_ON */
    (DataLength > SOAD_MAX_UINT_8) )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else if ( DataPtr == NULL_PTR )
  {
    errorId = SOAD_E_PARAM_POINTER;
  }
# if ( SOAD_MULTI_PARTITION == STD_ON ) /* COV_SOAD_QM_FEATURE_MULTI_PARTITION */
  /* Check validity of application identifier and the mapped instance index. */
  else if ( SoAd_Util_CheckApplicationIdValidity(partitionIdx) != E_OK )
  {
    errorId = SOAD_E_INV_APPLICATION_ID;
  }
# endif /* SOAD_MULTI_PARTITION == STD_ON */
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Check if component is initialized. */
    if ( SoAd_State_GetState() == SOAD_STATE_INIT )
    {
#if ( (SOAD_DHCPV6_CLIENT_ENABLED == STD_ON) || (SOAD_DHCPV4_CLIENT_ENABLED == STD_ON) )
# if ( SOAD_DHCPV6_CLIENT_ENABLED == STD_ON )
      /* #40 Forward request to TcpIp module for DHCPv6. */
      if ( SoAd_GetDomainOfLocalAddr(localAddrIdx, partitionIdx) == SOAD_AF_INET6 )
      {
        retVal = SoAd_TcpIpApiDhcpV6WriteOption(IpAddrId, Option, DataLength, DataPtr);                                /* SBSW_SOAD_CONST_POINTER_FORWARD */
      }
# endif /* SOAD_DHCPV6_CLIENT_ENABLED == STD_ON */
# if ( SOAD_DHCPV4_CLIENT_ENABLED == STD_ON )
      /* #50 Forward request to TcpIp module for DHCPv4. */
      if ( SoAd_GetDomainOfLocalAddr(localAddrIdx, partitionIdx) == SOAD_AF_INET )
      {
        retVal = SoAd_TcpIpApiDhcpWriteOption(IpAddrId, (uint8)Option, (uint8)DataLength, DataPtr);                    /* SBSW_SOAD_CONST_POINTER_FORWARD */
      }
# endif /* SOAD_DHCPV4_CLIENT_ENABLED == STD_ON */
#else
      SOAD_DUMMY_STATEMENT(IpAddrId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
      SOAD_DUMMY_STATEMENT(Option); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
      SOAD_DUMMY_STATEMENT(DataLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
      SOAD_DUMMY_STATEMENT(DataPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* (SOAD_DHCPV6_CLIENT_ENABLED == STD_ON) || (SOAD_DHCPV4_CLIENT_ENABLED == STD_ON) */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #60 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_WRITE_DHCP_OPT, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* SoAd_WriteDhcpOption() */

/* ----- Functions ----------------------------------------------------- */

/**********************************************************************************************************************
 *  SoAd_LocalAddr_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_LocalAddr_Init(
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_LocalAddrIterType localAddrIter; /* Not initialized: For-loop counter. */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all local addresses. */
  for ( localAddrIter = 0u; localAddrIter < SoAd_GetSizeOfLocalAddr(PartitionIdx); localAddrIter++ )
  {
    /* #20 Initialize IP address assignment state. */
    SoAd_SetIpAddrStateOfLocalAddrDyn(localAddrIter, SOAD_IPADDR_STATE_UNASSIGNED, PartitionIdx);                      /* SBSW_SOAD_CSL02_CSL05 */
  }
} /* SoAd_LocalAddr_Init() */

/**********************************************************************************************************************
 *  SoAd_LocalAddr_GetDomain()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(SoAd_DomainType, SOAD_CODE) SoAd_LocalAddr_GetDomain(
  SoAd_SizeOfLocalAddrType LocalAddrIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Return domain of local address. */
  const SoAd_DomainType domain = SoAd_GetDomainOfLocalAddr(LocalAddrIdx, PartitionIdx);

  return domain;
} /* SoAd_LocalAddr_GetDomain() */

/**********************************************************************************************************************
 *  SoAd_LocalAddr_GetIpAddr()
 *********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_LocalAddr_GetIpAddr(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) LocalAddrPtr,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) NetmaskPtr,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) DefaultRouterPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  const SoAd_SizeOfLocalAddrType localAddrIdx = SoAd_SoCon_GetLocalAddrIdx(SoConIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get local IP address from TcpIp module. */
  if ( SoAd_TcpIpApiGetIpAddr(SoAd_GetAddrIdOfLocalAddr(localAddrIdx, PartitionIdx),                                   /* SBSW_SOAD_POINTER_FORWARD */
      LocalAddrPtr,
      NetmaskPtr,
      DefaultRouterPtr) != E_OK )
  {
    /* #100 Use wildcard IP address if call to TcpIp module fails. */
    /* PRQA S 310, 3305 12 */ /* MD_SoAd_PointerCastOnStruct */
#if ( SOAD_IPV6 == STD_ON )
    if ( SoAd_GetDomainOfLocalAddr(localAddrIdx, PartitionIdx) == SOAD_AF_INET6 )
    {
      ((SoAd_SockAddrInet6Type*)LocalAddrPtr)->addr[0u] = SOAD_IPADDR_ANY;                                             /* SBSW_SOAD_EXPECTED_SIZE_WRITE */
      ((SoAd_SockAddrInet6Type*)LocalAddrPtr)->addr[1u] = SOAD_IPADDR_ANY;                                             /* SBSW_SOAD_EXPECTED_SIZE_WRITE */
      ((SoAd_SockAddrInet6Type*)LocalAddrPtr)->addr[2u] = SOAD_IPADDR_ANY;                                             /* SBSW_SOAD_EXPECTED_SIZE_WRITE */
      ((SoAd_SockAddrInet6Type*)LocalAddrPtr)->addr[3u] = SOAD_IPADDR_ANY;                                             /* SBSW_SOAD_EXPECTED_SIZE_WRITE */
    }
    else
#endif /* SOAD_IPV6 == STD_ON */
    {
      ((SoAd_SockAddrInetType*)LocalAddrPtr)->addr[0u] = SOAD_IPADDR_ANY;                                              /* SBSW_SOAD_EXPECTED_SIZE_WRITE */
    }
  }
} /* SoAd_LocalAddr_GetIpAddr() */

/**********************************************************************************************************************
 *  SoAd_LocalAddr_GetState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(SoAd_IpAddrStateType, SOAD_CODE) SoAd_LocalAddr_GetState(
  SoAd_SizeOfLocalAddrType LocalAddrIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Return state of local address. */
  const SoAd_IpAddrStateType localAddrState = SoAd_GetIpAddrStateOfLocalAddrDyn(LocalAddrIdx, PartitionIdx);

  return localAddrState;
} /* SoAd_LocalAddr_GetState() */

/**********************************************************************************************************************
 *  SoAd_LocalAddr_GetTcpIpLocalAddrId()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(SoAd_LocalAddrIdType, SOAD_CODE) SoAd_LocalAddr_GetTcpIpLocalAddrId(
  SoAd_SizeOfLocalAddrType LocalAddrIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Return address ID of local address. */
  const SoAd_LocalAddrIdType tcpIpLocalAddrId = SoAd_GetAddrIdOfLocalAddr(LocalAddrIdx, PartitionIdx);

  return tcpIpLocalAddrId;
} /* SoAd_LocalAddr_GetTcpIpLocalAddrId() */

/**********************************************************************************************************************
 *  SoAd_LocalAddr_IsAddrActiveOnCtrl
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
FUNC(Std_ReturnType, SOAD_CODE) SoAd_LocalAddr_IsAddrActiveOnCtrl(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_IpAddrConstPtrType IpAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  const SoAd_SizeOfLocalAddrType       localAddrIdx = SoAd_SoCon_GetLocalAddrIdx(SoConIdx, PartitionIdx);
  SoAd_LocalAddrByTcpIpCtrlIndIterType tcpIpCtrlIndIter; /* Not initialized: For-loop counter. */
  const SoAd_SizeOfTcpIpCtrlType       tcpIpCtrlIdx = SoAd_GetTcpIpCtrlIdxOfLocalAddr(localAddrIdx, PartitionIdx);
  Std_ReturnType                       retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all local addresses of the TcpIp controller. */
  for ( tcpIpCtrlIndIter = SoAd_GetLocalAddrByTcpIpCtrlIndStartIdxOfTcpIpCtrl(tcpIpCtrlIdx, PartitionIdx);
    tcpIpCtrlIndIter < SoAd_GetLocalAddrByTcpIpCtrlIndEndIdxOfTcpIpCtrl(tcpIpCtrlIdx, PartitionIdx);
    tcpIpCtrlIndIter++ )
  {
    SoAd_SizeOfLocalAddrType localAddrIdxTmp = SoAd_GetLocalAddrByTcpIpCtrlInd(tcpIpCtrlIndIter, PartitionIdx);
#if ( SOAD_IPV6 == STD_ON )
    SoAd_SockAddrInetXType localIpAddr = { 0u, 0u, { 0u, 0u, 0u, 0u } };
    SoAd_SockAddrInetXType defaultRouter = { 0u, 0u, { 0u, 0u, 0u, 0u } };
#else
    SoAd_SockAddrInetXType localIpAddr = { 0u, 0u, { 0u } };
    SoAd_SockAddrInetXType defaultRouter = { 0u, 0u, { 0u } };
#endif /* SOAD_IPV6 == STD_ON */
    uint8                    netmask = 0u;

    /* #100 Get local IP address from TcpIp. */
    /* PRQA S 310 5 */ /* MD_SoAd_PointerCastOnStruct */
    if ( SoAd_TcpIpApiGetIpAddr(                                                                                       /* SBSW_SOAD_SOCK_ADDR_VARIABLE_POINTER_AND_VARIABLE_POINTER_FORWARD */
        SoAd_GetAddrIdOfLocalAddr(localAddrIdxTmp, PartitionIdx),
        (P2VAR(SoAd_SockAddrType, AUTOMATIC, AUTOMATIC))&localIpAddr,
        &netmask,
        (P2VAR(SoAd_SockAddrType, AUTOMATIC, AUTOMATIC))&defaultRouter) == E_OK )
    {
      /* #1000 Compare IP address to local IP address. */
      if ( SoAd_Util_CompareIpAddr(                                                                                    /* SBSW_SOAD_CONST_POINTER_FORWARD */
          SoAd_GetDomainOfLocalAddr(localAddrIdxTmp, PartitionIdx),
          localIpAddr.addr,
          IpAddrPtr) == E_OK )
      {
        /* #10000 Stop iteration if a local IP address matches. */
        retVal = E_OK;
        break;
      }
    }
    /* #101 Ignore local IP address if call to TcpIp failed. */
    else
    {
      /* nothing to do */
    }
  }

  return retVal;
} /* SoAd_LocalAddr_IsAddrActiveOnCtrl() */

/**********************************************************************************************************************
 *  SoAd_LocalAddr_IsRetryEnabled()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(boolean, SOAD_CODE) SoAd_LocalAddr_IsRetryEnabled(
  SoAd_SizeOfLocalAddrType LocalAddrIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns true if feature transmission retry is available on TcpIp controller. */
  const boolean retryEnabled = (boolean) SoAd_IsRetryEnabledOfTcpIpCtrl(
    SoAd_GetTcpIpCtrlIdxOfLocalAddr(LocalAddrIdx, PartitionIdx), PartitionIdx);

  return retryEnabled;
} /* SoAd_LocalAddr_IsRetryEnabled() */

/* ----- Callbacks ----------------------------------------------------- */

/**********************************************************************************************************************
 *  SoAd_LocalIpAddrAssignmentChg()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, SOAD_CODE) SoAd_LocalIpAddrAssignmentChg(
  SoAd_LocalAddrIdType IpAddrId,
  SoAd_IpAddrStateType State)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfLocalAddrType    localAddrIdx; /* Not initialized: Variable is initialized by another function. */
  SoAd_PartitionConfigIdxType partitionIdx; /* Not initialized: Variable is initialized by another function. */
  uint8                       errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state. */
  if ( SoAd_State_GetState() < SOAD_STATE_INIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* #20 Check plausibility of all input parameters. */
  else if ( (IpAddrId >= SoAd_GetSizeOfLocalAddrIdMap())
# if ( SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
    || SoAd_IsInvalidHndOfLocalAddrIdMap(IpAddrId)
# endif /* SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE */
    )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  if ( SoAd_LocalAddr_GetLocalAddrIdxByTcpIpLocalAddrId(IpAddrId, &localAddrIdx, &partitionIdx) != E_OK )              /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
  {
    /* This is a valid use-case. No DETs are expected when the local address is not mapped to any partition since not
     * all TcpIp local addresses are used by SoAd. */
  }
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  else if ( (State != SOAD_IPADDR_STATE_ASSIGNED) &&
    (State != SOAD_IPADDR_STATE_ONHOLD) &&
    (State != SOAD_IPADDR_STATE_UNASSIGNED) )
  {
    errorId = SOAD_E_INV_ARG;
  }
# if ( SOAD_MULTI_PARTITION == STD_ON ) /* COV_SOAD_QM_FEATURE_MULTI_PARTITION */
  /* Check validity of application identifier and the mapped instance index. */
  else if ( SoAd_Util_CheckApplicationIdValidity(partitionIdx) != E_OK )
  {
    errorId = SOAD_E_INV_APPLICATION_ID;
  }
# endif /* SOAD_MULTI_PARTITION == STD_ON */
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Handle local address state change. */
    SoAd_LocalAddr_LocalIpAddrAssignmentChg(localAddrIdx, partitionIdx, State);
  }
  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #40 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_LOCAL_IP_ADDR_ASSIGNMENT_CHG, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */
} /* SoAd_LocalIpAddrAssignmentChg() */

/**********************************************************************************************************************
 *  SoAd_DhcpEvent()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, SOAD_CODE) SoAd_DhcpEvent(
  SoAd_LocalAddrIdType IpAddrId,
  SoAd_DhcpEventType Event)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state. */
  if ( SoAd_State_GetState() < SOAD_STATE_INIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* #20 Check plausibility of all input parameters. */
  else if ( (IpAddrId >= SoAd_GetSizeOfLocalAddrIdMap())
# if ( SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
    || SoAd_IsInvalidHndOfLocalAddrIdMap(IpAddrId)
# endif /* SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE */
    )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else if ( Event == SOAD_DHCP_EVENT_INVALID )
  {
    errorId = SOAD_E_INV_ARG;
  }
# if ( SOAD_MULTI_PARTITION == STD_ON ) /* COV_SOAD_QM_FEATURE_MULTI_PARTITION */
  /* Check validity of application identifier and the mapped instance index if TcpIp local address is used by SoAd. */
  /* PRQA S 3415 2 */ /* MD_SoAd_RightHandOperandWithoutSideEffects */
  else if ( SoAd_IsPartitionSpecificLocalAddrUsedOfLocalAddrIdMap(IpAddrId) && (SoAd_Util_CheckApplicationIdValidity(
        SoAd_GetPartitionSpecificLocalAddrPartitionIdxOfLocalAddrIdMap(IpAddrId)) != E_OK) )
  {
    errorId = SOAD_E_INV_APPLICATION_ID;
  }
# endif /* SOAD_MULTI_PARTITION == STD_ON */
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( (SOAD_DHCPV6_CLIENT_ENABLED == STD_ON) || (SOAD_DHCPV4_CLIENT_ENABLED == STD_ON) )
    SoAd_DhcpEventCbkIterType cbkIter; /* Not initialized: For-loop counter. */

    /* #30 Forward DHCP event to user (if DHCP is enabled). */
    for ( cbkIter = 0u; cbkIter < SoAd_GetSizeOfDhcpEventCbk(); cbkIter++ )
    {
      if ( SoAd_GetDhcpEventCbk(cbkIter) != NULL_PTR )
      {
        SoAd_GetDhcpEventCbk(cbkIter)(IpAddrId, Event);                                                                /* SBSW_SOAD_FUNCTION_PTR */
      }
    }
#else
    SOAD_DUMMY_STATEMENT(IpAddrId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
    SOAD_DUMMY_STATEMENT(Event); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* (SOAD_DHCPV6_CLIENT_ENABLED == STD_ON) || (SOAD_DHCPV4_CLIENT_ENABLED == STD_ON) */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #40 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_DHCP_EVENT, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */
} /* SoAd_DhcpEvent() */

#define SOAD_STOP_SEC_CODE
#include "SoAd_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_MemMap */

/* Justification for module-specific MISRA deviations:
 * see file SoAd.c
 */

/**********************************************************************************************************************
 *  END OF FILE: SoAd_LocalAddr.c
 *********************************************************************************************************************/
