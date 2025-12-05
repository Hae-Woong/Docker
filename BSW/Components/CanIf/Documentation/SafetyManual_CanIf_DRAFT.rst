Safety Manual CanIf
###################

Assumed safety requirements
***************************

.. smi:: SMI-1268398

    This component assumes the following safety requirements: 
    
    .. list-table::
       :header-rows: 1
       :widths: 25 45 20 10
    
       * - ID
         - Safety Requirement
         - Associated TSR
         - ASIL 

       * - CREQ-922
         - CanIf shall provide a service to initialize itself.
         - TSR-1
         - ASIL D

       * - CREQ-923
         - CanIf shall provide a service to set the mode of a CAN controller. 
         - TSR-100551
         - ASIL D

       * - CREQ-924
         - CanIf shall provide a service to get the mode of a CAN controller. 
         - TSR-100551
         - ASIL D

       * - CREQ-925
         - CanIf shall provide a service to set the PDU mode of a CAN controller. 
         - TSR-100551
         - ASIL D

       * - CREQ-926
         - CanIf shall provide a service to get the PDU mode of a CAN controller.
         - TSR-100551
         - ASIL D

       * - CREQ-927
         - CanIf shall provide a callback to notify about the BusOff-event of a CAN controller.
         - TSR-100551
         - ASIL D

       * - CREQ-933
         - CanIf shall provide a service to process received messages.
         - TSR-111108
         - ASIL D

       * - CREQ-941
         - CanIf shall provide a service to transmit a message.
         - TSR-111108
         - ASIL D

       * - CREQ-942
         - CanIf shall provide a buffer for Tx-messages.
         - TSR-111108
         - ASIL D

       * - CREQ-102740
         - CanIf shall provide a configurable buffer for Tx-messages.
         - TSR-111108
         - ASIL D
    
    
    This component in addition to the assumed safety requirements ensures freedom from interference with respect to memory and timing with up to ASIL D.


Configuration constraints
*************************

.. smi:: SMI-461418

    The user of MICROSAR Safe shall configure and verify the following attributes to disable meta data support:
    
    - Set /MICROSAR/CanIf/CanIfPublicCfg/CanIfMetaDataSupport to FALSE.
    
    for each Rx- and Tx-PDU

    - Delete /MICROSAR/EcuC/EcucPduCollection/Pdu/MetaDataLength
    
    Verify that the defines CANIF_META_DATA_RX_SUPPORT and CANIF_META_DATA_TX_SUPPORT in CanIf_Cfg.h are set to STD_OFF.
    
    This requirement is necessary to ensure usage in a released configuration.
    
    This requirement is only applicable if TSR-111108 is assumed as a safety requirement.


.. smi:: SMI-461417

    The user of MICROSAR Safe shall configure and verify the following attributes to disable J1939 - Address translation:
    
    - Set /MICROSAR/CanIf/CanIfPublicCfg/CanIfJ1939DynAddrSupport to DISABLED.
    
    for each CAN controller

    - Set /MICROSAR/CanIf/CanIfCtrlDrvCfg/CanIfCtrlCfg/CanIfCtrlJ1939DynAddrSupport to DISABLED.
    
    Verify that the define CANIF_J1939_DYN_ADDR_SUPPORT in CanIf_Cfg.h is set to CANIF_J1939_DYN_ADDR_DISABLED.
    
    This requirement is necessary to ensure usage in a released configuration.
    
    This requirement is only applicable if TSR-111108 is assumed as a safety requirement.


.. smi:: SMI-461415

    The user of MICROSAR Safe shall configure and verify the following attribute for each CAN driver to disable HW cancellation:
    
    - Set /MICROSAR/CanIf/CanIfCtrlDrvCfg/CanIfCtrlDrvTxCancellation to FALSE.
    
    Verify that the define CANIF_TRANSMIT_CANCELLATION in CanIf_Cfg.h is set to STD_OFF.
    
    This requirement is necessary to prevent the transmit cancellation of CAN messages initiated by a CAN driver. 
    
    This requirement is necessary to ensure usage in a released configuration.
    
    This requirement is only applicable if TSR-111108 is assumed as a safety requirement.


.. smi:: SMI-461416

    The user of MICROSAR Safe shall configure and verify the following attribute to disable SW cancellation:
    
    - Set /MICROSAR/CanIf/CanIfPublicCfg/CanIfPublicCancelTransmitSupport to FALSE.
    
    Verify that the define CANIF_CANCEL_SUPPORT_API in CanIf_Cfg.h is set to STD_OFF.
    
    This requirement is necessary to prevent the transmit cancellation of CAN messages due to an explicit cancellation request.
    
    This requirement is necessary to ensure usage in a released configuration.
    
    This requirement is only applicable if TSR-111108 is assumed as a safety requirement.


