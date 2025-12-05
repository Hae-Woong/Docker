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
 *         \file  GmProt.h
 *        \brief  GmProt header file
 *
 *      \details  GmProt stub header.
 *
 *********************************************************************************************************************/


#if !defined (GMPROT_H)
# define GMPROT_H

# include "Std_Types.h"
# define E2EXF_LEGACY_STATEMACHINE STD_OFF
# include "E2E_P01.h"
# include "E2E_SM.h"

# define GMPROT_E_OK E2E_E_OK

typedef E2E_SMConfigType GmProt_SMConfigType;
typedef E2E_SMCheckStateType GmProt_SMCheckStateType;


typedef E2E_SMStatePtrType GmProt_SMStatePtrType;
typedef E2E_SMConfigPtrType GmProt_SMConfigPtrType;
typedef E2E_SMProfileStatusType GmProt_SMProfileStatusType;
typedef E2E_PCheckStatusType GmProt_PCheckStatusType;

FUNC (Std_ReturnType, E2E_CODE) GmProt_SMCheck (GmProt_SMProfileStatusType ProfileStatus,
                                             GmProt_SMConfigPtrType ConfigPtr,
                                             GmProt_SMStatePtrType StatePtr);

# define GmProt_SMCheckInit(StatePtr, ConfigPtr) 0; (StatePtr)->SMState = E2E_SM_NODATA


typedef E2E_P01ConfigType GmProt_P01ConfigType;
typedef E2E_P01ProtectStateType GmProt_P01ProtectStateType;
typedef E2E_P01CheckStateType GmProt_P01CheckStateType;
typedef E2E_P01CheckStatusType GmProt_P01CheckStatusType;

FUNC (Std_ReturnType, E2E_CODE) GmProt_P01Protect (P2CONST(GmProt_P01ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
                                                P2VAR(GmProt_P01ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
                                                E2E_BytePtrType DataPtr);


# define GmProt_P01ProtectInit(StatePtr) 0; (StatePtr)->Counter = 0


FUNC (Std_ReturnType, E2E_CODE) GmProt_P01Check (P2CONST(GmProt_P01ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
                                              P2VAR(GmProt_P01CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
                                              P2CONST (uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr);



# define GmProt_P01CheckInit(StatePtr) 0; (StatePtr)->LastValidCounter = 0


FUNC (GmProt_PCheckStatusType, E2E_CODE) GmProt_P01MapStatusToSM (VAR (Std_ReturnType, AUTOMATIC) CheckReturn,
                                                            VAR (GmProt_P01CheckStatusType, AUTOMATIC) Status,
                                                            VAR (boolean, AUTOMATIC) profileBehavior);










#endif /* GMPROT_H */

/**********************************************************************************************************************
 *  END OF FILE: GmProt.h
 *********************************************************************************************************************/
