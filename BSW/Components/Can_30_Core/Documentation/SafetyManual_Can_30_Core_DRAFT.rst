Safety Manual Can
#################

Assumed Safety Requirements
***************************

This component assumes the following safety requirements:

.. list-table::
   :header-rows: 1
   :widths: 25 45 20 10

   * - ID 
     - Safety Requirement 
     - Associated TSR 
     - ASIL 
   * - CREQ-CanCore-Initialization
     - Can Core shall provide a service to initialize itself.
     - TSR-1
     - ASIL D
   * - CREQ-CanCore-ModeChange
     - Can Core shall provide a service to change modes.
     - TSR-100551
     - ASIL D
   * - CREQ-CanCore-BusoffEventHandling
     - Can Core shall provide a service for bus off handling.
     - TSR-100551
     - ASIL D
   * - CREQ-CanCore-Transmit
     - Can Core shall provide a service to send a CAN frame.
     - TSR-111108
     - ASIL D
   * - CREQ-CanCore-TransmitConfirmation
     - Can Core shall provide a callout to confirm successful transmission of a CAN frame.
     - TSR-111108
     - ASIL D
   * - CREQ-CanCore-TransmitSupportFIFO
     - Can Core shall provide a service to support Tx FIFO functionality.
     - TSR-111108
     - ASIL D
   * - CREQ-CanCore-TransmitMultiplexed
     - Can Core shall provide a service to support multiplexed transmission.
     - TSR-111108
     - ASIL D
   * - CREQ-CanCore-ReceiveIndication
     - Can Core shall provide a callout to indicate successful reception of a CAN frame.
     - TSR-111108
     - ASIL D
   * - CREQ-CanCore-ReceiveQueue
     - Can Core shall provide a service to queue received CAN frames.
     - TSR-111108
     - ASIL D
   * - CREQ-CanCore-Interrupt
     - Can Core shall provide a service to disable and enable CAN interrupts.
     - TSR-101876
     - ASIL D
   * - CREQ-CanCore-PollingProcessing
     - Can Core shall provide services to trigger events without using interrupt.
     - TSR-100551, TSR-111108
     - ASIL D

This component in addition to the assumed safety requirements ensures freedom from interference with respect to memory and timing with up to ASIL D.

Configuration Constraints
*************************

.. smi:: SMI-1141633

    The user of MICROSAR Safe shall configure /MICROSAR/Can_30_Core/Can/CanGeneral/CanHardwareLoopCheck to TRUE.

    Verify that ``Can_30_Core_IsHardwareLoopCheckOfGeneralFeature(Index)`` in Can_30_Core_Cfg.h is defined to ``(((TRUE)) != FALSE)``.

    The user of MICROSAR Safe shall configure /MICROSAR/Can_30_Core/Can/CanGeneral/CanTimeoutDuration to an appropriate value.

    Verify that ``Can_30_Core_GetTimeoutDurationOfControllerConfig(Index)`` defined in Can_30_Core_Cfg.h returns the configured timeout duration in ticks for each index (CAN controller).
    Depending on the configuration ``Can_30_Core_GetTimeoutDurationOfControllerConfig(Index)`` is directly defined to ``CAN_30_CORE_OS_US2TICKS_<OsCounter>(<TimeoutDuration>)`` or
    it refers via ``Can_30_Core_GetControllerConfigOf[PB, PC]Config()[(Index)].TimeoutDurationOfControllerConfig`` to the table ``Can_30_Core_ControllerConfig`` in Can_30_Core_Lcfg.c or Can_30_Core_PBcfg.c.
    Each entry for an active CAN controller in table ``Can_30_Core_ControllerConfig`` for element ``TimeoutDuration`` have to be set to ``CAN_30_CORE_OS_US2TICKS_<OsCounter>(<TimeoutDuration>)``.

    The placeholder ``<TimeoutDuration>`` represents the value configured in /MICROSAR/Can_30_Core/Can/CanGeneral/CanTimeoutDuration in microseconds.

    The placeholder ``<OsCounter>`` represents the used Os counter. 
    In case the feature multi-partition (/MICROSAR/Can_30_Core/Can/CanGeneral/CanSupportMultipartition) is enabled the Os counter is CAN controller specific (/MICROSAR/Can_30_Core/Can/CanConfigSet/CanController/CanOsCounterRef) configured otherwise for the whole Can Core component (/MICROSAR/Can_30_Core/Can/CanGeneral/CanCounterRef).

    This requirement is necessary to ensure freedom from interference with respect to timing


