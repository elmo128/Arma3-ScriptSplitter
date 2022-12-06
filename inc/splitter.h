#pragma once

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

#include <string>
#include <fstream>

class Fsplitter
{
public:
    Fsplitter(const std::filesystem::path &Outpath, const std::string &fnc_tag, const std::string &fnc_category);
    bool Split_Script_to_functionsLibrary(const std::filesystem::path &Filetoprocess, const std::filesystem::path &FilewithHeader);
    bool createOutdir(const std::filesystem::path& Outpath);

private:
    std::pair <std::string, int> removeComments(std::string &workingline, int commentstate);
    void buildIndex(const std::multiset <std::string> &FunctionsMSet);
    void updateFunctionNames(const std::multimap <std::string, std::string> &FunctionsReplacementMMap);
    bool AddHeaderFile(const std::filesystem::path &FileToModify, const std::filesystem::path &Headerfile);
    
    std::string tempExtention_;
    std::string fnc_category_;
    std::string fnc_tag_;
    std::string fnc_libFilename_;

    std::filesystem::path Outpath_;
};