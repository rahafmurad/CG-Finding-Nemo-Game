#include <GL\glew.h>  // A cross-platform open-source C/C++ extension loading library
#include <stdlib.h>
#include <GL\freeglut.h>   // handle the window-managing operations
#include <iostream>
#include <conio.h>
#include<math.h>
#include <sstream>
#include <windows.h>
#include <MMSystem.h>

using namespace std;

//////////////////////
// Global variables //
//////////////////////
GLint win_width = 500, win_hight = 500;
GLint obj; GLuint backgroundTexture; GLuint nemoTexture;
float fishPosX, fishPosY, fishPosZ;
int angle;
int times;
float camX = -0.5;
#define PI 3.1415926
//////////////////////

//////////////////////////
//  Background(texture) //
//////////////////////////
/* http://stackoverflow.com/questions/12518111/how-to-load-a-bmp-on-glut-to-use-it-as-a-texture */
GLuint LoadTexture(const char* filename, int width, int height)
{
	GLuint texture;
	unsigned char* data;
	FILE* file;
	//The following code will read in our RAW file
	errno_t returnValue = fopen_s(&file, filename, "rb");
	if (file == NULL)
	{
		cout << "Unable to open the image file" << endl << "Program will exit :(" << endl;
		exit(0);
		return 0;
	}
	data = (unsigned char*)malloc(width * height * 3);
	fread(data, width * height * 3, 1, file);
	fclose(file);
	// reorder the image colors to RGB not BGR
	for (int i = 0; i < width * height; ++i)
	{
		int index = i * 3;
		unsigned char B, R;
		B = data[index];
		R = data[index + 2];
		data[index] = R;
		data[index + 2] = B;
	}
	////////////////////////////////////////////////////////////////////////////
	//           All Exercises TODO: load another texture image               //
	////////////////////////////////////////////////////////////////////////////
	glGenTextures(1, &texture);            //generate the texture with the loaded data
	glBindTexture(GL_TEXTURE_2D, texture); //bind the texture to it's array
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //set texture environment parameters
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	/////////////////////////////////////////
	free(data); //free the texture array
	if (glGetError() != GL_NO_ERROR)
		printf("GLError in genTexture()\n");
	return texture; //return whether it was successfull  
}
void init() { 
	//------- Texture ---------//
	backgroundTexture = LoadTexture("sea.bmp", 1504, 500);
	//-------------------------//
}
void background()
{
	//background render
	glEnable(GL_TEXTURE_2D);// Enable the Texture
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, backgroundTexture);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0); glVertex2f(-1.0, -1.0);
	glTexCoord2f(1, 0); glVertex2f(-1.0, 1.0);
	glTexCoord2f(1, 1); glVertex2f(1.0, 1.0);
	glTexCoord2f(0, 1); glVertex2f(1.0, -1.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);// Disable the Texture
}
//////////////////////

////////////////
//    Fish    //
////////////////
static void circle(float x, float y, float radius) 
{
	int count;
	int sections = 200;
	GLfloat TWOPI = 2.0f * 3.14159f;
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y);
	for (count = 0; count <= sections; count++)
	{
		glVertex2f(x + radius * cos(count * TWOPI / sections), y + radius * sin(count * TWOPI / sections));
	}
	glEnd();
}
void eye(float radius1)
{
	float x = 0.0;
	float y = 0.1;
	float radius = 0.2;
	int count;
	int sections = 200;
	GLfloat TWOPI = 2.0f * 3.14159f;
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y);
	for (count = 0; count <= sections; count++)
	{
		glVertex2f(x + radius * cos(count * TWOPI / sections), y + radius * sin(count * TWOPI / sections));
	}
	glEnd();
	//black
	float x1 = 0.0;
	float y1 = 0.1;
	int count1;
	int sections1 = 200;
	GLfloat TWOPI1 = 2.0f * 3.14159f;
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);
	for (count1 = 0; count1 <= sections1; count1++)
	{
		glVertex2f(x + radius1 * cos(count1 * TWOPI1 / sections1), y + radius1 * sin(count1 * TWOPI1 / sections1));
	}
	glEnd();
}
void traingle() {
	glBegin(GL_TRIANGLES);
	glVertex2f(0.0, 0.0);
	glVertex2f(0.0, 1.0);
	glVertex2f(1.0, 0.5);
	glEnd();
}
void tail() {
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.8, 0.4, 0.6);
	glVertex2f(0.0, 0.0);
	glColor3f(0.5, 0.1, 0);
	glVertex2f(1.5, 1.0);
	glColor3f(0.5, 0.1, 0);
	glVertex2f(1.75, 0.25);
	glColor3f(0.5, 0.1, 0);
	glVertex2f(1.7, -0.5);
	glColor3f(0.5, 0.1, 0);
	glVertex2f(1.2, -1.0);
	glEnd();
}
void drawFish() {
	glPushMatrix();
	glTranslatef(0.9, 0.05, 0);
	glScalef(0.7, 0.7, 1);

	tail();
	glPopMatrix();
	glColor3f(0.8, 0.4, 0.5);
	glPushMatrix();
	glScalef(0.9, 0.5, 1);
	circle(0.0, 0.1, 1.2);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-0.5, 0.19, 0);
	glScalef(0.8, 0.8, 1);
	eye(0.1);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-1.09, -0.2, 0);
	glScalef(0.4, 0.4, 1);
	glColor3f(0., 0, 0);
	traingle();
	glPopMatrix();
}
void fish() {
	glPushMatrix();
	glTranslatef(-19.0, 0.0, 0);
	glScalef(0.23, 0.2, 0.0);
	glRotated(180, 0.0, 1.0, 0.0);
	drawFish();
	glPopMatrix();
}
//////////////////////

