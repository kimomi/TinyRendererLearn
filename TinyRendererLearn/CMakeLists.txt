# CMakeList.txt: TinyRendererLearn 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("TinyRendererLearn")

# 将源代码添加到此项目的可执行文件。
add_executable (TinyRendererLearn "TinyRendererLearn.cpp" "TinyRendererLearn.h" "tgaimage.cpp" "tgaimage.h" "model.cpp" "model.h" "geometry.h" "geometry.cpp" "shader.h" "basicShader.h" "basicShader.cpp" "gouraudShader.h" "gouraudShader.cpp" "bindShader.cpp" "bindShader.h" "normalMappingShader.cpp" "normalMappingShader.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET TinyRendererLearn PROPERTY CXX_STANDARD 20)
endif()

# TODO: 如有需要，请添加测试并安装目标。
