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
/**        \file  Dcm_AuthMgr.c
 *         \unit  AuthMgr
 *        \brief  Contains the implementation of Authentication Manager unit.
 *
 *      \details  MICROSAR DCM based on AR 4.0.3
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/
#define DCM_AUTHMGR_SOURCE

#ifdef __PRQA__                                                                                                                                      /* COV_DCM_PRQA_UNREACHABLE XF */
# pragma PRQA_MACRO_MESSAGES_OFF "Dcm_UtiGetReqData" 2985 /* MD_Dcm_Redundant_2985 */
# pragma PRQA_MACRO_MESSAGES_OFF "Dcm_UtiGetResData" 2985 /* MD_Dcm_Redundant_2985 */
                                                                                                                                                     /* PRQA S 2991 EOF */ /* MD_Dcm_ConstExpr */
                                                                                                                                                     /* PRQA S 2992 EOF */ /* MD_Dcm_ConstExpr */
                                                                                                                                                     /* PRQA S 2995 EOF */ /* MD_Dcm_ConstExpr */
                                                                                                                                                     /* PRQA S 2996 EOF */ /* MD_Dcm_ConstExpr */
#endif
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_AuthMgr.h"
#if (DCM_AUTHMGR_SUPPORT_ENABLED == STD_ON)                                                                                                          /* COV_MSR_UT_OPTIONAL_UNIT */
# include "Dcm_Debug.h"
# include "Dcm_Tmr.h"
# include "Dcm_Diag.h"
# include "Dcm_Net.h"
# include "Dcm_NetPbCfg.h"
# if (DCM_AUTHMGR_PERSIST_STATE_ENABLED == STD_ON)
#  include "Dcm_Mode.h"
# endif
# if (DCM_AUTHMGR_WHITELIST_SERVICES_ENABLED == STD_ON)
#  include "Dcm_DidMgr.h"
# endif
# if (DCM_AUTHMGR_WHITELIST_RID_ENABLED == STD_ON)
#  include "Dcm_RidMgr.h"
# endif
# if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
#  include "Dcm_State.h"
# endif
# if (DCM_AUTHMGR_APCE_INTERNAL_ENABLED == STD_ON) || (DCM_SVC_29_04_SUPPORT_ENABLED == STD_ON)
#  include "KeyM.h"
# endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
# if (DCM_AUTHMGR_CERT_ROLE_SIZE > 4)
#  error "Authentication role size greater 4 is not supported."
# endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
/*! Returns TRUE if the authentication role pre-condition of a diagnostic entity is met */
# define Dcm_AuthMgrIsSupportedInRole(precondPckgRef, authInfoRef)   ((Dcm_AuthMgrActiveRole(authInfoRef) & (precondPckgRef).AuthRole) != 0u)        /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
/*! Authentication session timers */
# define Dcm_AuthMgrGetTimerIdx(authInfoRef)                         (authInfoRef)                                                                   /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_AuthMgrTimerGet(authInfoRef)                            (Dcm_AuthMgrSingletonContext.DelayTime[Dcm_AuthMgrGetTimerIdx((authInfoRef))])  /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define Dcm_AuthMgrTimerActive(authInfoRef)                         ((boolean)(Dcm_AuthMgrTimerGet((authInfoRef)) != 0u))                           /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define Dcm_AuthMgrTimerExpireMaskReset()                           (Dcm_AuthMgrSingletonContext.TimerExpiredMask = 0u)                             /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define Dcm_AuthMgrTimerExpireMaskGet()                             (Dcm_AuthMgrSingletonContext.TimerExpiredMask)                                  /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define Dcm_AuthMgrAnyTimerExpired()                                (Dcm_AuthMgrSingletonContext.TimerExpiredMask != 0u)
/*! Authentication role */
# define Dcm_AuthMgrActiveRole(authInfoRef)                          (Dcm_AuthMgrNvMData[(authInfoRef)].ConnContext.ActiveRole)                      /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
/*! Service white list operations */
# if (DCM_AUTHMGR_WHITELIST_SERVICES_ENABLED == STD_ON)
#  define Dcm_AuthMgrIsGlobalAccessGranted()                         (Dcm_AuthMgrSingletonContext.GrantGlobalAccess == TRUE)                         /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_AuthMgrSetGlobalAccess(value)                          (Dcm_AuthMgrSingletonContext.GrantGlobalAccess = (value))                       /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_AuthMgrNumSvcWLElements(authInfoRef)                   (Dcm_AuthMgrNvMData[(authInfoRef)].ConnContext.WhiteListContext.WhiteListServiceContext.NumServiceWhiteListElements) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_AuthMgrSvcWLElemLen(authInfoRef, iter)                 (Dcm_AuthMgrNvMData[(authInfoRef)].ConnContext.WhiteListContext.WhiteListServiceContext.WhiteListServiceElement[iter].WhiteListEntryLength) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_AuthMgrSvcWLElemDataAt(authInfoRef, iter, pos)         (Dcm_AuthMgrNvMData[(authInfoRef)].ConnContext.WhiteListContext.WhiteListServiceContext.WhiteListServiceElement[iter].WhiteListData[pos]) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_AuthMgrSvcWLElemIsSID(authInfoRef, iter, sid)          (Dcm_AuthMgrSvcWLElemDataAt((authInfoRef), (iter), 0u) == (sid))                /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_AuthMgrSvcWLElemIs22(authInfoRef, iter)                (Dcm_AuthMgrSvcWLElemIsSID((authInfoRef), (iter), DCM_DIDMGR_SVC_READ))         /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_AuthMgrSvcWLElemIs2E(authInfoRef, iter)                (Dcm_AuthMgrSvcWLElemIsSID((authInfoRef), (iter), DCM_DIDMGR_SVC_WRITE))        /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_AuthMgrSvcWLElemIs2F(authInfoRef, iter)                (Dcm_AuthMgrSvcWLElemIsSID((authInfoRef), (iter), DCM_DIDMGR_SVC_IOCTRL))       /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_AuthMgrSvcWLElemIsDIDWRIO(authInfoRef, iter)           ((Dcm_AuthMgrSvcWLElemLen((authInfoRef), (iter)) == 3u) && (Dcm_AuthMgrSvcWLElemIs22((authInfoRef), (iter)) || Dcm_AuthMgrSvcWLElemIs2E((authInfoRef), (iter)) || Dcm_AuthMgrSvcWLElemIs2F((authInfoRef), (iter)))) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_AuthMgrSvcWLElemIs31(authInfoRef, iter)                (Dcm_AuthMgrSvcWLElemIsSID((authInfoRef), (iter), 0x31u))                       /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_AuthMgrSvcWLElemIsRID(authInfoRef, iter)               ((Dcm_AuthMgrSvcWLElemLen((authInfoRef), (iter)) == 4u) && Dcm_AuthMgrSvcWLElemIs31((authInfoRef), (iter))) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_AuthMgrSvcWLElemIsDSP(authInfoRef, iter)               (Dcm_AuthMgrSvcWLElemIsDIDWRIO((authInfoRef), (iter)) || Dcm_AuthMgrSvcWLElemIsRID((authInfoRef), (iter))) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# endif
/*! DID white list operations */
# if (DCM_AUTHMGR_WHITELIST_DID_ENABLED == STD_ON)
#  define Dcm_AuthMgrNumDidWhiteListElements(authInfoRef)            (Dcm_AuthMgrNvMData[(authInfoRef)].ConnContext.WhiteListContext.WhiteListDidContext.NumDidWhiteListElements) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_AuthMgrDidWLElem(authInfoRef, iter)                    (Dcm_AuthMgrNvMData[(authInfoRef)].ConnContext.WhiteListContext.WhiteListDidContext.WhiteListDidElement[(iter)]) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# endif
/*! RID white list operations */
# if (DCM_AUTHMGR_WHITELIST_RID_ENABLED == STD_ON)
#  define Dcm_AuthMgrNumRidWhiteListElements(authInfoRef)            (Dcm_AuthMgrNvMData[(authInfoRef)].ConnContext.WhiteListContext.WhiteListRidContext.NumRidWhiteListElements) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_AuthMgrRidWLElem(authInfoRef, iter)                    (Dcm_AuthMgrNvMData[(authInfoRef)].ConnContext.WhiteListContext.WhiteListRidContext.WhiteListRidElement[(iter)]) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# endif
/*! MEM white list operations */
# if (DCM_AUTHMGR_WHITELIST_MEM_ENABLED == STD_ON)
#  define Dcm_AuthMgrNumMemWhiteListElements(authInfoRef)            (Dcm_AuthMgrNvMData[(authInfoRef)].ConnContext.WhiteListContext.WhiteListMemContext.NumMemWhiteListElements) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_AuthMgrMemWLElem(authInfoRef, iter)                    (Dcm_AuthMgrNvMData[(authInfoRef)].ConnContext.WhiteListContext.WhiteListMemContext.WhiteListMemElement[(iter)]) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# endif
/*! Redirects the reading of the authenticaion roles */
# if (DCM_AUTHMGR_USER_SPECIFIC_ROLES_ENABLED == STD_ON)
#  define Dcm_AuthMgrCertElementGet                                  Dcm_GetAuthenticationRoles                                                      /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# else
#  define Dcm_AuthMgrCertElementGet                                  KeyM_CertElementGet                                                             /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# endif

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
# if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
struct DCM_AUTHMGRPENDINGDATATYPE_TAG
{
#  if (DCM_AUTHMGR_EXT_SETTER_ENABLED == STD_ON) && (DCM_AUTHMGR_WHITELISTS_ENABLED == STD_ON)
  Dcm_ReadOnlyWhiteListContextPtrType WhiteListsPointer; /*!< Pointer to pending white lists from Dcm_AuthenticateConnection() */
#  endif
  Dcm_CfgStateGroupMemType            Role;              /*!< Pending role from Dcm_SetDeauthenticatedRole()/Dcm_(De)AuthenticateConnection() */
#  if (DCM_AUTHMGR_SET_DEAUTH_ROLE_ENABLED == STD_ON) || (DCM_AUTHMGR_EXT_SETTER_ENABLED == STD_ON)
  uint8                               ServiceId;         /*!< Pending service ID from Dcm_SetDeauthenticatedRole()/Dcm_(De)AuthenticateConnection() */
#  endif
};
typedef struct DCM_AUTHMGRPENDINGDATATYPE_TAG Dcm_AuthMgrPendingDataType;
# endif

# if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
/*! Pending data for processing of Dcm_SetDeauthenticatedRole()/Dcm_(De)AuthenticateConnection() */
struct DCM_AUTHMGRPENDINGINFOTYPE_TAG
{
  uint32                     ConnectionMask;                                          /*!< Bit mask for the connetionId for which Dcm_SetDeauthenticatedRole() was called */
  Dcm_AuthMgrPendingDataType PendingConnectionData[DCM_AUTHMGR_NUM_AUTH_CONNECTIONS]; /*!< Array with Pending Data for each connection */
};
typedef struct DCM_AUTHMGRPENDINGINFOTYPE_TAG Dcm_AuthMgrPendingInfoType;
# endif

