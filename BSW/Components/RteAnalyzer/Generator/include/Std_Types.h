/**********************************************************************************************************************
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
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  Std_Types.h
 *    Component:  -
 *       Module:  -
 *    Generator:  -
 *
 *  Description:  Provision of Standard Types
 *  
 *  -------------------------------------------------------------------------------------------------------------------
 *  MISRA VIOLATIONS
 *  -------------------------------------------------------------------------------------------------------------------
 *      
 *  
 *********************************************************************************************************************/

#ifndef STD_TYPES_H
# define STD_TYPES_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

# include "Platform_Types.h"
# include "Compiler.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/


# define STD_TYPES_VENDOR_ID               (30u)  /* SREQ00015345, SREQ00015361 */
# define STD_TYPES_MODULE_ID               (197u) /* SREQ00015345, SREQ00015361 */

/* ESCAN00067740 */
# define STD_VENDOR_ID                     STD_TYPES_VENDOR_ID
# define STD_MODULE_ID                     STD_TYPES_MODULE_ID

# define COMMONASR__COMMON_IMPL_STDTYPES_VERSION 0x0305
# define COMMONASR__COMMON_IMPL_STDTYPES_RELEASE_VERSION 0x00

/* AUTOSAR Software Specification Version Information */
/* AUTOSAR release R21-11 -> 4.7.0 */
# define STD_TYPES_AR_RELEASE_MAJOR_VERSION       (4u)
# define STD_TYPES_AR_RELEASE_MINOR_VERSION       (7u)
# define STD_TYPES_AR_RELEASE_REVISION_VERSION    (0u)

/*ESCAN00067740 */
# define STD_AR_RELEASE_MAJOR_VERSION             STD_TYPES_AR_RELEASE_MAJOR_VERSION
# define STD_AR_RELEASE_MINOR_VERSION             STD_TYPES_AR_RELEASE_MINOR_VERSION
# define STD_AR_RELEASE_REVISION_VERSION          STD_TYPES_AR_RELEASE_REVISION_VERSION


/* Component Version Information */
# define STD_TYPES_SW_MAJOR_VERSION       (3u)
# define STD_TYPES_SW_MINOR_VERSION       (5u)
# define STD_TYPES_SW_PATCH_VERSION       (0u)

/* ESCAN00067740 */
# define STD_SW_MAJOR_VERSION             STD_TYPES_SW_MAJOR_VERSION
# define STD_SW_MINOR_VERSION             STD_TYPES_SW_MINOR_VERSION
# define STD_SW_PATCH_VERSION             STD_TYPES_SW_PATCH_VERSION

# define STD_HIGH     1u /* Physical state 5V or 3.3V */
# define STD_LOW      0u /* Physical state 0V */

# define STD_ACTIVE   1u /* Logical state active */
# define STD_IDLE     0u /* Logical state idle */

# define STD_ON       1u
# define STD_OFF      0u

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/* This typedef has been added for OSEK compliance */
# ifndef STATUSTYPEDEFINED
#  define STATUSTYPEDEFINED
#  define E_OK      0u
typedef unsigned char StatusType; /* OSEK compliance */
# endif

# define E_NOT_OK  1u

typedef uint8 Std_ReturnType;

typedef struct
{
   uint16 vendorID;
   uint16 moduleID;
   uint8  sw_major_version;
   uint8  sw_minor_version;
   uint8  sw_patch_version;
} Std_VersionInfoType;

typedef uint8 Std_TransformerErrorCode;

typedef uint8 Std_TransformerClass;
# define STD_TRANSFORMER_UNSPECIFIED 0u
# define STD_TRANSFORMER_SERIALIZER  1u
# define STD_TRANSFORMER_SAFETY      2u
# define STD_TRANSFORMER_SECURITY    3u
# define STD_TRANSFORMER_CUSTOM      0xFFu


typedef uint8 Std_MessageTypeType;
# define STD_MESSAGETYPE_REQUEST  0u
# define STD_MESSAGETYPE_RESPONSE 1u

typedef uint8 Std_MessageResultType ;
# define STD_MESSAGERESULT_OK 0u 
# define STD_MESSAGERESULT_ERROR 1u 

typedef struct
{
  Std_TransformerErrorCode errorCode;
  Std_TransformerClass     transformerClass;
} Std_TransformerError;


typedef Std_ReturnType (*Std_ExtractProtocolHeaderFieldsType) ( 
  const uint8* buffer, 
  uint32 bufferLength, 
  Std_MessageTypeType* messageType, 
  Std_MessageResultType* messageResult 
);

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

 
/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

 
#endif  /* STD_TYPES_H */

/**********************************************************************************************************************
 *  END OF FILE: Std_Types.h
 *********************************************************************************************************************/