.. smi:: SMI-CanCore-RamCheckConfig

    The user of MICROSAR Safe shall configure the following attributes to disable RAM check:
    
    - Set /MICROSAR/Can_30_Core/Can/CanGeneral/CanRamCheck to NONE.

    For each CAN controller

    - Set /MICROSAR/Can_30_Core/Can/CanConfigSet/CanController/CanRamCheck to NONE.

    Verify that ``Can_30_Core_GetRamCheckOfControllerConfig(Index)`` defined in Can_30_Core_Cfg.h returns ``CAN_30_CORE_RAMCHECK_NONE`` for each index (CAN controller).
    Depending on the configuration ``Can_30_Core_GetRamCheckOfControllerConfig(Index)`` is directly defined to ``CAN_30_CORE_RAMCHECK_NONE`` or
    it refers via ``Can_30_Core_GetControllerConfigOf[PB, PC]Config()[(Index)].RamCheckOfControllerConfig`` to the table ``Can_30_Core_ControllerConfig`` in Can_30_Core_Lcfg.c or Can_30_Core_PBcfg.c.
    Each entry in table ``Can_30_Core_ControllerConfig`` for element ``RamCheck`` have to be set to ``CAN_30_CORE_RAMCHECK_NONE``.

    This requirement is necessary to ensure TSR-1 and TSR-111108.


.. smi:: SMI-CanCore-WakeUpConfig

    The user of MICROSAR Safe shall configure the following attribute to disable wake up:

    For each CAN controller

    - Set MICROSAR/Can_30_Core/Can/CanConfigSet/CanController/CanWakeupSupport to FALSE.

    Verify that ``Can_30_Core_IsWakeupSupportOfControllerConfig(Index)`` defined in Can_30_Core_Cfg.h returns ``(((FALSE)) != FALSE)`` for each index (CAN controller).
    Depending on the configuration ``Can_30_Core_IsWakeupSupportOfControllerConfig(Index)`` is directly defined to ``(((FALSE)) != FALSE)`` or
    it refers via ``Can_30_Core_GetControllerConfigOf[PB, PC]Config()[(Index)].WakeupSupportOfControllerConfig`` to the table ``Can_30_Core_ControllerConfig`` in Can_30_Core_Lcfg.c or Can_30_Core_PBcfg.c.
    Each entry in table ``Can_30_Core_ControllerConfig`` for element ``WakeupSupport`` have to be set to ``FALSE``.

    This requirement is necessary to ensure TSR-100551.


.. smi:: SMI-CanCore-SilentModeConfig

    The user of MICROSAR Safe shall configure the following attribute to disable silent mode:

    - Set /MICROSAR/Can_30_Core/Can/CanGeneral/CanSilentModeSupport to FALSE.

    Verify that ``Can_30_Core_IsSilentModeOfGeneralFeature(Index)`` in Can_30_Core_Cfg.h is defined to ``(((FALSE)) != FALSE)``.

    This requirement is necessary to ensure TSR-100551 and TSR-111108.


