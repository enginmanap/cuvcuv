Cuvcuv Raytracer

This is a raytracer that can import multiple obj/mtl files, apply arbitrary transformations on them and render with high performance. Rendering is multithreaded using openmp.

It supports unlimited vertex data and face definition with up to 20 vertex rendered as triangle fan, with per vertex normal and texture support. sphere primitive is also supported, in scene description. Antialiasing and soft shadows are supported using Monte carlo technique. Blinn-phong shading is available, so specular/diffuse light can be used, reflection and refraction is also available depending on material properties. Dissolve transparency is supported too.

The scene is partitioned using octree structure before render, and all render steps are tightly optimised to minimize time, it can render stanford dragon under 3 seconds (full runtime on a core2quad).

Requirements:
	gcc 4.8
	autotools
	Freeimage3 (optional)
	
Build & Run:
	To build use command "make". To run ./bin/rayTracer scenefileName (.exe for windows)


Features:

* primitives
	- Triangle (per pixel normals, texture support)
	- Sphere
* full transformation stack
* Blinn-phong shading
* Reflection
* Refraction (using fresnell equations)
* dissolve
* Spatial partitioning (octree)
* Anti aliasing (stochastic)
* Soft shadows
* Depth of Field

Input: 
	A scene description file is used to render, since OBJ format has only model information. Commands available are listed below.
	

	Settings:

		size: Output resolution. Required. This command must be the first, and should not be repeated. Only first one is effective. 
		sample: size 1920 1080

		camera: camera definition. Required. first 3 real numbers are camera coordinates, second 3 are looking point  third 3 are up direction. last one is an integer, and it is field of view angle, for horizontal axis. only the last of these commands are effective, multiple cameras not supported.
		sample: camera -10.5 20.5 5.5 0 18 0 0 1 0 45
		
		cameraAperture: Camera aperture setting. Default is 0, meaning there will be no depth of field. The size determines how blurry the forward and backward of looking point. DofRate must be set for acceptable results.
		sample: cameraAperture 0.25
		
		DOFRate: how many samples will be used to simulate the depth of field effect. The value is subpixel rate, so 2 means 4 samples, and 3 means 9 etc. Camera aperture must be set.
		sample: DOFRate 2

		output: filename for png output. if not given, a file with name of scene description will be used.
        	sample: output render.png

		maxdepth: Maximum depth to search for reflextion/refraction. Default value is 5. This command has a major role in performance, big numbers does not worth the result it gives for reflective surfaces, but for refractive models, little depth can cause black spots, because of total internal reflection. It has no effect on all diffuse scenes.
		sample: maxdepth 10
	
		sampleRate: Antialiasing factor. Default is 1(disabled). The number is subpixel factor, so 2 will result 4 subpixel, and 3 will result 9.
		sample: sampleRate 2

		shadowGrid: Soft shadow factor. Default is 1(disabled). It defines how many rays should be used to determine penumbra. it defines side of square, so 2 means 4 rays, and 3 means 9. Don't forget you need lamp lights for soft shadows.
		sample: shadowGrid 6

	Ligth definitions:

		point: A point light. it has 6 parameters, first 3 real numbers are position, second is color. Color is not clamped, so each must be between 0-1.
		sample: point 0 20 -40 1 1 1

		directional: A directional light. it has 6 parameters, first 3 real numbers are light direction(auto normalised), second is color. Color is not clamped, so each must be between 0-1.

		lamp: it has 7 parameters, first 3 real numbers are position, fourth one is lamb size,  rest are color. Color is not clamped, so each must be between 0-1. This is only light type that casts soft shadows, and the lamb size is used to calculate size of penumbra. You need to add shadowGrid to have penumbra too.
		sample: lamp 50 50 99 3 1 1 1

		attenuation: Makes lights less bright with distance. takes three real parameters, first one is the constant attenuation, second one is distance related attenuation, and last one is distance square related attenuation. 
		sample:attenuation 1 0.2 0.1

	Material definitions

		Ka: Ambient color in rgb. Takes three real parameters. This value is used to alter image to look like there is ambient light.
		Sample: Ka 0.5 0.7 0.7

		Kd: Diffuse color in rgb. Takes three real parameters. It is shaded to give realistic color to objects.
		Sample: Kd 0.7 0.7 0.5

		Ks: Specular color in rgb. Takes three real parameters. This value gives a tint as defined to reflection.
		Sample: Ks 0.7 0.5 0.7

		Ns: Reflectivity factor. Takes one integer parameter. It is used to define how reflective the surface is. it takes values bigger than 0.
		Sample: Ns 100

		mtllib: loads material data from another file. full description can be found on wavefront specification.
		Sample: mtllib butterfly.mtl


	Object definitions:

		sphere: Creates a sphere. First three real values are for center position, fourth real value is the radius of sphere.
		Sample: sphere 0.5 0.5 0.5 2.5

		v: Used to define a vertex position. It takes three real values as parameter.
		Sample: v 0.5 0.5 0.5

		vn: Normal direction definition. It takes three real values as parameter.
		Sample: vn 0 1 0
	
		vt: Texture coordinate definition. It defines texture coordinate to use. Takes two real parameters.
		sample vt 0.543 0.234

		f: used to define a face. Takes atleast 3 integer values, as vertex indexes. vertex indexes starts from 1(not 0).
		Sample: f 1 3 5
		If face needs texture information, this information can be given per vertex, seperated by a slash. Baricentric coordinates will be calculated to match face position and texture.
		sample: f 1/1 3/10 5/15
		if face needs normal information, this information can be given per vertex, seperated by double slashes. Normals will be interpolated automatically per pixel.
		sample: f 1//1 3//10 5//15
		If face contains both normal and texture information, both can be given.
		sample: f 1/1/1 3/10/10 5/15/15

		loadModel: loads a obj model. Since obj models can request materials, they are loaded as expected.
		Sample: loadModel butterfly.obj

	

	Transformations:
	
	Full transform stack is supported. Three basic transforms can be combined in anyway. 
	
		scale: model is scaled using 3 factors.
		Sample: scale 0.25 0.25 0.25

		translate: Model is moved given amount.
		translate translate 5 0 0

		rotate: Model is rotated on an arbitrary axis. first 3 parameters are real values for axis definition. Last parameter is integer value for rotation in degrees.
		Sample: rotate 0 1 0 45


	pushTransform: Used to add another level to transformation stack. Transform stack starts empty, so this command should be used before any transformation. Does not take any parameters. All the basic transforms are going to be combined to one transformation.
	Sample: pushTransform

	popTransform: Removes last added level of transform, including all basic transformations in it.
	Sample: popTransform
	
		

Output:
	PNG. Either using Freeimage or build in exporter, decided at compile time. Freeimage generates a better compressed image.
	
missing features:

Major:	The application does not have any advanced memory management(paging/persistance etc.), if model or does not fit the memory, it will simply crash. You can lower maximum octree depth to make it use less memory, but it will result in worse performance. 

Major:	caustics are missing. currently transparent objects does not cast any shadows. Checking photon mapping for this.

Major:	~~Normal mapping: It should be fairly easy to read the normals from textures, but it is not implemented because of time restrictions.~~ It is implemented.

Minor:	~~Depth of field should be fairly easy to implement, but it is not implemented. It is under development.~~ It is implemented.

Minor:	Sphere texturing: The texture loading is implemented, but since spheres are not used in obj, this feature was not implemented.


Credits:
Engin Manap

Thanks:
Sean T. Barrett. for PNG output and texture loading. you can check his work at http://nothings.org/
