// assign1.cpp : Defines the entry point for the console application.
//
   /* Avinash Akka */
/*
  CSCI 420 Computer Graphics
  Assignment 1: Height Fields
  C++ starter code
*/

#include "stdafx.h"
#include <pic.h>
#include <windows.h>
#include <stdlib.h>
#include <GL/glu.h>
#include <GL/glut.h>

int g_iMenuId;

int g_vMousePos[2] = {0, 0};
int g_iLeftMouseButton = 0;    /* 1 if pressed, 0 if not */
int g_iMiddleMouseButton = 0;
int g_iRightMouseButton = 0;

typedef enum { ROTATE, TRANSLATE, SCALE } CONTROLSTATE;

CONTROLSTATE g_ControlState = ROTATE;

/* state of the world */
float g_vLandRotate[3] = {0.0, 0.0, 0.0};
float g_vLandTranslate[3] = {0.0, 0.0, 0.0};
float g_vLandScale[3] = {1.0, 1.0, 1.0};

/* see <your pic directory>/pic.h for type Pic */
Pic * g_pHeightData;

const int W = 640;		/* initial window width */
const int H = 480;		/* initial window height */

bool stop_counter = true;
int a,Count;
GLfloat rot, color3=1.0;

/* Write a screenshot to the specified filename */
void saveScreenshot (char *filename)
{
  int i;
  Pic *in = NULL;

  if (filename == NULL)
    return;

  /* Allocate a picture buffer */
  in = pic_alloc(640, 480, 3, NULL);

  printf("File to save to: %s\n", filename);

  for (i=479; i>=0; i--) {
    glReadPixels(0, 479-i, 640, 1, GL_RGB, GL_UNSIGNED_BYTE,
                 &in->pix[i*in->nx*in->bpp]);
  }

  if (jpeg_write(filename, in))
    printf("File saved Successfully\n");
  else
    printf("Error in Saving\n");

  pic_free(in);
}


void myinit()
{
  /* setup gl view here */
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glEnable(GL_DEPTH_TEST);
  
}


/* Reference: Page 242- Interactive Computer Graphics-A TopDown Approach by Edward Angel 6th Edition */
/* The core idea behind my height field implementation is: Use the color channel data of each pixel to get the height value, 
   Since the image is greyscale all RGB values for each pxel will be equal so can use any of R,G,B to extract color channel. 
   Then for every pixel along the Width and Height of image will draw the New Height Enabled image using 2 For loops(width = 640, height = 480) 
   Color data is not in 0-1 format so convert by dividing 255(i.e 0-255) */
   
void drawHeightFieldImage()
{
  int i, j;
  GLfloat x, y, z, color;
  int width = g_pHeightData->nx;  /* number of pixels along X */
  int height = g_pHeightData->ny; /* number of pixels along Y */
  
  /* Since we have Width and Height data, we will iterate using 2 For loops along the X and Y axis of the image and read the color channel and x,y coordinate data
      And then draw along the same axes*/
    /* We are using the switch to choose either Points, Lines, or Triangles to render the image */
  switch(a)
  {
     case 1:
    {
     for (i = 0; i < width-1; i++)
        {
    x = (GLfloat) i;
    glBegin(GL_POINTS);/* Render using Points*/
    for (j = 0; j < height-1; j++) 
          {
      y = (GLfloat) j;
      z = (GLfloat) PIC_PIXEL(g_pHeightData, i, j, 0); 
      color = (z/255.0);               
	  glColor3f(color, color, color3);
      glVertex3f(x/(W), y/(H), z/255.0); 
      z = (GLfloat) PIC_PIXEL(g_pHeightData, i+1, j, 0);
	  color = (z/255.0);
      glColor3f(color, color, color3);
      glVertex3f((x+1.0)/(W), y/(H), z/255.0);
   	      }
    glEnd();
        }
   break;
	}

	case 2:
    {
     for (i = 0; i < width-1; i++)
        {
    x = (GLfloat) i;
    glBegin(GL_LINES);/* Render using Lines*/
    for (j = 0; j < height-1; j++) 
          {
      y = (GLfloat) j;
      z = (GLfloat) PIC_PIXEL(g_pHeightData, i, j, 0); 
      color = (z/255.0);               
	  glColor3f(color, color, color3);
      glVertex3f(x/(W), y/(H), z/255.0); 
      z = (GLfloat) PIC_PIXEL(g_pHeightData, i+1, j, 0);
	  color = (z/255.0);
      glColor3f(color, color, color3);
      glVertex3f((x+1.0)/(W), y/(H), z/255.0);
   
	      }
    glEnd();
        }
   break;
	}

	/* By default we will render using Triangle Strips*/
	  default :
		case 3:
{
  /* for each pixel along x,y read the height data i.e the color channel using PIC_PIXEL,convert it to coordinate and render using Triangle Strips*/
  for (i = 0; i < width-1; i++)/* for each pixel along the width*/
      {
          x = (GLfloat) i;/* Typecast to float*/ /* Save vertex X*/

    glBegin(GL_TRIANGLE_STRIP); /* Traingle Fan doesnot render the way Strips does as depth mapping is rendered good with strips*/
    
	    for (j = 0; j < height-1; j++) /* for each pixel along height */
       {
          y = (GLfloat) j; /* Savevertex Y */
          z = (GLfloat) PIC_PIXEL(g_pHeightData, i, j, 0); /* get the height by reading the color value from the saved picture data,
			    	 									     Since it is a grey scale image we can use any RGB value as they have the same*/
      color = (z/255.0);               /* Converting it to value between 0 & 1
									      We will use the same colors for 2 other values in RGB as the image data has same values for all three,But to render an 
										  3D image that shows the shades of height its best to do that in one of the color keeping to maximum 
										  i.e either of RGB should be 1.0*/ 
										/* Can choose different shades by pressing 1 or 2 or 3*/
	  glColor3f(color, color, color3);     /* by Default we will obtain image in Blue shades as per the output shown in the assigmment submission page*/
      glVertex3f(x/W, y/H, z/255.0); /* Since we iterating using 2 loops we will input 2 vertices at a time */
											  /*  starts from (x,0,0)  */
	  
	  z = (GLfloat) PIC_PIXEL(g_pHeightData, i+1, j, 0);
	  
	  color = (z/255.0);
      glColor3f(color, color, color3);
      glVertex3f((x+1.0)/W, y/H, z/255.0);/*  begins at (x+1,0,0) 
													   And draws for the whole height of 480 pixels
													   So the image is drawn by 2 pixels (x,y,z) and (x+1,y,z) along the height and then the 
													   x vertices are incremented to x+2,x+3 and so on and so forth*/

   
	}
    glEnd();
  }
   break;
		}
		   
  }	   
  }

