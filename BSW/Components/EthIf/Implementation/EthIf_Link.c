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
/*!        \file  EthIf_Link.c
 *        \brief  EthIf Link public API source file
 *
 *      \details  Implementation of the public API of the sub-module Link of EthIf.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/

#define ETHIF_LINK_SOURCE

/* PRQA S 0779 EOF */ /* MD_MSR_5.1_779 */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/*********************************************************************************************************
 * Predefined file includes based on \trace DSGN-EthIfDiag1975
 *********************************************************************************************************/
#include "EthIf_Link.h"
#include "EthIf_Link_Cbk.h"
#include "EthIf_Link_Int.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Check the version of Msn header file */
#if (  (ETHIF_SW_MAJOR_VERSION != (17u)) \
    || (ETHIF_SW_MINOR_VERSION != (2u)) \
    || (ETHIF_SW_PATCH_VERSION != (0u)) )
# error "Vendor specific version numbers of EthIf_Link.c and EthIf.h are inconsistent"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#if !defined (ETHIF_LINK_LOCAL)                                                                                         /* COV_ETHIF_LOCAL_INLINE */
# define ETHIF_LINK_LOCAL                                             static
#endif

#if !defined (ETHIF_LINK_LOCAL_INLINE)                                                                                  /* COV_ETHIF_LOCAL_INLINE */
# define ETHIF_LINK_LOCAL_INLINE                                      LOCAL_INLINE
#endif

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  EthIf_ProcessLinkStateChangeForAllEthIfCtrls
 *********************************************************************************************************************/
/*! \brief          Processes the link state changes for all EthIf controllers
 *  \details        This function checks for link state changes on all EthIf controllers and notifies the EthIf users
 *                  registered for a link state change notification about changes.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
ETHIF_LINK_LOCAL_INLINE FUNC(void, ETHIF_LINK_INLINE_CODE) EthIf_ProcessLinkStateChangeForAllEthIfCtrls( void );

#if ( ETHIF_SWT_PORT_GROUP_MODE_REQ_SUPPORT == STD_ON )
/**********************************************************************************************************************
 *  EthIf_ProcessAllEthIfSwtPortGroupLinkStateChanges
 *********************************************************************************************************************/
/*! \brief          Processes the link state changes for all EthIf switch port groups
 *  \details        This function checks for link state changes on all EthIf switch port groups and notifies the BswM
 *                  about the change.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
ETHIF_LINK_LOCAL_INLINE FUNC(void, ETHIF_LINK_INLINE_CODE) EthIf_ProcessAllEthIfSwtPortGroupLinkStateChanges( void );
#endif /* ETHIF_SWT_PORT_GROUP_MODE_REQ_SUPPORT */

/**********************************************************************************************************************
 *  EthIf_InitLinkCounts
 *********************************************************************************************************************/
/*! \brief          Initializes EthIf controller an EthIf switch port group link counts
 *  \details        -
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
ETHIF_LINK_LOCAL_INLINE FUNC(void, ETHIF_LINK_INLINE_CODE) EthIf_InitLinkCounts( void );

#if ( ETHIF_ETHTRCV_USED == STD_ON )                                                                                    /* COV_ETHIF_SAFE_FEAT */
/**********************************************************************************************************************
 *  EthIf_PollTrcvLinks
 *********************************************************************************************************************/
/*! \brief          Poll the link state of Ethernet transceivers
 *  \details        This function polls the link state of the Ethernet transceivers by utilizing the respective
 *                  hardware driver.
 *  \pre            EthIf_DetectLinkStateChange() has to initialize all EthIf_EthIfCtrlLinkCounts to 0 before calling
 *                  EthIf_PollTrcvLinks() and EthIf_PollPortLinks().
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
ETHIF_LINK_LOCAL_INLINE FUNC(void, ETHIF_LINK_INLINE_CODE) EthIf_PollTrcvLinks( void );

/**********************************************************************************************************************
 *  EthIf_SetTrcvLinkStateActive
 *********************************************************************************************************************/
/*! \brief          Set the link state of the transceiver to ACTIVE
 *  \details        Set the link state of transceiver stored in the EthIf link state table to ACTIVE.
 *                  - When this transceiver is secured, the link state stored in EthIf link state table will be set to
 *                    ETHIF_PHYELEM_LINK_STATE_PHY_ACTIVE, and report the pae index of this transceiver and the link
 *                    state ETHTRCV_LINK_STATE_ACTIVE to Mka.
 *                  - When this transceiver is not secured, the link state stored in EthIf link state table will be set
 *                    to ETHIF_PHYELEM_LINK_STATE_ACTIVE, no reporting to Mka.
 *  \param[in]      ethTrcvIdx  Index of the Ethernet switch port within the context of the Ethernet Interface
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
ETHIF_LINK_LOCAL_INLINE FUNC(void, ETHIF_LINK_INLINE_CODE) EthIf_SetTrcvLinkStateActive(
  EthIf_EthTrcvIterType ethTrcvIdx);

/**********************************************************************************************************************
 *  EthIf_SetTrcvLinkStateDown
 *********************************************************************************************************************/
/*! \brief          Set the link state of the transceiver to DOWN
 *  \details        Set the link state of transceiver stored in the EthIf link state table to DOWN.
 *                  - When this transceiver is secured, the link state stored in EthIf link state table will be set to
 *                    ETHIF_PHYELEM_LINK_STATE_PHY_DOWN, and report the pae index and the link state
 *                    ETHTRCV_LINK_STATE_DOWN to Mka.
 *                  - When this transceiver is not secured, the link state stored in EthIf link state table will be set
 *                    to ETHIF_PHYELEM_LINK_STATE_DOWN, no reporting to Mka.
 *  \param[in]      ethTrcvIdx  Index of the Ethernet switch port within the context of the Ethernet Interface
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
ETHIF_LINK_LOCAL_INLINE FUNC(void, ETHIF_LINK_INLINE_CODE) EthIf_SetTrcvLinkStateDown(
  EthIf_EthTrcvIterType ethTrcvIdx);

/**********************************************************************************************************************
 *  EthIf_IncEthIfCtrlsLinkCount
 *********************************************************************************************************************/
