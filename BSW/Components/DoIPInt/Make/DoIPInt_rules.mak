############################################################################### 
# File Name  : DoIPInt_rules.mak 
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
# 1.00.00   2019-07-01  vismda  Initial version
#------------------------------------------------------------------------------
# TemplateVersion = 1.02
###############################################################################

# Component Files
CC_FILES_TO_BUILD       += DoIPInt$(BSW_SRC_DIR)\DoIPInt*.c
GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\DoIPInt_Lcfg.c

# Library Settings
LIBRARIES_TO_BUILD      += DoIPInt
DoIPInt_FILES           += DoIPInt$(BSW_SRC_DIR)\DoIPInt*.c

