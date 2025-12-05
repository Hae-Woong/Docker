Safety Manual EthIf
###################

Assumed safety requirements
***************************

This component does not assume safety requirements. This component is developed according to ISO 26262 only to ensure freedom from interference with respect to memory  with up to ASIL D.

Configuration constraints
*************************

.. smi:: SMI-159340

    The user of MICROSAR Safe shall configure and verify the following settings:
    
    
    .. list-table::
       :header-rows: 1
    
       * - ID
         - Configuration element
         - Constraint

       * - 1
         - /MICROSAR/EthIf/EthIfGeneral/EthIfSafeBswChecks
         - TRUE

       * - 2
         - /MICROSAR/EthIf/EthIfConfigSet/EthIfExtendedTrafficHandling
         - No such container must exist

       * - 3
         - /MICROSAR/EthIf/EthIfConfigSet/EthIfController/EthIfEcucPartitionRef
         - No such reference must exist
    
    
    These configuration constraints are mapped to respective feature defines in EthIf_Cfg.h stating if feature is enabled. These constraints and their relation to the configuration elements are shown in the following table:
    
    
    .. list-table::
       :header-rows: 1
    
       * - Define
         - Constraint
         - Related ID

       * - ETHIF_DEV_ERROR_DETECT
         - STD_ON
         - 1

       * - ETHIF_EXTENDED_TRAFFIC_HANDLING_TYPE
         - ETHIF_NO_EXTENDED_TRAFFIC_HANDLING
         - 2

       * - ETHIF_MULTI_CORE_SUPPORT
         - STD_OFF
         - 3
    
    
    These settings are checked by the MSSV plugin.


Additional verification measures
********************************

.. smi:: SMI-751765

    The user of MICROSAR Safe shall verify that the Ethernet Switch management object returned by EthSwt_GetRxMgmtObject() and EthSwt_GetTxMgmtObj() is valid when E_OK is returned by the corresponding API. Verification can be performed by a review of the used EthSwt module.
    
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


.. smi:: SMI-751766

    The user of MICROSAR Safe shall verify that the buffer returned by Eth_ProvideTxBuffer() is valid and at least as large as the requested length when BUFREQ_OK is returned. Verification can be performed by a review of the used Eth driver.
    
    This requirement is fulfilled if the MICROSAR Safe Eth module is used.


Safety requirements required from other components
**************************************************

This component does not require safety requirements from other components.

Dependencies to hardware
************************

This component does not use a direct hardware interface.