/*! \brief          Increase the EthIfCtrl link count
 *  \details        Increase the EthIfCtrl link count for the EthIfCtrl, who belongs to this transceiver
 *                  The link count of this EthIfCtrl will be updated, when the link state of this transceiver stored
 *                  in the EthIf link state table is not DOWN, AND one of the following conditions is fulfilled:
 *                  - The link state of this transceiver stored in the EthIf link state table is ACTIVE
 *                  - No MACSec is enabled for this EthIfCtrl
 *  \param[in]      ethTrcvIdx  Index of the Ethernet switch port within the context of the Ethernet Interface
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
ETHIF_LINK_LOCAL_INLINE FUNC(void, ETHIF_LINK_INLINE_CODE) EthIf_IncEthIfCtrlsLinkCount(
  EthIf_EthTrcvIterType ethTrcvIdx);
#endif /* ETHIF_ETHTRCV_USED */

#if ( ETHIF_ETHSWT_USED == STD_ON )
/**********************************************************************************************************************
 *  EthIf_PollPortLinks
 *********************************************************************************************************************/
/*! \brief          Poll the link state of Ethernet switch ports
 *  \details        This function polls the link state of the Ethernet switch ports by utilizing the respective
 *                  hardware driver.
 *  \pre            EthIf_DetectLinkStateChange() has to initialize all EthIf_EthIfCtrlLinkCounts to 0 before calling
 *                  EthIf_PollTrcvLinks() and EthIf_PollPortLinks().
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
ETHIF_LINK_LOCAL_INLINE FUNC(void, ETHIF_LINK_INLINE_CODE) EthIf_PollPortLinks( void );

/**********************************************************************************************************************
 *  EthIf_PortLinkStateHandling
 *********************************************************************************************************************/
/*! \brief          Handle the link state of Ethernet switch ports
 *  \details        This function polls the link state of the Ethernet switch ports by utilizing the respective
 *                  hardware driver.
 *  \param[in]      portIdx    Index of the Ethernet switch port within the context of the Ethernet Interface
 *  \param[in]      linkState  Link state to be handled
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
ETHIF_LINK_LOCAL_INLINE FUNC(void, ETHIF_LINK_INLINE_CODE) EthIf_PortLinkStateHandling(
  uint8                 portIdx,
  EthTrcv_LinkStateType linkState);

/**********************************************************************************************************************
 *  EthIf_SetPortLinkStateActive
 *********************************************************************************************************************/
/*! \brief          Set the link state of the port to ACTIVE
 *  \details        Set the link state of port stored in the EthIf link state table to ACTIVE.
 *                  - When this port is secured, the link state stored in EthIf link state table will be set to
 *                    ETHIF_PHYELEM_LINK_STATE_PHY_ACTIVE, and report the pae index and the link state
 *                    ETHTRCV_LINK_STATE_ACTIVE to Mka.
 *                  - When this port is not secured, the link state stored in EthIf link state table will be set to
 *                    ETHIF_PHYELEM_LINK_STATE_ACTIVE, no reporting to Mka.
 *  \param[in]      portIdx    Index of the Ethernet switch port within the context of the Ethernet Interface
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
ETHIF_LINK_LOCAL_INLINE FUNC(void, ETHIF_LINK_INLINE_CODE) EthIf_SetPortLinkStateActive(
  uint8 portIdx);

/**********************************************************************************************************************
 *  EthIf_SetPortLinkStateDown
 *********************************************************************************************************************/
/*! \brief          Set the link state of the port to DOWN
 *  \details        Set the link state of port stored in the EthIf link state table to DOWN.
 *                  - When this port is secured, the link state stored in EthIf link state table will be set to
 *                    ETHIF_PHYELEM_LINK_STATE_PHY_DOWN, and report the pae index and the link state
 *                    ETHTRCV_LINK_STATE_DOWN to Mka.
 *                  - When this port is not secured, the link state stored in EthIf link state table will be set to
 *                    ETHIF_PHYELEM_LINK_STATE_DOWN, no reporting to Mka.
 *  \param[in]      portIdx    Index of the Ethernet switch port within the context of the Ethernet Interface
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
ETHIF_LINK_LOCAL_INLINE FUNC(void, ETHIF_LINK_INLINE_CODE) EthIf_SetPortLinkStateDown(
  uint8 portIdx);

/**********************************************************************************************************************
 *  EthIf_IncPortGroupLinkCount
 *********************************************************************************************************************/
/*! \brief          Increase the port group link counts in EthIf
 *  \details        Increase the port group link counts for the groups, which this port belongs to.
 *                  - When lax link aggregation is enabled, PortGroupLaxLinkAggregationCount will be increased.
 *                  - When lax link aggregation is disabled and this port is host port or up link port,
 *                    PortGroupHostOrUpLinkPortRoleLinkCount will be increased.
 *                  - When lax link aggregation is disabled and this port has no role (neither host port
 *                    nor uplink port), PortGroupHostOrUpLinkPortRoleLinkCount will be increased.
 *  \param[in]      portIdx    Index of the Ethernet switch port within the context of the Ethernet Interface
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
ETHIF_LINK_LOCAL_INLINE FUNC(void, ETHIF_LINK_INLINE_CODE) EthIf_IncPortGroupLinkCount(
  uint8 portIdx);

/**********************************************************************************************************************
 *  EthIf_PollPortGroupLinks
 *********************************************************************************************************************/
/*! \brief          Poll the link state of Ethernet switch port groups
 *  \details        This function polls the link state of the Ethernet switch port groups.
 *  \pre            EthIf_DetectLinkStateChange() has to initialize all EthIf_EthIfCtrlLinkCounts to 0 before calling
 *                  EthIf_PollPortLinks() and thereafter EthIf_PollPortGroupLinks().
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
ETHIF_LINK_LOCAL_INLINE FUNC(void, ETHIF_LINK_INLINE_CODE) EthIf_PollPortGroupLinks( void );
#endif /* ETHIF_ETHSWT_USED */

/**********************************************************************************************************************
 *  EthIf_DetectLinkStateChangesOnEthIfCtrls
 *********************************************************************************************************************/
