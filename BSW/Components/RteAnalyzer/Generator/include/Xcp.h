/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  
 *        \brief  Xcp Header for RTE Analyzer
 *
 *      \details  This header provides the prototypes of the XCP module
 *                that are required for the static analysis with RTE Analyzer.
 *
 *********************************************************************************************************************/
#ifndef _XCP_H
# define _XCP_H 1

# include "RteAnalyzer/Source/Xcp_Cfg.h"

FUNC(uint8, XCP_CODE) Xcp_Event(uint32 event);



#endif
