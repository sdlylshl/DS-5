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
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/PIC18F2638.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/PIC18F2638.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=Source/system/adc.c Source/system/hlvd.c Source/system/interrupt_manager.c Source/system/mcc.c Source/system/memory.c Source/system/pin_manager.c Source/system/spi1.c Source/system/tmr0.c Source/system/tmr1.c Source/system/tmr2.c Source/system/eusart.c Source/system/pwm1.c Source/ClassB_main.c Source/SSL_ClockTest.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/Source/system/adc.p1 ${OBJECTDIR}/Source/system/hlvd.p1 ${OBJECTDIR}/Source/system/interrupt_manager.p1 ${OBJECTDIR}/Source/system/mcc.p1 ${OBJECTDIR}/Source/system/memory.p1 ${OBJECTDIR}/Source/system/pin_manager.p1 ${OBJECTDIR}/Source/system/spi1.p1 ${OBJECTDIR}/Source/system/tmr0.p1 ${OBJECTDIR}/Source/system/tmr1.p1 ${OBJECTDIR}/Source/system/tmr2.p1 ${OBJECTDIR}/Source/system/eusart.p1 ${OBJECTDIR}/Source/system/pwm1.p1 ${OBJECTDIR}/Source/ClassB_main.p1 ${OBJECTDIR}/Source/SSL_ClockTest.p1
POSSIBLE_DEPFILES=${OBJECTDIR}/Source/system/adc.p1.d ${OBJECTDIR}/Source/system/hlvd.p1.d ${OBJECTDIR}/Source/system/interrupt_manager.p1.d ${OBJECTDIR}/Source/system/mcc.p1.d ${OBJECTDIR}/Source/system/memory.p1.d ${OBJECTDIR}/Source/system/pin_manager.p1.d ${OBJECTDIR}/Source/system/spi1.p1.d ${OBJECTDIR}/Source/system/tmr0.p1.d ${OBJECTDIR}/Source/system/tmr1.p1.d ${OBJECTDIR}/Source/system/tmr2.p1.d ${OBJECTDIR}/Source/system/eusart.p1.d ${OBJECTDIR}/Source/system/pwm1.p1.d ${OBJECTDIR}/Source/ClassB_main.p1.d ${OBJECTDIR}/Source/SSL_ClockTest.p1.d

# Object Files
OBJECTFILES=${OBJECTDIR}/Source/system/adc.p1 ${OBJECTDIR}/Source/system/hlvd.p1 ${OBJECTDIR}/Source/system/interrupt_manager.p1 ${OBJECTDIR}/Source/system/mcc.p1 ${OBJECTDIR}/Source/system/memory.p1 ${OBJECTDIR}/Source/system/pin_manager.p1 ${OBJECTDIR}/Source/system/spi1.p1 ${OBJECTDIR}/Source/system/tmr0.p1 ${OBJECTDIR}/Source/system/tmr1.p1 ${OBJECTDIR}/Source/system/tmr2.p1 ${OBJECTDIR}/Source/system/eusart.p1 ${OBJECTDIR}/Source/system/pwm1.p1 ${OBJECTDIR}/Source/ClassB_main.p1 ${OBJECTDIR}/Source/SSL_ClockTest.p1