.. smi:: SMI-CanCore-TxCancelConfig

    The user of MICROSAR Safe shall configure the following attributes to disable tx hardware and software cancellation:

    - Set /MICROSAR/Can_30_Core/Can/CanGeneral/CanHardwareCancellation to FALSE.
    - Set /MICROSAR/Can_30_Core/Can/CanGeneral/CanIdenticalIdCancellation to FALSE.
    - Set /MICROSAR/Can_30_Core/Can/CanGeneral/CanSoftwareCancellation to FALSE

    Verify that ``Can_30_Core_IsHardwareCancellationTxOfGeneralFeature(Index)``, ``Can_30_Core_IsIdenticalIdCancellationOfGeneralFeature(Index)`` and ``Can_30_Core_IsTxCancelInSwOfGeneralFeature(Index)``
    in Can_30_Core_Cfg.h are defined to ``(((FALSE)) != FALSE)``.

    This requirement is necessary to ensure TSR-111108.


.. smi:: SMI-CanCore-GenericPreTransmitConfig

    The user of MICROSAR Safe shall configure the following attribute to disable the generic pretransmit callout function:

    - Set /MICROSAR/Can_30_Core/Can/CanGeneral/CanGenericPreTransmit to FALSE.

    Verify that ``Can_30_Core_IsGenericPreTransmitOfGeneralFeature(Index)`` in Can_30_Core_Cfg.h is defined to ``(((FALSE)) != FALSE)``.

    This requirement is necessary to ensure TSR-111108.


.. smi:: SMI-CanCore-GenericConfirmationConfig

    The user of MICROSAR Safe shall configure the following attributes to disable the generic confirmation callout function:

    - Set /MICROSAR/Can_30_Core/Can/CanGeneral/CanGenericConfirmation to NONE.
    - Set /MICROSAR/Can_30_Core/Can/CanGeneral/CanGenericConfirmationAPI2 to FALSE.

    Verify that ``Can_30_Core_GetGenericConfirmationOfGeneralFeature(Index)`` in Can_30_Core_Cfg.h is defined to ``CAN_30_CORE_GENERICCONF_NONE``.

    This requirement is necessary to ensure TSR-111108.


.. smi:: SMI-CanCore-GenericPrecopyConfig

    The user of MICROSAR Safe shall configure the following attribute to disable the generic precopy callout function:

    - Set /MICROSAR/Can_30_Core/Can/CanGeneral/CanGenericPrecopy to FALSE.

    Verify that ``Can_30_Core_IsGenericPrecopyOfGeneralFeature`` in Can_30_Core_Cfg.h is defined to ``(((FALSE)) != FALSE)``.

    This requirement is necessary to ensure TSR-111108.


.. smi:: SMI-CanCore-MirrorModeConfig

    The user of MICROSAR Safe shall configure the following attribute to disable mirror mode:

    - /MICROSAR/Can_30_Core/Can/CanGeneral/CanMirrorModeSupport to FALSE.

    Verify that ``Can_30_Core_IsMirrorModeOfGeneralFeature(Index)`` in Can_30_Core_Cfg.h is defined to ``(((FALSE)) != FALSE)``.

    This requirement is necessary to ensure TSR-111108.


.. smi:: SMI-CanCore-InterruptLockConfig

    The user of MICROSAR Safe shall configure the following attribute to disable interrupt lock by application:

    - /MICROSAR/Can_30_Core/Can/CanGeneral/CanInterruptLock to DRIVER.

    Verify that ``Can_30_Core_GetInterruptLockOfGeneralFeature(Index)`` in Can_30_Core_Cfg.h is defined to ``CAN_30_CORE_ILOCK_DRIVER``.

    This requirement is necessary to ensure TSR-101876.


Additional Verification Measures
********************************

