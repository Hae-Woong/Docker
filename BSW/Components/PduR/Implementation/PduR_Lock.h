/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         \file  PduR_Lock.h
 *         \unit  Lock
 *        \brief  Pdu Router Lock header file
 *      \details  This is the implementation of the MICROSAR Pdu Router module.
 *                The basic software module is based on the AUTOSAR PduR specification.
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
#if !defined (PDUR_LOCK_H)
# define PDUR_LOCK_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
/* \trace SPEC-2020230, SPEC-38402 */

# include "PduR_Types.h"
# include "PduR_Cfg.h"
# include "PduR_Lcfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define PDUR_START_SEC_CODE
# include "PduR_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */

# if (PDUR_SPINLOCKRAM == STD_ON)
/**********************************************************************************************************************
 * PduR_Lock_Init
 *********************************************************************************************************************/
/*! \brief       PduR Lock Ram Variable initialization function.
 *  \details     -
 *  \param[in]   memIdx  memory section index
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \warning     PduR_Lock_Init shall not pre-empt any PDUR function.
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Lock_Init(PduR_MemIdxType memIdx);
# endif

/**********************************************************************************************************************
* PduR_Lock_LockRoutingPathBySrcPdu
*********************************************************************************************************************/
/*! \brief       This API locks the corresponding configured lock mechanism.
 *  \details     For details see documentation at PduR_Lock_Lock.
 *  \param[in]   rmSrcRomIdx  internal PduRSrcPdu ID
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmSrcRomIdx < PduR_GetSizeOfRmSrcRom();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Lock_LockRoutingPathBySrcPdu(PduR_RmSrcRomIterType rmSrcRomIdx);

/**********************************************************************************************************************
* PduR_Lock_UnlockRoutingPathBySrcPdu
*********************************************************************************************************************/
/*! \brief       This API unlocks the corresponding configured lock mechanism.
 *  \details     For details see documentation at PduR_Lock_Unlock.
 *  \param[in]   rmSrcRomIdx  internal PduRSrcPdu ID
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmSrcRomIdx < PduR_GetSizeOfRmSrcRom();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Lock_UnlockRoutingPathBySrcPdu(PduR_RmSrcRomIterType rmSrcRomIdx);

/**********************************************************************************************************************
* PduR_Lock_LockRoutingPathByDestPdu
*********************************************************************************************************************/
/*! \brief       This API locks the corresponding configured lock mechanism.
 *  \details     For details see documentation at PduR_Lock_Lock.
 *  \param[in]   rmDestRomIdx  internal PduRDestPdu ID
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmDestRomIdx < PduR_GetSizeOfRmDestRom();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Lock_LockRoutingPathByDestPdu(PduR_RmDestRomIterType rmDestRomIdx);

/**********************************************************************************************************************
* PduR_Lock_UnlockRoutingPathByDestPdu
*********************************************************************************************************************/
/*! \brief       This API unlocks the corresponding configured lock mechanism.
 *  \details     For details see documentation at PduR_Lock_Unlock.
 *  \param[in]   rmDestRomIdx  internal PduRDestPdu ID
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmDestRomIdx < PduR_GetSizeOfRmDestRom();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Lock_UnlockRoutingPathByDestPdu(PduR_RmDestRomIterType rmDestRomIdx);

/**********************************************************************************************************************
* PduR_Lock_LockRoutingPathByGDestPdu
*********************************************************************************************************************/
/*! \brief       This API locks the corresponding configured lock mechanism.
 *  \details     For details see documentation at PduR_Lock_Lock.
 *  \param[in]   rmGDestRomIdx  internal global destination ID
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmGDestRomIdx < PduR_GetSizeOfRmGDestRom();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Lock_LockRoutingPathByGDestPdu(PduR_RmGDestRomIterType rmGDestRomIdx);

/**********************************************************************************************************************
* PduR_Lock_UnlockRoutingPathByGDestPdu
*********************************************************************************************************************/
/*! \brief       This API unlocks the corresponding configured lock mechanism.
 *  \details     For details see documentation at PduR_Lock_Unlock.
 *  \param[in]   rmGDestRomIdx  internal global destination ID
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmGDestRomIdx < PduR_GetSizeOfRmGDestRom();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Lock_UnlockRoutingPathByGDestPdu(PduR_RmGDestRomIterType rmGDestRomIdx);

# if (PDUR_FMFIFORAM == STD_ON)
/**********************************************************************************************************************
* PduR_Lock_LockBuffersByFmFifo
*********************************************************************************************************************/
/*! \brief       This API locks the corresponding configured lock mechanism.
 *  \details     For details see documentation at PduR_Lock_Lock.
 *  \param[in]   fmFifoRomIdx  FIFO ID
 *  \param[in]   memIdx  memory section index
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires fmFifoRomIdx < PduR_GetSizeOfFmFifoRom(memIdx);
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Lock_LockBuffersByFmFifo(PduR_FmFifoRomIterType fmFifoRomIdx, PduR_MemIdxType memIdx);
# endif

# if (PDUR_FMFIFORAM == STD_ON)
/**********************************************************************************************************************
* PduR_Lock_UnlockBuffersByFmFifo
*********************************************************************************************************************/
/*! \brief       This API locks the corresponding configured lock mechanism.
 *  \details     For details see documentation at PduR_Lock_Unlock.
 *  \param[in]   fmFifoRomIdx  FIFO ID
 *  \param[in]   memIdx  memory section index
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires fmFifoRomIdx < PduR_GetSizeOfFmFifoRom(memIdx);
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Lock_UnlockBuffersByFmFifo(PduR_FmFifoRomIterType fmFifoRomIdx, PduR_MemIdxType memIdx);
# endif

#if (PDUR_SPINLOCKRAM == STD_ON)
/**********************************************************************************************************************
 * Appl_TryToGetSpinlock
 *********************************************************************************************************************/
