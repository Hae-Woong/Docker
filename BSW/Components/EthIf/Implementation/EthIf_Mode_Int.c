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
/*!        \file  EthIf_Mode_Int.c
 *        \brief  EthIf Mode internal source file
 *
 *      \details  Contains the definition of data and implementation of functions of the sub-module Mode of EthIf
 *                only to be used EthIf internal.
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

#define ETHIF_MODE_INT_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/*********************************************************************************************************
 * Predefined file includes based on \trace DSGN-EthIfDiag1975
 *********************************************************************************************************/
#include "EthIf_Mode_Int.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

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
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  EthIf_Mode_Init
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
FUNC(void, ETHIF_CODE) EthIf_Mode_Init( void )
{
  EthIf_EthIfCtrlIterType  ethIfCtrlIdx;
  EthIf_EthCtrlIterType    ethCtrlIdx;
#if ( ETHIF_ETHTRCV_USED == STD_ON )                                                                                    /* COV_ETHIF_SAFE_FEAT */
  EthIf_EthTrcvIterType    ethTrcvIdx;
#endif /* ETHIF_ETHTRCV_USED */
#if ( ETHIF_ETHSWT_USED == STD_ON )
  EthIf_EthSwtIterType     ethSwtIdx;
  EthIf_EthSwtPortIterType ethSwtPortIdx;
  EthIf_PortGroupIterType  portGroupIdx;
#endif /* ETHIF_ETHSWT_USED */

  /* #10 Initialize the parallel processing protection semaphores for EthIf_ControllerInit()
   *     and EthIf_SetControllerMode() */
  EthIf_SetInitLocked(FALSE);                                                                                           /* SBSW_ETHIF_COMMON_CFG_PTR_INT_FCT_CALL_CONV */
  EthIf_SetModeChangeLocked(FALSE);                                                                                     /* SBSW_ETHIF_COMMON_CFG_PTR_INT_FCT_CALL_CONV */

  /* #20 Initialize mode to DOWN and init state to UNINIT for each EthIf controller */
  for( ethIfCtrlIdx = 0;
       ethIfCtrlIdx < EthIf_GetSizeOfEthIfCtrl();
       ethIfCtrlIdx++ )
  {
    EthIf_SetEthIfCtrlMode(ethIfCtrlIdx, ETH_MODE_DOWN);                                                                /* SBSW_ETHIF_COMMON_CSL01 */
    EthIf_SetEthIfCtrlInitState(ethIfCtrlIdx, ETHIF_ETHIFCTRL_INIT_STATE_UNINIT);                                       /* SBSW_ETHIF_COMMON_CSL01 */
  }

  /* #30 Initialize initialization and mode counts to 0 for each Ethernet controller */
  for( ethCtrlIdx = 0;
       ethCtrlIdx < EthIf_GetSizeOfEthCtrl();
       ethCtrlIdx++ )
  {
    EthIf_SetEthCtrlInitCnt(ethCtrlIdx, 0u);                                                                            /* SBSW_ETHIF_COMMON_CSL01 */
    EthIf_SetEthCtrlModeCnt(ethCtrlIdx, 0u);                                                                            /* SBSW_ETHIF_COMMON_CSL01 */
  }

#if ( ETHIF_ETHTRCV_USED == STD_ON )                                                                                    /* COV_ETHIF_SAFE_FEAT */
  /* #40 Initialize initialization and mode counts to 0 for each Ethernet transceiver */
  for( ethTrcvIdx = 0;
       ethTrcvIdx < EthIf_GetSizeOfEthTrcv();
       ethTrcvIdx++ )
  {
    EthIf_SetEthTrcvInitCnt(ethTrcvIdx, 0u);                                                                            /* SBSW_ETHIF_COMMON_CSL01 */
    EthIf_SetEthTrcvModeCnt(ethTrcvIdx, 0u);                                                                            /* SBSW_ETHIF_COMMON_CSL01 */
  }
#endif /* ETHIF_ETHTRCV_USED */

#if ( ETHIF_ETHSWT_USED == STD_ON )
  /* #50 Initialize initialization counts to 0 for each Ethernet switch */
  for( ethSwtIdx = 0;
       ethSwtIdx < EthIf_GetSizeOfEthSwt();
       ethSwtIdx++ )
  {
    EthIf_SetEthSwtInitCnt(ethSwtIdx, 0u);                                                                              /* SBSW_ETHIF_COMMON_CSL01 */
  }

  /* #60 Initialize mode counts to 0 for each Ethernet switch port */
  for( ethSwtPortIdx = 0;
       ethSwtPortIdx < EthIf_GetSizeOfEthSwtPort();
       ethSwtPortIdx++ )
  {
    EthIf_SetEthSwtPortModeCnt(ethSwtPortIdx, 0u);                                                                      /* SBSW_ETHIF_COMMON_CSL01 */
  }

  /* #70 Initialize mode counts to 0 for each EthIf switch port group */
  for( portGroupIdx = 0;
       portGroupIdx < EthIf_GetSizeOfPortGroup();
       portGroupIdx++ )
  {
    EthIf_SetPortGroupMode(portGroupIdx, ETHTRCV_MODE_DOWN);                                                            /* SBSW_ETHIF_COMMON_CSL01 */
  }

  /* #80 Initialize port start active timer related counters and flags */
  EthIf_SetPortStartupActiveTimeCounter(0u);                                                                            /* SBSW_ETHIF_COMMON_CFG_PTR_INT_FCT_CALL_CONV */
  EthIf_SetPortStartupActiveTimerActive(FALSE);                                                                         /* SBSW_ETHIF_COMMON_CFG_PTR_INT_FCT_CALL_CONV */

#endif /* ETHIF_ETHSWT_USED */
} /* EthIf_Mode_Init() */

