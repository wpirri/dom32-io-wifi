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
ifeq "$(wildcard nbproject/Makefile-local-Booteable.mk)" "nbproject/Makefile-local-Booteable.mk"
include nbproject/Makefile-local-Booteable.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=Booteable
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/Programa-2.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/Programa-2.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=src/misc/delay.c src/misc/strfunc.c src/sd/fsio.c src/sd/sd_spi.c src/wifi/wifi.c src/wifi/uart1.c src/config.c src/control.c src/http.c src/log.c src/main.c src/rtcc.c src/system.c src/io.c src/wiegand.c src/adc.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/src/misc/delay.o ${OBJECTDIR}/src/misc/strfunc.o ${OBJECTDIR}/src/sd/fsio.o ${OBJECTDIR}/src/sd/sd_spi.o ${OBJECTDIR}/src/wifi/wifi.o ${OBJECTDIR}/src/wifi/uart1.o ${OBJECTDIR}/src/config.o ${OBJECTDIR}/src/control.o ${OBJECTDIR}/src/http.o ${OBJECTDIR}/src/log.o ${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/rtcc.o ${OBJECTDIR}/src/system.o ${OBJECTDIR}/src/io.o ${OBJECTDIR}/src/wiegand.o ${OBJECTDIR}/src/adc.o
POSSIBLE_DEPFILES=${OBJECTDIR}/src/misc/delay.o.d ${OBJECTDIR}/src/misc/strfunc.o.d ${OBJECTDIR}/src/sd/fsio.o.d ${OBJECTDIR}/src/sd/sd_spi.o.d ${OBJECTDIR}/src/wifi/wifi.o.d ${OBJECTDIR}/src/wifi/uart1.o.d ${OBJECTDIR}/src/config.o.d ${OBJECTDIR}/src/control.o.d ${OBJECTDIR}/src/http.o.d ${OBJECTDIR}/src/log.o.d ${OBJECTDIR}/src/main.o.d ${OBJECTDIR}/src/rtcc.o.d ${OBJECTDIR}/src/system.o.d ${OBJECTDIR}/src/io.o.d ${OBJECTDIR}/src/wiegand.o.d ${OBJECTDIR}/src/adc.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/src/misc/delay.o ${OBJECTDIR}/src/misc/strfunc.o ${OBJECTDIR}/src/sd/fsio.o ${OBJECTDIR}/src/sd/sd_spi.o ${OBJECTDIR}/src/wifi/wifi.o ${OBJECTDIR}/src/wifi/uart1.o ${OBJECTDIR}/src/config.o ${OBJECTDIR}/src/control.o ${OBJECTDIR}/src/http.o ${OBJECTDIR}/src/log.o ${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/rtcc.o ${OBJECTDIR}/src/system.o ${OBJECTDIR}/src/io.o ${OBJECTDIR}/src/wiegand.o ${OBJECTDIR}/src/adc.o

# Source Files
SOURCEFILES=src/misc/delay.c src/misc/strfunc.c src/sd/fsio.c src/sd/sd_spi.c src/wifi/wifi.c src/wifi/uart1.c src/config.c src/control.c src/http.c src/log.c src/main.c src/rtcc.c src/system.c src/io.c src/wiegand.c src/adc.c



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

# The following macros may be used in the pre and post step lines
_/_=\\
ShExtension=.bat
Device=PIC32MM0256GPM028
ProjectDir="C:\Users\walte\OneDrive\Documentos\Git\dom32-io-wifi\Programa-2"
ProjectName=Dom32P-IO
ConfName=Booteable
ImagePath="${DISTDIR}\Programa-2.${IMAGE_TYPE}.${OUTPUT_SUFFIX}"
ImageDir="${DISTDIR}"
ImageName="Programa-2.${IMAGE_TYPE}.${OUTPUT_SUFFIX}"
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IsDebug="true"
else
IsDebug="false"
endif

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-Booteable.mk ${DISTDIR}/Programa-2.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
	@echo "--------------------------------------"
	@echo "User defined post-build step: [cp dist/Booteable/production/Programa-2.production.hex ./pgm.hex]"
	@cp dist/Booteable/production/Programa-2.production.hex ./pgm.hex
	@echo "--------------------------------------"

MP_PROCESSOR_OPTION=32MM0256GPM028
MP_LINKER_FILE_OPTION=,--script="..\..\pic32mmBoot\Programa\app_p32MM0256GPM028.ld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/src/misc/delay.o: src/misc/delay.c  .generated_files/flags/Booteable/2c16e146fa106cbfac88131eb5320d0f57ad1c73 .generated_files/flags/Booteable/2929bbcf261be56c36d51eeaa474451d34a88bb3
	@${MKDIR} "${OBJECTDIR}/src/misc" 
	@${RM} ${OBJECTDIR}/src/misc/delay.o.d 
	@${RM} ${OBJECTDIR}/src/misc/delay.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/misc/delay.o.d" -o ${OBJECTDIR}/src/misc/delay.o src/misc/delay.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/src/misc/strfunc.o: src/misc/strfunc.c  .generated_files/flags/Booteable/a02fcb258c05951f86df828b3d5706be6573b692 .generated_files/flags/Booteable/2929bbcf261be56c36d51eeaa474451d34a88bb3
	@${MKDIR} "${OBJECTDIR}/src/misc" 
	@${RM} ${OBJECTDIR}/src/misc/strfunc.o.d 
	@${RM} ${OBJECTDIR}/src/misc/strfunc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/misc/strfunc.o.d" -o ${OBJECTDIR}/src/misc/strfunc.o src/misc/strfunc.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/src/sd/fsio.o: src/sd/fsio.c  .generated_files/flags/Booteable/311dd11f0df741757adb9863a54eda0ae828b30a .generated_files/flags/Booteable/2929bbcf261be56c36d51eeaa474451d34a88bb3
	@${MKDIR} "${OBJECTDIR}/src/sd" 
	@${RM} ${OBJECTDIR}/src/sd/fsio.o.d 
	@${RM} ${OBJECTDIR}/src/sd/fsio.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/sd/fsio.o.d" -o ${OBJECTDIR}/src/sd/fsio.o src/sd/fsio.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/src/sd/sd_spi.o: src/sd/sd_spi.c  .generated_files/flags/Booteable/c6f8e3a9198b74470af0d98a1b7bb8872ada46f0 .generated_files/flags/Booteable/2929bbcf261be56c36d51eeaa474451d34a88bb3
	@${MKDIR} "${OBJECTDIR}/src/sd" 
	@${RM} ${OBJECTDIR}/src/sd/sd_spi.o.d 
	@${RM} ${OBJECTDIR}/src/sd/sd_spi.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/sd/sd_spi.o.d" -o ${OBJECTDIR}/src/sd/sd_spi.o src/sd/sd_spi.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/src/wifi/wifi.o: src/wifi/wifi.c  .generated_files/flags/Booteable/814feafdd8f7420e3a9da08494c981b0fa4462d0 .generated_files/flags/Booteable/2929bbcf261be56c36d51eeaa474451d34a88bb3
	@${MKDIR} "${OBJECTDIR}/src/wifi" 
	@${RM} ${OBJECTDIR}/src/wifi/wifi.o.d 
	@${RM} ${OBJECTDIR}/src/wifi/wifi.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/wifi/wifi.o.d" -o ${OBJECTDIR}/src/wifi/wifi.o src/wifi/wifi.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/src/wifi/uart1.o: src/wifi/uart1.c  .generated_files/flags/Booteable/9e47abebe5ba4ea62219a9929cd7341932a39521 .generated_files/flags/Booteable/2929bbcf261be56c36d51eeaa474451d34a88bb3
	@${MKDIR} "${OBJECTDIR}/src/wifi" 
	@${RM} ${OBJECTDIR}/src/wifi/uart1.o.d 
	@${RM} ${OBJECTDIR}/src/wifi/uart1.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/wifi/uart1.o.d" -o ${OBJECTDIR}/src/wifi/uart1.o src/wifi/uart1.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/src/config.o: src/config.c  .generated_files/flags/Booteable/77c62736efefb2eeaf46380196bcd55174a5feda .generated_files/flags/Booteable/2929bbcf261be56c36d51eeaa474451d34a88bb3
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/config.o.d 
	@${RM} ${OBJECTDIR}/src/config.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/config.o.d" -o ${OBJECTDIR}/src/config.o src/config.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/src/control.o: src/control.c  .generated_files/flags/Booteable/56a18206ad29853677ca917da414fd756dc617fc .generated_files/flags/Booteable/2929bbcf261be56c36d51eeaa474451d34a88bb3
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/control.o.d 
	@${RM} ${OBJECTDIR}/src/control.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/control.o.d" -o ${OBJECTDIR}/src/control.o src/control.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/src/http.o: src/http.c  .generated_files/flags/Booteable/9fea1552517e801a0e0c2e3bb97930aed4b3b9d0 .generated_files/flags/Booteable/2929bbcf261be56c36d51eeaa474451d34a88bb3
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/http.o.d 
	@${RM} ${OBJECTDIR}/src/http.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/http.o.d" -o ${OBJECTDIR}/src/http.o src/http.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/src/log.o: src/log.c  .generated_files/flags/Booteable/f86657207d26ae3ea86138abd888678cc64f7c04 .generated_files/flags/Booteable/2929bbcf261be56c36d51eeaa474451d34a88bb3
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/log.o.d 
	@${RM} ${OBJECTDIR}/src/log.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/log.o.d" -o ${OBJECTDIR}/src/log.o src/log.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/src/main.o: src/main.c  .generated_files/flags/Booteable/f0030d869ba2c6811bd6ae7c4f2f4cf4d8af9850 .generated_files/flags/Booteable/2929bbcf261be56c36d51eeaa474451d34a88bb3
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/main.o.d 
	@${RM} ${OBJECTDIR}/src/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/main.o.d" -o ${OBJECTDIR}/src/main.o src/main.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/src/rtcc.o: src/rtcc.c  .generated_files/flags/Booteable/ed71e661f0153c870b8f9db3bf3ab43088bcf5fe .generated_files/flags/Booteable/2929bbcf261be56c36d51eeaa474451d34a88bb3
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/rtcc.o.d 
	@${RM} ${OBJECTDIR}/src/rtcc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/rtcc.o.d" -o ${OBJECTDIR}/src/rtcc.o src/rtcc.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/src/system.o: src/system.c  .generated_files/flags/Booteable/326a1099e3a1ec6be945fba0f17bd24d8765a7cd .generated_files/flags/Booteable/2929bbcf261be56c36d51eeaa474451d34a88bb3
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/system.o.d 
	@${RM} ${OBJECTDIR}/src/system.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/system.o.d" -o ${OBJECTDIR}/src/system.o src/system.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/src/io.o: src/io.c  .generated_files/flags/Booteable/843dc6135b2105515b53c6e5016b1ce4ce893c59 .generated_files/flags/Booteable/2929bbcf261be56c36d51eeaa474451d34a88bb3
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/io.o.d 
	@${RM} ${OBJECTDIR}/src/io.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/io.o.d" -o ${OBJECTDIR}/src/io.o src/io.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/src/wiegand.o: src/wiegand.c  .generated_files/flags/Booteable/c01f4aca38fb1e1da32500d1af12040b491cf01f .generated_files/flags/Booteable/2929bbcf261be56c36d51eeaa474451d34a88bb3
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/wiegand.o.d 
	@${RM} ${OBJECTDIR}/src/wiegand.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/wiegand.o.d" -o ${OBJECTDIR}/src/wiegand.o src/wiegand.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/src/adc.o: src/adc.c  .generated_files/flags/Booteable/5b1636cdcd76cb737ac5c19af1e81cb22a7f9bb .generated_files/flags/Booteable/2929bbcf261be56c36d51eeaa474451d34a88bb3
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/adc.o.d 
	@${RM} ${OBJECTDIR}/src/adc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/adc.o.d" -o ${OBJECTDIR}/src/adc.o src/adc.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)    
	
else
${OBJECTDIR}/src/misc/delay.o: src/misc/delay.c  .generated_files/flags/Booteable/b5f73d96fb28c0d75de036da8f2abe8dae8c4210 .generated_files/flags/Booteable/2929bbcf261be56c36d51eeaa474451d34a88bb3
	@${MKDIR} "${OBJECTDIR}/src/misc" 
	@${RM} ${OBJECTDIR}/src/misc/delay.o.d 
	@${RM} ${OBJECTDIR}/src/misc/delay.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/misc/delay.o.d" -o ${OBJECTDIR}/src/misc/delay.o src/misc/delay.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/src/misc/strfunc.o: src/misc/strfunc.c  .generated_files/flags/Booteable/aac70361c3c30fac2a2dd3a3a1cc47c6b1b760df .generated_files/flags/Booteable/2929bbcf261be56c36d51eeaa474451d34a88bb3
	@${MKDIR} "${OBJECTDIR}/src/misc" 
	@${RM} ${OBJECTDIR}/src/misc/strfunc.o.d 
	@${RM} ${OBJECTDIR}/src/misc/strfunc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/misc/strfunc.o.d" -o ${OBJECTDIR}/src/misc/strfunc.o src/misc/strfunc.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/src/sd/fsio.o: src/sd/fsio.c  .generated_files/flags/Booteable/e8d891f3bbc0b4ffb6639bccf22f0cacef4bb979 .generated_files/flags/Booteable/2929bbcf261be56c36d51eeaa474451d34a88bb3
	@${MKDIR} "${OBJECTDIR}/src/sd" 
	@${RM} ${OBJECTDIR}/src/sd/fsio.o.d 
	@${RM} ${OBJECTDIR}/src/sd/fsio.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/sd/fsio.o.d" -o ${OBJECTDIR}/src/sd/fsio.o src/sd/fsio.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/src/sd/sd_spi.o: src/sd/sd_spi.c  .generated_files/flags/Booteable/93ce002c5684ab0de2e7ad8a262cab61d9bc6633 .generated_files/flags/Booteable/2929bbcf261be56c36d51eeaa474451d34a88bb3
	@${MKDIR} "${OBJECTDIR}/src/sd" 
	@${RM} ${OBJECTDIR}/src/sd/sd_spi.o.d 
	@${RM} ${OBJECTDIR}/src/sd/sd_spi.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/sd/sd_spi.o.d" -o ${OBJECTDIR}/src/sd/sd_spi.o src/sd/sd_spi.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/src/wifi/wifi.o: src/wifi/wifi.c  .generated_files/flags/Booteable/14d51e84bd2c2a6b1791765e2ee3489b3c1adac .generated_files/flags/Booteable/2929bbcf261be56c36d51eeaa474451d34a88bb3
	@${MKDIR} "${OBJECTDIR}/src/wifi" 
	@${RM} ${OBJECTDIR}/src/wifi/wifi.o.d 
	@${RM} ${OBJECTDIR}/src/wifi/wifi.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/wifi/wifi.o.d" -o ${OBJECTDIR}/src/wifi/wifi.o src/wifi/wifi.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/src/wifi/uart1.o: src/wifi/uart1.c  .generated_files/flags/Booteable/824c2a3ab9c9ea8b041e0892bd4f0f1a182e58a8 .generated_files/flags/Booteable/2929bbcf261be56c36d51eeaa474451d34a88bb3
	@${MKDIR} "${OBJECTDIR}/src/wifi" 
	@${RM} ${OBJECTDIR}/src/wifi/uart1.o.d 
	@${RM} ${OBJECTDIR}/src/wifi/uart1.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/wifi/uart1.o.d" -o ${OBJECTDIR}/src/wifi/uart1.o src/wifi/uart1.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/src/config.o: src/config.c  .generated_files/flags/Booteable/840c0e2e86aafade552269b2c77a78c9615b2639 .generated_files/flags/Booteable/2929bbcf261be56c36d51eeaa474451d34a88bb3
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/config.o.d 
	@${RM} ${OBJECTDIR}/src/config.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/config.o.d" -o ${OBJECTDIR}/src/config.o src/config.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/src/control.o: src/control.c  .generated_files/flags/Booteable/bf657617b84cf3cdeb118cceb9524fe9d2991f6 .generated_files/flags/Booteable/2929bbcf261be56c36d51eeaa474451d34a88bb3
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/control.o.d 
	@${RM} ${OBJECTDIR}/src/control.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/control.o.d" -o ${OBJECTDIR}/src/control.o src/control.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/src/http.o: src/http.c  .generated_files/flags/Booteable/ec482fba2b1f93f5150f33d854acad298bac8d3d .generated_files/flags/Booteable/2929bbcf261be56c36d51eeaa474451d34a88bb3
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/http.o.d 
	@${RM} ${OBJECTDIR}/src/http.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/http.o.d" -o ${OBJECTDIR}/src/http.o src/http.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/src/log.o: src/log.c  .generated_files/flags/Booteable/3b18bf521a3d5dc80b9a51b7f51b07a2f64af669 .generated_files/flags/Booteable/2929bbcf261be56c36d51eeaa474451d34a88bb3
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/log.o.d 
	@${RM} ${OBJECTDIR}/src/log.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/log.o.d" -o ${OBJECTDIR}/src/log.o src/log.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/src/main.o: src/main.c  .generated_files/flags/Booteable/2bc266079767daa4054d19f590d92163c590ce04 .generated_files/flags/Booteable/2929bbcf261be56c36d51eeaa474451d34a88bb3
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/main.o.d 
	@${RM} ${OBJECTDIR}/src/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/main.o.d" -o ${OBJECTDIR}/src/main.o src/main.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/src/rtcc.o: src/rtcc.c  .generated_files/flags/Booteable/4740e0f9f0ac79b2119fc2dfe91770662144b508 .generated_files/flags/Booteable/2929bbcf261be56c36d51eeaa474451d34a88bb3
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/rtcc.o.d 
	@${RM} ${OBJECTDIR}/src/rtcc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/rtcc.o.d" -o ${OBJECTDIR}/src/rtcc.o src/rtcc.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/src/system.o: src/system.c  .generated_files/flags/Booteable/78d8eb7152901e94a7a94973555825216da19aa0 .generated_files/flags/Booteable/2929bbcf261be56c36d51eeaa474451d34a88bb3
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/system.o.d 
	@${RM} ${OBJECTDIR}/src/system.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/system.o.d" -o ${OBJECTDIR}/src/system.o src/system.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/src/io.o: src/io.c  .generated_files/flags/Booteable/3b56dfd47dc9942b555e9bc56bfce1769ba92b23 .generated_files/flags/Booteable/2929bbcf261be56c36d51eeaa474451d34a88bb3
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/io.o.d 
	@${RM} ${OBJECTDIR}/src/io.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/io.o.d" -o ${OBJECTDIR}/src/io.o src/io.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/src/wiegand.o: src/wiegand.c  .generated_files/flags/Booteable/fac0ffa4741466af2f8075c40eb382ce80287860 .generated_files/flags/Booteable/2929bbcf261be56c36d51eeaa474451d34a88bb3
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/wiegand.o.d 
	@${RM} ${OBJECTDIR}/src/wiegand.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/wiegand.o.d" -o ${OBJECTDIR}/src/wiegand.o src/wiegand.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/src/adc.o: src/adc.c  .generated_files/flags/Booteable/64e797532370d95472610611e2b417175f124b91 .generated_files/flags/Booteable/2929bbcf261be56c36d51eeaa474451d34a88bb3
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/adc.o.d 
	@${RM} ${OBJECTDIR}/src/adc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/adc.o.d" -o ${OBJECTDIR}/src/adc.o src/adc.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)    
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/Programa-2.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    ../../pic32mmBoot/Programa/app_p32MM0256GPM028.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g -mdebugger -D__MPLAB_DEBUGGER_PK3=1 -mprocessor=$(MP_PROCESSOR_OPTION) -Os -o ${DISTDIR}/Programa-2.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)   -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC00490:0x1FC016FF -mreserve=boot@0x1FC00490:0x1FC00BEF  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=__MPLAB_DEBUGGER_PK3=1,--gc-sections,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",-s,--memorysummary,${DISTDIR}/memoryfile.xml 
	
else
${DISTDIR}/Programa-2.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   ../../pic32mmBoot/Programa/app_p32MM0256GPM028.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION) -Os -o ${DISTDIR}/Programa-2.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--gc-sections,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",-s,--memorysummary,${DISTDIR}/memoryfile.xml 
	${MP_CC_DIR}\\xc32-bin2hex ${DISTDIR}/Programa-2.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
	@echo Normalizing hex file
	@"C:/Program Files/Microchip/MPLABX/v6.00/mplab_platform/platform/../mplab_ide/modules/../../bin/hexmate" --edf="C:/Program Files/Microchip/MPLABX/v6.00/mplab_platform/platform/../mplab_ide/modules/../../dat/en_msgs.txt" ${DISTDIR}/Programa-2.${IMAGE_TYPE}.hex -o${DISTDIR}/Programa-2.${IMAGE_TYPE}.hex

endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
