# Short overview
## com
- -source COMn
## direct
- device RTTD
## http
- try out
## jlinkRTTLogger
- device jlinkRTTLogger
- 8 byte TRICE packages
## random
- -device RND
## rttfile
- devive RTTF
- 4 or 8 byte TRICE packages
## segger
- device RTT
## simulator
- device SIM




# Short overview
## com
-port COMn
- format bare|sure
## tcp
- port localhost:19021
- format bare|sure
## random
-port RND
## file
- port file.name
- format bare|sure
## simulator
- port SIM
- format bare|sure
## jlinkRTTLogger
- port JLRTT
- format bare|sure



trice log -src COMn [-speed 115200] [-format bare|sure]
trice log -src localhost:19021
trice log -src 127.0.0.1:19021
trice log -src 19021
trice log -src RND
trice log -src SIM
trice log -src JRTT
trice log -src STRTT
trice log -src file.name