#if ( ETHIF_ETHSWT_USED == STD_ON )
/**********************************************************************************************************************
 *  EthIf_AdjustEthIfSwtPortGroupModeCount
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
FUNC(void, ETHIF_MODE_INLINE_CODE) EthIf_AdjustEthIfSwtPortGroupModeCount(
  EthIf_PortGroupIterType  portGroupIdx,
  EthTrcv_ModeType         targetMode)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter exclusive area (Reason: Ensure consistency of mode and link state information) */
  EthIf_EnterExclusiveArea(SET_CTRL_MODE); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* #20 Check if EthIf switch port group is of semantic type control or independent */
  if( (EthIf_GetSemanticOfPortGroup(portGroupIdx) == ETHIF_PORTGROUP_SEMANTIC_CONTROL_SEMANTICOFPORTGROUP) ||
      (EthIf_GetSemanticOfPortGroup(portGroupIdx) == ETHIF_PORTGROUP_SEMANTIC_INDEPENDENT_SEMANTICOFPORTGROUP) )
  {
    /* #210 Check if action for ETHTRCV_MODE_ACTIVE should be done */
    if( targetMode == ETHTRCV_MODE_ACTIVE )
    {
      EthIf_EthSwtPortIndIterType indIdx = 0u;

      /* #2100 Increment mode counter of each port in the ethSwtPortGroup */
      for( indIdx = EthIf_GetEthSwtPortIndStartIdxOfPortGroup(portGroupIdx);
           indIdx < EthIf_GetEthSwtPortIndEndIdxOfPortGroup(portGroupIdx);
           indIdx++ )
      {
        EthIf_EthSwtPortIndType portIdx = EthIf_GetEthSwtPortInd(indIdx);

        EthIf_IncEthSwtPortModeCnt(portIdx);                                                                            /* SBSW_ETHIF_COMMON_CSL03 */
      }
    }
    /* #220 Check if action for ETHTRCV_MODE_DOWN should be done */
    else
    {
      EthIf_EthSwtPortIndIterType indIdx = 0u;

      /* #2200 Decrement mode counter of each port in the ethSwtPortGroup */
      for( indIdx = EthIf_GetEthSwtPortIndStartIdxOfPortGroup(portGroupIdx);
           indIdx < EthIf_GetEthSwtPortIndEndIdxOfPortGroup(portGroupIdx);
           indIdx++ )
      {
        EthIf_EthSwtPortIndType portIdx = EthIf_GetEthSwtPortInd(indIdx);

        EthIf_DecEthSwtPortModeCnt(portIdx);                                                                            /* SBSW_ETHIF_COMMON_CSL03 */
      }
    }
  }

  /* #30 Leave exclusive area -> processing on elements finished */
  EthIf_ExitExclusiveArea(SET_CTRL_MODE); /* PRQA S 3109 */ /* MD_MSR_14.3 */
} /* EthIf_AdjustEthIfSwtPortGroupModeCount() */

