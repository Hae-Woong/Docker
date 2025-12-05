Safety Manual Com
#################

Assumed safety requirements
***************************

This component assumes the following safety requirements:


.. list-table::
   :header-rows: 1
   :widths: 25 45 20 10

   * - ID
     - Safety Requirement
     - Associated TSR
     - ASIL 

   * - CREQ-103153 
     - Com shall provide a service to initialize the Com module. 
     - TSR-1 
     - ASIL D

   * - CREQ-103154 
     - Com shall provide a service to shutdown. 
     - TSR-1 
     - ASIL D

   * - CREQ-103157 
     - Com shall provide immediate and deferred signal processing. 
     - TSR-111108 
     - ASIL D

   * - CREQ-102790 
     - Com shall provide a service to receive I-PDUs. 
     - TSR-111108 
     - ASIL D

   * - CREQ-103160 
     - Com shall provide a callout to inhibit further processing of a reception I-PDU. 
     - TSR-111108 
     - ASIL D

   * - CREQ-103161 
     - Com shall provide at least one main function for reception processing. 
     - TSR-111108 
     - ASIL D

   * - CREQ-103162 
     - Com shall provide a service to transmit I-PDUs. 
     - TSR-111108 
     - ASIL D

   * - CREQ-103163 
     - Com shall provide a service that retries a failed transmit request. 
     - TSR-111108 
     - ASIL D

   * - CREQ-103164 
     - Com shall provide a service to guarantee a minimum distance between transmissions. 
     - TSR-111108 
     - ASIL D

   * - CREQ-103165 
     - Com shall provide a service  for trigger transmit. 
     - TSR-111108 
     - ASIL D

   * - CREQ-103166 
     - Com shall provide a callout to process the trigger transmit events. 
     - TSR-111108 
     - ASIL D

   * - CREQ-103167 
     - Com shall provide a callout to inhibit the transmission of a I-PDU. 
     - TSR-111108 
     - ASIL D

   * - CREQ-103168 
     - Com shall provide at least one main function for transmission processing. 
     - TSR-111108 
     - ASIL D

   * - CREQ-103169 
     - Com shall provide a service for N-Time transmissions. 
     - TSR-111108 
     - ASIL D

   * - CREQ-171934 
     - Com shall provide a service to transmit meta data. 
     - TSR-111108 
     - ASIL D

   * - CREQ-103195 
     - Com shall provide transmission modes for PDUs. 
     - TSR-111108 
     - ASIL D

   * - CREQ-103170 
     - Com shall provide a service for filter state dependent transmission mode selection. 
     - TSR-111108, TSR-100551 
     - ASIL D

   * - CREQ-103171 
     - Com shall provide a service to switch the transmission mode filter independent. 
     - TSR-111108, TSR-100551 
     - ASIL D

   * - CREQ-103158 
     - Com shall provide a service to receive transport protocol PDUs. 
     - TSR-111108 
     - ASIL D

   * - CREQ-103159 
     - Com shall provide a service to transmit transport protocol PDUs. 
     - TSR-111108 
     - ASIL D

   * - CREQ-103172 
     - Com shall provide a service to start and stop I-Pdu Groups. 
     - TSR-100551 
     - ASIL D

   * - CREQ-103173 
     - Com shall provide a service to activate and deactivate the reception deadline monitoring of I-Pdu Groups. 
     - TSR-100551 
     - ASIL D

   * - CREQ-103174 
     - Com shall provide a service for deadline monitoring of signal reception. 
     - TSR-111108 
     - ASIL D

   * - CREQ-103175 
     - Com shall provide a service to monitor the time between transmit and confirmation. 
     - TSR-111108 
     - ASIL D

   * - CREQ-103177 
     - Com shall provide a service to write signal values. 
     - TSR-111108 
     - ASIL D

   * - CREQ-103178 
     - Com shall provide a service to read signal values. 
     - TSR-111108 
     - ASIL D

   * - CREQ-103179 
     - Com shall provide signal transfer property. 
     - TSR-111108 
     - ASIL D

   * - CREQ-103180 
     - Com shall provide a service that performs Sign Extension. 
     - TSR-111108 
     - ASIL D

   * - CREQ-103181 
     - Com shall provide a service that performs Endianness Conversion. 
     - TSR-111108 
     - ASIL D

   * - CREQ-103182 
     - Com shall provide configurable filter algorithms. 
     - TSR-111108 
     - ASIL D

   * - CREQ-103183 
     - Com shall provide a service for reception filter. 
     - TSR-111108 
     - ASIL D

   * - CREQ-103185 
     - Com shall provide a service to detect if a signal was updated by the sender application. 
     - TSR-111108 
     - ASIL D

   * - CREQ-103186 
     - Com shall provide a service to detect the reception of an invalid signal value. 
     - TSR-111108 
     - ASIL D

   * - CREQ-103187 
     - Com shall provide a service to transmit an invalid signal value. 
     - TSR-111108 
     - ASIL D

   * - CREQ-103188 
     - Com shall provide a service to update dynamic length signals. 
     - TSR-111108 
     - ASIL D

   * - CREQ-103189 
     - Com shall provide a service to receive dynamic length signals. 
     - TSR-111108 
     - ASIL D

   * - CREQ-103190 
     - Com shall provide a service that guarantees that a collection of signal values is written without interruption. 
     - TSR-111108 
     - ASIL D

   * - CREQ-103191 
     - Com shall provide a service that guarantees that a collection of signals is read without interruption. 
     - TSR-111108 
     - ASIL D

   * - CREQ-1171 
     - Com shall provide a service to transmit a uint8-array based signal group. 
     - TSR-111108 
     - ASIL D

   * - CREQ-1172 
     - Com shall provide a service to receive a uint8-array based signal group. 
     - TSR-111108 
     - ASIL D

   * - CREQ-103192 
     - Com shall provide a main function for routing. 
     - TSR-111108 
     - ASIL D

   * - CREQ-102455 
     - Com shall provide Gateway routing timeout. 
     - TSR-111108 
     - ASIL D

   * - CREQ-115135 
     - Com shall provide signal transfer property for destination description. 
     - TSR-111108 
     - ASIL D

   * - CREQ-102454 
     - Com shall provide a basic routing mode. 
     - TSR-111108 
     - ASIL D

   * - CREQ-115134 
     - Com shall support Update Bits for description routing. 
     - TSR-111108 
     - ASIL D

   * - CREQ-103193 
     - Com shall provide a service to route a rx signal to one or multiple tx signals. 
     - TSR-111108 
     - ASIL D