# if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
/*! Authentication manager context */
struct DCM_AUTHMGRSINGLETONCONTEXTTYPE_TAG
{
  Dcm_AuthMgrPendingInfoType       PendingData;                              /*!< Pending data for processing of Dcm_SetDeauthenticatedRole() */
  Dcm_AuthMgrTimerExpiredMaskType  TimerExpiredMask;                         /*!< A flag to store expired timers for authentication sessions */
#  if (DCM_AUTHMGR_DEFAULT_SESSION_TIMEOUT_ENABLED == STD_ON)
  Dcm_AuthMgrTimerType             DelayTime[DCM_AUTHMGR_NUM_DELAY_TIMERS];  /*!< Array of delay timers for authentication sessions */

#  endif
#  if (DCM_AUTHMGR_WHITELIST_SERVICES_ENABLED == STD_ON)
  boolean                          GrantGlobalAccess;                        /*!< Global access flag */
#  endif
#  if (DCM_STATE_AUTHENTICATION_BYPASS_ENABLED == STD_ON)
  boolean                          BypassEnabled;                            /* A flag to indicate whether the bypass feature is on or off */
#  endif
};
typedef struct DCM_AUTHMGRSINGLETONCONTEXTTYPE_TAG Dcm_AuthMgrSingletonContextType;
# endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_AUTHMGR_DEFAULT_SESSION_TIMEOUT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrTimerSet()
 *********************************************************************************************************************/
/*! \brief          Sets the value of a specific idle connection session timer.
 *  \details        -
 *  \param[in]      authInfoRef  The authentication info reference
 *  \param[in]      value        Timer value
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_AuthMgrTimerSet(
  Dcm_CfgNetAuthInfoRefMemType authInfoRef,
  Dcm_AuthMgrTimerType value
  );
# endif

# if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrSetConnAuthState()
 *********************************************************************************************************************/
/*! \brief          Sets the authentication state of a connection of the given index.
 *  \details        -
 *  \param[in]      authInfoRef  Index to the ConnAuthStates array
 *  \param[in]      value        New authentication state of connection
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_AuthMgrSetConnAuthState(
  Dcm_CfgNetAuthInfoRefMemType authInfoRef,
  Dcm_AuthenticationStateType value
  );
# endif

# if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrSetPendingRole()
 *********************************************************************************************************************/
/*! \brief          Sets the pending role coming from external API Dcm_SetDeauthenticatedRole()/Dcm_(De)AuthenticateConnection().
 *  \details        -
 *  \param[in]      authInfoRef  The authentication information reference
 *  \param[in]      pendingRole  New role of connection which is pending
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_AuthMgrSetPendingRole(
  Dcm_CfgNetAuthInfoRefMemType authInfoRef,
  Dcm_CfgStateGroupMemType pendingRole
  );
# endif

# if (DCM_AUTHMGR_SET_DEAUTH_ROLE_ENABLED == STD_ON) || (DCM_AUTHMGR_EXT_SETTER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrSetPendingService()
 *********************************************************************************************************************/
/*! \brief          Sets the pending state coming from external API Dcm_SetDeauthenticatedRole()/Dcm_(De)AuthenticateConnection().
 *  \details        -
 *  \param[in]      authInfoRef  The authentication information reference
 *  \param[in]      serviceId    ID of the service function:
 *                               - DCM_SID_SETDEAUTHENTICATEDROLE,
 *                               - DCM_SID_DEAUTHENTICATECONNECTION or
 *                               - DCM_SID_AUTHENTICATECONNECTION
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_AuthMgrSetPendingService(
  Dcm_CfgNetAuthInfoRefMemType authInfoRef,
  uint8 serviceId
  );
# endif

# if (DCM_AUTHMGR_EXT_SETTER_ENABLED == STD_ON)
#  if (DCM_AUTHMGR_WHITELISTS_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrSetPendingWhiteListContextPtr()
 *********************************************************************************************************************/
/*! \brief          Sets the pending white list coming from external API Dcm_(De)AuthenticateConnection().
 *  \details        -
 *  \param[in]      authInfoRef              The authentication information reference
 *  \param[in]      pPendingWhiteListContext Pointer to new white lists of connection which are pending
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_AuthMgrSetPendingWhiteListContextPtr(
  Dcm_CfgNetAuthInfoRefMemType authInfoRef,
  CONSTP2CONST(Dcm_WhiteListContextType, AUTOMATIC, AUTOMATIC) pPendingWhiteListContext
  );
#  endif
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_AUTHMGR_WHITELISTS_ENABLED == STD_ON) && (DCM_AUTHMGR_APCE_INTERNAL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrGetCertWLElement()
 *********************************************************************************************************************/
/*! \brief          Reads a white list element of a certificate.
 *  \details        -
 *  \param[in]      certificateId              Certificate Id
 *  \param[in]      certElementId              Certificate element Id
 *  \param[out]     pElementData               Pointer to store the element data
 *  \param[in,out]  pElementDataLength         Element data length
 *  \param[in]      opStatus                   The operation status
 *  \param[in,out]  pRepContext                The repeater context
 *  \param[out]     ErrorCode                  The NRC
 *  \return         DCM_E_NOT_OK               Send negative response
 *  \return         DCM_E_OK                   No element left (end of list)
 *  \return         DCM_E_LOOP                 Element successfully read
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrGetCertWLElement(
  uint16 certificateId,
  uint16 certElementId,
  Dcm_MsgType pElementData,
  Dcm_Uint32VarDataPtrType pElementDataLength,
  Dcm_OpStatusType opStatus,
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
# endif

# if (DCM_AUTHMGR_WHITELIST_SERVICES_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrCheckServiceWLDispatcher()
 *********************************************************************************************************************/
/*! \brief          Authentication service white list check for DSD
 *  \details        Checks that the request matches first bytes of an element in service white list
 *  \param[in]      sid                        The service identifier
 *  \param[in,out]  pMsgContext                The message context
 *  \param[in]      authInfoRef                The authentication information reference
 *  \return         DCM_E_NOT_OK               Send negative response
 *  \return         DCM_E_OK                   Successful
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrCheckServiceWLDispatcher(
  uint8 sid,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_CfgNetAuthInfoRefMemType authInfoRef
  );
# endif

# if (DCM_AUTHMGR_DID_CHECK_ENABLED == STD_ON)
#  if (DCM_AUTHMGR_WHITELIST_DID_ENABLED == STD_ON) || \
      (DCM_AUTHMGR_WHITELIST_SERVICES_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrCheckDidWL()
 *********************************************************************************************************************/
/*! \brief          Authentication DID white list check
 *  \details        Checks if DID and op matches element in DID white list
 *  \param[in]      did                        The DID identifier
 *  \param[in]      didOpMask                  The operation (bit-wise)
 *  \param[in]      authInfoRef                The authentication information reference
 *  \return         DCM_E_NOT_OK               Send negative response
 *  \return         DCM_E_OK                   Successful
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrCheckDidWL(
  uint16 did,
  Dcm_DidOpType didOpMask,
  Dcm_CfgNetAuthInfoRefMemType authInfoRef
  );
#  endif
# endif

# if (DCM_AUTHMGR_WHITELIST_RID_ENABLED == STD_ON) || \
     (DCM_AUTHMGR_WHITELIST_SERVICES_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrCheckRidWL()
 *********************************************************************************************************************/
/*! \brief          Authentication RID white list check
 *  \details        Checks if RID and op matches element in RID white list
 *  \param[in]      rid                        The RID identifier
 *  \param[in]      ridSubfunc                 The routine control subfunction
 *  \param[in]      authInfoRef                The authentication information reference
 *  \return         DCM_E_NOT_OK               Send negative response
 *  \return         DCM_E_OK                   Successful
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrCheckRidWL(
  uint16 rid,
  uint8 ridSubfunc,
  Dcm_CfgNetAuthInfoRefMemType authInfoRef
  );
# endif

# if (DCM_AUTHMGR_WHITELIST_MEM_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrCheckMemWL()
 *********************************************************************************************************************/
/*! \brief          Authentication MEM white list check
 *  \details        Checks if MemorySelection matches element in MEM white list
 *  \param[in]      memId                      The MemorySelection
 *  \param[in]      authInfoRef                The authentication information reference
 *  \return         DCM_E_NOT_OK               Send negative response
 *  \return         DCM_E_OK                   Successful
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrCheckMemWL(
  uint8 memId,
  Dcm_CfgNetAuthInfoRefMemType authInfoRef
  );
# endif

# if (DCM_AUTHMGR_EXT_SETTER_ENABLED == STD_ON) && (DCM_AUTHMGR_WHITELISTS_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrIsWhiteListValid()
 *********************************************************************************************************************/
/*! \brief          Checks wether the given white list is valid.
 *  \details        -
 *  \param[in]      pWhiteListContext Pointer to new white lists of connection which are pending
 *  \return         TRUE              White list is valid
 *  \return         FALSE             White list is not valid
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(boolean, DCM_CODE) Dcm_AuthMgrIsWhiteListValid(
  Dcm_ReadOnlyWhiteListContextPtrType pWhiteListContext
  );
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL DATA DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_VAR_NO_INIT_UNSPECIFIED
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*! Authentication manager data that can be read from and written to NvM (if NvM is configured) */
# if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
VAR(Dcm_AuthMgrNvMDataType, DCM_VAR_NOINIT) Dcm_AuthMgrNvMData[DCM_AUTHMGR_NUM_AUTH_CONNECTIONS];
# endif

/*! Singleton context of AuthMgr unit */
# if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
DCM_LOCAL VAR(Dcm_AuthMgrSingletonContextType, DCM_VAR_NOINIT) Dcm_AuthMgrSingletonContext;
# endif
# define DCM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_AUTHMGR_DEFAULT_SESSION_TIMEOUT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrTimerSet()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_AuthMgrTimerSet(
  Dcm_CfgNetAuthInfoRefMemType authInfoRef,
  Dcm_AuthMgrTimerType value
  )
{
  uint8_least lTimerIdx = Dcm_AuthMgrGetTimerIdx(authInfoRef);

  if(Dcm_DebugDetectRuntimeError(lTimerIdx >= DCM_AUTHMGR_NUM_DELAY_TIMERS))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
  }
  else
  {
    Dcm_AuthMgrSingletonContext.DelayTime[lTimerIdx] = value;                                                                                        /* SBSW_DCM_POINTER_WRITE_DELAYTIME_AUTH */
  }
}
# endif

# if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrSetConnAuthState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_AuthMgrSetConnAuthState(
  Dcm_CfgNetAuthInfoRefMemType authInfoRef,
  Dcm_AuthenticationStateType value
  )
{
  if(Dcm_DebugDetectRuntimeError(authInfoRef >= DCM_AUTHMGR_NUM_AUTH_CONNECTIONS))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
  }
  else
  {
    Dcm_AuthMgrNvMData[authInfoRef].ConnContext.AuthState = value;                                                                                   /* SBSW_DCM_POINTER_WRITE_AUTHNVMDATA */
  }
}
# endif