////////////////
//    Pipe    //
////////////////
void rectangle()
{
	glBegin(GL_QUADS);
	glVertex2f(-0.5, 0.2);
	glVertex2f(0.5, 0.2);
	glVertex2f(0.5, -0.2);
	glVertex2f(-0.5, -0.2);
	glEnd();
}
void drawPipe()
{
	glPushMatrix();
	glColor3f(0.5, 0.0, 0.0);
	rectangle();
	glPushMatrix();
	glScalef(1.0, 0.63, 1.0);
	glTranslatef(-0.3, 0.0, 0.0);
	glRotatef(90, 0, 0, 1);
	rectangle();
	glPopMatrix();
}
void leftPipe() {
	glPushMatrix();
	glTranslatef(0.95, 0.0, 0);
	glScalef(0.1, 0.4, 0.0);
	drawPipe();
	glPopMatrix();
}
void rightPipe() {
	glPushMatrix();
	glTranslatef(-19.0, 0.0, 0);
	glRotated(180, 0.0, 1.0, 0.0);
	drawPipe();
	glPopMatrix();
}
void pipe() {
	leftPipe();
	rightPipe();
}
//////////////////////

////////////////
//   Trawl    //
////////////////
void drawTrawl()
{
	glLineWidth(5);
	glBegin(GL_LINES);
    glColor3f(0.4, 0.2, 0.0);
	glVertex2f(-0.4, 1);
	glVertex2f(-0.5, 0.85);
	glVertex2f(-0.4, 1);
	glVertex2f(-0.3, 0.85);
	glVertex2f(-0.2, 1);
	glVertex2f(-0.3, 0.85);
	glVertex2f(-0.2, 1);
	glVertex2f(-0.1, 0.85);
	glVertex2f(-0.4, 0.7);
	glVertex2f(-0.5, 0.85);
	glVertex2f(-0.4, 0.7);
	glVertex2f(-0.3, 0.85);
	glVertex2f(-0.2, 0.7);
	glVertex2f(-0.3, 0.85);
	glVertex2f(-0.2, 0.7);
	glVertex2f(-0.1, 0.85);
	glVertex2f(-0.2, 0.7);
	glVertex2f(-0.3, 0.55);
	glVertex2f(-0.4, 0.7);
	glVertex2f(-0.3, 0.55);
	glVertex2f(-0.4, 0.7);
	glVertex2f(-0.5, 0.55);
	glVertex2f(-0.2, 0.7);
	glVertex2f(-0.1, 0.55);
	glVertex2f(-0.1, 0.55);
	glVertex2f(-0.2, 0.4);
	glVertex2f(-0.2, 0.4);
	glVertex2f(-0.3, 0.55);
	glVertex2f(-0.3, 0.55);
	glVertex2f(-0.4, 0.4);
	glVertex2f(-0.4, 0.4);
	glVertex2f(-0.5, 0.55);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(-0.51, 1.01);
	glVertex2f(-0.09, 1.01);
	glVertex2f(-0.51, 1.01);
	glVertex2f(-0.51, 0.39);
	glVertex2f(-0.09, 0.39);
	glVertex2f(-0.51, 0.39);
	glVertex2f(-0.09, 0.39);
	glVertex2f(-0.09, 1.01);
	glEnd();
}
void trawl(void)
{
	////////////////////////
	//  First trawl | Up  //
	glPushMatrix();
	glScaled(0.35, 1.5, 1);
	glTranslated(-1.2, -0.2, 0);
	drawTrawl();
	if ((fishPosX >= 14.4601 && fishPosX < 16.5602) && (fishPosY >= 0.8 && fishPosY < 2.3))
	{
		MessageBox(NULL, L"[ Looks like you're someone's dinner tonight ]", L"!!!GameOver!!!", NULL);
		exit(0);
	}
	glPopMatrix();
	// First trawl | Down //
	glPushMatrix();
	glScaled(0.35, 1.5, 1);
	glTranslated(-1.2, -1.07, 0);
	drawTrawl();
	if ((fishPosX >= 14.4601 && fishPosX < 16.5602) && (fishPosY >= -2.5 && fishPosY < -0.3))
	{
		MessageBox(NULL, L"[ Looks like you're someone's dinner tonight ]", L"!!!GameOver!!!", NULL);
		exit(0);
	}
	glPopMatrix();
	///////////////////////

	///////////////////////
	//   2nd trawl | Up  //
	glPushMatrix();
	glScaled(0.35, 1.5, 1);
	glTranslated(-0.4, -0.34, 0);
	drawTrawl(); 
	if ((fishPosX >= 11.0001 && fishPosX < 14.0001) && (fishPosY >= 0.4 && fishPosY < 2.3)) 
	{
		MessageBox(NULL, L"[ Looks like you're someone's dinner tonight ]", L"!!!GameOver!!!", NULL);
		exit(0);
	}
	glPopMatrix();
	//  2nd trawl | Down //
	glPushMatrix();
	glScaled(0.35, 1.9, 1);
	glTranslated(-0.4, -1.2, 0);
	drawTrawl();
	if ((fishPosX >= 11.0001 && fishPosX < 14.0001) && (fishPosY >= -2.3 && fishPosY < -1)) 
	{
		MessageBox(NULL, L"[ Looks like you're someone's dinner tonight ]", L"!!!GameOver!!!", NULL);
		exit(0);
	}
	glPopMatrix();
	///////////////////////
	
	///////////////////////
	//   3rd trawl | Up  //
	glPushMatrix();
	glScaled(0.35, 1.5, 1);
	glTranslated(0.4, -0.25, 0);
	drawTrawl();
	glPopMatrix();
	if ((fishPosX >= 8.88001 && fishPosX < 11.0001) && (fishPosY >= 0.7 && fishPosY < 2.3)) 
	{
		MessageBox(NULL, L"[ Looks like you're someone's dinner tonight ]", L"!!!GameOver!!!", NULL);
		exit(0);
	}
	// 3rd trawl | Down  //
	glPushMatrix();
	glScaled(0.35, 1.9, 1);
	glTranslated(0.4, -1.07, 0);
	drawTrawl();
	if ((fishPosX >= 8.88001 && fishPosX < 11.0001) && (fishPosY >= -2.5 && fishPosY < -0.3))
	{
		MessageBox(NULL, L"[ Looks like you're someone's dinner tonight ]", L"!!!GameOver!!!", NULL);
		exit(0);
	}
	glPopMatrix();
	///////////////////////
	
	///////////////////////
	//   4th trawl | Up  //
	glPushMatrix();
	glScaled(0.35, 1.5, 1);
	glTranslated(1.2, -0.34, 0);
	drawTrawl();
	if ((fishPosX >= 6.18 && fishPosX < 8.32) && (fishPosY >= 0.4 && fishPosY < 2.3))
	{
		MessageBox(NULL, L"[ Looks like you're someone's dinner tonight ]", L"!!!GameOver!!!", NULL);
		exit(0);
	}
	glPopMatrix();
	//  4th trawl | Down  //
	glPushMatrix();
	glScaled(0.35, 1.9, 1);
	glTranslated(1.2, -1.2, 0);
	drawTrawl();
	if ((fishPosX >= 6.18 && fishPosX < 8.32) && (fishPosY >= -2.3 && fishPosY < -1)) 
	{
		MessageBox(NULL, L"[ Looks like you're someone's dinner tonight ]", L"!!!GameOver!!!", NULL);
		exit(0);
	}
	glPopMatrix();
	///////////////////////
	
	///////////////////////
	//  Last trawl | Up  //
	glPushMatrix();
	glScaled(0.35, 1.5, 1);
	glTranslated(2.0, -0.25, 0);
	drawTrawl();
	if ((fishPosX >= 4.36 && fishPosX < 5.66) && (fishPosY >= 0.8 && fishPosY < 2.3)) 
	{
		MessageBox(NULL, L"[ Looks like you're someone's dinner tonight ]", L"!!!GameOver!!!", NULL);
		exit(0);
	}	
	glPopMatrix();
	//  Last trawl | Down //
	glPushMatrix();
	glScaled(0.35, 1.9, 1);
	glTranslated(2.0, -1.07, 0);
	drawTrawl();
	if ((fishPosX >= 4.36 && fishPosX < 5.66) && (fishPosY >= -2.5 && fishPosY < -0.3))
	{
		MessageBox(NULL, L"[ Looks like you're someone's dinner tonight ]", L"!!!GameOver!!!", NULL);
		exit(0);
	}
	glPopMatrix();
	///////////////////////
}
//////////////////////