/**********************************************************************************************************************
 *  EthIf_SetEthIfSwtPortGroupMode
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
FUNC(Std_ReturnType, ETHIF_MODE_INLINE_CODE) EthIf_SetEthIfSwtPortGroupMode( /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
  EthIf_PortGroupIterType  portGroupIdx,
  EthTrcv_ModeType         targetMode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if ethSwtPortGroup has semantic type link info */
  if( EthIf_GetSemanticOfPortGroup(portGroupIdx) == ETHIF_PORTGROUP_SEMANTIC_LINK_INFO_SEMANTICOFPORTGROUP )
  {
    retVal = E_OK;
  }
  /* #20 Check if ethSwtPortGroup has semantic type control or independent */
  else if( (EthIf_GetSemanticOfPortGroup(portGroupIdx) == ETHIF_PORTGROUP_SEMANTIC_CONTROL_SEMANTICOFPORTGROUP) ||
           (EthIf_GetSemanticOfPortGroup(portGroupIdx) == ETHIF_PORTGROUP_SEMANTIC_INDEPENDENT_SEMANTICOFPORTGROUP) )
  {
    /* #200 Check if action for ETHTRCV_MODE_ACTIVE should be done */
    if( targetMode == ETHTRCV_MODE_ACTIVE )
    {
      EthIf_EthSwtPortIndIterType indIdx = 0u;

      /* #2000 Switch port mode to ETHTRCV_MODE_ACTIVE for each still inactive port of the ethSwtPortGroup */
      for( indIdx = EthIf_GetEthSwtPortIndStartIdxOfPortGroup(portGroupIdx);
           indIdx < EthIf_GetEthSwtPortIndEndIdxOfPortGroup(portGroupIdx);
           indIdx++ )
      {
        EthIf_EthSwtPortIndType portIdx = EthIf_GetEthSwtPortInd(indIdx);

        if( EthIf_GetEthSwtPortModeCnt(portIdx) == ETHIF_MIN_ETHSWTPORTMODECNT )
        {
          EthIf_EthSwtIdxOfEthSwtPortType ethSwtIdx = EthIf_GetEthSwtIdxOfEthSwtPort(portIdx);
          EthIf_EthSwtDrvApiIdxOfEthSwtType ethSwtDrvApiIdx = EthIf_GetEthSwtDrvApiIdxOfEthSwt(ethSwtIdx);

          retVal = EthIf_GetSetSwtPortModeOfEthSwtDrvApi(ethSwtDrvApiIdx)((uint8)EthIf_GetSnvOfEthSwt(ethSwtIdx),
                                                                          (uint8)EthIf_GetSnvOfEthSwtPort(portIdx),
                                                                          ETHTRCV_MODE_ACTIVE);                         /* SBSW_ETHIF_DRVAPI_FCT_PTR_MAN_CSL03 */
        }
        else
        {
          retVal = E_OK;
        }

        if ( retVal != E_OK )
        {
          break;
        }
      }

      /* #2010 If switching of port mode was successful and port group semantic is independent set port group mode to ACTIVE */
      if( (retVal == E_OK) &&
          (EthIf_GetSemanticOfPortGroup(portGroupIdx) == ETHIF_PORTGROUP_SEMANTIC_INDEPENDENT_SEMANTICOFPORTGROUP) )
      {
        EthIf_SetPortGroupMode(portGroupIdx, ETHTRCV_MODE_ACTIVE);                                                      /* SBSW_ETHIF_INT_FCT_IDX_PASS */
      }
    }
    /* #300 Check if action for ETHTRCV_MODE_DOWN should be done */
    else
    {
      EthIf_EthSwtPortIndIterType indIdx = 0u;
      boolean changePortGroupLinkState = FALSE;

      /* #3000 Switch port mode to ETHTRCV_MODE_DOWN for each still active port of the ethSwtPortGroup */
      for( indIdx = EthIf_GetEthSwtPortIndStartIdxOfPortGroup(portGroupIdx);
           indIdx < EthIf_GetEthSwtPortIndEndIdxOfPortGroup(portGroupIdx);
           indIdx++ )
      {
        EthIf_EthSwtPortIndType portIdx = EthIf_GetEthSwtPortInd(indIdx);

        if( EthIf_GetEthSwtPortModeCnt(portIdx) == ETHIF_DRV_DOWN_REQUEST_MODE_CNT )
        {
          EthIf_EthSwtIdxOfEthSwtPortType ethSwtIdx = EthIf_GetEthSwtIdxOfEthSwtPort(portIdx);
          EthIf_EthSwtDrvApiIdxOfEthSwtType ethSwtDrvApiIdx = EthIf_GetEthSwtDrvApiIdxOfEthSwt(ethSwtIdx);

          retVal = EthIf_GetSetSwtPortModeOfEthSwtDrvApi(ethSwtDrvApiIdx)((uint8)EthIf_GetSnvOfEthSwt(ethSwtIdx),
                                                                          (uint8)EthIf_GetSnvOfEthSwtPort(portIdx),
                                                                          ETHTRCV_MODE_DOWN);                           /* SBSW_ETHIF_DRVAPI_FCT_PTR_MAN_CSL03 */
          changePortGroupLinkState = TRUE;
        }
        else
        {
          retVal = E_OK;
        }

        if ( retVal != E_OK )
        {
          break;
        }
      }

      /* #3010 If switching of port mode was successful and port group semantic is independent set port group mode to DOWN */
      if( (retVal == E_OK) &&
          (EthIf_GetSemanticOfPortGroup(portGroupIdx) == ETHIF_PORTGROUP_SEMANTIC_INDEPENDENT_SEMANTICOFPORTGROUP) &&
          (changePortGroupLinkState == TRUE) )
      {
        /* #30110 Enter exclusive area (Reason: Ensure consistency of controller mode and link state information) */
        EthIf_EnterExclusiveArea(SET_CTRL_MODE); /* PRQA S 3109 */ /* MD_MSR_14.3 */

        /* #30120 Update mode and link state information */
        EthIf_SetPortGroupMode(portGroupIdx, ETHTRCV_MODE_DOWN);                                                        /* SBSW_ETHIF_INT_FCT_IDX_PASS */

        if( EthIf_GetPortGroupLinkState(portGroupIdx) == ETHIF_PORTGROUP_LINK_STATE_LINK )
        {
          EthIf_SetPortGroupLinkState(portGroupIdx, ETHIF_PORTGROUP_LINK_STATE_LINK_CHANGE_DOWN);                       /* SBSW_ETHIF_INT_FCT_IDX_PASS */
        }

        /* #30130 Leave exclusive area -> processing on elements finished */
        EthIf_ExitExclusiveArea(SET_CTRL_MODE); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      }
    }
  }
  else
  {
    retVal = E_NOT_OK;
  }

  return retVal;
} /* EthIf_SetEthIfSwtPortGroupMode() */

