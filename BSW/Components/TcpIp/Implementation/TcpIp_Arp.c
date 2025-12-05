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
 *  -------------------------------------------------------------------------------------------------------------------
 *         \file  TcpIp_Arp.c
 *        \brief  Implementation of Address Resolution Protocol (ARP) for IPv4
 *
 *      \details  This file is part of the TcpIp IPv4 submodule.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the TcpIp IPv4 submodule. >> TcpIp_IpV4.h / TcpIp.h (Version 8.xx.xx and higher)
 *********************************************************************************************************************/

/*lint -e451 */ /* Suppress ID451 because TcpIp_MemMap.h / MemMap.h cannot use a include guard */


/* PRQA S 0828 EOF */ /* MD_MSR_1.1 */

/* The ARP module is an integral part of the IP module. For this the ARP module may directly access
   the IP configuration. */

#define TCPIP_ARP_SOURCE

#include "TcpIp_Cfg.h"
#include "TcpIp_Lcfg.h"

#if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                      /* COV_TCPIP_IPV4_ALWAYS_ON */
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "TcpIp_Arp.h"
# include "TcpIp_Arp_Cbk.h"
# include "TcpIp_IpV4_Priv.h"
# include "TcpIp_Priv.h"


# include "EthIf.h"


# include "TcpIp_IpV4.h"  /* needed for dynamic IP LL config */

# if (TCPIP_SUPPORT_ICMPV4 == STD_ON)                                                                                   /* COV_TCPIP_ICMPV4_ALWAYS_ON */
#  include "TcpIp_IcmpV4_Cbk.h"
# endif

# if (TCPIP_DEV_ERROR_REPORT == STD_ON)
#  include "Det.h"
# endif

# if !defined (STATIC)                                                                                                  /* COV_TCPIP_COMPATIBILITY */
#  define STATIC static
# endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

# define IPV4_ARP_HW_ADDR_TYPE_OFS         0u
# define IPV4_ARP_PR_ADDR_TYPE_OFS         2u
# define IPV4_ARP_HW_ADDR_SIZE_OFS         4u
# define IPV4_ARP_PR_ADDR_SIZE_OFS         5u
# define IPV4_ARP_OP_OFS                   6u
# define IPV4_ARP_SRC_MC_ADDR_OFS          8u
# define IPV4_ARP_SRC_IP_ADDR_OFS          14u
# define IPV4_ARP_DST_MC_ADDR_OFS          18u
# define IPV4_ARP_DST_IP_ADDR_OFS          24u

# define IPV4_ARP_INVALID_RES_IDX          TcpIp_GetSizeOfArpTableEntry()

/* Gratuitous ARP reply on Startup */
# define IPV4_ARP_GRATUITOUS_REPLY_INTERVAL (400u)  /* Time for the next gratuitous ARP reply ( every 2 seconds) */

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
/* PRQA S 3453 1 */ /* MD_MSR_FctLikeMacro */
# define IPV4_ARP_TIMESTAMP_IS_OLDER(TS_A, TS_B) IPV4_UINT32_TIMESTAMP_IS_OLDER((TS_A), (TS_B))

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

typedef uint32 IpV4_Arp_MainFuncCycleType;


/**********************************************************************************************************************
 *  LOCAL DATA
 *********************************************************************************************************************/
/* ----- */

# define TCPIP_START_SEC_VAR_NO_INIT_32
# include "TcpIp_MemMap.h"                                                                                              /* PRQA S 5087 */ /*  MD_MSR_19.1 */

STATIC VAR(IpV4_Arp_MainFuncCycleType, TCPIP_VAR_NO_INIT) IpV4_Arp_MainFuncCycle;

# define TCPIP_STOP_SEC_VAR_NO_INIT_32
# include "TcpIp_MemMap.h"                                                                                              /* PRQA S 5087 */ /*  MD_MSR_19.1 */

# define TCPIP_START_SEC_CONST_8
# include "TcpIp_MemMap.h"                                                                                              /* PRQA S 5087 */ /*  MD_MSR_19.1 */

STATIC CONST(IpBase_EthPhysAddrType, TCPIP_CONST) IpV4_Arp_PhysBroadcastAddr =
{
  0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU
};

# define TCPIP_STOP_SEC_CONST_8
# include "TcpIp_MemMap.h"                                                                                              /* PRQA S 5087 */ /*  MD_MSR_19.1 */



# define TCPIP_START_SEC_VAR_NO_INIT_8
# include "TcpIp_MemMap.h"                                                                                              /* PRQA S 5087 */ /*  MD_MSR_19.1 */

# if (TCPIP_MULTICORE_SUPPORT_ENABLED == STD_ON)                                                                        /* COV_TCPIP_MULTICORE_SUPPORT_NEVER_ON */
TCPIP_LOCAL VAR(TcpIp_OsApplicationType, TCPIP_VAR_NO_INIT) TcpIp_Arp_MainApplicationId;
# endif

# define TCPIP_STOP_SEC_VAR_NO_INIT_8
# include "TcpIp_MemMap.h"                                                                                              /* PRQA S 5087 */ /*  MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define TCPIP_START_SEC_CODE
# include "TcpIp_MemMap.h"                                                                                              /* PRQA S 5087 */ /*  MD_MSR_19.1 */
/**********************************************************************************************************************
 *  IpV4_Arp_VSendMessage
 *********************************************************************************************************************/
/*! \brief        Sends an ARP message.
 *  \details      -
 *  \param[in]    IpV4CtrlIdx           Index of the IP controller/instance.
                                        \spec requires IpV4CtrlIdx < TcpIp_GetSizeOfIpV4Ctrl(); \endspec
 *  \param[in]    RemNetAddr            Remote Network address(Destination Ip address).
 *  \param[in]    LocNetAddr            Local Network address(Source Ip address).
 *  \param[in]    RemPhysAddr           Remote Physical address( the destination MAC address).
 *                                      CONSTRAINT: [Must point to the 6 bytes of memory location.]
 *  \param[in]    Operation             Type of operation (Reply or Request).
 *  \return       E_OK                  ARP message sent successfully.
 *  \return       E_NOT_OK              ARP message could not be sent.
 *  \pre          PRE_TCPIP_ARP_INIT
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *  \synchronous  TRUE
  *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) IpV4_Arp_VSendMessage(
  TcpIp_IpV4CtrlIterType                   IpV4CtrlIdx,
  IpBase_AddrInType                        RemNetAddr,
  IpBase_AddrInType                        LocNetAddr,
  IPV4_P2C(uint8)                          RemPhysAddr,
  uint16                                   Operation);

/**********************************************************************************************************************
 *  IpV4_Arp_IsGratuitousArpReply
 *********************************************************************************************************************/
/*! \brief        Checks if the current ARP message is a gratuitous ARP message of the type 'Reply'.
 *  \details      -
 *  \param[in]    Operation             Type of ARP operation.
 *  \param[in]    RemPhysAddrPtr        Remote Physical address (the destination MAC address).
 *                                      CONSTRAINT: [Must point to the 6 bytes of memory location.]
 *  \param[in]    LocPhysAddrPtr        Local Physical address (the source MAC address).
 *                                      CONSTRAINT: [Must point to the 6 bytes of memory location.]
 *  \param[in]    RemNetAddr            Remote Network address (Destination Ip address).
 *  \param[in]    LocNetAddr            Local Network address (Source Ip address).
 *  \return       TRUE                  Message is a gratuitous ARP reply message.
 *  \return       FALSE                 Message is not a gratuitous ARP reply message.
 *  \pre          PRE_TCPIP_ARP_INIT
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *  \synchronous  TRUE
  *********************************************************************************************************************/
TCPIP_LOCAL FUNC(boolean, TCPIP_CODE) IpV4_Arp_IsGratuitousArpReply(
  uint16                                   Operation,
  IPV4_P2C(uint8)                          RemPhysAddrPtr,
  IPV4_P2C(uint8)                          LocPhysAddrPtr,
  IpBase_AddrInType                        RemNetAddr,
  IpBase_AddrInType                        LocNetAddr);

/**********************************************************************************************************************
*  IpV4_Arp_VUpdateEntryParameters
*********************************************************************************************************************/
/*! \brief          Sets the updates the entry parameters in the dynamic ARP table to validate the entry.
*  \details        -
*  \param[in]      PhysAddr                 Physical address (network byte order).
                                            CONSTRAINT: [Must point to the 6 bytes of memory location.]
*  \param[in]      NetAddr                  Network address.
*  \param[in]      ArpTableEntryIdx         Entry Index in the ARP resolution table.
*                                           CONSTRAINT: [TcpIp_GetArpTableEntryStartIdxOfIpV4Ctrl(IpV4CtrlIdx)...
*                                                        TcpIp_GetArpTableEntryEndIdxOfIpV4Ctrl(IpV4CtrlIdx)-1]
*                                           \spec requires ArpTableEntryIdx < TcpIp_GetSizeOfArpTableEntry(); \endspec
*  \pre            PRE_TCPIP_ARP_INIT
*  \context        TASK|ISR2
*  \reentrant      FALSE
*  \synchronous    TRUE
********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) IpV4_Arp_VUpdateEntryParameters(
  IPV4_P2C(uint8)                          PhysAddr,
  IpBase_AddrInType                        NetAddr,
  TcpIp_SizeOfArpTableEntryType            ArpTableEntryIdx);

/**********************************************************************************************************************
 *  IpV4_Arp_VUpdateEntry
 *********************************************************************************************************************/
/*! \brief          Sets the state of an entry in the dynamic ARP table to valid and notifies the upper layer if required.
 *  \details        -
 *  \param[in]      IpV4CtrlIdx              Index of the IP controller/instance.
 *                                           \spec requires IpV4CtrlIdx < TcpIp_GetSizeOfIpV4Ctrl(); \endspec
 *  \param[in]      PhysAddr                 Physical address (network byte order).
                                             CONSTRAINT: [Must point to the 6 bytes of memory location.]
 *  \param[in]      ArpTableEntryIdx         Entry Index in the ARP resolution table.
 *                                           CONSTRAINT: [TcpIp_GetArpTableEntryStartIdxOfIpV4Ctrl(IpV4CtrlIdx)...
 *                                                        TcpIp_GetArpTableEntryEndIdxOfIpV4Ctrl(IpV4CtrlIdx)-1]
 *                                           \spec requires ArpTableEntryIdx < TcpIp_GetSizeOfArpTableEntry(); \endspec
 *  \param[in]      EntryInTable             Indicates if an existing entry is updated.
 *  \pre            PRE_TCPIP_ARP_INIT
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
  ********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) IpV4_Arp_VUpdateEntry(
  TcpIp_IpV4CtrlIterType                   IpV4CtrlIdx,
  IPV4_P2C(uint8)                          PhysAddr,
  IpBase_AddrInType                        NetAddr,
  TcpIp_SizeOfArpTableEntryType            ArpTableEntryIdx,
  boolean                                  EntryInTable);

/**********************************************************************************************************************
 *  IpV4_Arp_VStoreDynamicEntry
 *********************************************************************************************************************/
/*! \brief        Stores an entry into the dynamic ARP table.
 *  \details      Called only if ARPCONFIG exist for the desired Ip controller.
 *  \param[in]    IpV4CtrlIdx              Index of the IP controller/instance
 *                                         \spec requires IpV4CtrlIdx < TcpIp_GetSizeOfIpV4Ctrl(); \endspec
 *                                         \spec requires TcpIp_IsArpConfigUsedOfIpV4Ctrl(IpV4CtrlIdx) == TRUE; \endspec
 *  \param[in]    NetAddr                  Network address.
 *  \param[in]    PhysAddr                 Physical address (network byte order).
 *                                         CONSTRAINT: [Must point to the 6 bytes of memory location.]
 *  \param[in]    AddIfNotExisting         TRUE:  Add a new entry if no matching entry is found.
 *                                         FALSE: Leave ARP table unchanged if no matching entry is found.
 *  \pre          PRE_TCPIP_ARP_INIT
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *  \synchronous  TRUE
  ********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(void, TCPIP_CODE) IpV4_Arp_VStoreDynamicEntry(
  TcpIp_IpV4CtrlIterType                   IpV4CtrlIdx,
  IpBase_AddrInType                        NetAddr,
  IPV4_P2C(uint8)                          PhysAddr,
  boolean                                  AddIfNotExisting);

/**********************************************************************************************************************
 *  IpV4_Arp_VPhysAddrTableChgNotification
 *********************************************************************************************************************/
/*! \brief        Notifies upper layer about a change in the ARP table related to controller, specified by IpV4CtrlIdx.
 *  \details      -
 *  \param[in]    IpV4CtrlIdx        IP controller index of the related ARP table.
 *                                   \spec requires IpV4CtrlIdx < TcpIp_GetSizeOfIpV4Ctrl(); \endspec
 *  \param[in]    ArpTableEntryIdx   ARP table entryIndex.
 *                                   \spec requires ArpTableEntryIdx < TcpIp_GetSizeOfArpTableEntry(); \endspec
 *  \param[in]    Valid              Specifies if the ARP table entry is added or changed (TRUE) or has been removed (FALSE).
 *  \pre          PRE_TCPIP_ARP_INIT
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(void, TCPIP_CODE) IpV4_Arp_VPhysAddrTableChgNotification(
  TcpIp_IpV4CtrlIterType        IpV4CtrlIdx,
  TcpIp_SizeOfArpTableEntryType ArpTableEntryIdx,
  boolean                       Valid);

/**********************************************************************************************************************
 *  IpV4_Arp_VPhysAddrTableChgDiscardedNotification
 *********************************************************************************************************************/
/*! \brief        Notifies ULs about a new ARP-table entry that was discarded or the oldest entry was replaced because the ARP-table was already full.
 *  \details      Called when new entry gets discarded only in case the functionality defined by DSGN-TcpIpIpV4AddressResolutionModes is active, i.e.:
 *                TCPIP_ARP_SUPPORT_DISCARD_HANDLING == STD_ON or the ARP table is updated with new MAC address or old entry gets overwritten when
 *                TCPIP_ARP_SUPPORT_DISCARD_HANDLING == STD_OFF
 *                Does nothing if no callback for this event is configured.
 *  \param[in]    IpV4CtrlIdx              IP controller index of the related ARP table.
 *                                         \spec requires IpV4CtrlIdx < TcpIp_GetSizeOfIpV4Ctrl(); \endspec
 *  \param[in]    NetAddr                  Specifies the IP address of the discarded/overwritten ARP table entry
 *  \pre          PRE_TCPIP_ARP_INIT
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \trace        DSGN-TcpIpIpV4AddressResolutionModes
 *********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(void, TCPIP_CODE) IpV4_Arp_VPhysAddrTableChgDiscardedNotification(
  TcpIp_IpV4CtrlIterType IpV4CtrlIdx,
  IpBase_AddrInType      NetAddr);

/**********************************************************************************************************************
 *  IpV4_Arp_VStaticTableLookup
 *********************************************************************************************************************/
/*! \brief        Lookup static configured IP address to physical address mapping.
 *  \details      This function returns the physical address for the configured IP address for the given IP controller.
 *  \param[in]    IpV4CtrlIdx           Index of the IP controller/instance
 *                                      \spec requires IpV4CtrlIdx < TcpIp_GetSizeOfIpV4Ctrl(); \endspec
 *  \param[in]    NetAddr               Network address
 *  \param[out]   PhysAddrPtr           Pointer to physical address (network byte order).
 *                                      CONSTRAINT: [Must point to the 6 bytes of memory location.]
 *                                      (May be NULL_PTR in case it shall only be checked if an entry exists.)
 *  \return       E_OK                  Static mapping has been found. *PhysAddrPtr has been updated if != NULL_PTR
 *  \return       E_NOT_OK              No static mapping has been found. *PhysAddrPtr unchanged
 *  \pre          PRE_TCPIP_ARP_INIT
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(Std_ReturnType, TCPIP_CODE) IpV4_Arp_VStaticTableLookup(
  TcpIp_IpV4CtrlIterType                     IpV4CtrlIdx,
  IpBase_AddrInType                          NetAddr,
  P2VAR(uint8, TCPIP_APPL_DATA, TCPIP_CONST) PhysAddrPtr);

/**********************************************************************************************************************
 *  IpV4_Arp_VDetectLinkLocalAddressConflict
 *********************************************************************************************************************/
/*! \brief        Notifies the link-local address configuration module about IP addresses used in received ARP packets.
 *  \details      The link-local address configuration module uses this information to detect if the IP address of this
 *                node conflicts with the address of other nodes.
 *  \param[in]    IpV4CtrlIdx      Index of the IP controller/instance.
 *                                 \spec requires IpV4CtrlIdx < TcpIp_GetSizeOfIpV4Ctrl(); \endspec
 *  \param[in]    ArpOperation     Operation of the ARP packet. (IPV4_ARP_OP_REQUEST or IPV4_ARP_OP_REPLY)
 *  \param[in]    LocIpAddr        Unicast IP address used by the IP controller/instance.
 *  \param[in]    ArpSrcIpAddr     Source IP address contained in the ARP packet.
 *  \param[in]    ArpDstIpAddr     Target IP address contained in the ARP packet.
 *  \param[in]    ArpSrcMacAddrPtr Source MAC address contained in the ARP packet.
 *                                 CONSTRAINT: [Must point to the 6 bytes of memory location.]
 *  \return                        E_OK:     No conflict was detected.
 *  \return                        E_NOT_OK: Address conflict detected.
 *  \pre          PRE_TCPIP_ARP_INIT
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) IpV4_Arp_VDetectLinkLocalAddressConflict(
  TcpIp_IpV4CtrlIterType IpV4CtrlIdx,
  uint16                 ArpOperation,
  IpBase_AddrInType      LocIpAddr,
  IpBase_AddrInType      ArpSrcIpAddr,
  IpBase_AddrInType      ArpDstIpAddr,
  IPV4_P2C(uint8)        ArpSrcMacAddrPtr);

/**********************************************************************************************************************
 *  IpV4_Arp_VUpdateTable
 *********************************************************************************************************************/
/*! \brief        Updates an existing or inserts a new entry into the dynamic ARP table.
 *  \details      -
 *  \param[in]    IpV4CtrlIdx             Index of the IP controller/instance.
 *                                        \spec requires IpV4CtrlIdx < TcpIp_GetSizeOfIpV4Ctrl(); \endspec
 *  \param[in]    ArpSrcIpAddr            Source IP address contained in the ARP packet.
 *  \param[in]    ArpSrcMacAddr           Source MAC address contained in the ARP packet.
 *                                        CONSTRAINT: [Must point to the 6 bytes of memory location.]
 *  \param[in]    AddIfNotExisting TRUE:  Add a new entry if no matching entry is found.
 *                                 FALSE: Leave ARP table unchanged if no matching entry is found.
 *  \pre          PRE_TCPIP_ARP_INIT
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(void, TCPIP_CODE) IpV4_Arp_VUpdateTable(
  TcpIp_IpV4CtrlIterType                   IpV4CtrlIdx,
  IpBase_AddrInType                        ArpSrcIpAddr,
  IPV4_P2C(uint8)                          ArpSrcMacAddr,
  boolean                                  AddIfNotExisting);

/**********************************************************************************************************************
 *  IpV4_Arp_VRespondToPacket
 *********************************************************************************************************************/
