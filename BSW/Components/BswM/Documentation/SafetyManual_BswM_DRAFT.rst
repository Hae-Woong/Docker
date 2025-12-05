Safety Manual BswM
##################

Assumed Safety requirements
***************************

.. smi:: SMI-546762

    This component assumes the following safety requirements:
    
    
    .. list-table::
       :header-rows: 1
       :widths: 25 45 20 10
    
       * - ID
         - Safety Requirement
         - Associated TSR
         - ASIL 

       * - CREQ-1020
         - BswM shall provide a service to initialize itself
         - TSR-1, TSR-100551
         - ASIL D

       * - CREQ-1232
         - BswM shall provide a service to deinitialize itself
         - TSR-1, TSR-100551
         - ASIL D

       * - CREQ-1202
         - BswM shall provide a service to enable/disable the arbitration of a rule
         - TSR-100551
         - ASIL D

       * - CREQ-255428
         - BswM shall provide a service to request BswM modes
         - TSR-100551
         - ASIL D

       * - CREQ-255429
         - BswM shall provide a service to request CanSM modes
         - TSR-100551
         - ASIL D

       * - CREQ-255430
         - BswM shall provide a service to request ComM modes
         - TSR-100551
         - ASIL D

       * - CREQ-255431
         - BswM shall provide a service to request Dcm modes
         - TSR-100551
         - ASIL D

       * - CREQ-DoIP_ModeRequest
         - BswM shall provide a service to request DoIP modes
         - TSR-100551
         - ASIL D

       * - CREQ-DoIPInt_ModeRequest
         - BswM shall provide a service to request DoIPInt modes
         - TSR-100551
         - ASIL D

       * - CREQ-255432
         - BswM shall provide a service to request EcuM modes
         - TSR-100551
         - ASIL D

       * - CREQ-255433
         - BswM shall provide a service to request EthIf modes
         - TSR-100551
         - ASIL D

       * - CREQ-255434
         - BswM shall provide a service to request EthSM modes
         - TSR-100551
         - ASIL D

       * - CREQ-255435
         - BswM shall provide a service to request FrSM modes
         - TSR-100551
         - ASIL D

       * - CREQ-255436
         - BswM shall provide a service to request J1939DcM modes
         - TSR-100551
         - ASIL D

       * - CREQ-255437
         - BswM shall provide a service to request J1939Nm modes
         - TSR-100551
         - ASIL D

       * - CREQ-255438
         - BswM shall provide a service to request LinSM modes
         - TSR-100551
         - ASIL D

       * - CREQ-255439
         - BswM shall provide a service to request LinTP modes
         - TSR-100551
         - ASIL D

       * - CREQ-255440
         - BswM shall provide a service to request NM modes
         - TSR-100551
         - ASIL D

       * - CREQ-255441
         - BswM shall provide a service to request NvM modes
         - TSR-100551
         - ASIL D

       * - CREQ-255442
         - BswM shall provide a service to request PduR modes
         - TSR-100551
         - ASIL D

       * - CREQ-255443
         - BswM shall provide a service to request Sd modes
         - TSR-100551
         - ASIL D

       * - CREQ-256593
         - BswM shall provide a service to request Swc modes
         - TSR-100551
         - ASIL D

       * - CREQ-255444
         - BswM shall provide a service to request WdgM modes
         - TSR-100551
         - ASIL D

       * - CREQ-1234
         - BswM shall have configurable behavior to react on mode request
         - TSR-100551
         - ASIL D


This component in addition to the assumed safety requirements ensures freedom from interference with respect to memory and timing with up to ASIL D.

Configuration constraints
*************************

.. smi:: SMI-1156730

    The user of MICROSAR Safe shall make the following memory section read-only for the software with an ASIL lower than the highest ASIL, allocated to the ECU:

    - /MICROSAR/BswM_Impl/ResourceConsumption/VAR_NO_INIT_UNSPECIFIED


Additional verification measures
********************************

This component does not require additional verification measures.

Safety requirements required from other components
**************************************************

.. smi:: SMI-553189

    This component requires mode switching functionality as an assumed safety requirement (TSR-100551) from other interfacing components.
    
    For components provided as part of MICROSAR Safe, the respective section of the Safety Manual for that component allows you identifying if this requirement is fulfilled.
    
    This requirement is only applicable if TSR-100551 is assumed as a safety requirement.


Dependencies to hardware
************************

This component does not use a direct hardware interface.

