QuickManager
============

QuickManager is database management system for Food Aid organisation Akshaypatra Foundation.


Implementation
================

Main.cpp
    |---------> mainwindow.cpp
        |---------> maindatabase.cpp
        |---------> filemanager.cpp
        |---------> alltaluka.cpp
            |---------> taluka.cpp
        |---------> allschool.cpp
            |---------> school.cpp
        |---------> schoolmanager.cpp
        |---------> schoolbillmanager.cpp
        |---------> foodlistmanager.cpp

The detail about each file can be found in the respective header file.


For Contribution
================
Source can be downloaded from : https://github.com/Vijay13/QuickManager.git

STEPS for building the application:
0. Install QtCreator 3.0.0 (or more) based on Qt 5.2.0 (MSVC 2010) (or more).
1. Open QtCreator
2. File-> Open file or Project
3. Change the file type in newly opened window from "All type" to "Qt Project file(*.pro)"
4. Open QuickManager.pro file from source code.


TODO
================
1. Add Holiday Master
2. Add total at vertical end in School Bill Manager
3. Add export functionalty to School Bill Manager
4. Add route priority to School Manager
5. Add column for Sukhadi
6. Add Horizontal bar for Total of vessels of all routes for perticular date