.. smi:: SMI-486725

    The user of MICROSAR Safe shall configure and verify the following attributes to disable data checksum support:
    
    - Set /MICROSAR/CanIf/CanIfPrivateCfg/CanIfDataChecksumRxSupport to FALSE.
    - Delete /MICROSAR/CanIf/CanIfDispatchCfg/CanIfDispatchDataChecksumRxErrorIndicationName.
    - Delete /MICROSAR/CanIf/CanIfDispatchCfg/CanIfDispatchDataChecksumRxErrorIndicationUL.
    - Set /MICROSAR/CanIf/CanIfPrivateCfg/CanIfDataChecksumTxSupport to FALSE.
    
    for each Rx-PDU

    - Delete /MICROSAR/CanIf/CanIfInitCfg/CanIfRxPduCfg/CanIfRxPduDataChecksumPdu.
    
    for each Tx-PDU

    - Delete /MICROSAR/CanIf/CanIfInitCfg/CanIfTxPduCfg/CanIfTxPduDataChecksumPdu.
    
    Verify that the defines CANIF_DATA_CHECKSUM_RX_SUPPORT and CANIF_DATA_CHECKSUM_TX_SUPPORT in CanIf_Cfg.h are set to STD_OFF.
    
    This requirement is necessary to ensure usage in a released configuration.
    
    This requirement is only applicable if TSR-111108 is assumed as a safety requirement.


.. smi:: SMI-486727

    The user of MICROSAR Safe shall configure and verify the following attributes to disable PDU reception mode support:
    
    - Set /MICROSAR/CanIf/CanIfPrivateCfg/CanIfSetPduReceptionModeSupport to FALSE.
    
    for each Rx-PDU

    - Delete /MICROSAR/CanIf/CanIfInitCfg/CanIfRxPduCfg/CanIfRxPduSetReceptionModePdu.
    
    Verify that the define CANIF_SET_PDU_RECEPTION_MODE_SUPPORT in CanIf_Cfg.h is set to STD_OFF.
    
    This requirement is necessary to prevent the set of the reception mode of Rx-PDUs and therefore the non-indication or the indication with an unexpected Rx-PDU-Id of a received CAN message.
    
    This requirement is necessary to ensure usage in a released configuration.
    
    This requirement is only applicable if TSR-111108 is assumed as a safety requirement.


.. smi:: SMI-1191404

    The user of MICROSAR Safe shall configure and verify the following attributes to disable support of dynamic Rx PDUs:
    
    - Set /MICROSAR/CanIf/CanIfPublicCfg/CanIfPublicSetDynamicRxIdApi to FALSE.
    - Delete /MICROSAR/CanIf/CanIfDispatchCfg/CanIfDispatchUserSetDynamicRxIdName.
    - Delete /MICROSAR/CanIf/CanIfDispatchCfg/CanIfDispatchUserSetDynamicRxIdUL.
    
    for each Rx PDU

    - Set /MICROSAR/CanIf/CanIfInitCfg/CanIfRxPduCfg/CanIfRxPduType to STATIC.
    
    Verify that the define CANIF_SETDYNAMICRXID_API in CanIf_Cfg.h is set to STD_OFF.
    
    This requirement is necessary to prevent the usage of dynamic Rx PDUs and therefore the non-indication or the indication with an unexpected Rx PDU ID of a received CAN frame.
    
    This requirement is necessary to ensure usage in a released configuration.
    
    This requirement is only applicable if TSR-111108 is assumed as a safety requirement.


.. smi:: SMI-486728

    The user of MICROSAR Safe shall configure and verify the following attributes to disable partial networking Tx-PDU filter support:
    
    - Set /MICROSAR/CanIf/CanIfPublicCfg/CanIfPnWakeupTxPduFilterSupport to FALSE.
    
    for each Tx-PDU

    - Delete /MICROSAR/CanIf/CanIfInitCfg/CanIfTxPduCfg/CanIfTxPduPnFilterPdu.
    
    Verify that the define CANIF_PN_WU_TX_PDU_FILTER in CanIf_Cfg.h is set to STD_OFF.
    
    This requirement is necessary to prevent CAN messages from not being transmitted due to an activated Tx-PDU filter at the affected CAN controller.
    
    This requirement is necessary to ensure usage in a released configuration.
    
    This requirement is only applicable if TSR-111108 is assumed as a safety requirement.