/*! \brief        Responds to a received ARP packet if required.
 *  \details      -
 *  \param[in]    IpV4CtrlIdx        Index of the IP controller/instance.
 *                                   \spec requires IpV4CtrlIdx < TcpIp_GetSizeOfIpV4Ctrl(); \endspec
 *  \param[in]    ArpOperation       Operation of the ARP packet. (IPV4_ARP_OP_REQUEST or IPV4_ARP_OP_REPLY)
 *  \param[in]    ArpSrcIpAddr       Source IP address contained in the ARP packet.
 *  \param[in]    ArpDstIpAddr       Target IP address contained in the ARP packet.
 *  \param[in]    ArpSrcMacAddr      Source MAC address contained in the ARP packet.
 *                                   CONSTRAINT: [Must point to the 6 bytes of memory location.]
 *  \pre          PRE_TCPIP_ARP_INIT
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) IpV4_Arp_VRespondToPacket(
  TcpIp_IpV4CtrlIterType                   IpV4CtrlIdx,
  uint16                                   ArpOperation,
  IpBase_AddrInType                        ArpSrcIpAddr,
  IpBase_AddrInType                        ArpDstIpAddr,
  IPV4_P2C(uint8)                          ArpSrcMacAddr);

/**********************************************************************************************************************
 *  IpV4_Arp_VHandlePacket
 *********************************************************************************************************************/
/*! \brief        Handles a received ARP packet.
 *  \details      -
 *  \param[in]    IpV4CtrlIdx        Index of the IP controller/instance.
 *                                   \spec requires IpV4CtrlIdx < TcpIp_GetSizeOfIpV4Ctrl(); \endspec
 *  \param[in]    ArpOperation       Operation of the ARP packet. (IPV4_ARP_OP_REQUEST or IPV4_ARP_OP_REPLY)
 *  \param[in]    ArpSrcIpAddr       Source IP address contained in the ARP packet.
 *  \param[in]    ArpDstIpAddr       Target IP address contained in the ARP packet.
 *  \param[in]    ArpSrcMacAddr      Source MAC address contained in the ARP packet.
 *                                   CONSTRAINT: [Must point to the 6 bytes of memory location.]
 *  \pre          PRE_TCPIP_ARP_INIT
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) IpV4_Arp_VHandlePacket(
  TcpIp_IpV4CtrlIterType                   IpV4CtrlIdx,
  uint16                                   ArpOperation,
  IpBase_AddrInType                        ArpSrcIpAddr,
  IpBase_AddrInType                        ArpDstIpAddr,
  IPV4_P2C(uint8)                          ArpSrcMacAddr);

/**********************************************************************************************************************
 * IpV4_Arp_VReadStaticTable
 *********************************************************************************************************************/
/*! \brief        Copies the elements of the static ARP table into the given user defined buffer.
 *  \details      The function is used to read the number of total arp entries available in the static table or write
 *                those available entries into the buffer provided by the user
 *  \param[in]    IpV4CtrlIdx            Index of the IP controller/instance.
 *                                       \spec requires IpV4CtrlIdx < TcpIp_GetSizeOfIpV4Ctrl(); \endspec
 *  \param[in]    NumOfElements          Maximum number of entries that can be stored.
 *  \param[out]   EntryListPtr           Pointer to the memory where the list of cache entries are stored.
 *                                       (May be NULL_PTR if only the number of entries shall be returned.)
 *  \param[out]   EntryListIdxPtr        Entry Index Pointer for the User defined buffer.
 *  \return                              EntryListPtr != NULL_PTR: Number of entries written to EntryList.
 *                                       EntryListPtr == NULL_PTR: Total number of entries to be read
 *                                                                 OR Number of all entries in Static Arp Table(cache).
 *  \pre          PRE_TCPIP_ARP_INIT
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \trace        CREQ-111995
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(uint32, TCPIP_CODE) IpV4_Arp_VReadStaticTable(
  TcpIp_IpV4CtrlIterType                                          IpV4CtrlIdx,
  uint32                                                          NumOfElements,
  P2VAR(TcpIp_ArpCacheEntryType, TCPIP_APPL_DATA, TCPIP_APPL_VAR) EntryListPtr,
  IPV4_P2V(uint32)                                                EntryListIdxPtr);

/**********************************************************************************************************************
 * IpV4_Arp_VWriteValidEntries
 *********************************************************************************************************************/
/*! \brief         Copies Dynamic ARP Table elements into the given, user defined buffer.
 *  \details       -
 *  \param[in]     IpV4CtrlIdx                       Index of the IP controller/instance.
 *                                                   \spec requires IpV4CtrlIdx < TcpIp_GetSizeOfIpV4Ctrl(); \endspec
 *  \param[in]     ArpConfigIdx                      Index to access the dynamic arp table and other config parameters.
 *                                                   \spec requires ArpConfigIdx < TcpIp_GetSizeOfArpConfig(); \endspec
 *  \param[in]     EntryListPtr                      Pointer to the memory where the list of cache entries shall be stored.
 *  \param[in,out] EntryListIdxPtr                   Entry Index Pointer for the User defined buffer.
 *                                                   CONSTRAINT: [0...TcpIp_GetTableSizeOfArpConfig(ArpConfigIdx)-1]
 *  \param[in]     DynElementsToReadLimitIdx         End Index of the Arp dynamic table.This parameter is used to
 *                                                   limit the Arp cache entries that are read from the
 *                                                   cache(dynamic table) and written in to user defined buffer.
 *                                                   CONSTRAINT: [0  ( TcpIp_GetArpTableEntryEndIdxOfIpV4Ctrl(IpV4CtrlIdx)
 *                                                                   - TcpIp_GetArpTableEntryStartIdxOfIpV4Ctrl(IpV4CtrlIdx))]
 *  \pre           PRE_TCPIP_ARP_INIT
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \trace         CREQ-111995
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) IpV4_Arp_VWriteValidEntries(
  TcpIp_IpV4CtrlIterType            IpV4CtrlIdx,
  TcpIp_SizeOfArpConfigType         ArpConfigIdx,
  IPV4_P2V(TcpIp_ArpCacheEntryType) EntryListPtr,
  IPV4_P2V(uint32)                  EntryListIdxPtr,
  uint32                            DynElementsToReadLimitIdx);

/**********************************************************************************************************************
 * IpV4_Arp_VReadDynTable
 *********************************************************************************************************************/
/*! \brief         Copies Dynamic ARP Table elements into the given, user defined buffer.
 *  \details       -
 *  \param[in]     IpV4CtrlIdx            Index of the IP controller/instance.
 *                                        \spec requires IpV4CtrlIdx < TcpIp_GetSizeOfIpV4Ctrl(); \endspec
 *  \param[in]     NumOfElements          Number of entries that can be read from the dynamic Arp table.
 *  \param[out]    EntryListPtr           Pointer to the memory where the list of cache entries shall be stored.
 *                                        (May be NULL_PTR if only the number of entries shall be returned.)
 *  \param[in]     EntryListIdxPtr        Index Pointer for the User defined  Entry List buffer.
 *  \return                               EntryListPtr != NULL_PTR: Number of entries written to EntryList.
 *                                        EntryListPtr == NULL_PTR: Total number of entries in cache.
 *                                        CONSTRAINT: [0...TcpIp_GetTableSizeOfArpConfig(ArpConfigIdx)-1]
 *  \pre           PRE_TCPIP_ARP_INIT
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \trace         CREQ-111995
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(uint32, TCPIP_CODE) IpV4_Arp_VReadDynTable(
  TcpIp_IpV4CtrlIterType            IpV4CtrlIdx,
  uint32                            NumOfElements,
  IPV4_P2V(TcpIp_ArpCacheEntryType) EntryListPtr,
  IPV4_P2V(uint32)                  EntryListIdxPtr);

/**********************************************************************************************************************
 * IpV4_Arp_VCreateEntry
 *********************************************************************************************************************/
/*! \brief          Create new entry and trigger ARP request in case the caller desires that.
 *  \details        -
 *  \param[in]      IpV4CtrlIdx            Index of the IP controller/instance.
 *                                         \spec requires IpV4CtrlIdx < TcpIp_GetSizeOfIpV4Ctrl(); \endspec
 *  \param[in]      IpV4SocketIdx          Socket index
 *                                         \spec requires (IpV4SocketIdx < TcpIp_GetSizeOfIpV4SocketDyn() || IpV4SocketIdx == IPV4_INV_SOCK_IDX); \endspec
 *  \param[in]      NetAddr                Network address (network byte order).
 *  \param[in]      DiscardNewEntry        Flag to indicate that Table contains the oldest entry that will be overwritten or new entry will be discarded.
 *  \param[in]      ArpTableEntryIdx       Index of ARP entry that shall be created.
 *                                         \spec requires ArpTableEntryIdx < TcpIp_GetSizeOfArpTableEntry(); \endspec
 *  \return         TCPIP_E_PHYS_ADDR_MISS  Address could not be resolved, resolution has been triggered successfully.
 *                  TCP_E_NOT_OK            Address could not be resolved, resolution has NOT been triggered
 *
 *  \pre            PRE_TCPIP_ARP_INIT
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
IPV4_LOCAL FUNC(TcpIp_ReturnType, TCPIP_CODE) IpV4_Arp_VCreateEntry(
  TcpIp_IpV4CtrlIterType        IpV4CtrlIdx,
  TcpIp_IpV4SocketDynIterType   IpV4SocketIdx,
  IpBase_AddrInType             NetAddr,
  boolean                       DiscardNewEntry,
  TcpIp_SizeOfArpTableEntryType ArpTableEntryIdx);

/**********************************************************************************************************************
 * IpV4_Arp_VCheckValidityOfEntry
 *********************************************************************************************************************/
/*! \brief          Check if the Entry is Valid or out of date.
 *  \details        This function verifies whether an ARP entry is too old, if so then it resets the entry timer,
 *                  changes status of that entry to INPROGRESS and tries to send the ARP request again.
 *  \param[in]      IpV4CtrlIdx             Index of the IP controller/instance.
 *                                          \spec requires IpV4CtrlIdx < TcpIp_GetSizeOfIpV4Ctrl(); \endspec
 *  \param[out]     PhysAddrPtr             Pointer to the Physical address.
 *                                          \spec requires $lengthOf(PhysAddrPtr) >= IPBASE_ETH_PHYS_ADDR_LEN_BYTE; \endspec
 *  \param[in]      NetAddr                 Network address (network byte order).
 *  \param[in]      ArpTableEntryIdx        ARP resolution table Index.
 *                                          \spec requires ArpTableEntryIdx < TcpIp_GetSizeOfArpTableEntry(); \endspec
 *  \return         TCPIP_E_PHYS_ADDR_MISS  ARP request already is in progress.
 *                  TCP_E_NOT_OK            Not resolved
 *                  TCP_E_OK                there is a valid entry for the given network address

 *  \pre            PRE_TCPIP_ARP_INIT
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(TcpIp_ReturnType, TCPIP_CODE) IpV4_Arp_VCheckValidityOfEntry(
  TcpIp_IpV4CtrlIterType                   IpV4CtrlIdx,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) PhysAddrPtr,
  IpBase_AddrInType                        NetAddr,
  TcpIp_SizeOfArpTableEntryType            ArpTableEntryIdx);

/**********************************************************************************************************************
 * IpV4_Arp_VCheckArpEntry
 *********************************************************************************************************************/
/*! \brief          Check if the entry is already in the ARP table or not.
 *  \details        This function checks whether a valid entry exists in ARP resolution table.
 *  \param[in]      IpV4CtrlIdx               Index of the IP controller/instance.
 *                                            \spec requires IpV4CtrlIdx < TcpIp_GetSizeOfIpV4Ctrl(); \endspec
 *  \param[in]      IpV4SocketIdx             Socket index
 *                                            \spec requires (IpV4SocketIdx < TcpIp_GetSizeOfIpV4SocketDyn() || IpV4SocketIdx == IPV4_INV_SOCK_IDX); \endspec
 *                                            (If a valid socket index is specified, the index of the matching ARP entry, if there is one,
 *                                            is stored related to the socket in order to allow an optimized lookup next time.)
 *  \param[in]      NetAddr                   Network address (network byte order).
 *  \param[out]     DiscardNewEntryPtr        Indicates whether the ARP cache is completely filled and new incoming entry should be discarded.
 *                                            CONSTRAINT: [Parameter is updated only when no matching entry is found.]
 *  \param[out]     OldestArpTableEntryIdxPtr Index of the oldest ARP table entry.
 *                                            This parameter is used only if the dynamic arp table is full and we need space to add new arp entry.
 *  \return         Index of the matching entry                      if any valid maching entry found.
 *                  TCPIP_NO_ARPTABLEENTRYIDXOFIPV4SOCKETDYN         if no matching entry is found.
 *  \pre            PRE_TCPIP_ARP_INIT
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(TcpIp_SizeOfArpTableEntryType, TCPIP_CODE) IpV4_Arp_VCheckArpEntry(
  TcpIp_IpV4CtrlIterType                                          IpV4CtrlIdx,
  TcpIp_IpV4SocketDynIterType                                     IpV4SocketIdx,
  IpBase_AddrInType                                               NetAddr,
  P2VAR(boolean, AUTOMATIC, TCPIP_APPL_VAR)                       DiscardNewEntryPtr,
  P2VAR(TcpIp_SizeOfArpTableEntryType, AUTOMATIC, TCPIP_APPL_VAR) OldestArpTableEntryIdxPtr);

/**********************************************************************************************************************
 * IpV4_Arp_VProcessPendingEntries
 *********************************************************************************************************************/
/*! \brief          Retransmits ARP requests of pending entries if required and handles address resolution timeout.
 *  \details        -
 *  \param[in]      IpV4CtrlIdx              Index of the IP controller/instance.
 *                                           \spec requires IpV4CtrlIdx < TcpIp_GetSizeOfIpV4Ctrl(); \endspec
 *  \pre            PRE_TCPIP_ARP_INIT
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) IpV4_Arp_VProcessPendingEntries(TcpIp_IpV4CtrlIterType IpV4CtrlIdx);

/**********************************************************************************************************************
 *  IpV4_Arp_GetPhysicalAddressFromDynamicTables
 *********************************************************************************************************************/
/*! \brief        Get physical address from a dynamic ARP table for a given network address.
 *  \details      The cache table access is optimized for requesting the same address many times for the same socket.
 *  \param[in]    IpV4CtrlIdx             Index of the IP controller/instance.
 *                                        \spec requires IpV4CtrlIdx < TcpIp_GetSizeOfIpV4Ctrl(); \endspec
 *  \param[in]    IpV4SocketIdx           Socket index
 *                                        \spec requires (IpV4SocketIdx < TcpIp_GetSizeOfIpV4SocketDyn() || IpV4SocketIdx == IPV4_INV_SOCK_IDX); \endspec
 *  \param[in]    NetAddr                 Network address (network byte order)
 *  \param[out]   PhysAddrPtr             Pointer for physical address (network byte order)
 *                                        \spec requires $lengthOf(PhysAddrPtr) >= IPBASE_ETH_PHYS_ADDR_LEN_BYTE; \endspec
 *  \param[in]    InitRes                 Specifies if the address resolution shall be initiated (TRUE) or not (FALSE) in
 *                                        case the physical address related to the specified IP address is currently
 *                                        unknown.
 *  \return       TCP_OK                  Address could be resolved
 *                TCP_E_NOT_OK            Address could not be resolved, resolution has NOT been triggered, even in case
 *                                        the parameter InitRes has indicated the callers wish to do so,
 *                TCPIP_E_PHYS_ADDR_MISS  Address could not be resolved, resolution has been triggered successfully,
 *                                        depending on the value of the parameter InitRes.
 *  \pre           PRE_TCPIP_ARP_INIT
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(TcpIp_ReturnType, TCPIP_CODE) IpV4_Arp_GetPhysicalAddressFromDynamicTables(
  TcpIp_IpV4CtrlIterType                   IpV4CtrlIdx,
  TcpIp_SizeOfIpV4SocketDynType            IpV4SocketIdx,
  IpBase_AddrInType                        NetAddr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) PhysAddrPtr,
  boolean                                  InitRes);

/**********************************************************************************************************************
 *  IpV4_Arp_VReadMsgData
 *********************************************************************************************************************/
/*! \brief        Reads data from a received ARP message.
 *  \details      Called by the Arp_RxIndication when a message arrives.
 *  \param[in]    EthIfCtrlIdx          EthIf controller index
 *                                      \spec requires EthIfCtrlIdx < TcpIp_GetSizeOfEthIfCtrl(); \endspec
 *  \param[in]    DataPtr               Pointer to the received data
 *  \param[in]    LenByte               Length of received data in bytes
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) IpV4_Arp_VReadMsgData(
  TcpIp_SizeOfEthIfCtrlType                  EthIfCtrlIdx,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint16                                     LenByte);

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  IpV4_Arp_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
FUNC(void, TCPIP_CODE) IpV4_Arp_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_IpV4SocketDynIterType ipV4SocketIdx;
  TcpIp_ArpTableEntryIterType arpTableEntryIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize dynamic ARP table entries */
  for (arpTableEntryIdx = 0; arpTableEntryIdx < TcpIp_GetSizeOfArpTableEntry(); arpTableEntryIdx++)
  {
    /* #20 Invalidate all entries in the ARP table. */
    /* Set all elements of the arp table as Invalid. */
    TcpIp_SetStatusOfArpTableEntry(arpTableEntryIdx, IPV4_ARP_ELSTATUS_INVALID);
  }

  /* Set Ip controller to invalid for all sockets. */
  /* Optimization to get the last stored arp entry for this socket index. */
  /* #30 Iterate over all socket index and reset the arp entry and Ipv4 controller index referenced by it. */
  for (ipV4SocketIdx = 0; ipV4SocketIdx < TcpIp_GetSizeOfIpV4SocketDyn(); ipV4SocketIdx++)
  {
    TcpIp_SetIpV4CtrlIdxOfIpV4SocketDyn(ipV4SocketIdx, TCPIP_NO_IPV4CTRLIDXOFIPV4SOCKETDYN);
    /* Reset the arp entry which was saved for this socket index. */
    TcpIp_SetArpTableEntryIdxOfIpV4SocketDyn(ipV4SocketIdx, TCPIP_NO_ARPTABLEENTRYIDXOFIPV4SOCKETDYN);
  }

  if (TcpIp_GetSizeOfGratuitousArp() != 0u)
  {
    TcpIp_GratuitousArpIterType gratuitousArpIdx;

    /* If gratuitous Arps are configured, clear counters */
    for (gratuitousArpIdx = 0; gratuitousArpIdx < TcpIp_GetSizeOfGratuitousArp(); gratuitousArpIdx++)
    {
      TcpIp_SetTransmissionCntOfGratuitousArp(gratuitousArpIdx, 0u);
      TcpIp_SetNextTransmissionMsOfGratuitousArp(gratuitousArpIdx, 0u);
    }
  }

# if (TCPIP_MULTICORE_SUPPORT_ENABLED == STD_ON)                                                                        /* COV_TCPIP_MULTICORE_SUPPORT_NEVER_ON */
  /* Set the application which calls Init as the main application */
  TcpIp_Arp_MainApplicationId = GetApplicationID();
# endif
  {
    TcpIp_IpV4CtrlDynIterType ipV4CtrlIdx;

    /* Reset pending ARP for each controller */
    for (ipV4CtrlIdx = 0; ipV4CtrlIdx < TcpIp_GetSizeOfIpV4CtrlDyn(); ipV4CtrlIdx++)
    {
      TcpIp_SetNumPendArpElemOfIpV4CtrlDyn(ipV4CtrlIdx, 0);
    }
  }

  /* #40 Reset the global variables.*/
  IpV4_Arp_MainFuncCycle = 0;

} /* IpV4_Arp_Init() */

