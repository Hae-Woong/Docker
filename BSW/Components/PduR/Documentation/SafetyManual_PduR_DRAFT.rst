Safety Manual PduR
##################

Assumed safety requirements
***************************

This component assumes the following safety requirements:


.. list-table::
   :header-rows: 1

   * - ID 
     - Safety Requirement 
     - Associated TSR 
     - ASIL 

   * - CREQ-PduR-Initialization 
     - The PduR shall provide a service to initialize all PduR RAM variables, structures and memory.
     - TSR-1
     - ASIL D

   * - CREQ-PduR-RoutingPathGroup 
     - The PduR shall provide a service to activate or deactivate routings.
     - TSR-100551
     - ASIL D

   * - CREQ-PduR-IfRouting 
     - The PduR shall provide a service to route communication interface PDUs from one or multiple sources to one or multiple destinations.
     - TSR-111108
     - ASIL D

   * - CREQ-PduR-IfQueue 
     - The PduR shall provide a service to queue Interface PDUs.
     - TSR-111108
     - ASIL D

   * - CREQ-PduR-TpCancelReception 
     - The PduR shall provide a service to cancel the reception of a PDU.
     - TSR-111108
     - ASIL D

   * - CREQ-PduR-TpCancelTransmission 
     - The PduR shall provide a service to cancel the transmission of a PDU.
     - TSR-111108
     - ASIL D

   * - CREQ-PduR-TpChangeParameter 
     - The PduR shall provide a service to change transport protocol parameters.
     - TSR-111108
     - ASIL D

   * - CREQ-PduR-TpRouting 
     - The PduR shall provide a service to route transport protocol PDUs from one or multiple sources to one or multiple destinations.
     - TSR-111108
     - ASIL D

   * - CREQ-PduR-TpQueue 
     - The PduR shall provide a service to queue Tp PDUs.
     - TSR-111108
     - ASIL D

   * - CREQ-PduR-DeferredProcessing 
     - The PduR shall provide a main function for deferred processing.
     - TSR-111108
     - ASIL D

This component in addition to the assumed safety requirements ensures freedom from interference with respect to memory and timing with up to ASIL D.

Configuration constraints
*************************

.. smi:: SMI-109811

    The user of MICROSAR Safe shall configure and verify the following attributes:

    - Set */MICROSAR/PduR/PduRGeneral/PduR_QueueOverflowNotification* to FALSE.
    
    This setting is enforced by a MSSV plugin.
    
    This requirement is necessary to ensure freedom from interference with respect to timing by this component and is applicable if TSR-111108 is assumed as a safety requirement.


.. smi:: SMI-1077669

    The user of MICROSAR Safe shall configure and verify the following attributes:
    
    -    Set */MICROSAR/PduR/PduRGeneral/PduRExtendedErrorChecks* to TRUE.
    
    This setting is enforced by a MSSV plugin.
    
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


.. smi:: SMI-1077670

    The user of MICROSAR Safe shall configure and verify the following attributes:
    
    -    Set */MICROSAR/PduR/PduRGeneral/PduRPduCallback* to FALSE.
    
    This requirement is necessary to ensure freedom from interference with respect to timing by this component and is applicable if TSR-111108 is assumed as a safety requirement.


Additional verification measures
********************************

.. smi:: SMI-1078015

    The user of MICROSAR Safe shall ensure that the following statements are verified for routing paths with safety related availability requirements.
    
    Verify that the routing path is used in the same partition
    
    - The validation is performed using the configuration file.
    - Validate that the source pdu(s) (*/MICROSAR/PduR/PduRRoutingTables/PduRRoutingTable/PduRRoutingPath/PduRSrcPdu/PduRSrcPduRef/Pdu*) and destination pdu(s) (*/MICROSAR/PduR/PduRRoutingTables/PduRRoutingTable/PduRRoutingPath/PduRDestPdu/PduRDestPduRef/Pdu*) are using the same partition reference.
    
    This requirement is only applicable if TSR-111108 is assumed as a safety requirement.


