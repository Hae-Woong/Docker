Safety Manual FrTSyn
####################

Assumed safety requirements
***************************

.. smi:: SMI-664560

    This component assumes the following safety requirements:
    
    
    .. list-table::
       :header-rows: 1
       :widths: 25 45 20 10
    
       * - ID
         - Safety Requirement
         - Linked TSR
         - ASIL 

       * - CREQ-128010
         - FrTSyn shall provide a service to initialize itself.
         - TSR-1
         - ASIL D

       * - CREQ-127788
         - FrTSyn shall provide a service to trigger time base synchronization transmission
         - TSR-111109
         - ASIL D

       * - CREQ-129671
         - FrTSyn shall provide a service to trigger offset time base transmission
         - TSR-111109
         - ASIL D

       * - CREQ-127789
         - FrTSyn shall provide a service to provide time base after reception of a valid protocol information
         - TSR-111109
         - ASIL D

       * - CREQ-129672
         - FrTSyn shall provide a service to provide time base offset
         - TSR-111109
         - ASIL D

       * - CREQ-127790
         - FrTSyn shall provide a service to protect the time synchronization protocol
         - TSR-111109
         - ASIL D
    
    
    This component in addition to the assumed safety requirements ensures freedom from interference with respect to memory and timing with up to ASIL D.


Configuration constraints
*************************

.. smi:: SMI-368294

    The user of MICROSAR Safe shall enable CRC validation if time gateway is configured. 
    
    This setting is necessary to protect the SGW Bit in SYNC and OFS messages against corruption.
    For time masters this setting shall be configured and verified by:
    
    - Set /MICROSAR/FrTSyn/FrTSynGlobalTimeDomain/FrTSynGlobalTimeMaster/FrTSynGlobalTimeTxCrcSecured to CRC_SUPPORTED for all time masters of the same time domain (for the global time master and for all time gateways).
    
    - Verify that FrTSyn_MasterConfig table in FrTSyn_Cfg.c contains the value  FRTSYN_SYNC_CRC_TXCRCSECUREDOFMASTERCONFIG (standard time base) or FRTSYN_OFS_CRC_TXCRCSECUREDOFMASTERCONFIG (offset time base) in struct member TxCrcSecured of the time domain. The relations between the rows in FrTSyn_MasterConfig table and the time domain can be identified with the comment in the struct member DomainConfigIdx, which is the short name of the global time domain.
    
    For time slaves this setting shall be configured and verified by:
    
    - Set /MICROSAR/FrTSyn/FrTSynGlobalTimeDomain/FrTSynGlobalTimeSlave/FrTSynRxCrcValidated to CRC_VALIDATED for all time slaves of the same time domain (for the time gateways and all slaves on all subnets which requires functional safety)
    
    - Verify that FrTSyn_SlaveConfig table in FrTSyn_Cfg.c contains the value FRTSYN_VALIDATED_RXCRCVALIDATEDOFSLAVECONFIG in struct member RxCrcValidated of the time domain. The relations between the rows in FrTSyn_SlaveConfig table and the time domain can be identified with the comment in the struct member DomainConfigIdx, which is the short name of the global time domain.


Additional verification measures
********************************

.. smi:: SMI-369014

    **The user of MICROSAR Safe shall implement a reverse Time Synchronization to verify the integrity of the distributed time.** 
    
    To reliably detect all potential failure modes, most notably a constant offset as well as drift within leap check bounds, the user of MICROSAR Safe shall configure reverse time synchronization. This requires every slave to also act as verification master for a different time domain. The user of MICROSAR Safe shall configure a dedicated time validator, available at the highest integrity level in the system, as verification slave for each time domain created by its actual slaves. The ECU intended to be a slave can thereby return its local time to this time validator. The user of MICROSAR Safe shall then compare the time returned by the verification slave to the actual time and check if deviations are in an acceptable range regarding the required precision. If the time is within an acceptable range, the slave application has to be informed on a separate channel.
    The time synchronization must be considered failed for the complete system otherwise.
    
    This is required as the time taken from and distributed over FlexRay (via FlexRay Interface and Driver) may be corrupted.
    This is not required if the provision of global time, cycle length and macrotick duration via FlexRay Interface and Driver as well as in hardware fulfills safety requirements.
    For components provided as part of MICROSAR Safe the respective section of the Safety Manual for that component allows you identifying if this requirement is fulfilled.


Safety requirements required from other components
**************************************************

.. smi:: SMI-368295

    FrTSyn requires the following safety requirements from the StbM:
    
    - timeout monitoring on slave side
    - time leap check on slave side
    
    These requirements are necessary in order to detect wrong time stamps in e.g. corrupted or unintended messages.
    
    Timeout monitoring and time leap check shall be configured to match the required precision of the synchronized time. 
    
    This requirement is fulfilled if an ASIL StbM by Vector is used and the required functionality is activated.


Dependencies to hardware
************************

This component does not use a direct hardware interface.