/**********************************************************************************************************************
 *  IpV4_AddArpCacheEntry
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
FUNC(Std_ReturnType, TCPIP_CODE) IpV4_AddArpCacheEntry(
  TcpIp_SizeOfEthIfCtrlDynType       IpV4CtrlIdx,
  IpBase_AddrInType                  IpAddr,
  TCPIP_P2C(uint8)                   PhysAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 phyAddrDummy[6];

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Verify that this entry do not exists in the static ARP table */
/* coding -v34 */ /* VCR_TCPIP_E_NOT_OK */
  if (IpV4_Arp_VStaticTableLookup(IpV4CtrlIdx, IpAddr, &phyAddrDummy[0]) == E_NOT_OK)
/* coding +v34 */
  {
    const TcpIp_ArpTableEntryIterType endIdx = TcpIp_GetArpTableEntryEndIdxOfIpV4Ctrl(IpV4CtrlIdx);
# if (TCPIP_SUPPORT_ARP_DISCARDED_ENTRY_HANDLING == STD_ON)
    TcpIp_ArpTableEntryIterType oldestArpTableEntryIdx = endIdx;
    IpV4_Arp_MainFuncCycleType oldestTimeStamp = IpV4_Arp_MainFuncCycle;
# endif
    TcpIp_ArpTableEntryIterType foundArpTableEntryIdx = endIdx;
    TcpIp_ArpTableEntryIterType currArpTableEntryIdx = 0u;

    /* #20 Search for free or fitting entries in dynamic ARP table */
    IPV4_VENTER_CRITICAL_SECTION();
    for (currArpTableEntryIdx = TcpIp_GetArpTableEntryStartIdxOfIpV4Ctrl(IpV4CtrlIdx);
         currArpTableEntryIdx < endIdx;
         currArpTableEntryIdx++)
    {
      if (   (TcpIp_GetNetAddrOfArpTableEntry(currArpTableEntryIdx) == IpAddr)
          && (TcpIp_IsInfinitTimeOfArpTableEntry(currArpTableEntryIdx) == FALSE))
      {
        foundArpTableEntryIdx = currArpTableEntryIdx;
        break; /* exit for, this Item can be used */
      }
      else if (TcpIp_GetStatusOfArpTableEntry(currArpTableEntryIdx) == IPV4_ARP_ELSTATUS_INVALID)
      {
        foundArpTableEntryIdx = currArpTableEntryIdx;
      }
      else
      {
        /* ARP table entry is not relevant */
      }

# if (TCPIP_SUPPORT_ARP_DISCARDED_ENTRY_HANDLING == STD_ON)
      /* Also search for the oldest, not via API set entry */
      if ((TcpIp_GetTimeStampOfArpTableEntry(currArpTableEntryIdx) < oldestTimeStamp)
          && (TcpIp_IsInfinitTimeOfArpTableEntry(currArpTableEntryIdx) == FALSE))
      {
        oldestTimeStamp = TcpIp_GetTimeStampOfArpTableEntry(currArpTableEntryIdx);
        oldestArpTableEntryIdx = currArpTableEntryIdx;                                                                  /* PRQA S 2983 */ /* MD_TCPIP_FalsePossible_Rule2983 */
      }
# endif
    }

# if (TCPIP_SUPPORT_ARP_DISCARDED_ENTRY_HANDLING == STD_ON)
    /* #30 If no usable entry is found, use the oldest entry */
    if (   (foundArpTableEntryIdx == endIdx)
        && (oldestArpTableEntryIdx != endIdx))
    {
      foundArpTableEntryIdx = oldestArpTableEntryIdx;

      /* #40 Inform the UL that the oldes found address will be overwritten */
      IpV4_Arp_VPhysAddrTableChgDiscardedNotification(IpV4CtrlIdx, (TcpIp_SizeOfArpTableEntryType)foundArpTableEntryIdx);
    }
# endif

    /* #50 Write new ARP entry to table or overwrite it */
    if (foundArpTableEntryIdx != endIdx)
    {
      /* If a entry in state "InProgress" is overwritten, decrease the pend counter. */
      if (TcpIp_GetStatusOfArpTableEntry(foundArpTableEntryIdx) == IPV4_ARP_ELSTATUS_INPROGESS)
      {
        TcpIp_SubNumPendArpElemOfIpV4CtrlDyn(IpV4CtrlIdx, 1u);
      }
      TcpIp_SetStatusOfArpTableEntry(foundArpTableEntryIdx, IPV4_ARP_ELSTATUS_VALID);
      TcpIp_SetTimeStampOfArpTableEntry(foundArpTableEntryIdx, IpV4_Arp_MainFuncCycle);
      TcpIp_SetCreationTimeStampOfArpTableEntry(foundArpTableEntryIdx, IpV4_Arp_MainFuncCycle);
      TcpIp_SetInfinitTimeOfArpTableEntry(foundArpTableEntryIdx, TRUE);
      TcpIp_SetNetAddrOfArpTableEntry(foundArpTableEntryIdx, IpAddr);
      /* Write phys addr to entry */
      TcpIp_VCopyPhysAddr(&(TcpIp_GetAddrPhysAddrOfArpTableEntry(foundArpTableEntryIdx)->data[0u]), &PhysAddrPtr[0u]);

      IpV4_Arp_VPhysAddrTableChgNotification(IpV4CtrlIdx, (TcpIp_SizeOfArpTableEntryType)foundArpTableEntryIdx, TRUE);
      retVal = E_OK;
    }
    else
    {
      /* No usable entry found, return E_NOT_OK */
      IpV4_Arp_VPhysAddrTableChgDiscardedNotification(IpV4CtrlIdx, IpAddr);
    }
    IPV4_VLEAVE_CRITICAL_SECTION();
  } /* Test for static Table */
  else
  {
# if (TCPIP_DEV_ERROR_REPORT == STD_ON)
    TcpIp_CallDetReportError(TCPIP_API_ID_ARP_API, TCPIP_E_NOT_SUPPORTED);
# endif
  }

  return retVal;
}

/**********************************************************************************************************************
 *  IpV4_DelArpCacheEntry
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) IpV4_DelArpCacheEntry(
  TcpIp_SizeOfEthIfCtrlDynType       IpV4CtrlIdx,
  IpBase_AddrInType                  IpAddr)
{
  /* ----- Local Variables ---------------------------------------------- */
  const TcpIp_ArpTableEntryIterType endIdx = TcpIp_GetArpTableEntryEndIdxOfIpV4Ctrl(IpV4CtrlIdx);
  TcpIp_ArpTableEntryIterType currArpTableEntryIdx;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* Throw DET error if IP address is part of thes static ARP table */
# if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  uint8 phyAddrDummy[6];
  if (IpV4_Arp_VStaticTableLookup(IpV4CtrlIdx, IpAddr, &phyAddrDummy[0]) == E_OK)
  {
#  if (TCPIP_DEV_ERROR_REPORT == STD_ON)
    TcpIp_CallDetReportError(TCPIP_API_ID_ARP_API, TCPIP_E_NOT_SUPPORTED);
#  endif
  }
  else
# endif
  {
    IPV4_VENTER_CRITICAL_SECTION();
    /* #10 Iterate over full dynamic ARP table */
    for (currArpTableEntryIdx = TcpIp_GetArpTableEntryStartIdxOfIpV4Ctrl(IpV4CtrlIdx);
         currArpTableEntryIdx < endIdx;
         currArpTableEntryIdx++)
    {
      /* #20 Test that the IPv4 address fits */
      if ((TcpIp_GetNetAddrOfArpTableEntry(currArpTableEntryIdx) == IpAddr)
        && (TcpIp_GetStatusOfArpTableEntry(currArpTableEntryIdx) != IPV4_ARP_ELSTATUS_INVALID))
      {
        /* #30 Clear variables */
        if (TcpIp_GetStatusOfArpTableEntry(currArpTableEntryIdx) == IPV4_ARP_ELSTATUS_INPROGESS)
        {
          TcpIp_SubNumPendArpElemOfIpV4CtrlDyn(IpV4CtrlIdx, 1u);
        }
        IpV4_Arp_VPhysAddrTableChgDiscardedNotification(IpV4CtrlIdx,
          /* Discarded */ TcpIp_GetNetAddrOfArpTableEntry(currArpTableEntryIdx));
        TcpIp_SetStatusOfArpTableEntry(currArpTableEntryIdx, IPV4_ARP_ELSTATUS_INVALID);
        TcpIp_SetNetAddrOfArpTableEntry(currArpTableEntryIdx, 0u);
        TcpIp_VClearPhysAddr(&(TcpIp_GetAddrPhysAddrOfArpTableEntry(currArpTableEntryIdx)->data[0u]));

        /* #40 Inform UL */
        IpV4_Arp_VPhysAddrTableChgNotification(IpV4CtrlIdx, (TcpIp_SizeOfArpTableEntryType)currArpTableEntryIdx, FALSE);

        retVal = E_OK;
      }
    }
    IPV4_VLEAVE_CRITICAL_SECTION();
  }
  return retVal;
}

/**********************************************************************************************************************
 *  IpV4_Arp_MapIpToPhysMulticastAddr
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) IpV4_Arp_MapIpToPhysMulticastAddr(
  IpBase_AddrInType NetMcAddr,
  IPV4_P2V(uint8)   PhysMcAddr)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(PhysMcAddr != NULL_PTR);
  TCPIP_ASSERT(IPV4_ADDR_IS_MULTICAST(NetMcAddr));
  {
    /* #10 Calculate corresponding MAC group address from specified multicast IP address. */
    /* "An IP host group address is mapped to an Ethernet multicast address
      *  by placing the low-order 23-bits of the IP address into the low-order
      *  23 bits of the Ethernet multicast address 01-00-5E-00-00-00 (hex)."
      * [RFC1112 6.4. Extensions to an Ethernet Local Network Module]
      */
      /* #20 Convert IP address from network byte order into host byte order. */
    IpBase_AddrInType netMcAddrHbo = TCPIP_NTOHL(NetMcAddr);

    /* #30 Set first three bytes 01:00:5e:XX:XX:XX */
    PhysMcAddr[0] = 0x01U;
    PhysMcAddr[1] = 0x00U;
    PhysMcAddr[2] = 0x5EU;

    /* #40 Set last 23 bits of IP address */
    PhysMcAddr[3] = (uint8)((netMcAddrHbo >> 16u) & 0x0000007FU); /* 7 bit */
    PhysMcAddr[4] = (uint8)(netMcAddrHbo >> 8u);                 /* 8 bit */
    PhysMcAddr[5] = (uint8)(netMcAddrHbo);                      /* 9 bit */
  }
} /* IpV4_Arp_MapIpToPhysMulticastAddr() */

/**********************************************************************************************************************
 *  IpV4_Arp_VCreateEntry
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
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(TcpIp_ReturnType, TCPIP_CODE) IpV4_Arp_VCreateEntry(
  TcpIp_IpV4CtrlIterType        IpV4CtrlIdx,
  TcpIp_IpV4SocketDynIterType   IpV4SocketIdx,
  IpBase_AddrInType             NetAddr,
  boolean                       DiscardNewEntry,
  TcpIp_SizeOfArpTableEntryType ArpTableEntryIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_ReturnType retValue = TCPIP_E_PHYS_ADDR_MISS;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 In case DiscardedEntryHandling is active: Only modify an entry in case the ARP-table-entry is NOT full. */
# if (TCPIP_SUPPORT_ARP_DISCARDED_ENTRY_HANDLING == STD_ON)
  /* Runtime check TcpIp_IsArpDiscardedEntryHandlingOfIpV4General(TCPIP_IPV4GENERAL_IDX) is converted into Preprocessor check.*/
  if (DiscardNewEntry == FALSE)
# else
  TCPIP_DUMMY_STATEMENT_CONST(DiscardNewEntry);                                                                         /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
# endif
  {
    TcpIp_ReturnType arpReqSent = E_NOT_OK;
    /* #20 Enter exclusive area. */
    IPV4_VENTER_CRITICAL_SECTION();
    /* Set entry parameters. */
    TcpIp_VClearPhysAddr(&(TcpIp_GetAddrPhysAddrOfArpTableEntry(ArpTableEntryIdx)->data[0u]));
    /* Overwrite the details of "ArpTableEntryIdx" with new information. */
    TcpIp_SetNetAddrOfArpTableEntry(ArpTableEntryIdx, NetAddr);
    /* #30 Set status of entry as "In Progress". */
    TcpIp_SetStatusOfArpTableEntry(ArpTableEntryIdx, IPV4_ARP_ELSTATUS_INPROGESS);
    TcpIp_SetTimeStampOfArpTableEntry(ArpTableEntryIdx, IpV4_Arp_MainFuncCycle);
    TcpIp_SetCreationTimeStampOfArpTableEntry(ArpTableEntryIdx, IpV4_Arp_MainFuncCycle);
    TcpIp_SetInfinitTimeOfArpTableEntry(ArpTableEntryIdx, FALSE);
    IPV4_VLEAVE_CRITICAL_SECTION();

    /* #40 Send ARP request. */
    arpReqSent = IpV4_Arp_SendArpRequest(IpV4CtrlIdx, NetAddr);

    /* #50 Enter exclusive area. */
    IPV4_VENTER_CRITICAL_SECTION();
    /* #60 Update the socket related information of "ArpTableEntryIdx", if ARP request is sent successfully. */
    if (arpReqSent == E_OK)
    {
      /* #70 Verify that the Socket Index is valid. */
      if (IpV4SocketIdx != IPV4_INV_SOCK_IDX)
      {
        TcpIp_SetIpV4CtrlIdxOfIpV4SocketDyn(IpV4SocketIdx, (TcpIp_SizeOfIpV4CtrlType)IpV4CtrlIdx);
        /* #80 Optimization: ARP Entry index is saved for the socket.So next time if the same socket is used to send the
                             ARP request, saved ARP entry is checked first instead to iterate over whole ARP table. */
        TcpIp_SetArpTableEntryIdxOfIpV4SocketDyn(IpV4SocketIdx, ArpTableEntryIdx);
      }
    }
    /* #90 Increment the global variable of pending entry, which is checked in IpV4_Arp_MainFunction. */
    TcpIp_IncNumPendArpElemOfIpV4CtrlDyn(IpV4CtrlIdx);

    IPV4_VLEAVE_CRITICAL_SECTION();
  }
# if (TCPIP_SUPPORT_ARP_DISCARDED_ENTRY_HANDLING == STD_ON)
  /* #100 Otherwise Inform the upper layer about discarded entry, in case the new entry overwrite an existing entry when table is full.*/
  else
  {
    /* Due to the if-statement of this if-else-block it is sure that DiscardedEntryHandling is active, when we get here. */
    /* In case the new entry would overwrite an existing entry because the table is "full":
       Conduct the optional callout to indicate, which entry was discarded.
       IpV4_Arp_VPhysAddrTableChgNotification() is NOT used to do the discard callback, because in case of a new entry being discarded the ARP table is not changed. */
    IpV4_Arp_VPhysAddrTableChgDiscardedNotification(IpV4CtrlIdx, /* Discarded */ NetAddr);
    retValue = TCPIP_E_NOT_OK;
  }
# endif
  return retValue;
} /* IpV4_Arp_VCreatNewEntry() */

/**********************************************************************************************************************
 *  IpV4_Arp_VCheckValidityOfEntry
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
/* PRQA S 6050, 6080 1 */ /* MD_TcpIp_STCAL_CslAccess, MD_MSR_STMIF */
TCPIP_LOCAL FUNC(TcpIp_ReturnType, TCPIP_CODE) IpV4_Arp_VCheckValidityOfEntry(
  TcpIp_IpV4CtrlIterType                   IpV4CtrlIdx,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) PhysAddrPtr,
  IpBase_AddrInType                        NetAddr,
  TcpIp_SizeOfArpTableEntryType            ArpTableEntryIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_ReturnType retValue = TCPIP_E_PHYS_ADDR_MISS;
  TcpIp_SizeOfArpConfigType arpConfigIdx = TcpIp_GetArpConfigIdxOfIpV4Ctrl(IpV4CtrlIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the entry is too old or not. */
  /* timestamp contains the value in terms of mainfunction cycles */
  IpV4_Arp_MainFuncCycleType timestamp = TcpIp_GetCreationTimeStampOfArpTableEntry(ArpTableEntryIdx);

  TCPIP_ASSERT(arpConfigIdx     < TcpIp_GetSizeOfArpConfig());
  TCPIP_ASSERT(ArpTableEntryIdx < TcpIp_GetSizeOfArpTableEntry());

  /* we know that IpV4_Arp_MainFuncCycle will always be bigger than timestamp, so the values can be subtracted without considering the integer overflow */
  if (   (TcpIp_GetEntryTimeoutOfArpConfig(arpConfigIdx) == 0u)                                                         /* PRQA S 3415 2 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
      || ((IpV4_Arp_MainFuncCycle - timestamp) < TcpIp_GetEntryTimeoutOfArpConfig(arpConfigIdx)))
  {
    /* table entry is not out of date */
    if (   (TcpIp_GetStatusOfArpTableEntry(ArpTableEntryIdx) == IPV4_ARP_ELSTATUS_VALID)                                /* PRQA S 3415 2 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
        || (TcpIp_GetStatusOfArpTableEntry(ArpTableEntryIdx) == IPV4_ARP_ELSTATUS_PROBE))
    {
      /* #20 If Valid  entry is found for this network address, then return physical address. */
      TcpIp_VCopyPhysAddr(PhysAddrPtr, &TcpIp_GetAddrPhysAddrOfArpTableEntry(ArpTableEntryIdx)->data[0]);
      retValue = TCPIP_OK;
    }
    /* #30 Otherwise: Send Arp request, If Retry time is not expired. */
    else if ( (IpV4_Arp_MainFuncCycle - TcpIp_GetCreationTimeStampOfArpTableEntry(ArpTableEntryIdx))
             < TcpIp_GetRetryTimeOfArpConfig(arpConfigIdx))
    {
      /* #40 Send Arp Request if Upper layer has triggered transmission after ArpRequestTimeout. */
      if (  (IpV4_Arp_MainFuncCycle - TcpIp_GetTimeStampOfArpTableEntry(ArpTableEntryIdx))
          >= TcpIp_GetRequestTimeoutOfArpConfig(arpConfigIdx))
      {
        if (IpV4_Arp_SendArpRequest(IpV4CtrlIdx, NetAddr) == E_OK)
        {
          TcpIp_SetTimeStampOfArpTableEntry(ArpTableEntryIdx, IpV4_Arp_MainFuncCycle);
        }
      }
    }
    else
    {
      /* else: ARP request already is in progress, RetValue remains TCPIP_E_PHYS_ADDR_MISS */
    }
  }
  else
  {
    /* #50 If Entry is too old, Change its state to PROBE continue and reset timestamp. */
    TcpIp_SetStatusOfArpTableEntry(ArpTableEntryIdx, IPV4_ARP_ELSTATUS_PROBE);
    TcpIp_SetTimeStampOfArpTableEntry(ArpTableEntryIdx, IpV4_Arp_MainFuncCycle);
    TcpIp_SetCreationTimeStampOfArpTableEntry(ArpTableEntryIdx, IpV4_Arp_MainFuncCycle);

    TcpIp_IncNumPendArpElemOfIpV4CtrlDyn(IpV4CtrlIdx);

    retValue = TCPIP_OK;
    /*#60 Send Arp Request for the above too old  arp entry but still existed in the arp Resolution table(in state PROBE).*/
    if(IpV4_Arp_SendArpRequest(IpV4CtrlIdx, NetAddr) != E_OK)
    {
      retValue = TCPIP_E_NOT_OK;
    }
    /* else RetValue remains TCPIP_OK */
  }
  return retValue;
} /* IpV4_Arp_VCheckValidityOfEntry() */

