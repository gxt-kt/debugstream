# print value
macro(PrintVariable var_name)
  message(STATUS "${var_name}=${${var_name}}")
endmacro()