/*! \brief          Detects if a link change for the EthIf controllers has occurred
 *  \details        This functions detects if a link state change for the EthIf controllers has occurred.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
ETHIF_LINK_LOCAL_INLINE FUNC(void, ETHIF_LINK_INLINE_CODE) EthIf_DetectLinkStateChangesOnEthIfCtrls( void );

#if ( ETHIF_ETHSWT_USED == STD_ON )
/**********************************************************************************************************************
 *  EthIf_DetectLinkStateChangesOnEthIfSwtPortGroups
 *********************************************************************************************************************/
/*! \brief          Detects if a link change for the EthIf switch port groups have occurred
 *  \details        This functions detects if a link state change for the EthIf switch port groups have occurred.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
ETHIF_LINK_LOCAL_INLINE FUNC(void, ETHIF_LINK_INLINE_CODE) EthIf_DetectLinkStateChangesOnEthIfSwtPortGroups( void );

/**********************************************************************************************************************
 *  EthIf_IsPortGroupLinkStateActive
 *********************************************************************************************************************/
/*! \brief          Check if the link state of the port group is ACTIVE
 *  \details        This function check if the link state of the port group is ACTIVE
 *                  When the Lax Link Aggregation is not active, the port group is considered as ACTIVE only when
 *                  it fulfilled both the following two conditions (According to ECUC_EthIf_00259 in ASR 22-11):
 *                  - The host port and the relevant up link ports are ACTIVE.
 *                  - At least one no-role port is ACTIVE or there is no no-role port.
 *                  When the Lax Link Aggregation is active, the port group is considered as ACTIVE when:
 *                  - The needed amount of Lax Link Aggregation ports are ACTIVE.
 *  \param[in]      portGroupIdx  Index of the port group within the context of the Ethernet Interface
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
ETHIF_LINK_LOCAL_INLINE FUNC(boolean, ETHIF_LINK_INLINE_CODE) EthIf_IsPortGroupLinkStateActive(
  EthIf_PortGroupIterType portGroupIdx);
#endif /* ETHIF_ETHSWT_USED */

/**********************************************************************************************************************
 *  EthIf_DetectLinkStateChange
 *********************************************************************************************************************/
/*! \brief          Detects if a link change for the EthIf controllers has occurred
 *  \details        This functions detects if a link state change for the EthIf controllers has occurred.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
ETHIF_LINK_LOCAL_INLINE FUNC(void, ETHIF_LINK_INLINE_CODE) EthIf_DetectLinkStateChange(void);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  EthIf_ProcessLinkStateChangeForAllEthIfCtrls
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
ETHIF_LINK_LOCAL_INLINE FUNC(void, ETHIF_LINK_INLINE_CODE) EthIf_ProcessLinkStateChangeForAllEthIfCtrls( void )
{
  /* ----- Local Variables ---------------------------------------------- */
  EthIf_EthIfCtrlIterType ethIfCtrlIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check each EthIf controller for a link change */
  for( ethIfCtrlIdx = 0;
       ethIfCtrlIdx < EthIf_GetSizeOfEthIfCtrl();
       ethIfCtrlIdx++ )
  {
    EthIf_ProcessLinkStateChange(ethIfCtrlIdx);
  }
} /* EthIf_ProcessLinkStateChangeForAllEthIfCtrls() */

#if ( ETHIF_SWT_PORT_GROUP_MODE_REQ_SUPPORT == STD_ON )
/**********************************************************************************************************************
 *  EthIf_ProcessAllEthIfSwtPortGroupLinkStateChanges
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
ETHIF_LINK_LOCAL_INLINE FUNC(void, ETHIF_LINK_INLINE_CODE) EthIf_ProcessAllEthIfSwtPortGroupLinkStateChanges( void )
{
  /* ----- Local Variables ---------------------------------------------- */
  EthIf_PortGroupIterType portGroupIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check each EthIf switch port group with semantic type independent for a link change */
  for( portGroupIdx = 0;
       portGroupIdx < EthIf_GetSizeOfPortGroup();
       portGroupIdx++ )
  {
    if ( EthIf_GetSemanticOfPortGroup(portGroupIdx) == ETHIF_PORTGROUP_SEMANTIC_INDEPENDENT_SEMANTICOFPORTGROUP )
    {
      EthIf_ProcessSwtPortGroupLinkStateChange(portGroupIdx);
    }
  }
} /* EthIf_ProcessAllEthIfSwtPortGroupLinkStateChanges() */
#endif /* ETHIF_SWT_PORT_GROUP_MODE_REQ_SUPPORT */

/**********************************************************************************************************************
 *  EthIf_InitLinkCounts
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
ETHIF_LINK_LOCAL_INLINE FUNC(void, ETHIF_LINK_INLINE_CODE) EthIf_InitLinkCounts( void )
{
  EthIf_EthIfCtrlLinkCountIterType ethIfCtrlLinkCntIdx;
#if ( ETHIF_ETHSWT_USED == STD_ON )
  EthIf_PortGroupIterType portGroupIdx;
#endif /* ETHIF_ETHSWT_USED */

  /* #10 Initialize link counts of the EthIf controllers */
  for( ethIfCtrlLinkCntIdx = 0;
       ethIfCtrlLinkCntIdx < EthIf_GetSizeOfEthIfCtrlLinkCount();
       ethIfCtrlLinkCntIdx++ )
  {
    EthIf_SetEthIfCtrlLinkCount(ethIfCtrlLinkCntIdx, 0u);                                                               /* SBSW_ETHIF_COMMON_CSL01 */
  }

#if ( ETHIF_ETHSWT_USED == STD_ON )
  /* #20 Initialize link counts of EthIf switch port groups */
  for( portGroupIdx = 0u;
       portGroupIdx < EthIf_GetSizeOfPortGroup();
       portGroupIdx++ )
  {
    EthIf_SetPortGroupLaxLinkAggregationCount(portGroupIdx, 0u);                                                        /* SBSW_ETHIF_COMMON_CSL01 */
    EthIf_SetPortGroupHostOrUpLinkPortRoleLinkCount(portGroupIdx, 0u);                                                  /* SBSW_ETHIF_COMMON_CSL01 */
    EthIf_SetPortGroupNoRoleLinkCount(portGroupIdx, 0u);                                                                /* SBSW_ETHIF_COMMON_CSL01 */
  }