/**********************************************************************************************************************
 *  IpV4_Arp_VCheckArpEntry
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
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(TcpIp_SizeOfArpTableEntryType, TCPIP_CODE) IpV4_Arp_VCheckArpEntry(
  TcpIp_IpV4CtrlIterType                                          IpV4CtrlIdx,
  TcpIp_IpV4SocketDynIterType                                     IpV4SocketIdx,
  IpBase_AddrInType                                               NetAddr,
  P2VAR(boolean, AUTOMATIC, TCPIP_APPL_VAR)                       DiscardNewEntryPtr,
  P2VAR(TcpIp_SizeOfArpTableEntryType, AUTOMATIC, TCPIP_APPL_VAR) OldestArpTableEntryIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_ArpTableEntryIterType arpTableEntryIdx;

  /* Assume that first entry is the least recently used entry, this value is updated below if an older entry is found. */
  *OldestArpTableEntryIdxPtr = TcpIp_GetArpTableEntryStartIdxOfIpV4Ctrl(IpV4CtrlIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* there is no valid entry in mapping table */
  /* #10 Iterate over ARP entry table. */
  for (arpTableEntryIdx = TcpIp_GetArpTableEntryStartIdxOfIpV4Ctrl(IpV4CtrlIdx);
       arpTableEntryIdx < TcpIp_GetArpTableEntryEndIdxOfIpV4Ctrl(IpV4CtrlIdx);
       arpTableEntryIdx++)
  {
    /* #20 Check if ARP entry already exists (without entry in mapping table). */
    if (TcpIp_GetStatusOfArpTableEntry(arpTableEntryIdx) != IPV4_ARP_ELSTATUS_INVALID)
    {
      /* #30 Compare network addresses. */
      if (TcpIp_GetNetAddrOfArpTableEntry(arpTableEntryIdx) == NetAddr)
      {
        /* #40 If the entry is found("valid" or "in progress"), update socket mapping table. So entry is found faster next time. */
        if (IpV4SocketIdx != IPV4_INV_SOCK_IDX)
        {
          TcpIp_SetIpV4CtrlIdxOfIpV4SocketDyn(IpV4SocketIdx, (TcpIp_SizeOfIpV4CtrlType)IpV4CtrlIdx);
          TcpIp_SetArpTableEntryIdxOfIpV4SocketDyn(IpV4SocketIdx, (TcpIp_SizeOfArpTableEntryType)arpTableEntryIdx);
        }
        break;
      }
      /* #50 If the entry is found as valid and too old, store it as a last recently used entry. */
      /* ARP entries with infinit Time is set to TRUE, shall not overwritten */
      if (   (IPV4_ARP_ELSTATUS_INVALID != TcpIp_GetStatusOfArpTableEntry(*OldestArpTableEntryIdxPtr))                  /* PRQA S 3415 3 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
          && (TcpIp_IsInfinitTimeOfArpTableEntry(arpTableEntryIdx) == FALSE)                                            /* PRQA S 3415 3 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
          && IPV4_ARP_TIMESTAMP_IS_OLDER(TcpIp_GetTimeStampOfArpTableEntry(arpTableEntryIdx),
                                         TcpIp_GetTimeStampOfArpTableEntry(*OldestArpTableEntryIdxPtr)))
      {
        /* Store index of least recently used entry, if we need space for a new element later */
        *OldestArpTableEntryIdxPtr = (TcpIp_SizeOfArpTableEntryType)arpTableEntryIdx;
        /* Flag to denote if the table is full or not.*/
        /* If discarded handling is on, table will be kept unmodified and new entry will be discarded */
# if (TCPIP_SUPPORT_ARP_DISCARDED_ENTRY_HANDLING == STD_ON)
        *DiscardNewEntryPtr = TRUE;
# else
        *DiscardNewEntryPtr = FALSE;
# endif
      }
    }
    else
    {
      /* #60 ARP entry does not exist. Store last invalid (unused) entry as oldest entry. */
      *OldestArpTableEntryIdxPtr = (TcpIp_SizeOfArpTableEntryType)arpTableEntryIdx;
      *DiscardNewEntryPtr = FALSE;
    }
  }

  /* Return TCPIP_NO_ARPTABLEENTRYIDXOFIPV4SOCKETDYN if no matching entry was found. */
  if (arpTableEntryIdx >= TcpIp_GetArpTableEntryEndIdxOfIpV4Ctrl(IpV4CtrlIdx))
  {
    arpTableEntryIdx = TCPIP_NO_ARPTABLEENTRYIDXOFIPV4SOCKETDYN;
  }

  /*@ assert *OldestArpTableEntryIdxPtr < TcpIp_GetSizeOfArpTableEntry(); */                                            /* VCA_TCPIP_LRU_ALWAYS_IN_VALID_RANGE */
  return (TcpIp_SizeOfArpTableEntryType)arpTableEntryIdx;
} /* IpV4_Arp_VCheckArpEntry() */

/**********************************************************************************************************************
 *  IpV4_Arp_VProcessPendingEntries
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
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) IpV4_Arp_VProcessPendingEntries(TcpIp_IpV4CtrlIterType IpV4CtrlIdx)
{
  TcpIp_ArpTableEntryIterType arpTableEntryIdx;
  TcpIp_SizeOfArpConfigType   arpConfigIdx = TcpIp_GetArpConfigIdxOfIpV4Ctrl(IpV4CtrlIdx);

  /* #10 Iterate over the ARP table. */
  for (arpTableEntryIdx = TcpIp_GetArpTableEntryStartIdxOfIpV4Ctrl(IpV4CtrlIdx);
       arpTableEntryIdx < TcpIp_GetArpTableEntryEndIdxOfIpV4Ctrl(IpV4CtrlIdx);
       arpTableEntryIdx++)
  {
    /*@ assert arpTableEntryIdx < TcpIp_GetSizeOfArpTableEntry(); */                                                    /* VCA_TCPIP_CSL03_START_END */
    TCPIP_ASSERT(arpConfigIdx < TcpIp_GetSizeOfArpConfig());

    /* #20 Check the status of each entry. */
    if (   (TcpIp_GetStatusOfArpTableEntry(arpTableEntryIdx) == IPV4_ARP_ELSTATUS_INPROGESS)                            /* PRQA S 3415 2 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
        || (TcpIp_GetStatusOfArpTableEntry(arpTableEntryIdx) == IPV4_ARP_ELSTATUS_PROBE) )
    {
      /* #30 Remove entry from ARP table if retry time has exceeded. */
      if (  (IpV4_Arp_MainFuncCycle - TcpIp_GetCreationTimeStampOfArpTableEntry(arpTableEntryIdx))
          >= TcpIp_GetRetryTimeOfArpConfig(arpConfigIdx))
      {
        TCPIP_VENTER_CRITICAL_SECTION();
        /* #40 Invalidate the ARP entry and decrement the global variable for the pending entry. */
        /* timeout expired, invalidate the table entry */
        IpV4_Arp_VPhysAddrTableChgNotification(IpV4CtrlIdx, (TcpIp_SizeOfArpTableEntryType)arpTableEntryIdx, FALSE);
        TcpIp_SetStatusOfArpTableEntry(arpTableEntryIdx, IPV4_ARP_ELSTATUS_INVALID);
        TcpIp_DecNumPendArpElemOfIpV4CtrlDyn(IpV4CtrlIdx);
        {
          TcpIp_SockAddrBaseType DestSockAddrIn;                                                                        /* PRQA S 0759 */ /* MD_MSR_Union */
          DestSockAddrIn.SockAddrIn.sin_family = IPBASE_AF_INET;
          DestSockAddrIn.SockAddrIn.sin_addr   = TcpIp_GetNetAddrOfArpTableEntry(arpTableEntryIdx);
          /* #50 Notifies the TcpIp that the link layer address resolution for the specified destination timed out. */
          /* call timeout function */
          TcpIp_Cbk_VAddrResTimeout(&DestSockAddrIn);
# if (TCPIP_SUPPORT_ICMPV4 == STD_ON)                                                                                   /* COV_TCPIP_ICMPV4_ALWAYS_ON */
          /* #60 Call the upper layer to cancel ICMP echo reply by reseting the Echo reply pending flag in case the time out occurred. */
          IpV4_Icmp_Cbk_VAddrResTimeout(IpV4CtrlIdx, DestSockAddrIn.SockAddrIn.sin_addr);
# endif
        }
        TCPIP_VLEAVE_CRITICAL_SECTION();
        /* #70 if no pending entry exists, break the for loop. Don't do anything. */
        if (TcpIp_GetNumPendArpElemOfIpV4CtrlDyn(IpV4CtrlIdx) == 0u)
        {
          break;
        }

      }
      /* #80 Send ARP Request if time interval between ARP requests has been reached, otherwise wait for Arp response or timeout. */
      else if (   (TcpIp_GetRetryIntervalOfArpConfig(arpConfigIdx) !=0u)                                                /* PRQA S 3415 3 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
               && (( (IpV4_Arp_MainFuncCycle - TcpIp_GetTimeStampOfArpTableEntry(arpTableEntryIdx))
                    % TcpIp_GetRetryIntervalOfArpConfig(arpConfigIdx)) == 0u)
                  /* Check to ensure that ARP entry is at least one retry interval old */
               && (IpV4_Arp_MainFuncCycle != TcpIp_GetTimeStampOfArpTableEntry(arpTableEntryIdx)))                      /* PRQA S 3415 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
      {
        /* trigger retry */
        (void)IpV4_Arp_SendArpRequest(IpV4CtrlIdx, TcpIp_GetNetAddrOfArpTableEntry(arpTableEntryIdx));
        TcpIp_SetTimeStampOfArpTableEntry(arpTableEntryIdx, IpV4_Arp_MainFuncCycle);
      }
      else
      {
         /* Wait for ARP response or next retry timeout. */
      }
    }
    else if (TcpIp_IsInfinitTimeOfArpTableEntry(arpTableEntryIdx) == TRUE)
    {
      /* This ARP entry is set via API and shall stay valid till it is removed via API again. */
      /* Update the timestamp so that it stay marked as active. */
      TcpIp_SetCreationTimeStampOfArpTableEntry(arpTableEntryIdx, IpV4_Arp_MainFuncCycle);
    }
    else
    {
      /* Nothing to do */
    }
  }
} /* IpV4_Arp_VProcessPendingEntries() */

/**********************************************************************************************************************
 *  IpV4_Arp_GetPhysicalAddressFromDynamicTables
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
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(TcpIp_ReturnType, TCPIP_CODE) IpV4_Arp_GetPhysicalAddressFromDynamicTables(
  TcpIp_IpV4CtrlIterType                   IpV4CtrlIdx,
  TcpIp_SizeOfIpV4SocketDynType            IpV4SocketIdx,
  IpBase_AddrInType                        NetAddr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) PhysAddrPtr,
  boolean                                  InitRes)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_ReturnType retValue = TCPIP_E_PHYS_ADDR_MISS;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the Dynamic table is configured for the given controller. */
  if (TcpIp_IsArpConfigUsedOfIpV4Ctrl(IpV4CtrlIdx) == TRUE)
  {
# if defined(TCPIP_ASSERT_HANDLER)                                                                                      /* COV_TCPIP_CANOE_DEBUG */
    TcpIp_SizeOfArpConfigType arpConfigIdx = TcpIp_GetArpConfigIdxOfIpV4Ctrl(IpV4CtrlIdx);;
# endif
    boolean                          elemInTable = FALSE;
    TcpIp_SizeOfArpTableEntryType    arpTableEntryIdx = 0;
    TcpIp_SizeOfArpTableEntryType oldestArpTableEntryIdx = 0;  /* Oldest ARP entry that can be replaced if ARP table is full */
    boolean discardNewEntry = TRUE; /* flag to indicate that oldestArpTableEntryIdx contains an entry that will be overwritten or new incoming entry will be discarded (i.e. not an entry that's still unused) */

    TCPIP_ASSERT(arpConfigIdx < TcpIp_GetSizeOfArpConfig());
    TCPIP_ASSERT(TcpIp_GetTableSizeOfArpConfig(arpConfigIdx) > 0u);
     /* Do not check the whole table but only the element for this socket
      - check if IP address is still the same
      - read physical address */
    /* #20 Check if there is an entry in socket mapping table mapped to this socket. */
    if (IpV4SocketIdx != IPV4_INV_SOCK_IDX)
    {
      if (TcpIp_GetIpV4CtrlIdxOfIpV4SocketDyn(IpV4SocketIdx) != IpV4CtrlIdx)
      {
        arpTableEntryIdx = TCPIP_NO_ARPTABLEENTRYIDXOFIPV4SOCKETDYN;
      }
      else
      {
        arpTableEntryIdx = TcpIp_GetArpTableEntryIdxOfIpV4SocketDyn(IpV4SocketIdx);
      }
    }
    else
    {
      arpTableEntryIdx = TCPIP_NO_ARPTABLEENTRYIDXOFIPV4SOCKETDYN;
    }

    if (   (arpTableEntryIdx != TCPIP_NO_ARPTABLEENTRYIDXOFIPV4SOCKETDYN )                                              /* PRQA S 3415 2 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
        && (TcpIp_GetStatusOfArpTableEntry(arpTableEntryIdx) != IPV4_ARP_ELSTATUS_INVALID)
        && (TcpIp_GetNetAddrOfArpTableEntry(arpTableEntryIdx) == NetAddr))
    {
      /* there is an entry for this network address ("valid" or "in progress") */
      elemInTable = TRUE;
    }
    else
    {
      TCPIP_VENTER_CRITICAL_SECTION();  /* lock interrupts */
      /* there is no valid entry in mapping table */
      /* #30 Otherwise check if ARP entry already exists (without entry in mapping table). */
      arpTableEntryIdx = IpV4_Arp_VCheckArpEntry(IpV4CtrlIdx, IpV4SocketIdx, NetAddr, &discardNewEntry, &oldestArpTableEntryIdx);

      elemInTable = (boolean)(arpTableEntryIdx != TCPIP_NO_ARPTABLEENTRYIDXOFIPV4SOCKETDYN);                            /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
      /* Below check makes sure that arpTableEntryIdx would not get overwritten due to RxIndication Interrupt. */
      /* Set the oldest entry to In_Progress, if the entry is not present in table */
      if ((elemInTable == FALSE) && (InitRes == TRUE) && (discardNewEntry == FALSE))
      {
# if (TCPIP_SUPPORT_ARP_DISCARDED_ENTRY_HANDLING == STD_OFF)
        /* Inform the upper layer about the creation of the new entry, but set the valid flag to FALSE because the address has not yet been resolved
          (callout will be called again with valid = TRUE in the Rx - indication, later on)
           Calling the callout immediately after creation of the new entry(and any change in ARP table) is an AUTOSAR requirement */
        /* If no entry is present, LRU shall be deleted (overwritten) and new arp entry will be created later. */
        /* #40 Inform upper layer about deletion of LRU entry if the LRU is previously valid. */
        if (TcpIp_GetStatusOfArpTableEntry(oldestArpTableEntryIdx) != IPV4_ARP_ELSTATUS_INVALID)
        {
          IpV4_Arp_VPhysAddrTableChgNotification(IpV4CtrlIdx, oldestArpTableEntryIdx, FALSE);
          /*@ assert oldestArpTableEntryIdx < TcpIp_GetSizeOfArpTableEntry(); */                                        /* VCA_TCPIP_CONFIG_PTR_RESTORE */ /* TAR-18097 */
          IpV4_Arp_VPhysAddrTableChgDiscardedNotification(IpV4CtrlIdx, /* OverWritten */ TcpIp_GetNetAddrOfArpTableEntry(oldestArpTableEntryIdx));
        }
# endif
        /* Set the LRU state to INPROGRESS to ensure that it is not overwritten in a different context */
        TcpIp_SetStatusOfArpTableEntry(oldestArpTableEntryIdx, IPV4_ARP_ELSTATUS_INPROGESS);
        /* Overwrite the LRU ipaddr at this point of time, to avoid occurences of two same ARP entries in the ARP table. */
        TcpIp_SetNetAddrOfArpTableEntry(oldestArpTableEntryIdx, NetAddr);
      }
      TCPIP_VLEAVE_CRITICAL_SECTION();  /* unlock interrupts */
    }

    if (elemInTable == TRUE)
    {
      /*@ assert arpTableEntryIdx < TcpIp_GetSizeOfArpTableEntry(); */                                                  /* VCA_TCPIP_ARP_TABLE_ENTRY_IN_VALID_RANGE */

      /* #50 Check if Entry is too old. */
      TCPIP_ASSERT(arpTableEntryIdx >= TcpIp_GetArpTableEntryStartIdxOfIpV4Ctrl(IpV4CtrlIdx));
      TCPIP_ASSERT(arpTableEntryIdx <  TcpIp_GetArpTableEntryEndIdxOfIpV4Ctrl(IpV4CtrlIdx));

      retValue = IpV4_Arp_VCheckValidityOfEntry(IpV4CtrlIdx, PhysAddrPtr, NetAddr, arpTableEntryIdx);
    }
    else
    {
      /* #60 No matching element in table. Create a new entry and trigger ARP request in case the caller desires that. */
      if (InitRes == TRUE)
      {
        TCPIP_ASSERT(oldestArpTableEntryIdx >= TcpIp_GetArpTableEntryStartIdxOfIpV4Ctrl(IpV4CtrlIdx));
        TCPIP_ASSERT(oldestArpTableEntryIdx <  TcpIp_GetArpTableEntryEndIdxOfIpV4Ctrl(IpV4CtrlIdx));
        retValue = IpV4_Arp_VCreateEntry(IpV4CtrlIdx, IpV4SocketIdx, NetAddr, discardNewEntry, oldestArpTableEntryIdx);
      }
    }
  }

  return retValue;
}  /* IpV4_Arp_GetPhysicalAddressFromDynamicTables() */

