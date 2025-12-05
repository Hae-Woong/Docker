/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH.                                                All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*      \file   Crypto_30_LibCv_KeyExchange.h
 *      \brief  MICROSAR Crypto Driver (Crypto)
 *
 *      \details  Dispatcher for Key Exchange Services.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#ifndef CRYPTO_30_LIBCV_KEYEXCHANGE_H
# define CRYPTO_30_LIBCV_KEYEXCHANGE_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Crypto_30_LibCv_Cfg.h"

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
# define CRYPTO_30_LIBCV_KEY_EXCHANGE_SIZEOF_ALGORITHM                (1u)

  /* ECDHE */
# define CRYPTO_30_LIBCV_ECDHE_256_ID                                 (1u)
# define CRYPTO_30_LIBCV_ECDHE_384_ID                                 (2u)
# define CRYPTO_30_LIBCV_ECDHE_521_ID                                 (3u)

  /* ECBD max key size */
# if(CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_P256R1_BD_ENABLED == STD_ON)
#  define CRYPTO_30_LIBCV_MAX_SIZEOF_PRIVATE_ECBD_KEY                 (CRYPTO_30_LIBCV_SIZEOF_ECC_256_KEY_PRIVATE)
#  define CRYPTO_30_LIBCV_MAX_SIZEOF_PUBLIC_ECBD_KEY                  (CRYPTO_30_LIBCV_SIZEOF_ECC_256_KEY_PUBLIC)
#  define CRYPTO_30_LIBCV_MAX_SIZEOF_PUBLIC_ECBD_KEY_XY               (CRYPTO_30_LIBCV_SIZEOF_ECC_256_KEY_PUBLIC_XY)
# elif(CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_NISTP224R1_BD_ENABLED == STD_ON)
#  define CRYPTO_30_LIBCV_MAX_SIZEOF_PRIVATE_ECBD_KEY                 (CRYPTO_30_LIBCV_SIZEOF_ECC_224_KEY_PRIVATE)
#  define CRYPTO_30_LIBCV_MAX_SIZEOF_PUBLIC_ECBD_KEY                  (CRYPTO_30_LIBCV_SIZEOF_ECC_224_KEY_PUBLIC)
#  define CRYPTO_30_LIBCV_MAX_SIZEOF_PUBLIC_ECBD_KEY_XY               (CRYPTO_30_LIBCV_SIZEOF_ECC_224_KEY_PUBLIC_XY)
# endif

  /* Key Exchange ECBD */
# if ((CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_NISTP224R1_BD_ENABLED == STD_ON) || (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_P256R1_BD_ENABLED == STD_ON))
#  define CRYPTO_30_LIBCV_KEY_EXCHANGE_ECBD_ENABLED                   STD_ON
# else
#  define CRYPTO_30_LIBCV_KEY_EXCHANGE_ECBD_ENABLED                   STD_OFF
# endif

  /* Key Exchange Generic */
# if ((CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_ANSIP256R1_ENABLED == STD_ON) || (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_SECP256R1_ENABLED == STD_ON) \
     || (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_SECP384R1_ENABLED == STD_ON) || (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_SECP521R1_ENABLED == STD_ON))
#  define CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_GENERIC_ENABLED      STD_ON
# else
#  define CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_GENERIC_ENABLED      STD_OFF
# endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS PROTOTYPES
 *********************************************************************************************************************/

# define CRYPTO_30_LIBCV_START_SEC_CODE
# include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# if (CRYPTO_30_LIBCV_KEYX25519SECRET == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_KeyX25519Secret()
 *********************************************************************************************************************/
/*! \brief          Dispatches the KeyX25519Secret job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \param[in]      mode               Operation mode in which the job currently is.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a valid job object.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyX25519Secret(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* CRYPTO_30_LIBCV_KEYX25519SECRET */

# if (CRYPTO_30_LIBCV_KEYX448SECRET == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_KeyX448Secret()
 *********************************************************************************************************************/
/*! \brief          Dispatches the KeyX448Secret job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \param[in]      mode               Operation mode in which the job currently is.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a valid job object.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyX448Secret(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* CRYPTO_30_LIBCV_KEYX448SECRET */

# if (CRYPTO_30_LIBCV_KEYP256R1SECRET == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_KeyP256R1Secret()
 *********************************************************************************************************************/