///////////////////
//    Camera     //
///////////////////
void reshapeFunc(GLint new_width, GLint new_hight)
{
	/* Save the new width and height */
	win_width = 1500;
	win_hight = 500;
}
//////////////////////

///////////////////
//    Movement   //
///////////////////
void upNdown(int obj) //represent each face with a number as type
{
	if (obj == 1) 
	{
		glPushMatrix();
		glTranslatef(fishPosX, fishPosY, 0.0); 	/// auto movement
		fish();
		glPopMatrix();
	}
}
void TimerFunc(int n) //works like an infinte loop
{
	fishPosX += 0.02;
	if (camX <= -0.5 && camX > -998.907)
	{
	camX -= 1.2;
    }  
	glutTimerFunc(5, TimerFunc, n++); //glutTimerFunc: a timer callback per milliseconds (1)
	glutPostRedisplay(); //redisplay current window
}
static void keyboard(unsigned char key, int x, int y)
{
	switch (key) 
	{
	case 32: //space
		obj = 1;
		glutTimerFunc(100, TimerFunc, times);
		break;
	case 'W': case 'w':
		fishPosY += 0.1;
		break;
	case 'S': case 's':
		fishPosY -= 0.1;
		break;
	case 'Q': case 'q': case 27: //esc
		exit(0);
		break;
	}
	glutPostRedisplay();
}
//////////////////////