.. smi:: SMI-486959

    The user of MICROSAR Safe shall configure and verify the following attributes to disable dynamic Tx-PDU support:
    
    - Set /MICROSAR/CanIf/CanIfPublicCfg/CanIfPublicSetDynamicTxIdApi to FALSE.
    
    for each Tx-PDU

    - Set /MICROSAR/CanIf/CanIfInitCfg/CanIfTxPduCfg/CanIfTxPduType to STATIC.
    
    Verify that the define CANIF_SETDYNAMICTXID_API in CanIf_Cfg.h is set to STD_OFF.
    
    This requirement is necessary to prevent changes of CAN-Ids from Tx-PDUs due to explicit change requests and therefore to prevent the transmission of CAN messages with not the configured CAN-Ids.
    
    This requirement is necessary to ensure usage in a released configuration.
    
    This requirement is only applicable if TSR-111108 is assumed as a safety requirement.


.. smi:: SMI-1230095

    The user of MICROSAR Safe shall configure and verify the following attributes to disable CAN XL:
    
    - Set /MICROSAR/CanIf/CanIfPrivateCfg/CanIfSupportCanXL to FALSE.
    
    for each PDU
    
    - Delete /MICROSAR/CanIf/CanIfInitCfg/CanIfRxPduCfg/CanIfRxPduXLParams.
    - Delete /MICROSAR/CanIf/CanIfInitCfg/CanIfTxPduCfg/CanIfTxPduXLParams.
    
    for each Hoh:

    - /MICROSAR/CanIf/CanIfInitCfg/CanIfInitHohCfg/CanIfHrhCfg/CanIfHrhIdSymRef is NOT set per user-define to a CanXLHardwareObject.
    - /MICROSAR/CanIf/CanIfInitCfg/CanIfInitHohCfg/CanIfHthCfg/CanIfHrhIdSymRef is NOT set per user-define to a CanXLHardwareObject.
    
    Verify that the define CANIF_SUPPORT_CAN_XL in CanIf_Cfg.h is set to STD_OFF.
    
    This requirement is necessary to ensure that the beta feature is disabled in a released configuration.


Additional verification measures
********************************

.. smi:: SMI-349

    The user of MICROSAR Safe shall verify for each entry of table ``CanIf_RxIndicationFctList`` that the signature of the function referred by member ``RxIndicationFct`` matches the expected signature that is selected the value of the member ``RxIndicationLayout``.
    
    The table ``CanIf_RxIndicationFctList`` can be found in CanIf_Lcfg.c.
    
    The following table lists the expected signatures. The placeholder ``<name>`` represents the function's name:
    
    
    .. list-table::
       :header-rows: 1
    
       * - Value of RxIndicationLayout
         - Signature of the function referred by RxIndicationFct 

       * - ``CanIf_SimpleRxIndicationLayout``
         - ``void <name>(PduIdType CanRxPduId, const uint8* CanSduPtr)``

       * - ``CanIf_AdvancedRxIndicationLayout``
         - ``void <name>(PduIdType CanRxPduId, const PduInfoType* PduInfoPtr)``

       * - ``CanIf_NmOsekRxIndicationLayout``
         - ``void <name>(PduIdType CanRxPduId, const uint8* CanSduPtr, Can_IdType CanId)``

       * - ``CanIf_CddRxIndicationLayout``
         - ``void <name>(PduIdType CanRxPduId, const PduInfoType* PduInfoPtr, Can_IdType CanId)``
    
    
    The value ``NULL_PTR`` is expected to be always one entry for a ``RxIndicationFct`` in the ``CanIf_RxIndicationFctList``.
    
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