/*! \brief          Dispatches the KeyP256R1Secret job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \param[in]      mode               Operation mode in which the job currently is.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a valid job object.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyP256R1Secret(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* CRYPTO_30_LIBCV_KEYP256R1SECRET */

# if (CRYPTO_30_LIBCV_KEYP384R1SECRET == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_KeyP384R1Secret()
 *********************************************************************************************************************/
/*! \brief          Dispatches the KeyP384R1Secret job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \param[in]      mode               Operation mode in which the job currently is.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a valid job object.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyP384R1Secret(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* CRYPTO_30_LIBCV_KEYP384R1SECRET */

# if (CRYPTO_30_LIBCV_KEYP521R1SECRET == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_KeyP521R1Secret()
 *********************************************************************************************************************/
/*! \brief          Dispatches the KeyP521R1Secret job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \param[in]      mode               Operation mode in which the job currently is.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a valid job object.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyP521R1Secret(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* CRYPTO_30_LIBCV_KEYP521R1SECRET */

# if (CRYPTO_30_LIBCV_KEYSPAKE2PSECRET == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_KeySpake2PSecret()
 *********************************************************************************************************************/
/*! \brief          Dispatches the Key Spake2+ Secret job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \param[in]      mode               Operation mode in which the job currently is.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a valid job object.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace          DSGN-DrvCrypto_Libcv_Spake2+
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeySpake2PSecret(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* CRYPTO_30_LIBCV_KEYSPAKE2PSECRET */

# if (CRYPTO_30_LIBCV_KEYP224R1BDSECRET == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_KeyP224R1BDSecret()
 *********************************************************************************************************************/
/*! \brief          Dispatches the Key P224R1 BD Secret job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \param[in]      mode               Operation mode in which the job currently is.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a valid job object.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace          DSGN-DrvCrypto_Libcv_ECBD
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyP224R1BDSecret(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* CRYPTO_30_LIBCV_KEYP224R1BDSECRET */

# if (CRYPTO_30_LIBCV_KEYP256R1BDSECRET == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_KeyP256R1BDSecret()
 *********************************************************************************************************************/
/*! \brief          Dispatches the Key P256R1 BD Secret job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \param[in]      mode               Operation mode in which the job currently is.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a valid job object.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace          DSGN-DrvCrypto_Libcv_ECBD
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyP256R1BDSecret(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* CRYPTO_30_LIBCV_KEYP256R1BDSECRET */

# if (CRYPTO_30_LIBCV_KEYX25519PUBVAL == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_KeyX25519PubVal()
 *********************************************************************************************************************/
/*! \brief          Dispatches the KeyX25519PubVal job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \param[in]      mode               Operation mode in which the job currently is.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a valid job object.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyX25519PubVal(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* CRYPTO_30_LIBCV_KEYX25519PUBVAL */

# if (CRYPTO_30_LIBCV_KEYX448PUBVAL == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_KeyX448PubVal()
 *********************************************************************************************************************/
/*! \brief          Dispatches the KeyX448PubVal job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \param[in]      mode               Operation mode in which the job currently is.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a valid job object.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyX448PubVal(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* CRYPTO_30_LIBCV_KEYX448PUBVAL */

# if (CRYPTO_30_LIBCV_KEYP256R1PUBVAL == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_KeyP256R1PubVal()
 *********************************************************************************************************************/
/*! \brief          Dispatches the KeyP256R1PubVal job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \param[in]      mode               Operation mode in which the job currently is.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a valid job object.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyP256R1PubVal(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* CRYPTO_30_LIBCV_KEYP256R1PUBVAL */

# if (CRYPTO_30_LIBCV_KEYP384R1PUBVAL == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_KeyP384R1PubVal()
 *********************************************************************************************************************/
/*! \brief          Dispatches the KeyP384R1PubVal job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \param[in]      mode               Operation mode in which the job currently is.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a valid job object.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyP384R1PubVal(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* CRYPTO_30_LIBCV_KEYP384R1PUBVAL */

# if (CRYPTO_30_LIBCV_KEYP224R1BDPUBVAL == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_KeyP224R1BDPubVal()
 *********************************************************************************************************************/
