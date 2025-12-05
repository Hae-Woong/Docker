/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         \file  E2E_PJ193976.h
 *        \brief  E2E header file for SAE Profile J1939-76
 *
 *      \details  E2E protection ensures data exchange which is protected at runtime against the effects of faults within
 *                the communication link. E2E Library provides mechanisms for E2E protection, adequate for safety-related
 *                communication having requirements up to ASIL D.
 *
 *********************************************************************************************************************/

/**** Protection against multiple inclusion **************************************************************************/
#if !defined(E2E_PJ193976_H)
#  define E2E_PJ193976_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#  include "E2E.h"

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef struct
{
    uint8 DataLength;       /*!< Length of data, in bits */
    uint8 MaxDeltaCounter;  /*!< Maximum allowed difference between two counter values of consecutively received valid messages */

} E2E_PJ193976ConfigType;

typedef struct
{
    uint8 SequenceNumber;       /*!< Counter to be used for protecting the next Data */

} E2E_PJ193976ProtectStateType;

typedef enum
{
  E2E_PJ193976STATUS_OK            = 0x00,
  E2E_PJ193976STATUS_WRONGCRC      = 0x07,
  E2E_PJ193976STATUS_REPEATED      = 0x08,
  E2E_PJ193976STATUS_OKSOMELOST    = 0x20,
  E2E_PJ193976STATUS_WRONGSEQUENCE = 0x40,
  E2E_PJ193976STATUS_INITIAL       = 0xA1,
  E2E_PJ193976STATUS_ERROR         = 0xEE
} E2E_PJ193976CheckStatusType; /*!< Enum containing all possible statuses */

typedef struct
{
    E2E_PJ193976CheckStatusType Status;      /*!< Result of the verification of the Data */
    uint8 LastValidCounter;                  /*!< Counter value most recently received */
} E2E_PJ193976CheckStateType;

/**********************************************************************************************************************
 *  Typedefs of structures, cannot be used before the declaration of structs
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#  define E2E_START_SEC_CODE
#  include "E2E_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  E2E_PJ193976Protect()
 *********************************************************************************************************************/
/*!
 * \brief           Protection function of E2Elib Profile J1939-76
 * \details         Protects the array/buffer to be transmitted using the E2E Profile J1939-76. This includes checksum calculation,
 *                  and handling of counter.
 * \param[in]       ConfigPtr Pointer to profile configuration
 * \param[in,out]   StatePtr  Pointer to communication state
 * \param[in,out]   DataPtr   Pointer to data to be protected
 * \return          E2E_E_INPUTERR_NULL: A pointer parameter is NULL
 *                  E2E_E_INPUTERR_WRONG: One parameter is erroneous
 *                  E2E_E_INTERR: Internal library error
 *                  E2E_E_OK: Protection successful
 * \pre             -
 * \context         TASK
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \trace           CREQ-1086
 */
Std_ReturnType E2E_PJ193976Protect(
    const E2E_PJ193976ConfigType* ConfigPtr,
    E2E_PJ193976ProtectStateType* StatePtr,
    uint8* DataPtr);

/**********************************************************************************************************************
 *  E2E_PJ193976ProtectInit()
 *********************************************************************************************************************/
/*!
 * \brief           Protection initialization function of E2Elib Profile PJ1939-76
 * \details         Initializes the protection state by resetting the counter
 * \param[in,out]   StatePtr  Pointer to communication state
 * \return          E2E_E_INPUTERR_NULL: Pointer parameter is NULL
 *                  E2E_E_OK: Initialization successful
 * \pre             -
 * \context         TASK
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \trace           CREQ-1087
 */
Std_ReturnType E2E_PJ193976ProtectInit(E2E_PJ193976ProtectStateType* StatePtr);

/**********************************************************************************************************************
 *  E2E_PJ193976Check()
 *********************************************************************************************************************/
/*!
 * \brief           Check function of E2Elib Profile PJ1939-76
 * \details         Checks the Data received using the E2E Profile PJ1939-76. This includes CRC calculation and handling of
 *                  the counter.
 * \param[in]       ConfigPtr Pointer to profile configuration
 * \param[in,out]   StatePtr  Pointer to communication state
 * \param[in]       DataPtr   Pointer to data to be protected
 * \return          E2E_E_INPUTERR_NULL: A pointer parameter is NULL
 *                  E2E_E_INPUTERR_WRONG: One parameter is erroneous
 *                  E2E_E_INTERR: Internal library error
 *                  E2E_E_OK: Protection successful
 * \pre             -
 * \context         TASK
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \trace           CREQ-1088
 */
Std_ReturnType E2E_PJ193976Check(
    const E2E_PJ193976ConfigType* ConfigPtr,
    E2E_PJ193976CheckStateType* StatePtr,
    const uint8* DataPtr);

/**********************************************************************************************************************
 *  E2E_PJ193976CheckInit()
 *********************************************************************************************************************/
/*!
 * \brief           Check initialization function of E2Elib Profile PJ1939-76
 * \details         -
 * \param[in,out]   StatePtr  Pointer to communication state
 * \return          E2E_E_INPUTERR_NULL: Pointer parameter is NULL
 *                  E2E_E_OK: Initialization successful
 * \pre             -
 * \context         TASK
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \trace           CREQ-1089
 */
Std_ReturnType E2E_PJ193976CheckInit(E2E_PJ193976CheckStateType* StatePtr);

/**********************************************************************************************************************
 *  E2E_PJ193976MapStatusToSM()
 *********************************************************************************************************************/
/*!
 * \brief           State mapping function of E2Elib Profile PJ1939-76
 * \details         Maps the check status of Profile PJ1939-76 to a generic check status, which can be used by E2E state machine
 *                  check function. The E2E Profile PJ1939-76 delivers a more fine-grained status, but this is not relevant for
 *                  the E2E state machine.
 * \param[in]       CheckReturn     Return value of E2E_PJ193976Check
 * \param[in]       Status          communication state
 * \return          Standard state value to be used in E2Elib state machine
 * \pre             -
 * \context         TASK
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \trace           CREQ-1175
 * \trace           SPEC-2056631
 */
E2E_PCheckStatusType E2E_PJ193976MapStatusToSM(
    const Std_ReturnType CheckReturn, 
    const E2E_PJ193976CheckStatusType Status);

#  define E2E_STOP_SEC_CODE
#  include "E2E_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* E2E_PJ193976_H */

/**********************************************************************************************************************
 *  END OF FILE: E2E_PJ193976.h
 *********************************************************************************************************************/
