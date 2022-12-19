# CMakeList.txt : CMake project for APM, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (APM			"APM.cpp" 
							"APM.h" 
							"components/sys/config.h" 
							"components/sys/string_support_functions.h" 							 
							"components/numerical_types/numerical_strings.h"
							"components/Binary_Register.h" 
							"components/numerical_types/Whole_Number.h" 
							"components/numerical_types/Whole_Number.cpp" 
							"components/numerical_types/Integer.h" 
							"components/numerical_types/Integer.cpp" 
							"components/numerical_types/Rational.h" 
							"components/numerical_types/Rational.cpp" 
							"components/numerical_types/Decimal.h" 
							"components/numerical_types/Decimal.cpp" 
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET APM PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
