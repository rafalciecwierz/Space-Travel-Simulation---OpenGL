#include "glew.h"
#include "freeglut.h"
#include "glm.hpp"
#include "ext.hpp"
#include <iostream>
#include <cmath>

#include "Shader_Loader.h"
#include "Render_Utils.h"
#include "Camera.h"
#include "Texture.h"


GLuint programColor;
GLuint programTexture;
GLuint programTextureNL;
GLuint program;
GLuint dzialkoTexture;
GLuint sun_Texture;
GLuint camo;
GLuint moon;
GLuint g1_Texture;
GLuint g2_Texture;
GLuint g3_Texture;
GLuint g4_Texture;
GLuint g5_Texture;
GLuint g6_Texture;
GLuint g7_Texture;
GLuint g8_Texture;
GLuint g9_Texture;
GLuint galaxy_Texture;
GLuint statek_Texture;

Core::Shader_Loader shaderLoader;
obj::Model gunModel;
obj::Model shipModel;
obj::Model sphereModel;
float cameraGunAngle = 0;
float cameraAngle = 0;
bool flaga = false;
glm::vec3 cameraPos = glm::vec3(-5, 0, 0);
glm::vec3 cameraDir;

glm::mat4 cameraMatrix, perspectiveMatrix, gunModelMatrix;

glm::vec3 lightDir = glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f));


void keyboard(unsigned char key, int x, int y)
{
	float angleSpeed = 0.1f;
	float moveSpeed = 0.1f;
	float moveSpeed2 = 0.9f;
	switch(key)
	{
	case 'z': cameraAngle -= angleSpeed; break;
	case 'x': cameraAngle += angleSpeed; break;
	case 'w': cameraPos += cameraDir * moveSpeed; break;
	case 's': cameraPos -= cameraDir * moveSpeed; break;
	case 'd': cameraPos += glm::cross(cameraDir, glm::vec3(0,1,0)) * moveSpeed; break;
	case 'a': cameraPos -= glm::cross(cameraDir, glm::vec3(0,1,0)) * moveSpeed; break;
	case 'e': cameraPos += cameraDir * moveSpeed2; break;
	case 'm':
		if(flaga == true)
			flaga=false;
		else flaga = true;
		break;

	case 'o':	cameraPos += glm::vec3(0, 0.5f, 0);	break;
	case 'l':	cameraPos -= glm::vec3(0, 0.5f, 0);	break;
	case 'p':	cameraGunAngle += angleSpeed; break;
	case 'i':	cameraGunAngle -= angleSpeed; break;
	}
}


glm::mat4 createCameraMatrix()
{
	// Obliczanie kierunku patrzenia kamery (w plaszczyznie x-z) przy uzyciu zmiennej cameraAngle kontrolowanej przez klawisze.
	cameraDir = glm::vec3(cosf(cameraAngle), 0.0f, sinf(cameraAngle));
	glm::vec3 up = glm::vec3(0,1,0);

	return Core::createViewMatrix(cameraPos, cameraDir, up);
}

void drawObjectTexture(obj::Model * model, glm::vec3 center, glm::vec3 distance, GLuint texture, glm::mat4 scaling, float time)
{
	GLuint program = programTexture;
	glm::mat4 rotation;


	rotation[0][0] = cosf(time);
	rotation[0][2] = sinf(time);
	rotation[2][0] = -sinf(time);
	rotation[2][2] = cosf(time);

	glm::mat4 modelMatrix = glm::translate(glm::vec3(center)) * rotation * glm::translate(glm::vec3(distance));
	
	glUseProgram(program);

	glm::vec3 lightDirection = glm::normalize(glm::vec3(modelMatrix[0].x, modelMatrix[0].y, modelMatrix[0].z));

	glUniform3f(glGetUniformLocation(program, "lightDir"), lightDirection.x, lightDirection.y, lightDirection.z);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix * scaling;

	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);

	Core::SetActiveTexture(texture, "texture", program, 0);
	Core::DrawModel(model);

	glUseProgram(0);
}


