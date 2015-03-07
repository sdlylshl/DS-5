#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/PIC16F639.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/PIC16F639.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=Source/Nordic/common/hal_nrf_hw.c Source/Nordic/nrf2401/NRF2401.c Source/Nordic/nrf2401/NRF2401_main.c Source/System/tmr0.c Source/System/mcc.c Source/System/pin_manager.c Source/System/interrupt_manager.c Source/System/spi.c Source/main.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/Source/Nordic/common/hal_nrf_hw.p1 ${OBJECTDIR}/Source/Nordic/nrf2401/NRF2401.p1 ${OBJECTDIR}/Source/Nordic/nrf2401/NRF2401_main.p1 ${OBJECTDIR}/Source/System/tmr0.p1 ${OBJECTDIR}/Source/System/mcc.p1 ${OBJECTDIR}/Source/System/pin_manager.p1 ${OBJECTDIR}/Source/System/interrupt_manager.p1 ${OBJECTDIR}/Source/System/spi.p1 ${OBJECTDIR}/Source/main.p1
POSSIBLE_DEPFILES=${OBJECTDIR}/Source/Nordic/common/hal_nrf_hw.p1.d ${OBJECTDIR}/Source/Nordic/nrf2401/NRF2401.p1.d ${OBJECTDIR}/Source/Nordic/nrf2401/NRF2401_main.p1.d ${OBJECTDIR}/Source/System/tmr0.p1.d ${OBJECTDIR}/Source/System/mcc.p1.d ${OBJECTDIR}/Source/System/pin_manager.p1.d ${OBJECTDIR}/Source/System/interrupt_manager.p1.d ${OBJECTDIR}/Source/System/spi.p1.d ${OBJECTDIR}/Source/main.p1.d

# Object Files
OBJECTFILES=${OBJECTDIR}/Source/Nordic/common/hal_nrf_hw.p1 ${OBJECTDIR}/Source/Nordic/nrf2401/NRF2401.p1 ${OBJECTDIR}/Source/Nordic/nrf2401/NRF2401_main.p1 ${OBJECTDIR}/Source/System/tmr0.p1 ${OBJECTDIR}/Source/System/mcc.p1 ${OBJECTDIR}/Source/System/pin_manager.p1 ${OBJECTDIR}/Source/System/interrupt_manager.p1 ${OBJECTDIR}/Source/System/spi.p1 ${OBJECTDIR}/Source/main.p1

