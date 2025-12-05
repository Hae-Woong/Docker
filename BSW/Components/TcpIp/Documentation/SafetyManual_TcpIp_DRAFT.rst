Safety Manual TcpIp
###################

Assumed safety requirements
***************************

This component does not assume safety requirements. This component is developed according to ISO 26262 only to ensure freedom from interference with respect to memory with up to ASIL B.

Configuration constraints
*************************

.. smi:: SMI-191230

    The user of MICROSAR Safe shall configure the following attributes:

    - Set /MICROSAR/TcpIp/TcpIpGeneral/TcpIpIpV6General/TcpIpIpV6Enabled to FALSE.
    - Set /MICROSAR/TcpIp/TcpIpGeneral/TcpIpIpV4General/TcpIpIcmpEnabled to TRUE.
    - Set /MICROSAR/TcpIp/TcpIpGeneral/TcpIpDhcpServerEnabled to FALSE.
    - Set /MICROSAR/TcpIp/TcpIpGeneral/TcpIpAsrTlsEnabled to FALSE.
    - Set /MICROSAR/TcpIp/TcpIpGeneral/TcpIpMultiCoreSupportEnabled to FALSE.
    - Set TCPIP_CONFIGURATION_VARIANT to TCPIP_CONFIGURATION_VARIANT_PRECOMPILE
     
    These settings are enforced by an MSSV plugin.


Additional verification measures
********************************

.. smi:: SMI-242896

    The user of MICROSAR Safe shall verify that the buffer returned by EthIf_ProvideTxBuffer() is valid and at least as large as the requested length when BUFREQ_OK is returned. Verification can be performed by a review of the used EthIf module.
    
    This requirement is fulfilled if the MICROSAR Safe EthIf module is used.


.. smi:: SMI-540959

    The user of MICROSAR Safe shall verify that the pointer (MeasurementDataPtr) passed to the function *TcpIp_GetAndResetMeasurementData* references a valid memory location of size *SIZE*, when invoked with TCPIP_MEAS_VENDOR_SPECIFIC_SA_PAIRS.
    *SIZE* is calculated by:
    
    *SIZE* = *NumOfSaEntryPair* x (sizeof(TcpIp_IpSecSaStatusType))
    
    To obtain the value of *NumOfSaEntryPair*, the user MUST first invoke the function TcpIp_GetAndResetMeasurementData() with TCPIP_MEAS_VENDOR_SPECIFIC_NR_SA_PAIRS.
    The user has to ensure, that both calls to *TcpIp_GetAndResetMeasurementData* must not be interrupted by calls to *TcpIp_IpSec_AddSaEntryPair* or *TcpIp_IpSec_DelSaEntryPair*.
    
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


.. smi:: SMI-540960

    The user of MICROSAR Safe shall verify that the pointer (*SaInfoPtr*) passed to the function *TcpIp_IpSecSaEntryPairAdd* references a valid element of *TcpIp_IpSecSaInfoType* which satisfies the following requirements:

    - SaInfoPtr->TrafficSelectors != NULL_PTR
    - SaInfoPtr->KeyMaterial.InboundKeyMatPtr references a valid Key Material array of size InboundKeyMatLen
    - SaInfoPtr->KeyMaterial.OutboundKeyMatPtr references a valid Key Material array of size OutboundKeyMatLen
    
    The function is called to create a pair of SA entries for IPsec.
    
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


Safety requirements required from other components
**************************************************

This component does not require safety requirements from other components.

Dependencies to hardware
************************

This component does not use a direct hardware interface.

