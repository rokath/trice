# How to use tracelog in your project
- #include [traceLog.h](traceLog.h) in your source file to use tracelog
- add [traceLog.c](traceLog.c) to your project
- copy, rename to 'trceLogConfig.h' and adapt [proj_traceLogConfig.h](proj_traceLogConfig.h) to your compiler
- copy, rename to 'traceLogInterface.h' and adapt
  [proj_traceLogInterface.h](proj_traceLogInterface.h) to your hardware
- [traceLogCheck.c](../testdata/traceLogDemoF030R8/Src/traceLogCheck.c) 
  is example code and for testing