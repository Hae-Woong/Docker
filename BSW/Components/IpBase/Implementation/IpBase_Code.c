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
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  IpBase_Code.c
 *         \unit  Code
 *        \brief  IpBase coding source file
 *      \details  IpBase encoding and decoding routines definition. 
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/
#define IPBASE_CODE_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "IpBase.h"           /* PRQA S 3453 3 */ /* MD_MSR_FctLikeMacro */
#include "IpBase_Priv.h"

/**********************************************************************************************************************
 *  CONSISTENCY CHECK
 *********************************************************************************************************************/
/* Check consistency of source and header file. */
#if ( (IPBASE_SW_MAJOR_VERSION != (4u)) \
   || (IPBASE_SW_MINOR_VERSION != (2u)) \
   || (IPBASE_SW_PATCH_VERSION != (1u)) )
# error "Vendor specific version numbers of IpBase_Code.c and IpBase.h are inconsistent"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
/* IPBASE encoding constants */
# define IPBASE_CODE_BASE64_EMPTY_CODE                    ((uint8) 64u)   /*!< Base64: empty digit */
# define IPBASE_CODE_BASE64_INV_CODE                      ((uint8) 65u)   /*!< Base64: invalid digit */
# define IPBASE_CODE_BASE64_LF_CHAR                       ((uint8) 0x0A)  /*!< Base64: line feed digit */
# define IPBASE_CODE_BASE64_CR_CHAR                       ((uint8) 0x0D)  /*!< Base64: carriage return digit */
# define IPBASE_CODE_BER_INFINITE_LEN                     ((uint8) 0u)    /*!< BER: infinite length encoding */
# define IPBASE_CODE_BER_MAX_LEN_U32                      ((uint8) 4u)    /*!< BER: max uint32 length encoding */

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
/* IPBASE encoding extension check */
# define IPBASE_BER_IS_EXTENDED_LEN(len)  (0x80u & (len))                                                               /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/
#define IPBASE_START_SEC_CONST_8
#include "IpBase_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */
  IPBASE_LOCAL IPBASE_CONSTCONST(uint8) IpBase_DecodingTab[] = {                                                        /* PRQA S 3218 */ /* MD_IpBase_3218 */
   65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, /* 000 - 009: */
   65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, /* 010 - 019: */
   65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, /* 020 - 029: */
   65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, /* 030 - 039: */
   65u, 65u, 65u, 62u, 65u, 65u, 65u, 63u, 52u, 53u, /* 040 - 049: +,/,0-1 */
   54u, 55u, 56u, 57u, 58u, 59u, 60u, 61u, 65u, 65u, /* 050 - 059: 2-9 */
   65u, 64u, 65u, 65u, 65u,  0u,  1u,  2u,  3u,  4u, /* 060 - 069: =,A-E */
    5u,  6u,  7u,  8u,  9u, 10u, 11u, 12u, 13u, 14u, /* 070 - 079: F-O */
   15u, 16u, 17u, 18u, 19u, 20u, 21u, 22u, 23u, 24u, /* 080 - 089: P-Y */
   25u, 65u, 65u, 65u, 65u, 65u, 65u, 26u, 27u, 28u, /* 090 - 099: Z, a-c */
   29u, 30u, 31u, 32u, 33u, 34u, 35u, 36u, 37u, 38u, /* 100 - 109: d-m */
   39u, 40u, 41u, 42u, 43u, 44u, 45u, 46u, 47u, 48u, /* 110 - 119: n-w */
   49u, 50u, 51u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, /* 120 - 129: x-z */
   65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, /* 130 - 139: */
   65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, /* 140 - 149: */
   65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, /* 150 - 159: */
   65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, /* 160 - 169: */
   65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, /* 170 - 179: */
   65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, /* 180 - 189: */
   65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, /* 190 - 199: */
   65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, /* 200 - 209: */
   65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, /* 210 - 219: */
   65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, /* 220 - 229: */
   65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, /* 230 - 239: */
   65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, 65u, /* 240 - 249: */
   65u, 65u, 65u, 65u, 65u, 65u                      /* 250 - 255: */
  };
#define IPBASE_STOP_SEC_CONST_8
#include "IpBase_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define IPBASE_START_SEC_CODE
# include "IpBase_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  IpBase_EncodeBase64Block
 *********************************************************************************************************************/
