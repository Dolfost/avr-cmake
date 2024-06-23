cmake_minimum_required(VERSION 3.6)

add_library(blink STATIC 
	./blink.c 
	./blink.h
)

target_compile_definitions(blink
	PRIVATE
		-DF_CPU=${F_CPU}
		-DBAUD=${BAUD}
)

target_include_directories(blink
	INTERFACE ${CMAKE_CURRENT_LIST_DIR}
)

target_compile_options(blink
	PRIVATE
		-Os -g -Wall -c
		-funsigned-char -funsigned-bitfields -fpack-struct 
		-fshort-enums -ffunction-sections -fdata-sections
		-mmcu=${MCU}
)
target_link_options(blink
	PRIVATE
		-Wl,-Map,${PROJECT_NAME}.map
		-Wl,--gc-sections -Wl,--relax
		# -Wl,-u,vfprintf -lprintf_min # smaller printf
		# -Wl,-u,vfprintf -lprintf_flt -lm # floating-point printf
		-mmcu=${MCU}
)
