cmake_minimum_required(VERSION 3.6)

set(PROG_TYPE usbasp)
set(MCU       atmega168a)
set(F_CPU     1000000)
set(BAUD      9600)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(blinker LANGUAGES C CXX ASM)

add_executable(${PROJECT_NAME} 
	./src/main.c
)

add_subdirectory(./lib/blink)
target_link_libraries(${PROJECT_NAME}
	blink
)

set_target_properties(${PROJECT_NAME} PROPERTIES 
	OUTPUT_NAME "${PROJECT_NAME}.elf"
	C_STANDARD 99
	CXX_STANDARD 98
)
target_compile_definitions(${PROJECT_NAME}
	PRIVATE 
		F_CPU=${F_CPU}
		BAUD=${BAUD}
)
target_include_directories(${PROJECT_NAME}
	PRIVATE
		./include
)
target_compile_options(${PROJECT_NAME}
	PRIVATE
		-Os -g -Wall -c
		-funsigned-char -funsigned-bitfields -fpack-struct 
		-fshort-enums -ffunction-sections -fdata-sections
		-mmcu=${MCU}
)
target_link_options(${PROJECT_NAME}
	PRIVATE
		-Wl,-Map,${PROJECT_NAME}.map
		-Wl,--gc-sections -Wl,--relax
		# -Wl,-u,vfprintf -lprintf_min # smaller printf
		# -Wl,-u,vfprintf -lprintf_flt -lm # floating-point printf
		-mmcu=${MCU}
)


# Compiling targets
add_custom_target(strip ALL 
	COMMAND ${AVRSTRIP} "${PROJECT_NAME}.elf" 
	DEPENDS ${PROJECT_NAME}
)
add_custom_target(hex ALL 
	COMMAND ${OBJCOPY} -j .text -j .data -O ihex "${PROJECT_NAME}.elf" "${PROJECT_NAME}.hex" 
	DEPENDS strip
)
add_custom_target(eeprom 
	COMMAND ${OBJCOPY} -j .eeprom --change-section-lma .eeprom=0 -O ihex "${PROJECT_NAME}.elf" "${PROJECT_NAME}.eeprom" 
	DEPENDS strip
)
add_custom_target(flash 
	COMMAND ${AVRDUDE} -c ${PROG_TYPE} -p ${MCU} -U flash:w:${PROJECT_NAME}.hex 
	DEPENDS hex
)
add_custom_target(noflash 
	COMMAND ${AVRDUDE} -c ${PROG_TYPE} -np ${MCU} -U flash:w:${PROJECT_NAME}.hex 
	DEPENDS hex
)
add_custom_target(size 
	COMMAND ${AVRSIZE} -C --mcu=${MCU} ${PROJECT_NAME}.elf 
	DEPENDS ${PROJECT_NAME}
)

set_directory_properties(PROPERTIES
	ADDITIONAL_CLEAN_FILES
	[[
		${PROJECT_NAME}.hex;
		${PROJECT_NAME}.map;
		${PROJECT_NAME}.eeprom;
		${PROJECT_NAME}.lst
	]]
)