# if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrSetPendingRole()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_AuthMgrSetPendingRole(
  Dcm_CfgNetAuthInfoRefMemType authInfoRef,
  Dcm_CfgStateGroupMemType pendingRole
  )
{
  if (Dcm_DebugDetectRuntimeError(authInfoRef >= DCM_AUTHMGR_NUM_AUTH_CONNECTIONS))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
  }
  else
  {
    Dcm_AuthMgrSingletonContext.PendingData.PendingConnectionData[authInfoRef].Role = pendingRole;                                                   /* SBSW_DCM_ARRAY_WRITE_PENDINGAUTHDATA */
  }
}
# endif

# if (DCM_AUTHMGR_SET_DEAUTH_ROLE_ENABLED == STD_ON) || (DCM_AUTHMGR_EXT_SETTER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrSetPendingService()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_AuthMgrSetPendingService(
  Dcm_CfgNetAuthInfoRefMemType authInfoRef,
  uint8 serviceId
  )
{
  if (Dcm_DebugDetectRuntimeError(authInfoRef >= DCM_AUTHMGR_NUM_AUTH_CONNECTIONS))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
  }
  else
  {
    Dcm_AuthMgrSingletonContext.PendingData.PendingConnectionData[authInfoRef].ServiceId = serviceId;                                                /* SBSW_DCM_ARRAY_WRITE_PENDINGAUTHDATA */
  }
}
# endif

# if (DCM_AUTHMGR_EXT_SETTER_ENABLED == STD_ON)
#  if (DCM_AUTHMGR_WHITELISTS_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrSetPendingWhiteListContextPtr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_AuthMgrSetPendingWhiteListContextPtr(
  Dcm_CfgNetAuthInfoRefMemType authInfoRef,
  CONSTP2CONST(Dcm_WhiteListContextType, AUTOMATIC, AUTOMATIC) pPendingWhiteListContext
  )
{
  if (Dcm_DebugDetectRuntimeError(authInfoRef >= DCM_AUTHMGR_NUM_AUTH_CONNECTIONS))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
  }
  else
  {
    Dcm_AuthMgrSingletonContext.PendingData.PendingConnectionData[authInfoRef].WhiteListsPointer = pPendingWhiteListContext;                         /* SBSW_DCM_ARRAY_WRITE_PENDINGAUTHDATA */
  }
}
#  endif
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_AUTHMGR_WHITELIST_SERVICES_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrCheckServiceWLDispatcher()
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
 */
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrCheckServiceWLDispatcher(                                                                        /* PRQA S 6080 */ /* MD_MSR_STMIF */
  uint8 sid,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_CfgNetAuthInfoRefMemType authInfoRef
  )
{
  Std_ReturnType lStdResult = DCM_E_NOT_OK;

  uint16_least lIter;

  /* Reset global access flag */
  Dcm_AuthMgrSetGlobalAccess(FALSE);

  /* If requesting connection is authenticated: */
  if(Dcm_AuthMgrAuthenticationState(authInfoRef) == DCM_AUTHENTICATED)
  {
    /* Loop through all service white list elements */
    for(lIter = 0u; lIter < Dcm_AuthMgrNumSvcWLElements(authInfoRef); lIter ++)
    {
      /* If service white list element should be processed in DSP: */
      if(!Dcm_AuthMgrSvcWLElemIsDSP(authInfoRef, lIter))
      {
        uint8_least lElemIter;
        boolean lFound = TRUE;
        /* If SID matches the first byte of element: */
        if(sid == Dcm_AuthMgrSvcWLElemDataAt(authInfoRef, lIter, 0u))
        {
          /* Loop through the rest of the element bytes */
          for(lElemIter = 0u; lElemIter < ((uint8_least)Dcm_AuthMgrSvcWLElemLen(authInfoRef, lIter) - 1u); lElemIter ++)
          {
            /* If the iterator does not exceed request data length: */
            if(lElemIter < pMsgContext->reqDataLen)
            {
              /* If the element byte doesn't match corresponding request byte: */
              if(pMsgContext->reqData[lElemIter] != Dcm_AuthMgrSvcWLElemDataAt(authInfoRef, lIter, (lElemIter + 1u)))
              {
                /* Set found boolean to false and skip element */
                lFound = FALSE;
                break;
              }
            }
            else
            {
              /* Element size needs to be less than or equal to the request length */
              lFound = FALSE;
            }
          }
        }
        else
        {
          /* SID doesn't match */
          lFound = FALSE;
        }

        /* If a matching element is found: */
        if(lFound == TRUE)
        {
          /* Grant global request access (no need to check subfunction or data) */
          Dcm_AuthMgrSetGlobalAccess(TRUE);
          lStdResult = DCM_E_OK;
          break;
        }
      }
    }
  }

  return lStdResult;
}
# endif

# if (DCM_AUTHMGR_DID_CHECK_ENABLED == STD_ON)
#  if (DCM_AUTHMGR_WHITELIST_DID_ENABLED == STD_ON) || \
      (DCM_AUTHMGR_WHITELIST_SERVICES_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrCheckDidWL()
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
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrCheckDidWL(                                                                                      /* PRQA S 6080 */ /* MD_MSR_STMIF */
  uint16 did,
  Dcm_DidOpType didOpMask,
  Dcm_CfgNetAuthInfoRefMemType authInfoRef
  )
{
  Std_ReturnType lStdResult = DCM_E_NOT_OK;
  uint16_least lIter;

#   if (DCM_AUTHMGR_WHITELIST_DID_ENABLED == STD_ON)
  /* Loop through entire DID WL */
  for(lIter = 0u; lIter < Dcm_AuthMgrNumDidWhiteListElements(authInfoRef); lIter++)
  {
    /* If we find the DID we're after */
    if(did == Dcm_AuthMgrDidWLElem(authInfoRef, lIter).DidNumber)
    {
      /* And the right DID op access */
      if(Dcm_UtiBitOpTest(uint8, Dcm_AuthMgrDidWLElem(authInfoRef, lIter).DidAccessMask, didOpMask))
      {
        lStdResult = DCM_E_OK;
        break;
      }
    }
  }
#   endif

#   if (DCM_AUTHMGR_WHITELIST_SERVICES_ENABLED == STD_ON)
#    if (DCM_AUTHMGR_WHITELIST_DID_ENABLED == STD_ON)
  if (lStdResult == DCM_E_NOT_OK)
#    endif
  {
    /* Check service white list for allowance */
    uint8 lSid = Dcm_DidMgrGetSidFromOp(didOpMask);
    if(lSid != DCM_DIDMGR_SVC_NONE)
    {
      /* Loop over all service WL elements */
      for(lIter = 0u; lIter < Dcm_AuthMgrNumSvcWLElements(authInfoRef); lIter ++)
      {
        if(Dcm_AuthMgrSvcWLElemIsDIDWRIO(authInfoRef, lIter))
        {
          /* Check the bytes of the white list element */
          if((lSid == Dcm_AuthMgrSvcWLElemDataAt(authInfoRef, lIter, 0u))
           && (Dcm_UtiGetHiByte(did) == Dcm_AuthMgrSvcWLElemDataAt(authInfoRef, lIter, 1u))
           && (Dcm_UtiGetLoByte(did) == Dcm_AuthMgrSvcWLElemDataAt(authInfoRef, lIter, 2u)))
          {
            /* We have found a matching element to the request! */
            lStdResult = DCM_E_OK;
            /* No need to check further */
            break;
          }
        }
      }
    }
  }
#   endif

  /* Return search result */
  return lStdResult;
}
#  endif
# endif

# if (DCM_AUTHMGR_WHITELIST_RID_ENABLED == STD_ON) || \
     (DCM_AUTHMGR_WHITELIST_SERVICES_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrCheckRidWL()
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
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrCheckRidWL(
  uint16 rid,
  uint8 ridSubfunc,
  Dcm_CfgNetAuthInfoRefMemType authInfoRef
  )
{
  Std_ReturnType lStdResult = DCM_E_NOT_OK;
  uint16_least lIter;

#  if (DCM_AUTHMGR_WHITELIST_RID_ENABLED == STD_ON)
  /* Loop through entire RID WL */
  for(lIter = 0u; lIter < Dcm_AuthMgrNumRidWhiteListElements(authInfoRef); lIter++)
  {
    /* If we find the RID we're after */
    if(rid == Dcm_AuthMgrRidWLElem(authInfoRef, lIter).RidNumber)
    {
      /* And the right RID op access */
      if(Dcm_UtiBitOpTest(uint8, Dcm_AuthMgrRidWLElem(authInfoRef, lIter).RidAccessMask, Dcm_RidMgrConvOpOfSubFunc(ridSubfunc)))
      {
        lStdResult = DCM_E_OK;
        break;
      }
    }
  }
#  endif

#  if (DCM_AUTHMGR_WHITELIST_SERVICES_ENABLED == STD_ON)
#   if (DCM_AUTHMGR_WHITELIST_RID_ENABLED == STD_ON)
  if (lStdResult == DCM_E_NOT_OK)
#   endif
  {
    /* Check service white list for allowance */
    for(lIter = 0u; lIter < Dcm_AuthMgrNumSvcWLElements(authInfoRef); lIter++)
    {
      if(Dcm_AuthMgrSvcWLElemIsRID(authInfoRef, lIter))
      {
        /* Check the bytes of the white list element */
        if((ridSubfunc == Dcm_AuthMgrSvcWLElemDataAt(authInfoRef, lIter, 1u))
         && (Dcm_UtiGetHiByte(rid) == Dcm_AuthMgrSvcWLElemDataAt(authInfoRef, lIter, 2u))
         && (Dcm_UtiGetLoByte(rid) == Dcm_AuthMgrSvcWLElemDataAt(authInfoRef, lIter, 3u)))
        {
          /* We have found a matching element to the request! */
          lStdResult = DCM_E_OK;
          /* No need to check further */
          break;
        }
      }
    }
  }
#  endif

  /* Return search result */
  return lStdResult;
}
# endif

# if (DCM_AUTHMGR_WHITELIST_MEM_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrCheckMemWL()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrCheckMemWL(
  uint8 memId,
  Dcm_CfgNetAuthInfoRefMemType authInfoRef
  )
{
  Std_ReturnType lStdResult = DCM_E_NOT_OK;

  uint16_least lIter;

  /* Loop through entire MEM WL */
  for(lIter = 0u; lIter < Dcm_AuthMgrNumMemWhiteListElements(authInfoRef); lIter++)
  {
    /* If we find the MEM we're after */
    if(memId == Dcm_AuthMgrMemWLElem(authInfoRef, lIter).MemNumber)
    {
      lStdResult = DCM_E_OK;
      break;
    }
  }

  /* Return search result */
  return lStdResult;
}
# endif

# if (DCM_AUTHMGR_WHITELISTS_ENABLED == STD_ON) && (DCM_AUTHMGR_APCE_INTERNAL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrGetCertWLElement()
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
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrGetCertWLElement(                                                                                /* PRQA S 6060 */ /* MD_MSR_STPAR */
  uint16 certificateId,
  uint16 certElementId,
  Dcm_MsgType pElementData,
  Dcm_Uint32VarDataPtrType pElementDataLength,
  Dcm_OpStatusType opStatus,
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;
  Std_ReturnType lKeyMResult;

    /* If first element to be read: */
  if(opStatus == DCM_INITIAL)
  {
    /* Retrieve first element with KeyM_CertElementGetFirst, which sets up KeyM iterator struct */
    lKeyMResult = KeyM_CertElementGetFirst(certificateId
                                          ,certElementId
                                          ,&pRepContext->CertElementIterator
                                          ,pElementData
                                          ,pElementDataLength);                                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
  else
  {
    /* Retrieve remaining elements with KeyM_CertElementGetNext */
    lKeyMResult = KeyM_CertElementGetNext(&pRepContext->CertElementIterator
                                         ,pElementData
                                         ,pElementDataLength);                                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  switch (lKeyMResult)
  {
  case E_OK: /* If KeyM_CertElementGet[First/Next] returns E_OK: */
    lStdResult = DCM_E_LOOP;
    break;
  case E_NOT_OK: /* Otherwise, if KeyM function returns E_NOT_OK: */
    /* End of elements list, return success */
    lStdResult = DCM_E_OK;
    break;
  default: /* Otherwise, send panic NRC */
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);
    break;
  }

  return lStdResult;
}
# endif

# if (DCM_AUTHMGR_WHITELISTS_ENABLED == STD_ON) && (DCM_AUTHMGR_EXT_SETTER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrIsWhiteListValid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_LOCAL FUNC(boolean, DCM_CODE) Dcm_AuthMgrIsWhiteListValid(                                                                                       /* PRQA S 6080 */ /* MD_MSR_STMIF */
  Dcm_ReadOnlyWhiteListContextPtrType pWhiteListContext
  )
{
  boolean isWhiteListValid = FALSE;

#  if (DCM_AUTHMGR_WHITELIST_SERVICES_ENABLED == STD_ON)
  if (pWhiteListContext->WhiteListServiceContext.NumServiceWhiteListElements <= DCM_AUTHMGR_NUM_SERVICE_WL_ELEMENTS)
#  endif
  {
#  if (DCM_AUTHMGR_WHITELIST_DID_ENABLED == STD_ON)
    if (pWhiteListContext->WhiteListDidContext.NumDidWhiteListElements <= DCM_AUTHMGR_NUM_DID_WL_ELEMENTS)
#  endif
    {
#  if (DCM_AUTHMGR_WHITELIST_RID_ENABLED == STD_ON)
      if (pWhiteListContext->WhiteListRidContext.NumRidWhiteListElements <= DCM_AUTHMGR_NUM_RID_WL_ELEMENTS)
#  endif
      {
#  if (DCM_AUTHMGR_WHITELIST_MEM_ENABLED == STD_ON)
        if (pWhiteListContext->WhiteListMemContext.NumMemWhiteListElements <= DCM_AUTHMGR_NUM_MEM_WL_ELEMENTS)
#  endif
        {
          isWhiteListValid = TRUE;

#  if (DCM_AUTHMGR_WHITELIST_SERVICES_ENABLED == STD_ON)
          for (uint16 lIter = 0; lIter < pWhiteListContext->WhiteListServiceContext.NumServiceWhiteListElements; ++lIter)
          {
            if (pWhiteListContext->WhiteListServiceContext.WhiteListServiceElement[lIter].WhiteListEntryLength > DCM_AUTHMGR_ELEMENT_SERVICE_SIZE)
            {
              isWhiteListValid = FALSE;
              break;
            }
          }
#  endif
        }
      }
    }
  }

  return isWhiteListValid;
}
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON) && \
     ((DCM_SVC_14_USER_MEM_ID_ENABLED == STD_ON) || (DCM_SVC_19_USER_MEM_ID_CHECK_ENABLED == STD_ON))
