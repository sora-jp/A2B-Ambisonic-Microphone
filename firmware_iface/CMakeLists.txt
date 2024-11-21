cmake_minimum_required(VERSION 3.5)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(PROG_TYPE avrispmkII)
#set(PORT "/dev/ttyACM0")

# Variables regarding the AVR chip
set(MCU   atmega328p)
set(F_CPU 1000000)
set(BAUD  9600)

add_definitions(-DF_CPU=${F_CPU})

# program names
set(AVRCPP   avr-g++)
set(AVRC     avr-gcc)
set(AVRSTRIP avr-strip)
set(OBJCOPY  avr-objcopy)
set(OBJDUMP  avr-objdump)
set(AVRSIZE  avr-size)
set(AVRDUDE  avrdude)

# Sets the compiler
# Needs to come before the project function
set(CMAKE_SYSTEM_NAME  Generic)
set(CMAKE_CXX_COMPILER ${AVRCPP})
set(CMAKE_C_COMPILER   ${AVRC})
set(CMAKE_ASM_COMPILER   ${AVRC})

project (firmware_iface C CXX ASM)

# Important project paths
set(BASE_PATH    "${${PROJECT_NAME}_SOURCE_DIR}")
set(INC_PATH     "${BASE_PATH}/include")
set(SRC_PATH     "${BASE_PATH}/src")
set(LIB_DIR_PATH "${BASE_PATH}/lib")

# Files to be compiled
file(GLOB SRC_FILES "${SRC_PATH}/*.cpp"
                    "${SRC_PATH}/*.cc"
                    "${SRC_PATH}/*.c"
                    "${SRC_PATH}/*.cxx"
                    "${SRC_PATH}/*.S"
                    "${SRC_PATH}/*.s"
                    "${SRC_PATH}/*.sx"
                    "${SRC_PATH}/*.asm")

set(LIB_SRC_FILES)
set(LIB_INC_PATH)

# Compiler flags
set(CSTANDARD "-std=gnu99")
set(CDEBUG    "-g -Og -ggdb")
set(CWARN     "-Wall -Wstrict-prototypes -Wl,--gc-sections -Wl,--relax -Wno-array-bounds")
set(CTUNING   "-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -ffunction-sections -fdata-sections")
set(COPT      "-Os -lm -lprintf_flt -nolibc")
set(CMCU      "-mmcu=${MCU}")
set(CDEFS     "-DF_CPU=${F_CPU} -DBAUD=${BAUD}")

set(CFLAGS   "${CMCU} ${CDEBUG} ${CDEFS} ${COPT} ${CWARN} ${CSTANDARD} ${CTUNING}")
set(CXXFLAGS "${CMCU} ${CDEBUG} ${CDEFS} ${COPT} ${CTUNING}")

set(CMAKE_C_FLAGS   "${CFLAGS}")
set(CMAKE_CXX_FLAGS "${CXXFLAGS}")
set(CMAKE_ASM_FLAGS   "${CFLAGS}")

# Project setup
include_directories(${INC_PATH} ${LIB_INC_PATH})
add_executable(${PROJECT_NAME} ${SRC_FILES} ${LIB_SRC_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}.elf")

# Compiling targets
add_custom_target(strip ALL     ${AVRSTRIP} "${PROJECT_NAME}.elf" DEPENDS ${PROJECT_NAME})
add_custom_target(hex   ALL     ${OBJCOPY} -R .eeprom -O ihex "${PROJECT_NAME}.elf" "${PROJECT_NAME}.hex" DEPENDS strip)
add_custom_target(eeprom        ${OBJCOPY} -j .eeprom --change-section-lma .eeprom=0 -O ihex "${PROJECT_NAME}.elf" "${PROJECT_NAME}.eeprom" DEPENDS strip)

set(LFUSE_PROG "-U lfuse:w:0x62:m")
set(HFUSE_PROG "-U hfuse:w:0xD9:m")
set(EFUSE_PROG "-U efuse:w:0xFD:m")

add_custom_target(flash ${AVRDUDE} -c ${PROG_TYPE} -p ${MCU} ${LFUSE_PROG} ${HFUSE_PROG} ${EFUSE_PROG} -U flash:w:${PROJECT_NAME}.hex DEPENDS hex)

set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${PROJECT_NAME}.hex;${PROJECT_NAME}.eeprom;${PROJECT_NAME}.lst")
