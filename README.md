WinLauncher
=======

WinLauncher is C++ WinApi bootstrap application for complicated (multi-library) Java applications/servers for Windows.

Instead of using complicated batch files you can use this application to force a formal approach to your 
Java application deployment.

IDE requirements
------------------------------------------------------------

You need to have Visual Studio C++ 2010 (Express will do - that is the one I am using).


Launcher environment and configuration
------------------------------------------------------------

If you wish to use this application for bootstrapping your java application, the tree layout (sample from the 
source code):


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
|           A.class     // compiled A.java (placed in git on purpose so you don't have to compile to run program from VS)
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


Singleton application and file locking
------------------------------------------------------------

WinLauncher expects that Java application (if desired) locks configuration file .launcher when started up.
This will allow WinLauncher to early fail (before JVM has been started) and inform user that previous instance
of the Java application should be closed first.

WinLauncher startup parameters
------------------------------------------------------------

Besides configuration file itself, launcher can have some parameters sent to it directly:

- **debug** parameter - to turn on debug output where some debug files are created


WinLauncher environment parameters
------------------------------------------------------------

- **winLauncherEnvParamWaitOnStartup** - you can use this environment parameter when starting the WinLauncher
application to ask WinLauncher to "sleep" for awhile before bootstrapping Java program. 

This is suitable in "application restart" cases where you don't want startup of Java
to fail because java still did not unlock .launcher file.

(c) 2007-2011 by Milan Aleksic