/**********************************************************************************************************************
 *  IpV4_Arp_GetPhysicalAddress
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
FUNC(TcpIp_ReturnType, TCPIP_CODE) IpV4_Arp_GetPhysicalAddress(
  TcpIp_IpV4CtrlIterType                   IpV4CtrlIdx,
  TcpIp_SizeOfIpV4SocketDynType            SocketIdx,
  IpBase_AddrInType                        NetAddr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) PhysAddrPtr,
  boolean                                  InitRes )
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_ReturnType retVal = TCPIP_E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(IpV4CtrlIdx < TcpIp_GetSizeOfIpV4Ctrl());
  TCPIP_ASSERT(PhysAddrPtr != NULL_PTR);

  /* #10 Return calculated group MAC address, if IP address is multicast address. */
  if (IPV4_ADDR_IS_MULTICAST(NetAddr))
  {
    IpV4_Arp_MapIpToPhysMulticastAddr(NetAddr, PhysAddrPtr);
    retVal = TCPIP_OK;
  }
  else
  {
    IpBase_AddrInType invertedNetMask = ~TcpIp_GetActiveNetMaskOfIpV4CtrlDyn(IpV4CtrlIdx);
    /* #20 Return broadcast MAC address, if IP address is global or local broadcast. */
    if ((NetAddr & invertedNetMask) == invertedNetMask)
    {
      TcpIp_VCopyPhysAddr(PhysAddrPtr, IpV4_Arp_PhysBroadcastAddr);
      retVal = TCPIP_OK;
    }
    /* #30 Verify if the IP address is valid. */
    else if ((invertedNetMask & NetAddr) == 0u)
    {
      retVal = TCPIP_E_NOT_OK;
    }
    /* #40 Check if static mapping is configured, if static ARP table is configured. */
    else if (IpV4_Arp_VStaticTableLookup(IpV4CtrlIdx, NetAddr, PhysAddrPtr) == E_OK)
    {
      retVal = TCPIP_OK;
    }
    /* #50 Search for cached mapping in dynamic ARP table or trigger address resolution if required. */
    else
    {
      retVal = IpV4_Arp_GetPhysicalAddressFromDynamicTables(IpV4CtrlIdx, SocketIdx, NetAddr, PhysAddrPtr, InitRes);
    }
  }
  return retVal;
} /* IpV4_Arp_GetPhysicalAddress() */

/**********************************************************************************************************************
 *  IpV4_Arp_RxIndication
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
/* PRQA S 6050, 6060, 6080 1 */ /* MD_TcpIp_STCAL_CslAccess, MD_MSR_STPAR, MD_MSR_STMIF */
FUNC(void, TCPIP_CODE) IpV4_Arp_RxIndication(
  TcpIp_SizeOfEthIfCtrlType                EthIfCtrlIdx,
  Eth_FrameType                            FrameType,
  boolean                                  IsBroadcast,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) PhysAddrPtr,                                                                 /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,                                                                     /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
  uint16                                   LenByte )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = IPV4_E_NO_ERROR;

  TCPIP_DUMMY_STATEMENT_CONST(FrameType);                                                                               /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
  TCPIP_DUMMY_STATEMENT_CONST(IsBroadcast);                                                                             /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
  TCPIP_DUMMY_STATEMENT_CONST(PhysAddrPtr);                                                                             /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */

  /* ----- Development Error Checks ------------------------------------- */
# if (TCPIP_DEV_ERROR_DETECT == STD_ON)                                                                                 /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Check component initialization state and input parameters. */
  if (IpV4_State == IPV4_STATE_UNINIT)
  {
    errorId = IPV4_E_NOT_INITIALIZED;
  }
  /* Check parameter 'EthIfCtrlIdx' */
  else if (EthIfCtrlIdx >= TcpIp_GetSizeOfEthIfCtrl())
  {
    errorId = IPV4_E_INV_PARAM;
  }
  /* Check parameter 'FrameType' */
  else if (FrameType != IPV4_ARP_FRAME_TYPE)
  {
    errorId = IPV4_E_INV_PARAM;
  }
   /* Check parameter 'PhysAddrPtr' */
  else if (PhysAddrPtr == NULL_PTR)
  {
    errorId = IPV4_E_INV_POINTER;
  }
  /* Check parameter 'DataPtr' */
  else if (DataPtr == NULL_PTR)
  {
    errorId = IPV4_E_INV_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Forward call to Subfunction. */
    /* Read the received message data.*/
    IpV4_Arp_VReadMsgData(EthIfCtrlIdx, DataPtr, LenByte);
  }

  /* ----- Development Error Report --------------------------------------- */
# if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != IPV4_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, IPV4_VINSTANCE_ID, IPV4_ARP_API_ID_RX_INDICATION, errorId);
  }
# else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif

} /* IpV4_Arp_RxIndication() */

/**********************************************************************************************************************
 *  IpV4_Arp_VReadMsgData
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) IpV4_Arp_VReadMsgData(
  TcpIp_SizeOfEthIfCtrlType                  EthIfCtrlIdx,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint16                                     LenByte)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if IP instance is configured for a given Controller. */
  if ((TcpIp_IsIpV4CtrlUsedOfEthIfCtrl(EthIfCtrlIdx) == TRUE) && (LenByte >= IPV4_ARP_PACKET_LEN_BYTE))                 /* PRQA S 3415 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
    {
      TcpIp_IpV4CtrlIterType ipV4CtrlIdx = TcpIp_GetIpV4CtrlIdxOfEthIfCtrl(EthIfCtrlIdx);
      /*@ assert ipV4CtrlIdx < TcpIp_GetSizeOfIpV4Ctrl(); */                                                            /* VCA_TCPIP_CSL03_IS_USEDOF */

      if ((TcpIp_GetStateOfIpV4CtrlDyn(ipV4CtrlIdx) != IPV4_IP_OFFLINE))
      {
        uint16 hwAddrType   = TCPIP_GET_UINT16(DataPtr, IPV4_ARP_HW_ADDR_TYPE_OFS);
        uint16 protAddrType = TCPIP_GET_UINT16(DataPtr, IPV4_ARP_PR_ADDR_TYPE_OFS);
        uint8  hwAddrSize   = TCPIP_GET_UINT8(DataPtr, IPV4_ARP_HW_ADDR_SIZE_OFS);
        uint8  protAddrSize = TCPIP_GET_UINT8(DataPtr, IPV4_ARP_PR_ADDR_SIZE_OFS);
        uint16 operation    = TCPIP_GET_UINT16(DataPtr, IPV4_ARP_OP_OFS);

        /* #20 Check ARP hardware and protocol address formats. */
        if (   (hwAddrType   == IPV4_ARP_HW_ADDR_TYPE_ETHERNET)
            && (hwAddrSize   == IPV4_ARP_HW_ADDR_SIZE_ETHERNET)
            && (protAddrType == IPV4_ARP_PROT_ADDR_TYPE_IPV4)
            && (protAddrSize == IPV4_ARP_PROT_ADDR_SIZE_IPV4))
        {
          IpBase_EthPhysAddrType srcPhysAddr = { 0 };
          IpBase_AddrInType srcIpAddr = TCPIP_GET_UINT32(DataPtr, IPV4_ARP_SRC_IP_ADDR_OFS);
          IpBase_AddrInType dstIpAddr = TCPIP_GET_UINT32(DataPtr, IPV4_ARP_DST_IP_ADDR_OFS);

          srcIpAddr = TCPIP_NTOHL(srcIpAddr);
          dstIpAddr = TCPIP_NTOHL(dstIpAddr);

          srcPhysAddr[0] = TCPIP_GET_UINT8(DataPtr, IPV4_ARP_SRC_MC_ADDR_OFS);
          srcPhysAddr[1] = TCPIP_GET_UINT8(DataPtr, IPV4_ARP_SRC_MC_ADDR_OFS + 1u);
          srcPhysAddr[2] = TCPIP_GET_UINT8(DataPtr, IPV4_ARP_SRC_MC_ADDR_OFS + 2u);
          srcPhysAddr[3] = TCPIP_GET_UINT8(DataPtr, IPV4_ARP_SRC_MC_ADDR_OFS + 3u);
          srcPhysAddr[4] = TCPIP_GET_UINT8(DataPtr, IPV4_ARP_SRC_MC_ADDR_OFS + 4u);
          srcPhysAddr[5] = TCPIP_GET_UINT8(DataPtr, IPV4_ARP_SRC_MC_ADDR_OFS + 5u);

          /* #30 Handle received ARP packet. */
          IpV4_Arp_VHandlePacket(ipV4CtrlIdx, operation, srcIpAddr, dstIpAddr, srcPhysAddr);
        }
      }
    } /* else: Ip instance is not configured and length is invalid */
}

/**********************************************************************************************************************
 *  IpV4_Arp_SendArpRequest
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) IpV4_Arp_SendArpRequest(
  TcpIp_IpV4CtrlIterType IpV4CtrlIdx,
  IpBase_AddrInType      NetAddr )
{
  /* ----- Local Variables ---------------------------------------------- */
  IpBase_AddrInType       locNetAddr = 0; /* Source IP address */
  Std_ReturnType          retVal = E_NOT_OK;
  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(IpV4CtrlIdx < TcpIp_GetSizeOfIpV4Ctrl());

  /* #10 Prepare ARP request. */
  locNetAddr = TcpIp_GetActiveNetAddrOfIpV4CtrlDyn(IpV4CtrlIdx);

  /* #20 Send an ARP request. */
  if (TcpIp_GetStateOfIpV4CtrlDyn(IpV4CtrlIdx) != IPV4_IP_ONHOLD)
  {
    retVal = IpV4_Arp_VSendMessage(IpV4CtrlIdx, NetAddr, locNetAddr, &IpV4_Arp_PhysBroadcastAddr[0], IPV4_ARP_OP_REQUEST);
  }

  return retVal;
} /* IpV4_Arp_SendArpRequest() */

/**********************************************************************************************************************
 *  IpV4_Arp_SendArpProbe
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) IpV4_Arp_SendArpProbe(
  TcpIp_IpV4CtrlIterType IpV4CtrlIdx,
  IpBase_AddrInType      IpAddrToProbe)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* "The 'target IP address' field MUST be set to the address being probed." */
  IpBase_AddrInType       remNetAddr  = IpAddrToProbe;
  /* "... with an all-zero 'sender IP address'." */
  IpBase_AddrInType       locNetAddr  = 0u;
  /* "ARP Request packet, broadcast on the local link" */
  uint16                  operation   = IPV4_ARP_OP_REQUEST;
  /* "The 'target hardware address' field is ignored and SHOULD be set to all zeros. */
  CONST(IpBase_EthPhysAddrType, TCPIP_CONST) remPhysAddr = { 0u };

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(IpV4CtrlIdx < TcpIp_GetSizeOfIpV4Ctrl());

  /* #10 Send ARP request. */
  return IpV4_Arp_VSendMessage(IpV4CtrlIdx, remNetAddr, locNetAddr, &remPhysAddr[0], operation);
} /* IpV4_Arp_SendArpProbe() */

/**********************************************************************************************************************
 *  IpV4_Arp_SendArpAnnouncement
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) IpV4_Arp_SendArpAnnouncement(
  TcpIp_IpV4CtrlIterType IpV4CtrlIdx,
  IpBase_AddrInType      IpAddrToAnnounce)
{
  /* ----- Local Variables ---------------------------------------------- */
  IpBase_EthPhysAddrType  remPhysAddr;

  Std_ReturnType          retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(IpV4CtrlIdx < TcpIp_GetSizeOfIpV4Ctrl());

  /* "'ARP Announcement' is [...] an ARP Request packet,
   *  broadcast on the local link, identical to the ARP Probe [...]
   *  except that both the sender and target IP address fields contain the IP address being announced."
   */
  /* #10 Check if controller is not in state OnHold */
  if (TcpIp_GetStateOfIpV4CtrlDyn(IpV4CtrlIdx) != IPV4_IP_ONHOLD)
  {
    uint8_least physAddrByteIdx;
    /* #20 Prepare ARP request. */
    IpBase_AddrInType locNetAddr = IpAddrToAnnounce;
    IpBase_AddrInType remNetAddr = IpAddrToAnnounce;    /* "The 'target IP address' field MUST be set to the address being probed." */
    uint16 operation = IPV4_ARP_OP_REQUEST;  /* "ARP Request packet, broadcast on the local link" */

    /* #30 Set all bytes of a target hardware address to zero.*/
    /* "The 'target hardware address' field is ignored and SHOULD be set to all zeros. */
    for (physAddrByteIdx = 0; physAddrByteIdx < IPBASE_ETH_PHYS_ADDR_LEN_BYTE; physAddrByteIdx++)
    {
      remPhysAddr[physAddrByteIdx] = 0;
    }

    /* #40 Send ARP request. */
    retVal = IpV4_Arp_VSendMessage(IpV4CtrlIdx, remNetAddr, locNetAddr, &remPhysAddr[0], operation);
  }
  return retVal;
} /* IpV4_Arp_SendArpAnnouncement() */

/**********************************************************************************************************************
 *  IpV4_Arp_SendGratuitousArpReq
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) IpV4_Arp_SendGratuitousArpReq(TcpIp_LocalAddrV4IterType LocalAddrV4Idx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType          retVal = E_NOT_OK;
  TcpIp_SizeOfIpV4CtrlType IpV4CtrlIdx = TcpIp_GetIpV4CtrlIdxOfLocalAddrV4(LocalAddrV4Idx);

  /* ----- Implementation ----------------------------------------------- */

  TCPIP_ASSERT(LocalAddrV4Idx < TcpIp_GetSizeOfLocalAddrV4());

  /* #10 Verify that IP instance is in state ONLINE. */
  if(TcpIp_GetStateOfIpV4CtrlDyn(IpV4CtrlIdx) == IPV4_IP_ONLINE_ACTIVE)
  {
    /* #20 Fill the C-struct parameter that is given to IpV4_Arp_VSendMessage() in order to send a broadcast ARP request to publish the LocIpAddr. */
    /* This is the format of a gratuitous ARP request message(RFC 3344 4.6.ARP, Proxy ARP, and Gratuitous ARP) :
     *  - type : Request
     *  - Sender IP == Target IP == IP ECU
     *  - Sender MAC == MAC ECU
     *  - Target MAC == field not used
     *
     * "[...] for a gratuitous ARP, the ARP packet MUST be
     *  transmitted as a local broadcast packet on the local link"
     * Thus, the ethernet header should have the broadcast MAC as destination MAC address.
     *
     * Since the target MAC address field is not used, it is simply set to the broadcast MAC.
     * Another way would be to set the MAC address to zeros, but the broadcast MACs should have the highest
     * compatibility effect with other implementations.
     */

    IpBase_AddrInType       locIpAddr  = TcpIp_GetActiveNetAddrOfIpV4CtrlDyn(IpV4CtrlIdx);
    IpBase_AddrInType       locNetAddr = locIpAddr;           /* "In either case, the ARP Sender Protocol Address [...] are both set to the IP address of the cache entry to be updated" */
    IpBase_AddrInType       remNetAddr = locIpAddr;           /* "In either case, the [...] ARP Target Protocol Address are both set to the IP address of the cache entry to be updated" */
    uint16                  operation  = IPV4_ARP_OP_REQUEST; /* "ARP Request packet, broadcast on the local link" */

    /* #30 Pass the request to send the ARP request to the internal helper function that transmits ARP packages. */
    /* The broadcast mac is passed as remote physical address. This value is used for both the ARP Target MAC field
     * as well as for the ethernet header as destination address. */
    retVal = IpV4_Arp_VSendMessage(IpV4CtrlIdx, remNetAddr, locNetAddr, &IpV4_Arp_PhysBroadcastAddr[0], operation);
  }
  return retVal;
} /* IpV4_Arp_SendGratuitousArpReq() */

/**********************************************************************************************************************
 *  IpV4_Arp_SendGratuitousArpReply
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) IpV4_Arp_SendGratuitousArpReply(TcpIp_IpV4CtrlIterType IpV4CtrlIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType          retVal = E_NOT_OK;
  IpBase_EthPhysAddrType  srcMacAddr;
  IpBase_AddrInType       locIpAddr = TcpIp_GetActiveNetAddrOfIpV4CtrlDyn(IpV4CtrlIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get source MAC address. */
  TCPIP_LL_GetPhysAddr(TcpIp_GetEthIfCtrlIdxOfIpV4Ctrl(IpV4CtrlIdx), srcMacAddr);

  /* #20 Pass the request to send the ARP request to the internal helper function that transmits ARP packages if Ctrl is not in state Onhold. */
  if (TcpIp_GetStateOfIpV4CtrlDyn(IpV4CtrlIdx) != IPV4_IP_ONHOLD)
  {
    /* #30 Fill the C-struct parameter that is given to IpV4_Arp_VSendMessage() in order to send gratuitous ARP reply to publish the LocIpAddr. */
    /* This is the format of a gratuitous ARP reply message(RFC 3344 4.6.ARP, Proxy ARP, and Gratuitous ARP) :
     *  - type : Reply
     *  - Sender MAC == Target MAC == MAC ECU
     *  - Sender IP == Target IP == IP ECU
     *
     * "[...] for a gratuitous ARP, the ARP packet MUST be
     *  transmitted as a local broadcast packet on the local link"
     * Thus, the ethernet header should have the broadcast MAC as destination MAC address.
     */

    /* "In either case, the ARP Sender Protocol Address [...] are both
     *  set to the IP address of the cache entry to be updated" */
    IpBase_AddrInType locNetAddr = locIpAddr;
    /* "In either case, the [...] ARP Target Protocol Address are both
     *  set to the IP address of the cache entry to be updated" */
    IpBase_AddrInType remNetAddr = locIpAddr;
    /* "ARP Reply packet, broadcast on the local link" */
    uint16 operation = IPV4_ARP_OP_REPLY;

    /* IpV4_Arp_VSendMessage currently has no support to enter a destination mac address for the ethernet
     * header. The requirement that an gratuitous ARP message shall be send a broadcast on the local link
     * must therefore be checked and fulfilled by IpV4_Arp_VSendMessage. So only the srcMacAddr is handed
     * over as destination mac address for the ARP message (because Sender MAC == Target MAC == MAC ECU). */
    retVal = IpV4_Arp_VSendMessage(IpV4CtrlIdx, remNetAddr, locNetAddr, srcMacAddr, operation);
  }

  return retVal;
} /* IpV4_Arp_SendGratuitousArpReply() */

/**********************************************************************************************************************
 *  IpV4_Arp_SendGratuitousArpOnStartup
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6050, 6080 1 */ /* MD_TcpIp_STCAL_CslAccess, MD_MSR_STMIF */
FUNC(void, TCPIP_CODE) IpV4_Arp_SendGratuitousArpOnStartup(TcpIp_IpV4CtrlIterType IpV4CtrlIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_ArpConfigIterType arpConfigIdx = TcpIp_GetArpConfigIdxOfIpV4Ctrl(IpV4CtrlIdx);

  /* ----- Implementation ----------------------------------------------- */
  TcpIp_GratuitousArpIterType gratuitousArpIdx = TcpIp_GetGratuitousArpIdxOfIpV4Ctrl(IpV4CtrlIdx);
  /*@ assert gratuitousArpIdx < TcpIp_GetSizeOfGratuitousArp(); */                                                      /* VCA_TCPIP_GRATUITOUS_ARP_ALWAYS_IN_VALID_RANGE */
  uint8 numOfTxGratuitousReply = TcpIp_GetTransmissionCntOfGratuitousArp(gratuitousArpIdx);

  /* #10 Set the time stamp to send the first Arp gratuitous reply. */
  if (numOfTxGratuitousReply == 0u)
  {
    TcpIp_SetNextTransmissionMsOfGratuitousArp(gratuitousArpIdx, IpV4_Arp_MainFuncCycle);
  }

  /* #20 Check if configured number of Gratuitous arp reply has been sent.*/
  if (numOfTxGratuitousReply < TcpIp_GetNumGratuitousArpOnStartupOfArpConfig(arpConfigIdx))
  {
    /* #30 Verify the time interval between two gratuitous arp replies. */
    if (IpV4_Arp_MainFuncCycle >= TcpIp_GetNextTransmissionMsOfGratuitousArp(gratuitousArpIdx))
    {
      Std_ReturnType retVal = IpV4_Arp_SendGratuitousArpReply(IpV4CtrlIdx);
      /* #40 Increment the count of number of transmitted gratuitous ARP replies and set the time for the next
              transmission, if ARP gratuitous reply has been successfully sent. */
      if (retVal == E_OK)
      {
        TcpIp_SetNextTransmissionMsOfGratuitousArp(gratuitousArpIdx,
                                                    (IpV4_Arp_MainFuncCycle
                                                    + IPV4_ARP_GRATUITOUS_REPLY_INTERVAL));
        TcpIp_IncTransmissionCntOfGratuitousArp(gratuitousArpIdx);
      }
    }
  }
} /* IpV4_Arp_SendGratuitousArpOnStartup() */