This component in addition to the assumed safety requirements ensures freedom from interference with respect to memory (and timing) with up to ASIL D.

Configuration constraints
*************************

.. smi:: SMI-805694

    The configuration of mixed ASIL and QM partitions is not supported.


Additional verification measures
********************************

.. smi:: SMI-1104

    The user of MICROSAR Safe shall ensure that the ``SignalDataPtr`` passed to ``Com_ReceiveSignal`` and ``Com_ReceiveShadowSignal`` points to a valid buffer which matches the configured /MICROSAR/Com/ComConfig/ComSignal/ComSignalType or /MICROSAR/Com/ComConfig/ComGroupSignal/ComSignalType. In case of the ComSignalType UINT8_N the caller must ensure that the array size matches to the configured /MICROSAR/Com/ComConfig/ComSignal/ComSignalLength or /MICROSAR/Com/ComConfig/ComGroupSignal/ComSignalLength.
    
    This can be verified by review by comparing the type of the pointer passed to ``SignalDataPtr`` to the name of the define returned by ``Com_GetRxSigBufferOfRxBufferInfo(Index)``. The name of the define must contain the fitting name of the ComSignalType, e.g. for ComSignalType = UINT8 the define must contain the name RXSIGBUFFERUINT8.
    
    If the define is ``COM_RXSIGBUFFERARRAYBASED_RXSIGBUFFEROFRXBUFFERINFO`` additionally verify that the value of ``Com_GetRxSigBufferLengthOfRxBufferInfo(Index)`` is less or equal to the size (in bytes) of the array passed to ``SignalDataPtr``.
    
    The parameter of ``Com_GetRxSigBufferOfRxBufferInfo(idxRxBufferInfo)`` and ``Com_GetRxSigBufferLengthOfRxBufferInfo(idxRxBufferInfo)`` is the ``idxRxBufferInfo`` which you can get from ``idxRxBufferInfo = Com_GetRxBufferInfoIdxOfRxAccessInfo(Index)``. The parameter of  ``Com_GetRxBufferInfoIdxOfRxAccessInfo(Index)`` is the ``SignalId`` and can be found in the generated header files.
    
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


.. smi:: SMI-976309

    The user of MICROSAR Safe shall ensure that the generated MainFunctionRx uses the correct handle ID to call ``Com_MainFunctionRxInternal()``. 
    This can be verified by review. The MainFunctions are generated in the file Com_GeneratedMainFunctions.c. 

    The handle Id can be found in the configuration under: /MICROSAR/Com/ComConfig/ComMainFunctionRx/ComMainFunctionRxHandleId.

    The generated function name depends on the short name of the container /MICROSAR/Com/ComConfig/ComMainFunctionRx.

    The symbolic name value that is used as handleId for the call of ``Com_MainFunctionRxInternal()`` can be found in the file Com_Cfg.h.
    
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


