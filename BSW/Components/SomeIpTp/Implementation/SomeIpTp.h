/***********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  SomeIpTp.h
 *        \brief  SomeIpTp header file
 *      \details  Header file of SomeIpTp to be included by dependent components
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  1.00.00  2017-10-13   vissuk                Implementation of SomeIpTp according to AR 4.3.1
 *  1.01.00  2018-01-04   vissuk  STORYC-3279   Bring the SomeIpTp to QM state - step I
 *                        vissuk  ESCAN00097732 Diverse compiler errors at SomeIpTp and PostBuildLoadable
 *           2018-01-08   visath  ESCAN00097734 Diverse compiler errors at SomeIpTp and when DEV OFF
 *  1.01.01  2018-02-15   vissuk  ESCAN00098420 Data copied after calling CopyTxData is now properly handled
 *  2.00.00  2018-09-13   vissuk  STORYC-3531   Bring the SomeIpTp to QM state - step II
 *  2.00.00  2018-09-17   vissuk  ESCAN00100623 Compiler error: Unexpected empty macros when no Rx SDUs are configured
 *  2.00.00  2018-10-01   vissuk                Rx Timeout handling according to AR 4.3.1
 *  2.00.00  2018-10-08   vissuk                Negative TX confirmation to upper layer when SomeIpTp_Transmit is
 *                                              called with a PDU ID which is currently used for ongoing segmentation.
 *  2.00.00  2018-10-11   vissuk                Fixed out of bounds access of TxState and RxState arrays in postbuild 
 *                                              configurations.
 *  2.00.00  2018-10-12   vissuk  STORYC-5248   MISRA-C:2012 Compliance
 *  2.00.00  2018-10-15   vissuk  ESCAN00100137 Compiler error: Wrong pragma command / unknown memory section used.
 *  3.00.00  2020-03-06   vissuk  SWAT-851      Increase transmission speed of SomeIpTp frames
 *  3.00.00  2020-03-06   vissuk  ESCAN00105581 Wrong handling of critical sections in SomeIpTp_MainFunctionTx
 *  3.01.00  2020-05-20   vissuk  SWAT-732      Support metadata according to ASR 4.3
 *  3.02.00  2020-06-24   vissuk  SWAT-1033     Metadata consistency check during reception of TP frames
 *  4.00.00  2020-08-17   vissuk  SWAT-954      Bring the SomeIpTp from QM to ASIL D
 *  4.00.00  2020-09-09   vissuk  ESCAN00100137 Compiler error: Wrong pragma command / unknown memory section used.
 *  4.00.00  2020-09-25   visath  ESCAN00107411 Functions of the API are always available.
 *  4.00.01  2020-11-10   visath                Improvements after SafetyCheck.
 *  4.00.01  2020-12-02   visath  ESCAN00107559 Added missing closure of a memory section.
 *  5.00.00  2021-10-28   vissuk  SWAT-1753     Usage of <Mip>_MemMap
 *  5.00.00  2021-11-02   vissuk  SWAT-1788     Complexity Analysis
 *  5.01.00  2022-01-20   vissuk                Updated SW version & Generator version due to SWAT-1369
 *  5.01.01  2023-03-22   vissuk  ESCAN00112538 Queue element not popped after a successful single frame transmission.
 **********************************************************************************************************************/

#if !defined (SOMEIPTP_H)
# define SOMEIPTP_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "ComStack_Types.h"
#include "SomeIpTp_Cfg.h"
#include "SomeIpTp_Lcfg.h"
#include "SomeIpTp_PBcfg.h"

/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/

/* Vendor and module identification */
# define SOMEIPTP_VENDOR_ID                           (30u)
# define SOMEIPTP_MODULE_ID                           (0xB1u)

/* AUTOSAR Software specification version information */
# define SOMEIPTP_AR_RELEASE_MAJOR_VERSION            (4u)
# define SOMEIPTP_AR_RELEASE_MINOR_VERSION            (3u)
# define SOMEIPTP_AR_RELEASE_REVISION_VERSION         (1u)

