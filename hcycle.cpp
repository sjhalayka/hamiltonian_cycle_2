/*

* C++ Program to Find Hamiltonian Cycle

*/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <algorithm>
using namespace std;

#include "hcycle.h"

#include "string_utilities.h"
using namespace string_utilities;


/*
* check if the vertex v can be added at index 'pos' in the Hamiltonian Cycle
*/

bool isSafe(int v, vector<vector<bool> > graph, size_t path[], int pos)
{
	if (graph[path[pos - 1]][v] == 0)
		return false;

	for (int i = 0; i < pos; i++)
		if (path[i] == v)
			return false;

	return true;
}



/* solve hamiltonian cycle problem */
bool hamCycleUtil(vector<vector<bool> > graph, size_t path[], int pos)
{
	cout << "hamCycleUtil begin" << endl;

	const size_t V = graph[0].size();

	if (pos == V)
	{
		if (graph[path[pos - 1]][path[0]] == 1)
			return true;
		else
			return false;
	}

	for (int v = 1; v < V; v++)
	{
		if (isSafe(v, graph, path, pos))
		{
			path[pos] = v;

			if (hamCycleUtil(graph, path, pos + 1) == true)
				return true;

			path[pos] = -1;
		}
	}

	cout << "hamCycleUtil end" << endl;

	return false;
}

void printSolution(int path[], const size_t V)
{
	cout << "Solution Exists:";
	cout << " Following is one Hamiltonian Cycle \n" << endl;

	for (int i = 0; i < V; i++)
		cout << path[i] << "  ";

	cout << path[0] << endl;
}


/* solves the Hamiltonian Cycle problem using Backtracking.*/
bool hamCycle(vector<vector<bool> > graph, vector<size_t> &path)
{
	const size_t V = graph[0].size();

	path.resize(V);

	for (size_t i = 0; i < V; i++)
		path[i] = -1;

	path[0] = 0;

	if (hamCycleUtil(graph, &path[0], 1) == false)
	{
		cout << "\nSolution does not exist" << endl;

		return false;
	}

//	printSolution(&path[0], V);

	return true;
}






int main(int argc, char **argv)
{
	srand(static_cast<unsigned int>(time(0)));

	ifstream vert_file("vertices.txt");

	string line;

	// Skip first two lines
	getline(vert_file, line);
	getline(vert_file, line);
	
	while (getline(vert_file, line))
	{
		if ("" == line)
			continue;

		vector<string> tokens = stl_str_tok(" ", line);

		if (tokens.size() != 3)
			continue;

		vertex_3 v;

		istringstream iss;
		iss.str(tokens[0]);
		iss >> v.x;

		iss.clear();
		iss.str(tokens[1]);
		iss >> v.y;

		iss.clear();
		iss.str(tokens[2]);
		iss >> v.z;

		vertices.push_back(v);
	}

	cout << "vertex count " << vertices.size() << endl;



	cout << "allocating graph" << endl;

	vector<bool> g(vertices.size(), false);
	vector< vector<bool> > graph(vertices.size(), g);

	cout << "done allocating graph" << endl;



	ifstream tri_file("triangles.txt");

	// Skip first line
	getline(tri_file, line);

	while (getline(tri_file, line))
	{
		if ("" == line)
			continue;

		vector<string> tokens = stl_str_tok(" ", line);

		if (tokens.size() != 3)
			continue;

		istringstream iss;

		size_t tri0_index = 0;
		size_t tri1_index = 0;
		size_t tri2_index = 0;

		iss.str(tokens[0]);
		iss >> tri0_index;

		iss.clear();
		iss.str(tokens[1]);
		iss >> tri1_index;

		iss.clear();
		iss.str(tokens[2]);
		iss >> tri2_index;

		graph[tri0_index][tri1_index] = true;
		graph[tri1_index][tri2_index] = true;
		graph[tri2_index][tri0_index] = true;

		triangle tri;

		tri.vertex[0] = vertices[tri0_index];
		tri.vertex[1] = vertices[tri1_index];
		tri.vertex[2] = vertices[tri2_index];

		tris.push_back(tri);
	}

	cout << "triangle count " << tris.size() << endl;



	hamCycle(graph, final_path);



	glutInit(&argc, argv);
	init_opengl(win_x, win_y);
	glutReshapeFunc(reshape_func);
	glutIdleFunc(idle_func);
	glutDisplayFunc(display_func);
	glutKeyboardFunc(keyboard_func);
	glutMouseFunc(mouse_func);
	glutMotionFunc(motion_func);
	glutPassiveMotionFunc(passive_motion_func);
	//glutIgnoreKeyRepeat(1);
	glutMainLoop();
	glutDestroyWindow(win_id);

	return 0;
}



void idle_func(void)
{
	glutPostRedisplay();
}

