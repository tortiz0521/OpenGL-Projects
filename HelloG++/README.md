This project is meant to be a quick introduction to the whole process of using g++! Use this to understand how to make a simple g++ process.

Why have a .cpp for a header???:

    When the compiler goes through the linking process, we want to ensure that it DOES NOT attempt to create multiple of the same definitions.
    Every file that includes 'func.h', or whatever header you are using, will reference the same definition, and there will be no errors caused!

Makefile:
    
    Makefiles can be made in a few different ways, but this was seems pretty helpful. One struggle with getting the 'make' command to work on the PowerShell
    terminal was simply not knowing that I need to have the path as general User/System variable on this PC. Doing that was simple; got to the
    computer's Environment Variables and add these paths to the 'Path' option of both User/System:

        C:\msys64\usr\bin
        C:\msys64\mingw64\bin

    Make sure whenever you create a Makefile to include some sort of clean up process. This will help with the different object variables that
    get created in the environment. It will also clean up the executable created when initially running the program.