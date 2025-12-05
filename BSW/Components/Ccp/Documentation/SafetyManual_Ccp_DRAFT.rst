Safety Manual CCP
#################

Safety features
***************

This component does not provide safety features.

.. smi:: SMI-33841

    This component is only partly developed according to ASIL development process. This part includes Safe disabling of the Ccp.
    
    The main part of this component is developed according to a QM development process. 
    Thus, this component shall only be enabled in an operating mode that do not impose risk for the health of persons.


Configuration constraints
*************************

.. smi:: SMI-33840

    The user of MICROSAR Safe shall configure the following:

    - Set ``/MICROSAR/Ccp/CcpGeneral/CcpControl`` to TRUE.
    
    The user of MICROSAR Safe shall verify that the corresponding configuration switch is set in the Ccp:
    
    
    .. list-table::
       :header-rows: 1
    
       * - File 
         - Define 
         - STD_ON/STD_OFF 

       * - Ccp_Cfg.h 
         - CCP_CONTROL 
         - \- 


.. smi:: SMI-33839

    The user of MICROSAR Safe shall set the global variable Ccp_ControlState to the constant kCcp_Control_Enable to activate the component and to kCcp_Control_Disable to deactivate the component.
    
    Activation and deactivation shall only be performed by a software component that is developed according to the highest ASIL that is allocated to the ECU.
    CCP shall only be activated in an operating mode that does not impose risk for the health of persons. 
    Note: CCP is active by default.


.. smi:: SMI-33838

    The user of MICROSAR Safe shall make the following memory sections **read-only** for the CCP component as well as all other software with an ASIL lower than the highest ASIL allocated to the ECU:

    - *CCP_START_SEC_VAR_INIT_UNSPECIFIED_SAFE*


Additional verification measures
********************************

.. smi:: SMI-33837

    The user of MICROSAR Safe shall verify during integration testing that CCP is disabled during normal operation.


Safety features required from other components
**********************************************

.. smi:: SMI-33836

    This component requires an operating system with enabled memory partitioning.


Dependencies to hardware
************************

This component does not use a direct hardware interface.