#endif /* ETHIF_ETHSWT_USED */
} /* EthIf_InitLinkCounts() */

#if ( ETHIF_ETHTRCV_USED == STD_ON )                                                                                    /* COV_ETHIF_SAFE_FEAT */
/**********************************************************************************************************************
 *  EthIf_PollTrcvLinks
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
ETHIF_LINK_LOCAL_INLINE FUNC(void, ETHIF_LINK_INLINE_CODE) EthIf_PollTrcvLinks( void ) /* PRQA S 6080 */ /* MD_MSR_STMIF */
{
  /* ----- Local Variables ---------------------------------------------- */
  EthIf_EthTrcvIterType ethTrcvIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 For each transceiver */
  for( ethTrcvIdx = 0U;
       ethTrcvIdx < EthIf_GetSizeOfEthTrcv();
       ethTrcvIdx++ )
  {
    /* #110 If the transceiver is in state ACTIVE */
    if( EthIf_GetEthTrcvModeCnt(ethTrcvIdx) > 0U )
    {
      EthIf_EthTrcvDrvApiIdxOfEthTrcvType ethTrcvDrvApiIdx = EthIf_GetEthTrcvDrvApiIdxOfEthTrcv(ethTrcvIdx);

      if( EthIf_GetGetLinkStateOfEthTrcvDrvApi(ethTrcvDrvApiIdx) != NULL_PTR )
      {
        EthTrcv_LinkStateType linkState;

        /* #1110 Retrieve the link and if link state is ACTIVE map it to the related EthIf controllers */
        if( EthIf_GetGetLinkStateOfEthTrcvDrvApi(ethTrcvDrvApiIdx)((uint8)EthIf_GetSnvOfEthTrcv(ethTrcvIdx),
                                                                   &linkState) == E_OK )                                /* SBSW_ETHIF_DRVAPI_FCT_PTR_OPT_WITH_REF_OF_STACK_VAR_PARAM_PTR_CSL03 */
        {
          /* #11110 If transceiver link state is active */
          if( linkState == ETHTRCV_LINK_STATE_ACTIVE )
          {
            /* #111110 Set the link state of this port to ACTIVE */
            EthIf_SetTrcvLinkStateActive(ethTrcvIdx);
          }
          /* #11120 Else... */
          else
          {
            /* #111210 Set the link state of this port to DOWN */
            EthIf_SetTrcvLinkStateDown(ethTrcvIdx);
          }

          /* #11130 Increase link count for EthCtrls related to this transceiver */
          EthIf_IncEthIfCtrlsLinkCount(ethTrcvIdx);
        }
      }
    }
  }
} /* EthIf_PollTrcvLinks() */

/**********************************************************************************************************************
 *  EthIf_SetTrcvLinkStateActive
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
ETHIF_LINK_LOCAL_INLINE FUNC(void, ETHIF_LINK_INLINE_CODE) EthIf_SetTrcvLinkStateActive(
  EthIf_EthTrcvIterType ethTrcvIdx)
{
  /* #10 If the link state of this transceiver stored in the EthIf link state table is still DOWN */
  if(EthIf_GetEthTrcvLinkState(ethTrcvIdx) == ETHIF_PHYELEM_LINK_STATE_DOWN)
  {
    EthIf_PhyElemLinkStateType currentState = ETHIF_PHYELEM_LINK_STATE_ACTIVE;

# if ( ETHIF_ENABLE_MACSEC_API == STD_ON )
    /* #110 If the transceiver is secured */
    if ( EthIf_IsMacSecSecuredOfEthTrcv(ethTrcvIdx) == TRUE )
    {
      /* #1110 Retrieve the pae index */
      uint8 paeIdx = EthIf_GetMkaPaeIdxOfEthTrcv(ethTrcvIdx);
      currentState = ETHIF_PHYELEM_LINK_STATE_PHY_ACTIVE;
      /* #1120 Report to pae index and the link state ACTIVE to Mka */
      (void)Mka_LinkStateChange(paeIdx, ETHTRCV_LINK_STATE_ACTIVE);
    }
# endif /* ETHIF_ENABLE_MACSEC_API */

    /* #120 Update the link state of this transceiver stored in the EthIf link state table */
    EthIf_SetEthTrcvLinkState(ethTrcvIdx, currentState);                                                                /* SBSW_ETHIF_COMMON_CSL01 */
  }
} /* EthIf_SetTrcvLinkStateActive() */

/**********************************************************************************************************************
 *  EthIf_SetTrcvLinkStateDown
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
ETHIF_LINK_LOCAL_INLINE FUNC(void, ETHIF_LINK_INLINE_CODE) EthIf_SetTrcvLinkStateDown(
  EthIf_EthTrcvIterType ethTrcvIdx)
{
  /* #10 If this link state of this transceiver stored in the EthIf link state table is not DOWN */
  if( EthIf_GetEthTrcvLinkState(ethTrcvIdx) != ETHIF_PHYELEM_LINK_STATE_DOWN )
  {
# if ( ETHIF_ENABLE_MACSEC_API == STD_ON )
    /* #110 If the transceiver is secured */
    if( EthIf_IsMacSecSecuredOfEthTrcv(ethTrcvIdx) == TRUE)
    {
      /* #1110 Retrieve the pae index */
      uint8 paeIdx = EthIf_GetMkaPaeIdxOfEthTrcv(ethTrcvIdx);
      /* #1120 Report the pae index and the link state DOWN to Mka */
      (void)Mka_LinkStateChange(paeIdx, ETHTRCV_LINK_STATE_DOWN);
    }
# endif /* ETHIF_ENABLE_MACSEC_API */
    /* #120 Set the link state of this transceiver stored in the EthIf link state table to DOWN */
    EthIf_SetEthTrcvLinkState(ethTrcvIdx, ETHIF_PHYELEM_LINK_STATE_DOWN);                                               /* SBSW_ETHIF_COMMON_CSL01 */
  }
} /* EthIf_SetTrcvLinkStateDown() */