# Source Files
SOURCEFILES=Source/Nordic/common/hal_nrf_hw.c Source/Nordic/nrf2401/NRF2401.c Source/Nordic/nrf2401/NRF2401_main.c Source/System/tmr0.c Source/System/mcc.c Source/System/pin_manager.c Source/System/interrupt_manager.c Source/System/spi.c Source/main.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/PIC16F639.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=16F639
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/Source/Nordic/common/hal_nrf_hw.p1: Source/Nordic/common/hal_nrf_hw.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source/Nordic/common" 
	@${RM} ${OBJECTDIR}/Source/Nordic/common/hal_nrf_hw.p1.d 
	@${RM} ${OBJECTDIR}/Source/Nordic/common/hal_nrf_hw.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/Nordic/common/hal_nrf_hw.p1  Source/Nordic/common/hal_nrf_hw.c 
	@-${MV} ${OBJECTDIR}/Source/Nordic/common/hal_nrf_hw.d ${OBJECTDIR}/Source/Nordic/common/hal_nrf_hw.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/Nordic/common/hal_nrf_hw.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Source/Nordic/nrf2401/NRF2401.p1: Source/Nordic/nrf2401/NRF2401.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source/Nordic/nrf2401" 
	@${RM} ${OBJECTDIR}/Source/Nordic/nrf2401/NRF2401.p1.d 
	@${RM} ${OBJECTDIR}/Source/Nordic/nrf2401/NRF2401.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/Nordic/nrf2401/NRF2401.p1  Source/Nordic/nrf2401/NRF2401.c 
	@-${MV} ${OBJECTDIR}/Source/Nordic/nrf2401/NRF2401.d ${OBJECTDIR}/Source/Nordic/nrf2401/NRF2401.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/Nordic/nrf2401/NRF2401.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Source/Nordic/nrf2401/NRF2401_main.p1: Source/Nordic/nrf2401/NRF2401_main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source/Nordic/nrf2401" 
	@${RM} ${OBJECTDIR}/Source/Nordic/nrf2401/NRF2401_main.p1.d 
	@${RM} ${OBJECTDIR}/Source/Nordic/nrf2401/NRF2401_main.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/Nordic/nrf2401/NRF2401_main.p1  Source/Nordic/nrf2401/NRF2401_main.c 
	@-${MV} ${OBJECTDIR}/Source/Nordic/nrf2401/NRF2401_main.d ${OBJECTDIR}/Source/Nordic/nrf2401/NRF2401_main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/Nordic/nrf2401/NRF2401_main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Source/System/tmr0.p1: Source/System/tmr0.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source/System" 
	@${RM} ${OBJECTDIR}/Source/System/tmr0.p1.d 
	@${RM} ${OBJECTDIR}/Source/System/tmr0.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/System/tmr0.p1  Source/System/tmr0.c 
	@-${MV} ${OBJECTDIR}/Source/System/tmr0.d ${OBJECTDIR}/Source/System/tmr0.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/System/tmr0.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Source/System/mcc.p1: Source/System/mcc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source/System" 
	@${RM} ${OBJECTDIR}/Source/System/mcc.p1.d 
	@${RM} ${OBJECTDIR}/Source/System/mcc.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/System/mcc.p1  Source/System/mcc.c 
	@-${MV} ${OBJECTDIR}/Source/System/mcc.d ${OBJECTDIR}/Source/System/mcc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/System/mcc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Source/System/pin_manager.p1: Source/System/pin_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source/System" 
	@${RM} ${OBJECTDIR}/Source/System/pin_manager.p1.d 
	@${RM} ${OBJECTDIR}/Source/System/pin_manager.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/System/pin_manager.p1  Source/System/pin_manager.c 
	@-${MV} ${OBJECTDIR}/Source/System/pin_manager.d ${OBJECTDIR}/Source/System/pin_manager.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/System/pin_manager.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Source/System/interrupt_manager.p1: Source/System/interrupt_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source/System" 
	@${RM} ${OBJECTDIR}/Source/System/interrupt_manager.p1.d 
	@${RM} ${OBJECTDIR}/Source/System/interrupt_manager.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/System/interrupt_manager.p1  Source/System/interrupt_manager.c 
	@-${MV} ${OBJECTDIR}/Source/System/interrupt_manager.d ${OBJECTDIR}/Source/System/interrupt_manager.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/System/interrupt_manager.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Source/System/spi.p1: Source/System/spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source/System" 
	@${RM} ${OBJECTDIR}/Source/System/spi.p1.d 
	@${RM} ${OBJECTDIR}/Source/System/spi.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/System/spi.p1  Source/System/spi.c 
	@-${MV} ${OBJECTDIR}/Source/System/spi.d ${OBJECTDIR}/Source/System/spi.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/System/spi.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Source/main.p1: Source/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source" 
	@${RM} ${OBJECTDIR}/Source/main.p1.d 
	@${RM} ${OBJECTDIR}/Source/main.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/main.p1  Source/main.c 
	@-${MV} ${OBJECTDIR}/Source/main.d ${OBJECTDIR}/Source/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/Source/Nordic/common/hal_nrf_hw.p1: Source/Nordic/common/hal_nrf_hw.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source/Nordic/common" 
	@${RM} ${OBJECTDIR}/Source/Nordic/common/hal_nrf_hw.p1.d 
	@${RM} ${OBJECTDIR}/Source/Nordic/common/hal_nrf_hw.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/Nordic/common/hal_nrf_hw.p1  Source/Nordic/common/hal_nrf_hw.c 
	@-${MV} ${OBJECTDIR}/Source/Nordic/common/hal_nrf_hw.d ${OBJECTDIR}/Source/Nordic/common/hal_nrf_hw.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/Nordic/common/hal_nrf_hw.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Source/Nordic/nrf2401/NRF2401.p1: Source/Nordic/nrf2401/NRF2401.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source/Nordic/nrf2401" 
	@${RM} ${OBJECTDIR}/Source/Nordic/nrf2401/NRF2401.p1.d 
	@${RM} ${OBJECTDIR}/Source/Nordic/nrf2401/NRF2401.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/Nordic/nrf2401/NRF2401.p1  Source/Nordic/nrf2401/NRF2401.c 
	@-${MV} ${OBJECTDIR}/Source/Nordic/nrf2401/NRF2401.d ${OBJECTDIR}/Source/Nordic/nrf2401/NRF2401.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/Nordic/nrf2401/NRF2401.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Source/Nordic/nrf2401/NRF2401_main.p1: Source/Nordic/nrf2401/NRF2401_main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source/Nordic/nrf2401" 
	@${RM} ${OBJECTDIR}/Source/Nordic/nrf2401/NRF2401_main.p1.d 
	@${RM} ${OBJECTDIR}/Source/Nordic/nrf2401/NRF2401_main.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/Nordic/nrf2401/NRF2401_main.p1  Source/Nordic/nrf2401/NRF2401_main.c 
	@-${MV} ${OBJECTDIR}/Source/Nordic/nrf2401/NRF2401_main.d ${OBJECTDIR}/Source/Nordic/nrf2401/NRF2401_main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/Nordic/nrf2401/NRF2401_main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Source/System/tmr0.p1: Source/System/tmr0.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source/System" 
	@${RM} ${OBJECTDIR}/Source/System/tmr0.p1.d 
	@${RM} ${OBJECTDIR}/Source/System/tmr0.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/System/tmr0.p1  Source/System/tmr0.c 
	@-${MV} ${OBJECTDIR}/Source/System/tmr0.d ${OBJECTDIR}/Source/System/tmr0.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/System/tmr0.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Source/System/mcc.p1: Source/System/mcc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source/System" 
	@${RM} ${OBJECTDIR}/Source/System/mcc.p1.d 
	@${RM} ${OBJECTDIR}/Source/System/mcc.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/System/mcc.p1  Source/System/mcc.c 
	@-${MV} ${OBJECTDIR}/Source/System/mcc.d ${OBJECTDIR}/Source/System/mcc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/System/mcc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Source/System/pin_manager.p1: Source/System/pin_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source/System" 
	@${RM} ${OBJECTDIR}/Source/System/pin_manager.p1.d 
	@${RM} ${OBJECTDIR}/Source/System/pin_manager.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/System/pin_manager.p1  Source/System/pin_manager.c 
	@-${MV} ${OBJECTDIR}/Source/System/pin_manager.d ${OBJECTDIR}/Source/System/pin_manager.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/System/pin_manager.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Source/System/interrupt_manager.p1: Source/System/interrupt_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source/System" 
	@${RM} ${OBJECTDIR}/Source/System/interrupt_manager.p1.d 
	@${RM} ${OBJECTDIR}/Source/System/interrupt_manager.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/System/interrupt_manager.p1  Source/System/interrupt_manager.c 
	@-${MV} ${OBJECTDIR}/Source/System/interrupt_manager.d ${OBJECTDIR}/Source/System/interrupt_manager.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/System/interrupt_manager.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Source/System/spi.p1: Source/System/spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source/System" 
	@${RM} ${OBJECTDIR}/Source/System/spi.p1.d 
	@${RM} ${OBJECTDIR}/Source/System/spi.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/System/spi.p1  Source/System/spi.c 
	@-${MV} ${OBJECTDIR}/Source/System/spi.d ${OBJECTDIR}/Source/System/spi.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/System/spi.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Source/main.p1: Source/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source" 
	@${RM} ${OBJECTDIR}/Source/main.p1.d 
	@${RM} ${OBJECTDIR}/Source/main.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/main.p1  Source/main.c 
	@-${MV} ${OBJECTDIR}/Source/main.d ${OBJECTDIR}/Source/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/PIC16F639.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) --chip=$(MP_PROCESSOR_OPTION) -G -mdist/${CND_CONF}/${IMAGE_TYPE}/PIC16F639.X.${IMAGE_TYPE}.map  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     --rom=default,-700-7fe --ram=default,-0-0,-65-70,-80-80,-f0-f0,-100-100,-170-170,-180-180,-1f0-1f0  -odist/${CND_CONF}/${IMAGE_TYPE}/PIC16F639.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	@${RM} dist/${CND_CONF}/${IMAGE_TYPE}/PIC16F639.X.${IMAGE_TYPE}.hex 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/PIC16F639.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) --chip=$(MP_PROCESSOR_OPTION) -G -mdist/${CND_CONF}/${IMAGE_TYPE}/PIC16F639.X.${IMAGE_TYPE}.map  --double=24 --float=24 --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,+osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -odist/${CND_CONF}/${IMAGE_TYPE}/PIC16F639.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