.. smi:: SMI-1141634

    The user of MICROSAR Safe shall verify depending on /MICROSAR/Can_30_Core/Can/CanGeneral/CanHardwareCancelByAppl is set to 

    - TRUE: 

      - that ``Can_30_Core_IsHardwareCancelByApplOfGeneralFeature(Index)`` in Can_30_Core_Cfg.h is defined to ``(((TRUE)) != FALSE)``.
      - that the implementation of ``Appl_30_CoreCanTimerStart()``, ``Appl_30_CoreCanTimerLoop()`` and ``Appl_30_CoreCanTimerEnd()`` follow the API definition given in the components header file, in order to end hardware loops.

    - FALSE:

      - that ``Can_30_Core_IsHardwareCancelByApplOfGeneralFeature(Index)`` in Can_30_Core_Cfg.h is defined to ``(((FALSE)) != FALSE)``.
      - that the related Os timer functionality ``GetCounterValue()`` and ``GetElapsedValue()`` return the timer value according to their definition in AUTOSAR.
    
    This requirement is necessary to ensure freedom from interference with respect to timing.


.. smi:: SMI-1003958

    The user of MICROSAR Safe shall verify that each partition has the same integrity level (parameter /MICROSAR/EcuC/EcucPartitionCollection/EcucPartition/ASIL), if multi-partition support is enabled (parameter /MICROSAR/Can_30_Core/Can/CanGeneral/CanSupportMultipartition).
    
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