/*! \brief        Encodes data in base64
 *  \details      Encodes the given data block
 *  \param[out]   TgtDataPtr     pointer for the encoded data (min 4 bytes length)
 *  \param[in]    SrcDataPtr     pointer to the raw data (up to 3 bytes)
 *  \param[in]    SrcLenByte     raw data length in bytes (up to 3 bytes length)
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \trace        CREQ-120244
 *********************************************************************************************************************/
IPBASE_LOCAL IPBASE_FUNCCODE(void) IpBase_EncodeBase64Block(IPBASE_P2VARAPPLDATA(uint8) TgtDataPtr,
                                                            IPBASE_P2CONSTAPPLDATA(uint8) SrcDataPtr,
                                                            uint32 SrcLenByte);
/**********************************************************************************************************************
 *  IpBase_DecodeBase64BlockRead
 *********************************************************************************************************************/
/*! \brief        Decodes base64 encoded block
 *  \details      Decodes the given data block
 *  \param[out]    TmpDataPtr       pointer to the temporary encoded data (4 bytes)
 *  \param[in]     SrcDataPtr       pointer to the encoded data (min 4 bytes)
 *  \param[in,out] TgtLenBytePtr    in: pointer for target length in bytes, out: increased by decoded block length
 *  \param[in,out] SrcLenBytePtr    in: pointer for source data length in bytes, out: increased by encoded block length
 *  \param[in]     SrcRemainingLen  remaining length of source bytes
 *  \return        E_OK             data encoded
 *  \return        E_NOT_OK         data code not be encoded
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \trace        CREQ-120244
 *********************************************************************************************************************/
IPBASE_LOCAL IPBASE_FUNCCODE(Std_ReturnType) IpBase_DecodeBase64BlockRead(
                                               IPBASE_P2VARAPPLDATA(uint8) TmpDataPtr,
                                               IPBASE_P2CONSTAPPLDATA(uint8) SrcDataPtr,
                                               IPBASE_P2VARAPPLDATA(uint32) SrcLenBytePtr,
                                               uint32 SrcRemainingLen);
/**********************************************************************************************************************
 *  IpBase_DecodeBase64BlockWrite
 *********************************************************************************************************************/
/*! \brief        Decodes base64 encoded block
 *  \details      Decodes the given data block
 *  \param[out]    TgtDataPtr       pointer for the decoded data (min 3 bytes length)
 *  \param[in]     TmpDataPtr       pointer to the temporary encoded data (4 bytes)
 *  \param[in,out] TgtIdxPtr        in: pointer for current target Index, out: increased by decoded block length
 *  \param[in]     TgtLen           length of the target array
 *  \return        E_OK             decoded data written
 *  \return        E_NOT_OK         decoded data not written
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \trace        CREQ-120244
 *********************************************************************************************************************/
IPBASE_LOCAL IPBASE_FUNCCODE(Std_ReturnType) IpBase_DecodeBase64BlockWrite(
                                                IPBASE_P2VARAPPLDATA(uint8) TgtDataPtr,
                                                IPBASE_P2CONSTAPPLDATA(uint8) TmpDataPtr,
                                                IPBASE_P2VARAPPLDATA(uint32) TgtIdxPtr,
                                                uint32 TgtLen);