/**********************************************************************************************************************
 *  EthIf_IncEthIfCtrlsLinkCount
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
ETHIF_LINK_LOCAL_INLINE FUNC(void, ETHIF_LINK_INLINE_CODE) EthIf_IncEthIfCtrlsLinkCount(
  EthIf_EthTrcvIterType ethTrcvIdx)
{
  /* #10 If the link state of this transceiver stored in the EthIf link state table is not DOWN */
  if(EthIf_GetEthTrcvLinkState(ethTrcvIdx) != ETHIF_PHYELEM_LINK_STATE_DOWN)
  {
    EthIf_EthIfCtrlOfEthTrcvIndIterType EthIfCtrlIndirectionIdx;
    /* #110 Iterate the EthIfCtrl Indirection Index of this transceiver */
    for( EthIfCtrlIndirectionIdx = EthIf_GetEthIfCtrlOfEthTrcvIndStartIdxOfEthTrcv(ethTrcvIdx);
          EthIfCtrlIndirectionIdx < EthIf_GetEthIfCtrlOfEthTrcvIndEndIdxOfEthTrcv(ethTrcvIdx);
          EthIfCtrlIndirectionIdx++ )
    {
      EthIf_EthIfCtrlOfEthTrcvIndType ethIfCtrlIdx = EthIf_GetEthIfCtrlOfEthTrcvInd(EthIfCtrlIndirectionIdx);

      /* #1110 Increase the EthIfCtrlLink counter if the link state of this transceiver stored in the EthIf
              link state table is ACTIVE or no MACSec is enabled for this EthIfCtrl */
      if( (EthIf_GetEthTrcvLinkState(ethTrcvIdx) == ETHIF_PHYELEM_LINK_STATE_ACTIVE)
        || (EthIf_GetMacSecTypeOfEthIfCtrl(ethIfCtrlIdx) == ETHIF_NO_MACSEC_MACSECTYPEOFETHIFCTRL) )
      {
        EthIf_IncEthIfCtrlLinkCount(ethIfCtrlIdx);                                                                        /* SBSW_ETHIF_COMMON_CSL03 */
      }
    }
  }
}
#endif /* ETHIF_ETHTRCV_USED */

#if ( ETHIF_ETHSWT_USED == STD_ON )
/**********************************************************************************************************************
 *  EthIf_PollPortLinks
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
ETHIF_LINK_LOCAL_INLINE FUNC(void, ETHIF_LINK_INLINE_CODE) EthIf_PollPortLinks( void )
{
  /* ----- Local Variables ---------------------------------------------- */
  EthIf_EthSwtPortIterType portIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter exclusive area (Reason: Ensure consistency of mode and link state information) */
  EthIf_EnterExclusiveArea(SET_CTRL_MODE); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* #20 For each switch port */
  for( portIdx = 0U;
       portIdx < EthIf_GetSizeOfEthSwtPort();
       portIdx++ )
  {
    /* #210 If the switch port is in state ACTIVE */
    if( EthIf_GetEthSwtPortModeCnt(portIdx) > 0U )
    {
      EthTrcv_LinkStateType linkState;
      EthIf_EthSwtIdxOfEthSwtPortType ethSwtIdx = EthIf_GetEthSwtIdxOfEthSwtPort(portIdx);
      EthIf_EthSwtDrvApiIdxOfEthSwtType ethSwtDrvApiIdx = EthIf_GetEthSwtDrvApiIdxOfEthSwt(ethSwtIdx);

      /* #2110 Retrieve the link and if link state is ACTIVE map it to the related EthIf switch port groups */
      if( EthIf_GetGetLinkStateOfEthSwtDrvApi(ethSwtDrvApiIdx)((uint8)EthIf_GetSnvOfEthSwt(ethSwtIdx),
                                                               (uint8)EthIf_GetSnvOfEthSwtPort(portIdx),
                                                               &linkState) == E_OK )                                    /* SBSW_ETHIF_DRVAPI_FCT_PTR_MAN_WITH_REF_OF_STACK_VAR_PARAM_PTR_CSL03 */
      {
        /* #21110 Do the port link state handling */
          EthIf_PortLinkStateHandling((uint8)portIdx, linkState);
      }
    }
  }

  /* #30 Leave exclusive area -> processing on elements finished */
  EthIf_ExitExclusiveArea(SET_CTRL_MODE); /* PRQA S 3109 */ /* MD_MSR_14.3 */
} /* EthIf_PollPortLinks() */

/**********************************************************************************************************************
 *  EthIf_PortLinkStateHandling
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
ETHIF_LINK_LOCAL_INLINE FUNC(void, ETHIF_LINK_INLINE_CODE) EthIf_PortLinkStateHandling(
  uint8                 portIdx,
  EthTrcv_LinkStateType linkState)
{
  /* #10 If transceiver link state is active */
  if( linkState == ETHTRCV_LINK_STATE_ACTIVE )
  {
    boolean isHostPort = (EthIf_GetPortRoleOfEthSwtPort(portIdx) ==
                         ETHIF_PORT_ROLE_HOST_PORT_PORTROLEOFETHSWTPORT) ? TRUE : FALSE;
    boolean isHostUplinkPortActive = (EthIf_GetEthSwtPortLinkState(EthIf_GetHostOrUpLinkPortOfEthSwtPort(portIdx)) ==
                                     ETHIF_PHYELEM_LINK_STATE_ACTIVE) ? TRUE: FALSE;
    /* #110 If this port is host port or the link state of the host/uplink port of this port stored in the EthIf link
            state table is ACTIVE */
    if( isHostPort || isHostUplinkPortActive )
    {
      /* #1110 Set the link state of this port to ACTIVE */
      EthIf_SetPortLinkStateActive(portIdx);
    }
    /* #120 Increase port group count */
    EthIf_IncPortGroupLinkCount(portIdx);
  }
  /* #20 Else... */
  else
  {
    /* #210 Set the link state of this port to DOWN */
    EthIf_SetPortLinkStateDown(portIdx);
  }
} /* EthIf_PortLinkStateHandling() */