/*! \brief          Dispatches the Key P224R1 BD PubVal job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \param[in]      mode               Operation mode in which the job currently is.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a valid job object.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace          DSGN-DrvCrypto_Libcv_ECBD
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyP224R1BDPubVal(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* CRYPTO_30_LIBCV_KEYP224R1BDPUBVAL */

# if (CRYPTO_30_LIBCV_KEYP256R1BDPUBVAL == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_KeyP256R1BDPubVal()
 *********************************************************************************************************************/
/*! \brief          Dispatches the Key P256R1 BD PubVal job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \param[in]      mode               Operation mode in which the job currently is.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a valid job object.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace          DSGN-DrvCrypto_Libcv_ECBD
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyP256R1BDPubVal(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* CRYPTO_30_LIBCV_KEYP256R1BDPUBVAL */

# if (CRYPTO_30_LIBCV_KEYP521R1PUBVAL == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_KeyP521R1PubVal()
 *********************************************************************************************************************/
/*! \brief          Dispatches the KeyP521R1PubVal job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \param[in]      mode               Operation mode in which the job currently is.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a valid job object.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyP521R1PubVal(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* CRYPTO_30_LIBCV_KEYP521R1PUBVAL */

# if (CRYPTO_30_LIBCV_KEYSPAKE2PPUBVAL == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_KeySpake2PPubVal()
 *********************************************************************************************************************/
/*! \brief          Dispatches the Key Spake2+ PubVal job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \param[in]      mode               Operation mode in which the job currently is.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a valid job object.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace          DSGN-DrvCrypto_Libcv_Spake2+
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeySpake2PPubVal(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* CRYPTO_30_LIBCV_KEYSPAKE2PPUBVAL */

# if (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyExchangeCalcPubVal()
 *********************************************************************************************************************/
/*! \brief         Dispatches the public value calculation.
 *  \details       Dispatches the public value calculation dependent on the given algorithm.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key which shall be used for the key exchange
 *                                         protocol.
 *  \param[out]    publicValuePtr          Contains the pointer to the data where the public value shall be stored.
 *  \param[in,out] publicValueLengthPtr    Holds a pointer to the memory location in which the public value length
 *                                         information is stored. On calling this function, this parameter shall
 *                                         contain the size of the buffer provided by publicValuePtr. When the request
 *                                         has finished, the actual length of the returned value shall be stored.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *                 CRYPTO_E_SMALL_BUFFER   Request failed, the provided buffer is too small to store the result.
 *  \pre           cryptoKeyId has to be a valid crypto key.
 *                 publicValuePtr has to be a valid pointer.
 *                 publicValueLengthPtr has to be a valid pointer.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcPubVal(
  uint32 cryptoKeyId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValuePtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValueLengthPtr);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyExchangeCalcSecret()
 *********************************************************************************************************************/
/*! \brief         Dispatch the shared secret calculation.
 *  \details       Dispatch the shared secret calculation depending on the given algorithm.
 *  \param[in]     cryptoKeyId              Holds the identifier of the key which shall be used for the key exchange
 *                                          protocol.
 *  \param[in]     partnerPublicValuePtr    Holds the pointer to the memory location which contains the partners
 *                                          public value.
 *  \param[in]     partnerPublicValueLength Holds the length of th partner public value.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *  \pre           cryptoKeyId has to be a valid crypto key.
 *                 partnerPublicValuePtr has to be a valid pointer.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcSecret(
  uint32 cryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) partnerPublicValuePtr,
  uint32 partnerPublicValueLength);
# endif /* (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM == STD_ON) */

# if ((CRYPTO_30_LIBCV_KDF_ALGO_NIST_800_56_A_ONE_PASS_C1E1S_SINGLE_STEP_KDF_SHA256_ENABLED == STD_ON)\
     || (CRYPTO_30_LIBCV_KDF_ALGO_ISO_15118_CERTIFICATE_HANDLING_ENABLED == STD_ON)\
     || (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_GENERIC_ENABLED == STD_ON)\
     || (CRYPTO_30_LIBCV_KDF_ALGO_ISO_15118_20_CERTIFICATE_HANDLING_ENABLED == STD_ON))
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_Dispatch_EcP_CalculateSharedSecret_With_Ws()
 *********************************************************************************************************************/
