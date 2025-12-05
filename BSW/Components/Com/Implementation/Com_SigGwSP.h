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
/*!        \file  Com_SigGwSP.h
 *         \unit  SigGwSP
 *        \brief  MICROSAR Communication header file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_SIGGWSP_H)
# define COM_SIGGWSP_H

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

# if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON) /* COV_COM_GATEWAY_MAINFUNCTION_ROUTESIGNALSAPI */

/**********************************************************************************************************************
  Com_SigGwSP_SignalGroupRouting
**********************************************************************************************************************/
/*! \brief        This function processes partition local signal group routings.
    \details      -
    \pre          -
    \param[in]    idxGwSigGrpMapping   Index of GwSigGrpMapping.
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxGwSigGrpMapping < Com_GetSizeOfGwSigGrpMapping();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_SigGwSP_SignalGroupRouting(Com_GwSigGrpMappingEndIdxOfGwInfoType idxGwSigGrpMapping);

/**********************************************************************************************************************
  Com_SigGwSP_SignalRouting
**********************************************************************************************************************/
/*! \brief        This function processes partition local signal routings.
    \details      -
    \pre          -
    \param[in]    idxGwSigMapping     Index of GwSigMapping.
    \param[in]    data                Pointer to the data
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxGwSigMapping < Com_GetSizeOfGwSigMapping();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_SigGwSP_SignalRouting(Com_GwSigMappingEndIdxOfGwInfoType idxGwSigMapping);

# endif

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_SIGGWSP_H */

/**********************************************************************************************************************
  END OF FILE: Com_SigGwSP.h
**********************************************************************************************************************/
