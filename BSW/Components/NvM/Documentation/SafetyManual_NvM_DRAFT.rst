Safety Manual NvM
#################

Assumed safety requirements
***************************

.. smi:: SMI-21

    This component provides the following safety requirements:
    
    
    .. list-table::
       :header-rows: 1
       :widths: 25 45 20 10
    
       * - ID
         - Safety Requirement
         - Associated TSR
         - ASIL 

       * - CREQ-723
         - NvM shall provide a service which initializes the module.
         - TSR-1
         - ASIL D

       * - CREQ-724
         - NvM shall provide a service to read a single NvM block from NVRAM.
         - TSR-5
         - ASIL D

       * - CREQ-725
         - NvM shall provide a service to write a single NvM block to NVRAM.
         - TSR-4
         - ASIL D

       * - CREQ-730
         - NvM shall provide a service to read all possible NvM blocks from NVRAM.
         - TSR-5
         - ASIL D

       * - CREQ-731
         - NvM shall provide a service to write all possible NvM block to NVRAM.
         - TSR-4
         - ASIL D

       * - CREQ-749
         - NvM shall provide data integrity functionality to ensure written/read data's consistency.
         - TSR-5,TSR-4
         - ASIL D

       * - CREQ-752
         - NvM shall provide a mechanism to ensure NV data belong to requested block.
         - TSR-4,TSR-5
         - ASIL D

       * - CREQ-746
         - NvM shall provide configurable callbacks to synchronize block data.
         - TSR-4,TSR-5
         - ASIL D
    
    
    This component in addition to the assumed safety requirements ensures freedom from interference with respect to memory and timing with up to ASIL D.


.. smi:: SMI-29

    The user of MICROSAR Safe must design the system in a way that in case of the absence of non‑volatile data it is still safe (e.g. safe state or degradation). It cannot be assured by the memory stack that data is saved completely or at all because a reset or loss of energy might happen at any time, e.g. brown-out, black-out.
    This also implies that it is in general impossible to guarantee that the latest information is available in the non-volatile memory, e.g. the system is reset before memory stack is even notified to write data to non-volatile memory.
    Thus, safety-related functionality may not rely on the availability of data in non-volatile memory.
    Since the availability of data in non-volatile memory cannot be guaranteed in any case, the only sensible use-case is reading safety-related calibration data.
    Writing of data into non-volatile memory must be verified to assure that the information is available in non-volatile memory. Verification can only be done manually in a protected environment, e.g. at end of line, in a workshop, etc.
    ECU software cannot verify if data was written, since at any time a reset could occur and the information that had to be written is lost immediately.
    Reading of data does not modify data stored in non-volatile memory. Thus, reading can be used by safety-related functionality. The memory stack assures that the read data is identical to the data stored in non-volatile memory.
    The availability may be increased by e.g. redundant storage.


.. smi:: SMI-51478

    The user of MICROSAR Safe must ensure that the selected CRC is suitable for the ASIL and data size. 
    
    The NvM uses a CRC32 to ensure integrity of data (see also SMI-25). Since fault detection capabilities depend on the CRC polynomial and maximum data size, this needs to be considered during definition of safety-related NvM blocks. 
    
    Please note that also the block ID with 3 bytes is taken into account for CRC calculation (see SMI-26).
    
    The NvM may also use the CRC to decide whether data in RAM is modified and needs to be written.


Configuration constraints
*************************

.. smi:: SMI-25

    The user of MICROSAR Safe shall configure and verify the following attributes for **each NvM block that contains safety-related data**:

    - Set /MICROSAR/NvM/NvMBlockDescriptor/NvMBlockUseCrc to TRUE.
    - Set /MICROSAR/NvM/NvMBlockDescriptor/NvMBlockCrcType to NVM_CRC32.
    
    Search for the NvM block to verify in NvM_Cfg.c (use the NvMBlockDescriptor short name for textual search, or the NvMNvramBlockIdentifier as index in NvM block array NvM_BlockDescriptorTable_at) - the block structure shall have NVM_BLOCK_CRC_32_ON set.


.. smi:: SMI-26

    The user of MICROSAR Safe shall configure and verify the following attribute:

    - Set /MICROSAR/NvM/NvMCommon/NvMUseBlockIdCheck to TRUE.
    
    This setting is enforced by MSSV plugin.


Additional verification measures
********************************

.. smi:: SMI-22

    The user of MICROSAR Safe shall pass the intended block ID for reading and writing of a single NvM block. NvM cannot detect if an unintended block that is configured is provided by the user. 
    Verification can be performed during integration testing.


.. smi:: SMI-23

    The user of MICROSAR Safe shall verify that the buffer passed for reading and writing of a single NvM block is valid and sufficiently large for the passed block ID.
    Verification can be performed by a  review of the generated configuration and the code passing the pointer and block ID to the NvM.


Safety requirements required from other components
**************************************************

.. smi:: SMI-28

    This component requires the CRC calculation functionality as an assumed safety requirement (TSR-4, TSR-5) from CRC component.


Dependencies to hardware
************************

This component does not use a direct hardware interface.