.. smi:: SMI-1030730

    The user of MICROSAR Safe shall verify that the service functions returned by ``Can_30_Core_Get<Name of the function>FctOfExtensionConfig(Index)`` in Can_30_Core_Cfg.h 
    for each ``Index`` (vCan Driver component) matches the expected type, name and signature according to its definition described in the table below.

    Depending on the configuration ``Can_30_Core_Get<Name of the function>FctOfExtensionConfig(Index)`` is directly defined to the service function ``<vCanName>_<Name of the function>`` or 
    it refers via ``Can_30_Core_GetExtensionConfigOfPCConfig()[(Index)].<Name of the function>FctOfExtensionConfig`` to the table ``Can_30_Core_ExtensionConfig`` in Can_30_Core_Lcfg.c.
    Each vCan Driver component is represented by its own set of service functions represented by one entry in the table ``Can_30_Core_ExtensionConfig``.

    The following table lists the expected function types and their names with signature. The placeholder ``<vCanName>`` represents the function's prefix related to used vCan Driver component:

    .. list-table::
       :header-rows: 1

       * - Function type
         - Name and signature of the function

       * - ``vCan_InitControllerPowerOnFctType``
         - ``Can_ReturnType <vCanName>_InitControllerPowerOn(uint8 Controller)``

       * - ``vCan_InitControllerBeginFctType``
         - ``Can_ReturnType <vCanName>_InitControllerBegin(uint8 Controller, Can_30_Core_InitHandle initHandle, uint8 ramCheckPattern)``

       * - ``vCan_InitControllerFctType``
         - ``Can_ReturnType <vCanName>_InitController(uint8 Controller, Can_30_Core_InitHandle initHandle, uint8 ramCheckPattern)``

       * - ``vCan_InitControllerEndFctType``
         - ``Can_ReturnType <vCanName>_InitControllerEnd(uint8 Controller, Can_30_Core_InitHandle initHandle, uint8 ramCheckPattern)``

       * - ``vCan_InitControllerMailBoxFctType``
         - ``Can_ReturnType <vCanName>_InitControllerMailBox(uint8 Controller, Can_HwHandleType mailboxHandle, uint8 ramCheckPattern)``

       * - ``vCan_CheckControllerMailboxFctType``
         - ``Can_ReturnType <vCanName>_CheckControllerMailbox(uint8 Controller, Can_HwHandleType mailboxHandle, uint8 ramCheckPattern)``

       * - ``vCan_InitControllerMailboxDeactivateFctType``
         - ``void <vCanName>_InitControllerMailboxDeactivate(uint8 Controller, Can_HwHandleType mailboxHandle)``

       * - ``vCan_GetStatusBeginFctType``
         - ``void <vCanName>_GetStatusBegin(uint8 Controller)``

       * - ``vCan_IsHwBusOffFctType``
         - ``boolean <vCanName>_IsHwBusOff(uint8 Controller)``

       * - ``vCan_IsHwPassiveFctType``
         - ``boolean <vCanName>_IsHwPassive(uint8 Controller)``

       * - ``vCan_IsHwWarningFctType``
         - ``boolean <vCanName>_IsHwWarning(uint8 Controller)``

       * - ``vCan_ModeTransitionFctType``
         - ``Can_ReturnType <vCanName>_ModeTransition(uint8 Controller, uint8 transitionRequest, boolean ContinueBusOffRecovery, boolean doRamCheck)``

       * - ``vCan_ErrorHandlingBeginFctType``
         - ``void <vCanName>_ErrorHandlingBegin(uint8 Controller)``

       * - ``vCan_ErrorStatePassiveTransitionOccurredFctType``
         - ``boolean <vCanPre>_ErrorStatePassiveTransitionOccurred(uint8 Controller)``

       * - ``vCan_ErrorNotificationHandlingFctType``
         - ``void <vCanName>_ErrorNotificationHandling(uint8 Controller)``

       * - ``vCan_BusOffOccurredFctType``
         - ``boolean <vCanName>_BusOffOccurred(uint8 Controller)``

       * - ``vCan_OverrunOccurred``
         - ``void <vCanName>_OverrunOccurred(uint8 Controller)``

       * - ``vCan_ErrorHandlingEndFctType``
         - ``void <vCanName>_ErrorHandlingEnd(uint8 Controller)``

       * - ``vCan_GetControllerRxErrorCounterFctType``
         - ``uint16 <vCanName>_GetControllerRxErrorCounter(uint8 Controller)``

       * - ``vCan_GetControllerTxErrorCounterFctType``
         - ``uint16 <vCanName>_GetControllerTxErrorCounter(uint8 Controller)``

       * - ``vCan_WakeUpOccurredFctType``
         - ``boolean <vCanName>_WakeUpOccurred(uint8 Controller)``

       * - ``vCan_WakeUpHandling``
         - ``void <vCanName>_WakeUpHandling(uint8 Controller)``

       * - ``vCan_DisableControllerInterruptsFctType``
         - ``void <vCanName>_DisableControllerInterrupts(uint8 Controller)``

       * - ``vCan_RestoreControllerInterruptsFctType``
         - ``void <vCanName>_RestoreControllerInterrupts(uint8 Controller)``

       * - ``vCan_RxPollingFctType``
         - ``void <vCanName>_RxPolling(uint8 Controller)``

       * - ``vCan_TxPollingFctType``
         - ``void <vCanName>_TxPolling(uint8 Controller)``

       * - ``vCan_TxStartFctType``
         - ``Can_ReturnType <vCanName>_TxStart(uint8 Controller, Can_HwHandleType mailboxHandle, Can_HwHandleType mailboxElement, Can_PduInfoPtrType PduInfoPtr)``

       * - ``vCan_TxCancelInHwFctType``
         - ``void <vCanName>_TxCancelInHw(uint8 Controller, Can_HwHandleType mailboxHandle, Can_HwHandleType mailboxElement)``

       * - ``vCan_TxConfIsMsgTransmittedFctType``
         - ``boolean <vCanName>_TxConfIsMsgTransmitted(uint8 Controller, Can_HwHandleType mailboxHandle, Can_HwHandleType mailboxElement)``

       * - ``vCan_InitPowerOnFctType``
         - ``void <vCanName>_InitPowerOn(void)`` OR ``NULL_PTR``

    The value ``NULL_PTR`` is not expected to be returned by ``Can_30_Core_Get<Name of the function>FctOfExtensionConfig(Index)`` (directly or as part of table ``Can_30_Core_ExtensionConfig``)
    except for ``<vCanName>_InitPowerOn`` if the ``vCanInitPowerOnCallSupport`` feature is set to false or does not exist in the corresponding vCan Driver component BSWMD file.

    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


