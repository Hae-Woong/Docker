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
/*!        \file  Com_Util.h
 *         \unit  Util
 *        \brief  MICROSAR Communication header file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_UTIL_H)
# define COM_UTIL_H

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
# include "Com_Lcfg.h"

/**********************************************************************************************************************
  GLOBAL CONSTANT MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL FUNCTION MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL DATA PROTOTYPES (Com)
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL FUNCTION PROTOTYPES
**********************************************************************************************************************/
# define COM_START_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

/**********************************************************************************************************************
  Com_Util_CompareFloat32Values
**********************************************************************************************************************/
/*! \brief        This function compares two float32 values with each other.
    \details      -
    \pre          -
    \param[in]    firstValue                  first float32 value.
    \param[in]    scndValue                   second float32 value.
    \return       TRUE                        if difference is smaller than COM_FLT_EPSILON.
                  FALSE                       Otherwise.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_Util_CompareFloat32Values(float32 firstValue, float32 scndValue);

/**********************************************************************************************************************
  Com_Util_CompareFloat64Values
**********************************************************************************************************************/
/*! \brief        This function compares two float64 values with each other.
    \details      -
    \pre          -
    \param[in]    firstValue                  first float64 value.
    \param[in]    scndValue                   second float64 value.
    \return       TRUE                        if difference is smaller than COM_FLT_EPSILON.
                  FALSE                       Otherwise.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_Util_CompareFloat64Values(float64 firstValue, float64 scndValue);

/**********************************************************************************************************************
  Com_Util_CompareArrayBasedValues
**********************************************************************************************************************/
/*! \brief        This function compares two array based values with each other.
    \details      -
    \pre          The arrays need to have the same length. The length of the arrays needs to be the length of the length parameter.
    \param[in]    firstValue                  first pointer to array based value.
    \param[in]    scndValue                   second pointer to array based value.
    \param[in]    length                      length of the arrays to be compared
    \return       TRUE                        if the arrays are the same
                  FALSE                       Otherwise.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE
    \spec
      requires scndValue != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_Util_CompareArrayBasedValues(CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) firstValue, CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) scndValue, PduLengthType length);

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_UTIL_H */
/**********************************************************************************************************************
  END OF FILE: Com_Util.h
**********************************************************************************************************************/
