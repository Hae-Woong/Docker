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
 *        \brief  Rtm Header for RTE Analyzer
 *
 *      \details  This header provides the prototypes of the RTM module
 *                that are required for the static analysis with RTE Analyzer.
 *
 *********************************************************************************************************************/
#if !defined (RTM_H)
# define RTM_H

#include "RteAnalyzer/Source/Rtm_Cfg.h"
typedef uint32 Rtm_MeasurementTimestampType;

void Rtm_Start(uint32 measurementId);
void Rtm_Stop(uint32 measurementId);
void Rtm_TriggerReading(void);

#endif