.. smi:: SMI-976310

    The user of MICROSAR Safe shall ensure that the generated MainFunctionTx uses the correct handle ID to call ``Com_MainFunctionTxInternal()``. 

    This can be verified by review. The MainFunctions are generated in the file Com_GeneratedMainFunctions.c. 

    The handle Id can be found in the configuration under: /MICROSAR/Com/ComConfig/ComMainFunctionTx/ComMainFunctionTxHandleId.

    The generated function name depends on the short name of the container /MICROSAR/Com/ComConfig/ComMainFunctionTx.

    The symbolic name value that is used as handleId for the call of ``Com_MainFunctionTxInternal()`` can be found in the file Com_Cfg.h.
    
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


.. smi:: SMI-1171318

    The user of MICROSAR Safe shall ensure that the generated MainFunctionRouteSignals uses the correct handle ID to call ``Com_MainFunctionRouteSignalsInternal()``. 
    This can be verified by review. The MainFunctions are generated in the file Com_GeneratedMainFunctions.c. 
    The handle Id can be found in the configuration under: /MICROSAR/Com/ComConfig/ComMainFunctionRouteSignals/ComMainFunctionRouteSignalsHandleId.
    The generated function name depends on the short name of the container /MICROSAR/Com/ComConfig/ComMainFunctionRouteSignals.
    The symbolic name value that is used as handleId for the call of ``Com_MainFunctionRouteSignalsInternal()`` can be found in the file Com_Cfg.h.
    
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


.. smi:: SMI-522161

    The user of MICROSAR Safe shall verify that a ``ComIPduCallout`` does not return false unintended.
    The callouts are defined in the following parameter:

    - ``Com/ComConfig/ComIPdu/ComIPduCallout``
    
    Verification can be performed by review of the defined callouts. Check where the Com_Cot.h file is included and verify the implementation of the callouts defined in Com_Cot.h.
    
    This requirement is necessary to prevent the unintended stopping of the processing of the referenced Pdu.
    
    This requirement is only applicable if TSR-111108 is assumed as a safety requirement.


.. smi:: SMI-1089821

    The user of MICROSAR Safe shall verify that a ``Rx-I-PDU-Callout`` does not modify the ``PduInfoPtr`` unintended. 
    The callout is defined in the following parameter:

    - ``Com/ComConfig/ComIPdu/ComIPduCallout``
    
    The ``ComIPduDirection`` of the ComIPdu must be ``RECEIVE``.
    
    Verification can be performed by review of the defined callouts. Check where the Com_Cot.h file is included and verify the implementation of the callouts defined in Com_Cot.h.
    
    This requirement is necessary to prevent the receiving of wrong modified data, because the passed ``PduInfoPtr`` is used by the Com for processing of the Pdu. 
    
    This requirement is only applicable if TSR-111108 is assumed as a safety requirement.


.. smi:: SMI-545336

    The user of MICROSAR Safe shall verify that a ``Tx-I-PDU-Callout`` does only modify the ``PduInfoPtr`` as intended. The SduDataPtr and the SduLength must be consistent to each other.
    The callout can be defined in the following parameters:

    - ``Com/ComConfig/ComIPdu/ComIPduCallout``
    - ``Com/ComConfig/ComIPdu/ComIPduTriggerTransmitCallout``
    
    The ``ComIPduDirection`` of the ComIPdu must be ``SEND``.
    
    Verification can be performed by review of the defined callouts. Check where the Com_Cot.h file is included and verify the implementation of the callouts defined in Com_Cot.h.
    
    This requirement is necessary to prevent the transmitting of wrong modified data, because the passed ``PduInfoPtr`` is used by the Com for transmission of the Pdu. 
    
    This requirement is only applicable if TSR-111108 is assumed as a safety requirement.


Safety requirements required from other components
**************************************************

.. smi:: SMI-548478

    This component requires mode switching functionality as an assumed safety requirement (TSR-100551) from other interfacing components.
    
    For components provided as part of MICROSAR Safe, the respective section of the Safety Manual for that component allows you identifying if this requirement is fulfilled.
    
    This requirement is only applicable if TSR-100551 is assumed as a safety requirement.


.. smi:: SMI-548477

    This component requires sending and receiving functionality as an assumed safety requirement (TSR-111108) from other interfacing components.
    
    For components provided as part of MICROSAR Safe, the respective section of the Safety Manual for that component allows you identifying if this requirement is fulfilled.
    
    This requirement is only applicable if TSR-111108 is assumed as a safety requirement.


Dependencies to hardware
************************

This component does not use a direct hardware interface.