/*! \brief          Application specific function which gets the lock on the provided variable.
 *  \details        The lock state of the passed variable must be checked and if the variable is not locked,
 *                  the lock must be acquired using an atomic operation.
 *                  If the lock was already acquired (e.g. by another core) E_NOT_OK must be returned.
 *  \param[in,out]  lockVar  pointer to the variable which shall be used for the lock.
 *  \return         E_OK     the lock was acquired
 *                  E_NOT_OK the lock could not be acquired
 *  \pre            -
 *  \context        TASK|ISR1|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) Appl_TryToGetSpinlock(P2VAR(uint32, AUTOMATIC, PDUR_APPL_DATA) lockVar);
#endif

#if (PDUR_SPINLOCKRAM == STD_ON)
/**********************************************************************************************************************
 * Appl_ReleaseSpinlock
 *********************************************************************************************************************/
/*! \brief          Application specific function which releases the lock on the provided variable.
 *  \details        Set the passed variable to the unlocked state.
 *                  E_OK is returned if the lock was successfully released.
 *                  Otherwise E_NOT_OK should be returned.
 *  \param[in,out]  lockVar  pointer to the variable which shall be used for the lock.
 *  \return         E_OK     the lock was released
 *                  E_NOT_OK the lock could not be released
 *  \pre            -
 *  \context        TASK|ISR1|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) Appl_ReleaseSpinlock(P2VAR(uint32, AUTOMATIC, PDUR_APPL_DATA) lockVar);
#endif

#if (PDUR_SPINLOCKRAM == STD_ON)
/**********************************************************************************************************************
 * Appl_GetSpinlockInitVal
 *********************************************************************************************************************/
/*! \brief          Application specific function which gets the init value for the lock variable.
 *  \details        The return value of this function will be used to set the inital value in the PduR.
 *  \return         Init value
 *  \pre            -
 *  \context        TASK|ISR1|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(uint32, PDUR_CODE) Appl_GetSpinlockInitVal(void);
#endif

# define PDUR_STOP_SEC_CODE
# include "PduR_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */
#endif /* PDUR_LOCK_H */
/**********************************************************************************************************************
 *  END OF FILE: PduR_Lock.h
 *********************************************************************************************************************/