/**********************************************************************************************************************
 *  IpV4_Arp_MainFunction
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
FUNC(void, TCPIP_CODE) IpV4_Arp_MainFunction(TcpIp_OsApplicationType ApplId)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_IpV4CtrlIterType ipV4CtrlIdx;

  /* ----- Implementation ----------------------------------------------- */
# if (TCPIP_MULTICORE_SUPPORT_ENABLED == STD_OFF)                                                                       /* COV_TCPIP_MULTICORE_SUPPORT_ALWAYS_OFF */
  TCPIP_DUMMY_STATEMENT_CONST(ApplId);                                                                                  /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
# endif

  /* #10 Increment MainFunction cycle counter. */
# if (TCPIP_MULTICORE_SUPPORT_ENABLED == STD_ON)                                                                        /* COV_TCPIP_MULTICORE_SUPPORT_NEVER_ON */
  /* Only update the Arp time in the main application */
  if (ApplId == TcpIp_Arp_MainApplicationId)
# endif
  {
    IpV4_Arp_MainFuncCycle++;
  }

  /* #20 Iterate over all configured IPv4 controllers. */
  for(ipV4CtrlIdx = 0; ipV4CtrlIdx < TcpIp_GetSizeOfIpV4Ctrl(); ipV4CtrlIdx++)
  {
# if (TCPIP_MULTICORE_SUPPORT_ENABLED == STD_ON)                                                                        /* COV_TCPIP_MULTICORE_SUPPORT_NEVER_ON */
    /* Only process the controller assigned to this application */
    if (ApplId == TcpIp_GetApplIdOfIpV4Ctrl(ipV4CtrlIdx))
# endif
    {
      /* #30 Send Gratuitous ARP reply if Ip address is assigned through DHCP or STATIC address assignment method. */
      /* PRQA S 3415 2 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
      if ((TcpIp_GetActiveAddrAssignmentMethodOfIpV4CtrlDyn(ipV4CtrlIdx) == TCPIP_IPADDR_ASSIGNMENT_DHCP)
        || (TcpIp_GetActiveAddrAssignmentMethodOfIpV4CtrlDyn(ipV4CtrlIdx) == TCPIP_IPADDR_ASSIGNMENT_STATIC))
      {
        /* #40 If gratuitous ARP is configured for the controller, transmit */
        if (TcpIp_IsGratuitousArpUsedOfIpV4Ctrl(ipV4CtrlIdx) == TRUE)
        {
          /* #50 Send configured number of Arp Gratuitous reply on start up. */
          IpV4_Arp_SendGratuitousArpOnStartup(ipV4CtrlIdx);
        }
      }

      /* #60 Check if there are pending entries in the ARP table and ARP Requests have to be sent. */
      IpV4_Arp_VProcessPendingEntries(ipV4CtrlIdx);
    }
  }
} /* IpV4_Arp_MainFunction() */

/**********************************************************************************************************************
 *  IpV4_Arp_Reset
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) IpV4_Arp_Reset(TcpIp_IpV4CtrlIterType IpV4CtrlIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SizeOfArpTableEntryType  arpTableEntryIdx;

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(IpV4CtrlIdx < TcpIp_GetSizeOfIpV4Ctrl());

  /* #10 Mark each element in the ARP table as invalid. */
  for (arpTableEntryIdx = TcpIp_GetArpTableEntryStartIdxOfIpV4Ctrl(IpV4CtrlIdx);
       arpTableEntryIdx < TcpIp_GetArpTableEntryEndIdxOfIpV4Ctrl(IpV4CtrlIdx);
       arpTableEntryIdx++)
  {
    /*@ assert arpTableEntryIdx < TcpIp_GetSizeOfArpTableEntry(); */                                                    /* VCA_TCPIP_CSL03_START_END */
    /* for all table elements */
    TcpIp_SetStatusOfArpTableEntry(arpTableEntryIdx, IPV4_ARP_ELSTATUS_INVALID);
    TcpIp_SetNetAddrOfArpTableEntry(arpTableEntryIdx, 0);
    TcpIp_SetInfinitTimeOfArpTableEntry(arpTableEntryIdx, FALSE);
  }

  /* #20 Reset counter of pending ARP resolutions. */
  TcpIp_SetNumPendArpElemOfIpV4CtrlDyn(IpV4CtrlIdx, 0);
}

/**********************************************************************************************************************
 *  IpV4_Arp_GetCacheEntries
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) IpV4_Arp_GetCacheEntries(
  TcpIp_SizeOfEthIfCtrlType         EthIfCtrlIdx,
  IPV4_P2V(uint32)                  NumberOfElementsPtr,
  IPV4_P2V(TcpIp_ArpCacheEntryType) EntryListPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */

  /* #10 Check if IP instance is configured for given EthIfCtrlIdx */
  if (TcpIp_IsIpV4CtrlUsedOfEthIfCtrl(EthIfCtrlIdx) == TRUE)
  {
    TcpIp_SizeOfIpV4CtrlType  IpV4CtrlIdx = TcpIp_GetIpV4CtrlIdxOfEthIfCtrl(EthIfCtrlIdx);
    /*@ assert IpV4CtrlIdx < TcpIp_GetSizeOfIpV4Ctrl(); */                                                              /* VCA_TCPIP_CSL03_IS_USEDOF */

    /* #20 Verify that IP instance is in state ONLINE. */
    if (TcpIp_GetStateOfIpV4CtrlDyn(IpV4CtrlIdx) == IPV4_IP_ONLINE_ACTIVE)
    {
      uint32 totalNumOfDynamicElements = 0u;
      uint32 totalNumOfStaticElements = 0u;
      uint32 globTableIdx = 0; /* Entrylist Index */
      /* Read only the number of valid entries into the ARP table. */
      if (*NumberOfElementsPtr == 0u)
      {
        totalNumOfStaticElements = IpV4_Arp_VReadStaticTable(IpV4CtrlIdx, *NumberOfElementsPtr, NULL_PTR, &globTableIdx);

        totalNumOfDynamicElements = IpV4_Arp_VReadDynTable(IpV4CtrlIdx, *NumberOfElementsPtr, NULL_PTR, &globTableIdx);
      }
      else
      {
        /* #30 Copy static table entries, if configured. */
        totalNumOfStaticElements = IpV4_Arp_VReadStaticTable(IpV4CtrlIdx, *NumberOfElementsPtr, EntryListPtr, &globTableIdx);
        /* IpV4_Arp_VReadStaticTable() does not return a value > *NumberOfElementsPtr. */
        *NumberOfElementsPtr -= totalNumOfStaticElements;
        /* #40 Copy dynamic table entries, if configured. */
        totalNumOfDynamicElements = IpV4_Arp_VReadDynTable(IpV4CtrlIdx, *NumberOfElementsPtr, EntryListPtr, &globTableIdx);
      }

      *NumberOfElementsPtr = totalNumOfDynamicElements + totalNumOfStaticElements;
      retVal = E_OK;
    }
  }

  return retVal;
} /* IpV4_Arp_GetCacheEntries() */

/**********************************************************************************************************************
 *  IpV4_Arp_ClearCache
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) IpV4_Arp_ClearCache(TcpIp_LocalAddrV4IterType LocalAddrV4Idx)
{
  TCPIP_ASSERT(LocalAddrV4Idx < TcpIp_GetSizeOfLocalAddrV4());
  /* #10 Clear dynamic ARP cache. */
  IpV4_Arp_Reset(TcpIp_GetIpV4CtrlIdxOfLocalAddrV4(LocalAddrV4Idx));
}

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  IpV4_Arp_VReadStaticTable
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
TCPIP_LOCAL FUNC(uint32, TCPIP_CODE) IpV4_Arp_VReadStaticTable(
  TcpIp_IpV4CtrlIterType                                          IpV4CtrlIdx,
  uint32                                                          NumOfElements,
  P2VAR(TcpIp_ArpCacheEntryType, TCPIP_APPL_DATA, TCPIP_APPL_VAR) EntryListPtr,
  IPV4_P2V(uint32)                                                EntryListIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 totalNumOfStaticElementsToRead = 0;  /* Number of static entries that are read from the static arp table.*/

  /* ----- Implementation ----------------------------------------------- */
  TcpIp_SizeOfStaticArpTableType staticArpTableIdx = TcpIp_GetStaticArpTableIdxOfIpV4Ctrl(IpV4CtrlIdx);
  /* #10 Check if the StaticArptable is configured. */
  if (staticArpTableIdx != TCPIP_NO_STATICARPTABLEIDXOFIPV4CTRL)
  {
    TcpIp_SizeOfStaticArpEntryType numElementsInStaticArpTable = (  TcpIp_GetStaticArpEntryEndIdxOfStaticArpTable(staticArpTableIdx)
                                                                  - TcpIp_GetStaticArpEntryStartIdxOfStaticArpTable(staticArpTableIdx));

    if ((EntryListPtr == NULL_PTR) || (NumOfElements > numElementsInStaticArpTable))
    {
      totalNumOfStaticElementsToRead = numElementsInStaticArpTable;
    }
    else
    {
      totalNumOfStaticElementsToRead = NumOfElements;
    }

    /* Check if the buffer is valid */
    if (EntryListPtr != NULL_PTR)
    {
      /* Index to iterate over arp cache entries stored in the related controller cache */
      uint32 staticArpEntryLimitIdx = TcpIp_GetStaticArpEntryStartIdxOfStaticArpTable(staticArpTableIdx) + totalNumOfStaticElementsToRead;
      /* Index to read cache entries */
      TcpIp_StaticArpEntryIterType staticArpEntryIdx;
      /* Index to write into the Entrylist buffer pointer */
      uint32 entryListIdx = 0;

      /* #20 Copy valid entries from  Static table. */
      for (staticArpEntryIdx = TcpIp_GetStaticArpEntryStartIdxOfStaticArpTable(staticArpTableIdx);
           staticArpEntryIdx < staticArpEntryLimitIdx;
           staticArpEntryIdx++)
      { /* #30 Read Ip address, Physical address and state of each entry. */
        TcpIp_PhysAddrOfStaticArpEntryPtrType physAddrPtr = TcpIp_GetAddrPhysAddrOfStaticArpEntry(staticArpEntryIdx);

        TCPIP_ASSERT(staticArpEntryIdx < TcpIp_GetStaticArpEntryEndIdxOfStaticArpTable(staticArpTableIdx));

        EntryListPtr[entryListIdx].InetAddr[0] = TcpIp_GetIpAddrOfStaticArpEntry(staticArpEntryIdx);
        EntryListPtr[entryListIdx].PhysAddr[0] = physAddrPtr->data[0];
        EntryListPtr[entryListIdx].PhysAddr[1] = physAddrPtr->data[1];
        EntryListPtr[entryListIdx].PhysAddr[2] = physAddrPtr->data[2];
        EntryListPtr[entryListIdx].PhysAddr[3] = physAddrPtr->data[3];
        EntryListPtr[entryListIdx].PhysAddr[4] = physAddrPtr->data[4];
        EntryListPtr[entryListIdx].PhysAddr[5] = physAddrPtr->data[5];
        EntryListPtr[entryListIdx].State = TCPIP_ARP_ENTRY_STATIC;

        entryListIdx++;
      }

      *EntryListIdxPtr = entryListIdx;
    }
  }
  return totalNumOfStaticElementsToRead;
} /* IpV4_Arp_VReadStaticTable() */
/**********************************************************************************************************************
 * IpV4_Arp_VWriteValidEntries
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) IpV4_Arp_VWriteValidEntries(
  TcpIp_IpV4CtrlIterType            IpV4CtrlIdx,
  TcpIp_SizeOfArpConfigType         ArpConfigIdx,
  IPV4_P2V(TcpIp_ArpCacheEntryType) EntryListPtr,
  IPV4_P2V(uint32)                  EntryListIdxPtr,
  uint32                            DynElementsToReadLimitIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_ArpTableEntryIterType     arpTableEntryIdx; /* ArpTable Entry index */
  /* Limit index to read the entries from the cache */
  uint32 limitIdxToReadEntries = DynElementsToReadLimitIdx;
  /* ----- Implementation ----------------------------------------------- */
  /* Get  configured ArpConfig(Arp Resolution Table) Index for the Ip controller Index */

   /* #10 Iterate over dynamic ARP table. */
  for (arpTableEntryIdx = TcpIp_GetArpTableEntryStartIdxOfIpV4Ctrl(IpV4CtrlIdx);
       arpTableEntryIdx < limitIdxToReadEntries;
       arpTableEntryIdx++)
  {
    TCPIP_ASSERT(ArpConfigIdx < TcpIp_GetSizeOfArpConfig());
    /* #20 Skip entry if state is INVALID or address resolution is still in progress. */
    if (   (TcpIp_GetStatusOfArpTableEntry(arpTableEntryIdx) == IPV4_ARP_ELSTATUS_INVALID)                              /* PRQA S 3415 2 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
        || (TcpIp_GetStatusOfArpTableEntry(arpTableEntryIdx) == IPV4_ARP_ELSTATUS_INPROGESS))
    {
      /* Skip entries that are invalid or not resolved yet and update the Limit index to read the entries. */
      if (limitIdxToReadEntries < TcpIp_GetArpTableEntryEndIdxOfIpV4Ctrl(IpV4CtrlIdx))
      {
        /* Update the Limit index to read the entries if an invalid entry is found.*/
        (limitIdxToReadEntries)++;
      }
    }
    /* #30 Otherwise: Write valid or stale entry to output entry list. */
    else
    {
      IPV4_P2C(TcpIp_PhysAddrType) physAddrPtr = TcpIp_GetAddrPhysAddrOfArpTableEntry(arpTableEntryIdx);
      IPV4_P2V(uint8) dstAddrPtr = &EntryListPtr[*EntryListIdxPtr].PhysAddr[0];
      IPV4_P2C(uint8) srcAddrPtr = &physAddrPtr->data[0];

      /* return valid or stale entry. */
      EntryListPtr[*EntryListIdxPtr].InetAddr[0] = TcpIp_GetNetAddrOfArpTableEntry(arpTableEntryIdx);

      TcpIp_VCopyPhysAddr(dstAddrPtr, srcAddrPtr);                                                                      /* VCA_TCPIP_ARP_GET_ARP_CACHE_ENTRIES */

      if (TcpIp_GetStatusOfArpTableEntry(arpTableEntryIdx) == IPV4_ARP_ELSTATUS_VALID)
      {
        if(   (IpV4_Arp_MainFuncCycle - TcpIp_GetCreationTimeStampOfArpTableEntry(arpTableEntryIdx))
           >= TcpIp_GetEntryTimeoutOfArpConfig(ArpConfigIdx))
        {
          EntryListPtr[*EntryListIdxPtr].State = TCPIP_ARP_ENTRY_STALE;
        }
        else
        {
          EntryListPtr[*EntryListIdxPtr].State = TCPIP_ARP_ENTRY_VALID;
        }
      }
      else
      {
        EntryListPtr[*EntryListIdxPtr].State = TCPIP_ARP_ENTRY_STALE;
      }

      (*EntryListIdxPtr)++;
    }
  }
} /* IpV4_Arp_VWriteValidEntries */
/**********************************************************************************************************************
 * IpV4_Arp_VReadDynTable
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
TCPIP_LOCAL FUNC(uint32, TCPIP_CODE) IpV4_Arp_VReadDynTable(
  TcpIp_IpV4CtrlIterType            IpV4CtrlIdx,
  uint32                            NumOfElements,
  IPV4_P2V(TcpIp_ArpCacheEntryType) EntryListPtr,
  IPV4_P2V(uint32)                  EntryListIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 numOfDynamicElementsToRead = 0u;
  uint32 invalidEntryCount = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if dynamic ARP table is configured on specified IP controller. */
  if (TcpIp_IsArpConfigUsedOfIpV4Ctrl(IpV4CtrlIdx) == TRUE)
  {
    /* Get configured Arpconfig Index for given Ip controller */
    TcpIp_SizeOfArpConfigType arpConfigIdx = TcpIp_GetArpConfigIdxOfIpV4Ctrl(IpV4CtrlIdx);
    /*@ assert arpConfigIdx < TcpIp_GetSizeOfArpConfig(); */                                                            /* VCA_TCPIP_CSL03_IS_USEDOF */

    /* Get the total number of Arp entries for a particular arpconfig */
    numOfDynamicElementsToRead = TcpIp_GetTableSizeOfArpConfig(arpConfigIdx);

    TCPIP_ASSERT(numOfDynamicElementsToRead == (uint32)(  TcpIp_GetArpTableEntryEndIdxOfIpV4Ctrl(IpV4CtrlIdx)
                                                        - TcpIp_GetArpTableEntryStartIdxOfIpV4Ctrl(IpV4CtrlIdx)));
    /* #20 Return number of valid entries in the ARP table, if no output buffer was supplied. */
    if (EntryListPtr == NULL_PTR)
    {
      TcpIp_ArpTableEntryIterType arpTableEntryIdx;

      for (arpTableEntryIdx = TcpIp_GetArpTableEntryStartIdxOfIpV4Ctrl(IpV4CtrlIdx);
           arpTableEntryIdx < TcpIp_GetArpTableEntryEndIdxOfIpV4Ctrl(IpV4CtrlIdx);
           arpTableEntryIdx++)
      {
        if (   (TcpIp_GetStatusOfArpTableEntry(arpTableEntryIdx) == IPV4_ARP_ELSTATUS_INVALID)                          /* PRQA S 3415 2 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
            || (TcpIp_GetStatusOfArpTableEntry(arpTableEntryIdx) == IPV4_ARP_ELSTATUS_INPROGESS))
        {
          /* Ignore the entry */
          invalidEntryCount++;
        }
      }
      numOfDynamicElementsToRead -= invalidEntryCount;
    }
    else
    /* #30 Otherwise copy valid entries from the ARP table into the output buffer.
          (Number of entries may be limited by the size of output buffer) */
    {
      uint32 previousTableIdx = *EntryListIdxPtr;
      /* End index to limit the reading of the arp entries from the dynamic table. */
      uint32 readEntryDyntableLimitIdx = numOfDynamicElementsToRead + TcpIp_GetArpTableEntryStartIdxOfIpV4Ctrl(IpV4CtrlIdx);
      /* limit maximum number of returned entries if provided output buffer cannot hold all entries. */
      if (NumOfElements < TcpIp_GetTableSizeOfArpConfig(arpConfigIdx))
      {
        readEntryDyntableLimitIdx = NumOfElements + TcpIp_GetArpTableEntryStartIdxOfIpV4Ctrl(IpV4CtrlIdx);
      }
      IpV4_Arp_VWriteValidEntries(IpV4CtrlIdx, arpConfigIdx, EntryListPtr, EntryListIdxPtr, readEntryDyntableLimitIdx);
      numOfDynamicElementsToRead = *EntryListIdxPtr - previousTableIdx;
    }
  }

  return numOfDynamicElementsToRead;
} /* IpV4_Arp_VReadDynTable() */