/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  IpBase_Encode
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
IPBASE_FUNCCODE(Std_ReturnType) IpBase_Encode(uint8 Code, IPBASE_P2VARAPPLDATA(uint8) TgtDataPtr,
                                              IPBASE_P2CONSTAPPLDATA(uint8) SrcDataPtr,
                                              IPBASE_P2VARAPPLDATA(uint32) TgtLenBytePtr,
                                              uint32 SrcLenByte)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = IPBASE_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check TgtDataPtr, SrcDataPtr, TgtLenBytePtr for NULL pointer */
  if ( (TgtDataPtr == NULL_PTR) || (SrcDataPtr == NULL_PTR) || (TgtLenBytePtr == NULL_PTR) )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Switch to requested encoding (limited to BASE64 encoding) */
    if ( Code == IPBASE_CODE_BASE64 )
    {
      uint32 srcByteIdx = 0u;
      uint32 tgtByteIdx = 0u;

      retVal = E_OK;
      while ( SrcLenByte > srcByteIdx ) /* FETA_IPBASE_CODE */
      { /* #30 Encode all chunks  */
        if ( (tgtByteIdx+IPBASE_CODE_BASE64_CODED_BLOCK_LEN) > (*TgtLenBytePtr) )
        { /* #40 Handle target buffer overrun */
          retVal = E_NOT_OK;
          break;
        }
        IpBase_EncodeBase64Block(&TgtDataPtr[tgtByteIdx], &SrcDataPtr[srcByteIdx],
                                 SrcLenByte - srcByteIdx); /* SBSW_IPBASE_PASSING_API_ENCODE_ARRAY */
        srcByteIdx += IPBASE_CODE_BASE64_RAW_BLOCK_LEN;
        tgtByteIdx += IPBASE_CODE_BASE64_CODED_BLOCK_LEN;
      }
      *TgtLenBytePtr = tgtByteIdx; /* SBSW_IPBASE_VALID_API_POINTER */
    }
    else
    { /* #50 Handle unknown encoding (ignore) */
    }
  }

  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #60 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_SID_ENCODE, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId);                                                                                      /* PRQA S 3112, 3199 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif

  return retVal;
} /* IpBase_Encode() */
/**********************************************************************************************************************
 *  IpBase_Decode
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
IPBASE_FUNCCODE(Std_ReturnType) IpBase_Decode(uint8 Code, IPBASE_P2VARAPPLDATA(uint8) TgtDataPtr,
                                              IPBASE_P2CONSTAPPLDATA(uint8) SrcDataPtr,
                                              IPBASE_P2VARAPPLDATA(uint32) TgtLenBytePtr,
                                              uint32 SrcLenByte)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = IPBASE_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check TgtDataPtr, SrcDataPtr, TgtLenBytePtr for NULL pointer */
  if ( (TgtDataPtr == NULL_PTR) || (SrcDataPtr == NULL_PTR) || (TgtLenBytePtr == NULL_PTR) )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Switch to requested decoding (limited to BASE64 decoding) */
    if ( Code == IPBASE_CODE_BASE64 )
    {
      uint32 srcByteIdx = 0u;
      uint32 tgtByteIdx = 0u;

      retVal = E_OK;
      while ( (SrcLenByte > srcByteIdx) && (*TgtLenBytePtr > tgtByteIdx) ) /* FETA_IPBASE_CODE */
      { /* #30 Decode all chunks  */
        uint8 tmpData[IPBASE_CODE_BASE64_CODED_BLOCK_LEN] = { 0u, 0u, 0u, 0u };

        retVal = IpBase_DecodeBase64BlockRead(&tmpData[0], &SrcDataPtr[srcByteIdx], &srcByteIdx, SrcLenByte - srcByteIdx); /* SBSW_IPBASE_PASSING_API_DECODE_ARRAY_READ */

        if(retVal == E_OK)
        {
          retVal = IpBase_DecodeBase64BlockWrite(&TgtDataPtr[tgtByteIdx], &tmpData[0],
                                                 &tgtByteIdx, *TgtLenBytePtr - tgtByteIdx); /* SBSW_IPBASE_PASSING_API_DECODE_ARRAY_WRITE */
        }

        if ( retVal != E_OK )
        { /* #40 Decode error */
          tgtByteIdx = 0u;
          break;
        }
      }
      *TgtLenBytePtr = tgtByteIdx; /* SBSW_IPBASE_VALID_API_POINTER */
    }
    else
    { /* #50 Unknown decoding (ignore) */
    }
  }

  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #60 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_SID_DECODE, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId);                                                                                      /* PRQA S 3112, 3199 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif

  return retVal;
} /* IpBase_Decode() */
/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  IpBase_EncodeBase64Block
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
IPBASE_LOCAL IPBASE_FUNCCODE(void) IpBase_EncodeBase64Block(IPBASE_P2VARAPPLDATA(uint8) TgtDataPtr,
                                                            IPBASE_P2CONSTAPPLDATA(uint8) SrcDataPtr,
                                                            uint32 SrcLenByte)
{
  /* ----- Local Variables ---------------------------------------------- */
  IPBASE_LOCAL IPBASE_CONSTCONST(char) encodingTab[] =
    {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', \
    'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', \
    's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/', '\0'};
  uint32 encodingTabIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Char0: Encode Byte0 6Bits, never '=' */
  TgtDataPtr[0] = (uint8) encodingTab[SrcDataPtr[0] >> 2]; /* SBSW_IPBASE_BASE64_ENCODED_BLOCK_LEN */
  /* #20 Char1: Encode Byte0 2Bits, Byte1 4 Bits, never '=' */
  /* PRQA S 4391 1 */ /* MD_IpBase_4391 */
  encodingTabIdx = (uint32)((SrcDataPtr[0] << 4) & IPBASE_CODE_BASE64_ENC_BYTE0_REM_2BITS_MASK);
  if ( SrcLenByte > 1u )
  {
    encodingTabIdx |= (uint32)(SrcDataPtr[1] >> 4);                                                                     /* PRQA S 4391 */ /* MD_IpBase_4391 */
  }
  TgtDataPtr[1] = (uint8) encodingTab[encodingTabIdx]; /* SBSW_IPBASE_BASE64_ENCODED_BLOCK_LEN */
  /* #30 Char2: Encode Byte1 4Bits, Byte2 2 Bits, maybe '=' */
  if ( SrcLenByte > 1u )
  {
    /* PRQA S 4391 1 */ /* MD_IpBase_4391 */
    encodingTabIdx = (uint32)((SrcDataPtr[1] << 2) & IPBASE_CODE_BASE64_ENC_BYTE1_REM_4BITS_MASK);
    if ( SrcLenByte > 2u )
    {
      encodingTabIdx |= (uint32)(SrcDataPtr[2] >> 6);                                                                   /* PRQA S 4391 */ /* MD_IpBase_4391 */
    }
    TgtDataPtr[2] = (uint8) encodingTab[encodingTabIdx]; /* SBSW_IPBASE_BASE64_ENCODED_BLOCK_LEN */
  }
  else
  {
    TgtDataPtr[2] = (uint8)'='; /* SBSW_IPBASE_BASE64_ENCODED_BLOCK_LEN */
  }
  /* #40 Char3: Encode Byte2 6Bits, maybe '=' */
  if ( SrcLenByte > 2u )
  {
    TgtDataPtr[3] = (uint8) encodingTab[SrcDataPtr[2] &
                                IPBASE_CODE_BASE64_ENC_BYTE2_REM_6BITS_MASK]; /* SBSW_IPBASE_BASE64_ENCODED_BLOCK_LEN */
  }
  else
  {
    TgtDataPtr[3] = (uint8)'='; /* SBSW_IPBASE_BASE64_ENCODED_BLOCK_LEN */
  }

} /* IpBase_EncodeBase64Block() */
/**********************************************************************************************************************
 *  IpBase_DecodeBase64BlockRead
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
IPBASE_LOCAL IPBASE_FUNCCODE(Std_ReturnType) IpBase_DecodeBase64BlockRead(
                                               IPBASE_P2VARAPPLDATA(uint8) TmpDataPtr,
                                               IPBASE_P2CONSTAPPLDATA(uint8) SrcDataPtr,
                                               IPBASE_P2VARAPPLDATA(uint32) SrcLenBytePtr,
                                               uint32 SrcRemainingLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  uint32 srcDataIdx = 0u;
  uint8 srcDataTmpIdx = 0u;

  /* ----- Implementation ----------------------------------------------- */
  while ( (srcDataTmpIdx < IPBASE_CODE_BASE64_CODED_BLOCK_LEN) && (retVal == E_OK) ) /* FETA_IPBASE_CODE */
  { /* #10 Decode all elements */
    if ( SrcRemainingLen <= srcDataIdx )
    { /* #20 Handle invalid padding */
      retVal = E_NOT_OK;

      /* configurable behavior?
         some specifications do not allow padding at the end
         (see RFC4648, 3.2. Padding of Encoded Data) */
    }
    else if ( (IPBASE_CODE_BASE64_LF_CHAR == SrcDataPtr[srcDataIdx]) ||
              (IPBASE_CODE_BASE64_CR_CHAR == SrcDataPtr[srcDataIdx]) )
    { /* #30 Ignore at least line feed and carriage return characters */
      srcDataIdx++;

      /* configurable behavior
         the Base64-encoded certificate from the Certificate Update Server contains line feed (0x0A) characters
         these characters are only allowed, if specification requires them
         (see RFC4648, 3.1. Line Feeds in Encoded Data) */
    }
    else
    { /* #40 Lookup decoded value */
      TmpDataPtr[srcDataTmpIdx] = IpBase_DecodingTab[SrcDataPtr[srcDataIdx]]; /* SBSW_IPBASE_VALID_API_POINTER */
      srcDataTmpIdx++;
      srcDataIdx++;
    }
  }

  if ( (IPBASE_CODE_BASE64_INV_CODE == TmpDataPtr[0u]) || (IPBASE_CODE_BASE64_INV_CODE == TmpDataPtr[1u]) ||
       (IPBASE_CODE_BASE64_INV_CODE == TmpDataPtr[2u]) || (IPBASE_CODE_BASE64_INV_CODE == TmpDataPtr[3u]) )
  { /* #50 Invalid character -> abort decoding
       (see RFC4648, 3.3. Interpretation of Non-Alphabet Characters in Encoded Data, 12. Security Considerations) */
    retVal =  E_NOT_OK;
  }
  else
  {
    *SrcLenBytePtr += srcDataIdx; /* SBSW_IPBASE_VALID_API_POINTER */
  }

  return retVal;
} /* IpBase_DecodeBase64BlockRead() */                                                                                  /* PRQA S 2006 */ /* MD_MSR 14.7 */