void drawObjectSatelite(obj::Model * model, glm::vec3 center, glm::vec3 planet, glm::vec3 distance, GLuint texture, glm::mat4 scaling, float time, float planetTime) {
	GLuint program = programTexture;
	glm::mat4 rotation;
	glm::mat4 planetRotation;


	rotation[0][0] = cosf(time);
	rotation[0][2] = sinf(time);
	rotation[2][0] = -sinf(time);
	rotation[2][2] = cosf(time);

	planetRotation[0][0] = cosf(planetTime);
	planetRotation[0][2] = sinf(planetTime);
	planetRotation[2][0] = -sinf(planetTime);
	planetRotation[2][2] = cosf(planetTime);

	glm::mat4 planetMatrix = glm::translate(glm::vec3(center)) * rotation * glm::translate(glm::vec3(planet));

	glm::mat4 modelMatrix = planetMatrix * planetRotation* glm::translate(glm::vec3(distance));

	glUseProgram(program);

	glm::vec3 lightDirection = glm::normalize(glm::vec3(modelMatrix[0].x, modelMatrix[0].y, modelMatrix[0].z));

	glUniform3f(glGetUniformLocation(program, "lightDir"), lightDirection.x, lightDirection.y, lightDirection.z);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix * scaling;

	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);

	Core::SetActiveTexture(texture, "texture", program, 0);
	Core::DrawModel(model);

	glUseProgram(0);

}

void drawObjectTextureNL(obj::Model * model, glm::mat4 modelMatrix, GLuint texture, glm::mat4 scaling, bool light){
		GLuint program = programTextureNL;

		glUseProgram(program);

		glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);

		glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix * scaling;
		glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
		glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);

		Core::SetActiveTexture(texture, "texture", program, 0);
		Core::DrawModel(model);

		glUseProgram(0);
	}
void drawObjectTexture(obj::Model * model, glm::mat4 modelMatrix, GLuint texture, glm::mat4 scaling)
{
	GLuint program = programTexture;
	glUseProgram(program);
	glm::vec3 lightDirection = glm::normalize(glm::vec3(modelMatrix[3].x, modelMatrix[3].y, modelMatrix[3].z));
	glUniform3f(glGetUniformLocation(program, "lightDir"), lightDirection.x, lightDirection.y, lightDirection.z);



	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix * scaling;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);

	Core::SetActiveTexture(texture, "texture", program, 0);
	Core::DrawModel(model);
	
	glUseProgram(0);
}