.. smi:: SMI-CanCore-HardwareLoopCheckSourceVerification

    The user of MICROSAR Safe must ensure that the used vCan Driver components do NOT use the same value for their hardware loop check sources as the CAN core's hardware loop check source ``CAN_30_CORE_LOOP_MODE_CHANGE``.

    The value of the hardware loop check source of the CAN Core is defined as ``CAN_30_CORE_LOOP_MODE_CHANGE`` in Can_30_Core_Cfg.h.

    Each value of a hardware loop check source of a used vCan Driver component is defined as ``<LOOP_NAME>`` in vCan_30_<VendorApiInfix>_Cfg.h.

    The placeholder ``<LOOP_NAME>`` represents the short name of a configuration container /MICROSAR/vCan_30_<VendorApiInfix>/vCan/vCanGeneral/vCanHardwareLoopCheck in capital letters.

    This requirement is necessary to ensure TSR-100551.


.. smi:: SMI-CanCore-ChangeBaudrateVerification

    The user of MICROSAR Safe must ensure that the following baudrate change service functions are never called:

    - ``Std_ReturnType Can_30_Core_SetBaudrate(uint8 Controller, uint16 BaudRateConfigID)``
    - ``Std_ReturnType Can_30_Core_ChangeBaudrate(uint8 Controller, uint16 Baudrate)``

    .. Service functions are available also if the corresponding feature is configured to disabled, because the static implementation use no preprocessor optimization.

    This requirement is necessary to ensure TSR-100551 or TSR-111108.


.. smi:: SMI-CanCore-CanInterruptLockVerification

    The user of MICROSAR Safe must ensure that only the following service functions are used to disable/enable CAN controller interrupts:

    - ``void Can_30_Core_DisableControllerInterrupts(uint8 Controller)``
    - ``void Can_30_Core_EnableControllerInterrupts(uint8 Controller)``

    The user of MICROSAR Safe must ensure that these service functions are used consistently.
    This means that the number of calls to enable the CAN controller interrupts must be equal to the number of calls to disable the CAN controller interrupts in order to re-enable the CAN controller interrupts.

    This requirement is necessary to ensure TSR-101876, TSR-100551 or TSR-111108.


Safety Requirements Required From Other Components
**************************************************

.. smi:: SMI-CanCore-vCanDriverInit

    This component requires the initialization functionality as an assumed safety requirement (TSR-1) from the interfacing vCan Driver components.

    If the vCan Driver components from MICROSAR Safe are used, this dependency is fulfilled.

    This requirement is only applicable if TSR-1 is assumed as a safety requirement.


.. smi:: SMI-CanCore-ModeChange

    This component requires the CAN controller mode change functionality as an assumed safety requirement (TSR-100551) from the interfacing vCan Driver components.

    If the vCan Driver components from MICROSAR Safe are used, this dependency is fulfilled.

    This requirement is only applicable if TSR-100551 is assumed as a safety requirement.


.. smi:: SMI-CanCore-DataConsistency

    This component requires data consistency functionality as an assumed safety requirement (TSR-101876) from the interfacing SchM component.

    If the SchM component from MICROSAR Safe is used, this dependency is fulfilled.

    This requirement is only applicable if TSR-100551, TSR-111108 or TSR-101876 is assumed as a safety requirement.


.. smi:: SMI-CanCore-TimeMeasurement

    This component requires a time measurement functionality as an assumed safety requirement (TSR-6) from the interfacing application code (depending on the configuration see SMI-1141634) or Os component.

    If the Os component from MICROSAR Safe is used, this dependency is fulfilled.

    This requirement is only applicable if TSR-100551 is assumed as a safety requirement.