void init_opengl(const int &width, const int &height)
{
	win_x = width;
	win_y = height;

	if (win_x < 1)
		win_x = 1;

	if (win_y < 1)
		win_y = 1;

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(win_x, win_y);
	win_id = glutCreateWindow("Binary Stereo Lithography file viewer");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glClearColor(background_colour.x, background_colour.y, background_colour.z, 1);
	glClearDepth(1.0f);

	main_camera.Set(0, 0, camera_w, camera_fov, win_x, win_y, camera_near, camera_far);
}

void reshape_func(int width, int height)
{
	win_x = width;
	win_y = height;

	if (win_x < 1)
		win_x = 1;

	if (win_y < 1)
		win_y = 1;

	glutSetWindow(win_id);
	glutReshapeWindow(win_x, win_y);
	glViewport(0, 0, win_x, win_y);

	main_camera.Set(main_camera.u, main_camera.v, main_camera.w, main_camera.fov, win_x, win_y, camera_near, camera_far);
}

// Text drawing code originally from "GLUT Tutorial -- Bitmap Fonts and Orthogonal Projections" by A R Fernandes
void render_string(int x, const int y, void *font, const string &text)
{
	for (size_t i = 0; i < text.length(); i++)
	{
		glRasterPos2i(x, y);
		glutBitmapCharacter(font, text[i]);
		x += glutBitmapWidth(font, text[i]) + 1;
	}
}
// End text drawing code.

void draw_objects(void)
{
	glEnable(GL_LIGHT1);

	glPushMatrix();

	glTranslatef(camera_x_transform, camera_y_transform, 0);


	glColor3f(1.0f, 0.5f, 0.0f);

	// Could probably stand to use a VBO here.
	for (size_t i = 0; i < vertices.size(); i++)
	{
		glPushMatrix();
		glTranslatef(vertices[i].x, vertices[i].y, vertices[i].z);
		glutSolidSphere(0.05, 4, 4);
		glPopMatrix();
	}

	glLineWidth(3.0f);
	glDisable(GL_LIGHTING);

	glBegin(GL_LINE_LOOP);

	glColor3f(1.0f, 0.5f, 0.0f);

	for (size_t i = 0; i < final_path.size(); i++)
		glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);

	glEnd();


	glLineWidth(1.0f);

	glBegin(GL_LINES);
	
	glColor3f(0.0f, 0.0f, 0.0f);

	for (size_t i = 0; i < tris.size(); i++)
	{
		glVertex3f(tris[i].vertex[0].x, tris[i].vertex[0].y, tris[i].vertex[0].z);
		glVertex3f(tris[i].vertex[1].x, tris[i].vertex[1].y, tris[i].vertex[1].z);

		glVertex3f(tris[i].vertex[1].x, tris[i].vertex[1].y, tris[i].vertex[1].z);
		glVertex3f(tris[i].vertex[2].x, tris[i].vertex[2].y, tris[i].vertex[2].z);

		glVertex3f(tris[i].vertex[2].x, tris[i].vertex[2].y, tris[i].vertex[2].z);
		glVertex3f(tris[i].vertex[0].x, tris[i].vertex[0].y, tris[i].vertex[0].z);

	}

	glEnd();

	glBegin(GL_TRIANGLES);

	glColor3f(1.0f, 1.0f, 1.0f);

	for (size_t i = 0; i < tris.size(); i++)
	{
		glVertex3f(tris[i].vertex[2].x, tris[i].vertex[2].y, tris[i].vertex[2].z);
		glVertex3f(tris[i].vertex[1].x, tris[i].vertex[1].y, tris[i].vertex[1].z);
		glVertex3f(tris[i].vertex[0].x, tris[i].vertex[0].y, tris[i].vertex[0].z);
	}

	//glEnd();

	// If we do draw the axis at all, make sure not to draw its outline.
	if (true == draw_axis)
	{
		glLineWidth(1.0f);

		glBegin(GL_LINES);

		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(1, 0, 0);
		glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 1, 0);
		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 1);

		glColor3f(0.5, 0.5, 0.5);
		glVertex3f(0, 0, 0);
		glVertex3f(-1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, -1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, -1);

		glEnd();
	}

	glPopMatrix();
}




