############################################################################### 
# File Name  : Can_30_Core_rules.mak 
# Description: Rules makefile 
#------------------------------------------------------------------------------
# COPYRIGHT
#------------------------------------------------------------------------------
# Copyright (c) 2024 by Vector Informatik GmbH.  All rights reserved.
#------------------------------------------------------------------------------
# REVISION HISTORY
#------------------------------------------------------------------------------
# Version   Date        Author     Description
#------------------------------------------------------------------------------
# 1.00.00   2021-06-11  vishum     Initial creation
# 3.00.00   2022-09-05  vispip     Adapt Make files as per latest template
# main-1    2024-02-26  visgaz     CANCORE-2371  Change history is maintained in the global ChangeHistory.txt file starting with this release
#------------------------------------------------------------------------------
# TemplateVersion = 1.02
###############################################################################

# Component Files
CC_FILES_TO_BUILD       += Can_30_Core$(BSW_SRC_DIR)\Can_30_Core.c \
                           Can_30_Core$(BSW_SRC_DIR)\Can_30_Core_Baudrate.c \
                           Can_30_Core$(BSW_SRC_DIR)\Can_30_Core_ErrorHandler.c \
                           Can_30_Core$(BSW_SRC_DIR)\Can_30_Core_DiagnosticHandler.c \
                           Can_30_Core$(BSW_SRC_DIR)\Can_30_Core_ExclusiveAreaHandler.c \
                           Can_30_Core$(BSW_SRC_DIR)\Can_30_Core_Initialization.c \
                           Can_30_Core$(BSW_SRC_DIR)\Can_30_Core_InterruptService.c \
                           Can_30_Core$(BSW_SRC_DIR)\Can_30_Core_ModeHandler.c \
                           Can_30_Core$(BSW_SRC_DIR)\Can_30_Core_ModeRequest.c \
                           Can_30_Core$(BSW_SRC_DIR)\Can_30_Core_RxIndicationHandler.c \
                           Can_30_Core$(BSW_SRC_DIR)\Can_30_Core_RxQueueHandler.c \
                           Can_30_Core$(BSW_SRC_DIR)\Can_30_Core_TimeoutHandler.c \
                           Can_30_Core$(BSW_SRC_DIR)\Can_30_Core_TxBufferHandler.c \
                           Can_30_Core$(BSW_SRC_DIR)\Can_30_Core_TxConfirmationHandler.c \
                           Can_30_Core$(BSW_SRC_DIR)\Can_30_Core_TxRequestHandler.c \
                           Can_30_Core$(BSW_SRC_DIR)\Can_30_Core_Version.c \
                           Can_30_Core$(BSW_SRC_DIR)\Can_30_Core_WakeupHandler.c \


GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\Can_30_Core_PBcfg.c $(GENDATA_DIR)\Can_30_Core_Lcfg.c

# Library Settings
LIBRARIES_TO_BUILD      += Can_30_Core
Can_30_Core_FILES       += Can_30_Core$(BSW_SRC_DIR)\Can_30_Core.c

