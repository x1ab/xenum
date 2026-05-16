cl /nologo /EHsc /std:c++11 /I.. /Zc:preprocessor /Fe:example.exe example_first_part.cpp example_second_part.cpp

@del *.obj >nul
