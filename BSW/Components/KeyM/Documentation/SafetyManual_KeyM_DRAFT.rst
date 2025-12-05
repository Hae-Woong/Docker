Safety Manual KeyM
##################

Assumed safety requirements
***************************

This component does not assume safety requirements. This component is developed according to ISO 26262 only to ensure freedom from interference with respect to memory with up to ASIL D.

Configuration constraints
*************************

.. smi:: SMI-803893

    The user of MICROSAR Safe shall ensure that the configuration fulfills the following constraints:
    
    The container ``/MICROSAR/KeyM/KeyMCertificate/KeyMOCSP`` may not be instantiated.
    
    The container ``/MICROSAR/KeyM/KeyMCRE`` may only be instantiated if ``/MICROSAR/KeyM/KeyMPublishedInformation/KeyMRemoteCREEnabled`` is set to ``TRUE`` and the container ``/MICROSAR/KeyM/KeyMCRE/KeyMCRENvmBlock`` is NOT instantiated.
    
    The parameter ``/MICROSAR/KeyM/KeyMGeneral/KeyMRemotePreconfigDestination`` may not be instantiated.
    
    The parameter ``/MICROSAR/KeyM/KeyMGeneral/KeyMServiceCertificateFunctionEnabled`` must be set to ``TRUE``.
    
    The parameter ``/MICROSAR/KeyM/KeyMGeneral/KeyMCryptoKeyPrepareFunctionEnabled`` must be set to ``FALSE``.
    
    The parameter ``/MICROSAR/KeyM/KeyMGeneral/KeyMCryptoKeyStartFinalizeFunctionEnabled`` must be set to ``FALSE``.
    
    The parameter ``/MICROSAR/KeyM/KeyMGeneral/KeyMCryptoKeyVerifyFunctionEnabled`` must be set to ``FALSE``.
    
    These settings are enforced by an MSSV plugin.
    
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


Additional verification measures
********************************

.. smi:: SMI-787814

    The user of MICROSAR Safe shall ensure that the configured callout functions for certificate init values provide valid buffer pointers and lengths.
    
    In order to identify the affected callout functions, the user shall search KeyM_Cfg.c for the table KeyM_CertificateInitCallout. If no such array exists in KeyM_Cfg.c, no further action is required. If the table exists, the user shall ensure for every configured callout function:

    - The callout function shall correctly initialize the members of the passed KeyM_ConstCertDataType struct **CertificateDataPtr**:
      
      - The member **certData** shall be set to a valid (i.e., non-null) pointer to a buffer of uint8 elements. The buffer must be valid at all times while Security_AsrKeyM is running and its contents may not change.
      - The member **certDataLength** shall be set to a value equal to or less than the length of the buffer referenced by the member certData.
    
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


.. smi:: SMI-789741

    The user of MICROSAR Safe shall verify that the buffer passed for reading and writing of a single NvM block is valid and sufficiently large for the passed block ID.
    Verification can be performed by a  review of the generated configuration and the code passing the pointer and block ID to the KeyM against the NvM block configuration.
    
    In order to identify the affected configuration, the user shall search KeyM_Cfg.c for the table KeyM_NvmBlock. If no such array exists in KeyM_Cfg.c, no further action is required. If the table exists, the user shall ensure for every configured NvM certificate:

    - The configured NvM block length matches the Length within KeyM_NvmBlock
    - The callout function KeyM_NvBlock_WriteTo_<Certificate> calls KeyM_NvBlock_WriteToBlock with the correct BlockIdx
    - KeyM_NvBlock_ReadFrom_<Certificate> calls KeyM_NvBlock_ReadFromBlock with the correct BlockIdx
    
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


.. smi:: SMI-789742

    The user of MICROSAR Safe shall ensure that KeyM data, which are stored in one or more NvM blocks, are restored unchanged. The data integrity can be ensured e.g. by NvM using CRC.
    
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


.. smi:: SMI-803894

    The user of MICROSAR Safe shall ensure that only synchronous CSM jobs are configured in the following configuration parameters of KeyM:
    
    - ``/MICROSAR/KeyM/KeyMCertificate/KeyMCertCsmSignatureGenerateJobRef``
    - ``/MICROSAR/KeyM/KeyMCertificate/KeyMCertCsmHashJobRef``
    
    If ``/MICROSAR/KeyM/KeyMCertificate/KeyMCertificateStorage`` is set to ``KEYM_STORAGE_REMOTE`` for a configured ``KeyMCertificate``, the user of MICROSAR Safe shall additionally ensure that the CSM job referenced by that ``KeyMCertificate``'s ``/MICROSAR/KeyM/KeyMCertificate/KeyMCertCsmSignatureVerifyJobRef`` is a synchronous CSM job.
    
    The user shall verify this by checking this in the generated configuration files of the CSM.
    
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


Safety requirements required from other components
**************************************************

This component does not require safety requirements from other components.

Dependencies to hardware
************************

This component does not use a direct hardware interface.

