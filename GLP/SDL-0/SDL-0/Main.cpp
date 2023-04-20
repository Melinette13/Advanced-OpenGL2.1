#include <iostream>
#include <SDL.h>
#include <glew.h>
#include <fstream>
#include <string>
using namespace std;
//#define GLEW_STATIC

string LoadShader(string fileName) {
	ifstream myFile;
	myFile.open(fileName);
	if (myFile.fail()) {
		cerr << "Error - failed to open " << fileName << endl;
	}
	string fileText = "";
	string line = "";
	while (getline(myFile, line)) {
		fileText += line + '\n';
	}

	myFile.close();
	return fileText;
}

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		cout << "SDL initialization failed. SDL Error: " << SDL_GetError();
	}
	else
	{
		cout << "SDL initialization succeeded!\n";
	}
	///////////SETTING UP SDL/////////////
	//Create a simple window
	int width = 900;
	int height = 900;
	unsigned int center = SDL_WINDOWPOS_CENTERED;
	SDL_Window* Window = SDL_CreateWindow("My window", center, center, width, height, SDL_WINDOW_OPENGL);
	//SDL_WINDOW_OPENGL is a u32 flag !


	//Create an OpenGL compatible context to let glew draw on it
	SDL_GLContext Context = SDL_GL_CreateContext(Window);

	/////////SETTING UP OPENGL WITH GLEW///
	//Initialize glew
	glewExperimental = GL_TRUE;
	if (glewInit() == GLEW_OK) {
		cout << "Glew initialized successfully\n";
	}

	//Set the viewing frame through which we will see the objects
	glViewport(0, 0, width, height);

	//Put the color you want here for the background
	glClearColor(0.0f, 0.5f, 0.0f, 0.0f);





	float vertices[] =
	{
	 0.0f, 0.0f, 0.0f, 1.f, 0.5f, 0.2f,
	 -2.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	 -2.0f,  0.0f, 0.0f, 1.f, 0.5f, 0.2f,
	 -2.0f,  -1.0f, 0.0f, 1.f, 0.5f, 0.2f,
	 0.0f, -2.5f, 0.0f,0.0f, 0.0f, 0.0f,
	 2.0f, -1.0f, 0.0f, 1.f, 0.5f, 0.2f,
	 2.0f, 0.0f, 0.0f, 1.f, 0.5f, 0.2f,
	 2.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	};
	
	

		//Create an ID to be given at object generation
	unsigned int vbo = 0;

	//Pass how many buffers should be created and the reference of the ID to get the value set
	glGenBuffers(1, &vbo);
	
	//Binds the buffer linked to this ID to the vertex array buffer to be rendered. Put 0 instead of vbo to reset the value.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//Finally send the vertices array in the array buffer (linked to vbo)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	string vs = LoadShader("NewVertexMieux.shader");
	const char* vertexShaderSource = vs.c_str();

	string fs = LoadShader("NewFragMieux.shader");
	const char* fragmentShaderSource = fs.c_str();


	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	

	//now that we have a vertex shader, let’s put the code text inside
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);



	//aaaaand… Compile !
	glCompileShader(vertexShader);


	//Do the same with the fragment shader !
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	


	

	//now attach shaders to use to the program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	//and link it 
	glLinkProgram(shaderProgram);

	//now that the program is complete, we can use it 
	glUseProgram(shaderProgram);

	//Create one ID to be given at object generation
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);


	//Binds the buffer linked to this ID to the vertex array buffer to be rendered. Put 0 instead of vbo to reset the value.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//Finally send the vertices array in the array buffer 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);



	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);



	//Use depth management
	glEnable(GL_DEPTH_TEST);

	//0 is our origin, the higher the z, the farther the object
	glDepthFunc(GL_LESS);




	

	//Main loop
	bool isRunning = true;
	while (isRunning) {
		// Inputs
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				isRunning = false;
				break;
			default:
				break;
			}
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen
		

		//Shader to use next
		glUseProgram(shaderProgram);

		//VAO to use next
		glBindVertexArray(vao);
		// Get the time in seconds 
		float timeValue = (float)SDL_GetTicks() / 1000;
		float redColor = (sin(timeValue) / 2.0f) + 0.5f;
		//int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		//glUseProgram(shaderProgram);
		//glUniform4f(vertexColorLocation, redColor, 1.0f, 0.0f, 1.0f);
		//OMG WE FINALLY DRAW ! We use the GL_TRIANGLES primitive type
		//We draw from vertex 0 and we will be drawing 3 vertices
		glDrawArrays(GL_TRIANGLE_FAN, 0, 8);
		

		float yPosition = (cos(timeValue * 1) / 3.0f);
		int YvertexPosLocation = glGetUniformLocation(shaderProgram, "YOfest");
		glUniform1f(YvertexPosLocation, yPosition);

		float xPosition = (cos(timeValue * 5) / 3.0f);
		int XvertexPosLocation = glGetUniformLocation(shaderProgram, "XOfest");
		glUniform1f(XvertexPosLocation, xPosition);

		SDL_GL_SwapWindow(Window); // Swapbuffer
	}
	// Quit
	SDL_DestroyWindow(Window);
	SDL_GL_DeleteContext(Context);


	std::cin.get();
	return 0;
}