/* Component version information  */
# define SOMEIPTP_SW_MAJOR_VERSION                    (5u)
# define SOMEIPTP_SW_MINOR_VERSION                    (1u)
# define SOMEIPTP_SW_PATCH_VERSION                    (1u)

# define SOMEIPTP_INSTANCE_ID_DET                     (0x00u)

/* ----- API service IDs ----- */
/*!< Service ID: SomeIpTp_GetVersionInfo() */
# define SOMEIPTP_SID_GET_VERSION_INFO                (0x01u)
/*!< Service ID: SomeIpTp_Init() */
# define SOMEIPTP_SID_INIT                            (0x02u)
/*!< Service ID: SomeIpTp_MainFunctionTx() */
# define SOMEIPTP_SID_MAINFUNCTION_TX                 (0x03u)
/*!< Service ID: SomeIpTp_MainFunctionRx() */
# define SOMEIPTP_SID_MAINFUNCTION_RX                 (0x04u)
/*!< Service ID: SomeIpTp_TxConfirmation() */
# define SOMEIPTP_SID_TXCONFIRMATION                  (0x40u)
/*!< Service ID: SomeIpTp_TriggerTransmit() */
# define SOMEIPTP_SID_TRIGGER_TRANSMIT                (0x41u)
/*!< Service ID: SomeIpTp_RxIndication() */
# define SOMEIPTP_SID_RXINDICATION                    (0x42u)
/*!< Service ID: SomeIpTp_Transmit() */
# define SOMEIPTP_SID_TRANSMIT                        (0x49u)


/* ----- Error codes ----- */
/*!
 *  /defgroup errorIDs Error IDs
 */
/*! used to check if no error occurred */
# define SOMEIPTP_E_NO_ERROR                     (0x00u)
/*! Error code: API service used without module initialization */
# define SOMEIPTP_E_NOTINIT                      (0x01u)
/*! Error code: API service used with invalid pointer */
# define SOMEIPTP_E_PARAM_POINTER                (0x02u)
/*! Error code: API service used with invalid value parameter */
# define SOMEIPTP_E_PARAM                        (0x03u)
/*! Error code: TP Flag of Message Type set to 0 */
# define SOMEIPTP_E_MESSAGE_TYPE                 (0x04u)
/*! Error code: Inconsistent subsequent segment received */
# define SOMEIPTP_E_INCONSISTENT_SEQUENCE        (0x05u)
/*! Error code: Inconsistent header received */
# define SOMEIPTP_E_INCONSISTENT_HEADER          (0x06u)
/*! Error code: Disassembly interrupt due to upper layer */
# define SOMEIPTP_E_DISASSEMBLY_INTERRUPT        (0x07u)
/*! Error code: Assembly interrupt due to upper layer */
# define SOMEIPTP_E_ASSEMBLY_INTERRUPT           (0x08u)
/*! Error code: Inconsistent metadata received */
# define SOMEIPTP_E_INCONSISTENT_METADATA        (0x09u)
/*! Error code: FATAL ERROR */
# define SOMEIPTP_E_FATAL_ERROR                  (0xFFu)


/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

# define SOMEIPTP_START_SEC_CODE
# include "SomeIpTp_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  SomeIpTp_InitMemory()
 ***********************************************************************************************************************/
/*! \brief      Function for *_INIT_*-variable initialization
 *  \details    Service to initialize module global variables at power up. This function initializes the
 *              variables in *_INIT_* sections. Used in case they are not initialized by the startup code.
 *  \pre        Module is uninitialized.
 *  \reentrant  FALSE
 *  \synchronous  TRUE
 *  \context    TASK
 ***********************************************************************************************************************/
extern FUNC(void, SOMEIPTP_CODE) SomeIpTp_InitMemory(void);

/***********************************************************************************************************************
 * SomeIpTp_Init()
 ***********************************************************************************************************************/
/*! \brief        Initializes the module SomeIpTp
 *  \details      Initializes all variables and sets the module state to initialized.
 *  \param[in]    ConfigPtr               Configuration structure for initializing the module
 *  \pre          Interrupts are disabled.
 *  \pre          Module is uninitialized.
 *  \pre          SomeIpTp_InitMemory has been called unless SomeIpTp_ModuleInitialized is initialized by start-up code.
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \trace        CREQ-169362
 ***********************************************************************************************************************/
