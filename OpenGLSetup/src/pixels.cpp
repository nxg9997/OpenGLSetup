#include "pixels.h"

extern int g_winWidth;
extern int g_winHeight;

extern unsigned int g_box_num;
extern Box* g_boxes;

extern unsigned int g_sphere_num;
extern Sphere * g_spheres;

extern Camera g_cam;

extern Light g_light;

float vertices[4 * 2] = { 0,   0,
						  g_winWidth, 0,
						  g_winWidth, g_winHeight,
						  0,   g_winHeight };
float texCoords[4 * 2] = { 0, 0,
						 1, 0,
						 1, 1,
						 0, 1 };

GLuint glTexID = -1;

unsigned char* imagedata;

void createTexture()
{
	imagedata = new unsigned char[g_winWidth * g_winHeight * 3];

	vec3 camInWorld = (vec3)(g_cam.eye * g_cam.mvMat);
	//cout << camInWorld.x << "," << camInWorld.y << "," << camInWorld.z << "," << endl;

	//int ind = 0;
	for (int i = 0; i < g_winHeight; i++) {
		for (int j = 0; j < g_winWidth; j++) {

			// - calculate the ray
			float x = g_cam.nbl.x + ((j * g_cam.nbr.x) / (float)g_winWidth) * 2.0f;
			float y = g_cam.nbl.y + ((i * g_cam.ntl.y) / (float)g_winHeight) * 2.0f;
			vec4 v(x, y, -g_cam.near_plane, 0);
			v = glm::normalize(v);

			// - check for ray collisions with the boxes
			for (int s = 0; s < g_box_num; s++) {

				vec3* s_cols = FindCollisionBox((vec3)g_cam.eye, (vec3)v, g_boxes[s]);

				if (s_cols != nullptr) {

					// - calculate phong color
					vec3 sp = normalize(g_light.pos - *s_cols);
					vec3 n = normalize(*s_cols);
					vec3 vp = normalize(*s_cols - vec3(g_cam.eye));
					vec3 r = (n + n) * dot(n, sp) - sp;
					float Ia = g_boxes[s].ambient;
					float Id = g_light.intensity * g_boxes[s].diffuse * dot(sp,n);
					float Is = g_light.intensity * g_boxes[s].phong * pow(dot(r,vp), 50);
					vec3 color = (Ia + Id) * g_light.color * g_boxes[s].color + Is * g_light.color;

					// - apply color to the image
					imagedata[((i * g_winWidth) + j) * 3 + 0] = color.x * 255;   // R
					imagedata[((i * g_winWidth) + j) * 3 + 1] = color.y * 255;   // G
					imagedata[((i * g_winWidth) + j) * 3 + 2] = color.z * 255;   // B
				}
			}

			// - find collisions with the spheres
			for (int s = 0; s < g_sphere_num; s++) {

				vec3* s_cols = FindCollisionSphere((vec3)g_cam.eye, (vec3)v, g_spheres[s]);

				if (s_cols != nullptr) {

					// - calculate phong color
					vec3 sp = normalize(g_light.pos - *s_cols);
					vec3 n = normalize(*s_cols);
					vec3 vp = normalize(*s_cols - vec3(g_cam.eye));
					vec3 r = (n + n) * dot(n, sp) - sp;
					float Ia = g_spheres[s].ambient;
					float Id = g_light.intensity * g_spheres[s].diffuse * dot(sp, n);
					float Is = g_light.intensity * g_spheres[s].phong * pow(dot(r, vp), 50);
					vec3 color = (Ia + Id) * g_light.color * g_spheres[s].color + Is * g_light.color;

					// - apply color to the image
					imagedata[((i * g_winWidth) + j) * 3 + 0] = color.x * 255;   // R
					imagedata[((i * g_winWidth) + j) * 3 + 1] = color.y * 255;   // G
					imagedata[((i * g_winWidth) + j) * 3 + 2] = color.z * 255;   // B
				}
			}
			//cout << ind << endl;
			//ind++;
		}
	}

	glGenTextures(1, &glTexID);
	glBindTexture(GL_TEXTURE_2D, glTexID);
	cout << "createtex " <<  glTexID << endl;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// send the imagedata (on CPU) to the GPU memory at glTexID (glTexID is a GPU memory location index)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, g_winWidth, g_winHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imagedata);
	//drawPlane();
}

/*void initGLStuff(void)
{
	createTexture();
}*/

void drawPlane()
{
	glDisable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, glTexID); // use the texture on the quad 
	glBegin(GL_QUADS);
	glTexCoord2fv(texCoords + 2 * 0); glVertex2fv(vertices + 2 * 0);
	glTexCoord2fv(texCoords + 2 * 1); glVertex2fv(vertices + 2 * 1);
	glTexCoord2fv(texCoords + 2 * 2); glVertex2fv(vertices + 2 * 2);
	glTexCoord2fv(texCoords + 2 * 3); glVertex2fv(vertices + 2 * 3);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

/*void display(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// if need to update imagedata, do it here, before displaying it as a texture on the quad 
	drawPlane();
	glutSwapBuffers();
}*/

/*void reshape(int w, int h)
{
	g_winWidth = w;
	g_winHeight = h;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 640.0, 0.0, 480.0);

	glViewport(0, 0, (GLsizei)g_winWidth, (GLsizei)g_winHeight);
}*/

/*void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	}
}*/

/*int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize((int)g_winWidth, (int)g_winHeight);

	glutCreateWindow("Draw Pixels");

	initGLStuff();

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}*/