/**********************************************************************************************************************
 *  IpV4_Arp_VSendMessage
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) IpV4_Arp_VSendMessage(
  TcpIp_IpV4CtrlIterType                   IpV4CtrlIdx,
  IpBase_AddrInType                        RemNetAddr,
  IpBase_AddrInType                        LocNetAddr,
  IPV4_P2C(uint8)                          RemPhysAddr,                                                                 /* PRQA S 3673 */ /* MD_IPV4_Rule8.3 */
  uint16                                   Operation)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType         retValue   = E_NOT_OK;
  uint8                  bufIdx     = 0u;
  uint16                 bufLenByte = IPV4_ARP_PACKET_LEN_BYTE;
  IPV4_P2V(Eth_DataType) ethIfTxBufferPtr;


  /* ----- Implementation ----------------------------------------------- */

  /* #10 Obtain ethernet TX buffer. */
  if (TCPIP_LL_ProvideTxBuffer(TcpIp_GetEthIfCtrlIdxOfIpV4Ctrl(IpV4CtrlIdx),
      IPV4_ARP_FRAME_TYPE,
      TcpIp_GetFramePrioDefaultOfIpV4Ctrl(IpV4CtrlIdx), &bufIdx, &ethIfTxBufferPtr, &bufLenByte) == BUFREQ_OK)
  {
    uint8* ethIfBufferPtr = (uint8*)&ethIfTxBufferPtr[0];                                                               /* PRQA S 0310 */ /* MD_TCPIP_Rule11.3_0310_CastToUInt8 */

    IpBase_EthPhysAddrType         ethDstAddr = { 0u };
    IpBase_EthPhysAddrType         srcMacAddr = { 0u };
    IpBase_EthPhysAddrType         dstMacAddr = { 0u };

    TCPIP_ASSERT(bufLenByte >= IPV4_ARP_PACKET_LEN_BYTE);
    if (bufLenByte >= IPV4_ARP_PACKET_LEN_BYTE)
    {
      /* #20 Build the ARP packet. */
      TCPIP_PUT_UINT16(ethIfBufferPtr, IPV4_ARP_HW_ADDR_TYPE_OFS, IPV4_ARP_HW_ADDR_TYPE_ETHERNET);
      TCPIP_PUT_UINT16(ethIfBufferPtr, IPV4_ARP_PR_ADDR_TYPE_OFS, IPV4_ARP_PROT_ADDR_TYPE_IPV4);
      TCPIP_PUT_UINT8(ethIfBufferPtr,  IPV4_ARP_HW_ADDR_SIZE_OFS, IPV4_ARP_HW_ADDR_SIZE_ETHERNET);
      TCPIP_PUT_UINT8(ethIfBufferPtr,  IPV4_ARP_PR_ADDR_SIZE_OFS, IPV4_ARP_PROT_ADDR_SIZE_IPV4);
      TCPIP_PUT_UINT16(ethIfBufferPtr, IPV4_ARP_OP_OFS, Operation);

      /* store local MAC address. */
      TCPIP_LL_GetPhysAddr(TcpIp_GetEthIfCtrlIdxOfIpV4Ctrl(IpV4CtrlIdx), srcMacAddr);

      TCPIP_PUT_UINT8(ethIfBufferPtr, IPV4_ARP_SRC_MC_ADDR_OFS,   srcMacAddr[0]);
      TCPIP_PUT_UINT8(ethIfBufferPtr, IPV4_ARP_SRC_MC_ADDR_OFS + 1u, srcMacAddr[1]);
      TCPIP_PUT_UINT8(ethIfBufferPtr, IPV4_ARP_SRC_MC_ADDR_OFS + 2u, srcMacAddr[2]);
      TCPIP_PUT_UINT8(ethIfBufferPtr, IPV4_ARP_SRC_MC_ADDR_OFS + 3u, srcMacAddr[3]);
      TCPIP_PUT_UINT8(ethIfBufferPtr, IPV4_ARP_SRC_MC_ADDR_OFS + 4u, srcMacAddr[4]);
      TCPIP_PUT_UINT8(ethIfBufferPtr, IPV4_ARP_SRC_MC_ADDR_OFS + 5u, srcMacAddr[5]);

      TCPIP_PUT_UINT32_RAW(ethIfBufferPtr, IPV4_ARP_SRC_IP_ADDR_OFS, LocNetAddr);

      /* store remote MAC address. */
      TcpIp_VCopyPhysAddr(dstMacAddr, RemPhysAddr);

      TCPIP_PUT_UINT8(ethIfBufferPtr, IPV4_ARP_DST_MC_ADDR_OFS, dstMacAddr[0]);
      TCPIP_PUT_UINT8(ethIfBufferPtr, IPV4_ARP_DST_MC_ADDR_OFS + 1u, dstMacAddr[1]);
      TCPIP_PUT_UINT8(ethIfBufferPtr, IPV4_ARP_DST_MC_ADDR_OFS + 2u, dstMacAddr[2]);
      TCPIP_PUT_UINT8(ethIfBufferPtr, IPV4_ARP_DST_MC_ADDR_OFS + 3u, dstMacAddr[3]);
      TCPIP_PUT_UINT8(ethIfBufferPtr, IPV4_ARP_DST_MC_ADDR_OFS + 4u, dstMacAddr[4]);
      TCPIP_PUT_UINT8(ethIfBufferPtr, IPV4_ARP_DST_MC_ADDR_OFS + 5u, dstMacAddr[5]);

      TCPIP_PUT_UINT32(ethIfBufferPtr, IPV4_ARP_DST_IP_ADDR_OFS, TCPIP_HTONL(RemNetAddr));

      /* #30 Set destination MAC address of the ethernet frame to broadcast for special use cases. */
      /* Use cases are:
       *   - case 1: if sender IP address is a link-local address
       *   - case 2: if a gratuitous ARP reply message is sent.
       *
       * Note that for a gratuitous ARP request message the address is already set to broadcast. So nothing
       * needs to be done here. */
      if (   IpV4_Arp_IsGratuitousArpReply(Operation, dstMacAddr, srcMacAddr, RemNetAddr, LocNetAddr)
          || ((LocNetAddr == 0u) || ((LocNetAddr & IPV4_IP_LL_NET_ADDR_MASK) == IPV4_IP_LL_NET_ADDR_NET)))
      {
        /* Case 1:
         * "All ARP packets (*replies* as well as requests) that contain a Link-
         *  Local 'sender IP address' MUST be sent using link-layer broadcast
         *  instead of link-layer unicast."
         * [RFC3927 2.5. Conflict Detection and Defense]
         *
         * Case 2:
         * "[...] for a gratuitous ARP, the ARP packet MUST be
         *  transmitted as a local broadcast packet on the local link"
         * [RFC 3344 4.6. ARP, Proxy ARP, and Gratuitous ARP]
         */
        TcpIp_VCopyPhysAddr(ethDstAddr, IpV4_Arp_PhysBroadcastAddr);
      }
      else
      {
        TcpIp_VCopyPhysAddr(ethDstAddr, RemPhysAddr);
      }

      /* #40 Transmit the message. */
      retValue = TCPIP_LL_Transmit( TcpIp_GetEthIfCtrlIdxOfIpV4Ctrl(IpV4CtrlIdx), bufIdx, IPV4_ARP_FRAME_TYPE, FALSE, IPV4_ARP_PACKET_LEN_BYTE, &ethDstAddr[0]);
    }
  }
  /* else: EthIf could not provide a buffer, function will return with "E_NOT_OK". */

  return retValue;
} /* IpV4_Arp_VSendMessage() */

/**********************************************************************************************************************
 *  IpV4_Arp_IsGratuitousArpReply
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  */
TCPIP_LOCAL FUNC(boolean, TCPIP_CODE) IpV4_Arp_IsGratuitousArpReply(
  uint16                                   Operation,
  IPV4_P2C(uint8)                          RemPhysAddrPtr,
  IPV4_P2C(uint8)                          LocPhysAddrPtr,
  IpBase_AddrInType                        RemNetAddr,
  IpBase_AddrInType                        LocNetAddr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isGratuitousArpReply = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the message is a gratuitous ARP reply message. */
  /* This is the format of a gratuitous ARP reply message (RFC 3344 4.6. ARP, Proxy ARP, and Gratuitous ARP):
   *   - Operation  == Reply
   *   - Sender MAC == Target MAC
   *   - Sender IP  == Target IP
   */
  if ((Operation == IPV4_ARP_OP_REPLY)
      && IPV4_LL_ADDR_IS_EQUAL(RemPhysAddrPtr, LocPhysAddrPtr)
      && (RemNetAddr == LocNetAddr))
  {
    isGratuitousArpReply = TRUE;
  }
  /* else: The ARP message is not a gratuitous ARP, function will return with "FALSE". */

  return isGratuitousArpReply;
} /* IpV4_Arp_IsGratuitousArpReply() */

/**********************************************************************************************************************
 *  IpV4_Arp_VUpdateEntryParameters
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) IpV4_Arp_VUpdateEntryParameters(
  IPV4_P2C(uint8)                          PhysAddr,                                                                    /* PRQA S 3673 */ /* MD_IPV4_Rule8.3 */
  IpBase_AddrInType                        NetAddr,
  TcpIp_SizeOfArpTableEntryType            ArpTableEntryIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  TCPIP_P2V(uint8) physAddrOfArpTableEntryPtr = &TcpIp_GetAddrPhysAddrOfArpTableEntry(ArpTableEntryIdx)->data[0];
  /* #10 Update parameters of the ARP entry. */
  TcpIp_SetNetAddrOfArpTableEntry(ArpTableEntryIdx, NetAddr);
  TcpIp_SetTimeStampOfArpTableEntry(ArpTableEntryIdx, IpV4_Arp_MainFuncCycle);
  TcpIp_SetCreationTimeStampOfArpTableEntry(ArpTableEntryIdx, IpV4_Arp_MainFuncCycle);
  TcpIp_VCopyPhysAddr(physAddrOfArpTableEntryPtr, PhysAddr);
  TcpIp_SetStatusOfArpTableEntry(ArpTableEntryIdx, IPV4_ARP_ELSTATUS_VALID);
}

/**********************************************************************************************************************
 *  IpV4_Arp_VUpdateEntry
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) IpV4_Arp_VUpdateEntry(
  TcpIp_IpV4CtrlIterType                   IpV4CtrlIdx,
  IPV4_P2C(uint8)                          PhysAddr,                                                                    /* PRQA S 3673 */ /* MD_IPV4_Rule8.3 */
  IpBase_AddrInType                        NetAddr,
  TcpIp_SizeOfArpTableEntryType            ArpTableEntryIdx,
  boolean                                  EntryInTable)
{
  /* ----- Implementation ----------------------------------------------- */
  TCPIP_P2C(uint8) physAddrOfArpTableEntryPtr = &TcpIp_GetAddrPhysAddrOfArpTableEntry(ArpTableEntryIdx)->data[0];
  /* Physical address initialization for later comparison. */
  CONST(IpBase_EthPhysAddrType, TCPIP_CONST) InitialArpPhyAddr =  { 0x0U, 0x0U, 0x0U, 0x0U, 0x0U, 0x0U };

  /* #10 Update existing entry. */
  if (EntryInTable == TRUE)
  {
    /* #20 Update the entry if it is in progress and physAddr has not been received earlier. */
    if ((TcpIp_GetStatusOfArpTableEntry(ArpTableEntryIdx) == IPV4_ARP_ELSTATUS_INPROGESS) && (IPV4_LL_ADDR_IS_EQUAL(physAddrOfArpTableEntryPtr, &InitialArpPhyAddr[0])))/* PRQA S 3415 2 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
    {
      /* This is an answer for a pending request. */
      TcpIp_DecNumPendArpElemOfIpV4CtrlDyn(IpV4CtrlIdx);
      IpV4_Arp_VUpdateEntryParameters(PhysAddr, NetAddr, ArpTableEntryIdx);
      IpV4_Arp_VPhysAddrTableChgNotification(IpV4CtrlIdx, ArpTableEntryIdx, TRUE);
    }
    /* #30 Update the entry if it is in prob and physAddr is same. */
    else if ((TcpIp_GetStatusOfArpTableEntry(ArpTableEntryIdx) == IPV4_ARP_ELSTATUS_PROBE) && (IPV4_LL_ADDR_IS_EQUAL(physAddrOfArpTableEntryPtr, PhysAddr)))/* PRQA S 3415 2 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
    {
      TcpIp_DecNumPendArpElemOfIpV4CtrlDyn(IpV4CtrlIdx);
      IpV4_Arp_VUpdateEntryParameters(PhysAddr, NetAddr, ArpTableEntryIdx);
    }
    /* #40 Update the entry if it is not invalid and physAddr is different. */
    else if (!(IPV4_LL_ADDR_IS_EQUAL(physAddrOfArpTableEntryPtr, PhysAddr))
            && (TcpIp_IsInfinitTimeOfArpTableEntry(ArpTableEntryIdx) == FALSE))
    {
      IpV4_Arp_VUpdateEntryParameters(PhysAddr, NetAddr, ArpTableEntryIdx);
      IpV4_Arp_VPhysAddrTableChgDiscardedNotification(IpV4CtrlIdx, /* Updated */ TcpIp_GetNetAddrOfArpTableEntry(ArpTableEntryIdx));
      IpV4_Arp_VPhysAddrTableChgNotification(IpV4CtrlIdx, ArpTableEntryIdx, TRUE);

    }
    else
    {
      /* Do not update the existing entry due to duplicate arp reply received. */
    }
  }
  /* #50 Update the Old existed entry or create the new one. */
  else
  {
    IpV4_Arp_VUpdateEntryParameters(PhysAddr, NetAddr, ArpTableEntryIdx);
  }
} /* IpV4_Arp_VUpdateEntry() */

/**********************************************************************************************************************
 *  IpV4_Arp_VStoreDynamicEntry
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL_INLINE FUNC(void, TCPIP_CODE) IpV4_Arp_VStoreDynamicEntry(
  TcpIp_IpV4CtrlIterType                   IpV4CtrlIdx,
  IpBase_AddrInType                        NetAddr,
  IPV4_P2C(uint8)                          PhysAddr,
  boolean                                  AddIfNotExisting)
{
   /* ----- Local Variables ---------------------------------------------- */
  boolean entryInTable = FALSE;
  TcpIp_ArpTableEntryIterType arpTableEntryIdx;
  TcpIp_ArpTableEntryIterType oldestArpTableEntryIdx   = TcpIp_GetArpTableEntryStartIdxOfIpV4Ctrl(IpV4CtrlIdx);
  /*@ assert oldestArpTableEntryIdx < TcpIp_GetSizeOfArpTableEntry(); */                                                /* VCA_TCPIP_CSL03_START_END */
  TcpIp_ArpTableEntryIterType freeArpResTabEleIdx = IPV4_ARP_INVALID_RES_IDX;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over ARP table and locate position of existing entry, unused entry or least recently used entry. */
  for (arpTableEntryIdx = TcpIp_GetArpTableEntryStartIdxOfIpV4Ctrl(IpV4CtrlIdx);
       arpTableEntryIdx < TcpIp_GetArpTableEntryEndIdxOfIpV4Ctrl(IpV4CtrlIdx);
       arpTableEntryIdx++)
  {
    /*@ assert arpTableEntryIdx < TcpIp_GetSizeOfArpTableEntry(); */                                                    /* VCA_TCPIP_CSL03_START_END */

    if (TcpIp_GetStatusOfArpTableEntry(arpTableEntryIdx) != IPV4_ARP_ELSTATUS_INVALID)
    {
      /* entry is "valid", "in progress" or "in prob" */
      if (TcpIp_GetNetAddrOfArpTableEntry(arpTableEntryIdx) == NetAddr)
      {
        /* matching entry found */
        entryInTable = TRUE;
        break;
      }
      if (IPV4_ARP_TIMESTAMP_IS_OLDER(TcpIp_GetTimeStampOfArpTableEntry(arpTableEntryIdx),
                                      TcpIp_GetTimeStampOfArpTableEntry(oldestArpTableEntryIdx)))
      {
        /* remember oldest entry */
        oldestArpTableEntryIdx = arpTableEntryIdx;
      }
    }
    else if (freeArpResTabEleIdx == IPV4_ARP_INVALID_RES_IDX)
    {
      /* store free entry index */
      freeArpResTabEleIdx = arpTableEntryIdx;
    }
    else
    {
      /* ignore further free entries */
    }
  }

  /*@ assert oldestArpTableEntryIdx < TcpIp_GetSizeOfArpTableEntry(); */                                                /* VCA_TCPIP_CSL03_START_END */

  /* #20 Update existing entry or create new entry, if requested. */
  if ((entryInTable == TRUE) || (AddIfNotExisting == TRUE))
  {
    if (entryInTable == TRUE)
    {
      /*@ assert arpTableEntryIdx < TcpIp_GetSizeOfArpTableEntry(); */                                                  /* VCA_TCPIP_CSL03_START_END */
      IpV4_Arp_VUpdateEntry(IpV4CtrlIdx, PhysAddr, NetAddr, (TcpIp_SizeOfArpTableEntryType)arpTableEntryIdx, entryInTable);
    }
    else  /* AddIfNotExisting == TRUE */
    {
      /* Found a free entry index. Create a new entry. */ /* Entry is new, table is not full */
      if (freeArpResTabEleIdx != IPV4_ARP_INVALID_RES_IDX)
      {
        /* use free entry */
        IpV4_Arp_VUpdateEntry(IpV4CtrlIdx, PhysAddr, NetAddr, (TcpIp_SizeOfArpTableEntryType)freeArpResTabEleIdx, entryInTable);
        IpV4_Arp_VPhysAddrTableChgNotification(IpV4CtrlIdx, (TcpIp_SizeOfArpTableEntryType)freeArpResTabEleIdx, TRUE);
      }
      /* #30 Check if an existing entry needs to be replaced by the new entry. */
      else
      {
# if (TCPIP_SUPPORT_ARP_DISCARDED_ENTRY_HANDLING == STD_OFF)
        /* Incomming Entry is new, table is full, existing old entry needs to be replaced */
        /* #40 Notify upper layer about the update of entry. */
        IpV4_Arp_VPhysAddrTableChgDiscardedNotification(IpV4CtrlIdx, /* Overwritten */ TcpIp_GetNetAddrOfArpTableEntry(oldestArpTableEntryIdx));
        IpV4_Arp_VUpdateEntry(IpV4CtrlIdx, PhysAddr, NetAddr, (TcpIp_SizeOfArpTableEntryType)oldestArpTableEntryIdx, entryInTable);
        IpV4_Arp_VPhysAddrTableChgNotification(IpV4CtrlIdx, (TcpIp_SizeOfArpTableEntryType)oldestArpTableEntryIdx, TRUE);
# else
        /* #50 Inform the upper layer about discarded entry, in case the new entry is discarded when table is full. */
        /* Table is full, Entry canont be overwritten */
        IpV4_Arp_VPhysAddrTableChgDiscardedNotification(IpV4CtrlIdx, /* Discarded */ NetAddr);
# endif
      }
    }
  }
} /* IpV4_Arp_VStoreDynamicEntry() */

