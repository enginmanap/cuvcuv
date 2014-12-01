Raytracer

This is a raytracer that I am doing as hobby. It is tested on ubuntu and windows.

Requirements:
	gcc 4.8
	autotools
	Freeimage3 (optional)
	
Build & Run:
	To build use command "make". To run ./bin/rayTracer scenefileName (.exe for windows)

Features:

* primitives
	- Triangle
	- Sphere
* Blinn-phong shading
* Spatial partitioning using octree
* Stochastic anti aliasing
* hard shadows

Input: 
	Human readable TXT

Output:
	PNG. Either using Freeimage or build in exporter, decided at compile time
	
Todo:
	Depth of field
	Refraction
	Soft shadows

Credits:
Engin Manap

Thanks:
Sean T. Barrett. for PNG output header