/**********************************************************************************************************************
 *  Dcm_AuthMgrCheckMem()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrCheckMem(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_CfgStateRefOptType stateRef,
  uint8 memId
  )
{
  Std_ReturnType lStdResult;

  Dcm_NetConnRefMemType lConnHdl = Dcm_DiagGetTranspObj(pContext->ThreadId)->ConnHdl;
  Dcm_CfgNetAuthInfoRefMemType lAuthInfoRef = Dcm_NetPbCfgGetConnAuthInfoRef(lConnHdl);

  /* Check authentication role */
  lStdResult = Dcm_AuthMgrCheckRole(stateRef, lAuthInfoRef);

#  if (DCM_AUTHMGR_WHITELIST_MEM_ENABLED == STD_ON)
  /* Check the MEM whitelist only in authenticated state */
  if( (lStdResult == DCM_E_NOT_OK)
     &&(Dcm_AuthMgrAuthenticationState(lAuthInfoRef) == DCM_AUTHENTICATED) )
  {
    lStdResult = Dcm_AuthMgrCheckMemWL(memId, lAuthInfoRef);
  }
#  else
  DCM_IGNORE_UNREF_PARAM(memId);
#  endif

  return lStdResult;
}
# endif

# if (DCM_TMR_NUM_AUTHENTICATION > 0u)
/**********************************************************************************************************************
 *  Dcm_OnTimeoutAuthentication()
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
 */
FUNC(Dcm_TmrTimerCntrMemType, DCM_CODE) Dcm_OnTimeoutAuthentication(
  Dcm_ThreadIdMemType threadId
  )
{
  Dcm_TmrTimerCntrMemType       lReloadTick = 0u;
  Dcm_CfgNetAuthInfoRefOptType  lTmrIter;

  DCM_IGNORE_UNREF_PARAM(threadId);

  /* Enter critical section (Reason: A timer can be modified from an ISR) */
  Dcm_UtiEnterCriticalSection();
  /*=================================*
    BEGIN CRITICAL SECTION
   *=================================*/
  /* For each software timer: */
  for(lTmrIter = 0; lTmrIter < DCM_AUTHMGR_NUM_DELAY_TIMERS; ++lTmrIter)
  {
    /* If the software timer is active: */
    if(Dcm_AuthMgrTimerActive(lTmrIter) == TRUE)                                                                                                     /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
    {
      /* Decrement the timer value */
      Dcm_AuthMgrTimerSet((Dcm_CfgNetAuthInfoRefMemType)lTmrIter, (Dcm_AuthMgrTimerType)(Dcm_AuthMgrTimerGet(lTmrIter) - 1u));

      /* If the software timer is still active */
      if(Dcm_AuthMgrTimerActive(lTmrIter) == TRUE)                                                                                                   /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
      {
        /* Reload the hardware timer */
        lReloadTick = 1u;
      }
      else
      {
        /* Otherwise, update the global expired timer mask flag with position of expired timer */
        Dcm_AuthMgrTimerExpireMaskSetBit((Dcm_CfgNetAuthInfoRefMemType)lTmrIter);
      }
    }
  }
  /*=================================*
    END CRITICAL SECTION
   *=================================*/
  /* Leave critical section */
  Dcm_UtiLeaveCriticalSection();

  /* If at least one timer has expired */
  if(Dcm_AuthMgrAnyTimerExpired())                                                                                                                   /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
  {
    /* Set the event to deauthenticate idle connections in default session */
    Dcm_TskSetEventByThread(DCM_TSK_ID_DIAG_WORK, DCM_TSK_EV_DIAG_WORK_IDLE_CONN, threadId);
  }

  return lReloadTick;
}
# endif

# if (DCM_AUTHMGR_DEFAULT_SESSION_TIMEOUT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrTimerStart()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_AuthMgrTimerStart(
  Dcm_CfgNetAuthInfoRefMemType authInfoRef,
  Dcm_AuthMgrTimerType value
  )
{
  /* Start the appropriate authentication timer */
  Dcm_AuthMgrTimerSet(authInfoRef, value);

  if(!Dcm_TmrIsTimerRunning(DCM_TMR_ID_AUTHENTICATION))
  {
    /* Start the base authentication timer */
    Dcm_TmrStartTimer(DCM_TMR_ID_AUTHENTICATION, 1u);
  }
}
# endif

# if (DCM_AUTHMGR_DEFAULT_SESSION_TIMEOUT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrTimerReload()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_AuthMgrTimerReload(
  Dcm_CfgNetAuthInfoRefMemType authInfoRef,
  Dcm_AuthMgrTimerType value
  )
{
  if(Dcm_AuthMgrTimerActive(authInfoRef))
  {
    Dcm_AuthMgrTimerSet(authInfoRef, value);
  }
}
# endif

# if (DCM_AUTHMGR_DEFAULT_SESSION_TIMEOUT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrTimerReset()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_AuthMgrTimerReset(
  Dcm_CfgNetAuthInfoRefMemType authInfoRef
  )
{
  Dcm_AuthMgrTimerSet(authInfoRef, 0u);
}
# endif

# if (DCM_AUTHMGR_DEFAULT_SESSION_TIMEOUT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrDetectIdleConnection()
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
FUNC(void, DCM_CODE) Dcm_AuthMgrDetectIdleConnection(
  Dcm_CfgStateGroupOptType oldSession
  )
{
  /* If this is not a transition from default to default session: */
  if (oldSession != DCM_STATE_VALUE_SESSION_INIT)
  {
    Dcm_CfgNetAuthInfoRefMemType lAuthInfoRef = Dcm_NetPbCfgGetConnAuthInfoRef(Dcm_NetGetSessionConnection());

    if (lAuthInfoRef != DCM_AUTHMGR_INVALID_AUTH_INFO_REF)
    {
      boolean lIsTimerActive;

      /* Enter critical section (Reason: A timer can be modified from an ISR or high priority task) */
      Dcm_UtiEnterCriticalSection();
      /*=================================*
      BEGIN CRITICAL SECTION
      *=================================*/
      lIsTimerActive = Dcm_AuthMgrTimerActive(lAuthInfoRef);                                                                                         /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
      /*=================================*
      END CRITICAL SECTION
      *=================================*/
      /* Leave critical section */
      Dcm_UtiLeaveCriticalSection();

      if (lIsTimerActive == FALSE)
      {
        if (Dcm_AuthMgrAuthenticationState(lAuthInfoRef) == DCM_AUTHENTICATED)
        {
          /* Deauthenticate the connection */
          Dcm_AuthMgrUpdateActiveRole(lAuthInfoRef, DCM_AUTHMGR_DEAUTHENTICATED_ROLE);
          Dcm_AuthMgrSetAuthentication(lAuthInfoRef, DCM_DEAUTHENTICATED);
        }
      }
    }
  }
}
# endif

# if (DCM_AUTHMGR_RID_CHECK_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrCheckRid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrCheckRid(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  uint16 rid,
  uint8 ridSubfunc,
  Dcm_CfgStateRefOptType stateRef
  )
{
  Std_ReturnType lStdResult;

  Dcm_NetConnRefMemType lConnHdl = Dcm_DiagGetTranspObj(pContext->ThreadId)->ConnHdl;
  Dcm_CfgNetAuthInfoRefMemType lAuthInfoRef = Dcm_NetPbCfgGetConnAuthInfoRef(lConnHdl);

  /* Check authentication role */
  lStdResult = Dcm_AuthMgrCheckRole(stateRef, lAuthInfoRef);

#  if (DCM_AUTHMGR_WHITELIST_RID_ENABLED == STD_ON) || \
      (DCM_AUTHMGR_WHITELIST_SERVICES_ENABLED == STD_ON)
  /* If Role check fails, check RID WL if in authenticated state */
  if((lStdResult == DCM_E_NOT_OK) && (Dcm_AuthMgrAuthenticationState(lAuthInfoRef) == DCM_AUTHENTICATED))
  {
    lStdResult = Dcm_AuthMgrCheckRidWL(rid, ridSubfunc, lAuthInfoRef);
  }
#  else
  DCM_IGNORE_UNREF_PARAM(rid);
  DCM_IGNORE_UNREF_PARAM(ridSubfunc);
#  endif

  return lStdResult;
}
# endif