/**********************************************************************************************************************
 *  EthIf_SetPortLinkStateActive
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
ETHIF_LINK_LOCAL_INLINE FUNC(void, ETHIF_LINK_INLINE_CODE) EthIf_SetPortLinkStateActive(
  uint8 portIdx)
{
  EthIf_PhyElemLinkStateType currentState = ETHIF_PHYELEM_LINK_STATE_ACTIVE;
  /* #10 If the link state of this port stored in the EthIf link state table is still DOWN */
  if ( EthIf_GetEthSwtPortLinkState(portIdx) == ETHIF_PHYELEM_LINK_STATE_DOWN )
  {
# if ( ETHIF_ENABLE_MACSEC_API == STD_ON )
    /* #110 If the port is secured */
    if( EthIf_IsMacSecSecuredOfEthSwtPort(portIdx) == TRUE )
    {
      /* #1110 Retrieve the pae index */
      uint8 paeIdx = EthIf_GetMkaPaeIdxOfEthSwtPort(portIdx);
      currentState = ETHIF_PHYELEM_LINK_STATE_PHY_ACTIVE;
      /* #1120 Report to pae index and the link state ACTIVE to Mka */
      (void)Mka_LinkStateChange(paeIdx, ETHTRCV_LINK_STATE_ACTIVE);
    }
# endif /* ETHIF_ENABLE_MACSEC_API */
    /* #120 Update the link state of this port stored in the EthIf link state table */
    EthIf_SetEthSwtPortLinkState(portIdx, currentState);                                                                /* SBSW_ETHIF_INT_FCT_IDX_PASS */
  }
} /* EthIf_SetPortLinkStateActive() */

/**********************************************************************************************************************
 *  EthIf_SetPortLinkStateDown
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
ETHIF_LINK_LOCAL_INLINE FUNC(void, ETHIF_LINK_INLINE_CODE) EthIf_SetPortLinkStateDown(
  uint8 portIdx)
{
  /* #10 If this link state of this port stored in the EthIf link state table is not DOWN */
  if( EthIf_GetEthSwtPortLinkState(portIdx) != ETHIF_PHYELEM_LINK_STATE_DOWN )
  {
# if ( ETHIF_ENABLE_MACSEC_API == STD_ON )
    /* #110 If the port is secured */
    if( EthIf_IsMacSecSecuredOfEthSwtPort(portIdx) == TRUE)
    {
      /* #1110 Retrieve the pae index */
      uint8 paeIdx = EthIf_GetMkaPaeIdxOfEthSwtPort(portIdx);
      /* #1120 Report the pae index and the link state DOWN to Mka */
      (void)Mka_LinkStateChange(paeIdx, ETHTRCV_LINK_STATE_DOWN);
    }
# endif /* ETHIF_ENABLE_MACSEC_API */
    /* #120 Set the link state of this port stored in the EthIf link state table to DOWN */
    EthIf_SetEthSwtPortLinkState(portIdx, ETHIF_PHYELEM_LINK_STATE_DOWN);                                             /* SBSW_ETHIF_INT_FCT_IDX_PASS */
  }
} /* EthIf_SetPortLinkStateDown() */

/**********************************************************************************************************************
 *  EthIf_IncPortGroupLinkCount
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
ETHIF_LINK_LOCAL_INLINE FUNC(void, ETHIF_LINK_INLINE_CODE) EthIf_IncPortGroupLinkCount(
  uint8 portIdx)
{
  EthIf_PortGroupIndIterType ethIfPortGroupIndirectionIdx;
  /* #10 For each port in the context of Ethernet interface */
  for( ethIfPortGroupIndirectionIdx = EthIf_GetPortGroupIndStartIdxOfEthSwtPort(portIdx);
    ethIfPortGroupIndirectionIdx < EthIf_GetPortGroupIndEndIdxOfEthSwtPort(portIdx);
    ethIfPortGroupIndirectionIdx++ )
  {
    EthIf_PortGroupIterType portGroupIdx = EthIf_GetPortGroupInd(ethIfPortGroupIndirectionIdx);
    /* #110 If the lax link aggregation is enabled */
    if( EthIf_IsLaxLinkAggregationEnabledOfPortGroup(portGroupIdx) == TRUE )
    {
      /* #1110 Update the counter for port group lax link aggregation */
      EthIf_IncPortGroupLaxLinkAggregationCount(portGroupIdx);                                                        /* SBSW_ETHIF_COMMON_CSL03 */
    }
    /* #120 Else... */
    else
    {
      /* #1210 If this link state of this port stored in the EthIf link state table is ACTIVE */
      if( EthIf_GetEthSwtPortLinkState(portIdx) == ETHIF_PHYELEM_LINK_STATE_ACTIVE )
      {
        /* #12110 If this port is host port or up link port */
        if( (EthIf_GetPortRoleOfEthSwtPort(portIdx) == ETHIF_PORT_ROLE_HOST_PORT_PORTROLEOFETHSWTPORT) ||
          (EthIf_GetPortRoleOfEthSwtPort(portIdx) == ETHIF_PORT_ROLE_UP_LINK_PORT_PORTROLEOFETHSWTPORT))
        {
          /* #121110 Update the counter for host/uplink port in port group */
          EthIf_IncPortGroupHostOrUpLinkPortRoleLinkCount(portGroupIdx);                                              /* SBSW_ETHIF_COMMON_CSL03 */
        }
        /* #12120 Else... */
        else
        {
          /* #121210 Update the counter for no role port in port group */
          EthIf_IncPortGroupNoRoleLinkCount(portGroupIdx);                                                            /* SBSW_ETHIF_COMMON_CSL03 */
        }
      }
    }
  }
} /* EthIf_IncPortGroupLinkCount() */

