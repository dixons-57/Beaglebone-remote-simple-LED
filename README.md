# Beaglebone Black remote simple LED flash


INTRODUCTION

This is a simple project that shows you how to flash an LED you have connected to GPIO48 on your Beaglebone Black using a C++ program to manipulate the state of the pins through the Linux kernel. 

It is intended to be cross-compiled using the Linaro g++ ARM cross-compiler. It also can also send and remotely execute the program over SSH.

Moreover, it can be seen as a gentle introduction to "make".


REQUIREMENTS

You need to have the Linaro g++ ARM cross-compiler built and available on your system. 

You should have an LED connected between GPIO48 and GND of the Beaglebone Black (with an appropriate resistor)

If you want to automatically send and execute the program remotely, your Beaglebone Black needs to be connected to your network. You also need to enable "passphraseless"-SSH as described in the top answer on this post:

https://serverfault.com/questions/241588/how-to-automate-ssh-login-with-password


HOW TO USE

To simply build the program, you need to appropriately set the following two variables in the makefile: "COMPILER" "INCLUDES" (examples are given in the file for reference). Then run "make build" to produce the executable.

To send or run the program over SSH, you should also set the following three variables in the makefile "SSHIP" "USER" "TARGETLOC". 

Running "make" or "make all" will build, send, and execute the program remotely over ssh. 

Running "make send" or "make run" either only sends or only runs the program respectively