/**********************************************************************************************************************
 *  IpBase_DecodeBase64BlockWrite
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
IPBASE_LOCAL IPBASE_FUNCCODE(Std_ReturnType) IpBase_DecodeBase64BlockWrite(
                                              IPBASE_P2VARAPPLDATA(uint8) TgtDataPtr,
                                              IPBASE_P2CONSTAPPLDATA(uint8) TmpDataPtr,
                                              IPBASE_P2VARAPPLDATA(uint32) TgtIdxPtr,
                                              uint32 TgtLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Byte0: Decode Char0 6Bits, Char1 2Bits */
  TgtDataPtr[0u] = (uint8)(((TmpDataPtr[0u] << 2u) & IPBASE_CODE_BASE64_DEC_1ST_6BITS_MASK) |
                           ((TmpDataPtr[1u] >> 4u) &
                             IPBASE_CODE_BASE64_DEC_REM_2BITS_MASK)); /* SBSW_IPBASE_VALID_API_POINTER */
  (*TgtIdxPtr)++; /* SBSW_IPBASE_VALID_API_POINTER */

  /* #20 Byte1: Decode Char1 4Bits, Char2 4Bits */

  if ( IPBASE_CODE_BASE64_EMPTY_CODE != TmpDataPtr[2u] )
  {
    if(TgtLen > 1u)
    {
      (*TgtIdxPtr)++; /* SBSW_IPBASE_VALID_API_POINTER */
      TgtDataPtr[1u] = (uint8)(((TmpDataPtr[1u] << 4u) & IPBASE_CODE_BASE64_DEC_1ST_4BITS_MASK) |
                               ((TmpDataPtr[2u] >> 2u) &
                                 IPBASE_CODE_BASE64_DEC_REM_4BITS_MASK)); /* SBSW_IPBASE_VALID_API_POINTER */
    }
    else
    {
      retVal = E_NOT_OK;
    }
  }

  /* #30 Byte2: Decode Char2 2Bits, Char3 6Bits */

  if ( IPBASE_CODE_BASE64_EMPTY_CODE != TmpDataPtr[3u] )
  {
    if(TgtLen > 2u)
    {
      (*TgtIdxPtr)++; /* SBSW_IPBASE_VALID_API_POINTER */
      TgtDataPtr[2u] = (uint8)(((TmpDataPtr[2u] << 6u) & IPBASE_CODE_BASE64_DEC_1ST_2BITS_MASK) |
                               (TmpDataPtr[3u] &
                                IPBASE_CODE_BASE64_DEC_REM_6BITS_MASK)); /* SBSW_IPBASE_VALID_API_POINTER */
    }
    else
    {
      retVal = E_NOT_OK;
    }
  }

  return retVal;
} /* IpBase_DecodeBase64BlockWrite() */

# define IPBASE_STOP_SEC_CODE
# include "IpBase_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  END OF FILE: IpBase_Code.c
 *********************************************************************************************************************/
