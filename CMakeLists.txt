cmake_minimum_required(VERSION 3.20)

project(ai_tempconroler LANGUAGES C ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(PROJECT_TARGET ai_tempconroler)
set(LV_CONF_PATH "${CMAKE_CURRENT_SOURCE_DIR}/UI/LVGL/lv_conf.h" CACHE FILEPATH "LVGL configuration header")

add_subdirectory(Middlewares/Third_Party/lvgl)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/project-sources.cmake)

add_executable(${PROJECT_TARGET}
  ${APP_SOURCES}
)

target_include_directories(${PROJECT_TARGET} PRIVATE
  Core/Inc
  FATFS/Target
  FATFS/App
  LIBJPEG/App
  LIBJPEG/Target
  Drivers/STM32F4xx_HAL_Driver/Inc
  Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
  Drivers/BSP/Components/ili9341
  Drivers/BSP/Components/xpt2046
  Drivers/BSP/Components/Common
  UI/Core/Inc
  UI/LVGL
  Utilities/Fonts
  Utilities/Log
  Middlewares/Third_Party/FreeRTOS/Source/include
  Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS
  Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F
  Middlewares/Third_Party/FatFs/src
  Middlewares/Third_Party/LibJPEG/include
  Drivers/CMSIS/Device/ST/STM32F4xx/Include
  Drivers/CMSIS/Include
)

target_compile_definitions(${PROJECT_TARGET} PRIVATE
  USE_HAL_DRIVER
  STM32F407xx
)

target_compile_options(${PROJECT_TARGET} PRIVATE
  $<$<COMPILE_LANGUAGE:C>:--target=arm-arm-none-eabi>
  $<$<COMPILE_LANGUAGE:C>:-mcpu=cortex-m4>
  $<$<COMPILE_LANGUAGE:C>:-mthumb>
  $<$<COMPILE_LANGUAGE:C>:-mfpu=fpv4-sp-d16>
  $<$<COMPILE_LANGUAGE:C>:-mfloat-abi=hard>
  $<$<COMPILE_LANGUAGE:C>:-ffunction-sections>
  $<$<COMPILE_LANGUAGE:C>:-fdata-sections>
  $<$<COMPILE_LANGUAGE:C>:-fshort-enums>
  $<$<COMPILE_LANGUAGE:C>:-fshort-wchar>
  $<$<COMPILE_LANGUAGE:C>:-g>
  $<$<COMPILE_LANGUAGE:C>:-O0>
  $<$<COMPILE_LANGUAGE:C>:-std=c99>
  $<$<COMPILE_LANGUAGE:ASM>:--cpu=Cortex-M4.fp>
)

target_link_options(${PROJECT_TARGET} PRIVATE
  --scatter=${CMAKE_CURRENT_SOURCE_DIR}/MDK-ARM/ai_tempconroler_ai_tempconroler.sct
  --map
  --xref
  --callgraph
  --symbols
  --info=sizes,totals,unused,veneers,summarysizes
  --strict
  --summary_stderr
  --entry=Reset_Handler
)

target_link_libraries(${PROJECT_TARGET} PRIVATE
  lvgl
)

set_target_properties(${PROJECT_TARGET} PROPERTIES SUFFIX ".axf")

add_custom_command(TARGET ${PROJECT_TARGET} POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} --i32 --output ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_TARGET}.hex $<TARGET_FILE:${PROJECT_TARGET}>
  COMMAND ${CMAKE_OBJCOPY} --bin --output ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_TARGET}.bin $<TARGET_FILE:${PROJECT_TARGET}>
  COMMAND ${CMAKE_OBJCOPY} --text -z --output ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_TARGET}.size.txt $<TARGET_FILE:${PROJECT_TARGET}>
  COMMENT "Generating HEX/BIN and size report"
)