/**********************************************************************************************************************
 *  EthIf_PollPortGroupLinks
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHIF_LINK_LOCAL_INLINE FUNC(void, ETHIF_LINK_INLINE_CODE) EthIf_PollPortGroupLinks( void )
{
  /* ----- Local Variables ---------------------------------------------- */
  EthIf_PortGroupIterType portGroupIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 For each switch port group */
  for( portGroupIdx = 0U;
       portGroupIdx < EthIf_GetSizeOfPortGroup();
       portGroupIdx++ )
  {
    /* #110 If the semantic of the port group is CONTROL or LINK INFO */
    if( (EthIf_GetSemanticOfPortGroup(portGroupIdx) == ETHIF_PORTGROUP_SEMANTIC_CONTROL_SEMANTICOFPORTGROUP) ||
         (EthIf_GetSemanticOfPortGroup(portGroupIdx) == ETHIF_PORTGROUP_SEMANTIC_LINK_INFO_SEMANTICOFPORTGROUP) )
    {
      /* #1110 If the port group is ACTIVE */
      if( EthIf_IsPortGroupLinkStateActive(portGroupIdx) == TRUE )
      {
        EthIf_EthIfCtrlOfPortGroupIndIterType ethIfCtrlIndirectionIdx;

        for( ethIfCtrlIndirectionIdx = EthIf_GetEthIfCtrlOfPortGroupIndStartIdxOfPortGroup(portGroupIdx);
              ethIfCtrlIndirectionIdx < EthIf_GetEthIfCtrlOfPortGroupIndEndIdxOfPortGroup(portGroupIdx);
              ethIfCtrlIndirectionIdx++ )
        {
          EthIf_EthIfCtrlOfPortGroupIndType ethIfCtrlIdx = EthIf_GetEthIfCtrlOfPortGroupInd(ethIfCtrlIndirectionIdx);

          EthIf_IncEthIfCtrlLinkCount(ethIfCtrlIdx);                                                                    /* SBSW_ETHIF_COMMON_CSL03 */
        }
      }
    }
  }
} /* EthIf_PollPortGroupLinks() */
#endif /* ETHIF_ETHSWT_USED */

/**********************************************************************************************************************
 *  EthIf_DetectLinkStateChangesOnEthIfCtrls
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
ETHIF_LINK_LOCAL_INLINE FUNC(void, ETHIF_LINK_INLINE_CODE) EthIf_DetectLinkStateChangesOnEthIfCtrls( void )
{
  EthIf_EthIfCtrlIterType ethIfCtrlIdx;

  /* #10 Enter exclusive area (Reason: Ensure consistency of controller mode and link state information) */
  EthIf_EnterExclusiveArea(SET_CTRL_MODE); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* #20 For each EthIf controller */
  for( ethIfCtrlIdx = 0;
       ethIfCtrlIdx < EthIf_GetSizeOfEthIfCtrl();
       ethIfCtrlIdx++ )
  {
    /* #200 If EthIf controller is ACTIVE */
    if( EthIf_GetEthIfCtrlMode(ethIfCtrlIdx) == ETH_MODE_ACTIVE )
    {
      /* #2000 If link is established for the needed amount of phy layer elements */
      if( EthIf_GetEthIfCtrlLinkCount(ethIfCtrlIdx) >= EthIf_GetLinkAggrThresholdOfEthIfCtrl(ethIfCtrlIdx) )
      {
        /* #20000 Issue a link state to UP if EthIf controller hasn't established a link before */
        if( EthIf_GetEthIfCtrlLinkState(ethIfCtrlIdx) != ETHIF_ETHIFCTRL_LINK_STATE_LINK )
        {
          /* issue a link up on the EthIf controller */
          EthIf_SetEthIfCtrlLinkState(ethIfCtrlIdx, ETHIF_ETHIFCTRL_LINK_STATE_LINK_CHANGE_UP);                         /* SBSW_ETHIF_COMMON_CSL01 */
        }
      }
      /* #2010 Otherwise (if link isn't established on the needed amount of phy layer elements) */
      else
      {
        /* #20100 Issue a link state to DOWN if EthIf controller has established a link before */
        if( EthIf_GetEthIfCtrlLinkState(ethIfCtrlIdx) != ETHIF_ETHIFCTRL_LINK_STATE_NO_LINK )
        {
          /* issue a link down on the EthIf controller */
          EthIf_SetEthIfCtrlLinkState(ethIfCtrlIdx, ETHIF_ETHIFCTRL_LINK_STATE_LINK_CHANGE_DOWN);                       /* SBSW_ETHIF_COMMON_CSL01 */
        }
      }
    }
  }

  /* #30 Leave exclusive area -> processing on elements finished */
  EthIf_ExitExclusiveArea(SET_CTRL_MODE); /* PRQA S 3109 */ /* MD_MSR_14.3 */
} /* EthIf_DetectLinkStateChangesOnEthIfCtrls() */

#if ( ETHIF_ETHSWT_USED == STD_ON )
/**********************************************************************************************************************
 *  EthIf_DetectLinkStateChangesOnEthIfSwtPortGroups
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
ETHIF_LINK_LOCAL_INLINE FUNC(void, ETHIF_LINK_INLINE_CODE) EthIf_DetectLinkStateChangesOnEthIfSwtPortGroups( void )
{
  EthIf_PortGroupIterType portGroupIdx;

  /* #10 Enter exclusive area (Reason: Ensure consistency of controller mode and link state information) */
  EthIf_EnterExclusiveArea(SET_CTRL_MODE); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* #20 For each EthIf switch port group */
  for( portGroupIdx = 0U;
       portGroupIdx < EthIf_GetSizeOfPortGroup();
       portGroupIdx++ )
  {
    /* #210 If EthIf switch port group is active and of semantic type independent */
    if ( (EthIf_GetPortGroupMode(portGroupIdx) == ETHTRCV_MODE_ACTIVE) &&
         (EthIf_GetSemanticOfPortGroup(portGroupIdx) == ETHIF_PORTGROUP_SEMANTIC_INDEPENDENT_SEMANTICOFPORTGROUP) )
    {
      /* #2110 If the port group is ACTIVE */
      if( EthIf_IsPortGroupLinkStateActive(portGroupIdx) == TRUE )
      {
        /* #21110 Issue a link state to UP if EthIf switch port group hasn't established a link before */
        if( EthIf_GetPortGroupLinkState(portGroupIdx) == ETHIF_PORTGROUP_LINK_STATE_NO_LINK )
        {
          /* issue a link up on the EthIf switch port group */
          EthIf_SetPortGroupLinkState(portGroupIdx, ETHIF_PORTGROUP_LINK_STATE_LINK_CHANGE_UP);                         /* SBSW_ETHIF_COMMON_CSL01 */
        }
      }
      /* #2120 Else... */
      else
      {
        /* #21210 Issue a link state to DOWN if EthIf switch port group has established a link before */
        if( EthIf_GetPortGroupLinkState(portGroupIdx) == ETHIF_PORTGROUP_LINK_STATE_LINK )
        {
          /* issue a link down on the EthIf switch port group */
          EthIf_SetPortGroupLinkState(portGroupIdx, ETHIF_PORTGROUP_LINK_STATE_LINK_CHANGE_DOWN);                       /* SBSW_ETHIF_COMMON_CSL01 */
        }
      }
    }
  }

  /* #30 Leave exclusive area -> processing on elements finished */
  EthIf_ExitExclusiveArea(SET_CTRL_MODE); /* PRQA S 3109 */ /* MD_MSR_14.3 */
} /* EthIf_DetectLinkStateChangesOnEthIfSwtPortGroups() */