# if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrCheckRole()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrCheckRole(
  Dcm_CfgStateRefOptType stateRef,
  Dcm_CfgNetAuthInfoRefMemType authInfoRef
  )
{
  Std_ReturnType lStdResult = DCM_E_OK;

#  if(DCM_STATE_AUTHENTICATION_BYPASS_ENABLED == STD_ON)

  if (Dcm_AuthMgrSingletonContext.BypassEnabled == FALSE)
#  endif
  {
    /* Check if authentication restriction is supported for this connection and the bypass preconditions check feature is not set */
    if(authInfoRef != DCM_AUTHMGR_INVALID_AUTH_INFO_REF)
    {
#  if (DCM_AUTHMGR_WHITELIST_SERVICES_ENABLED == STD_ON)
      /* Check if global access has been granted through service whitelist at DSD */
      if(Dcm_AuthMgrIsGlobalAccessGranted())
      {
        lStdResult = DCM_E_OK;
      }
      else
#  endif
      {
        if(!Dcm_AuthMgrIsSupportedInRole(Dcm_StateGetPreconditionStates(stateRef), authInfoRef))
        {
          lStdResult = DCM_E_NOT_OK;
        }
      }
    }
  }
  return lStdResult;
}
# endif

# if (DCM_AUTHMGR_APCE_INTERNAL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrGetCertRole()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrGetCertRole(
  Dcm_OpStatusType opStatus,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lKeyMResult;
  Std_ReturnType lStdResult = DCM_E_OK;

  uint32                   lRoleElementLength = DCM_AUTHMGR_CERT_ROLE_SIZE;
  uint8                    lRoleReadElement[4] = { 0 };
  Dcm_CfgStateGroupMemType lRoleInLittleEndian;

  DCM_IGNORE_UNREF_PARAM(opStatus);

  /* Extract the role from the certificate */
  lKeyMResult = Dcm_AuthMgrCertElementGet(Dcm_CfgWrapAuthMgrNetConnInfo(pRepContext->AuthInfoHdl)->ConnectionCertificateId
                                         ,Dcm_CfgWrapAuthMgrNetConnInfo(pRepContext->AuthInfoHdl)->CertRoleId
                                         ,lRoleReadElement
                                         ,&lRoleElementLength);                                                                                      /* SBSW_DCM_POINTER_WRITE_CERTIFICATE_ROLE */

  /* If role extraction from certificate was successful */
  if(lKeyMResult == E_OK)
  {
    /* If the size of the read role is equal to configured role size */
    if(lRoleElementLength == DCM_AUTHMGR_CERT_ROLE_SIZE)
    {
      /* Store role in role context. Cast to Dcm_CfgStateGroupMemType to truncate unwanted upper bytes */
      lRoleInLittleEndian = ((Dcm_CfgStateGroupMemType)(Dcm_UtiMake32Bit(lRoleReadElement[3]
                                                                        ,lRoleReadElement[2]
                                                                        ,lRoleReadElement[1]
                                                                        ,lRoleReadElement[0])));

      pRepContext->ConnContext.ActiveRole = lRoleInLittleEndian;                                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
    }
    else
    {
      if(Dcm_DebugDetectError(lRoleElementLength > DCM_AUTHMGR_CERT_ROLE_SIZE))
      {
        /* Send panic NRC */
        *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
        lStdResult = DCM_E_NOT_OK;
        Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_BUFFER_OVERFLOW);
      }
      else
      {
        /* Otherwise return NRC 55 */
        *ErrorCode = DCM_E_CVF_INVALIDCONTENT;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
        lStdResult = DCM_E_NOT_OK;
      }
    }
  }
  else
  {
    /* Role extraction wasn't successful, return Error */
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);
  }

  return lStdResult;
}
# endif

# if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrCheckDispatcherLevel()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrCheckDispatcherLevel(
  uint8 sid,
  Dcm_CfgStateRefOptType stateRef,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_CfgNetAuthInfoRefMemType authInfoRef,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;

#  if (DCM_AUTHMGR_WHITELIST_SERVICES_ENABLED == STD_ON)
  /* Reset global access flag */
  Dcm_AuthMgrSetGlobalAccess(FALSE);
#  endif

  /* Check authentication role */
  lStdResult = Dcm_AuthMgrCheckRole(stateRef, authInfoRef);

#  if (DCM_AUTHMGR_WHITELIST_SERVICES_ENABLED == STD_ON)
  if (lStdResult == DCM_E_NOT_OK)
  {
    /* If role check fails, try whitelist */
    lStdResult = Dcm_AuthMgrCheckServiceWLDispatcher(sid, pMsgContext, authInfoRef);                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
#  else
  DCM_IGNORE_UNREF_PARAM(sid);
  DCM_IGNORE_UNREF_PARAM(pMsgContext);
#  endif

  if(lStdResult == DCM_E_NOT_OK)
  {
    *ErrorCode = DCM_E_AUTHENTICATIONREQUIRED;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
  }

  return lStdResult;
}
# endif

# if (DCM_AUTHMGR_WHITELIST_SERVICES_ENABLED == STD_ON) && (DCM_AUTHMGR_APCE_INTERNAL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrServiceWLReadList()
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
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrServiceWLReadList(                                                                                         /* PRQA S 6080 */ /* MD_MSR_STMIF */
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType                     lStdResult = DCM_E_OK;
  Dcm_OpStatusType                   lOpStatus  = DCM_INITIAL;
  Dcm_WhiteListServiceContextPtrType pWhiteListServiceContext = &pRepContext->ConnContext.WhiteListContext.WhiteListServiceContext;
  uint8                              lServiceReadElement[DCM_AUTHMGR_ELEMENT_SERVICE_SIZE];

  /* Reset number of service white list entries */
  pWhiteListServiceContext->NumServiceWhiteListElements = 0u;                                                                                        /* SBSW_DCM_GLOBAL_PTR_WRITE */

  /* If service white list supported by tester */
  if(Dcm_CfgWrapAuthMgrNetConnInfo(pRepContext->AuthInfoHdl)->CertWLServicesId != DCM_AUTHMGR_INVALID_WL_KEYM_ELEMENT_ID)
  {
    /* Loop over all service white list elements until there are no more left */
    do
    {
      uint32 lElementLength = DCM_AUTHMGR_ELEMENT_SERVICE_SIZE;

      lStdResult = Dcm_AuthMgrGetCertWLElement(Dcm_CfgWrapAuthMgrNetConnInfo(pRepContext->AuthInfoHdl)->ConnectionCertificateId
                                              ,Dcm_CfgWrapAuthMgrNetConnInfo(pRepContext->AuthInfoHdl)->CertWLServicesId
                                              ,&lServiceReadElement[0]
                                              ,&lElementLength
                                              ,lOpStatus
                                              ,pRepContext
                                              ,ErrorCode);                                                                                           /* SBSW_DCM_POINTER_WRITE_WL_ELEMENT */ /* SBSW_DCM_COMB_PTR_FORWARD */

      /* If element successfully read: */
      if(lStdResult == DCM_E_LOOP)
      {
        /* If retrieved element length fits within allocated buffer: */
        if((lElementLength > 0u) && (lElementLength <= DCM_AUTHMGR_ELEMENT_SERVICE_SIZE))
        {
          if(pWhiteListServiceContext->NumServiceWhiteListElements < DCM_AUTHMGR_NUM_SERVICE_WL_ELEMENTS)
          {
            /* Store length and data of service white list element */
            Dcm_UtiMemCopy(&lServiceReadElement[0], &(pWhiteListServiceContext->WhiteListServiceElement[pWhiteListServiceContext->NumServiceWhiteListElements].WhiteListData[0]), lElementLength); /* SBSW_DCM_POINTER_WRITE_SERVICE_WL_ELEMENT */
            pWhiteListServiceContext->WhiteListServiceElement[pWhiteListServiceContext->NumServiceWhiteListElements].WhiteListEntryLength = (uint8)lElementLength; /* SBSW_DCM_POINTER_WRITE_SERVICE_WL_ELEMENT */

            /* Increment number of service white list entries for next iteration */
            pWhiteListServiceContext->NumServiceWhiteListElements++;                                                                                 /* SBSW_DCM_GLOBAL_PTR_WRITE */
            lOpStatus = DCM_PENDING;
          }
          else
          {
            /* Buffer too small for the number of entries: Send NRC 0x55 (DCM_E_CVF_INVALIDCONTENT) */
            *ErrorCode = DCM_E_CVF_INVALIDCONTENT;                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
            lStdResult = DCM_E_NOT_OK;
          }
        }
        else
        {
          /* Otherwise, if read element is bigger than expected: */
          if(Dcm_DebugDetectError(lElementLength > DCM_AUTHMGR_ELEMENT_SERVICE_SIZE))
          {
            /* Send panic NRC */
            *ErrorCode = DCM_E_PANIC_NRC;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
            lStdResult = DCM_E_NOT_OK;
            Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_BUFFER_OVERFLOW);
          }
          else
          {
            /* Otherwise, send NRC 0x55 (DCM_E_CVF_INVALIDCONTENT) */
            *ErrorCode = DCM_E_CVF_INVALIDCONTENT;                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
            lStdResult = DCM_E_NOT_OK;
          }
        }
      } /* Otherwise, all elements read or send prepared NRC */
    } while(lStdResult == DCM_E_LOOP);
  }

  return lStdResult;
}
# endif

