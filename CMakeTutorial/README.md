This project is meant to be the basic setup for CMake!

How did we do it? Well it was honestly mind numbingly painful, but it really dowesn't have to be.
First Step: Download it. It can be downloaded and installed online.
Second Step: Make sure you have vcpkg installed properly. Just go online and you can find this anywhere.
Final Step: Copy and Paste the setup docs! It really is just that simple.

What about the IntelliSense issues? Well those are not necessarily difficult to combat. Go to the C++ extension,
find the settings, and include the path needed to allow the IntelliSense to pick up all of the header files.
All you need to type in the "Include Path" section is:

    ${workFolder}/<path from base workFolder to vcpkg>/**

Another important note to keep in mind is the compiler being used. Sometimes the linking process gets busted
because you are using "x64-windows" instead of "x64-mingw-static" or "x64-mingw-dynamic". Make sure to install the
proper version and establish as such in your 'CMakeUserPresets.json' CacheVariables!