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

Fsplitter::Fsplitter(const std::filesystem::path &Outpath, const std::string &fnc_tag, const std::string &fnc_category)
{
    fnc_category_ = fnc_category;
    fnc_tag_ = fnc_tag;
    Outpath_ = Outpath;
    fnc_libFilename_ = Outpath.string();
    fnc_libFilename_ += FUNCTIONSLIBFNAME;
    tempExtention_ = TEMPEXTENTION;
}
bool Fsplitter::createOutdir(const std::filesystem::path &Outpath)
{
    if (std::filesystem::exists(Outpath))
    {
        std::cout << "Directory: >" << Outpath << "< already existed" << std::endl;
        return false;
    }
    std::filesystem::create_directories(Outpath);
    if (std::filesystem::exists(Outpath))
    {
        std::cout << "NewDir: " << Outpath << std::endl;
        return true;
    }
    else
        return false;
}
bool Fsplitter::Split_Script_to_functionsLibrary(const std::filesystem::path &Filetoprocess, const std::filesystem::path &FilewithHeader)
{
    std::ifstream istrm(Filetoprocess, std::ios::in);
    std::ofstream ofuncFile;
    std::string results;
    std::string resultsB;
    std::string fname;
    std::multiset <std::string> FunctionsMSet;
    std::multimap <std::string, std::string> FunctionsReplacementMMap;
    int commentstate = 0;
    bool functiondefined = false;
    bool firstBraces = false;
    int functionopen = 0;
    if (istrm.is_open() && istrm.good())
    {
        for (std::string line; std::getline(istrm, line); )
        {
            int pos = 0;
            if (DEBUG >= DEBUG_LVL1) std::cout << "forStart" << std::endl;
            std::pair <std::string, int> nocomments = removeComments(line, commentstate);
            std::string workingline = std::get<std::string>(nocomments);
            commentstate = std::get<int>(nocomments);
            //if (commentstate < 0)
                //throw std::runtime_error("error");
            if (DEBUG >= DEBUG_LVL1) std::cout << "workingline:" << workingline << "commentstate:" << std::to_string(commentstate) << std::endl;
            // get new function
            if ((workingline.empty() == false) && (commentstate <= 0))
            {
                commentstate = 0;
                if (functionopen <= 0)
                {
                    functiondefined = false;
                    functionopen = 0;
                    fname.clear();

                    // search if function name in line
                    std::smatch matchresultsA;
                    std::regex fnc_regex("(?:^|(?:[.!?]\s))([a-zA-Z0-9_]+)([^={]*)([=])", std::regex::icase);
                    if (std::regex_search(workingline, matchresultsA, fnc_regex))
                    {
                        std::string resultsA = matchresultsA.str();

                        //extract functionname
                        std::smatch matchresultsB;
                        std::regex fnc_name_extract_regex("(?:^|(?:[.!?]\s))([a-zA-Z0-9_]+)", std::regex::icase);
                        if (std::regex_search(workingline, matchresultsB, fnc_name_extract_regex))
                        {
                            resultsB = matchresultsB.str();
                            results = resultsB;
                            // check for tag
                            std::smatch matchresultsC;
                            std::string tagCheck_regex_tmpstr = "(?:^|(?:[.!?]\s))^";
                            tagCheck_regex_tmpstr += fnc_tag_;
                            std::regex fnc_tagCheck_regex(tagCheck_regex_tmpstr, std::regex::icase);
                            if (std::regex_search(results, matchresultsC, fnc_tagCheck_regex))
                            {
                                results.erase(results.find(matchresultsC.str()), matchresultsC.str().size());
                            }
                            // remove preceding "_"
                            std::smatch matchresultsD;
                            std::regex fnc_name_cleanupA("^_+", std::regex::icase);
                            while (std::regex_search(results, matchresultsD, fnc_name_cleanupA))
                            {
                                results.erase(results.find(matchresultsD.str()), matchresultsD.str().size());
                            }

                            firstBraces = true; 
                            fname = Outpath_.string();
                            fname += "fn_";
                            fname += results;
                            if (DEBUG >= DEBUG_LVL1) std::cout << "newFunction:" << fname << std::endl;
                            fname += ".";
                            fname += tempExtention_;
                            while (true)
                            {
                                if (fname.find("__") != std::string::npos)
                                {
                                    fname.erase(fname.find("__"), 1);
                                }
                                else if ((fname.find("_") != std::string::npos) && (fname.find_first_not_of("_") != std::string::npos) && ((fname.find("_")) < (fname.find_first_not_of("_"))) && ((fname.find("_") == 0)))
                                {
                                    fname.erase(0, 1);
                                }
                                else
                                    break;
                            };
                        }
                        workingline.erase(workingline.find(resultsA), resultsA.size());

                        functiondefined = true;
                    }

                    if (functiondefined == true)
                    {
                        if ((workingline.find("{") != std::string::npos) && (firstBraces == true))
                        {
                            // add function name to functions list
                            std::string tmpstr = fnc_tag_;
                            tmpstr += "_fnc_";
                            tmpstr += results;
                            FunctionsReplacementMMap.insert(std::pair(resultsB, tmpstr));
                            FunctionsMSet.insert(results);
                            // create function file, add function name on top
                            std::cout << "NewFile: " << fname << std::endl;
                            ofuncFile.open(fname, std::ios::app);
                            AddHeaderFile(fname, FilewithHeader);
                            ofuncFile << "// Function extracted with Arma-ScriptSplitter: " << tmpstr << std::endl;
                            ofuncFile << "// Source File: " << Filetoprocess.filename() << std::endl;
                            ofuncFile.close();
                            firstBraces = false;
                            workingline.erase(workingline.find("{"), 1);
                            functionopen++;
                        }
                    }
                }
                if (workingline.empty() == false)
                {
                    int strindex = 0;
                    functionopen += (int)std::count(workingline.begin(), workingline.end(), '{');
                    functionopen -= (int)std::count(workingline.begin(), workingline.end(), '}');

                    if ((functiondefined == true) && (functionopen > 0))
                    {
                        if (DEBUG >= DEBUG_LVL1) std::cout << "writeLine" << std::endl;
                        ofuncFile.open(fname, std::ios::app);
                        ofuncFile << workingline << std::endl;
                        ofuncFile.close();
                    }
                }
            }
        }
    }
    updateFunctionNames(FunctionsReplacementMMap);
    buildIndex(FunctionsMSet);

    return true;
}
bool Fsplitter::AddHeaderFile(const std::filesystem::path &FileToModify, const std::filesystem::path &Headerfile)
{
    std::ifstream istrm(Headerfile, std::ios::in);
    std::ofstream ofuncFile;
    if (istrm.is_open() && istrm.good())
    {
        ofuncFile.open(FileToModify, std::ios::app);
        for (std::string line; std::getline(istrm, line); )
        {
            ofuncFile << line << std::endl;
        }
    }
    ofuncFile.close();
    bool eofreached = istrm.eof();
    istrm.close();
    return eofreached;
};
std::pair <std::string, int> Fsplitter::removeComments(std::string &workingline, int commentstate)
{
    // remove Arguments
    if (workingline.find("//") != std::string::npos)
    {
        workingline.erase(workingline.find("//"), workingline.size() - workingline.find("//"));
    }
    while ((workingline.find("/*") != std::string::npos) && (workingline.find("*/") != std::string::npos))
    {
        if ((workingline.find("/*")) < (workingline.find("*/")))
        {
            workingline.erase(workingline.find("/*"), (workingline.find("*/")) - (workingline.find("/*")));
        }
        else
            break;
    }
    if ((workingline.find("*/") != std::string::npos))
    {
        workingline.erase(0, workingline.find("*/"));
        commentstate--;
    }
    if ((workingline.find("/*") != std::string::npos))
    {
        workingline.erase(workingline.find("/*"), workingline.size() - workingline.find("/*"));
        commentstate++;
    }
    if (commentstate > 0)
        workingline.clear();
    return make_pair(workingline, commentstate);
};

