Safety Manual Crypto_30_LibCv
#############################

Assumed safety requirements
***************************

This component assumes the following safety requirements:


.. list-table::
   :header-rows: 1

   * - ID 
     - Safety Requirement 
     - Associated TSR 
     - ASIL 

   * - CREQ-158946 
     - Crypto shall provide a service to process a job for signature verification with SECp384r1.
     - TSR-10
     - ASIL B


Other than that, this component does not assume safety requirements. This component is developed according to ISO 26262 only to ensure freedom from interference with respect to memory and timing with up to ASIL D.

Configuration constraints
*************************

This component does not have configuration constraints.

Additional verification measures
********************************

.. smi:: SMI-471271

    The user of MICROSAR Safe shall ensure that Crypto_30_LibCv data, which are stored in one or more NvM blocks configured by */MICROSAR/Crypto_30_LibCv/Crypto/CryptoNvStorage/CryptoNvBlock*, are restored unchanged. The data integrity can be ensured e.g. by NvM using CRC.
    
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


.. smi:: SMI-722498

    The user of MICROSAR Safe shall ensure that the data which is exported and imported by save and restore context is unchanged. The option to save and restore context can be determined by /MICROSAR/Crypto_30_LibCv/Crypto/CryptoPrimitives/CryptoPrimitive/CryptoPrimitiveSupportContext.
    The imported context data is not checked for correctness. The data integrity can be ensured e.g. by using CRC.
    
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


.. smi:: SMI-863486

    The user of MICROSAR Safe shall verify that the buffer passed for reading and writing of a single NvM block is valid and sufficiently large for the passed block ID.
    Verification can be performed by a review of the generated configuration and the code passing the pointer and block ID to the Crypto_30_LibCv against the NvM block configuration.
    
    In order to identify the affected configuration, the user shall search Crypto_30_LibCv_Cfg.c for the table Crypto_30_LibCv_NvBlock. If no such array exists in Crypto_30_LibCv_Cfg.c, no further action is required. 
    If the table exists, the user shall ensure for every configured NvM Block:

    - The configured NvM block length matches the Length within Crypto_30_LibCv_NvBlock
    - The configured KeyStorageEndIdx minus KeyStorageStartIdx is less than or equal to the length in the Crypto_30_LibCv_NvBlock
    - The callout function Crypto_30_LibCv_NvBlock_WriteTo_CryptoNvBlock_<Name> calls Crypto_30_LibCv_NvBlock_WriteToBlock with the correct BlockIdx
    - Crypto_30_LibCv_NvBlock_ReadFrom_CryptoNvBlock_<Name> calls Crypto_30_LibCv_NvBlock_ReadFromBlock with the correct BlockIdx
    
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


Additional verification measures for ECDSA SECp384r1 signature verification job
===============================================================================

.. smi:: SMI-857456

    The user of MICROSAR Safe for the Signature verify ECDSA SEC P384R1 job must ensure that the Crypto_30_LibCv_ProcessJob API is only called with a job with valid parameters as described below:
    
    - The primitive service (job.primitiveInfo.service) must be set to CRYPTO_SIGNATUREVERIFY.
    - The algorithm family (job.primitiveInfo.algorithm.family) must be set to CRYPTO_ALGOFAM_CUSTOM_ECCSEC.
    - The secondary algorithm family (job.primitiveInfo.algorithm.secondaryFamily) must be set to one of the following values CRYPTO_ALGOFAM_NOT_SET, CRYPTO_ALGOFAM_SHA1, CRYPTO_ALGOFAM_SHA2_256, CRYPTO_ALGOFAM_SHA2_384, CRYPTO_ALGOFAM_SHA2_512.
    - The algorithm mode (job.jobPrimitiveInputOutput.mode) must be set to CRYPTO_ALGOMODE_CUSTOM_P384R1.
    - The processing type (job.jobPrimitiveInfo.processingType) must be set to CRYPTO_PROCESSING_ASYNC or CRYPTO_PROCESSING_SYNC.
    - A valid input pointer and length (job.jobPrimitiveInputOutput.inputPtr and job.jobPrimitiveInputOutput.inputLength, referencing the message and its length). Valid in this context means that the inputPtr must not be a null pointer and reference a buffer of the length inputLength.
    - A valid secondary input pointer and length (job.jobPrimitiveInputOutput.secondaryInputPtr and job.jobPrimitiveInputOutput.secondaryInputLength, referencing the signature and its length). Valid in this context means that the secondaryInputPtr must not be a null pointer and reference a buffer of the length secondaryInputLength. The secondaryInputLength must be even. The signature consists of two parts (R and S), which must be of equal size. 
    - A valid verify pointer (job.jobPrimitiveInputOutput.verifyPtr). Valid in this context means that the verifyPtr must not be a null pointer.
    - A valid operation mode (job.jobPrimitiveInputOutput.mode) of value CRYPTO_OPERATIONMODE_START, CRYPTO_OPERATIONMODE_UPDATE or CRYPTO_OPERATIONMODE_FINISH in this order or of value CRYPTO_OPERATIONMODE_SINGLECALL.
    
    This requirement is only applicable if TSR-10 is assumed as a safety requirement.