/* Reference Interactive Computer Graphics-A TopDown Approach by Edward Angel 6th Edition*/ 
   
void display()
{
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity(); /* Load the Matrix for all the Transformations*/
  
  gluLookAt(-1.0,-1.0,1.0,1.0,1.0,-0.5,0.0,1.0,1.0);/* Camera placement*/
  
  
  glTranslatef(g_vLandTranslate[0],g_vLandTranslate[1],g_vLandTranslate[2]);

  glRotatef(g_vLandRotate[0], 1.0, 0.0, 0.0);
  glRotatef(g_vLandRotate[1], 0.0, 1.0, 0.0);
  glRotatef(g_vLandRotate[2], 0.0, 0.0, 1.0);

  glRotatef(rot*0.5, 0.0, 0.1, 0.5);
  
  glScalef(g_vLandScale[0], g_vLandScale[1], g_vLandScale[2]);
 
   
  drawHeightFieldImage();		
  
  glFlush();
  glutSwapBuffers();
}

void menufunc(int value)
{
  switch (value)
  {
    case 0:
      exit(0);
      break;
  }
}

void doIdle()
{  
  /* do some stuff... */

	rot=0.009*(GLfloat)glutGet(GLUT_ELAPSED_TIME); 

  /* make the screen update */
  glutPostRedisplay();

}

/* converts mouse drags into information about 
rotation/translation/scaling */
void mousedrag(int x, int y)
{
  int vMouseDelta[2] = {x-g_vMousePos[0], y-g_vMousePos[1]};
  
  switch (g_ControlState)
  {
    case TRANSLATE:  
      if (g_iLeftMouseButton)
      {
        g_vLandTranslate[0] += vMouseDelta[0]*0.01;
        g_vLandTranslate[1] -= vMouseDelta[1]*0.01;
      }
      if (g_iMiddleMouseButton)
      {
        g_vLandTranslate[2] += vMouseDelta[1]*0.01;
      }
      break;
    case ROTATE:
      if (g_iLeftMouseButton)
      {
       g_vLandRotate[0] += vMouseDelta[0];
        g_vLandRotate[1] += vMouseDelta[1];
		g_vLandRotate[2] += vMouseDelta[0];
      }
      if (g_iMiddleMouseButton)
      {
        g_vLandRotate[2] += vMouseDelta[0];
      }
      break;
    case SCALE:
      if (g_iLeftMouseButton)
      {
        g_vLandScale[0] *= 1.0+vMouseDelta[0]*0.01;
        g_vLandScale[1] *= 1.0-vMouseDelta[1]*0.01;
      }
      if (g_iMiddleMouseButton)
      {
        g_vLandScale[2] *= 1.0-vMouseDelta[1]*0.01;
      }
      break;
  }
  g_vMousePos[0] = x;
  g_vMousePos[1] = y;
}