void display_func(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw the model's components using OpenGL/GLUT primitives.
	draw_objects();

	if (true == draw_control_list)
	{
		// Text drawing code originally from "GLUT Tutorial -- Bitmap Fonts and Orthogonal Projections" by A R Fernandes
		// http://www.lighthouse3d.com/opengl/glut/index.php?bmpfontortho
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, win_x, 0, win_y);
		glScalef(1, -1, 1); // Neat. :)
		glTranslatef(0, -win_y, 0); // Neat. :)
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glColor3f(control_list_colour.x, control_list_colour.y, control_list_colour.z);

		size_t break_size = 22;
		size_t start = 20;
		ostringstream oss;

		render_string(10, start, GLUT_BITMAP_HELVETICA_18, string("Mouse controls:"));
		render_string(10, start + 1 * break_size, GLUT_BITMAP_HELVETICA_18, string("  LMB + drag: Rotate camera"));
		render_string(10, start + 2 * break_size, GLUT_BITMAP_HELVETICA_18, string("  RMB + drag: Zoom camera"));

		render_string(10, start + 4 * break_size, GLUT_BITMAP_HELVETICA_18, string("Keyboard controls:"));
		render_string(10, start + 5 * break_size, GLUT_BITMAP_HELVETICA_18, string("  q: Draw mesh"));
		render_string(10, start + 6 * break_size, GLUT_BITMAP_HELVETICA_18, string("  w: Draw axis"));
		render_string(10, start + 7 * break_size, GLUT_BITMAP_HELVETICA_18, string("  e: Draw text"));

		oss.clear();
		oss.str("");
		oss << "  s: Smooth shading -- ";

		if (true == smooth_shading)
			oss << "ON";
		else
			oss << "OFF";

		render_string(10, start + 8 * break_size, GLUT_BITMAP_HELVETICA_18, oss.str());

		render_string(10, start + 9 * break_size, GLUT_BITMAP_HELVETICA_18, string("  u: Rotate camera +u"));
		render_string(10, start + 10 * break_size, GLUT_BITMAP_HELVETICA_18, string("  i: Rotate camera -u"));
		render_string(10, start + 11 * break_size, GLUT_BITMAP_HELVETICA_18, string("  o: Rotate camera +v"));
		render_string(10, start + 12 * break_size, GLUT_BITMAP_HELVETICA_18, string("  p: Rotate camera -v"));



		vertex_3 eye = main_camera.eye;
		vertex_3 eye_norm = eye;
		eye_norm.normalize();

		oss.clear();
		oss.str("");
		oss << "Camera position: " << eye.x << ' ' << eye.y << ' ' << eye.z;
		render_string(10, win_y - 2 * break_size, GLUT_BITMAP_HELVETICA_18, oss.str());

		oss.clear();
		oss.str("");
		oss << "Camera position (normalized): " << eye_norm.x << ' ' << eye_norm.y << ' ' << eye_norm.z;
		render_string(10, win_y - break_size, GLUT_BITMAP_HELVETICA_18, oss.str());

		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		// End text drawing code.
	}

	glFlush();
	glutSwapBuffers();
}

void keyboard_func(unsigned char key, int x, int y)
{
	switch (tolower(key))
	{
	case 'q':
	{
		draw_mesh = !draw_mesh;
		break;
	}
	case 'w':
	{
		draw_axis = !draw_axis;
		break;
	}
	case 'e':
	{
		draw_control_list = !draw_control_list;
		break;
	}
	case 's':
	{
		smooth_shading = !smooth_shading;
		break;
	}
	case 'u':
	{
		main_camera.u -= u_spacer;
		main_camera.Set();
		break;
	}
	case 'i':
	{
		main_camera.u += u_spacer;
		main_camera.Set();
		break;
	}
	case 'o':
	{
		main_camera.v -= v_spacer;
		main_camera.Set();
		break;
	}
	case 'p':
	{
		main_camera.v += v_spacer;
		main_camera.Set();
		break;
	}

	default:
		break;
	}
}

void mouse_func(int button, int state, int x, int y)
{
	if (GLUT_LEFT_BUTTON == button)
	{
		if (GLUT_DOWN == state)
			lmb_down = true;
		else
			lmb_down = false;
	}
	else if (GLUT_MIDDLE_BUTTON == button)
	{
		if (GLUT_DOWN == state)
			mmb_down = true;
		else
			mmb_down = false;
	}
	else if (GLUT_RIGHT_BUTTON == button)
	{
		if (GLUT_DOWN == state)
			rmb_down = true;
		else
			rmb_down = false;
	}
}

void motion_func(int x, int y)
{
	int prev_mouse_x = mouse_x;
	int prev_mouse_y = mouse_y;

	mouse_x = x;
	mouse_y = y;

	int mouse_delta_x = mouse_x - prev_mouse_x;
	int mouse_delta_y = prev_mouse_y - mouse_y;

	if (true == lmb_down && (0 != mouse_delta_x || 0 != mouse_delta_y))
	{
		main_camera.u -= static_cast<float>(mouse_delta_y)*u_spacer;
		main_camera.v += static_cast<float>(mouse_delta_x)*v_spacer;
	}
	else if (true == rmb_down && (0 != mouse_delta_y))
	{
		main_camera.w -= static_cast<float>(mouse_delta_y)*w_spacer;

		if (main_camera.w < 1.1f)
			main_camera.w = 1.1f;
		else if (main_camera.w > 20.0f)
			main_camera.w = 20.0f;
	}

	main_camera.Set(); // Calculate new camera vectors.
}

void passive_motion_func(int x, int y)
{
	mouse_x = x;
	mouse_y = y;
}