/**********************************************************************************************************************
 *  EthIf_IsPortGroupLinkStateActive()
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
ETHIF_LINK_LOCAL_INLINE FUNC(boolean, ETHIF_LINK_INLINE_CODE) EthIf_IsPortGroupLinkStateActive(
  EthIf_PortGroupIterType  portGroupIdx)
{
  boolean retVal = FALSE;

  /* #10 If the Lax Link Aggregation is active */
  if( EthIf_IsLaxLinkAggregationEnabledOfPortGroup(portGroupIdx) == TRUE)
  {
    /* #110 If the counter (independent of role) is greater than threshold */
    if( EthIf_GetPortGroupLaxLinkAggregationCount(portGroupIdx) >= EthIf_GetLinkAggrThresholdOfPortGroup(portGroupIdx) )
    {
      /* #1110 Return TRUE */
      retVal = TRUE;
    }
  }
  else
  /* #20 Else... */
  {
    /* #210 If the Host or LinkUp counter is bigger or same as the Host/Uplink threshold of this port group,
            And 1) The No Role counter is bigger as 0 or 2) The Host/Uplink threshold is the same as the port group size */
    if( (EthIf_GetPortGroupHostOrUpLinkPortRoleLinkCount(portGroupIdx) >= EthIf_GetHostAndUpLinkPortThresholdOfPortGroup(portGroupIdx)) &&
            ((EthIf_GetPortGroupNoRoleLinkCount(portGroupIdx) >= EthIf_GetNoRolePortThresholdOfPortGroup(portGroupIdx))) )
    {
      /* #2110 Return TRUE */
      retVal = TRUE;
    }
  }
  return retVal;
} /* EthIf_IsPortGroupLinkStateActive() */
#endif /* ETHIF_ETHSWT_USED */

/**********************************************************************************************************************
 *  EthIf_DetectLinkStateChange
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHIF_LINK_LOCAL_INLINE FUNC(void, ETHIF_LINK_INLINE_CODE) EthIf_DetectLinkStateChange( void )
{
  /* #10 Trigger link counts initialization */
  EthIf_InitLinkCounts(); /* PRQA S 2987 */ /* MD_EthIf_2987 */

  /* #20 Trigger retrieval of the link states of all physical layer elements */
#if ( ETHIF_ETHTRCV_USED == STD_ON )                                                                                    /* COV_ETHIF_SAFE_FEAT */
  EthIf_PollTrcvLinks();
#endif /* ETHIF_ETHTRCV_USED */
#if ( ETHIF_ETHSWT_USED == STD_ON )
  EthIf_PollPortLinks();
  EthIf_PollPortGroupLinks(); /* PRQA S 2987 */ /* MD_EthIf_2987 */
#endif /* ETHIF_ETHSWT_USED */

  /* #30 Trigger detection of link state changes for EthIf controllers and EthIf switch port groups */
  EthIf_DetectLinkStateChangesOnEthIfCtrls();
#if ( ETHIF_ETHSWT_USED == STD_ON )
  EthIf_DetectLinkStateChangesOnEthIfSwtPortGroups();
#endif /* ETHIF_ETHSWT_USED */
} /* EthIf_DetectLinkStateChange() */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  EthIf_MainFunctionState
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, ETHIF_CODE) EthIf_MainFunctionState( void )
{
  /* #10 Check if module is initialized */
  if( EthIf_ModuleInitialized == ETHIF_STATE_INIT )
  {
    /* #100 Get all relevant links of transceivers and switch ports */
    EthIf_DetectLinkStateChange();

    /* #110 Process possible link changes */
    EthIf_ProcessLinkStateChangeForAllEthIfCtrls();

#if ( ETHIF_SWT_PORT_GROUP_MODE_REQ_SUPPORT == STD_ON )
    /* #120 Process possible link changes for EthIf switch port groups */
    EthIf_ProcessAllEthIfSwtPortGroupLinkStateChanges();
#endif /* ETHIF_SWT_PORT_GROUP_MODE_REQ_SUPPORT */
  }

#if ( ETHIF_ENABLE_SIGNAL_QUALITY_API == STD_ON )

  if( EthIf_GetSignalQualityDecoupleFreqCnt() == 0u )
  {
    EthIf_PollTrcvSignalQualities();
    EthIf_PollPortSignalQualities();
    EthIf_SetSignalQualityDecoupleFreqCnt(EthIf_GetSignalQualityChgMainReload());                                       /* SBSW_ETHIF_COMMON_CFG_PTR_INT_FCT_PREV_MODULE_INIT_CHECK */
  }
  /* Decrease polling frequency counter */
  EthIf_DecSignalQualityDecoupleFreqCnt();                                                                              /* SBSW_ETHIF_COMMON_CFG_PTR_INT_FCT_PREV_MODULE_INIT_CHECK */
#endif
} /* EthIf_MainFunctionState() */

#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: EthIf_Link.c
 *********************************************************************************************************************/