void mouseidle(int x, int y)
{
  g_vMousePos[0] = x;
  g_vMousePos[1] = y;
}

void mousebutton(int button, int state, int x, int y)
{

  switch (button)
  {
    case GLUT_LEFT_BUTTON:
      g_iLeftMouseButton = (state==GLUT_DOWN);
      break;
    case GLUT_MIDDLE_BUTTON:
      g_iMiddleMouseButton = (state==GLUT_DOWN);
      break;
    case GLUT_RIGHT_BUTTON:
      g_iRightMouseButton = (state==GLUT_DOWN);
      break;
  }
 
  switch(glutGetModifiers())
  {
    case GLUT_ACTIVE_CTRL:
      g_ControlState = TRANSLATE;
      break;
    case GLUT_ACTIVE_SHIFT:
      g_ControlState = SCALE;
      break;
    default:
      g_ControlState = ROTATE;
      break;
  }

  g_vMousePos[0] = x;
  g_vMousePos[1] = y;
}

/* Timer function use was learnt from an example from http://www.glprogramming.com/ but has been coded to be used for this program*/

void timeFunction(int value)
{
  switch(value)
  {
    case 0: 
      if(!stop_counter) break;
      char filename[40];
      sprintf(filename, "%00d.jpg", Count);
      saveScreenshot(filename);
      Count++;
      if(Count == 301) stop_counter = false;
      break;
    default:
		break;
  }
  glutTimerFunc(100, timeFunction, 0);
}



/* keyboard callback */
/* Extra Credit Implementation: Press q/Q to quit, I/i to increase height, D/d to decrease height */
void keyboard(unsigned char key, int x, int y)
{
  switch (key) 
  {
    case 'q': case 'Q':
               exit(0);
                 break;

	case 'I': case 'i':
		    g_vLandScale[2] = g_vLandScale[2] * 1.01;
			   break;

	case 'D': case 'd':
             g_vLandScale[2] -= 0.01;
			 break;

	case 'p' :
	case 'P' :a=1;
				break;

	case 'l': 
	case 'L':a=2;
				break;
		      break;

    
	case 'T':
	case 't':a=3;
			break;

			
	case '1' :   color3=1.0;
				 break;
	
	case '2' :   color3=0.5;
				 break;
				 
	case '3' :   color3=0.2;
				 break;

  }

  glutPostRedisplay();
}

/* callback function for reshape, will keep the aspect ratio w.r.t the resized window */
void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0,(GLfloat)width/height, 0.1, 1000.0);
    glMatrixMode(GL_MODELVIEW);
}

/* main function */
int main (int argc, char **argv)
{
	// I've set the argv[1] to spiral.jpg.
	// To change it, on the "Solution Explorer",
	// right click "assign1", choose "Properties",
	// go to "Configuration Properties", click "Debugging",
	// then type your texture name for the "Command Arguments"
  if (argc<2)
  {  
    printf ("usage: %s heightfield.jpg\n", argv[0]);
    exit(1);
  }

  g_pHeightData = jpeg_read(argv[1], NULL);
  if (!g_pHeightData)
  {
    printf ("error reading %s.\n", argv[1]);
    exit(1);
  }

        glutInit(&argc,(char **)argv);
    
	/*
		create a window here..should be double buffered and use depth testing
  	    the code past here will segfault if you don't have a window set up....
	    replace the exit once you add those calls.
	*/

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Avinash Akka: Assignment 1: Height Fields");

  /* tells glut to use a particular display function to redraw */
	glutDisplayFunc(display);
  
	/* allow the user to quit using the right mouse button menu */
	//g_iMenuId = glutCreateMenu(menufunc);
	//glutSetMenu(g_iMenuId);
	//glutAddMenuEntry("Quit",0);
	//glutAttachMenu(GLUT_RIGHT_BUTTON);
  
	/* replace with any animate code */
	glutIdleFunc(doIdle);

	/* callback for mouse drags */
	glutMotionFunc(mousedrag);
	/* callback for idle mouse movement */
	glutPassiveMotionFunc(mouseidle);
	/* callback for mouse button changes */
	glutMouseFunc(mousebutton);

    glutReshapeFunc(reshape);	/* callback reshape function */
    glutKeyboardFunc(keyboard);	/* callback for keyboard */

    glutTimerFunc(15, timeFunction, 0);/* Used to save screenshots*/

    /* do initialization */
    myinit();
 
    glutMainLoop();
    return 0;
}