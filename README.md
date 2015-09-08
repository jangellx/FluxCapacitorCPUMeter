# Flux Capacitor CPU Meter and Low Memory Indicator
Windows and Arduino code to turn a flux capacitor replica into a CPU activity and low memory indicator.  Full details of the build can be found (on my website)[http://www.tmproductions.com/projects-blog/2014/12/20/flux-capacitor-cpu-meter].

This is a project to convert a replica Back to the Future flux capacitor into an Arduino-driven CPU activity meter and low memory monitor through an assocaited Windows system tray application

The Windows code is my first foray in t WFA and Windows managed code and is written in C++, although it's clear I should have learned enoguh C# to simplify development.  It mostly works, although it has a few issues.  It has manual overrides, but normally it just reads the CPU speed and memory usage once a second (same as Task Manager), encodes it as a string, and sends it to the Arduino as a speed and color flag over teh serial port.

The Arduino code simply reads data from teh serial port, and changes the speed of the LEDs to match.  If the color flag is set, the alternate LED color is used (red to indicate low memory, blue-ish to indicate normal operation).  This was my first real Arduino project, but it's just C so it's pretty straight forward.

The Eagle project describes the simple construction of the electronics.  It is my first eagle project, but there's not too much to it.

This was rather specifically made for my own purposes, but if anyone else finds it useful, go for it.

## Windows Source: FluxCapacitorCPUMeter vs. FluxCapacitorCPUMeter-WFA
FluxCapacitorCPUMeter is an early version of this project, and can safely be ignored.  FluxCapacitorCPUMeter-WFA is the one I run daily, and is really the one you want to look at.

## Arduino Source
FluxCapacitorCPUMeter is the code that runs on my Ardiuno.  I also incldued a modified version of Blink that I was playing around with while developing this.
