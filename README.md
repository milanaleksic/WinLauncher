WinLauncher
===========

WinLauncher is C++ WinApi bootstrap application for complicated (multi-library) Java applications/servers for Windows.

Instead of using complicated batch files you can use this application to force a formal approach to your 
Java application deployment.


IDE/runtime requirements
------------------------

You need to have Visual Studio C++ 2010 (Express will do - that is the one I am using).

Source code depends on STL and WinApi headers, so be careful which IDE you're choosing to use.

It does not have any runtime dependencies except basic Windows DLLs. In "Release" build, after compressing using UPX,
executable is only 80KB big.

Project source code already contains built/compiled Java code so you don't have to compile them before
running code from Visual Studio.


Launcher environment and configuration
--------------------------------------

If you wish to use this application for bootstrapping your java application, the tree layout (the following listing
is based on project files in sample directory in WinLauncher source code):


```
.
|   .launcher           // **one and only configuration file**
|   debug-output.log    // debug-*.log files will be generated if "debug" is sent to WinLauncher.exe
|   debug-stderr.log
|   debug-stdout.log
|
+---lib
|       b_is_in_here.jar // this JAR file has B class which is referenced in A.java
|
+---out
|   +---production
|           A.class     // compiled A.java 
|                       // it is in git so you don't have to compile before debugging in VS)
|
\---src
        A.java          // class with main method (the class which is targeted with startup parameter)
```

You can choose to have your code in either any jar file in the lib directory or outside it in bin directory.

Lib folder is mandatory library location (although it can be empty if you don't need dependencies).

If your IDE does not compile into out directory as IntelliJ Idea does, you can replace value in
**extraClasspathDir** from the sample **.launcher** file

Additional parameters that you can place in .launcher file:

- **mainClass** - Main class to be executed (in above example it is A)
- **extra** - (optional) VM parameters (like GC or heap management or environment params setup etc)
- **executor** - (optional) program to be "executing" the bootstrap. javaw is default, but you can change it to java.exe 
  OR you can put absolute path to a java executable in environments with more than one JVM installation
- **showSplash** - (optional) If set to false it will not find first JPEG file besides .launcher file 
and use it as Java splash
- **programArgs** - (optional) You can send parameters to your Java program using this parameter
- **extraClasspathDir** - (optional) If not all of your classes are in lib\*.jar libraries but in a directory you
simply want to add to path (like out\production in above example), you can use this parameter to mention that dir.


### Java API

As part of the project source code, you are also given the simple Java API classes to ease the cooperation between 
the Java process and WinLauncher executable. The ode is straight-forward and utilized in the sample project, so
I believe no additional information is needed. 

Still, if you need any other information concerning Java API please feel free to contact me with questions and/or
open issues if you see something is wrong with the code.


Advanced topics
---------------

### "Well known" directories

To further clean up your deployment target folder, you can choose to push all files Java expects to be 
present at the start directory into a "well known" directory.

During boot-up, WinLauncher will go through the list of well known directories and if it finds any one from that list
inside the directory where executable is positioned, it will move into that directory and then search for
.launcher file etc.

Currently, the "well known" list is in the code and contains only one item: "startup".


### Singleton application and file locking

WinLauncher expects that Java application (if desired) locks configuration file .launcher when started up.
This will allow WinLauncher to early fail (before JVM has been started) and inform user that previous instance
of the Java application should be closed first.

### WinLauncher startup parameters

Besides configuration file itself, launcher can have some parameters sent to it directly:

- **debug** parameter - to turn on debug output where some debug files are created
- **nochangedir** parameter - by default, WinLauncher sets (as part of bootup process)
"current directory" to the directory where the executable is. You can turn off this behavior by sending the
"nochangedir" parameter. One suitable case where you wish to use this parameter is when you are debugging C++ code 
and you'd like to avoid changing of the current directory (instead, you want to set it to location of ".launcher" file).

Example call with parameters:
```
Launch.exe debug nochangedir
```


### WinLauncher environment parameters

- **winLauncherEnvParamWaitOnStartup** - you can use this environment parameter when starting the WinLauncher
application to ask WinLauncher to "sleep" for awhile before bootstrapping Java program. 

This is suitable in "application restart" cases where you don't want startup of Java
to fail because java still did not unlock .launcher file.



Contact & license
-----------------

My name is Milan Aleksic. You can find more about me by visiting my personal Web site 
[www.milanaleksic.net](www.milanaleksic.net). 
My email is milan.aleksic@gmail.com. 

WinLauncher has been maintained 2007-2012 by me before putting it on Github.

WinLauncher is free for non-commercial usage.
