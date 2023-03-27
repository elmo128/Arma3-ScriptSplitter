/*
* MIT License
*
* Copyright (c) 2022 elmo128 (elmo128@protonmail.com)
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <filesystem>
#include <deque>
#include <utility>
#include <stdexcept>
#include <algorithm>
#include <set>
#include <map>
#include <locale>

#include "../inc/main.h"
#include "../inc/splitter.h"

int main(int argc, char* argv[])
{
    std::cout << "Arma3 - ScriptSplitter v1.0.0\n";
    std::cout << "Visit https://github.com/elmo128/Arma3-ScriptSplitter for further information\n\n";
    std::cout << "Processing input...!" << std::endl;
    
    if (argc == 6)
    {
        std::cout << "1) Function Tag: >" << argv[1] << "<" << std::endl;
        std::cout << "2) Functions class: >" << argv[2] << "<" << std::endl;
        std::cout << "3) File to process: >" << argv[3] << "<" << std::endl;
        std::cout << "4) Header File: >" << argv[4] << "<" << std::endl;
        std::cout << "5) Output directory: >" << argv[5] << "<" << std::endl;

        Fsplitter fp(argv[5], argv[1], argv[2]);
        fp.createOutdir(argv[5]);
        fp.Split_Script_to_functionsLibrary(argv[3], argv[4]);

        std::cout << "Complete!" << std::endl;
    }
    else
    {
        std::cout << "Input invalid! Aborted" << std::endl;
        std::cout << "Parameters: <Function Tag> <Functions class name> <File to process> <Header File> <Output directory>" << std::endl;

        std::cout << "Arma3 - ScriptSplitter v1.0.0\n";
        std::cout << "Visit https://github.com/elmo128/Arma3-ScriptSplitter for further information\n\n";
        std::cout << "MIT License:\n";
        std::cout << "Copyright(c) 2022 elmo128(elmo128@protonmail.com)\n";
        std::cout << "Permission is hereby granted, free of charge, to any person obtaining a copy of this softwareand associated documentation files(the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and /or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:";
        std::cout << "The above copyright noticeand this permission notice shall be included in all copies or substantial portions of the Software.";
        std::cout << "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.";
    }
}