/**********************************************************************************************************************
 *  IpV4_Arp_VPhysAddrTableChgNotification
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
TCPIP_LOCAL_INLINE FUNC(void, TCPIP_CODE) IpV4_Arp_VPhysAddrTableChgNotification(
  TcpIp_IpV4CtrlIterType        IpV4CtrlIdx,
  TcpIp_SizeOfArpTableEntryType ArpTableEntryIdx,
  boolean                       Valid)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SockAddrBaseType sockAddrInet;                                                                                  /* PRQA S 0759 */ /* MD_MSR_Union */
  TCPIP_P2V(uint8) physAddrPtr = &TcpIp_GetAddrPhysAddrOfArpTableEntry(ArpTableEntryIdx)->data[0];

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(IpV4CtrlIdx < TcpIp_GetSizeOfIpV4Ctrl());
  TCPIP_ASSERT(ArpTableEntryIdx >= TcpIp_GetArpTableEntryStartIdxOfIpV4Ctrl(IpV4CtrlIdx));
  TCPIP_ASSERT(ArpTableEntryIdx <  TcpIp_GetArpTableEntryEndIdxOfIpV4Ctrl(IpV4CtrlIdx));

  sockAddrInet.SockAddrIn.sin_family = TCPIP_AF_INET;
  sockAddrInet.SockAddrIn.sin_addr   = TcpIp_GetNetAddrOfArpTableEntry(ArpTableEntryIdx);
  sockAddrInet.SockAddrIn.sin_port   = TCPIP_PORT_ANY;

  /* #10 Forward notification to TcpIp Core module.*/
  TcpIp_VPhysAddrTableChg(TcpIp_GetEthIfCtrlIdxOfIpV4Ctrl(IpV4CtrlIdx), &sockAddrInet, physAddrPtr, Valid);

} /* IpV4_Arp_VPhysAddrTableChgNotification() */

/**********************************************************************************************************************
 *  IpV4_Arp_VPhysAddrTableChgDiscardedNotification
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
TCPIP_LOCAL_INLINE FUNC(void, TCPIP_CODE) IpV4_Arp_VPhysAddrTableChgDiscardedNotification(
  TcpIp_IpV4CtrlIterType IpV4CtrlIdx,
  IpBase_AddrInType      NetAddr)
{
   /* ----- Local Variables ---------------------------------------------- */
  TcpIp_PhysAddrConfigIterType physAddrConfigIdx;
  TcpIp_SockAddrBaseType   sockAddrInet;                                                                                /* PRQA S 0759 */ /* MD_MSR_Union */

  /* ----- Implementation ----------------------------------------------- */
  sockAddrInet.SockAddrIn.sin_family = TCPIP_AF_INET; /* IPv4 protocol */
  sockAddrInet.SockAddrIn.sin_addr   = NetAddr;
  sockAddrInet.SockAddrIn.sin_port   = TCPIP_PORT_ANY;
  /* Iterate over the ....*/
  for (physAddrConfigIdx = 0; physAddrConfigIdx < TcpIp_GetSizeOfPhysAddrConfig(); physAddrConfigIdx++)
  {
    /* #10 Forward notification to TcpIp Core module. */
    TcpIp_PhysAddrTableChgDiscardedCbkType funcPtr = TcpIp_GetChgDiscardedFuncPtrOfPhysAddrConfig(physAddrConfigIdx);

    if (funcPtr != NULL_PTR)
    {
      funcPtr(TcpIp_GetEthIfCtrlIdxOfIpV4Ctrl(IpV4CtrlIdx), (TCPIP_P2C(TcpIp_SockAddrType)) &sockAddrInet.TcpIpSockAddr);
      TCPIP_DUMMY_STATEMENT(sockAddrInet.SockAddrIn);                                                                   /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
    }
  }

} /* IpV4_Arp_VPhysAddrTableChgDiscardedNotification() */

/**********************************************************************************************************************
 *  IpV4_Arp_VStaticTableLookup
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
TCPIP_LOCAL_INLINE FUNC(Std_ReturnType, TCPIP_CODE) IpV4_Arp_VStaticTableLookup(
  TcpIp_IpV4CtrlIterType                   IpV4CtrlIdx,
  IpBase_AddrInType                        NetAddr,
  P2VAR(uint8, TCPIP_APPL_DATA, TCPIP_CONST) PhysAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over static ARP table and search for entry with matching IP address. */
  if (TcpIp_IsStaticArpTableUsedOfIpV4Ctrl(IpV4CtrlIdx) == TRUE)
  {
    TcpIp_SizeOfArpTableEntryType staticArpTableIdx = TcpIp_GetStaticArpTableIdxOfIpV4Ctrl(IpV4CtrlIdx);
    TcpIp_StaticArpTableIterType staticArpEntryIdx;

    for (staticArpEntryIdx = TcpIp_GetStaticArpEntryStartIdxOfStaticArpTable(staticArpTableIdx);
         staticArpEntryIdx < TcpIp_GetStaticArpEntryEndIdxOfStaticArpTable(staticArpTableIdx);
         staticArpEntryIdx++)
    {
      if (NetAddr == TcpIp_GetIpAddrOfStaticArpEntry(staticArpEntryIdx))
      {
        if (PhysAddrPtr != NULL_PTR)
        {
          /* #20 Return configured physical/MAC address of matching entry. */
          TCPIP_P2C(uint8) srcAddrPtr = &TcpIp_GetAddrPhysAddrOfStaticArpEntry(staticArpEntryIdx)->data[0];
          TcpIp_VCopyPhysAddr(PhysAddrPtr, srcAddrPtr);
        }
        retVal = E_OK;
        break;
      }
    }
  }
  /* No static ARP table entry exists for the supplied NetAddr. */
  return retVal;

} /* IpV4_Arp_VStaticTableLookup() */

/**********************************************************************************************************************
 *  IpV4_Arp_VDetectLinkLocalAddressConflict
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
/* PRQA S 6060 1 */ /* MD_MSR_STPAR */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) IpV4_Arp_VDetectLinkLocalAddressConflict(
  TcpIp_IpV4CtrlIterType IpV4CtrlIdx,
  uint16                 ArpOperation,
  IpBase_AddrInType      LocIpAddr,
  IpBase_AddrInType      ArpSrcIpAddr,
  IpBase_AddrInType      ArpDstIpAddr,
  IPV4_P2C(uint8)        ArpSrcMacAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if link-local address assignment is configured on controller. */
  if (IpV4_Ip_VCfgGetAddrAssignmentPrio(IpV4CtrlIdx, TCPIP_IPADDR_ASSIGNMENT_LINKLOCAL) != 0u)
  {
    boolean                sentByOtherHost = FALSE;
    IpBase_EthPhysAddrType ownPhysAddr;
    /* Get Controller's Physical(MAC) address. */
    TCPIP_LL_GetPhysAddr(TcpIp_GetEthIfCtrlIdxOfIpV4Ctrl(IpV4CtrlIdx), (ownPhysAddr));
    /* Compare Source MAC address and Controller's MAC address. */
    sentByOtherHost = (boolean)(!(IPV4_LL_ADDR_IS_EQUAL(ownPhysAddr, ArpSrcMacAddrPtr)));                               /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */

    /* #20 Notify link-local address configuration that sender IP address is used by other node, if required. */
    if (ArpSrcIpAddr != 0u)
    {
      /* "If during this period, from the beginning of the probing process
        *  until ANNOUNCE_WAIT seconds after the last probe packet is sent,
        *  the host receives any ARP packet (Request *or* Reply) where the packet's
        *  'sender IP address' is the address being probed for, then the
        *  host MUST treat this address as being in use by some other host,
        *  and MUST select a new pseudo-random address and repeat the process."
        *  [RFC3927 2.2.1. Probe details]
        */

      IpV4_Ip_LlAddrInUse(IpV4CtrlIdx, ArpSrcIpAddr);
    }
    /* #30 Notify link-local address configuration that target IP address is used by other node, if required. */
    else if ((ArpOperation == IPV4_ARP_OP_REQUEST) && (sentByOtherHost == TRUE))
    {
      /* "In addition, if during this period the host receives any ARP Probe
        *  where the packet's 'target IP address' is the address being probed for,
        *  and the packet's 'sender hardware address' is not the hardware address
        *  of the interface the host is attempting to configure, then the host MUST
        *  similarly treat this as an address conflict and select a new address as above."
        */

      IpV4_Ip_LlAddrInUse(IpV4CtrlIdx, ArpDstIpAddr);
    }
    else
    {
      /* nothing to do */
    }

    /* #40 Notify link-local address configuration if a conflict was detected. */
    if((LocIpAddr != 0u) && (ArpSrcIpAddr == LocIpAddr) && (sentByOtherHost == TRUE))
    {
      /* "At any time, if a host receives an ARP packet (request *or* reply) on
        *  an interface where the 'sender IP address' is the IP address the host
        *  has configured for that interface, but the 'sender hardware address'
        *  does not match the hardware address of that interface, then this is a
        *  conflicting ARP packet, indicating an address conflict."
        *  [RFC3927 2.5. Conflict Detection and Defense]
        */

      /* -> defend local address or restart address configuration */
      IpV4_Ip_AddrConflictInd(TcpIp_GetLocalAddrV4UnicastIdxOfIpV4Ctrl(IpV4CtrlIdx));

      retVal = E_NOT_OK;
    }
  }

  return retVal;
} /* IpV4_Arp_VDetectLinkLocalAddressConflict() */

/**********************************************************************************************************************
 *  IpV4_Arp_VUpdateTable
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(void, TCPIP_CODE) IpV4_Arp_VUpdateTable(
  TcpIp_IpV4CtrlIterType                   IpV4CtrlIdx,
  IpBase_AddrInType                        ArpSrcIpAddr,
  IPV4_P2C(uint8)                          ArpSrcMacAddr,
  boolean                                  AddIfNotExisting)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if dynamic ARP table is configured for this controller. */
  if (TcpIp_IsArpConfigUsedOfIpV4Ctrl(IpV4CtrlIdx) == TRUE)
  {
# if defined(TCPIP_ASSERT_HANDLER)                                                                                      /* COV_TCPIP_CANOE_DEBUG */
    TcpIp_SizeOfArpConfigType arpConfigIdx = TcpIp_GetArpConfigIdxOfIpV4Ctrl(IpV4CtrlIdx);
    TCPIP_ASSERT(TcpIp_GetTableSizeOfArpConfig(arpConfigIdx) > 0u);
# endif
    /* #20 Verify that entry does not exist in static ARP table, if configured. */
    if (IpV4_Arp_VStaticTableLookup(IpV4CtrlIdx, ArpSrcIpAddr, NULL_PTR) != E_OK)
    {
      /* #30 Store entry in the dynamic ARP table. */
      IpV4_Arp_VStoreDynamicEntry(IpV4CtrlIdx, ArpSrcIpAddr, ArpSrcMacAddr, AddIfNotExisting);                          /* VCA_TCPIP_CSL03_IS_USEDOF */
    }
  }
}  /* IpV4_Arp_VUpdateTable() */

/**********************************************************************************************************************
 *  IpV4_Arp_VRespondToPacket
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/

TCPIP_LOCAL FUNC(void, TCPIP_CODE) IpV4_Arp_VRespondToPacket(
  TcpIp_IpV4CtrlIterType                   IpV4CtrlIdx,
  uint16                                   ArpOperation,
  IpBase_AddrInType                        ArpSrcIpAddr,
  IpBase_AddrInType                        ArpDstIpAddr,
  IPV4_P2C(uint8)                          ArpSrcMacAddr)
{
  /* ----- Local Variables ---------------------------------------------- */
  IpBase_AddrInType locIpAddr = TcpIp_GetActiveNetAddrOfIpV4CtrlDyn(IpV4CtrlIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if ARP packet is an ARP Request. */
  if (ArpOperation == IPV4_ARP_OP_REQUEST)
  {
    /* #20 Check if ARP Request is addressed to this node. */
    if ((locIpAddr != 0u) && (ArpDstIpAddr == locIpAddr))
    {
      /* #30 Send ARP Reply if ctrl is not in state OnHold. */
      if (TcpIp_GetStateOfIpV4CtrlDyn(IpV4CtrlIdx) != IPV4_IP_ONHOLD)
      {
        (void)IpV4_Arp_VSendMessage(IpV4CtrlIdx, ArpSrcIpAddr, locIpAddr, ArpSrcMacAddr, IPV4_ARP_OP_REPLY);
      }
    }
  }
  /* else: No action necessary. */
} /* IpV4_Arp_VProccessArpRequest() */

/**********************************************************************************************************************
 *  IpV4_Arp_VHandlePacket
 *********************************************************************************************************************/
/*! Internal comment removed.
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
 *********************************************************************************************************************/
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) IpV4_Arp_VHandlePacket(
  TcpIp_IpV4CtrlIterType                   IpV4CtrlIdx,
  uint16                                   ArpOperation,
  IpBase_AddrInType                        ArpSrcIpAddr,
  IpBase_AddrInType                        ArpDstIpAddr,
  IPV4_P2C(uint8)                          ArpSrcMacAddr)
{
   /* ----- Local Variables ---------------------------------------------- */
  /* get IP address of the controller */
  IpBase_AddrInType locIpAddr = TcpIp_GetActiveNetAddrOfIpV4CtrlDyn(IpV4CtrlIdx);
  IpBase_EthPhysAddrType locPhysAddr;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get local MAC address of the controller. */
  TCPIP_LL_GetPhysAddr(TcpIp_GetEthIfCtrlIdxOfIpV4Ctrl(IpV4CtrlIdx), locPhysAddr);

  /* #20 Check if received ARP packet is an ARP reply that announces the local IP address of the controller
         in combination with a different MAC address. */
  if (   (ArpOperation == IPV4_ARP_OP_REPLY)
      && (ArpSrcIpAddr == locIpAddr)
      && (TcpIp_CmpLLAddr(ArpSrcMacAddr, locPhysAddr) != TRUE))                                                         /* PRQA S 3415 */ /* MD_TCPIP_Rule13.5_3415_ReadOnly */
  {
    /* #30 Report security event to Intrusion Detection System Manager (IdsM) */
    TcpIp_ReportSecurityEvent(TCPIP_SECURITY_EVENT_ARP_IP_ADDR_CONFLICT);
  }

  /* #40 Forward relevant information from ARP packet to link-local address configuration, if configured. (in order to detect IP address conflict) */
  if (IpV4_Arp_VDetectLinkLocalAddressConflict(IpV4CtrlIdx, ArpOperation, locIpAddr, ArpSrcIpAddr, ArpDstIpAddr, ArpSrcMacAddr) == E_OK)
  {
    /* #50 Check if source IP address of ARP packet is valid. */
    if (IPV4_ADDR_IS_MULTICAST(ArpSrcIpAddr) || IPV4_ADDR_IS_BROADCAST(IpV4CtrlIdx, ArpSrcIpAddr))
    {
      /* Ignore invalid source IP address. */
    }
    /* #60 Check if source MAC address of ARP packet is valid. */
    else if (!IPV4_LL_ADDR_IS_VALID(ArpSrcMacAddr))
    {
      /* Ignore invalid source MAC address. */
    }
    else
    {
      /* #70 Update dynamic ARP table, if required. */
      if (ArpSrcIpAddr != 0u)
      {
        IpV4_Arp_VUpdateTable(IpV4CtrlIdx, ArpSrcIpAddr, ArpSrcMacAddr, (boolean)(ArpDstIpAddr == locIpAddr));          /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
      }
      else
      {
        /* this is an ARP probe (from an IP link local address configuration) or a packet with an invalid source IP address.
         -> do not store the physical address */
      }
      /* #80 Send Arp Reply.*/
      IpV4_Arp_VRespondToPacket(IpV4CtrlIdx, ArpOperation, ArpSrcIpAddr, ArpDstIpAddr, ArpSrcMacAddr);
    }
  }
} /* IpV4_Arp_VHandlePacket() */

# define TCPIP_STOP_SEC_CODE
# include "TcpIp_MemMap.h"                                                                                              /* PRQA S 5087 */ /*  MD_MSR_19.1 */

/*
   module specific MISRA deviations:
*/

/* VCA_JUSTIFICATION_START

  \ID VCA_TCPIP_LRU_ALWAYS_IN_VALID_RANGE
    \DESCRIPTION       The value of OldestArpTableEntryIdxPtr always references a valid ARP table entry.
    \COUNTERMEASURE \N The calling function IpV4_Arp_GetPhysicalAddressFromDynamicTables() ensures that a dynamic
                       ARP table is present by usage of TcpIp_IsArpConfigUsedOfIpV4Ctrl() function with valid
                       IpV4CtrlIdx. Within function IpV4_Arp_VCheckArpEntry() it is ensured that the value of
                       OldestArpTableEntryIdxPtr is within the range:
                       [TcpIp_GetArpTableEntryStartIdxOfIpV4Ctrl(), TcpIp_GetArpTableEntryEndIdxOfIpV4Ctrl()[

  \ID VCA_TCPIP_ARP_TABLE_ENTRY_IN_VALID_RANGE
    \DESCRIPTION       The parameter arpTableEntryIdx references a valid ARP table entry.
    \COUNTERMEASURE \N The value arpTableEntryIdx is ensured to be valid because it is calculated either:
                       - via TcpIp_GetArpTableEntryIdxOfIpV4SocketDyn(), which either evaluates to a valid or the INV
                         value,
                       - or as return value of function IpV4_Arp_VCheckArpEntry() which also evaluates to a valid or
                         the INV value.
                       In both cases it is ensured by a runtime check of parameter elemInTable, that the value does not
                       equal the INV value.

  \ID VCA_TCPIP_GRATUITOUS_ARP_ALWAYS_IN_VALID_RANGE
    \DESCRIPTION       The value of gratuitousArpIdx always references a valid gratuitous ARP entry.
    \COUNTERMEASURE \N The calling function IpV4_Arp_MainFunction() ensures that gratuitous ARPs are configured
                       by usage of function TcpIp_IsGratuitousArpUsedOfIpV4Ctrl().
                       Within function IpV4_Arp_SendGratuitousArpOnStartup() the gratuitousArpIdx is obtained via
                       indirection (qualified use-case CSL03 of ComStackLib).

  \ID VCA_TCPIP_ARP_GET_ARP_CACHE_ENTRIES
    \DESCRIPTION       Pointer passed via public API TcpIp_GetArpCacheEntries() is used to write to memory.
    \COUNTERMEASURE \S Public API is called with the following parameters:
                       - NumberOfElementsPtr: Which contains the maximum amount of entries which can be copied to
                                              the memory location pointed by EntryListPtr.
                       - EntryListPtr: Pointer to an external memory location given by the callee.
                       IpV4_Arp_VWriteValidEntries() is called by IpV4_Arp_VReadDynTable() where
                       DynElementsToReadLimitIdx is calculated so that maximal NumberOfElementsPtr elements are between
                       startIdx and the calculated index.
                       This idx is used in IpV4_Arp_VWriteValidEntries() as end index of a for-loop started
                       by the StartIdx of the ARP-table. Inside the for loop the end idx is increased if a invalid
                       element is reached, the maximum is the EndIdx of the table. Only valid elements are written to
                       the EntryListPtr buffer. Thus the maximum number of written elements is the number
                       NumberOfElementsPtr passed to the public API. The consistency of NumberOfElementsPtr and
                       EntryListPtr is documented and ensured by safety manual entry [SMI-16].

  VCA_JUSTIFICATION_END */


#endif /* (TCPIP_SUPPORT_IPV4 == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: IpV4_Arp.c
 *********************************************************************************************************************/
