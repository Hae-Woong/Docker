Safety Manual Nm
################

Assumed safety requirements
***************************

This component does not assume safety requirements. This component is developed according to ISO 26262 only to ensure freedom from interference with respect to memory with up to ASIL D.

Configuration constraints
*************************

The user of MICROSAR Safe shall ensure the following configuration constraints:

.. smi:: SMI-120514

    - For Generic BusNms with value: NmFiatB and NmFiatC the coordinator cluster index should not exist, as they should not be used for the coordinator algorithm.

    /MICROSAR/Nm/NmChannelConfig/NmBusType/NmGenericBusNmConfig/NmGenericBusNmPrefix
    /MICROSAR/Nm/NmChannelConfig/NmCoordClusterIndex
    
    Prove the following in the generated code:
    FOR PRE-COMPILE configurations:

    - Look for the array: "Nm_ChannelConfig[]"
    - Find the channels which are actively coordinated: The member "CoordinatorStateClusterIdx" of the channel entry is different from 255. 
    - Identify the Function Table reference corresponding to the coordinated channel: It is indicated by the "NmFunctionTableIdx" member.
    - Find out which BusNm corresponds to this coordinated channel: Find the array "Nm_NmFunctionTable[]" go to the element indicated by the "NmFunctionTableIdx" found. If the functions listed start with "NmFiatB" or "NmFiatC", this is NOT a safe configuration.
     
    
    FOR POST-BUILD configurations:

    - Look for the structure "Nm_PCConfig" and do the following for every element:
    - Identify the name of the pointer of the channel configuration of the identity.
    - Look for the array with the same name as the identified pointer.
    - Find the channels which are actively coordinated: The member "CoordinatorStateClusterIdx" of the channel entry is different from 255.
    - Identify their corresponding values for "NmBusNmAssociationStartIdx" and "NmBusNmAssociationLength"
    - Identify the name of the pointer of the bus association of the identity.
    - Look for the array with the same name as the identified pointer.
    - Localize the "NmFunctionTableIdx" member of the array elements from index "NmBusNmAssociationStartIdx" to index "NmBusNmAssociationStartIdx + (NmBusNmAssociationLength- 1)"
    - Find out which BusNm corresponds to this coordinated channel: Find the array "Nm_NmFunctionTable[]" go to the element indicated by the "NmFunctionTableIdx" found. If the functions listed start with "NmFiatB" or "NmFiatC", this is NOT a safe configuration.


.. smi:: SMI-772085

    The user of MICROSAR Safe shall ensure that the Nm master partition is assigned to the highest ASIL of all Nm partitions.
    This requirement is necessary to ensure the consistency of the state machine of this component.


Additional verification measures
********************************

.. smi:: SMI-120515

    - The switch NM_ENABLE_CHANNELTYPE_COORDINATED_NMFIATC should have the value STD_OFF
    
    This setting is enforced by an MSSV plugin.


Safety requirements required from other components
**************************************************

Safety requirements required from other components
==================================================

This component does not require safety requirements from other components.

Coexistence with other components
=================================

.. smi:: SMI-149

    This component requires coexistence with ComM, Com, BswM, Det, SchM/Rte, Rtm and bus network manager components if the interface for those components is configured.


Dependencies to hardware
************************

This component does not use a direct hardware interface.

