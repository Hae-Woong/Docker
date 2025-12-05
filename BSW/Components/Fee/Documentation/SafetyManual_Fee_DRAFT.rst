Safety Manual Fee
#################

Assumed safety requirements
***************************

This component does not assume safety requirements. This component is developed according to ISO 26262 only to ensure freedom from interference with respect to memory with up to ASIL D.

Configuration constraints
*************************

.. smi:: SMI-2553

    The user of MICROSAR Safe shall verify Fee's internal buffer size.
    
    The size is generated to macro *FEE_INTERNAL_BUFFER_SIZE*.
    The buffer size shall be at least the maximum of all configured partition's *AddressAlignment* values. Each partition is generated into the array *Fee_PartitionConfig_at*. Note that the alignments store the exponents, i.e. the power to which the value 2 shall be raised.
    
    Verification can be performed e.g. by review.


.. smi:: SMI-2595

    The user of MICROSAR Safe shall disable data conversion.
    
    Set */MICROSAR/Fee/FeeSpecificFeatures/FeeDataConversionApi* to *FALSE*.
    
    This setting is enforced by an MSSV plugin.


Additional verification measures
********************************

.. smi:: SMI-2594

    The user of MICROSAR Safe shall pass a valid pointer to a data buffer to Fee's read functions.
    The size of the data buffer must match the length provided in the read request, which is passed either explicitly (*Fee_Read*), or implicitly (i.e. one element of given data type) in *Fee_GetEraseCycle* and *Fee_GetWriteCycle*.
    
    If the NvM by Vector and in case *Fee_Read* is used, it is sufficient to verify that it is only called by the NvM (indirectly via *MemIf_Read*).
    
    Verification can be performed e.g. by review.


.. smi:: SMI-2554

    The user of MICROSAR Safe shall pass a valid Fee configuration pointer to *Fee_InitEx*.
    
    If *Fee_InitEx* is used instead of *Fee_Init* to initialize the component, the user has to make sure, the pointer points to a valid structure of correct type.
    
    Verification can be performed e.g. by review.


Safety requirements required from other components
**************************************************

This component does not require safety requirements from other components.

Dependencies to hardware
************************

This component does not use a direct hardware interface.

