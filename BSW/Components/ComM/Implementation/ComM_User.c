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
/*!         \file     ComM_User.c
 *          \unit     User
 *          \brief    Communication Manager ASR4
 *
 *          \details  Implementation of the Autosar Communication Manager for the User unit
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

#define CCL_ASR_COMM_USER_SOURCE

/**********************************************************************************************************************
 *  LOCAL MISRA / PCLINT JUSTIFICATIONS
 *********************************************************************************************************************/
/* PRQA S 2991, 2992, 2995, 2996 EOF */ /* MD_MSR_ConstantCondition */
/* PRQA S 6050 EOF */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ComM.h"
#include "ComM_User.h"
#include "ComM_ModeInhibition.h"
#include "ComM_ErrorReporting.h"
#include "ComM_Util.h"
#include "SchM_ComM.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
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

#if (COMM_PNC_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  ComM_IsUserMappedToPncWithoutChannels()
 *********************************************************************************************************************/
/*! \brief      Verifies if the provided PNC 'User' is mapped to PNCs without channels.
 *  \details    -
 *  \param[in]  ComM_UserHandleType        User
 *  \pre        -
 *  \context    TASK|ISR1
 *  \reentrant  TRUE
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE ) ComM_IsUserMappedToPncWithoutChannels( ComM_UserIterType User );
#endif


/**********************************************************************************************************************
 *  HELPER FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

#if (COMM_PNC_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  ComM_IsUserMappedToPncWithoutChannels()
 ********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
 /* PRQA S 6080 1 */ /* MD_MSR_STMIF */
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE ) ComM_IsUserMappedToPncWithoutChannels( ComM_UserIterType User )
{
  /* ----- Local Variable ---------------------------------------------- */
  boolean isUserMappedToPncWithoutChannels = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  if ( ComM_HasPncExtendedFunctionality() )
  {
    /* ----- Local Variables ---------------------------------------------- */
    ComM_UserByteMaskEndIdxOfUserType iByteMaskIndex;
    ComM_PncIdxOfUserPncByteMaskType pncIdx;

    if ( ComM_IsPncUserOfUser( User ) )
    {
      if ( ComM_HasPncChannelMapping() )
      {
        /* #10 Check special usecase: this PNC user is mapped to PNCs without channels. */
        for ( iByteMaskIndex = ComM_GetUserPncByteMaskStartIdxOfUser( User ); iByteMaskIndex < ComM_GetUserPncByteMaskEndIdxOfUser( User ); iByteMaskIndex++ ) /* FETA_COMM_CSL_USE_CASE */
        {
          /* #110 Get the internal PNC index from the given PNC identifier. */
          pncIdx = ComM_GetPncIdxOfUserPncByteMask( iByteMaskIndex );
          /* #120 The flag is set to true if PNC has no channels. */
          if ( !ComM_IsPncChannelMappingUsedOfPncPb( pncIdx ) )
          {
            isUserMappedToPncWithoutChannels = TRUE;
            break;
          }
        }
      }
      else
      {
        /* #20 The flag is set to true, if the PNC does not have a Channel mapping. */
        isUserMappedToPncWithoutChannels = TRUE;
      }
    }
  }

  COMM_DUMMY_STATEMENT( User ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */

  return isUserMappedToPncWithoutChannels;
}
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/*********************************************************************************************************************
 *  ComM_User_RequestComMode()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
 /* PRQA S 6010, 6030 1 */ /* MD_MSR_STPTH, MD_MSR_STCYC */
FUNC( Std_ReturnType, COMM_CODE ) ComM_User_RequestComMode( ComM_UserIterType User, ComM_ModeType ComMode, CONST( ComM_PCPartitionConfigIdxOfPartitionIdentifiersType, AUTOMATIC ) partitionIdx )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
#if (COMM_PNC_SUPPORT == STD_ON)
  ComM_UserPncByteMaskEndIdxOfUserType iByteMaskPncIndex;
  ComM_UserReqPncFullComType userReqPncFullCom;
  ComM_UserReqPncFullComIdxOfUserPncByteMaskType userReqPncFullComIdx;
#endif
  ComM_UserByteMaskEndIdxOfUserType iByteMaskUserIndex;
  ComM_UserReqFullComType userReqFullCom;
  ComM_UserReqFullComIdxOfUserByteMaskType userReqFullComIdx;

  /* ----- Implementation ----------------------------------------------- */
  SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();

  /* #10 If COMM_NO_COMMUNICATION is requested, clear the user request in each related byte of UserReqFullCom and UserReqPncFullCom. */
  if ( ComMode == COMM_NO_COMMUNICATION )
  {
    retVal = E_OK;
#if (COMM_MULTIPARTITION == STD_ON)
    if ( partitionIdx != COMM_MASTER_PARTITION_IDX )
    {
      ComM_SetForwardUserReqSyncCounter( User, (uint8)(ComM_GetForwardUserReqSyncCounter( User, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );
      ComM_SetForwardingUserReqMode( User, COMM_NO_COMMUNICATION, partitionIdx ); 
    }
#endif

    for ( iByteMaskUserIndex = ComM_GetUserByteMaskStartIdxOfUser( User ); iByteMaskUserIndex < ComM_GetUserByteMaskEndIdxOfUser( User ); iByteMaskUserIndex++ ) /* FETA_COMM_CSL_USE_CASE */
    {
      userReqFullCom = (ComM_UserReqFullComType)(ComM_GetUserReqFullCom( ComM_GetUserReqFullComIdxOfUserByteMask( iByteMaskUserIndex ), partitionIdx ) & ComM_GetClearMaskOfUserByteMask( iByteMaskUserIndex ));
      userReqFullComIdx = ComM_GetUserReqFullComIdxOfUserByteMask( iByteMaskUserIndex );
#if ( COMM_MULTIPARTITION == STD_ON)
      if ( partitionIdx != COMM_MASTER_PARTITION_IDX )
      {
        /* @ assert userReqFullComIdx < ComM_GetSizeOfUserReqFullCom( COMM_MASTER_PARTITION_IDX ); */ /* VCA_COMM_USERREQFULLCOM_IDX */
        ComM_SetUserReqFullCom( userReqFullComIdx, userReqFullCom, partitionIdx );
      }
      else
#endif
      {
        /* @ assert userReqFullComIdx < ComM_GetSizeOfUserReqFullCom( COMM_MASTER_PARTITION_IDX ); */ /* VCA_COMM_USERREQFULLCOM_IDX */
        ComM_SetUserReqFullCom( userReqFullComIdx, userReqFullCom, COMM_MASTER_PARTITION_IDX );
      }
    }

#if (COMM_PNC_SUPPORT == STD_ON)
    if ( ComM_HasPncPb() && ComM_IsPncUserOfUser( User ) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
    {
      for ( iByteMaskPncIndex = ComM_GetUserPncByteMaskStartIdxOfUser( User ); iByteMaskPncIndex < ComM_GetUserPncByteMaskEndIdxOfUser( User ); iByteMaskPncIndex++ ) /* FETA_COMM_CSL_USE_CASE */
      {
        userReqPncFullCom = (ComM_UserReqPncFullComType)(ComM_GetUserReqPncFullCom( ComM_GetUserReqPncFullComIdxOfUserPncByteMask( iByteMaskPncIndex ), partitionIdx ) & ComM_GetClearMaskOfUserPncByteMask( iByteMaskPncIndex ));
        userReqPncFullComIdx = ComM_GetUserReqPncFullComIdxOfUserPncByteMask( iByteMaskPncIndex );
# if ( COMM_MULTIPARTITION == STD_ON)
        if ( partitionIdx != COMM_MASTER_PARTITION_IDX )
        {
          ComM_SetUserReqPncFullCom( userReqPncFullComIdx, userReqPncFullCom, partitionIdx );
        }
        else
# endif
        {
          ComM_SetUserReqPncFullCom( userReqPncFullComIdx, userReqPncFullCom, COMM_MASTER_PARTITION_IDX );
        }
      }
    }
#endif
  }
  else
  {
    retVal = E_OK;

    /* #20 If COMM_FULL_COMMUNICATION is requested, set the user request in each related byte of UserReqFullCom and UserReqPncFullCom. */
#if (COMM_MULTIPARTITION == STD_ON)
    if ( partitionIdx != COMM_MASTER_PARTITION_IDX )
    {
      ComM_SetForwardUserReqSyncCounter( User, (uint8)(ComM_GetForwardUserReqSyncCounter( User, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );
      ComM_SetForwardingUserReqMode( User, COMM_FULL_COMMUNICATION, partitionIdx );
    }
#endif

    for ( iByteMaskUserIndex = ComM_GetUserByteMaskStartIdxOfUser( User ); iByteMaskUserIndex < ComM_GetUserByteMaskEndIdxOfUser( User ); iByteMaskUserIndex++ ) /* FETA_COMM_CSL_USE_CASE */
    {
      userReqFullCom = (ComM_UserReqFullComType)(ComM_GetUserReqFullCom( ComM_GetUserReqFullComIdxOfUserByteMask( iByteMaskUserIndex ), partitionIdx ) | ComM_GetSetMaskOfUserByteMask( iByteMaskUserIndex ));
      userReqFullComIdx = ComM_GetUserReqFullComIdxOfUserByteMask( iByteMaskUserIndex );
#if (COMM_MULTIPARTITION == STD_ON)
      if ( partitionIdx != COMM_MASTER_PARTITION_IDX )
      {
        /* @ assert userReqFullComIdx < ComM_GetSizeOfUserReqFullCom( COMM_MASTER_PARTITION_IDX ); */ /* VCA_COMM_USERREQFULLCOM_IDX */
        ComM_SetUserReqFullCom( userReqFullComIdx, userReqFullCom, partitionIdx );
      }
      else
#endif
      {
        /* @ assert userReqFullComIdx < ComM_GetSizeOfUserReqFullCom( COMM_MASTER_PARTITION_IDX ); */ /* VCA_COMM_USERREQFULLCOM_IDX */
        ComM_SetUserReqFullCom( userReqFullComIdx, userReqFullCom, COMM_MASTER_PARTITION_IDX );
      }

      /* #70 If Mode Limitation NO_COM or Prevent Wake-up is active, ComM stores the FULL_COM request but does not perform it. */
      if ( ComM_ChannelIsLimited( ComM_GetChannelOfUserByteMask( iByteMaskUserIndex ) ) == TRUE )
      {
        retVal = COMM_E_MODE_LIMITATION;
      }
    }

#if (COMM_PNC_SUPPORT == STD_ON)
    if ( ComM_HasPncPb() && ComM_IsPncUserOfUser( User ) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
    {
      for ( iByteMaskPncIndex = ComM_GetUserPncByteMaskStartIdxOfUser( User ); iByteMaskPncIndex < ComM_GetUserPncByteMaskEndIdxOfUser( User ); iByteMaskPncIndex++ ) /* FETA_COMM_CSL_USE_CASE */
      {
        userReqPncFullCom = (ComM_UserReqPncFullComType)(ComM_GetUserReqPncFullCom( ComM_GetUserReqPncFullComIdxOfUserPncByteMask( iByteMaskPncIndex ), partitionIdx ) | ComM_GetSetMaskOfUserPncByteMask( iByteMaskPncIndex ));
        userReqPncFullComIdx = ComM_GetUserReqPncFullComIdxOfUserPncByteMask( iByteMaskPncIndex );
# if (COMM_MULTIPARTITION == STD_ON)
        if ( partitionIdx != COMM_MASTER_PARTITION_IDX )
        {
          ComM_SetUserReqPncFullCom( userReqPncFullComIdx, userReqPncFullCom, partitionIdx );
        }
        else
# endif
        {
          ComM_SetUserReqPncFullCom( userReqPncFullComIdx, userReqPncFullCom, COMM_MASTER_PARTITION_IDX );
        }
      }
    }
#endif
  }

  /* #40 If FULL_COM request is inhibited, increment the inhibition counter and store it in NvM. */
  if ( retVal == COMM_E_MODE_LIMITATION )
  {
    ComM_InhibitionPtrType ComMInhibition = ComM_GetAddrInhibition( COMM_MASTER_PARTITION_IDX );

    if ( ComMInhibition->ComM_InhibitCnt < 65535u )
    {
      ComMInhibition->ComM_InhibitCnt++;
      ComM_SetNvMStoreReq( TRUE, COMM_MASTER_PARTITION_IDX );
    }
  }

  SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();

  COMM_DUMMY_STATEMENT_CONST( partitionIdx ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */

  return retVal;
}

/*********************************************************************************************************************
 *  ComM_User_GetMaxComMode()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_User_GetMaxComMode( ComM_UserIterType User, P2VAR( ComM_ModeType, AUTOMATIC, COMM_APPL_VAR ) ComMode )
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get the maximum allowed communication mode of the user:
  *      NO_COM if Mode Limitation NO_COM or Prevent Wake-up is active on at least one channel mapped to the user,
  *      FULL_COM otherwise. */
  *ComMode = (ComM_ModeType)COMM_FULL_COMMUNICATION;

  if ( ComM_Channel_HasModeLimitationNoNoneNmTypeOrWakeupInhibition() )
  {
    /* ----- Local Variable ---------------------------------------------- */
    ComM_UserByteMaskEndIdxOfUserType iByteMaskIndex;

    for ( iByteMaskIndex = ComM_GetUserByteMaskStartIdxOfUser( User ); iByteMaskIndex < ComM_GetUserByteMaskEndIdxOfUser( User ); iByteMaskIndex++ ) /* FETA_COMM_CSL_USE_CASE */
    {
      if ( ComM_ChannelIsLimited( ComM_GetChannelOfUserByteMask( iByteMaskIndex ) ) == TRUE )
      {
        *ComMode = (ComM_ModeType)COMM_NO_COMMUNICATION;
      }
    }
  }

  COMM_DUMMY_STATEMENT_CONST( User ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
}

/*********************************************************************************************************************
 *  ComM_User_GetRequestedComMode()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_User_GetRequestedComMode( ComM_UserIterType User, P2VAR( ComM_ModeType, AUTOMATIC, COMM_APPL_VAR ) ComMode, CONST( ComM_PCPartitionConfigIdxOfPartitionIdentifiersType, AUTOMATIC ) partitionIdx )
{
  /* ----- Local Variable ---------------------------------------------- */
  ComM_UserByteMaskEndIdxOfUserType iByteMaskIndex;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get the requested mode of the user:
  *      NO_COM if at least one channel mapped to the user is not requested or
  *      if this PNC user is mapped to PNCs without channels (special case) and at least one PNC is not requested.
  *      FULL_COM otherwise. */
  SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();

  /* Set the default value to NO_COM to consider users having no channels and no PNCs, in this case the function does not change it. ESCAN00092198 */
  *ComMode = COMM_NO_COMMUNICATION;
#if (COMM_PNC_SUPPORT == STD_ON)
  if ( ComM_IsUserMappedToPncWithoutChannels( User ) == TRUE )
  {
    /* Special case: this PNC user is mapped to PNCs without channels */
    for ( iByteMaskIndex = ComM_GetUserPncByteMaskStartIdxOfUser( User ); iByteMaskIndex < ComM_GetUserPncByteMaskEndIdxOfUser( User ); iByteMaskIndex++ ) /* FETA_COMM_CSL_USE_CASE */
    {
      if ( (ComM_GetUserReqPncFullCom( ComM_GetUserReqPncFullComIdxOfUserPncByteMask( iByteMaskIndex ), partitionIdx ) & ComM_GetSetMaskOfUserPncByteMask( iByteMaskIndex )) != COMM_NO_REQUESTS )
      {
        *ComMode = COMM_FULL_COMMUNICATION;
      }
      else
      {
        *ComMode = COMM_NO_COMMUNICATION;
        break;
      }
    }
  }
  else
#endif
  {
    for ( iByteMaskIndex = ComM_GetUserByteMaskStartIdxOfUser( User ); iByteMaskIndex < ComM_GetUserByteMaskEndIdxOfUser( User ); iByteMaskIndex++ ) /* FETA_COMM_CSL_USE_CASE */
    {
      if ( (ComM_GetUserReqFullCom( ComM_GetUserReqFullComIdxOfUserByteMask( iByteMaskIndex ), partitionIdx ) & ComM_GetSetMaskOfUserByteMask( iByteMaskIndex )) != COMM_NO_REQUESTS )
      {
        *ComMode = COMM_FULL_COMMUNICATION;
      }
      else
      {
        *ComMode = COMM_NO_COMMUNICATION;
        break;
      }
    }
  }

  SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();
}

/*********************************************************************************************************************
 *  ComM_User_GetCurrentComMode()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, COMM_CODE ) ComM_User_GetCurrentComMode( ComM_UserIterType User, P2VAR( ComM_ModeType, AUTOMATIC, COMM_APPL_VAR ) ComMode )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = COMM_E_NO_ERROR;
  ComM_UserByteMaskEndIdxOfUserType iByteMaskIndex;
  ComM_ModeType tmpComMode = COMM_NO_COMMUNICATION; /* ESCAN00116362 */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get the lowest BusSM mode of all channels mapped to the user. */
  SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();

  /* Set the default value to NO_COM to consider users having no channels and no PNCs, in this case the function does not change it. ESCAN00092198 */
  *ComMode = COMM_NO_COMMUNICATION; /* PRQA S 2982 */ /* MD_ComM_2982 */
  for ( iByteMaskIndex = ComM_GetUserByteMaskStartIdxOfUser( User ); iByteMaskIndex < ComM_GetUserByteMaskEndIdxOfUser( User ); iByteMaskIndex = ComM_GetUserByteMaskEndIdxOfUser( User ) ) /* FETA_COMM_CSL_USE_CASE */
  {
    /* If user has channels or PNCs, set the default value to FULL_COM by executing this loop exactly once. */
    *ComMode = COMM_FULL_COMMUNICATION;
  }

  for ( iByteMaskIndex = ComM_GetUserByteMaskStartIdxOfUser( User ); iByteMaskIndex < ComM_GetUserByteMaskEndIdxOfUser( User ); iByteMaskIndex++ ) /* FETA_COMM_CSL_USE_CASE */
  {
    retVal = ComM_GetGetCurrentBusSMModeApiOfChannel( ComM_GetChannelOfUserByteMask( iByteMaskIndex ) )(ComM_GetChannelOfUserByteMask( iByteMaskIndex ), &tmpComMode); /* VCA_COMM_CALL_EXTERNAL_FUNCTION_POINTER_ARGUMENT */

    if ( retVal != E_OK )
    {
      errorId = COMM_E_ERROR_IN_PROV_SERVICE;
      break;
    }
    else
    {
      if ( tmpComMode < *ComMode )
      {
        *ComMode = tmpComMode;
      }
    }
  }
  SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();

  if ( errorId == COMM_E_NO_ERROR )
  {
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
  ComM_ErrorReporting_ProcessDetErrorReport( COMM_SID_GETCURRENTCOMMODE, errorId );

  return retVal;
}

#if (COMM_PNC_SUPPORT == STD_ON)
/*********************************************************************************************************************
 *  ComM_User_GetCurrentPNCComMode()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC( Std_ReturnType, COMM_CODE ) ComM_User_GetCurrentPNCComMode( ComM_UserIterType User, P2VAR( ComM_ModeType, AUTOMATIC, COMM_APPL_VAR ) ComMode )
{
  /* ----------------------- Local Variables ----------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  ComM_PncIdxOfUserPncByteMaskType pncIdx;

  /* ------------------------ Implementation ------------------------------- */
  /* Set the default communication mode value to NO_COM.*/
  *ComMode = COMM_NO_COMMUNICATION;

  /* #10 Check if 'User' is a PNC user. */
  if ( !ComM_IsPncUserOfUser( User ) )
  {
    retVal = E_NO_PNC_ASSIGNED;
  }
  /* #20 Check if 'User' is assigned to multiple PNCs. */
  else if ( ComM_GetUserPncByteMaskLengthOfUser( User ) > 1u )
  {
    retVal = E_MULTIPLE_PNC_ASSIGNED;
  }
  else
  {
    retVal = E_OK;
    pncIdx = ComM_GetPncIdxOfUserPncByteMask( ComM_GetUserPncByteMaskStartIdxOfUser( User ) );
    /* #30 If there is exactly one PNC assigned to 'User', set the current communication mode of the PNC. */
       
    if ( ComM_GetActivePncComMode( pncIdx, COMM_MASTER_PARTITION_IDX ) != COMM_PNC_NO_COMMUNICATION )
    {
      *ComMode = COMM_FULL_COMMUNICATION;
    }
  }

  return retVal;
}
#endif

#define COMM_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
#include "ComM_MemMap.h"
