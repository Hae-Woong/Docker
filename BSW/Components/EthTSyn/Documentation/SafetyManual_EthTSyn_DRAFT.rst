Safety Manual EthTSyn
#####################

Assumed safety requirements
***************************

This component assumes the following safety requirements:


.. list-table::
   :header-rows: 1

   * - ID
     - Safety Requirement
     - Linked TSR
     - ASIL

   * - CREQ-EthTSyn-Initialization
     - EthTSyn shall provide a service to initialize itself.
     - TSR-1
     - ASIL D

   * - CREQ-EthTSyn-MasterSyncInfoTransmissionFormat
     - EthTSyn shall provide a service to trigger transmission of synchronization information in different formats.
     - TSR-111109
     - ASIL D

   * - CREQ-EthTSyn-MasterDistributeStatus
     - EthTSyn shall provide a service to distribute the status of a time base.
     - TSR-111109
     - ASIL D

   * - CREQ-EthTSyn-SlaveSyncInfoReceptionFormat
     - EthTSyn shall provide a service to handle received synchronization information in different formats.
     - TSR-111109
     - ASIL D

   * - CREQ-EthTSyn-SlaveForwardReceivedStatus
     - EthTSyn shall provide a service to calculate and forward a time base status received with a FollowUp message.
     - TSR-111109
     - ASIL D

   * - CREQ-EthTSyn-CrcProtection
     - EthTSyn shall provide a service to protect transmitted time synchronization information by CRC.
     - TSR-111109
     - ASIL D

   * - CREQ-EthTSyn-CrcValidation
     - EthTSyn shall provide a service to validate CRC of received protected time synchronization information.
     - TSR-111109
     - ASIL D


This component in addition to the assumed safety requirements ensures freedom from interference with respect to memory with up to ASIL D.

Configuration constraints
*************************

.. smi:: SMI-1026908

    The user of MICROSAR Safe shall disable the switch management functionality.

    This setting shall be configured and verified by:

    - Set /MICROSAR/EthTSyn/EthTSynGeneral/EthTSynEnableSwitchMgmt to FALSE
    - Verify that ETHTSYN_SWT_MGMT_SUPPORT is defined as STD_OFF in EthTSyn_Cfg.h

    This requirement is only applicable if TSR-111109 is assumed as a safety requirement.


.. smi:: SMI-1026909

    The user of MICROSAR Safe shall enable CRC protection and validation.

    For time masters this setting shall be configured and verified by:

    - Set /MICROSAR/EthTSyn/EthTSynGlobalTimeDomain/EthTSynPortConfig/EthTSynPortRole/EthTSynGlobalTimeMaster/EthTSynGlobalTimeTxCrcSecured of every master port CRC_SUPPORTED
    - Verify that ETHTSYN_CRC_SUPPORT is defined as STD_ON in EthTSyn_Cfg.h
    - Verify that EthTSyn_SyncSendSm table contains a value less than size of EthTSyn_CrcHndl table for the struct member CrcHndlIdx. Both tables are defined in EthTSyn_Lcfg.c

    For time slaves this setting shall be configured and verified by:

    - Set /MICROSAR/EthTSyn/EthTSynGlobalTimeDomain/EthTSynPortConfig/EthTSynPortRole/EthTSynGlobalTimeSlave/EthTSynRxCrcValidated to CRC_VALIDATED
    - Verify that ETHTSYN_CRC_SUPPORT is defined as STD_ON in EthTSyn_Cfg.h
    - Verify that EthTSyn_FupRxHandler table in EthTSyn_Lcfg.c contains:

      - ETHTSYN_RX_CRC_VALIDATED_CRC_VALIDATED for the struct member RxCrcValidated
      - a value leass than size of EthTSyn_CrcHndl table (defined in EthTSyn_Lcfg.c as well) for the struct member CrcHndlIdx

    Hints:

      - CRC protection and validation is only applied to AUTOSAR Sub-TLVs which are enabled by configuration
      - The Time Secured Sub-TLV can be used to add a CRC based on selected FollowUp message fields
      - The user shall configure the Sub-TLV usage in accordance with the safety concept

    This requirement is only applicable if TSR-111109 is assumed as a safety requirement.


