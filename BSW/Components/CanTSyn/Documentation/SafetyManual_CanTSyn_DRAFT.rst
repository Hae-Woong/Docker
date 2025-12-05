Safety Manual CanTSyn
#####################

Assumed safety requirements
***************************

.. smi:: SMI-271168

    This component assumes the following safety requirements:
    
    
    .. list-table::
       :header-rows: 1
       :widths: 25 45 20 10
    
       * - ID
         - Safety Requirement
         - Linked TSR
         - ASIL 

       * - CREQ-128008
         - CanTSyn shall provide a service to initialize itself.
         - TSR-1
         - ASIL D

       * - CREQ-126994
         - CanTSyn shall provide a service to trigger time base synchronization transmission and also handle transmission confirmation.
         - TSR-111109
         - ASIL D

       * - CREQ-129669
         - CanTSyn shall provide a service to trigger offset time base transmission and also handle transmission confirmation.
         - TSR-111109
         - ASIL D

       * - CREQ-126995
         - CanTSyn shall provide a service to provide a time base.
         - TSR-111109
         - ASIL D

       * - CREQ-129670
         - CanTSyn shall provide a service to provide time base offset.
         - TSR-111109
         - ASIL D

       * - CREQ-126996
         - CanTSyn shall provide a service to protect the time synchronization protocol
         - TSR-111109
         - ASIL D
    
    
    This component in addition to the assumed safety requirements ensures freedom from interference with respect to memory and timing with up to ASIL D.


Configuration constraints
*************************

.. smi:: SMI-271169

    The user of MICROSAR Safe shall enable CRC validation if time gateway is configured. 
    
    This setting is necessary to protect the SGW Bit in FUP, OFS and OFNS messages against corruption.
    For time masters this setting shall be configured and verified by:
    
    - Set /MICROSAR/CanTSyn/CanTSynGlobalTimeDomain/CanTSynGlobalTimeMaster/CanTSynGlobalTimeTxCrcSecured to CRC_SUPPORTED for all time masters of the same time domain (for the global time master and for all time gateways).
    
    - Verify that CanTSyn_MasterConfig table in CanTSyn_Cfg.c contains the value  CANTSYN_SYNC_CRC_TXCRCSECUREDOFMASTERCONFIG (standard time base) or CANTSYN_OFS_CRC_TXCRCSECUREDOFMASTERCONFIG (offset time base) in struct member TxCrcSecured of the time domain. The relations between the rows in CanTSyn_MasterConfig table and the time domain can be identified with the comment in the struct member DomainConfigIdx, which is the short name of the global time domain.
    
    For time slaves this setting shall be configured and verified by:
    
    - Set /MICROSAR/CanTSyn/CanTSynGlobalTimeDomain/CanTSynGlobalTimeSlave/CanTSynRxCrcValidated to CRC_VALIDATED for all time slaves of the same time domain (for the time gateways and all slaves on all subnets which requires functional safety)
    
    - Verify that CanTSyn_SlaveConfig table in CanTSyn_Cfg.c contains the value CANTSYN_VALIDATED_RXCRCVALIDATEDOFSLAVECONFIG in struct member RxCrcValidated of the time domain. The relations between the rows in CanTSyn_SlaveConfig table and the time domain can be identified with the comment in the struct member DomainConfigIdx, which is the short name of the global time domain.


Additional verification measures
********************************

.. smi:: SMI-1214718

    **The user of MICROSAR Safe shall implement a reverse Time Synchronization to verify the integrity of the distributed time.**
    
    To reliably detect all potential failure modes, most notably a constant offset as well as drift within leap check bounds, the user of MICROSAR Safe shall configure reverse time synchronization. This requires every slave to also act as verification master for a different time domain. The user of MICROSAR Safe shall configure a dedicated time validator, available at the highest integrity level in the system, as verification slave for each time domain created by its actual slaves. The ECU intended to be a slave can thereby return its local time to this time validator. The user of MICROSAR Safe shall then compare the time returned by the verification slave to the actual time and check if deviations are in an acceptable range regarding the required precision. If the time is within an acceptable range, the slave application has to be informed on a separate channel.
    The time synchronization must be considered failed for the complete system otherwise.
    
    This is required as the time taken from and distributed over CAN (via CAN Interface and Driver) may be corrupted.
    This is not required if the provision of global time via CAN Interface and Driver fulfills safety requirements. If hardware timestamping is used, respective safety requirements will need to be provided by the hardware as well.
    For components provided as part of MICROSAR Safe the respective section of the Safety Manual for that component allows you identifying if this requirement is fulfilled.


Safety requirements required from other components
**************************************************

.. smi:: SMI-271170

    CanTSyn requires the following safety requirements from the StbM:
    
    - timeout monitoring on slave side
    - time leap check on slave side
    
    These requirements are necessary in order to detect wrong time stamps in e.g. corrupted or unintended messages.
    
    Timeout monitoring and time leap check shall be configured to match the required precision of the synchronized time. 
    
    This requirement is fulfilled if an ASIL StbM by Vector is used and the required functionality is activated.


Dependencies to hardware
************************

This component does not use a direct hardware interface.

