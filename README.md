# Arma3/sqf-ScriptSplitter

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![MSBuild](https://github.com/elmo128/Arma3-ScriptSplitter/actions/workflows/msbuild.yml/badge.svg?event=push)](https://github.com/elmo128/Arma3-ScriptSplitter/actions/workflows/msbuild.yml)
[![Release](https://github.com/elmo128/Arma3-ScriptSplitter/actions/workflows/release.yml/badge.svg?event=release)](https://github.com/elmo128/Arma3-ScriptSplitter/actions/workflows/release.yml)

Tool to help splitting files containing several sqf functions into a
functions library.

This is a quick and dirty tool to assist you when splitting script
files, containing many functions into a functions library, although it
has some limitations (eg. no nested functions), it has the potential to
speed up your work, as it also renames functions defined in the input
file to TAG_fnc_functionName, whenever they are used in a one file
script.

Example of operation: If you call the main executable with the
parameters:

1)  The function tag, in the example below XY. If your input file does
    not contain these, please add any, using a text editor.<br />

2)  The name of the function Category*<br />

3)  A file having a structure similiar to:<br />
    Attention: the opening bracket must be in the same 
    line as the function name and equal sign
```
    mymod.sqf:
    	XY_function_1 = {
      	   ABC;
     	};
    	XY_function_2 = {
     	   [ ] call XY_function_1;
    	};
    	XY_function_3 = {
    	   DEF;
    	};
```
4)  A file holding the header for all Functions, eg. the license of the
    original file and some includes eg. license.txt 
    ```
    this is the content of license.txt
    ```

5)  The output directory<br /> Attention: The trailing slash is required!

by using the example commandline: **Arma3-ScriptSplitter.exe XY MyMod
d:/mymod.sqf d:/license.txt d:/mymodLib/**

The following files and directories will be created:
```
D:/mymodLib/
fn_function_1.sqf: 
this is the content of license.txt
// Function extracted with Arma-ScriptSplitter: XY_fnc_function_1
// Source File: "mymod.sqf"

    ABC;

fn_function_2.sqf:
    this is the content of license.txt
    // Function extracted with Arma-ScriptSplitter: XY_fnc_function_2
    // Source File: "mymod.sqf"

    [] call XY_fnc_function_1;

fn_function_3.sqf:
    this is the content of license.txt
    // Function extracted with Arma-ScriptSplitter: XY_fnc_function_3
    // Source File: "mymod.sqf"

    DEF;

cfgFunctions.hpp:
    tag = "XY"; // the functions will be named XY_fnc_xyz
    class MyMod
    {
        class function_1 {};
        class function_2 {};
        class function_3 {};
    };
```
Make sure neither the target directory, nor the files exist before executing
the program.

Additional manual tasks must be perfomed to make the newly created
functions library work, eg. registering it in description.ext, checking
if all functions have been extracted, adding additional parameters like pre-/postinit, 
filepath (file=), etc.

Further reading:
https://community.bistudio.com/wiki/Arma_3:_Functions_Library

Downloaded from: 
https://github.com/elmo128/Arma3-ScriptSplitter