.. smi:: SMI-859692

    If the job mode for the ECDSA SEC P384R1 signature verification job is not SingleCall (job.jobPrimitiveInputOutput.mode != CRYPTO_OPERATIONMODE_SINGLECALL), the MICROSAR Safe user must ensure that the job is called in a start (job.jobPrimitiveInputOutput.mode = CRYPTO_OPERATIONMODE_START), update (job.jobPrimitiveInputOutput.mode = CRYPTO_OPERATIONMODE_UPDATE) and finish (job.jobPrimitiveInputOutput.mode = CRYPTO_OPERATIONMODE_FINISH) sequence. A start-update-start sequence, results in the workspace being emptied and a new job being started in its place. However, a multiple update call is allowed.
    
    This requirement is only applicable if TSR-10 is assumed as a safety requirement.


.. smi:: SMI-859693

    The MICROSAR Safe user for the ECDSA SEC P384R1 signature verification job must ensure that the function pointer is set to the correct primitive dispatch function, this requires checking the table Crypto_30_LibCv_PrimitiveInfo in Crypto_30_LibCv_Cfg.c. In the table should be checked if the primitive is set correctly and is pointing to the ECDSA SEC P384R1 entry (Crypto_30_LibCv_Dispatch_EcP384Verify) in the table Crypto_30_LibCv_PrimitiveFct in Crypto_30_LibCv_Cfg.c.
    
    This requirement is only applicable if TSR-10 is assumed as a safety requirement.


.. smi:: SMI-859995

    The MICROSAR Safe user for the ECDSA SEC P384R1 signature verification job must ensure that the job verification result pointer (job.jobPrimitiveInputOutput.verifyPtr) is set to CRYPTO_E_VER_NOT_OK before starting the job.
    
    This requirement is only applicable if TSR-10 is assumed as a safety requirement.


.. smi:: SMI-859996

    The MICROSAR Safe user is not allowed to use the save and restore mode for the primitive ECDSA SEC P384R1 signature verification job, i.e.
    /MICROSAR/Crypto_30_LibCv/Crypto/CryptoPrimitives/CryptoPrimitive/CryptoPrimitiveSupportContext for ECDSA SEC P384R1 primitive must be set to false
    and
    the API Crypto_30_LibCv_ProcessJob must not be called with job.jobPrimitiveInputOutput.mode = CRYPTO_OPERATIONMODE_SAVE_CONTEXT or
    job.jobPrimitiveInputOutput.mode = CRYPTO_OPERATIONMODE_RESTORE_CONTEXT for this primitive.
    
    This requirement is only applicable if TSR-10 is assumed as a safety requirement.


Safety requirements required from other components
**************************************************

Additional verification measures for ECDSA SECp384r1 signature verification job
===============================================================================

.. smi:: SMI-859696

    This component requires ECDSA SEC P384R1 signature verification functionality as an assumed safety requirement (TSR-10) from vSecPrim. 
    
    For the ECDSA SEC P384R1 signature verification vSecPrim shall only return E_OK, if the signature is valid.
    
    For vSecPrim provided as part of MICROSAR Safe, the respective section of the Safety Manual for vSecprim allows you identifying, if this requirement is fulfilled.
    
    This requirement is only applicable if TSR-10 is assumed as a safety requirement.


Dependencies to hardware
************************

This component does not use a direct hardware interface.

