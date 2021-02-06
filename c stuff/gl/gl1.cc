#include "GL/freeglut.h"
#include "GL/gl.h"


void draw_triangle()
{
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0,1.0,1.0);
	glOrtho(-1.0,1.0,-1.0,1.0,-1.0,1.0);

	glBegin(GL_POLYGON);
	glVertex3f(-1,0,0);
	glVertex3f(0,1,0);
	glVertex3f(1,0,0);
	glVertex3f(0,-1,0);
	glEnd();
	glFlush();
}

int main(int argc,char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE);
	glutInitWindowSize(500,500);
	glutInitWindowPosition(100,100);
	glutCreateWindow("III-OpenGL");
	glutDisplayFunc(draw_triangle);
	glutMainLoop();
	return 0;
}
