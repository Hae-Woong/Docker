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
/*!         \file    ComM_PNC_SignalProcessing.c
 *          \unit    PNC_SignalProcessing
 *          \brief   Communication Manager ASR4
 *
 *          \details  Implementation of the Autosar Communication Manager for the PNC_SignalProcessing unit.
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

#define CCL_ASR_COMM_PNC_SIGNALPROCESSING_SOURCE

/**********************************************************************************************************************
 *  LOCAL MISRA / PCLINT JUSTIFICATIONS
 *********************************************************************************************************************/
/* PRQA S 2991, 2992, 2995, 2996 EOF */ /* MD_MSR_ConstantCondition */
/* PRQA S 6050 EOF */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
*  INCLUDES
*********************************************************************************************************************/
#include "ComM_PNC_SignalProcessing.h"
#include "ComM_PNC_General.h"
#include "ComM_ModeInhibition.h"
#include "SchM_ComM.h"

#if (COMM_MULTIPARTITION == STD_ON)
# include "ComM_MultiPartitionSynchronization.h"
#endif

#include "ComM_Util.h"

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
 *  ROM DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define COMM_START_SEC_CODE
 /* PRQA S 5087 1 */ /* MD_MSR_MemMap */
#include "ComM_MemMap.h"

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

#if ( COMM_PNC_SUPPORT == STD_ON )
/**********************************************************************************************************************
 *  ComM_PncModeSendSignal()
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
FUNC( void, COMM_CODE ) ComM_PncModeSendSignal( ComM_PncIterType PncHandle, boolean activeState, uint8 SigSendType )
{
  /* ----- Implementation ----------------------------------------------- */
  if ( ComM_HasEiraTxSignalIndirection() )
  {
    /* ----- Local Variables ---------------------------------------------- */
    ComM_PncSignalIndEndIdxOfPncPbType signalMappingIndex;
    ComM_SizeOfPncSignalType signalIndex;
    NetworkHandleType channel = 0;

    /* #10 Iterate over EIRA_TX signals mapped to the given PNC. Send the corresponding COM signal if
    *     1) the GW type of the corresponding channel matches the given SigSendType or
    *     2) all EIRA_TX signals have to be sent.
    *     The signal value (PNC bit) is set according to the PNC activation given by activeState.
    *     If PNC Routing Limitation is enabled, PNC activation can be set only if routing is not limited on the channel. */

    for ( signalMappingIndex = ComM_GetPncSignalIndStartIdxOfPncPb( PncHandle ); signalMappingIndex < ComM_GetPncSignalIndEndIdxOfPncPb( PncHandle ); signalMappingIndex++ ) /* FETA_COMM_CSL_USE_CASE */
    {
      signalIndex = ComM_GetPncSignalInd( signalMappingIndex );
      if ( ComM_GetTypeOfPncSignal( signalIndex ) == COMM_EIRA_TX_TYPEOFPNCSIGNAL ) /* Signal Type can be EIRA_TX or ERA_RX */
      {
        channel = ComM_GetChannelIdxOfPncSignal( signalIndex );
        if ( (!ComM_HasPncGwTypePassive()) || ((SigSendType == ComM_GetGwTypeOfChannel( channel )) || (SigSendType == COMM_SEND_ALL_SIG)) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
        {
          if ( activeState == TRUE )
          {
            if ( (!ComM_HasPncToChannelRoutingState( COMM_MASTER_PARTITION_IDX )) || (ComM_GetPncToChannelRoutingState( channel, COMM_MASTER_PARTITION_IDX ) != COMM_PNCROUTING_LIMITED) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
            {
              ComM_PncSetBitInSignal( signalIndex, PncHandle, COMM_MASTER_PARTITION_IDX );
# if (COMM_MULTIPARTITION == STD_ON)
              if ( ComM_GetPartitionConfigIdxOfChannel( channel ) != COMM_MASTER_PARTITION_IDX )
              {
                SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_5();
                /* @ assert channel < ComM_GetSizeOfChannel(); */ /* VCA_COMM_CHANNEL_CSL03 */
                ComM_SetForwardComSendSignalSyncCounter( channel, (uint8)(ComM_GetForwardComSendSignalSyncCounter( channel, ComM_GetPartitionConfigIdxOfChannel( channel ) ) + 1u), COMM_MASTER_PARTITION_IDX );
                ComM_SetForwardingComSendSignalIndex( channel, signalIndex, COMM_MASTER_PARTITION_IDX );
                SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_5();
              }
              else
# endif
              {
                (void)Com_SendSignal( ComM_GetSymbolicNameValueOfEiraTxSignalIndirection( signalIndex ), ComM_GetAddrPncSignalValues( ComM_GetPncSignalValuesStartIdxOfPncSignal( signalIndex ), COMM_MASTER_PARTITION_IDX ) ); /* PRQA S 0315 */ /* MD_ComM_0315 */
              }
            }
            ComM_PncNmRequestHandling( channel, PncHandle );
          }
          else
          {
            ComM_PncClearBitInSignal( signalIndex, PncHandle, COMM_MASTER_PARTITION_IDX );
# if (COMM_MULTIPARTITION == STD_ON)
            if ( ComM_GetPartitionConfigIdxOfChannel( channel ) != COMM_MASTER_PARTITION_IDX )
            {
              SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_5();
              /* @ assert channel < ComM_GetSizeOfChannel(); */ /* VCA_COMM_CHANNEL_CSL03 */
              ComM_SetForwardComSendSignalSyncCounter( channel, (uint8)(ComM_GetForwardComSendSignalSyncCounter( channel, ComM_GetPartitionConfigIdxOfChannel( channel ) ) + 1u), COMM_MASTER_PARTITION_IDX );
              ComM_SetForwardingComSendSignalIndex( channel, signalIndex, COMM_MASTER_PARTITION_IDX );
              SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_5();
            }
            else
# endif
            {
              (void)Com_SendSignal( ComM_GetSymbolicNameValueOfEiraTxSignalIndirection( signalIndex ), ComM_GetAddrPncSignalValues( ComM_GetPncSignalValuesStartIdxOfPncSignal( signalIndex ), COMM_MASTER_PARTITION_IDX ) ); /* PRQA S 0315 */ /* MD_ComM_0315 */
            }
          }
        }
      }
    }
  }

  COMM_DUMMY_STATEMENT( SigSendType ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  COMM_DUMMY_STATEMENT( activeState ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  COMM_DUMMY_STATEMENT( PncHandle ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
}


/**********************************************************************************************************************
 *  ComM_PncProcessRxSignalEira()
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
FUNC( void, COMM_CODE ) ComM_PncProcessRxSignalEira( ComM_SizeOfPncSignalType InternalSignalId, const uint8 *SignalDataRx, CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx )
{
  /* ----- Implementation ----------------------------------------------- */
  if ( ComM_HasPncEiraRx() )
  {
    /* ----- Local Variables ---------------------------------------------- */
    ComM_PncSignalIterType signalIndex;
    ComM_SizeOfPncType pncIndex;

    /* #10 Iterate over PNCs and update the target PNC state. */
    for ( pncIndex = 0u; pncIndex < ComM_GetSizeOfPnc(); pncIndex++ ) /* FETA_COMM_CSL_USE_CASE */
    {
      if ( (SignalDataRx[ComM_GetSignalByteIndexOfPncPb( pncIndex )] & ComM_GetSetMaskOfPnc( pncIndex )) != 0u )
      {
        /* #110 If a PNC is requested via EIRA RX, set the target state to COMM_PNC_READY_SLEEP. */
# if ( COMM_MULTIPARTITION == STD_ON )
        if ( partitionIdx != COMM_MASTER_PARTITION_IDX )
        {
          SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_4();
          ComM_SetBusPncComModeReq( pncIndex, COMM_PNC_READY_SLEEP, partitionIdx );
          /* Increment sync counter for ComM_BusPncComModeReq by 1 (relative to the corresponding Master-Counter) */
          ComM_SetBusPncComModeReqSyncCounter( pncIndex,
            (uint8)(ComM_GetBusPncComModeReqMasterSyncCounter( (uint8)(ComM_GetBusPncComModeReqMasterSyncCounterStartIdxOfPncPartitionIndirection( partitionIdx ) + pncIndex), COMM_MASTER_PARTITION_IDX ) + 1u),
                                               partitionIdx );
          SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_4();
        }
        else
# endif
        {
          ComM_SetBusPncComModeReq( pncIndex, COMM_PNC_READY_SLEEP, partitionIdx );
        }

        ComM_PncSetBitInSignal( InternalSignalId, pncIndex, partitionIdx );
      }
      else
      {
        /* #120 If a PNC is released via EIRA RX, check if it is requested on other bus types.
         *      If it is released on all other bus types, set the target state to COMM_PNC_NO_COMMUNICATION. */
        uint8 SigValuesArb = 0u; /* PRQA S 2983 */ /* MD_ComM_2983 */
        ComM_PncClearBitInSignal( InternalSignalId, pncIndex, partitionIdx );

        if ( ComM_GetPncEiraRxCount( COMM_MASTER_PARTITION_IDX ) > 1u ) /* If only one EIRA RX (bus type), no need to loop over signals */
        {
          SigValuesArb = 0u;
          for ( signalIndex = 0u; signalIndex < ComM_GetSizeOfPncSignal(); signalIndex++ ) /* FETA_COMM_CSL_USE_CASE */
          {
            if ( ComM_GetTypeOfPncSignal( signalIndex ) == COMM_EIRA_RX_TYPEOFPNCSIGNAL )
            {
              SigValuesArb |= ComM_GetPncSignalValues( (uint8)(ComM_GetPncSignalValuesStartIdxOfPncSignal( signalIndex ) + ComM_GetSignalByteIndexOfPncPb( pncIndex )), partitionIdx ); /* PRQA S 2985 */ /* MD_ComM_2985 */
            }
          }
        }

        if ( (ComM_GetPncEiraRxCount( COMM_MASTER_PARTITION_IDX ) == 1u) || ((SigValuesArb & ComM_GetSetMaskOfPnc( pncIndex )) == 0u) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
        {
# if ( COMM_MULTIPARTITION == STD_ON )
          if ( partitionIdx != COMM_MASTER_PARTITION_IDX )
          {
            SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_4();
            ComM_SetBusPncComModeReq( pncIndex, COMM_PNC_NO_COMMUNICATION, partitionIdx );
            /* Increment sync counter for ComM_BusPncComModeReq by 1 (relative to the corresponding Master-Counter) */
            ComM_SetBusPncComModeReqSyncCounter( pncIndex,
              (uint8)(ComM_GetBusPncComModeReqMasterSyncCounter( (uint8)(ComM_GetBusPncComModeReqMasterSyncCounterStartIdxOfPncPartitionIndirection( partitionIdx ) + pncIndex), COMM_MASTER_PARTITION_IDX ) + 1u),
                                                 partitionIdx );
            SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_4();
          }
          else
# endif
          {
            ComM_SetBusPncComModeReq( pncIndex, COMM_PNC_NO_COMMUNICATION, partitionIdx );
          }
        }
      }
    }
  }

  COMM_DUMMY_STATEMENT_CONST( SignalDataRx ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  COMM_DUMMY_STATEMENT( InternalSignalId ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
}

/**********************************************************************************************************************
 *  ComM_PncProcessRxSignalEra()
 **********************************************************************************************************************/
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
FUNC( void, COMM_CODE ) ComM_PncProcessRxSignalEra( ComM_SizeOfPncSignalType InternalSignalId, const uint8 *SignalDataRx )
{
  uint8 SigValuesArb;
  ComM_SizeOfPncType pncIndex;
  ComM_PncSignalIndEndIdxOfPncPbType ComM_PncSignalMappingIndex;
  ComM_PncSignalIterType signalIndex;
  ComM_UserIterType systemUser;
  ComM_UserPncByteMaskEndIdxOfUserType iByteMaskPncIndex;
  ComM_UserReqPncFullComType userReqPncFullCom;
  ComM_UserReqPncFullComEndIdxOfPncPbType userReqPncFullComIndex;
  ComM_UserByteMaskIterType iByteMaskUserIndex;
  ComM_UserReqFullComType userReqFullCom;
  ComM_UserReqFullComIdxOfUserByteMaskType userReqFullComIdx;

  /* #10 Iterate over PNCs and request the system user accordingly. */
  for ( pncIndex = 0u; pncIndex < ComM_GetSizeOfPnc(); pncIndex++ ) /* FETA_COMM_CSL_USE_CASE */
  {
    if ( ComM_IsCoordinatedOfPnc( pncIndex ) ) /*lint !e506 */
    {
      /* #110 If Era is received on channel with GW type NONE, the request is ignored.
       *      Note: Generator ensures that either all channels of the coordinated PNC have GW type NONE or no channel has GW type NONE.
              Managed Channels, mapped to a PNC, always have GW type NONE and thus cannot be coordinated, and therfore do not contribute to the coordination of the PNC. */
      if ( ComM_GetGwTypeOfChannel( ComM_GetChannelIdxOfPncSignal( InternalSignalId ) ) != COMM_GATEWAY_TYPE_NONE )
      {

        systemUser = (ComM_UserIterType)ComM_GetSystemUserOfPnc( pncIndex );
        if ( (SignalDataRx[ComM_GetSignalByteIndexOfPncPb( pncIndex )] & ComM_GetSetMaskOfPnc( pncIndex )) > 0u ) /* PRQA S 2823 1 */ /* MD_ComM_2823 */
        {
          /* #120 If a PNC is requested via ERA, request the corresponding system user. */
          ComM_PncSetBitInSignal( InternalSignalId, pncIndex, ComM_GetPartitionConfigIdxOfChannel( ComM_GetChannelIdxOfPncSignal( InternalSignalId ) ) );

          /* It is insured by the validator that the generated System User of the affected Pnc is always on the COMM_MASTER_PARTITION_IDX */
          SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();
          for ( iByteMaskUserIndex = ComM_GetUserByteMaskStartIdxOfUser( systemUser ); iByteMaskUserIndex < ComM_GetUserByteMaskEndIdxOfUser( systemUser ); iByteMaskUserIndex++ ) /* FETA_COMM_CSL_USE_CASE */
          {
            userReqFullComIdx = ComM_GetUserReqFullComIdxOfUserByteMask( iByteMaskUserIndex );
            userReqFullCom = (ComM_UserReqFullComType)(ComM_GetUserReqFullCom( userReqFullComIdx, COMM_MASTER_PARTITION_IDX ) | ComM_GetSetMaskOfUserByteMask( iByteMaskUserIndex ));

            /* @ assert userReqFullComIdx < ComM_GetSizeOfUserReqFullCom( COMM_MASTER_PARTITION_IDX ); */ /* VCA_COMM_USERREQFULLCOM_IDX */
            ComM_SetUserReqFullCom( userReqFullComIdx, userReqFullCom, COMM_MASTER_PARTITION_IDX );
          }

          for ( iByteMaskPncIndex = ComM_GetUserPncByteMaskStartIdxOfUser( systemUser ); iByteMaskPncIndex < ComM_GetUserPncByteMaskEndIdxOfUser( systemUser ); iByteMaskPncIndex++ ) /* FETA_COMM_CSL_USE_CASE */
          {
            userReqPncFullComIndex = ComM_GetUserReqPncFullComIdxOfUserPncByteMask( iByteMaskPncIndex );
            userReqPncFullCom = (ComM_UserReqPncFullComType)(ComM_GetUserReqPncFullCom( userReqPncFullComIndex, COMM_MASTER_PARTITION_IDX ) | ComM_GetSetMaskOfUserPncByteMask( iByteMaskPncIndex ));

            ComM_SetUserReqPncFullCom( userReqPncFullComIndex, userReqPncFullCom, COMM_MASTER_PARTITION_IDX );
          }
          SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();
        }
        else
        {
          /* #130 If a PNC is released on all related ERA signals, release the corresponding system user. */
          ComM_PncClearBitInSignal( InternalSignalId, pncIndex, ComM_GetPartitionConfigIdxOfChannel( ComM_GetChannelIdxOfPncSignal( InternalSignalId ) ) );

          SigValuesArb = 0u;
          for ( ComM_PncSignalMappingIndex = ComM_GetPncSignalIndStartIdxOfPncPb( pncIndex ); ComM_PncSignalMappingIndex < ComM_GetPncSignalIndEndIdxOfPncPb( pncIndex ); ComM_PncSignalMappingIndex++ ) /* FETA_COMM_CSL_USE_CASE */
          {
            signalIndex = ComM_GetPncSignalInd( ComM_PncSignalMappingIndex );
            if ( ComM_GetTypeOfPncSignal( signalIndex ) == COMM_ERA_RX_TYPEOFPNCSIGNAL )
            {
#if (COMM_MULTIPARTITION == STD_ON)
              SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_4();
              if ( ComM_GetPncSignalValuesSyncCounter( (ComM_SizeOfPncSignalValuesSyncCounterType)(ComM_GetPncSignalValuesStartIdxOfPncSignal( signalIndex ) + ComM_GetSignalByteIndexOfPncPb( pncIndex )), /* PRQA S 2985 */ /* MD_ComM_2985 */
                ComM_GetPartitionConfigIdxOfChannel( ComM_GetChannelIdxOfPncSignal( signalIndex ) ) )
                == (uint8)(ComM_GetPncSignalValuesSyncCounter( (ComM_SizeOfPncSignalValuesSyncCounterType)(ComM_GetPncSignalValuesStartIdxOfPncSignal( signalIndex ) + ComM_GetSignalByteIndexOfPncPb( pncIndex )), /* PRQA S 2985 */ /* MD_ComM_2985 */
                  COMM_MASTER_PARTITION_IDX ) + 1u) )
              {
                SigValuesArb |= ComM_GetPncSignalValues( (ComM_SizeOfPncSignalValuesType)(ComM_GetPncSignalValuesStartIdxOfPncSignal( signalIndex ) + ComM_GetSignalByteIndexOfPncPb( pncIndex )), /* PRQA S 2985, 2823 */ /* MD_ComM_2985, MD_ComM_2823 */
                  ComM_GetPartitionConfigIdxOfChannel( ComM_GetChannelIdxOfPncSignal( signalIndex ) ) );
              }
              else
#endif
              {
                SigValuesArb |= ComM_GetPncSignalValues( (ComM_SizeOfPncSignalValuesType)(ComM_GetPncSignalValuesStartIdxOfPncSignal( signalIndex ) + ComM_GetSignalByteIndexOfPncPb( pncIndex )), /* PRQA S 2985, 2823 */ /* MD_ComM_2985, MD_ComM_2823 */
                  COMM_MASTER_PARTITION_IDX );
              }
#if (COMM_MULTIPARTITION == STD_ON)
              SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_4();
#endif
            }
          }
          if ( (SigValuesArb & ComM_GetSetMaskOfPnc( pncIndex )) == 0u )
          {
            SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();
            for ( iByteMaskUserIndex = ComM_GetUserByteMaskStartIdxOfUser( systemUser ); iByteMaskUserIndex < ComM_GetUserByteMaskEndIdxOfUser( systemUser ); iByteMaskUserIndex++ ) /* FETA_COMM_CSL_USE_CASE */
            {
              userReqFullComIdx = ComM_GetUserReqFullComIdxOfUserByteMask( iByteMaskUserIndex );
              userReqFullCom = (ComM_UserReqFullComType)(ComM_GetUserReqFullCom( userReqFullComIdx, COMM_MASTER_PARTITION_IDX ) & ComM_GetClearMaskOfUserByteMask( iByteMaskUserIndex ));

              /* @ assert userReqFullComIdx < ComM_GetSizeOfUserReqFullCom( COMM_MASTER_PARTITION_IDX ); */ /* VCA_COMM_USERREQFULLCOM_IDX */
              ComM_SetUserReqFullCom( userReqFullComIdx, userReqFullCom, COMM_MASTER_PARTITION_IDX );
            }

            for ( iByteMaskPncIndex = ComM_GetUserPncByteMaskStartIdxOfUser( systemUser ); iByteMaskPncIndex < ComM_GetUserPncByteMaskEndIdxOfUser( systemUser ); iByteMaskPncIndex++ ) /* FETA_COMM_CSL_USE_CASE */
            {
              userReqPncFullComIndex = ComM_GetUserReqPncFullComIdxOfUserPncByteMask( iByteMaskPncIndex );
              userReqPncFullCom = (ComM_UserReqPncFullComType)(ComM_GetUserReqPncFullCom( userReqPncFullComIndex, COMM_MASTER_PARTITION_IDX ) & ComM_GetClearMaskOfUserPncByteMask( iByteMaskPncIndex ));

              ComM_SetUserReqPncFullCom( userReqPncFullComIndex, userReqPncFullCom, COMM_MASTER_PARTITION_IDX );
            }
            SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();
          }
        }
      }
    }
  }
}


/***********************************************************************************************************************
 *  ComM_PncClearBitInSignal()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_PncClearBitInSignal( ComM_SizeOfPncSignalType InternalSignalId, ComM_PncIterType InternalPncId, CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx )
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Calculate the new value of PNC signal byte by setting the corresponding PNC bit to 0. */
  ComM_SizeOfPncSignalValuesType signalByteIndex = (ComM_SizeOfPncSignalValuesType)(ComM_GetPncSignalValuesStartIdxOfPncSignal( InternalSignalId ) + ComM_GetSignalByteIndexOfPncPb( InternalPncId )); /* PRQA S 2985 */ /* MD_ComM_2985 */

  /* #20 Update the PNC byte in the signal with the new value. */
  if ( (signalByteIndex < ComM_GetSizeOfPncSignalValues( COMM_MASTER_PARTITION_IDX ))
# if (COMM_MULTIPARTITION == STD_ON)
       && (partitionIdx < ComM_GetSizeOfPartitionIdentifiers())
# endif
       )
  {
    /* #20 Calculate the initial value of the signalByte from the master partition.
     *     In case of MultiPartition, assign the PNC byte from the signal with the new value in the satellite partition data.
     *     If the signalByte was previously calculated on the satellite partition,
     *     get the signalValue from the satellite and reevaluate the signalByte value.
     */
    ComM_PncSignalValuesType signalByte = (ComM_PncSignalValuesType)(ComM_GetPncSignalValues( signalByteIndex, COMM_MASTER_PARTITION_IDX ) & ComM_GetClearMaskOfPnc( InternalPncId ));
# if (COMM_MULTIPARTITION == STD_ON)
    if ( partitionIdx != COMM_MASTER_PARTITION_IDX )
    {
      SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_4();
      if ( ComM_GetPncSignalValuesSyncCounter( signalByteIndex, partitionIdx ) == (uint8)(ComM_GetPncSignalValuesSyncCounter( signalByteIndex, COMM_MASTER_PARTITION_IDX ) + 1u) )
      {
        signalByte = (ComM_PncSignalValuesType)(ComM_GetPncSignalValues( signalByteIndex, partitionIdx ) & ComM_GetClearMaskOfPnc( InternalPncId ));
        ComM_SetPncSignalValues( signalByteIndex, signalByte, partitionIdx );
      }
      else
      {
        ComM_SetPncSignalValues( signalByteIndex, signalByte, partitionIdx );
        ComM_SetPncSignalValuesSyncCounter( signalByteIndex, (uint8)(ComM_GetPncSignalValuesSyncCounter( signalByteIndex, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );
      }
      SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_4();
    }
    else
# endif
    {
      ComM_SetPncSignalValues( signalByteIndex, signalByte, COMM_MASTER_PARTITION_IDX );
    }
  }

  COMM_DUMMY_STATEMENT( InternalPncId ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  COMM_DUMMY_STATEMENT( InternalSignalId ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  COMM_DUMMY_STATEMENT_CONST( partitionIdx ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
}


/***********************************************************************************************************************
 *  ComM_PncSetBitInSignal()
 **********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_PncSetBitInSignal( ComM_SizeOfPncSignalType InternalSignalId, ComM_PncIterType InternalPncId, CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx )
{
  /* #10 Calculate the new value of PNC signal byte by setting the corresponding PNC bit to 1. */
  ComM_SizeOfPncSignalValuesType signalByteIndex = (ComM_SizeOfPncSignalValuesType)(ComM_GetPncSignalValuesStartIdxOfPncSignal( InternalSignalId ) + ComM_GetSignalByteIndexOfPncPb( InternalPncId )); /* PRQA S 2985 */ /* MD_ComM_2985 */

  if ( (signalByteIndex < ComM_GetSizeOfPncSignalValues( partitionIdx ))
# if (COMM_MULTIPARTITION == STD_ON)
    && (partitionIdx < ComM_GetSizeOfPartitionIdentifiers())
# endif
    )
  {
    /* #20 Calculate the initial value of the signalByte from the master partition.
     *     In case of MultiPartition, assign the PNC byte from the signal with the new value in the satellite partition data.
     *     If the signalByte was previously calculated on the satellite partition,
     *     get the signalValue from the satellite and reevaluate the signalByte value.
     */
    ComM_PncSignalValuesType signalByte = (ComM_PncSignalValuesType)(ComM_GetPncSignalValues( signalByteIndex, COMM_MASTER_PARTITION_IDX ) | ComM_GetSetMaskOfPnc( InternalPncId ));
# if (COMM_MULTIPARTITION == STD_ON)
    if ( partitionIdx != COMM_MASTER_PARTITION_IDX )
    {
      SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_4();
      if ( ComM_GetPncSignalValuesSyncCounter( signalByteIndex, partitionIdx ) == (uint8)(ComM_GetPncSignalValuesSyncCounter( signalByteIndex, COMM_MASTER_PARTITION_IDX ) + 1u) )
      {
        signalByte = (ComM_PncSignalValuesType)(ComM_GetPncSignalValues( signalByteIndex, partitionIdx ) | ComM_GetSetMaskOfPnc( InternalPncId ));
        ComM_SetPncSignalValues( signalByteIndex, signalByte, partitionIdx );
      }
      else
      {
        ComM_SetPncSignalValues( signalByteIndex, signalByte, partitionIdx );
        ComM_SetPncSignalValuesSyncCounter( signalByteIndex, (uint8)(ComM_GetPncSignalValuesSyncCounter( signalByteIndex, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );
      }
      SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_4();
    }
    else
# endif
    {
      ComM_SetPncSignalValues( signalByteIndex, signalByte, COMM_MASTER_PARTITION_IDX );
    }
  }

  COMM_DUMMY_STATEMENT( InternalPncId ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  COMM_DUMMY_STATEMENT( InternalSignalId ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  COMM_DUMMY_STATEMENT_CONST( partitionIdx ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
}

/**********************************************************************************************************************
 *  ComM_PncIsEraReceivedOnActiveChannel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( boolean, COMM_CODE ) ComM_PncIsEraReceivedOnActiveChannel( ComM_PncIterType pncIndex )
{
  /* ----- Local Variable ---------------------------------------------- */
  boolean retValue = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  if ( ComM_HasPncGwTypeActive() )
  {
    /* ----- Local Variables ---------------------------------------------- */
    ComM_PncSignalIndEndIdxOfPncPbType pncSignalIndIter;
    ComM_SizeOfPncSignalType  pncSignalInd;
    ComM_GwTypeOfChannelType gwTypeOfChannel;

    /* #10 Return TRUE if at least one PNC request is received via ERA on a channel with GW type ACTIVE. Otherwise return FALSE. */
    /* Note: if PNC is coordinated if an ERA signal is mapped to it */
    for ( pncSignalIndIter = ComM_GetPncSignalIndStartIdxOfPncPb( pncIndex ); /* FETA_COMM_CSL_USE_CASE */
          pncSignalIndIter < ComM_GetPncSignalIndEndIdxOfPncPb( pncIndex );
          pncSignalIndIter++ )
    {
      pncSignalInd = ComM_GetPncSignalInd( pncSignalIndIter );
      if ( ComM_GetTypeOfPncSignal( pncSignalInd ) == COMM_ERA_RX_TYPEOFPNCSIGNAL )
      {
        gwTypeOfChannel = ComM_GetGwTypeOfChannel( ComM_GetChannelIdxOfPncSignal( pncSignalInd ) );
        if ( (gwTypeOfChannel == COMM_GATEWAY_TYPE_ACTIVE) && (ComM_IsSignalStoredPncBitSet( pncSignalInd, pncIndex, COMM_MASTER_PARTITION_IDX )) ) /* PRQA S 3415, 2985 */ /* MD_ComM_3415, MD_ComM_2985 */
        {
          retValue = TRUE;
          break;
        }
      }
    }
  }

  COMM_DUMMY_STATEMENT( pncIndex ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */

  return retValue;
}

/**********************************************************************************************************************
 *  ComM_PncIsEraRequestReceivedOnChannel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
 /* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC( boolean, COMM_CODE ) ComM_PncIsEraRequestReceivedOnChannel( NetworkHandleType Channel )
{
  /* ----- Local Variable ---------------------------------------------- */
  boolean isEraRequestReceivedOnChannel = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  if ( ComM_HasPncToChannelRoutingState( COMM_MASTER_PARTITION_IDX ) )
  {
    /* ----- Local Variables ---------------------------------------------- */
    ComM_PncSignalValuesEndIdxOfPncSignalType pncSignalByteIndex;
    ComM_SizeOfPncSignalType pncSignalIndex;

    for ( pncSignalIndex = 0u; pncSignalIndex < ComM_GetSizeOfPncSignal(); pncSignalIndex++ ) /* FETA_COMM_CSL_USE_CASE */
    {
      if ( (ComM_GetTypeOfPncSignal( pncSignalIndex ) == COMM_ERA_RX_TYPEOFPNCSIGNAL) && (ComM_GetChannelIdxOfPncSignal( pncSignalIndex ) == Channel) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
      {
        for ( pncSignalByteIndex = ComM_GetPncSignalValuesStartIdxOfPncSignal( pncSignalIndex ); pncSignalByteIndex < ComM_GetPncSignalValuesEndIdxOfPncSignal( pncSignalIndex ); pncSignalByteIndex++ ) /* FETA_COMM_CSL_USE_CASE */
        {
          if ( ComM_GetPncSignalValues( pncSignalByteIndex, COMM_MASTER_PARTITION_IDX ) != 0x00u )
          {
            /* #10 Return TRUE if at least one PNC is requested via ERA_RX signal on the given channel. Return FALSE otherwise. */
            isEraRequestReceivedOnChannel = TRUE;
            break;
          }
        }
        break; /* Because there is one ERA_RX signal per channel at most. */
      }
    }
  }

  COMM_DUMMY_STATEMENT( Channel ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */

  return isEraRequestReceivedOnChannel;
}
#endif


#define COMM_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
#include "ComM_MemMap.h"
