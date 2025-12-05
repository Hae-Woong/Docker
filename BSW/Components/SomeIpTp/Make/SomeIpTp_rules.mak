############################################################################### 
# File Name  : SomeIpTp_rules.mak 
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
# 1.00.00   2018-01-18  ath     Initial Version
# 1.01.00   2019-02-06  vircbl  Added support of component-based SIP structure
#------------------------------------------------------------------------------
# TemplateVersion = 1.02
###############################################################################

# Component Files
CC_FILES_TO_BUILD       += SomeIpTp$(BSW_SRC_DIR)\SomeIpTp.c
GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\SomeIpTp_Lcfg.c $(GENDATA_DIR)\SomeIpTp_PBcfg.c

# Library Settings
LIBRARIES_TO_BUILD      += SomeIpTp
SomeIpTp_FILES          += SomeIpTp$(BSW_SRC_DIR)\SomeIpTp.c