extern FUNC(void, SOMEIPTP_CODE) SomeIpTp_Init(P2CONST(SomeIpTp_ConfigType, AUTOMATIC, SOMEIPTP_PBCFG) ConfigPtr);

/***********************************************************************************************************************
 * SomeIpTp_Transmit()
 ***********************************************************************************************************************/
/*! \brief        Requests transmission of a SomeIpTp Pdu.
 *  \details      -
 *  \param[in]    TxPduId       Pdu ID passed by the upper layer which is used for transmission
 *  \param[in]    PduInfoPtr    Pdu info passed by the upper layer. The passed buffer shall be at least as long as the
 *                              SOME/IP header (8 bytes) plus the length of the PDU's metadata.
 *  \return       E_OK     Transmit request was accepted.
 *  \return       E_NOT_OK Transmit request was rejected (wrong state or invalid length).
 *  \pre          Interrupts are enabled.
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different PDU IDs
 *  \synchronous  TRUE
 *  \config       This function is only available if SOMEIPTP_TXNSDU is STD_ON.
 *  \trace        CREQ-169364, CREQ-169365, CREQ-169375
 ***********************************************************************************************************************/
extern FUNC(Std_ReturnType, SOMEIPTP_CODE) SomeIpTp_Transmit(VAR(PduIdType, SOMEIPTP_APPL_VAR) TxPduId, P2CONST(PduInfoType, 
  AUTOMATIC, SOMEIPTP_APPL_DATA) PduInfoPtr);

#if (SOMEIPTP_VERSION_INFO_API == STD_ON)
/***********************************************************************************************************************
 * SomeIpTp_GetVersionInfo()
 ***********************************************************************************************************************/
/*! \brief        Returns module ID and SW version information.
 *  \details      -
 *  \param[in]    -
 *  \param[out]   VersionInfo   Pointer to store the version information of the module. Parameter must point to a
 *                              valid buffer.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \config       This function is only available if SOMEIPTP_VERSION_INFO_API is STD_ON.
 *  \trace        CREQ-169361
 ***********************************************************************************************************************/
extern FUNC(void, SOMEIPTP_CODE) SomeIpTp_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) VersionInfo);
#endif

/***********************************************************************************************************************
 * SomeIpTp_MainFunctionTx()
 ***********************************************************************************************************************/
/*! \fn           void SomeIpTp_MainFunctionTx(void)
 *  \brief        Cyclic mainfunction for transmission.
 *  \details      Supervisions Separation time, Tx Confirmation time and triggers consecutive frames.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \trace        CREQ-169363, CREQ-169365, CREQ-169366
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * SomeIpTp_MainFunctionRx()
 ***********************************************************************************************************************/
/*! \fn           void SomeIpTp_MainFunctionRx(void)
 *  \brief        Cyclic mainfunction for reception.
 *  \details      Supervisions Rx Timeout time.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \trace        CREQ-169363
 ***********************************************************************************************************************/

# define SOMEIPTP_STOP_SEC_CODE
# include "SomeIpTp_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  EXCLUSIVE AREA DEFINITION
 **********************************************************************************************************************/
/*!
 * \exclusivearea  SOMEIPTP_EXCLUSIVE_AREA_0
 *                 Ensure the consistency of the flags and counters related to Tx PDUs.
 * \protects       SomeIpTp_TxQueue
 *                 SomeIpTp_TxState and Header information of TX NSDUs
 * \usedin         SomeIpTp_Transmit, SomeIpTp_AddQueueElement, SomeIpTp_DelQueueElement, SomeIpTp_MainFunctionTx, 
 *                 SomeIpTp_MainFunctionRx
 * \exclude        Modifications to SomeIpTp_RxState and/or SomeIpTp_TxState. SomeIpTp_TxQueue from another context.
 * \length         SHORT used only when adding/deleting elements to TxQueue and modifying the Tx States.
 * \endexclusivearea
 */

#endif /* SOMEIPTP_H */
/***********************************************************************************************************************
 *  END OF FILE: SomeIpTp.h
 **********************************************************************************************************************/