# Source Files
SOURCEFILES=Source/system/adc.c Source/system/hlvd.c Source/system/interrupt_manager.c Source/system/mcc.c Source/system/memory.c Source/system/pin_manager.c Source/system/spi1.c Source/system/tmr0.c Source/system/tmr1.c Source/system/tmr2.c Source/system/eusart.c Source/system/pwm1.c Source/ClassB_main.c Source/SSL_ClockTest.c


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
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/PIC18F2638.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F2680
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/Source/system/adc.p1: Source/system/adc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source/system" 
	@${RM} ${OBJECTDIR}/Source/system/adc.p1.d 
	@${RM} ${OBJECTDIR}/Source/system/adc.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,+plib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/system/adc.p1  Source/system/adc.c 
	@-${MV} ${OBJECTDIR}/Source/system/adc.d ${OBJECTDIR}/Source/system/adc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/system/adc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Source/system/hlvd.p1: Source/system/hlvd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source/system" 
	@${RM} ${OBJECTDIR}/Source/system/hlvd.p1.d 
	@${RM} ${OBJECTDIR}/Source/system/hlvd.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,+plib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/system/hlvd.p1  Source/system/hlvd.c 
	@-${MV} ${OBJECTDIR}/Source/system/hlvd.d ${OBJECTDIR}/Source/system/hlvd.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/system/hlvd.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Source/system/interrupt_manager.p1: Source/system/interrupt_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source/system" 
	@${RM} ${OBJECTDIR}/Source/system/interrupt_manager.p1.d 
	@${RM} ${OBJECTDIR}/Source/system/interrupt_manager.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,+plib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/system/interrupt_manager.p1  Source/system/interrupt_manager.c 
	@-${MV} ${OBJECTDIR}/Source/system/interrupt_manager.d ${OBJECTDIR}/Source/system/interrupt_manager.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/system/interrupt_manager.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Source/system/mcc.p1: Source/system/mcc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source/system" 
	@${RM} ${OBJECTDIR}/Source/system/mcc.p1.d 
	@${RM} ${OBJECTDIR}/Source/system/mcc.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,+plib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/system/mcc.p1  Source/system/mcc.c 
	@-${MV} ${OBJECTDIR}/Source/system/mcc.d ${OBJECTDIR}/Source/system/mcc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/system/mcc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Source/system/memory.p1: Source/system/memory.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source/system" 
	@${RM} ${OBJECTDIR}/Source/system/memory.p1.d 
	@${RM} ${OBJECTDIR}/Source/system/memory.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,+plib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/system/memory.p1  Source/system/memory.c 
	@-${MV} ${OBJECTDIR}/Source/system/memory.d ${OBJECTDIR}/Source/system/memory.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/system/memory.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Source/system/pin_manager.p1: Source/system/pin_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source/system" 
	@${RM} ${OBJECTDIR}/Source/system/pin_manager.p1.d 
	@${RM} ${OBJECTDIR}/Source/system/pin_manager.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,+plib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/system/pin_manager.p1  Source/system/pin_manager.c 
	@-${MV} ${OBJECTDIR}/Source/system/pin_manager.d ${OBJECTDIR}/Source/system/pin_manager.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/system/pin_manager.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Source/system/spi1.p1: Source/system/spi1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source/system" 
	@${RM} ${OBJECTDIR}/Source/system/spi1.p1.d 
	@${RM} ${OBJECTDIR}/Source/system/spi1.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,+plib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/system/spi1.p1  Source/system/spi1.c 
	@-${MV} ${OBJECTDIR}/Source/system/spi1.d ${OBJECTDIR}/Source/system/spi1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/system/spi1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Source/system/tmr0.p1: Source/system/tmr0.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source/system" 
	@${RM} ${OBJECTDIR}/Source/system/tmr0.p1.d 
	@${RM} ${OBJECTDIR}/Source/system/tmr0.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,+plib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/system/tmr0.p1  Source/system/tmr0.c 
	@-${MV} ${OBJECTDIR}/Source/system/tmr0.d ${OBJECTDIR}/Source/system/tmr0.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/system/tmr0.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Source/system/tmr1.p1: Source/system/tmr1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source/system" 
	@${RM} ${OBJECTDIR}/Source/system/tmr1.p1.d 
	@${RM} ${OBJECTDIR}/Source/system/tmr1.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,+plib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/system/tmr1.p1  Source/system/tmr1.c 
	@-${MV} ${OBJECTDIR}/Source/system/tmr1.d ${OBJECTDIR}/Source/system/tmr1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/system/tmr1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Source/system/tmr2.p1: Source/system/tmr2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source/system" 
	@${RM} ${OBJECTDIR}/Source/system/tmr2.p1.d 
	@${RM} ${OBJECTDIR}/Source/system/tmr2.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,+plib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/system/tmr2.p1  Source/system/tmr2.c 
	@-${MV} ${OBJECTDIR}/Source/system/tmr2.d ${OBJECTDIR}/Source/system/tmr2.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/system/tmr2.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Source/system/eusart.p1: Source/system/eusart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source/system" 
	@${RM} ${OBJECTDIR}/Source/system/eusart.p1.d 
	@${RM} ${OBJECTDIR}/Source/system/eusart.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,+plib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/system/eusart.p1  Source/system/eusart.c 
	@-${MV} ${OBJECTDIR}/Source/system/eusart.d ${OBJECTDIR}/Source/system/eusart.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/system/eusart.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Source/system/pwm1.p1: Source/system/pwm1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source/system" 
	@${RM} ${OBJECTDIR}/Source/system/pwm1.p1.d 
	@${RM} ${OBJECTDIR}/Source/system/pwm1.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,+plib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/system/pwm1.p1  Source/system/pwm1.c 
	@-${MV} ${OBJECTDIR}/Source/system/pwm1.d ${OBJECTDIR}/Source/system/pwm1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/system/pwm1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Source/ClassB_main.p1: Source/ClassB_main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source" 
	@${RM} ${OBJECTDIR}/Source/ClassB_main.p1.d 
	@${RM} ${OBJECTDIR}/Source/ClassB_main.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,+plib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/ClassB_main.p1  Source/ClassB_main.c 
	@-${MV} ${OBJECTDIR}/Source/ClassB_main.d ${OBJECTDIR}/Source/ClassB_main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/ClassB_main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Source/SSL_ClockTest.p1: Source/SSL_ClockTest.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source" 
	@${RM} ${OBJECTDIR}/Source/SSL_ClockTest.p1.d 
	@${RM} ${OBJECTDIR}/Source/SSL_ClockTest.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,+plib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/SSL_ClockTest.p1  Source/SSL_ClockTest.c 
	@-${MV} ${OBJECTDIR}/Source/SSL_ClockTest.d ${OBJECTDIR}/Source/SSL_ClockTest.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/SSL_ClockTest.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/Source/system/adc.p1: Source/system/adc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source/system" 
	@${RM} ${OBJECTDIR}/Source/system/adc.p1.d 
	@${RM} ${OBJECTDIR}/Source/system/adc.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,+plib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/system/adc.p1  Source/system/adc.c 
	@-${MV} ${OBJECTDIR}/Source/system/adc.d ${OBJECTDIR}/Source/system/adc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/system/adc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Source/system/hlvd.p1: Source/system/hlvd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source/system" 
	@${RM} ${OBJECTDIR}/Source/system/hlvd.p1.d 
	@${RM} ${OBJECTDIR}/Source/system/hlvd.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,+plib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/system/hlvd.p1  Source/system/hlvd.c 
	@-${MV} ${OBJECTDIR}/Source/system/hlvd.d ${OBJECTDIR}/Source/system/hlvd.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/system/hlvd.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Source/system/interrupt_manager.p1: Source/system/interrupt_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source/system" 
	@${RM} ${OBJECTDIR}/Source/system/interrupt_manager.p1.d 
	@${RM} ${OBJECTDIR}/Source/system/interrupt_manager.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,+plib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/system/interrupt_manager.p1  Source/system/interrupt_manager.c 
	@-${MV} ${OBJECTDIR}/Source/system/interrupt_manager.d ${OBJECTDIR}/Source/system/interrupt_manager.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/system/interrupt_manager.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Source/system/mcc.p1: Source/system/mcc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source/system" 
	@${RM} ${OBJECTDIR}/Source/system/mcc.p1.d 
	@${RM} ${OBJECTDIR}/Source/system/mcc.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,+plib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/system/mcc.p1  Source/system/mcc.c 
	@-${MV} ${OBJECTDIR}/Source/system/mcc.d ${OBJECTDIR}/Source/system/mcc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/system/mcc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Source/system/memory.p1: Source/system/memory.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source/system" 
	@${RM} ${OBJECTDIR}/Source/system/memory.p1.d 
	@${RM} ${OBJECTDIR}/Source/system/memory.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,+plib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/system/memory.p1  Source/system/memory.c 
	@-${MV} ${OBJECTDIR}/Source/system/memory.d ${OBJECTDIR}/Source/system/memory.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/system/memory.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Source/system/pin_manager.p1: Source/system/pin_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source/system" 
	@${RM} ${OBJECTDIR}/Source/system/pin_manager.p1.d 
	@${RM} ${OBJECTDIR}/Source/system/pin_manager.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,+plib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/system/pin_manager.p1  Source/system/pin_manager.c 
	@-${MV} ${OBJECTDIR}/Source/system/pin_manager.d ${OBJECTDIR}/Source/system/pin_manager.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/system/pin_manager.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Source/system/spi1.p1: Source/system/spi1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source/system" 
	@${RM} ${OBJECTDIR}/Source/system/spi1.p1.d 
	@${RM} ${OBJECTDIR}/Source/system/spi1.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,+plib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/system/spi1.p1  Source/system/spi1.c 
	@-${MV} ${OBJECTDIR}/Source/system/spi1.d ${OBJECTDIR}/Source/system/spi1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/system/spi1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Source/system/tmr0.p1: Source/system/tmr0.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source/system" 
	@${RM} ${OBJECTDIR}/Source/system/tmr0.p1.d 
	@${RM} ${OBJECTDIR}/Source/system/tmr0.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,+plib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/system/tmr0.p1  Source/system/tmr0.c 
	@-${MV} ${OBJECTDIR}/Source/system/tmr0.d ${OBJECTDIR}/Source/system/tmr0.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/system/tmr0.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Source/system/tmr1.p1: Source/system/tmr1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source/system" 
	@${RM} ${OBJECTDIR}/Source/system/tmr1.p1.d 
	@${RM} ${OBJECTDIR}/Source/system/tmr1.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,+plib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/system/tmr1.p1  Source/system/tmr1.c 
	@-${MV} ${OBJECTDIR}/Source/system/tmr1.d ${OBJECTDIR}/Source/system/tmr1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/system/tmr1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Source/system/tmr2.p1: Source/system/tmr2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source/system" 
	@${RM} ${OBJECTDIR}/Source/system/tmr2.p1.d 
	@${RM} ${OBJECTDIR}/Source/system/tmr2.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,+plib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/system/tmr2.p1  Source/system/tmr2.c 
	@-${MV} ${OBJECTDIR}/Source/system/tmr2.d ${OBJECTDIR}/Source/system/tmr2.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/system/tmr2.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Source/system/eusart.p1: Source/system/eusart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source/system" 
	@${RM} ${OBJECTDIR}/Source/system/eusart.p1.d 
	@${RM} ${OBJECTDIR}/Source/system/eusart.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,+plib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/system/eusart.p1  Source/system/eusart.c 
	@-${MV} ${OBJECTDIR}/Source/system/eusart.d ${OBJECTDIR}/Source/system/eusart.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/system/eusart.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Source/system/pwm1.p1: Source/system/pwm1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source/system" 
	@${RM} ${OBJECTDIR}/Source/system/pwm1.p1.d 
	@${RM} ${OBJECTDIR}/Source/system/pwm1.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,+plib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/system/pwm1.p1  Source/system/pwm1.c 
	@-${MV} ${OBJECTDIR}/Source/system/pwm1.d ${OBJECTDIR}/Source/system/pwm1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/system/pwm1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Source/ClassB_main.p1: Source/ClassB_main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source" 
	@${RM} ${OBJECTDIR}/Source/ClassB_main.p1.d 
	@${RM} ${OBJECTDIR}/Source/ClassB_main.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,+plib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/ClassB_main.p1  Source/ClassB_main.c 
	@-${MV} ${OBJECTDIR}/Source/ClassB_main.d ${OBJECTDIR}/Source/ClassB_main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/ClassB_main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Source/SSL_ClockTest.p1: Source/SSL_ClockTest.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Source" 
	@${RM} ${OBJECTDIR}/Source/SSL_ClockTest.p1.d 
	@${RM} ${OBJECTDIR}/Source/SSL_ClockTest.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,+plib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Source/SSL_ClockTest.p1  Source/SSL_ClockTest.c 
	@-${MV} ${OBJECTDIR}/Source/SSL_ClockTest.d ${OBJECTDIR}/Source/SSL_ClockTest.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Source/SSL_ClockTest.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/PIC18F2638.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) --chip=$(MP_PROCESSOR_OPTION) -G -mdist/${CND_CONF}/${IMAGE_TYPE}/PIC18F2638.X.${IMAGE_TYPE}.map  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,+plib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     --rom=default,-fd30-ffff --ram=default,-cf4-cff,-f9c-f9c,-fd4-fd4,-fdb-fdf,-fe3-fe7,-feb-fef,-ffd-fff  -odist/${CND_CONF}/${IMAGE_TYPE}/PIC18F2638.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	@${RM} dist/${CND_CONF}/${IMAGE_TYPE}/PIC18F2638.X.${IMAGE_TYPE}.hex 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/PIC18F2638.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) --chip=$(MP_PROCESSOR_OPTION) -G -mdist/${CND_CONF}/${IMAGE_TYPE}/PIC18F2638.X.${IMAGE_TYPE}.map  --double=24 --float=24 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=free -P -N255 --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,+plib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -odist/${CND_CONF}/${IMAGE_TYPE}/PIC18F2638.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	
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
