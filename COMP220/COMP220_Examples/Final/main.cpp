//main.cpp - defines the entry point of the application

#include "main.h"

int main(int argc, char* args[])
{
	//Initialises the SDL Library, passing in SDL_INIT_VIDEO to only initialise the video subsystems
	//https://wiki.libsdl.org/SDL_Init
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		//Display an error message box
		//https://wiki.libsdl.org/SDL_ShowSimpleMessageBox
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, SDL_GetError(), "SDL_Init failed", NULL);
		return 1;
	}

	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

	//Create a window, note we have to free the pointer returned using the DestroyWindow Function
	//https://wiki.libsdl.org/SDL_CreateWindow
	SDL_Window* window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	//Checks to see if the window has been created, the pointer will have a value of some kind
	if (window == nullptr)
	{
		//Show error
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, SDL_GetError(), "SDL_CreateWindow failed", NULL);
		//Close the SDL Library
		//https://wiki.libsdl.org/SDL_Quit
		SDL_Quit();
		return 1;
	}

	//lets ask for a 3.2 core profile version of OpenGL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GLContext GL_Context = SDL_GL_CreateContext(window);
	if (GL_Context == nullptr)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, SDL_GetError(), "SDL GL Create Context failed", NULL);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}
	//Initialize GLEW
	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, (char*)glewGetErrorString(glewError), "GLEW Init Failed", NULL);
	}


	std::vector<Mesh*> meshes;
	loadMeshFromFile("Tank1.FBX", meshes);

	GLuint textureID = loadTextureFromFile("Tank1DF.png");

	vec3 trianglePosition = vec3(0.0f,0.0f,0.0f);
	vec3 triangleScale = vec3(1.0f, 1.0f, 1.0f);
	vec3 triangleRotation = vec3(radians(0.0f), 0.0f, 0.0f);

	
	mat4 translationMatrix = translate(trianglePosition);
	mat4 scaleMatrix = scale(triangleScale);
	mat4 rotationMatrix= rotate(triangleRotation.x, vec3(1.0f, 0.0f, 0.0f))*rotate(triangleRotation.y, vec3(0.0f, 1.0f, 0.0f))*rotate(triangleRotation.z, vec3(0.0f, 0.0f, 1.0f));

	mat4 modelMatrix = translationMatrix*rotationMatrix*scaleMatrix;

	// Camera Properties
	vec3 cameraPosition = vec3(0.0f, 0.0f, -8.0f);
	vec3 cameraTarget = vec3(0.0f, 0.0f, 0.0f);
	vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);
	vec3 cameraDirection = vec3(0.0f);
	vec3 FPScameraPos = vec3(0.0f);	
	float CameraX = 0.0f;
	float CameraY = 0.0f;
	float CameraDistance = (float)(cameraTarget - cameraPosition).length();
	

	mat4 viewMatrix = lookAt(cameraPosition, cameraTarget, cameraUp);

	mat4 projectionMatrix = perspective(radians(90.0f), float(800 / 600), 0.1f, 100.0f);

	//Light
	vec3 lightDirection = vec3(0.0f, 0.0f, -1.0f);
	vec4 diffuseLightColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	vec4 specularLightColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	vec4 ambientLightColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);

	//Material
	vec4 ambientMaterialColour = vec4(0.2f, 0.2f, 0.2f, 1.0f);
	vec4 diffuseMaterialColour = vec4(0.6f, 0.6, 0.6f, 1.0f);
	vec4 specularMaterialColour = vec4(1.0f, 1.0, 1.0, 1.0f);
	float specularPower = 25.0f;

	GLuint programID = LoadShaders("lightingVert.glsl", "lightingFrag.glsl");

	

	static const GLfloat fragColour[] = { 0.0f,1.0f,0.0f,1.0f };

	GLint fragColourLocation = glGetUniformLocation(programID, "fragColour");
	GLint currentTimeLocation= glGetUniformLocation(programID, "time");
	GLint modelMatrixLocation = glGetUniformLocation(programID, "modelMatrix");
	GLint viewMatrixLocation = glGetUniformLocation(programID, "viewMatrix");
	GLint projectionMatrixLocation = glGetUniformLocation(programID, "projectionMatrix");
	GLint textureLocation = glGetUniformLocation(programID, "baseTexture");
	//Camera Location
	GLint cameraPositionLocation = glGetUniformLocation(programID, "cameraPosition");

	//Lights
	GLint lightDirectionLocation = glGetUniformLocation(programID, "lightDirection");
	GLint ambientLightColourLocation = glGetUniformLocation(programID, "ambientLightColour");
	GLint diffuseLightColourLocation = glGetUniformLocation(programID, "diffuseLightColour");
	GLint specularLightColourLocation = glGetUniformLocation(programID, "specularLightColour");

	//Materials
	GLint ambientMaterialColourLocation = glGetUniformLocation(programID, "ambientMaterialColour");
	GLint diffuseMaterialColourLocation = glGetUniformLocation(programID, "diffuseMaterialColour");
	GLint specularMaterialColourLocation = glGetUniformLocation(programID, "specularMaterialColour");
	GLint specularPowerLocation = glGetUniformLocation(programID, "specularPower");

	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetRelativeMouseMode(SDL_bool(SDL_ENABLE));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	int lastTicks = SDL_GetTicks();
	int currentTicks = SDL_GetTicks();


	//Event loop, we will loop until running is set to false, usually if escape has been pressed or window is closed
	bool running = true;
	//SDL Event structure, this will be checked in the while loop
	SDL_Event ev;
	while (running)
	{
		//Poll for the events which have happened in this frame
		//https://wiki.libsdl.org/SDL_PollEvent
		while (SDL_PollEvent(&ev))
		{
			//Switch case for every message we are intereted in
			switch (ev.type)
			{
				//QUIT Message, usually called when the window has been closed
			case SDL_QUIT:
				running = false;
				break;

			case SDL_MOUSEMOTION:
				// Get Mouse Motion of X and Y
				CameraX += ev.motion.xrel / 200.0f;
				CameraY += -ev.motion.yrel / 200.0f;
				// Limit camera range
				if (CameraY > 150.0f) CameraY = 150.0f; else if (CameraY < -150.0f) CameraY = -150.0f;
				// Calculate camera target using CameraX and CameraY
				cameraTarget = cameraPosition + CameraDistance * vec3(cos(CameraX), tan(CameraY), sin(CameraX));
				// Normalised camera direction
				cameraDirection = normalize(cameraTarget - cameraPosition);
				
				break;

				//KEYDOWN Message, called when a key has been pressed down
			case SDL_KEYDOWN:
				//Check the actual key code of the key that has been pressed
				switch (ev.key.keysym.sym)
				{
					// Keys
				case SDLK_ESCAPE:
					running = false;
					break;
				case SDLK_w:
					FPScameraPos = cameraDirection * 0.1f;
					cameraPosition += FPScameraPos;
					cameraTarget += FPScameraPos;
					break;
				case SDLK_s:
					FPScameraPos = -cameraDirection * 0.1f;
					cameraPosition += FPScameraPos;
					cameraTarget += FPScameraPos;
					break;
				case SDLK_a:
					FPScameraPos = -cross(cameraDirection, cameraUp) * 0.5f;
					cameraPosition += FPScameraPos;
					cameraTarget += FPScameraPos;
					break;
				case SDLK_d:
					FPScameraPos = cross(cameraDirection, cameraUp) * 0.5f;
					cameraPosition += FPScameraPos;
					cameraTarget += FPScameraPos;
					break;
				case SDLK_e:
					FPScameraPos = cameraUp * 0.1f;
					cameraPosition += FPScameraPos;
					cameraTarget += FPScameraPos;
					break;
				case SDLK_q:
					FPScameraPos = cameraUp * 0.1f;
					cameraPosition -= FPScameraPos;
					cameraTarget -= FPScameraPos;
					break;
				}
			}
		}
		//Update Game and Draw with OpenGL!!

		//Recalculate translations
		rotationMatrix = rotate(triangleRotation.x, vec3(1.0f, 0.0f, 0.0f))*rotate(triangleRotation.y, vec3(0.0f, 1.0f, 0.0f))*rotate(triangleRotation.z, vec3(1.0f, 0.0f, 1.0f));
		modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
		viewMatrix = lookAt(cameraPosition, cameraTarget, cameraUp);

		currentTicks = SDL_GetTicks();
		float deltaTime = (float)(currentTicks - lastTicks) / 1000.0f;

		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glUseProgram(programID);

		glUniform4fv(fragColourLocation, 1, fragColour);
		glUniform1f(currentTimeLocation, (float)(currentTicks) / 1000.0f);
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(modelMatrix));
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, value_ptr(viewMatrix));
		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, value_ptr(projectionMatrix));

		glUniform3fv(cameraPositionLocation, 1, value_ptr(cameraPosition));

		glUniform1i(textureLocation, 0);

		glUniform3fv(lightDirectionLocation, 1, value_ptr(lightDirection));
		glUniform4fv(diffuseLightColourLocation, 1, value_ptr(diffuseLightColour));
		glUniform4fv(ambientLightColourLocation, 1, value_ptr(ambientLightColour));
		glUniform4fv(specularLightColourLocation, 1, value_ptr(specularLightColour));

		glUniform4fv(ambientMaterialColourLocation, 1, value_ptr(ambientMaterialColour));
		glUniform4fv(diffuseMaterialColourLocation, 1, value_ptr(diffuseMaterialColour));
		glUniform4fv(specularMaterialColourLocation, 1, value_ptr(specularMaterialColour));
		glUniform1f(specularPowerLocation, specularPower);

		// Draw
		for (Mesh* currentMesh : meshes)
		{
			currentMesh->render();
		}
		SDL_GL_SwapWindow(window);

	}

	auto iter = meshes.begin();
	while (iter != meshes.end())
	{
		if ((*iter))
		{
			(*iter)->destroy();
			delete (*iter);
			iter = meshes.erase(iter);
		}
		else
		{
			iter++;
		}
	}

	meshes.clear();

	glDeleteProgram(programID);
	glDeleteTextures(1, &textureID);

	//Delete context
	SDL_GL_DeleteContext(GL_Context);

	//Destroy the window and quit SDL2, NB we should do this after all cleanup in this order!!!
	//https://wiki.libsdl.org/SDL_DestroyWindow
	SDL_DestroyWindow(window);

	IMG_Quit();

	//https://wiki.libsdl.org/SDL_Quit
	SDL_Quit();

	return 0;
}