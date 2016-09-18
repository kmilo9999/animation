Report
CS560  Animation
Camilo Diaz
c.diaz@digipen.edu

This project was compiled in Visual Studio 2013 using C++ in debug mode X86 - Win32.

Camera Controls:
Use the AWSD Keys to move the camera around.
Keep press the mouse's middle button and drag it in order to rotate the camera.

All the libraries required for the project are in the folder "Libraries":
- Assimp : Free Library that loads 3D models and their animations.
- Anttweakbar : GUI Libray to debug the project.
- glew: Opengl Wrapper
- glfw: Create the window and handle the input
- stb-master: Loads image files.

IN ORDER TO RUN IT SUCCESSFULLY :
The following dll's must be in the Debug Folder:
- AntTweakBar.dll
- assimpd.dll
- glfw3.dll

I have implemented my own Math library (you can find it in the folder "Math"). This one uses Vectors of 2,3 and 4 dimensions,
Matrices and Quaternions. Additionally, I have implemented my own View and Projections matrices, quaternions operations
such as addition, subtraction, multiplication, conjugate and Rotational Matrix using Quaternions.
The Camera and the transformations applied to the models are VQS. The interpolation is made by SLERP.

The GUI displays several options such as draw the bones, change the model and the animation. Also, it shows how to use the 
camera controls.
Two models exist in the project a lizzard which count with 3 animation and a police officer which has only 1 animation.