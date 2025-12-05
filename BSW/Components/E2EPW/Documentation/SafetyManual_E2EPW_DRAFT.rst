Safety Manual E2E Protection Wrapper
####################################


.. list-table::
   :header-rows: 1

   * - Term/Abbreviation 
     - Definition

   * - E2EConfig
     - E2E Protection Wrapper configuration file. The E2EConfig file is a data file containing information about the protected areas within I-PDUs of a given AUTOSAR system that are to be protected by the E2Elib and the E2EPW and how they are to be protected. It serves as input for the E2EPWG.

   * - E2EPWG
     - E2E Protection Wrapper Generator. It takes an E2EConfig file as input and generates the E2E Protection Wraper code.

   * - node 
     - node name (also SWC)

   * - port or p 
     - port

   * - pde or o 
     - protected data element (PDE)

   * - direc
     - direction for which a PDE-type is configured (*rx* or *tx*)


 
The shortcuts *p* and *o* are used for the E2EPW API functions. For other identifiers (like filenames) the notation *port* and *pde* is preferred. 
  
Some identifiers in this document include concatenations of these terms. 
E.g., *E2EPW_Write_p_o_direc ()* (i.e., ".. for a combination of some port *p*, some data element *o* and some direction *direc*).  
A protected area (PA) is the container of configuration data for a certain protected data element (PDE).

Safety features
***************

.. smi:: SMI-143

    This component provides the following safety features:

    - Creation of end-to-end protection for data
    - Check of end-to-end protection for data


Configuration constraints
*************************

.. smi:: SMI-124

    The user of MICROSAR Safe shall correctly select and configure I-PDUs and their signal groups (protected areas) that are to be protected by the end-to-end protection. That is, the selection and configuration shall be made according to the communication safety requirements of the system. This includes also the selection of the E2E profile.


.. smi:: SMI-125

    The user of MICROSAR Safe shall only configure PAs for the same ECU in one E2EConfig.
    If source code needs to be generated for different ECUs, then an individual E2EConfig file must be defined for each ECU.


Additional verification measures
********************************

Additional verification of E2EConfig file
=========================================

.. smi:: SMI-126

    The user of MICROSAR Safe shall ensure that the length of the following identifiers can be uniquely identified by the compiler and linker.
    
    The following strings are internal identifiers: 

    - E2EPW_Marshal_pde_h
    - E2EPW_CheckDeserial_pde_h
    - E2EPW_node_port_pde_RX_H
    - E2EPW_node_port_pde_TX_H
    
    The following strings are external identifiers:

    - E2EPW_Marshal_pde
    - E2EPW_CheckDeserial_pde
    - E2EPW_Init_p_o_rx
    - E2EPW_Init_p_o_tx
    - E2EPW_Get_SenderState_p_o
    - E2EPW_Get_ReceiverState_p_o
    - E2EPW_Write_p_o
    - E2EPW_Read_p_o
    
    This requirement applies for all PAs in the E2EConfig with *node*, *p(ort)* and *pde/o*. The number of significant initial characters of the compiler and linker can usually be found in their documentation.


.. smi:: SMI-127

    The user of MICROSAR Safe shall verify that all PAs in an E2EConfig with the same *PDE_Name* have the same values in the following fields:

    - PDE_Type
    - Byte_Order_CPU
    - Bit_Order
    - Bit_Counting
    - Unused_Bit_Value
    - Category
    - Data_Length
    - Is_Opaque


.. smi:: SMI-128

    The user of MICROSAR Safe shall verify that each signal in a PA is also be defined in all other PAs with same *PDE_Name*, and the signals must also have the same values in the following fields:

    - Signal_Name
    - Signal_Type
    - Signal_Property
    - Byte_Order
    - Bit_Length
    - Bit_Position


.. smi:: SMI-131

    The user of MICROSAR Safe shall verify that the attributes of each PA are defined in the E2EConfig as intended.
    
    .. list-table::
       :header-rows: 1
    
       * - Attribute
         - Requirement

       * - *PDE_Type* 
         - *PDE_Type* shall equal the data type of the corresponding PDE in the AUTOSAR application and the RTE functions that are invoked by the E2EPW code.

       * - *Node_Name* 
         - *Node_Name* shall equal the name of the SWC. The preprocessor sets *Node_Name* to the SWC's name. If you do not use the preprocessor or manipulate the E2EConfig file, then make sure that the requirement is still met.

       * - *Direction* 
         - *Direction* shall equal the direction expected for the corresponding PDE in the RTE.

       * - *Byte_Order_CPU* 
         - *Byte_Order_CPU* shall equal the byte order of the used CPU.

       * - *Unused_Bit_Value* 
         - *Unused_Bit_Value* shall equal the fillbit configured in the COM layer for the I-PDU representation of the PDE.

       * - *Check_DeSerial* 
         - *Check_DeSerial* shall be set to YES if it is required that the E2EPW code checks for deserialization errors on the receiver side.

       * - *Includes_H* 
         - *Includes_H* shall list the header file that defines the data type of the PDE, the header file that defines the type Rte_Instance (if used) and the header file that defines RTE_E_OK.

       * - *Includes_C* 
         - *Includes_C* shall list the header file(s) that declare(s) the functions Rte_Read_p_o (), Rte_Write_p_o () and Rte_IsUpdated_p_o () (if used). This requirement only applies if the header files are not already included in the field "Includes_H".

       * - *Category* 
         - *Category* shall equal the short name of the E2Elib profile selected for the PDE.

       * - *Data_Length* 
         - *Data_Length* shall equal the bit-length of the I-PDU representation of the PDE.

       * - *Data_ID* and *Data_ID_List* 
         - *Data_IDs and _Data_ID_List* shall fulfill the requirements of the respective E2E profile.

       * - *Max_Delta_Counter_Init* 
         - *Max_Delta_Counter_Init* shall equal the required initial value of MaxDeltaCounter of the E2E library communication state for the PDE.

       * - *Data_Id_Mode* 
         - *Data_Id_Mode* shall equal the requirements from the respective E2E profile.

       * - *Offset* 
         - *Offset* shall equal the communication specification by the OEM.

       * - *Max_No_New_Or_Repeated_Data* 
         - *Max_No_New_Or_Repeated_Data* shall equal the requirements from the respective E2E profile.


.. smi:: SMI-130

    The user of MICROSAR Safe shall verify that each signal that is configured in a PA shall be assigned to a signal in the corresponding PDE in the RTE. Vice versa, each signal in the PDE in the RTE shall be assigned to a signal in the corresponding PA.
    
    If a signal in the RTE is not configured for protection, it will not be protected.
    Note that the set of signals in the PA represents the corresponding signal group in the PDE.
    
    
    .. list-table::
       :header-rows: 1
    
       * - Attribute 
         - Requirement 

       * - *Port_Name* 
         - *Port_Name* shall equal the Port Prototype name that is used by the RTE for the related data element in the RTE.

       * - *VDP_Name* 
         - *VDP_Name* shall always be equal to *PDE_Name*. *VDP_Name* shall equal the VDP name that is used by the RTE for the related data element in the RTE.

       * - *Is_Opaque* 
         - *Is_Opaque* shall always be FALSE. The option *Is_Opaque* is for future versions of the E2EPW. It allows to pass PAs as UINT8-arrays without the necessity of marshaling. The E2EPW then treats the data element as opaque (no marshaling is performed).

       * - *Use_Call_By_Ref* 
         - *Use_Call_By_Ref* shall be YES if the data element is not a primitive data type or configured for reception. If YES, then a pointer to the data element is passed as an argument to *Rte_Read_p_o()* and *Rte_Write_p_o()*, respectively ("call by reference"). If NO, then the value of data element is passed ("call by value"). Note that for *Rte_Read_p_o()*, *Use_Call_By_Ref* is always YES.

       * - *Use_Rte_Update* 
         - *Use_Rte_Update* shall be YES if and only if the protection wrapper shall check for new received data elements on the RTE level before calling *Rte_Read_p_o()*. If YES, then *Rte_Read_p_o()* is only called if *Rte_IsUpdated_p_o()* returns TRUE before. If NO, then *Rte_Read_p_o()* is always called. Note that *Rte_IsUpdated_p_o()* is only available in AUTOSAR 4.0 or higher.

       * - *Use_Rte_Instance* 
         - *Use_Rte_Instance* shall be YES if and only if the *instance* parameter is required as an argument for *Rte_Write_p_o()*, *Rte_Read_p_o()* and *Rte_IsUpdated_p_o()*. Note that the *instance* parameter is currently not interpreted by the E2EPW, hence different instances share the same E2EPW and E2E library code and data. Therefore, *Use_Rte_Instance* may only be YES if each port *p* and data element *o* is not used by more than one instance.
    
    
    The user of MICROSAR Safe shall verify that the combination of *Port_Name* and *VDP_Name* is unique over all PDEs for the same node.


.. smi:: SMI-132

    The user of MICROSAR Safe shall verify that the attributes of each signal in a PA are defined in the E2EConfig as intended.
    
    
    .. list-table::
       :header-rows: 1
    
       * - Attribute 
         - Requirement 

       * - *Signal_Name* 
         - *Signal_Name* shall equal the name of the corresponding signal in the data element in the RTE.

       * - *Signal_Type* 
         - *Signal_Type* shall equal the type of the corresponding signal in the data element in the RTE.

       * - *Byte_Order* 
         - *Byte_Order* shall equal the byte order that is defined for the corresponding signal in the I-PDU for the I-PDU mapping in the COM layer.

       * - *Bit_Length* 
         - *Bit_Length* shall equal the bit length of the corresponding signal in the I-PDU.

       * - *Bit_Position* 
         - *Bit_Position* shall equal the start-bit position of the corresponding signal relative to the protected signal group (protected area) and depending on the endianess. If the signal is mapped with Little Endian in the I-PDU, then *Bit_Position* is the least significant bit's position in the least significant byte. If the signal is mapped with Big Endian in the I-PDU, then *Bit_Position* is the most significant bit's position in the most significant byte. For a signal of type UINT8N, the *Bit_Position* is the least significant bit's position in the byte with the lowest address.


.. smi:: SMI-133

    The user of MICROSAR Safe shall verify that for all instances of a PDE of the corresponding PA in the various E2EConfig files,

    - the profile configurations are equal throughout all PA instances (except *Max_Delta_Counter_Init*),
    - all signal configurations are equal throughout all PA instances (except *Signal_Name* and *Signal_ID*) and
    - the following field values are equal throughout all PA instances:
     
      - *Bit_Order*
      - *Bit_Counting*
      - *Unused_Bit_Value*
     
    For example, a sender PDE and a receiver PDE on different ECUs must be configured in different E2EConfig files.


Additional verification of generator execution
==============================================

.. smi:: SMI-134

    The user of MICROSAR Safe shall ensure that the output path for the generated E2EPW code (command-line argument "outpath-path") shall be empty before the generator is started.
    If the output path is not empty, code from previous generation runs may be accidentally integrated into the system.
    This requirement is fulfilled if generation is performed using DaVinci Configurator PRO.


.. smi:: SMI-135

    The user of MICROSAR Safe shall inspect the messages of the E2EPW generator execution.
    If the generator aborts the generation process with an error message, the (partially) generated output files shall not be used in the system.
    If the generator detects an error, a message starting with "ERROR" is displayed on the standard output.
    If the generator shows a warning message starting with "WARNING", the user of MICROSAR Safe shall ensure that the cause of the warning does not invalidate the generated output files.
    The generator shows a warning message in the following cases:
    1. The E2EConfig file ends with some text after the configuration definitions of all protected areas.
    2. The E2EConfig file has at least two PDEs defined with the same *PDE_Name*.
    It is strongly recommended to analyze these cases.


.. smi:: SMI-142

    The user of MICROSAR Safe shall verify that each generated file is complete, i.e. each file must end with an "EOF" comment. 


Additional verification for application
=======================================

.. smi:: SMI-136

    The user of MICROSAR Safe shall verify that the E2E library communication states are not modified by the application while an E2EPW API function is running.
    If a pointer to the senders/receivers E2E library communication state is required, it shall be queried with *E2EPW_GetProtectState_p_o()* / *E2EPW_GetCheckState_p_o()*.
    Make sure that the values of the E2E library communication state are only read and not altered by the application or a module other than the E2EPW or the E2E library unless intended so.
    For special purposes, modification of the sequence counter by the application may be useful or required. Be aware that this is done in the responsibility of the user of MICROSAR Safe.


.. smi:: SMI-137

    The user of MICROSAR Safe shall verify that, when a data element is passed to *E2EPW_Write_p_o()* or *E2EPW_Read_p_o()* using call by reference, then the data element is not altered while *E2EPW_Write_p_o()* or *E2EPW_Read_p_o()* is running. 


.. smi:: SMI-141

    The user of MICROSAR Safe shall verify that an E2EPW API function for a certain *port* / *pde* combination is not called while another or the same E2EPW API function is running for the same combination.
    This applies to the following functions:

    - *E2EPW_WriteInit_p_o()*, Note: *E2EPW_WriteInit_p_o()* is not reentrant
    - *E2EPW_Get_ProtectState_p_o()*
    - *E2EPW_Write_p_o()*
    - *E2EPW_ReadInit_p_o()*, Note: *E2EPW_ReadInit_p_o()* is not reentrant
    - *E2EPW_Get_CheckState_p_o()*
    - *E2EPW_Read_p_o()*


.. smi:: SMI-138

    The user of MICROSAR Safe shall verify that the E2E library communication state is checked for plausibility with *E2EPW_Get[Protect|Check]State_p_o()* after a startup/restart.


.. smi:: SMI-139

    The user of MICROSAR Safe shall verify that that *E2EPW_ReadInit_p_o()* and *E2EPW_WriteInit_p_o()* is only called at partition startup/restart. 
    Note: *E2EPW_ReadInit_p_o()* and *E2EPW_WriteInit_p_o()* have to be called prior to any other E2EPW API function call.
    This initializes the communication state on sender and receiver side. The E2E library communication state should be initialized when the application is initialized or reset.


.. smi:: SMI-140

    The user of MICROSAR Safe shall verify that the intended parameters are passed. This especially applies to:

    - instance IDs. Note: only one instance ID is used for each *p*, *o* combination.
    - AppData parameter for *E2EPW_Write_p_o()* and *E2EPW_Read_p_o()*


Dependencies to other components
********************************

Safety features required from other components
==============================================

.. smi:: SMI-129

    This component requires the initialization, protection, mapping and check features from the E2E library as safety features.


Coexistence with other components
=================================

.. smi:: SMI-144

    This component requires coexistence with the used Rte.
    It is assumed that the user of E2EPW has the required ASIL.


Dependencies to hardware
************************

This component does not use a direct hardware interface.

