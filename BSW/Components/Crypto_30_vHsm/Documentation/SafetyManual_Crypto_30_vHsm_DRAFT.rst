Safety Manual Crypto_30_vHsm
############################

Assumed safety requirements
***************************

This component does not assume safety requirements. This component is developed according to ISO 26262 only to ensure freedom from interference with respect to memory with up to ASIL D.

This component does not assume safety requirements. It is developed according to ISO 26262 ASIL D only to ensure freedom from interference with respect to memory and timing.

Configuration constraints
*************************

The user of MICROSAR Safe shall configure the following attributes:

.. smi:: SMI-229862

    ``/MICROSAR/Crypto_30_vHsm/Crypto/CryptovHsm/CryptoMemoryArea/CryptoAreaType`` may only be set to the value CRYPTO_AREA_HSM_READ_ACCESS.
    
    ``/MICROSAR/Crypto_30_vHsm/Crypto/CryptoDriverObjects/CryptoDriverObject/CryptoBufferSize`` may only be set to values other than 0.
    
    These settings are enforced by a MSSV plugin.
    
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


.. smi:: SMI-427853

    The user of MICROSAR Safe shall verify that the value(s) of the generated register addresses match the addresses of the registers the driver is intended to use.
    
    - The value of the define ``CRYPTO_30_VHSM_HOST2_HSM_ADDRESS`` must match an address writeable for HOST and readable for HSM. This can be either a special hardware register  the driver intended to use or an 32 bit variable located in initialized shared memory.
    
    - The value of the define ``CRYPTO_30_VHSM_HSM2_HOST_ADDRESS`` must match an address readable for Host and writeable for HSM. This can be either a special hardware register the driver intended to use or an 32 bit variable located in initialized shared memory. 
    
    - If enabled in ``/MICROSAR/Crypto_30_vHsm/Crypto/CryptovHsm/CryptoJobRequestInterrupt/CryptoJobRequestInterruptFlagSetting``, the value of the define ``CRYPTO_30_VHSM_JOB_REQUEST_INTERRUPT_FLAG_SET_ADDRESS`` must match the register address to set the inter processor interrupt for notifying HSM about the request.
    
    - If enabled in ``/MICROSAR/Crypto_30_vHsm/Crypto/CryptovHsm/CryptoJobResponseInterrupt/CryptoJobResponseInterruptFlagClearing``, the value of the define ``CRYPTO_30_VHSM_JOB_RESPONSE_INTERRUPT_FLAG_CLEAR_ADDRESS`` must match the register address to clear an inter processor interrupt flag when getting a notification about repsonse from HSM via interrupt.
    
    All defines, ``CRYPTO_30_VHSM_HOST2_HSM_ADDRESS``, ``CRYPTO_30_VHSM_HSM2_HOST_ADDRESS``, ``CRYPTO_30_VHSM_JOB_REQUEST_INTERRUPT_FLAG_SET_ADDRESS`` and ``CRYPTO_30_VHSM_JOB_RESPONSE_INTERRUPT_FLAG_CLEAR_ADDRESS`` can be found in Crypto_30_vHsm_Cfg.h.
    
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


Additional verification measures
********************************

.. smi:: SMI-1093552

    The user of MICROSAR Safe shall ensure, that the HSM core can only modify allowed memory areas. This can be achieved with an MPU. Allowed memory areas on the host core is everything mapped to the following memory mapping sections: 

    - CRYPTO_30_VHSM_*_SEC_IPC_MEMORY_VAR_NOINIT_32BIT
    - CRYPTO_30_VHSM_*_SEC_GLOBAL_VAR_NOINIT_32BIT 
    - CRYPTO_30_VHSM_*_SEC_GLOBAL_VAR_NOINIT_UNSPECIFIED.
    
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


.. smi:: SMI-1102870

    The user of MICROSAR Safe shall ensure that an unresponsive HSM does not impact the timing behaviour of safety-critical tasks.
    
    This component can block due to an unresponsive HSM. Not blocking tasks, which have safety-related availability requirements allocated, can be achieved by mapping the dedicated tasks, which make use of the HSM to lower priorities.
    
    This requirement is necessary to ensure freedom from interference with respect to timing by this component.


Safety requirements required from other components
**************************************************

This component does not require safety requirements from other components.

Dependencies to hardware
************************

This component does not use a direct hardware interface.