void renderScene()
{
	// Aktualizacja macierzy widoku i rzutowania. Macierze sa przechowywane w zmiennych globalnych, bo uzywa ich funkcja drawObject.
	// (Bardziej elegancko byloby przekazac je jako argumenty do funkcji, ale robimy tak dla uproszczenia kodu.
	//  Jest to mozliwe dzieki temu, ze macierze widoku i rzutowania sa takie same dla wszystkich obiektow!)
	cameraMatrix = createCameraMatrix();
	perspectiveMatrix = Core::createPerspectiveMatrix();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//ruch
	float time = glutGet(GLUT_ELAPSED_TIME) / 10000.0f;


	glm::mat4 scale = glm::scale(glm::vec3(6.0f));
	//glm::mat4 translate = glm::translate(glm::vec3(0.5, 0.5, -35));

	// Macierz statku "przyczepia" go do kamery. Warto przeanalizowac te linijke i zrozumiec jak to dziala.
	glm::mat4 shipModelMatrix = glm::translate(cameraPos + cameraDir * 0.5f + glm::vec3(0, -0.25f, 0)) * glm::rotate(-cameraAngle + glm::radians(90.0f), glm::vec3(0,1,0)) * glm::scale(glm::vec3(0.25f));
	gunModelMatrix = glm::translate(cameraPos + cameraDir * 0.5f + glm::vec3(0, -0.25f, 0)) * glm::rotate(-cameraGunAngle - cameraAngle + glm::radians(90.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(0.25f));
	

	
	
	drawObjectTexture(&gunModel, gunModelMatrix, dzialkoTexture, glm::scale(glm::vec3(0.4f)));
	drawObjectTexture(&shipModel, shipModelMatrix, statek_Texture, glm::scale(glm::vec3(1.0f)));
	drawObjectTextureNL(&sphereModel, glm::translate(glm::vec3(-10, 0, -10)), galaxy_Texture, glm::scale(glm::vec3(90.2f)), true);
	if(flaga == true) drawObjectTexture(&sphereModel, shipModelMatrix, camo, glm::scale(glm::vec3(1.0f)));
	drawObjectTexture(&sphereModel, glm::vec3(0, 0, 0), glm::vec3(10, 0, 0), g1_Texture, glm::scale(glm::vec3(1.6f)), time*0.5);

	drawObjectTexture(&sphereModel, glm::vec3(0, 0, 0), glm::vec3(23, 0, 0), g2_Texture, glm::scale(glm::vec3(1.6f)), time*0.7);
	drawObjectSatelite(&sphereModel, glm::vec3(0, 0, 0), glm::vec3(23, 0, 0), glm::vec3(2, 0, 0), moon, glm::scale(glm::vec3(0.5f)), time*0.7, time * 10);

	drawObjectTexture(&sphereModel, glm::vec3(0, 0, 0), glm::vec3(34, 0, 0), g3_Texture, glm::scale(glm::vec3(1.6f)), time*0.9);
	drawObjectSatelite(&sphereModel, glm::vec3(0, 0, 0), glm::vec3(34, 0, 0), glm::vec3(2, 0, 0), moon, glm::scale(glm::vec3(0.5f)), time*0.7, time * 10);

	drawObjectTexture(&sphereModel, glm::vec3(0, 0, 0), glm::vec3(48, 0, 0), g4_Texture, glm::scale(glm::vec3(1.6f)), time*1.0);

	drawObjectTexture(&sphereModel, glm::vec3(0, 0, 0), glm::vec3(62, 0, 0), g5_Texture, glm::scale(glm::vec3(1.6f)), time*1.3);

	drawObjectTexture(&sphereModel, glm::vec3(0, 0, 0), glm::vec3(75, 0, 0), g6_Texture, glm::scale(glm::vec3(1.6f)), time*1.5);

	drawObjectTexture(&sphereModel, glm::vec3(0, 0, 0), glm::vec3(87, 0, 0), g7_Texture, glm::scale(glm::vec3(1.6f)), time*1.7);
	drawObjectTextureNL(&sphereModel, glm::translate(glm::vec3(0, 0, 0)), sun_Texture, scale, true);
	

	glutSwapBuffers();
}

void init()
{
	glEnable(GL_DEPTH_TEST);
	programColor = shaderLoader.CreateProgram("shaders/shader_color.vert", "shaders/shader_color.frag");
	programTexture = shaderLoader.CreateProgram("shaders/shader_tex.vert", "shaders/shader_tex.frag");
	programTextureNL = shaderLoader.CreateProgram("shaders/shader_tex.vert", "shaders/shader_texNL.frag");
	galaxy_Texture = Core::LoadTexture("textures/galaxy.png");
	sun_Texture = Core::LoadTexture("textures/slonce.png");
	g1_Texture = Core::LoadTexture("textures/planet1.png");
	g2_Texture = Core::LoadTexture("textures/planet2.png");
	g3_Texture = Core::LoadTexture("textures/earth.png");
	g4_Texture = Core::LoadTexture("textures/planet3.png");
	g5_Texture = Core::LoadTexture("textures/planet6.png");
	g6_Texture = Core::LoadTexture("textures/planet5.png");
	g7_Texture = Core::LoadTexture("textures/planet7.png");
	statek_Texture = Core::LoadTexture("textures/ufo.png");
	sphereModel = obj::loadModelFromFile("models/sphere.obj");
	shipModel = obj::loadModelFromFile("models/ufo.obj");
	gunModel = obj::loadModelFromFile("models/dzialko.obj");
	dzialkoTexture = Core::LoadTexture("textures/dzialko.png");
	camo = Core::LoadTexture("textures/kamuflage.png");
	moon = Core::LoadTexture("textures/moon.png");
}

void shutdown()
{
	shaderLoader.DeleteProgram(programColor);
	shaderLoader.DeleteProgram(programTexture);
}

void idle()
{
	glutPostRedisplay();
}

int main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Uk³ad Planetarny - Rafa³.C  i Krzysztof.S");
	glewInit();

	init();
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(renderScene);
	glutIdleFunc(idle);

	glutMainLoop();

	shutdown();

	return 0;
}
