# OpenBurn

OpenBurn is an open-source solid rocket engine simulation and design platform designed from the ground up for the amateur
rocketry community. OpenBurn is based on C++/Qt5, and has support across all major platforms (Windows, OSX, Linux).

## Features

- OpenBurn is intuitive and easy to use for designing any solid rocket motor. 
- Motor cross section (grain geometry and nozzle) updates in real time as you design
- Grain regression and chamber pressure are calculated for a steady-state chamber based on Saint Robert's law `r = aP^n`. 
- OpenBurn will additionally calculate thrust and total impulse for the motor using the isentropic flow models, accounting for two-phase flow, skin friction, and divergent shape losses in the nozzle expansion cone.
- Support for many different units
- Animated cross section view of the motor burn
- Mass flux at any point in the chamber

### OpenBurn Design
![OpenBurn main screen](https://i.imgur.com/afWFBhU.png)

### OpenBurn Simulation
![OpenBurn results screen](https://i.imgur.com/7noiiTj.png)

## Planned/In-Progress Features (\* - in progress)

- Every major 2D port (core) geometry
- A 2D geometric-based regression model allowing for any arbitrary 2D grain shape. 
- An internal chemical equilibrium solver for shifting or frozen equlibrium, so the user can easily
calculate necesscary propellant constants such as C\*, rho, gamma, T0, Pr, and more. 
- \*If the user supplies advanced propellant interal ballistics parameters such as Prandtl number, adiabatic flame temperature, and gas viscosity (calculated using OpenBurn's internal CE solver), OpenBurn will calculate erosive burning effects on the grain surface.

## Compiling

Compiling OpenBurn _should_ be easy on any platform as long as you have the [latest version of the Qt SDK.](https://www.qt.io/download-qt-for-application-development) Note that OpenBurn requires at least Qt5, so you may have to update your Qt installation.

On Linux/OSX: 

```
$ qmake OpenBurn.pro
$ make
$ ./OpenBurn
```  

Notes:  
1) If you wish to add new source files to OpenBurn, run the following to generate a new OpenBurn.pro file:  
`$ qmake -project "QT+= core gui widgets printsupport" "greaterThan(QT_MAJOR_VERSION, 5):"`