.. smi:: SMI-1078016

    The user of MICROSAR Safe shall ensure that the following statements are verified for routing paths with safety related availability requirements.
       
    Verify that the routing path references an exclusive area.
    
    - The validation is performed using the generated files **PduR_<L|PB>cfg.c**.
    - Use the short name(s) of the source pdus(s) (*/MICROSAR/PduR/PduRRoutingTables/PduRRoutingTable/PduRRoutingPath/PduRSrcPdu*) to look up the **LockRomIdx** via the 'Referable Keys' in the **PduR_RmSrcRom** table.
    - Use the reference to the global destination (*/MICROSAR/PduR/PduRRoutingTables/PduRRoutingTable/PduRRoutingPath/PduRDestPdu/PduRDestPduRef*) to look up the **LockRomIdx** via the 'Referable Keys' in the **PduR_RmGDestRom** table.
    - The determined **LockRomIdx** should be the same for all source(s) and destination(s).
    
    - Validate that the **LockRomIdx** references an exclusive area. 
    - The entry in the **PduR_LockRom** based on the **LockRomIdx** should evaluate to:

        ExclusiveAreaRomIdx != PDUR_NO_EXCLUSIVEAREAROMIDXOFLOCKROM
        SpinlockRamIdx == PDUR_NO_SPINLOCKRAMIDXOFLOCKROM
        SpinlockRamPartitionIdx == PDUR_NO_SPINLOCKRAMPARTITIONIDXOFLOCKROM
    
    This requirement is only applicable if TSR-111108 is assumed as a safety requirement.


.. smi:: SMI-1077671

    The user of MICROSAR Safe shall ensure that the following statements are verified for routing paths with safety related availability requirements.
    
    Verify that the referenced queue is configured in the same partition.
    
    The queue short name can be extracted from the configuration based on the queue reference on destination level of the routing path with safety related availability requirements (*/MICROSAR/PduR/PduRRoutingTables/PduRRoutingTable/PduRRoutingPath/PduRDestPdu/PduRQueueRef*).
    
    Depending on the associated queue type the following validation steps have to be performed:
    
    **PduRSingleBuffer** (*/MICROSAR/PduR/PduRRoutingTables/PduRQueue/PduRSingleBuffer*)
    
    Verify that the assigned SingleBuffer is in the same partition.

    - The validation is performed using the partition specific generated files **PduR_<L|PB>cfg_<PartitionName>.c**.
    - The relevant SingleBuffer should be found by the short name of its referenced queue (*/MICROSAR/PduR/PduRRoutingTables/PduRQueue*) as 'Referable Keys' in the **PduR_SingleBufferRam<PartitionName>** and **PduR_SingleBufferArrayRam<PartitionName>** table of the partition specific configuration file.
    - The referenced queue short name should only be found in one specific partition configuration file.
    
    **PduRCommunicationInterfaceQueue** (*/MICROSAR/PduR/PduRRoutingTables/PduRQueue/PduRCommunicationInterfaceQueue*)
    
    Verify that the assigned communication interface queue is in the same partition.

    - The validation is performed using the partition specific generated files **PduR_<L|PB>cfg_<PartitionName>.c**.
    - The relevant PduRCommunicationInterfaceQueue should be found by the short name of its referenced queue (*/MICROSAR/PduR/PduRRoutingTables/PduRQueue*) as 'Referable Keys' in the **PduR_InterfaceFifoQueueRam<PartitionName>**, **PduR_InterfaceFifoQueueElementRam<PartitionName>** and  **PduR_InterfaceFifoQueueArrayRam<PartitionName>** table of the partition specific configuration file.
    - The referenced queue short name should only be found in one specific partition configuration file.
    
    **PduRSharedBufferQueue** (*/MICROSAR/PduR/PduRRoutingTables/PduRQueue/PduRSharedBufferQueue*)
    
    Verify that the assigned PduRSharedBufferQueue and its PduRTxBuffers are in the same partition.

    - The validation is performed using the partition specific generated files **PduR_<L|PB>cfg_<PartitionName>.c**.
    - The relevant PduRSharedBufferQueue should be found by the short name of its referenced queue (*/MICROSAR/PduR/PduRRoutingTables/PduRQueue*) as 'Referable Keys' in the **PduR_FmFifoRam<PartitionName>** and **PduR_FmFifoElementRam<PartitionName>** table of the partition specific configuration file.
    - The referenced queue short name should only be found in one specific partition configuration file.
    
    If the PduRSharedBufferQueue was mapped in the correct partition, the partition context of all associated explicit or implicit TxBuffer references have to be validated.
    
    - The validation is performed using the partition specific generated files **PduR_<L|PB>cfg_<PartitionName>.c**.
    - The relevant PduRTxBuffers should all be found by their short name (*/MICROSAR/PduR/PduRRoutingTables/PduRTxBufferTable/PduRTxBuffer*) as 'Referable Keys' in the **PduR_BmTxBufferRam<PartitionName>**, **PduR_BmTxBufferInstanceRam<PartitionName>** and **PduR_BmTxBufferArrayRam<PartitionName>** table of the partition specific configuration file.
    - The PduRTxBuffers short name should only be found in one specific partition configuration file.
    
    This requirement is only applicable if TSR-111108 is assumed as a safety requirement.