void Fsplitter::buildIndex(const std::multiset <std::string> &FunctionsMSet)
{
    // build cfgFunctions file
    if (FunctionsMSet.empty() == false)
    {
        std::ofstream ofilelib;
        std::cout << "NewFile: " << fnc_libFilename_ << std::endl;
        ofilelib.open((std::filesystem::path)fnc_libFilename_, std::ios::app);
        ofilelib << "tag = " << "\"" << fnc_tag_ << "\"; // the functions will be named " << fnc_tag_ << "_fnc_xyz" << std::endl;
        ofilelib << "class " << fnc_category_ << std::endl;
        ofilelib << "{" << std::endl;
        for (auto const& entry : FunctionsMSet)
        {
            ofilelib << "\tclass " << entry << " {};" << std::endl;
        }
        ofilelib << "};" << std::endl;
        ofilelib.close();
    }
}
void Fsplitter::updateFunctionNames(const std::multimap <std::string, std::string> &FunctionsReplacementMMap)
{
    std::ofstream ofuncFile;
    for (const auto& entry : std::filesystem::directory_iterator(Outpath_))
    {
        std::filesystem::path fname = entry.path();
        std::filesystem::path fnameOld = fname;
        if (fname.extension().string().find(tempExtention_) != std::string::npos)
        {
            std::ifstream istrm(fname, std::ios::in);
            if (istrm.is_open() && istrm.good())
            {
                for (std::string line; std::getline(istrm, line); )
                {
                    for (auto const& entry : FunctionsReplacementMMap)
                    {
                        int pos = 0;

                        while ((pos = (int)line.find(entry.first, pos)) != std::string::npos)
                        {
                            line.erase(pos, entry.first.size());
                            line.insert(pos, entry.second);
                            if (DEBUG >= DEBUG_LVL1) std::cout << ".";
                            pos += (int)entry.second.size();
                        }
                    }
                    fname.replace_extension(".sqf");
                    ofuncFile.open(fname, std::ios::app);
                    ofuncFile << line << std::endl;
                    ofuncFile.close();
                }
                istrm.close();
            }
            std::filesystem::remove(fnameOld);
        }
    }
}
