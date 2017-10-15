# OpenBurn

OpenBurn is an open-source internal balistics simulation software suite designed from the ground up for the amateur
rocketry community. OpenBurn is based on C++/Qt5, and will have support across all major platforms.

### Features

OpenBurn is designed for all sizes of solid rocket engines, and will feature every major grain geometry in addition
to a 2D geometric-based regression model allowing for any arbitrary 2D grain shape. 
OpenBurn calculates grain regression for a steady-state chamber based on Saint Robert's law `r = aP^n`. If the user 
supplies advanced propellant info such as Prandtl number, Adiabatic flame temperature, and combustion gas viscosity, 
OpenBurn will calculate erosive burning effects in the chamber.

### Planned Features

OpenBurn will support an internal chemical equilibrium solver for shifting or frozen equlibrium, so the user can easily
calculate necesscary propellant constants such as C*, rho, gamma, Cp, Pr, and more. 

### Compiling

Compiling OpenBurn _should_ be easy on any platform as long as you have the [latest version of the Qt SDK.](https://doc.qt.io/archives/sdk-1.2/sdk-installing.html) 

On Linux/OSX: 

```
$ qmake OpenBurn.pro
$ make
$ ./OpenBurn
```  
Notes:  
1) If you run `make` and see: `make: Nothing to be done for 'first'.`, you will need to clean the moc_* object files that qmake generates:  
`$ make clean`  
2) If you wish to add new source files to OpenBurn, run the following to generate a new OpenBurn.pro file:  
`$ qmake -project "QT+= core gui" "greaterThan(QT_MAJOR_VERSION, 4): QT += widgets"`   
3) You can turn off the automatic -Wall that qmake adds to the makefile with  
`qmake OpenBurn.pro "CONFIG += warn_off"`  