.. smi:: SMI-1085999

    The user of MICROSAR Safe shall ensure that the following statements are verified for routing paths with safety related availability requirements.
       
    Verify that a PduRSharedBufferQueue which is used by a routing path with safety related availability requirements does not reference a PduRLock.
    
    - The validation is performed using the generated files **PduR_<L|PB>cfg.c**.
    - Use the short name(s) of the shared buffer queue(s) parent container (*/MICROSAR/PduR/PduRRoutingTables/PduRQueue*) to look up the **LockRomIdx** via the 'Referable Keys' in the **PduR_FmFifoRom<PartitionName>** table.
    - Validate that the **LockRomIdx** is equal to PDUR_NO_LOCKROMIDXOFFMFIFOROM. 
    
    This requirement is only applicable if TSR-111108 is assumed as a safety requirement.


.. smi:: SMI-1077672

    The user of MICROSAR Safe shall ensure that the following statements are verified for routing paths with safety related availability requirements.
    
    Verify that the routing path solely referenced in routing path groups, which do not contain other multi partition routing paths or routing paths from other partitions. Verify that this routing path group is only enabled or disabled on the same partition as the routing path is assigned to.
    
    - The validation is performed using the partition specific generated files **PduR_<L|PB>cfg_<PartitionName>.c**.
    - The relevant routing path groups should all be found by their short name as 'Referable Keys' in the **PduR_RpgRam<PartitionName>** table of the partition specific configuration file.
    - The relevant routing path group short name should only be found in one specific partition configuration file.
    
    For all PduRDestPdus assigned to the corresponding routing path group, verify the following:

    - The validation is performed using the partition specific generated files **PduR_<L|PB>cfg_<PartitionName>.c**.
    - The relevant PduRDestPdus should all be found by their short name as 'Referable Keys' in the **PduR_RmDestRpgRam<PartitionName>** table of the partition specific configuration file.
    - The relevant PduRDestPdu short name should only be found in one specific partition configuration file.
    
    This requirement is only applicable if TSR-111108 is assumed as a safety requirement.


Safety requirements required from other components
**************************************************

.. smi:: SMI-1077674

    This component requires correct API functionality as an assumed safety requirement (TSR-100551, TSR-111108) from other interfacing components.
    
    This requirement is only applicable if TSR-100551 and TSR-111108 are assumed safety requirements.


For components provided as part of MICROSAR Safe, the respective section of the Safety Manual for that component allows you identifying if this requirement is fulfilled.

Dependencies to hardware
************************

This component does not use a direct hardware interface.

