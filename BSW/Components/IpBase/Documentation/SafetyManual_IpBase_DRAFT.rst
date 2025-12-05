Safety Manual IpBase
####################

Assumed safety requirements
***************************

This component does not assume safety requirements. This component is developed according to ISO 26262 only to ensure freedom from interference with respect to memory with up to ASIL B. 

Configuration constraints
*************************

This component does not have configuration constraints.

Additional verification measures
********************************

.. smi:: SMI-1020239

    The user of MICROSAR Safe shall verify that the pointer (*StackPtr*) passed to the function *IpBase_BerInitWorkspace* references a valid array of *IpBase_BerStackElementType* which satisfies the following requirement:

    - *BerDepth* <= number of elements in *StackPtr*


.. smi:: SMI-1020240

    The user of MICROSAR Safe shall verify that the pointer (*WorkspacePtr*) passed to the function *IpBase_BerGetElement* is previously initialized via function *IpBase_BerInitWorkspace* where SMI-1020239 holds true.


.. smi:: SMI-1026681

    The user of MICROSAR Safe shall verify that the pointer (*StrPtr*) passed to the functions *IpBase_ConvInt2String*, *IpBase_ConvInt2StringFront*, *IpBase_ConvInt2HexString* references a string which satisfies the following requirement:

    - content of *StrLenPtr* <= number of elements in content of *StrPtr*


.. smi:: SMI-1027479

    The user of MICROSAR Safe shall verify that the pointer (*SrcPtr*, *Str1Ptr*, *Str2Ptr*) passed to the functions *IpBase_StrCpy*, *IpBase_StrCmp*, *IpBase_StrCmpNoCase* references a string which satisfies the following requirement:

    - content of *SrcPtr*, *Str1Ptr*, *Str2Ptr* ends with '\\0'


.. smi:: SMI-1031276

    The user of MICROSAR Safe shall verify that the pointer (*PBufPtr*) passed to the function *IpBase_CopyString2PbufAt* references a valid array of *IpBase_PbufType* which satisfies the following requirement:

    - *PBufPtr->len* <= number of elements in *PBufPtr->payload*
    - *PBufPtr->totlen* <= sum of all elements in *PBufPtr->len*


.. smi:: SMI-1031277

    The user of MICROSAR Safe shall verify that the pointer (*SockPtr*) passed to the function *IpBase_DelSockAddr*, *IpBase_CopySockAddr* references a valid array of *IpBase_SockAddrType* which satisfies the following requirement:

    - if *Family* == IPBASE_AF_INET

      sizeof(IpBase_SockAddrInType) <= number of bytes in *SockPtr*

    - if *Family* == IPBASE_AF_INET6

      sizeof(IpBase_SockAddrIn6Type) <= number of bytes in *SockPtr*


.. smi:: SMI-1134760

    The user of MICROSAR Safe shall verify that the pointers (*TgtDataPtr*, *SrcDataPtr*) passed to the function *IpBase_TcpIpChecksumCopyAdd* reference valid arrays of type *uint8* which satisfy the following requirements:

    - lengthof(TgtDataPtr) >= LenByte
    - lengthof(SrcDataPtr) >= LenByte


Safety requirements required from other components
**************************************************

This component does not require safety requirements from other components.

Dependencies to hardware
************************

This component does not use a direct hardware interface.