# if (DCM_AUTHMGR_WHITELIST_DID_ENABLED == STD_ON) && (DCM_AUTHMGR_APCE_INTERNAL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrDidWLReadList()
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
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrDidWLReadList(                                                                                             /* PRQA S 6080 */ /* MD_MSR_STMIF */
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType                 lStdResult = DCM_E_OK;
  Dcm_OpStatusType               lOpStatus  = DCM_INITIAL;
  Dcm_WhiteListDidContextPtrType pWhiteListDidContext = &pRepContext->ConnContext.WhiteListContext.WhiteListDidContext;
  uint32                         lElementLength = DCM_AUTHMGR_ELEMENT_DID_SIZE;
  uint8                          lDidReadElement[DCM_AUTHMGR_ELEMENT_DID_SIZE];

  /* Reset number of DID white list entries */
  pWhiteListDidContext->NumDidWhiteListElements = 0u;                                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */

  /* If DID white list supported by tester */
  if(Dcm_CfgWrapAuthMgrNetConnInfo(pRepContext->AuthInfoHdl)->CertWLDidId != DCM_AUTHMGR_INVALID_WL_KEYM_ELEMENT_ID)
  {
    /* Loop over all service white list elements until there are no more left */
    do
    {
      lStdResult = Dcm_AuthMgrGetCertWLElement(Dcm_CfgWrapAuthMgrNetConnInfo(pRepContext->AuthInfoHdl)->ConnectionCertificateId
                                              ,Dcm_CfgWrapAuthMgrNetConnInfo(pRepContext->AuthInfoHdl)->CertWLDidId
                                              ,&lDidReadElement[0u]
                                              ,&lElementLength
                                              ,lOpStatus
                                              ,pRepContext
                                              ,ErrorCode);                                                                                           /* SBSW_DCM_POINTER_WRITE_WL_ELEMENT */ /* SBSW_DCM_COMB_PTR_FORWARD */

      /* If element successfully read: */
      if(lStdResult == DCM_E_LOOP)
      {
        /* If retrieved element length is as expected: */
        if(lElementLength == DCM_AUTHMGR_ELEMENT_DID_SIZE)
        {
          if(pWhiteListDidContext->NumDidWhiteListElements < DCM_AUTHMGR_NUM_DID_WL_ELEMENTS)
          {
            /* Store DID element data into context (big endian format) */
            pWhiteListDidContext->WhiteListDidElement[pWhiteListDidContext->NumDidWhiteListElements].DidNumber = (Dcm_UtiMake16Bit(lDidReadElement[0u], lDidReadElement[1u])); /* SBSW_DCM_POINTER_WRITE_DID_WL_ELEMENT */
            pWhiteListDidContext->WhiteListDidElement[pWhiteListDidContext->NumDidWhiteListElements].DidAccessMask = lDidReadElement[2u];            /* SBSW_DCM_POINTER_WRITE_DID_WL_ELEMENT */

            /* Increment number of DID white list entries for next iteration */
            pWhiteListDidContext->NumDidWhiteListElements++;                                                                                         /* SBSW_DCM_GLOBAL_PTR_WRITE */
            lOpStatus = DCM_PENDING;
          }
          else
          {
            /* Buffer too small for the number of entries: Send NRC 0x55 (DCM_E_CVF_INVALIDCONTENT) */
            *ErrorCode = DCM_E_CVF_INVALIDCONTENT;                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
            lStdResult = DCM_E_NOT_OK;
          }
        }
        else
        {
          /* Otherwise, if read element is bigger than expected: */
          if(Dcm_DebugDetectError(lElementLength > DCM_AUTHMGR_ELEMENT_DID_SIZE))
          {
            /* Send panic NRC */
            *ErrorCode = DCM_E_PANIC_NRC;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
            lStdResult = DCM_E_NOT_OK;
            Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_BUFFER_OVERFLOW);
          }
          else
          {
            /* Otherwise, send NRC 0x55 (DCM_E_CVF_INVALIDCONTENT) */
            *ErrorCode = DCM_E_CVF_INVALIDCONTENT;                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
            lStdResult = DCM_E_NOT_OK;
          }
        }
      }/* Otherwise, all elements read or send prepared NRC */
    } while(lStdResult == DCM_E_LOOP);
  }

  return lStdResult;
}
# endif

# if (DCM_AUTHMGR_WHITELIST_MEM_ENABLED == STD_ON) && (DCM_AUTHMGR_APCE_INTERNAL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrMemWLReadList()
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
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrMemWLReadList(                                                                                             /* PRQA S 6080 */ /* MD_MSR_STMIF */
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType                 lStdResult = DCM_E_OK;
  Dcm_OpStatusType               lOpStatus  = DCM_INITIAL;
  Dcm_WhiteListMemContextPtrType pWhiteListMemContext = &pRepContext->ConnContext.WhiteListContext.WhiteListMemContext;
  uint32                         lElementLength = DCM_AUTHMGR_ELEMENT_MEM_SIZE;
  uint8                          lMemReadElement[DCM_AUTHMGR_ELEMENT_MEM_SIZE];

  /* Reset number of MEM white list entries */
  pWhiteListMemContext->NumMemWhiteListElements = 0u;                                                                                                /* SBSW_DCM_GLOBAL_PTR_WRITE */

  /* If MEM white list supported by tester */
  if (Dcm_CfgWrapAuthMgrNetConnInfo(pRepContext->AuthInfoHdl)->CertWLMemId != DCM_AUTHMGR_INVALID_WL_KEYM_ELEMENT_ID)
  {
    /* Loop over all service white list elements until there are no more left */
    do
    {
      lStdResult = Dcm_AuthMgrGetCertWLElement(Dcm_CfgWrapAuthMgrNetConnInfo(pRepContext->AuthInfoHdl)->ConnectionCertificateId
                                              ,Dcm_CfgWrapAuthMgrNetConnInfo(pRepContext->AuthInfoHdl)->CertWLMemId
                                              ,&lMemReadElement[0u]
                                              ,&lElementLength
                                              ,lOpStatus
                                              ,pRepContext
                                              ,ErrorCode);                                                                                           /* SBSW_DCM_POINTER_WRITE_WL_ELEMENT */ /* SBSW_DCM_COMB_PTR_FORWARD */

      /* If element successfully read: */
      if(lStdResult == DCM_E_LOOP)
      {
        /* If retrieved element length is as expected: */
        if(lElementLength == DCM_AUTHMGR_ELEMENT_MEM_SIZE)
        {
          if(pWhiteListMemContext->NumMemWhiteListElements < DCM_AUTHMGR_NUM_MEM_WL_ELEMENTS)
          {
            /* Store MEM element data into context */
            pWhiteListMemContext->WhiteListMemElement[pWhiteListMemContext->NumMemWhiteListElements].MemNumber = lMemReadElement[0u];                /* SBSW_DCM_POINTER_WRITE_MEM_WL_ELEMENT */

            /* Increment number of MEM white list entries for next iteration */
            pWhiteListMemContext->NumMemWhiteListElements++;                                                                                         /* SBSW_DCM_GLOBAL_PTR_WRITE */
            lOpStatus = DCM_PENDING;
          }
          else
          {
            /* Buffer too small for the number of entries: Send NRC 0x55 (DCM_E_CVF_INVALIDCONTENT) */
            *ErrorCode = DCM_E_CVF_INVALIDCONTENT;                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
            lStdResult = DCM_E_NOT_OK;
          }
        }
        else
        {
          /* Otherwise, if read element is bigger than expected: */
          if(Dcm_DebugDetectError(lElementLength > DCM_AUTHMGR_ELEMENT_MEM_SIZE))
          {
            /* Send panic NRC */
            *ErrorCode = DCM_E_PANIC_NRC;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
            lStdResult = DCM_E_NOT_OK;
            Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_BUFFER_OVERFLOW);
          }
          else
          {
            /* Otherwise, send NRC 0x55 (DCM_E_CVF_INVALIDCONTENT) */
            *ErrorCode = DCM_E_CVF_INVALIDCONTENT;                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
            lStdResult = DCM_E_NOT_OK;
          }
        }
      }/* Otherwise, all elements read or send prepared NRC */
    } while(lStdResult == DCM_E_LOOP);
  }

  return lStdResult;
}
# endif

# if(DCM_AUTHMGR_WHITELISTS_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrGetWLContext()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrGetWLContext(
  Dcm_CfgNetAuthInfoRefMemType authInfoRef,
  CONSTP2VAR(Dcm_WhiteListContextPtrType, AUTOMATIC, AUTOMATIC) pWhiteListContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lResult;

  if (Dcm_DebugDetectRuntimeError(authInfoRef >= DCM_AUTHMGR_NUM_AUTH_CONNECTIONS))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);

    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    lResult = DCM_E_NOT_OK;
  }
  else
  {
    *pWhiteListContext = &Dcm_AuthMgrNvMData[authInfoRef].ConnContext.WhiteListContext;                                                              /* SBSW_DCM_PARAM_PTR_WRITE */
    lResult = DCM_E_OK;
  }

  return lResult;
}
# endif

# if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrUpdateActiveRole()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_AuthMgrUpdateActiveRole(
  Dcm_CfgNetAuthInfoRefMemType authInfoRef,
  Dcm_CfgStateGroupMemType authRole
  )
{

  if (Dcm_DebugDetectRuntimeError(authInfoRef >= DCM_AUTHMGR_NUM_AUTH_CONNECTIONS))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
  }
  else
  {
    Dcm_AuthMgrNvMData[authInfoRef].ConnContext.ActiveRole = authRole;                                                                               /* SBSW_DCM_POINTER_WRITE_AUTHNVMDATA */
  }
}
# endif

# if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrSetAuthentication()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_AuthMgrSetAuthentication(
  Dcm_CfgNetAuthInfoRefMemType authInfoRef,
  Dcm_AuthenticationStateType authState
  )
{
  /* Set Authentication flag for connection to new state */
  Dcm_AuthMgrSetConnAuthState(authInfoRef, authState);

#  if (DCM_AUTHMGR_APCE_INTERNAL_ENABLED == STD_ON)
  /* Rte mode switch to new state */
  Dcm_AuthMgrModeSwitchAuthentication(authInfoRef, authState);
#  endif
}
# endif

/**********************************************************************************************************************
 *  Dcm_AuthMgrInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_AuthMgrInit(
  void
  )
{
# if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
#  if (DCM_AUTHMGR_PERSIST_STATE_ENABLED == STD_ON)
  Dcm_NegativeResponseCodeType lErrorCode;
#  endif

#  if (DCM_AUTHMGR_PERSIST_STATE_ENABLED == STD_ON)
  if (Dcm_ModeCheck(DCM_MODE_AUTH_PERSIST_STATE_MODERULE_REF, &lErrorCode) != DCM_E_OK)                                                              /* SBSW_DCM_POINTER_FORWARD_STACK */
#  endif
  {
    Dcm_CfgNetAuthInfoRefOptType lAuthInfoRef;

    for (lAuthInfoRef = 0u; lAuthInfoRef < DCM_AUTHMGR_NUM_AUTH_CONNECTIONS; lAuthInfoRef++)
    {
      Dcm_AuthMgrSetConnAuthState((Dcm_CfgNetAuthInfoRefMemType)lAuthInfoRef, DCM_DEAUTHENTICATED);
      Dcm_AuthMgrUpdateActiveRole((Dcm_CfgNetAuthInfoRefMemType)lAuthInfoRef, DCM_AUTHMGR_DEAUTHENTICATED_ROLE);
    }
  }

  Dcm_AuthMgrSingletonContext.PendingData.ConnectionMask = 0u;
  Dcm_AuthMgrTimerExpireMaskReset();
# endif
# if (DCM_STATE_AUTHENTICATION_BYPASS_ENABLED == STD_ON)
  Dcm_AuthMgrSingletonContext.BypassEnabled = FALSE;
# endif
# if (DCM_AUTHMGR_WHITELIST_SERVICES_ENABLED == STD_ON)
  Dcm_AuthMgrSetGlobalAccess(FALSE);
# endif
}