.. smi:: SMI-CanCore-MainFunctionScheduling

    This component requires the cyclic trigger of main function execution as an assumed safety requirement (TSR-6) from the interfacing SchM component.

    If the SchM component from MICROSAR Safe is used, this dependency is fulfilled.

    This requirement is only applicable if TSR-100551 is assumed as a safety requirement.
    This requirement is only applicable if TSR-111108 is assumed as a safety requirement and if Tx or Rx events are configured as polling processing (at least one CanHardwareObject has /MICROSAR/Can_30_Core/Can/CanConfigSet/CanHardwareObject/CanHwProcessing set to TRUE).


.. smi:: SMI-CanCore-vCanDriverBusOffInterrupt

    This component requires the triggering of bus off events via interrupt as an assumed safety requirement (TSR-100551) from the interfacing vCan Driver components.

    If the vCan Driver components from MICROSAR Safe are used, this dependency is fulfilled.

    This requirement is only applicable if TSR-100551 is assumed as a safety requirement and 
    if bus off event processing is configured as interrupt (at least one CAN controller has /MICROSAR/Can_30_Core/Can/CanConfigSet/CanController/CanBusoffProcessing set to INTERRUPT) for the responsible vCan Driver component.


.. smi:: SMI-CanCore-Communication

    This component requires the general CAN communication functionality as an assumed safety requirement (TSR-111108) from the interfacing VStdLib component and vCan Driver components.

    If the VStdLib component from MICROSAR Safe is used, this dependency is fulfilled.
    If the vCan Driver components from MICROSAR Safe are used, this dependency is fulfilled.

    This requirement is only applicable if TSR-111108 is assumed as a safety requirement.

    .. There is no currently no specific TSR for the VStdLib. Therefore the VStdLib fulfils all TSRs.


.. smi:: SMI-CanCore-vCanDriverComInterrupts

    This component requires the triggering of Rx and Tx events via interrupts as an assumed safety requirement (TSR-111108) from the interfacing vCan Driver components.

    If the vCan Driver components from MICROSAR Safe are used, this dependency is fulfilled.

    This requirement is only applicable if TSR-111108 is assumed as a safety requirement and 
    if Rx or Tx event processing is configured as interrupt (at least one CanHardwareObject has /MICROSAR/Can_30_Core/Can/CanConfigSet/CanHardwareObject/CanHwProcessing set to FALSE) for the responsible vCan Driver component.


.. smi:: SMI-CanCore-MultiplexedTx

    This component requires the multiplexed transmission functionality as an assumed safety requirement (TSR-111108) from the interfacing vCan Driver components.

    If the vCan Driver components from MICROSAR Safe are used, this dependency is fulfilled.

    This requirement is only applicable if TSR-111108 is assumed as a safety requirement and
    if this functionality is configured (at least one CanHardwareObject has /MICROSAR/Can_30_Core/Can/CanConfigSet/CanHardwareObject/CanObjectMultiplexedTransmission set to TRUE) for the responsible vCan Driver component.


.. smi:: SMI-CanCore-FifoTx

    This component requires the FIFO transmission functionality as an assumed safety requirement (TSR-111108) from the interfacing vCan Driver components.

    If the vCan Driver components from MICROSAR Safe are used, this dependency is fulfilled.

    This requirement is only applicable if TSR-111108 is assumed as a safety requirement and
    if this functionality is configured (at least one CanHardwareObject has /MICROSAR/Can_30_Core/Can/CanConfigSet/CanHardwareObject/CanObjectHwFifo set to TRUE) for the responsible vCan Driver component.


.. smi:: SMI-CanCore-CanInterruptLock
 
    This component requires the functionality to enable/disable CAN controller interrupts as an assumed safety requirement (TSR-101876) from the interfacing vCan Driver components.
 
    If the vCan Driver components from MICROSAR Safe are used, this dependency is fulfilled.

    This requirement is only applicable if TSR-100551, TSR-111108 or TSR-101876 is assumed as a safety requirement.


Dependencies To Hardware
************************

This component does not use a direct hardware interface.