/*! \brief          Calculate shared secret
 *  \details        Calculate shared secret for ECDHE Prime for the given values.
 *  \param[in]      privateKeyPtr           Pointer to the private key array.
 *  \param[in]      privateKeyLength        Length of the private key array.
 *  \param[in]      partnerPubKeyPtr        Pointer to the partner public key array.
 *  \param[in]      partnerPubKeyLength     Length of the partner public key array.
 *  \param[out]     sharedSecretPtr         Pointer to the shared secret.
 *  \param[in]      keaId                   Key exchange algorithm identifier.
 *  \param[in,out]  workspace               Contains workspace.
 *  \return         E_OK                    Request successful.
 *                  E_NOT_OK                Request failed.
 *  \pre            privateKeyPtr has to be a valid pointer with the length privateKeyLength.
 *                  partnerPubKeyPtr has to be a valid pointer with the length partnerPubKeyLength.
 *                  sharedSecretPtr has to be a valid pointer with the length 2*esl_getLengthOfEcPsecret_comp(EcDomainPtr).
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Dispatch_EcP_CalculateSharedSecret_With_Ws(
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) privateKeyPtr,
  uint32 privateKeyLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) partnerPubKeyPtr,
  uint32 partnerPubKeyLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) sharedSecretPtr,
  uint8 keaId,
  P2VAR(eslt_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace);

# endif /* ((CRYPTO_30_LIBCV_KDF_ALGO_NIST_800_56_A_ONE_PASS_C1E1S_SINGLE_STEP_KDF_SHA256_ENABLED == STD_ON)\
              || (CRYPTO_30_LIBCV_KDF_ALGO_ISO_15118_CERTIFICATE_HANDLING_ENABLED == STD_ON)\
              || (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_GENERIC_ENABLED == STD_ON)) */

# if ((CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_X448_ENABLED == STD_ON)\
     || (CRYPTO_30_LIBCV_KDF_ALGO_ISO_15118_20_CERTIFICATE_HANDLING_ENABLED == STD_ON))
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_Dispatch_EdDH_CalculateSharedSecret_With_Ws()
 *********************************************************************************************************************/
/*! \brief         Calculation of the secret
 *  \details       Calculates the shared secret key for the key exchange with the key material of the key identified
 *                 by the cryptoKeyId and the partner public key. The shared secret key is stored
 *                 as a key element in the same key. The write access right for the destination key element
 *                 must be less than or equal to WA_INTERNAL_COPY.
 *  \param[in]     privateKeyPtr           Holds the private key which shall be used for the key exchange
 *                                         protocol.
 *  \param[in]     privateKeyLength        The length of the private key which shall be used for the key exchange
 *                                         protocol.
 *  \param[in]     partnerPubKeyPtr        Holds the partner public key which shall be used for the key exchange
 *                                         protocol.
 *  \param[in]     partnerPubKeyLength     The length of the partner public key which shall be used for the key exchange
 *                                         protocol.
 *  \param[out]    sharedSecretPtr         Pointer to the memory location where the calculated shared secret should be stored.
 *  \param[in,out] sharedSecretLengthPtr   Pointer to the memory location where the number of available bytes for the
 *                                         shared secret is stored. The memory location will be overwritten by the actual
 *                                         stored length of the shared secret.
 *  \param[in,out] workspace               Contains the workspace.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Dispatch_EdDH_CalculateSharedSecret_With_Ws(
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) privateKeyPtr,
  uint32 privateKeyLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) partnerPubKeyPtr,
  uint32 partnerPubKeyLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) sharedSecretPtr,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) sharedSecretLengthPtr,
  P2VAR(eslt_WorkSpaceEdDH, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace);
# endif /* ((CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_X448_ENABLED == STD_ON)\
         || (CRYPTO_30_LIBCV_KDF_ALGO_ISO_15118_20_CERTIFICATE_HANDLING_ENABLED == STD_ON)) */

# define CRYPTO_30_LIBCV_STOP_SEC_CODE
# include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* CRYPTO_30_LIBCV_KEYEXCHANGE_H */
/**********************************************************************************************************************
 *  END OF FILE: Crypto_30_LibCv_KeyExchange.h
 *********************************************************************************************************************/
