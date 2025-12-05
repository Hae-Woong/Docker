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
 *        \brief  E2E header file for AUTOSAR E2E Library Profile J193976
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
typedef uint16 E2E_PJ193976LengthType; /*!< Length in bytes */
typedef uint16 E2E_PJ193976CounterType;
typedef uint32 E2E_PJ193976OffsetType;
typedef uint32 E2E_PJ193976CrcType;
typedef uint32 E2E_PJ193976IdType;
typedef P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) E2E_PJ193976DataPtrType;
typedef P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) E2E_PJ193976ConstDataPtrType;


typedef struct
{
  uint32 DataLength;        /*!< Data length, in bits */
  uint16 MaxDeltaCounter;   /*!< Maximum allowed difference between two counter values of consecutively received valid messages */
} E2E_PJ193976ConfigType;

typedef struct
{
  uint16 Counter; /*!< Counter to be used for protecting the next Data */
} E2E_PJ193976ProtectStateType;

typedef enum
{
  E2E_PJ193976STATUS_OK            = 0x00,
  E2E_PJ193976STATUS_NONEWDATA     = 0x01,
  E2E_PJ193976STATUS_ERROR         = 0x07,
  E2E_PJ193976STATUS_REPEATED      = 0x08,
  E2E_PJ193976STATUS_OKSOMELOST    = 0x20,
  E2E_PJ193976STATUS_WRONGSEQUENCE = 0x40
} E2E_PJ193976CheckStatusType; /*!< Enum containing all possible statuses */

typedef struct
{
  E2E_PJ193976CheckStatusType Status; /*!< Result of verification of the Data in this cycle */
  uint16 Counter;                /*!< Counter of the Data in previous cycle */
} E2E_PJ193976CheckStateType;

/**********************************************************************************************************************
 *  Typedefs of structures, cannot be used before the declaration of structs
 *********************************************************************************************************************/
typedef P2CONST(E2E_PJ193976ConfigType, AUTOMATIC, E2E_APPL_CONST) E2E_PJ193976ConstConfigPtrType;
typedef P2VAR(E2E_PJ193976ProtectStateType, AUTOMATIC, E2E_APPL_DATA) E2E_PJ193976ProtectStatePtrType;
typedef P2VAR(E2E_PJ193976CheckStateType, AUTOMATIC, E2E_APPL_DATA) E2E_PJ193976CheckStatePtrType;
typedef P2CONST(E2E_PJ193976CheckStateType, AUTOMATIC, E2E_APPL_DATA) E2E_PJ193976ConstCheckStatePtrType;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#  define E2E_START_SEC_CODE
#  include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  E2E_PJ193976Protect()
 *********************************************************************************************************************/
/*!
 * \brief           Protection function of E2Elib Profile J193976
 * \details         Protects the array/buffer to be transmitted using the E2E Profile J193976. This includes checksum calculation,
 *                  handling of counter and Data ID.
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
FUNC(Std_ReturnType, E2E_CODE) E2E_PJ193976Protect(
    E2E_PJ193976ConstConfigPtrType ConfigPtr,
    E2E_PJ193976ProtectStatePtrType StatePtr,
    E2E_PJ193976DataPtrType DataPtr
);

/**********************************************************************************************************************
 *  E2E_PJ193976ProtectInit()
 *********************************************************************************************************************/
/*!
 * \brief           Protection initialization function of E2Elib Profile J193976
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
/* Set any element of the struct to mark the struct initialized. */
# define E2E_PJ193976ProtectInit(StatePtr) 0; (StatePtr)->Counter = 0

/**********************************************************************************************************************
 *  E2E_PJ193976Check()
 *********************************************************************************************************************/
/*!
 * \brief           Check function of E2Elib Profile J193976
 * \details         Checks the Data received using the E2E Profile J193976. This includes CRC calculation, handling of
 *                  the counter and Data ID.
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
FUNC(Std_ReturnType, E2E_CODE) E2E_PJ193976Check(
    E2E_PJ193976ConstConfigPtrType ConfigPtr,
    E2E_PJ193976CheckStatePtrType StatePtr,
    E2E_PJ193976ConstDataPtrType DataPtr
);

/**********************************************************************************************************************
 *  E2E_PJ193976CheckInit()
 *********************************************************************************************************************/
/*!
 * \brief           Check initialization function of E2Elib Profile J193976
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
/* Set any element of the struct to mark the struct initialized. */
# define E2E_PJ193976CheckInit(StatePtr) 0; (StatePtr)->Counter = 0

/**********************************************************************************************************************
 *  E2E_PJ193976MapStatusToSM()
 *********************************************************************************************************************/
/*!
 * \brief           State mapping function of E2Elib Profile J193976
 * \details         Maps the check status of Profile J193976 to a generic check status, which can be used by E2E state machine
 *                  check function. The E2E Profile J193976 delivers a more fine-grained status, but this is not relevant for
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
FUNC(E2E_PCheckStatusType, E2E_CODE) E2E_PJ193976MapStatusToSM(
    const Std_ReturnType CheckReturn, 
    const E2E_PJ193976CheckStatusType Status
);

#  define E2E_STOP_SEC_CODE
#  include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* E2E_PJ193976_H */

/**********************************************************************************************************************
 *  END OF FILE: E2E_PJ193976.h
 *********************************************************************************************************************/
