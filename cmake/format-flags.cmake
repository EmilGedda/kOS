set(FLAGS "-checks=*"
	  "-clang-analyzer-alpha.*"
	  "-warnings-as-errors=*"
	  "-hicpp-no-assembler"
	  "-hicpp-signed-bitwise"
	  "-hicpp-braces-around-statements"
	  "-readability-inconsistent-declaration-parameter-name"
	  "-readability-implicit-bool-conversion"
	  "-cppcoreguidelines-pro-type-reinterpret-cast"
	  "-cppcoreguidelines-pro-bounds-array-to-pointer-decay" PARENT_SCOPE)
string(REPLACE ";" "," CLANG_TIDY_FLAGS "${FLAGS}")