.. smi:: SMI-350

    The user of MICROSAR Safe shall verify for each entry of table ``CanIf_TxConfirmationFctList``  that function referred by member ``TxConfirmationFctList`` has the following signature (the placeholder ``<name>`` represents the function's name):
    
    ``void <name>(PduIdType CanTxPduId)``
    
    The value ``NULL_PTR`` is expected to be always one entry referred by member ``TxConfirmationFctList`` in the table ``CanIf_TxConfirmationFctList``.
    
    The table ``CanIf_TxConfirmationFctList`` can be found in CanIf_Lcfg.c.
    
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


.. smi:: SMI-746

    The user of MICROSAR Safe shall verify for each callback function listed in the following table that the signature of the function matches the expected signature.
    
    All callback functions can be found in file CanIf_Lcfg.c. Please note that depending on configuration you must verify only the provided ones.
    
    
    .. list-table::
       :header-rows: 1
    
       * - Callback function
         - Expected signature of the function

       * - ``CanIf_BusOffNotificationFctPtr``
         - ``void <name> (uint8 ControllerId)``

       * - ``CanIf_CtrlModeIndicationFctPtr``
         - ``void <name> (uint8 ControllerId, CanIf_ControllerModeType ControllerMode)``

       * - ``CanIf_TrcvModeIndicationFctPtr``
         - ``void <name> (uint8 TransceiverId, CanTrcv_TrcvModeType TransceiverMode)``

       * - ``CanIf_ConfirmPnAvailabilityFctPtr``
         - ``void <name> (uint8 TransceiverId)``

       * - ``CanIf_ClearTrcvWufFlagIndicationFctPtr``
         - ``void <name> (uint8 TransceiverId)``

       * - ``CanIf_CheckTrcvWakeFlagIndicationFctPtr``
         - ``void <name> (uint8 TransceiverId)``

       * - ``CanIf_WakeUpValidationFctPtr``
         - ``void <name> (EcuM_WakeupSourceType CanWakeupEvents)``

       * - ``CanIf_RamCheckCorruptControllerIndFctPtr``
         - ``void <name> (uint8 ControllerId)``

       * - ``CanIf_RamCheckCorruptMailboxIndFctPtr``
         - ``void <name> (uint8 ControllerId, CanIf_HwHandleType HwHandle)``

       * - ``CanIf_DataChecksumRxErrFctPtr``
         - ``void <name> (PduIdType CanIfRxPduId)``
    
    
    Configuration depending not provided callback functions have the value ``NULL_PTR`` as a valid value.
    
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


.. smi:: SMI-25089

    The user of MICROSAR Safe shall ensure that any write access within the callout ``CanIf_TransmitSubDataChecksumTxAppend`` to the array referenced by parameter ``txPduDataWidthChecksumPtr`` is within its valid range.
    
    This measure is only applicable if attribute  ``/MICROSAR/CanIf/CanIfPrivateCfg/CanIfDataChecksumTxSupport`` is configured to ``TRUE``.
    
    The size of the array is defined by ``CANIF_CFG_MAXTXDLC_PLUS_DATACHECKSUM`` which can be found in file CanIf_Cfg.h.
    
    This requirement is fulfilled if the implementation of ``CanIf_TransmitSubDataChecksumTxAppend`` is provided by Vector.
    
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


.. smi:: SMI-948886

    The user of MICROSAR Safe shall verify that each partition has the same integrity level (parameter */MICROSAR/EcuC/EcucPartitionCollection/EcucPartition/ASIL*), if multi-partition support is enabled (parameter */MICROSAR/CanIf/CanIfPublicCfg/CanIfSupportMultiPartition*).
    
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


.. smi:: SMI-1115873

    The user of MICROSAR Safe shall verify for each entry in the table ``CanIf_CanDrvFctTbl`` that the signatures of the functions referred by member ``SetControllerModeFct`` and ``WriteFct``
    match the expected signatures that are selected by the value of the member ``CanDrvEqcArVersion`` from table ``CanIf_CanControllerIdUpToLowMap``.
    
    The member ``CanDrvFctTblIdx`` from table ``CanIf_CanControllerIdUpToLowMap`` determines the corresponding entry in table ``CanIf_CanDrvFctTbl``.
    
    This measure is only applicable if multiple CAN Drivers are configured and for at least one CAN Driver the parameter ``CanIf/CanIfCtrlDrvCfg/CanIfCtrlDrvArVersion`` is set to ``CAN_DRV_EQC_AR_431`` and for at least one CAN Driver the parameter ``CanIf/CanIfCtrlDrvCfg/CanIfCtrlDrvArVersion`` is set to ``CAN_DRV_EQC_MSRC``.
    
    The table ``CanIf_CanDrvFctTbl`` can be found in CanIf_Lcfg.c.
    The table ``CanIf_CanControllerIdUpToLowMap`` can be found in CanIf_Lcfg.c for configuration variant Pre-Compile or in CanIf_PBcfg.c for configuration variant Post-Build Loadable.
    
    The following table lists the expected signatures:
    
    
    .. list-table::
       :header-rows: 1
    
       * - Value of CanDrvEqcArVersion
         - Signature of the function referred by SetControllerModeFct
         - Signature of the function referred by WriteFct

       * - ``CAN_DRV_EQC_MSRC`` 
         - ``Can_ReturnType Can_<VendorId>_<VendorApiInfix>_SetControllerMode(uint8 Controller, Can_StateTransitionType Transition)``                                                                                                   
         - ``Can_ReturnType Can_<VendorId>_<VendorApiInfix>_ Write(Can_HwHandleType Hth, const Can_PduType* PduInfo)``

       * - ``CAN_DRV_EQC_AR_431``
         - ``Std_ReturnType Can_<VendorId>_<VendorApiInfix>_SetControllerMode(uint8 Controller, Can_ControllerStateType Transition)``
         - ``Std_ReturnType Can_<VendorId>_<VendorApiInfix>_Write(Can_HwHandleType Hth, const Can_PduType* PduInfo)``
    
    
    The value ``NULL_PTR`` is no expected entry for member ``SetControllerModeFct`` and ``WriteFct`` in the table ``CanIf_CanDrvFctTbl``.
    
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


.. smi:: SMI-1134759

    The user of MICROSAR Safe shall verify for each entry in the table ``CanIf_CanDrvFctTbl`` that the signature of the function referred by member ``CheckWakeupFct`` match the expected signature that is selected by the value of the member ``CanDrvEqcArVersion`` from table ``CanIf_CanControllerIdUpToLowMap``.
    
    The member ``CanDrvFctTblIdx`` from table ``CanIf_CanControllerIdUpToLowMap`` determines the corresponding entry in table ``CanIf_CanDrvFctTbl``.
    
    This measure is only applicable if parameter ``CanIf/CanIfPrivateCfg/CanIfWakeupSupport`` is set to true, multiple CAN Drivers are configured and for at least one CAN Driver the parameter ``CanIf/CanIfCtrlDrvCfg/CanIfCtrlDrvArVersion`` is set to ``CAN_DRV_EQC_AR_403`` or ``CAN_DRV_EQC_AR_421`` and for at least one CAN Driver the parameter ``CanIf/CanIfCtrlDrvCfg/CanIfCtrlDrvArVersion`` is set to ``CAN_DRV_EQC_MSRC``.
    
    The table ``CanIf_CanDrvFctTbl`` can be found in CanIf_Lcfg.c.
    The table ``CanIf_CanControllerIdUpToLowMap`` can be found in CanIf_Lcfg.c for configuration variant Pre-Compile or in CanIf_PBcfg.c for configuration variant Post-Build Loadable.
    
    The following table lists the expected signatures:
    
    
    .. list-table::
       :header-rows: 1
    
       * - Value of CanDrvEqcArVersion
         - Signature of the function referred by CheckWakeupFct 
         - ..

       * - ..
         - At least one CAN controller of the CAN Driver has parameter Can/CanConfigSet/CanController/CanWakeupSupport set to true
         - All CAN controllers of the CAN Driver have parameter Can/CanConfigSet/CanController/CanWakeupSupport set to false 

       * - ``CAN_DRV_EQC_MSRC`` 
         - ``Std_ReturnType Can_<VendorId>_<VendorApiInfix>_CheckWakeup(uint8 Controller)`` 
         - ``Std_ReturnType Can_<VendorId>_<VendorApiInfix>_CheckWakeup(uint8 Controller)`` 

       * - ``CAN_DRV_EQC_AR_403``
         - ``Can_ReturnType Can_<VendorId>_<VendorApiInfix>_CheckWakeup(uint8 Controller)`` 
         - ``NULL_PTR`` 
    
    
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


.. smi:: SMI-1191144

    The user of MICROSAR Safe shall verify that the signature of the callout function referred by the function pointer ``CanIf_SetDynamicRxIdFctPtr`` has the following signature (the placeholder ``<name>`` represents the function's name):
    
    ``void <name>(PduIdType RxPduId, Can_IdType* CanId)``
    
    The function pointer can be found in file CanIf_Lcfg.c.
    
    This measure is only applicable if attribute  ``/MICROSAR/CanIf/CanIfPublicCfg/CanIfPublicSetDynamicRxIdApi`` is configured to ``TRUE``.
    
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


Safety requirements required from other components
**************************************************

.. smi:: SMI-551281

    This component requires sending and receiving functionality as an assumed safety requirement (TSR-111108) from other interfacing components.
    
    For components provided as part of MICROSAR Safe, the respective section of the Safety Manual for that component allows you identifying if this requirement is fulfilled.
    
    This requirement is only applicable if TSR-111108 is assumed as a safety requirement.


Dependencies to hardware
************************

This component does not use a direct hardware interface.