Additional verification measures
********************************

.. smi:: SMI-233513

    The user of MICROSAR Safe shall verify that the buffer returned by EthIf_ProvideTxBuffer() is valid and at least as large as the requested length when BUFREQ_OK is returned.
    Verification can be performed by a review of the used EthIf module.

    For components provided as part of MICROSAR Safe, the respective section of the Safety Manual for that component allows you identifying if this requirement is fulfilled.
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


.. smi:: SMI-585337

    The user of MICROSAR Safe shall verify that the Ethernet Switch management object returned by EthIf_GetRxMgmtObj() or EthIf_GetTxMgmtObj() is valid when E_OK is returned by the corresponding API.
    Verification can be performed by a review of the used EthIf module.

    For components provided as part of MICROSAR Safe, the respective section of the Safety Manual for that component allows you identifying if this requirement is fulfilled.
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


.. smi:: SMI-1031617

    The user of MICROSAR Safe shall verify consistency of the synchronized time base identifier for all generated configuration structures of time domain.
    The structure, EthTSyn_ComCtrlTimeDomain, is generated to EthTSyn_Lcfg.c. Symbolic name value of the reference synchronized time base is generated to the struct member SynchronizedTimeBaseId.
    The relation of the rows in the EthTSyn_ComCtrlTimeDomain table and configuration can be identified with the comment of Referable Keys which is the path of the corresponding EthTSyn time domain.

    This requirement is only applicable if TSR-111109 is assumed as a safety requirement.


.. smi:: SMI-1217297

    The user of MICROSAR Safe shall implement a reverse Time Synchronization to verify the integrity of the distributed time.

    To reliably detect all potential failure modes, most notably a constant offset as well as a drift within leap check bounds, the user of MICROSAR Safe shall configure reverse time synchronization.
    This requires every slave to also act as verification master for a different time domain.
    The user of MICROSAR Safe shall configure a dedicated time validator, available at the highest integrity level in the system,
    as verification slave for each time domain created by its actual slaves.
    The ECU intended to be a slave can thereby return its local time to this time validator.
    The user of MICROSAR Safe shall then compare the time returned by the verification slave to the actual time and check if deviations are in an acceptable range regarding the required precision.
    If the time is within an acceptable range, the slave application has to be informed on a separate channel.
    The time synchronization must be considered failed for the complete system otherwise.

    This is required as the time taken from and distributed over Ethernet (via Ethernet Interface and Driver) may be corrupted.
    This is not required if the provision of global time via Ethernet Interface and Driver fulfills safety requirements.
    If hardware timestamping is used, respective safety requirements will need to be provided by the hardware as well.
    For components provided as part of MICROSAR Safe the respective section of the Safety Manual for that component allows you identifying if this requirement is fulfilled.

    This requirement is only applicable if TSR-111109 is assumed as a safety requirement.


Safety requirements required from other components
**************************************************

.. smi:: SMI-1025788

    EthTSyn requires the following safety requirements from the StbM:

    - retrieve the current time tuple together with the status of the time base via StbM_BusGetCurrentTime()

    This requirement is fulfilled if an ASIL StbM by Vector is used and the required functionality is activated.

    This requirement is only applicable if TSR-111109 is assumed as a safety requirement.


.. smi:: SMI-1032413

    EthTSyn requires the following safety requirement from the Crc:

    - calculate 8-bit 0x2F CRC (Crc_CalculateCRC8H2F)

    This requirement is necessary to ensure correct CRC calculation and hence, proper detection of corrupted FollowUp message content.

    This requirement is fulfilled if an ASIL Crc by Vector is used.

    This requirement is only applicable if TSR-111109 is assumed as a safety requirement.


.. smi:: SMI-1040542

    EthTSyn requires the following safety requirements from the VStdLib:

    - clear memory via VStdLib_MemClr()
    - copy memory via VStdLib_MemCpy_s()

    This requirement is necessary to ensure correct message serialization in the EthTSyn.

    This requirement is fulfilled if an ASIL VStdLib by Vector is used.

    This requirement is only applicable if TSR-111109 is assumed as a safety requirement.


Dependencies to hardware
************************

This component does not use a direct hardware interface.

