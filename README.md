# Frequentist-Analysis-CMS

The code of the frequentist analysis for the compositeness and large extra dimension (LED) search at CMS. There are two folders in this repository: one is CMS-Frequentist-MonteCarlo, which contains Monte Carlo code to find the frequentist limits, and the other is called FrequentistAnalyticAnalysis, which contains the analytic code and used to contain the code using a hybrid approach until it got accidentally deleted. 
## Compilation and Use Directions
### Information about CLion
CLion is a cross-platform IDE that works similarly to Visual Studio.  If you are at IMSA, you can sign up for JetBrains' Educational License, which will allow you to use CLion for free. This project originally used Visual Studio as its IDE (if you're at IMSA and on Windows, lucky you, you don't have to work through WSL!), but as Visual Studio is not cross-platform and my main computer was a Linux machine, I ported the project to effectively use CLion as my IDE. If you are on Windows, please don't switch the project back to Visual Studio.
### Windows 
 1. Download and install [CLion](https://www.jetbrains.com/clion/) and [Git](https://gitforwindows.org/) if you don't have WSL2 installed.
 2. Configure CLion on Windows using MinGW-w64 using  [this tutorial](https://www.jetbrains.com/help/clion/quick-tutorial-on-configuring-clion-on-windows.html#MinGW).
 3. [Log into GitHub and clone this repository](https://www.jetbrains.com/help/clion/manage-projects-hosted-on-github.html).
 4. Build and run!
### macOS
Setup is largely the same as Linux (see below), although with macOS there is no need to worry about the installation of g++, pthread, and tbb.
When installing CLion, [use this guide for macOS](https://www.jetbrains.com/help/clion/quick-tutorial-on-configuring-clion-on-macos.html).
### Linux

1. Make sure that the a version of g++ compatible with C++17 is installed, as well as Intel's pthread and tbb that are compatible with C++17 (see this [StackOverflow answer](http://web.archive.org/web/20201116224311/https://stackoverflow.com/questions/51031060/are-c17-parallel-algorithms-implemented-already/55989883) for more information.)
2. Download and install [CLion](https://www.jetbrains.com/clion/) for Linux.
3. [Log into GitHub and clone this repository](https://www.jetbrains.com/help/clion/manage-projects-hosted-on-github.html), or clone this repository while having an GitHub SSH key on the command line.
4. Build and run!
### Other Workflows
The workflows above are recommended for most people, but they are by no means the only ones. If you have another workflow or you want to come up with one, all the power to you! Just make sure you have these dependencies:
- A version of CMake >= 3.19
- A C++17 compatible compiler with parallelism support