# if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON) && (DCM_AUTHMGR_APCE_INTERNAL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrDeauthenticateIdleConn()
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
FUNC(void, DCM_CODE) Dcm_AuthMgrDeauthenticateIdleConn(                                                                                              /* PRQA S 6050 */ /* MD_MSR_STCAL */
  void
  )
{
  Dcm_CfgNetAuthInfoRefMemType    lTmrIter = 0u;
  Dcm_AuthMgrTimerExpiredMaskType lExpiredTimers;

  /* Enter critical section (Reason: A timer can be modified from an ISR) */
  Dcm_UtiEnterCriticalSection();
  /*=================================*
  BEGIN CRITICAL SECTION
  *=================================*/
  /* Retrieve all expired authentication timers atomically */
  lExpiredTimers = Dcm_AuthMgrTimerExpireMaskGet();

  /* Reset global expired timers flag */
  Dcm_AuthMgrTimerExpireMaskReset();
  /*=================================*
  END CRITICAL SECTION
  *=================================*/
  /* Leave critical section */
  Dcm_UtiLeaveCriticalSection();

  DCM_UTI_LOOP_BIT_SCAN(lExpiredTimers)
  {
    if (Dcm_UtiBitOpTest(Dcm_AuthMgrTimerExpiredMaskType, lExpiredTimers, 0x01u))
    {
      /* If the connection is authenticated */
      if (Dcm_AuthMgrAuthenticationState(lTmrIter) == DCM_AUTHENTICATED)
      {
        if ( (Dcm_StateIsDefaultSessionActive())
           ||(lTmrIter != Dcm_NetPbCfgGetConnAuthInfoRef(Dcm_NetGetSessionConnection())) )                                                           /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
        {
          Dcm_AuthMgrUpdateActiveRole(lTmrIter, DCM_AUTHMGR_DEAUTHENTICATED_ROLE);
          Dcm_AuthMgrSetAuthentication(lTmrIter, DCM_DEAUTHENTICATED);
        }
      }
    }

    ++lTmrIter;
  }
}
# endif

# if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrSetExtAuthentication()
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
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_AuthMgrSetExtAuthentication(                                                                                                /* PRQA S 6080 */ /* MD_MSR_STMIF */
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext
  )
{
  Dcm_NetConnRefMemType lConnHdl = 0u;
  uint32                lClearMask = 0u;
  uint32                lPendingMask;

  DCM_IGNORE_UNREF_PARAM(pContext);

  lPendingMask = Dcm_AuthMgrSingletonContext.PendingData.ConnectionMask;

  DCM_UTI_LOOP_BIT_SCAN(lPendingMask)
  {
    if (Dcm_UtiBitOpTest(uint32, lPendingMask, 0x01u))
    {
      Dcm_ThreadIdOptType lThreadId = Dcm_NetPbCfgGetProtocolThreadId(Dcm_NetPbCfgGetProtIdOfConnection(lConnHdl));

      if (!Dcm_DiagIsThreadBusy(lThreadId))
      {
        Dcm_CfgNetAuthInfoRefMemType lAuthInfoRef = Dcm_NetPbCfgGetConnAuthInfoRef(lConnHdl);

#  if (DCM_AUTHMGR_SET_DEAUTH_ROLE_ENABLED == STD_ON)
        if (Dcm_AuthMgrSingletonContext.PendingData.PendingConnectionData[lAuthInfoRef].ServiceId == DCM_SID_SETDEAUTHENTICATEDROLE)
        {
          if (Dcm_AuthMgrAuthenticationState(lAuthInfoRef) == DCM_DEAUTHENTICATED)
          {
            Dcm_AuthMgrUpdateActiveRole(lAuthInfoRef, Dcm_AuthMgrSingletonContext.PendingData.PendingConnectionData[lAuthInfoRef].Role);
          }
        }
        else
#  endif
        {
          Dcm_AuthMgrUpdateActiveRole(lAuthInfoRef, Dcm_AuthMgrSingletonContext.PendingData.PendingConnectionData[lAuthInfoRef].Role);

#  if (DCM_AUTHMGR_EXT_SETTER_ENABLED == STD_ON)
          if (Dcm_AuthMgrSingletonContext.PendingData.PendingConnectionData[lAuthInfoRef].ServiceId == DCM_SID_AUTHENTICATECONNECTION)
          {
#   if(DCM_AUTHMGR_WHITELISTS_ENABLED == STD_ON)
            Dcm_WhiteListContextPtrType pConnWhiteListContext;
            uint8                       lErrorCode;

            if (Dcm_AuthMgrGetWLContext(lAuthInfoRef, &pConnWhiteListContext, &lErrorCode) == DCM_E_OK)                                              /* SBSW_DCM_COMB_PTR_FORWARD */
            {
              *pConnWhiteListContext = *Dcm_AuthMgrSingletonContext.PendingData.PendingConnectionData[lAuthInfoRef].WhiteListsPointer;               /* SBSW_DCM_POINTER_WRITE_AUTHNVMDATA */
            }
#   endif

            Dcm_AuthMgrSetAuthentication(lAuthInfoRef, DCM_AUTHENTICATED);
            Dcm_ConnectionAuthenticated(Dcm_NetPbCfgGetConnId(lConnHdl));
          }
          else
#  endif
          {
            Dcm_AuthMgrSetAuthentication(lAuthInfoRef, DCM_DEAUTHENTICATED);
          }
        }
        lClearMask |= Dcm_UtiGetBitFromIndex(uint32, lConnHdl);
      }
    }
    ++lConnHdl;
  }

  /* Enter critical section (Reason: Protect against interrupts from Dcm_SetDeauthenticatedRole() API) */
  Dcm_UtiEnterCriticalSection();
  /*=================================*
    BEGIN CRITICAL SECTION
   *=================================*/
  Dcm_UtiBitOpClr(uint32, Dcm_AuthMgrSingletonContext.PendingData.ConnectionMask, lClearMask);
  /*=================================*
    END CRITICAL SECTION
   *=================================*/
  /* Leave critical section */
  Dcm_UtiLeaveCriticalSection();

  if (Dcm_AuthMgrSingletonContext.PendingData.ConnectionMask != 0u)
  {
    Dcm_TskSetLocalEvent(pEventContext->PostEv, DCM_TSK_EV_DIAG_WORK_EXT_AUTHENTICATION);                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
  }
}
# endif

# if (DCM_AUTHMGR_DID_CHECK_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrCheckDid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrCheckDid(
  Dcm_NetConnRefMemType connHdl,
  uint16 did,
  Dcm_DidOpType didOpMask,
  Dcm_CfgStateRefOptType stateRef
  )
{
  Std_ReturnType lStdResult = DCM_E_OK;

  if(connHdl != DCM_NET_INVALID_CONNHDL)
  {
    Dcm_CfgNetAuthInfoRefMemType lAuthInfoRef = Dcm_NetPbCfgGetConnAuthInfoRef(connHdl);

    /* Check authentication role */
    lStdResult = Dcm_AuthMgrCheckRole(stateRef, lAuthInfoRef);

#  if (DCM_AUTHMGR_WHITELIST_DID_ENABLED == STD_ON) || \
      (DCM_AUTHMGR_WHITELIST_SERVICES_ENABLED == STD_ON)
    /* If Role check fails, check DID WL if in authenticated state */
    if((lStdResult == DCM_E_NOT_OK) && (Dcm_AuthMgrAuthenticationState(lAuthInfoRef) == DCM_AUTHENTICATED))
    {
      lStdResult = Dcm_AuthMgrCheckDidWL(did, didOpMask, lAuthInfoRef);
    }
#  else
    DCM_IGNORE_UNREF_PARAM(did);
    DCM_IGNORE_UNREF_PARAM(didOpMask);
#  endif
  }

  return lStdResult;
}
# endif

# if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrAuthenticationState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_AuthenticationStateType, DCM_CODE) Dcm_AuthMgrAuthenticationState(
  Dcm_CfgNetAuthInfoRefMemType authInfoRef
  )
{
  return Dcm_AuthMgrNvMData[authInfoRef].ConnContext.AuthState;
}
# endif

# if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrTimerExpireMaskSetBit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_AuthMgrTimerExpireMaskSetBit(
  Dcm_CfgNetAuthInfoRefMemType authInfoRef
  )
{
  Dcm_UtiBitOpSet(Dcm_AuthMgrTimerExpiredMaskType, Dcm_AuthMgrSingletonContext.TimerExpiredMask, Dcm_UtiGetBitFromIndex(uint8_least, Dcm_AuthMgrGetTimerIdx(authInfoRef)));
}
# endif