/**********************************************************************************************************************
 *  EthIf_DeactivateAllPortGroups
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
FUNC(void, ETHIF_MODE_INLINE_CODE) EthIf_DeactivateAllPortGroups( void )
{
  Std_ReturnType retVal = E_NOT_OK;
  EthIf_PortGroupIterType portGroupIdx = 0u;

  /* #10 For each EthIf switch port group */
  for( portGroupIdx = 0;
      portGroupIdx < EthIf_GetSizeOfPortGroup();
      portGroupIdx++ )
  {
    /* #110 Set switch port group mode to down */
    retVal = EthIf_SetEthIfSwtPortGroupMode(portGroupIdx, ETHTRCV_MODE_DOWN);

    if ( retVal == E_OK )
    {
      /* #1110 Adjusting mode counter values of the ports in the port group */
      EthIf_AdjustEthIfSwtPortGroupModeCount(portGroupIdx, ETHTRCV_MODE_DOWN);

      /* #1120 If the port group is managed by upper layer */
      if (EthIf_GetSemanticOfPortGroup(portGroupIdx) == ETHIF_PORTGROUP_SEMANTIC_INDEPENDENT_SEMANTICOFPORTGROUP)
      {
        /* #11210 Immediately propagate the link state change to DOWN the upper layers */
        EthIf_ProcessSwtPortGroupLinkStateChange(portGroupIdx);
      }
    }
  }
}
#endif /* ETHIF_ETHSWT_USED */

#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: EthIf_Mode_Int.c
 *********************************************************************************************************************/

