cl -nologo -EHsc -std:c++20 -Wall -I.. -Zc:preprocessor -Fe:example-msvc.exe example_first_part.cpp example_second_part.cpp %*
cl -nologo -EHsc -std:c++20 -Wall -I.. -Zc:preprocessor -Fe:in-class-msvc.exe in-class.cpp %*

@del *.obj >nul