///////////////////
//  Print Text   //
///////////////////
void print(int x, int y, int z,const char* string)
{
	//set the position of the text in the window using the x, y and z coordinates
	glRasterPos3f(x, y, z);
	//get the length of the string to display
	int len = (int)strlen(string);
	//loop to display character by character
	for (int i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, string[i]);
	}
}
void textPrint() 
{
	glColor3f(1, 1, 1);
	glScalef(1, 0.2, 1);
	glTranslatef(-0.3, -3, 0.0);
	glPushMatrix();
	print(-19, 8, 0, "To start the game press (Space bar)");
	glPopMatrix();
	glPushMatrix();	
	print(-19, 7, 0, "For movement: W for Up, S for Down");
	glPopMatrix();
	glPushMatrix();
	print(-19, 6, 0, "Press Esc or Q to  Quit");
	glPopMatrix();

}
//////////////////////

////////////////
//  Display   //
////////////////
void display(void)
{
	/*  Clear the image */
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	/* Reset the viewport... */
	glViewport(camX, 0, win_width, win_hight);
	glPushMatrix();
	background();
	glPopMatrix();
	trawl();
	glPopMatrix();
	pipe();
	glPopMatrix();
	upNdown(obj);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glPopMatrix();
	trawl();
	glPopMatrix();
	pipe();
	glDisable(GL_DEPTH_TEST);
	glPopMatrix();
	textPrint();
	glPopMatrix();
	/* Winning case */
	if ((fishPosX >= 19 && fishPosX < 20.0603) && (fishPosY >= -0.2 && fishPosY < 0.2))
	{
		MessageBox(NULL, L"[ Welcome home, Nemo ]", L"!!!Congrats!!!", NULL);
		exit(0);
	}
	/////////////////
	glFlush();
	glutSwapBuffers();
}
//////////////////////

////////////////
//    Main    //
////////////////
int main(int argc, char** argv)
{
	glutInit(&argc, argv);   // Initialize GLUT
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);  //Specify the display Mode (RGBA)    
	glutInitWindowSize(win_width, win_hight);       // Set the window's initial width & height
	glutInitWindowPosition(100, 100); 	// Position the window's initial top-left corner
	glutCreateWindow("Nemo: The lost fish");     // Create a window with the given title
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshapeFunc);
	//////// Audio ////////
	PlaySound(TEXT("Sound.wav"), NULL, SND_FILENAME | SND_ASYNC);
	//////////////////////
	glutPostRedisplay();
	glutMainLoop();  // Enter the infinitely event-processing loop
	return 0;
}
