############################################################################### 
# File Name  : LinIf_rules.mak 
# Description: Rules makefile 
#------------------------------------------------------------------------------
# COPYRIGHT
#------------------------------------------------------------------------------
# Copyright (c) 2019 by Vector Informatik GmbH.  All rights reserved.
#------------------------------------------------------------------------------
# REVISION HISTORY
#------------------------------------------------------------------------------
# Version   Date        Author  Description
#------------------------------------------------------------------------------
# 1.00.00   2012-08-30  visbmo  Initial Version
# 1.01.00   2018-07-16  visbmo  Update to latest template (1.01)
# 1.02.00   2019-02-06  vircbl  Added support of component-based SIP structure
# 2.00.00   2019-06-28  visbmo  Added support for slave implementation
#------------------------------------------------------------------------------
# TemplateVersion = 1.02
###############################################################################

# Component Files
CC_FILES_TO_BUILD       += LinIf$(BSW_SRC_DIR)\LinIf.c LinIf$(BSW_SRC_DIR)\LinIf_Slave.c LinIf$(BSW_SRC_DIR)\LinIf_Master.c

GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\LinIf_Lcfg.c $(GENDATA_DIR)\LinIf_PBcfg.c
ifeq ($(LINIF_USE_LINTP),1)
GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\LinTp_Lcfg.c $(GENDATA_DIR)\LinTp_PBcfg.c
endif

# Library Settings
LIBRARIES_TO_BUILD      += LinIf
LinIf_FILES             = LinIf$(BSW_SRC_DIR)\LinIf.c LinIf$(BSW_SRC_DIR)\LinIf_Slave.c LinIf$(BSW_SRC_DIR)\LinIf_Master.c
