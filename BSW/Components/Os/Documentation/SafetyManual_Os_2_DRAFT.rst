Safety Manual Os (TriCore)
**************************

Assumed safety requirements
===========================

No additional safety requirements are provided.

Configuration constraints
=========================

No additional configuration constraints are required.

Additional verification measures
================================

.. smi:: SMI-474

    The user of MICROSAR Safe shall verify the values of the struct member *ProgramCounterRegister* within the generated structs with the names *OsCfg_Hal_Core_* *<Core Name>*.
    
    When *CoreId* of the struct is *0*, the member *ProgramCounterRegister* shall be *0* as well.
    
    For all other cores *ProgramCounterRegister* shall be *OS_HAL_CORE<pysical Core ID>_PC*.
    
    <physical Core ID> may have the values 0,1,2,3,4,6 (depending on the number of cores on your derivative)
    
    NOTE: the 5th core has the physical core ID 6


.. smi:: SMI-475

    The user of MICROSAR Safe shall verify the values of the struct member *DBGSRRegister* within the generated structs with the names *OsCfg_Hal_Core_* *<Core Name>*.
    
    When *CoreId* of the struct is *0*, the member *DBGSRRegister* shall be *0* as well.
    
    For all other cores *DBGSRRegister* shall be *OS_HAL_CORE<physical Core ID>_DBGSR*.
    
    <physical Core ID> may have the values 0,1,2,3,4,6 (depending on the number of cores on your derivative)
    
    NOTE: the 5th core has the physical core ID 6


.. smi:: SMI-46520

    The user of MICROSAR Safe shall verify the values of the struct member *SYSCON_CORECON_Register* within the generated structs with the names *OsCfg_Hal_Core_* *<Core Name>*.
    
    When *CoreId* of the struct is *0*, the member *SYSCON_CORECON_Register* shall be *0* as well.
    
    For all other cores *SYSCON_CORECON_Register* shall be *OS_HAL_CORE<physical Core ID>_SYSCON_CORECON*.
    
    <physical Core ID> may have the values 0,1,2,3,4,6 (depending on the number of cores on your derivative)
    
    NOTE: the 5th core has the physical core ID 6


.. smi:: SMI-478

    The user of MICROSAR Safe shall verify the timer configuration for its correctness.
    
    If the *GPT channel 0* is configured, the following attributes must be generated as follows:
    
    - *CounterRegisterAddress      = OS_HAL_TIMER_GPT_BASE + OS_HAL_TIMER_GPT_T3_OFFSET*
    - *CounterConfRegisterAddress  = OS_HAL_TIMER_GPT_BASE + OS_HAL_TIMER_GPT_T3CON_OFFSET*
    - *ReloadRegisterAddress       = OS_HAL_TIMER_GPT_BASE + OS_HAL_TIMER_GPT_T2_OFFSET*
    - *ReloadConfRegisterAddress   = OS_HAL_TIMER_GPT_BASE + OS_HAL_TIMER_GPT_T2CON_OFFSET*
    - *InterruptSRCRegisterAddress = OS_HAL_INT_SRC_BASE + OS_HAL_INT_SRC_GPT_CH0_OFFSET*
    
    If the *GPT channel 1* is configured, the following attributes must be generated as follows:
    
    - *CounterRegisterAddress      = OS_HAL_TIMER_GPT_BASE + OS_HAL_TIMER_GPT_T6_OFFSET*
    - *CounterConfRegisterAddress  = OS_HAL_TIMER_GPT_BASE + OS_HAL_TIMER_GPT_T6CON_OFFSET*
    - *ReloadRegisterAddress       = OS_HAL_TIMER_GPT_BASE + OS_HAL_TIMER_GPT_CAPREL_OFFSET*
    - *ReloadConfRegisterAddress   = OS_HAL_TIMER_GPT_BASE + OS_HAL_TIMER_GPT_CAPREL_OFFSET*
    - *InterruptSRCRegisterAddress = OS_HAL_INT_SRC_BASE + OS_HAL_INT_SRC_GPT_CH1_OFFSET*


.. smi:: SMI-479

    The user of MICROSAR Safe shall verify the timer configuration for its correctness.
    If an STM channel is configured, the following attributes must generated as follows, where <X> (0, 1, 2, 3, 4, 5) is STM ID and <Y> (0, 1) is the compare channel ID:
    
    - CompareRegisterAddress      = OS_HAL_TIMER_STM<X>_BASE + OS_HAL_TIMER_STM_CMP<Y>_OFFSET
    - CMCONRegisterAddress        = OS_HAL_TIMER_STM<X>_BASE + OS_HAL_TIMER_STM_CMCON_OFFSET
    - InterruptSRCRegisterAddress = OS_HAL_INT_SRC_BASE + OS_HAL_INT_SRC_STM<X>_CH<Y>_OFFSET
    - ICRRegisterAddress          = OS_HAL_TIMER_STM<X>_BASE + OS_HAL_TIMER_STM_ICR_OFFSET


Safety requirements required from other components
==================================================

No additional safety requirements are required from other components.

Dependencies to hardware
========================

The safety manual AURIX Safety Manual AP32224, v1.3 was used during development (for TC2xx derivatives). 
The safety manual AURIX TC3xx Safety Manual, v1.03 was used during development (for TC3xx derivatives). 
This OS does not implement any requirement from that safety manual.

It is assumed that the requirements related to OS functionality are related to latent faults only and ASIL D is still achievable without implementation by the OS. Such implementation would cause significant runtime overhead.

.. smi:: SMI-470

    The user of MICROSAR Safe shall initialize register LCX (Free CSA List Limit Pointer) some positions in front of the last CSA of the Free Context List to ensure that there are sufficient CSAs available upon exception handling when FCX reaches LCX. 
    
    The exception handlers of MICROSAR Safe require CSAs for proper execution (function calls). For execution of "Free Context List Depletion Exception (FCX = LCX)" it is therefore necessary that sufficient unused CSAs are available when FCX reaches LCX.  


.. smi:: SMI-471

    The user of MICROSAR Safe shall initialize register BTV prior to initialization of this component. 
    
    The value of BTV shall not be modified after initialization of the OS.


.. smi:: SMI-473

    The user of MICROSAR Safe shall initialize register BIV prior to initialization of this component.  
    
    The value of BIV.VSS shall be 0.
    The value of BIV must never be modified after initialization of the OS.


.. smi:: SMI-800

    This component requires exclusive access to the hardware registers of the unit it is intended to control. See the technical reference for the hardware register names and used hardware manuals.
    
    The user of MICROSAR Safe shall ensure that access to registers of the interrupt controller does not interfere with register contents for other ISRs.
    For the implementation of category 1 ISRs it might be necessary for the user software to access registers of the interrupt controller. 


.. smi:: SMI-1038

    The user of MICROSAR Safe shall initialize the System MPU by a lockstep core. The System MPU must not be accessed by non-lockstep cores.
    
    MICROSAR OS does not use the System MPU to achieve freedom of interference between cores. This has to be done by the application. 