# if (DCM_AUTHMGR_WHITELIST_RID_ENABLED == STD_ON) && (DCM_AUTHMGR_APCE_INTERNAL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrRidWLReadList()
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
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrRidWLReadList(                                                                                             /* PRQA S 6080 */ /* MD_MSR_STMIF */
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType                 lStdResult = DCM_E_OK;
  Dcm_OpStatusType               lOpStatus  = DCM_INITIAL;
  Dcm_WhiteListRidContextPtrType pWhiteListRidContext = &pRepContext->ConnContext.WhiteListContext.WhiteListRidContext;
  uint32                         lElementLength = DCM_AUTHMGR_ELEMENT_RID_SIZE;
  uint8                          lRidReadElement[DCM_AUTHMGR_ELEMENT_RID_SIZE];

  /* Reset number of RID white list entries */
  pWhiteListRidContext->NumRidWhiteListElements = 0u;                                                                                                /* SBSW_DCM_GLOBAL_PTR_WRITE */

  /* If RID white list supported by tester */
  if(Dcm_CfgWrapAuthMgrNetConnInfo(pRepContext->AuthInfoHdl)->CertWLRidId != DCM_AUTHMGR_INVALID_WL_KEYM_ELEMENT_ID)
  {
    /* Loop over all service white list elements until there are no more left */
    do
    {
      lStdResult = Dcm_AuthMgrGetCertWLElement(Dcm_CfgWrapAuthMgrNetConnInfo(pRepContext->AuthInfoHdl)->ConnectionCertificateId
                                              ,Dcm_CfgWrapAuthMgrNetConnInfo(pRepContext->AuthInfoHdl)->CertWLRidId
                                              ,&lRidReadElement[0u]
                                              ,&lElementLength
                                              ,lOpStatus
                                              ,pRepContext
                                              ,ErrorCode);                                                                                           /* SBSW_DCM_POINTER_WRITE_WL_ELEMENT */ /* SBSW_DCM_COMB_PTR_FORWARD */

      /* If element successfully read: */
      if(lStdResult == DCM_E_LOOP)
      {
        /* If retrieved element length is as expected: */
        if(lElementLength == DCM_AUTHMGR_ELEMENT_RID_SIZE)
        {
          if(pWhiteListRidContext->NumRidWhiteListElements < DCM_AUTHMGR_NUM_RID_WL_ELEMENTS)
          {
            /* Store RID element data into context (big endian format) */
            pWhiteListRidContext->WhiteListRidElement[pWhiteListRidContext->NumRidWhiteListElements].RidNumber = (Dcm_UtiMake16Bit(lRidReadElement[0u], lRidReadElement[1u])); /* SBSW_DCM_POINTER_WRITE_RID_WL_ELEMENT */
            pWhiteListRidContext->WhiteListRidElement[pWhiteListRidContext->NumRidWhiteListElements].RidAccessMask = lRidReadElement[2u];            /* SBSW_DCM_POINTER_WRITE_RID_WL_ELEMENT */

            /* Increment number of RID white list entries for next iteration */
            pWhiteListRidContext->NumRidWhiteListElements++;                                                                                         /* SBSW_DCM_GLOBAL_PTR_WRITE */
            lOpStatus = DCM_PENDING;
          }
          else
          {
            /* Buffer too small for the number of entries: Send NRC 0x55 (DCM_E_CVF_INVALIDCONTENT) */
            *ErrorCode = DCM_E_CVF_INVALIDCONTENT;                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
            lStdResult = DCM_E_NOT_OK;
          }
        }
        else
        {
          /* Otherwise, if read element is bigger than expected: */
          if(Dcm_DebugDetectError(lElementLength > DCM_AUTHMGR_ELEMENT_RID_SIZE))
          {
            /* Send panic NRC */
            *ErrorCode = DCM_E_PANIC_NRC;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
            lStdResult = DCM_E_NOT_OK;
            Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_BUFFER_OVERFLOW);
          }
          else
          {
            /* Otherwise, send NRC 0x55 (DCM_E_CVF_INVALIDCONTENT) */
            *ErrorCode = DCM_E_CVF_INVALIDCONTENT;                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
            lStdResult = DCM_E_NOT_OK;
          }
        }
      }/* Otherwise, all elements read or send prepared NRC */
    } while(lStdResult == DCM_E_LOOP);
  }

  return lStdResult;
}
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_AUTHMGR_SET_DEAUTH_ROLE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_SetDeauthenticatedRole()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_SetDeauthenticatedRole(                                                                                           /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
  uint16 connectionId,                                                                                                                               /* PRQA S 1330 */ /* MD_Dcm_RteSpecific_1330 */
  P2CONST(uint8, AUTOMATIC, RTE_DCM_APPL_DATA) deauthenticatedRole                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  uint8 lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else
  {
    Dcm_NetConnRefMemType lConnHdl = Dcm_NetGetConnHdlByConnectionId(connectionId);

    if (Dcm_DebugDetectError(lConnHdl == DCM_NET_INVALID_CONNHDL))
    {
      lErrorId = DCM_E_PARAM;
    }
    else
    {
      Dcm_CfgNetAuthInfoRefMemType lAuthInfoRef  = Dcm_NetPbCfgGetConnAuthInfoRef(lConnHdl);
      uint32                       lBitInBitMask = Dcm_UtiGetBitFromIndex(uint32, lConnHdl);

      if (Dcm_DebugDetectError(lAuthInfoRef == DCM_AUTHMGR_INVALID_AUTH_INFO_REF))
      {
        lErrorId = DCM_E_PARAM;
      }
      else if ( Dcm_DebugDetectError(((Dcm_AuthMgrSingletonContext.PendingData.ConnectionMask & lBitInBitMask) != 0u)) )
      {
        lErrorId = DCM_E_ILLEGAL_STATE;
      }
      else
      {
        /* ----- Implementation ---------------------------------------------- */
        uint8_least               lRoleIndex;
        Dcm_CfgStateGroupMemType  lRoleInLittleEndian = 0x00u;

        for (lRoleIndex = 0; lRoleIndex < DCM_AUTHMGR_CERT_ROLE_SIZE; lRoleIndex++)
        {
          lRoleInLittleEndian |= (Dcm_CfgStateGroupMemType)deauthenticatedRole[lRoleIndex]<<(lRoleIndex * 8u);                                       /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
        }

        Dcm_AuthMgrSetPendingRole(lAuthInfoRef, lRoleInLittleEndian);
        Dcm_AuthMgrSetPendingService(lAuthInfoRef, DCM_SID_SETDEAUTHENTICATEDROLE);

        Dcm_TskSetEventByThread(DCM_TSK_ID_DIAG_WORK, DCM_TSK_EV_DIAG_WORK_EXT_AUTHENTICATION, DCM_THREAD_ID_BASIC);

        /* Enter critical section (Reason: API is reentrant and needs to be protected against Dcm_(De)AuthenticateConnection/() ) */
        Dcm_UtiEnterCriticalSection();
        /*=================================*
          BEGIN CRITICAL SECTION
         *=================================*/
        Dcm_AuthMgrSingletonContext.PendingData.ConnectionMask |= lBitInBitMask;
        /*=================================*
          END CRITICAL SECTION
         *=================================*/
        Dcm_UtiLeaveCriticalSection();
      }
    }
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_SETDEAUTHENTICATEDROLE, lErrorId);

  return DCM_E_OK;
}
# endif

# if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DeauthenticateConnection()
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
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DeauthenticateConnection(
  uint16 connectionId
  )
{
  uint8          lErrorId   = DCM_E_NO_ERROR;
  Std_ReturnType lStdResult = DCM_E_NOT_OK;

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else
  {
    Dcm_NetConnRefMemType lConnHdl = Dcm_NetGetConnHdlByConnectionId(connectionId);

    if (Dcm_DebugDetectError(lConnHdl == DCM_NET_INVALID_CONNHDL))
    {
      lErrorId = DCM_E_PARAM;
    }
    else
    {
      Dcm_CfgNetAuthInfoRefMemType lAuthInfoRef  = Dcm_NetPbCfgGetConnAuthInfoRef(lConnHdl);
      uint32                       lBitInBitMask = Dcm_UtiGetBitFromIndex(uint32, lConnHdl);

      if (Dcm_DebugDetectError(lAuthInfoRef == DCM_AUTHMGR_INVALID_AUTH_INFO_REF))
      {
        lErrorId = DCM_E_PARAM;
      }
      else if ( Dcm_DebugDetectError(((Dcm_AuthMgrSingletonContext.PendingData.ConnectionMask & lBitInBitMask) != 0u)) )
      {
        lErrorId = DCM_E_ILLEGAL_STATE;
      }
      else
      {
        /* ----- Implementation ---------------------------------------------- */
#  if (DCM_AUTHMGR_SET_DEAUTH_ROLE_ENABLED == STD_ON) || (DCM_AUTHMGR_EXT_SETTER_ENABLED == STD_ON)
        Dcm_AuthMgrSetPendingService(lAuthInfoRef, DCM_SID_DEAUTHENTICATECONNECTION);
#  endif
        Dcm_AuthMgrSetPendingRole(lAuthInfoRef, DCM_AUTHMGR_DEAUTHENTICATED_ROLE);

        Dcm_TskSetEventByThread(DCM_TSK_ID_DIAG_WORK, DCM_TSK_EV_DIAG_WORK_EXT_AUTHENTICATION, DCM_THREAD_ID_BASIC);

        /* Enter critical section (Reason: API is reentrant and needs to be protected against Dcm_SetDeauthenticatedRole()/Dcm_AuthenticateConnection() ) */
        Dcm_UtiEnterCriticalSection();
        /*=================================*
          BEGIN CRITICAL SECTION
         *=================================*/
        Dcm_AuthMgrSingletonContext.PendingData.ConnectionMask |= lBitInBitMask;
        /*=================================*
          END CRITICAL SECTION
         *=================================*/
        Dcm_UtiLeaveCriticalSection();

        lStdResult = DCM_E_OK;
      }
    }
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_DEAUTHENTICATECONNECTION, lErrorId);

  return lStdResult;
}
# endif

# if (DCM_AUTHMGR_EXT_SETTER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthenticateConnection()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthenticateConnection(                                                                                           /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
  uint16 connectionId,
  P2CONST(Dcm_AuthenticationInfoType, AUTOMATIC, DCM_APPL_DATA) authenticationInfo
  )
{
  uint8          lErrorId   = DCM_E_NO_ERROR;
  Std_ReturnType lStdResult = DCM_E_NOT_OK;

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if (Dcm_DebugDetectError(authenticationInfo == NULL_PTR))
  {
    lErrorId = DCM_E_PARAM_POINTER;
  }
  else if (Dcm_DebugDetectError(authenticationInfo->AuthRole == 0u))
  {
    lErrorId = DCM_E_PARAM;
  }
#  if (DCM_AUTHMGR_WHITELISTS_ENABLED == STD_ON)
  else if (Dcm_DebugDetectError(Dcm_AuthMgrIsWhiteListValid(&authenticationInfo->WhiteLists) == FALSE))                                              /* SBSW_DCM_COMB_PTR_FORWARD */
  {
    lErrorId = DCM_E_PARAM;
  }
#  endif
  else
  {
    Dcm_NetConnRefMemType lConnHdl = Dcm_NetGetConnHdlByConnectionId(connectionId);

    if (Dcm_DebugDetectError(lConnHdl == DCM_NET_INVALID_CONNHDL))
    {
      lErrorId = DCM_E_PARAM;
    }
    else
    {
      Dcm_CfgNetAuthInfoRefMemType lAuthInfoRef  = Dcm_NetPbCfgGetConnAuthInfoRef(lConnHdl);
      uint32                       lBitInBitMask = Dcm_UtiGetBitFromIndex(uint32, lConnHdl);

      if (Dcm_DebugDetectError(lAuthInfoRef == DCM_AUTHMGR_INVALID_AUTH_INFO_REF))
      {
        lErrorId = DCM_E_PARAM;
      }
      else if ( Dcm_DebugDetectError(((Dcm_AuthMgrSingletonContext.PendingData.ConnectionMask & lBitInBitMask) != 0u)) )
      {
        lErrorId = DCM_E_ILLEGAL_STATE;
      }
      else
      {
        /* ----- Implementation ---------------------------------------------- */
#  if (DCM_AUTHMGR_WHITELISTS_ENABLED == STD_ON)
        Dcm_AuthMgrSetPendingWhiteListContextPtr(lAuthInfoRef, &(authenticationInfo->WhiteLists));                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
#  endif
        Dcm_AuthMgrSetPendingRole(lAuthInfoRef, authenticationInfo->AuthRole);
        Dcm_AuthMgrSetPendingService(lAuthInfoRef, DCM_SID_AUTHENTICATECONNECTION);

        Dcm_TskSetEventByThread(DCM_TSK_ID_DIAG_WORK, DCM_TSK_EV_DIAG_WORK_EXT_AUTHENTICATION, DCM_THREAD_ID_BASIC);

        /* Enter critical section (Reason: API is reentrant and needs to be protected against Dcm_SetDeauthenticatedRole()/Dcm_DeauthenticateConnection/() ) */
        Dcm_UtiEnterCriticalSection();
        /*=================================*
          BEGIN CRITICAL SECTION
         *=================================*/
        Dcm_AuthMgrSingletonContext.PendingData.ConnectionMask |= lBitInBitMask;
        /*=================================*
          END CRITICAL SECTION
         *=================================*/
        Dcm_UtiLeaveCriticalSection();

        lStdResult = DCM_E_OK;
      }
    }
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_AUTHENTICATECONNECTION, lErrorId);

  return lStdResult;
}
# endif

# if (DCM_STATE_AUTHENTICATION_BYPASS_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrSetAuthBypassValue()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_AuthMgrSetAuthBypassValue(
  boolean AuthBypass
)
{
  Dcm_AuthMgrSingletonContext.BypassEnabled = AuthBypass;
}
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (DCM_AUTHMGR_SUPPORT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_AuthMgr.c
 *********************************************************************************************************************/
