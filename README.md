# ode-solver
A tool for simulating charged particle orbits in fusion plasmas.

This tool was developed as part of the Bachelor project TIFX02-15-41 at the department for Applied Physics, Chalmers University of Technology (2015).

## Description
When designing a fusion device, knowledge of particle motion with the device is necessary. The aim of this project has been to develop a simulation tool capable of calculating orbits for different kinds of particles, and then to analyze the orbits and explain them.

The simulation tool utilizes two different methods for simulation: regular particle motion (derived directly from the Lorentz force), and the guiding-center method. These methods have also been compared in the thesis with regard to performance and agreement, and as expected it turns out that the guiding-center method is far superior in both of these.

## Simulating an alpha particle
The simplest way to simulate an alpha particle is by creating a "pi"-file (for "particle information") in the same directory as the 'solver' executable is located. Add the following contents to the pi-file:

```
tend=2.6e-5
r0=8,0,0.3
v0=-9.5487e6,-7.7664e6,-4.1652e6
magnetic_field=tests/iter2d.bkg
domain_file=tests/iter.wall_2d
mass=4
charge=2
output_file=particle.csv
print_settings!
```

Next, pass this file to the solver by typing

```
$ ./solver pi
```

in a terminal. This will create an output file with the name "particle.csv". The data should be fairly straight-forward to read and plot using for example Matlab.

## Authors
The tool was written by:
- Mathias Hoppe (hoppe)
- Aylwin Iantchenko (aylwin)
- Ingrid Strandberg (ingstra)

For further information, please contact either of the authors above by email (add @student.chalmers.se to the names within parantheses above).
