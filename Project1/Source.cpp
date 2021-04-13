#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <iostream>
#include <cmath>
#include <time.h>
#define M_PI 3.14159265358979323846

int model = 1;  // 1- punkty, 2- siatka, 3 - wype�nione tr�jk�ty

static GLfloat viewer[] = { 0.0, 0.0, 10.0 };
typedef float point3[3];
typedef float point2[2];
const int N = 20;
point3 tab[N + 1][N + 1];
point2 tab_param[N + 1][N + 1];  // 0 - u  1 - v
point3 tab_kolory[N + 1][N + 1];
static GLfloat azymut = 0.0;  
static GLfloat pix2anglex;   
static GLfloat pix2angley; 
static GLfloat elewacja = 0;

static GLint status = 0;       

static int x_pos_old = 0;      
static int y_pos_old = 0;

static int delta_x = 0;        // r�nica pomi�dzy pozycj� bie��c�
static int delta_y = 0;        // r�nica pomi�dzy pozycj� bie��c�
float R = 10.0;
/*************************************************************************************/
// Funkcja rysuj�ca osie uk�adu wsp�rz�dnych

GLbyte *LoadTGAImage(const char *FileName, GLint *ImWidth, GLint *ImHeight, GLint *ImComponents, GLenum *ImFormat)
{
	/*************************************************************************************/
	// Struktura dla nag��wka pliku  TGA

#pragma pack(1)            
	typedef struct
	{
		GLbyte    idlength;
		GLbyte    colormaptype;
		GLbyte    datatypecode;
		unsigned short    colormapstart;
		unsigned short    colormaplength;
		unsigned char     colormapdepth;
		unsigned short    x_orgin;
		unsigned short    y_orgin;
		unsigned short    width;
		unsigned short    height;
		GLbyte    bitsperpixel;
		GLbyte    descriptor;
	}TGAHEADER;
#pragma pack(8)


	FILE *pFile;
	TGAHEADER tgaHeader;
	unsigned long lImageSize;
	short sDepth;
	GLbyte    *pbitsperpixel = NULL;

	/*************************************************************************************/
	// Warto�ci domy�lne zwracane w przypadku b��du 

	*ImWidth = 0;
	*ImHeight = 0;
	*ImFormat = GL_BGR_EXT;
	*ImComponents = GL_RGB8;

	pFile = fopen(FileName, "rb");
	if (pFile == NULL)
		return NULL;
	/*************************************************************************************/

	// Przeczytanie nag��wka pliku 

	fread(&tgaHeader, sizeof(TGAHEADER), 1, pFile);

	/*************************************************************************************/
	// Odczytanie szeroko�ci, wysoko�ci i g��bi obrazu 

	*ImWidth = tgaHeader.width;
	*ImHeight = tgaHeader.height;
	sDepth = tgaHeader.bitsperpixel / 8;

	/*************************************************************************************/

	// Sprawdzenie, czy g��bia spe�nia za�o�one warunki (8, 24, lub 32 bity) 

	if (tgaHeader.bitsperpixel != 8 && tgaHeader.bitsperpixel != 24 && tgaHeader.bitsperpixel != 32)
		return NULL;

	/*************************************************************************************/
	// Obliczenie rozmiaru bufora w pami�ci

	lImageSize = tgaHeader.width * tgaHeader.height * sDepth;

	/*************************************************************************************/
	// Alokacja pami�ci dla danych obrazu

	pbitsperpixel = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));

	if (pbitsperpixel == NULL)
		return NULL;

	if (fread(pbitsperpixel, lImageSize, 1, pFile) != 1)
	{
		free(pbitsperpixel);
		return NULL;
	}

	/*************************************************************************************/
	// Ustawienie formatu OpenGL

	switch (sDepth)
	{
	case 3:
		*ImFormat = GL_BGR_EXT;
		*ImComponents = GL_RGB8;
		break;
	case 4:
		*ImFormat = GL_BGRA_EXT;
		*ImComponents = GL_RGBA8;
		break;
	case 1:
		*ImFormat = GL_LUMINANCE;
		*ImComponents = GL_LUMINANCE8;
		break;
	};

	fclose(pFile);

	return pbitsperpixel;
}

void Mouse(int btn, int state, int x, int y)
{


	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old = x;         // przypisanie aktualnie odczytanej pozycji kursora 
		y_pos_old = y;
		status = 1;        
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{

		y_pos_old = y;
		status = 2;
	}
	else

		status = 0;          // nie zosta� wci�ni�ty �aden klawisz 
}

/*************************************************************************************/

void Motion(GLsizei x, GLsizei y)
{

	delta_x = x - x_pos_old;     // obliczenie r�nicy po�o�enia kursora myszy
	delta_y = y - y_pos_old;
	x_pos_old = x;            // podstawienie bie��cego po�o�enia jako poprzednie
	y_pos_old = y;
	glutPostRedisplay();     // przerysowanie obrazu sceny
}

void losujkolor()
{
	for (int i = 0; i < N + 1; i++)
	{
		for (int j = 0; j < N + 1; j++)
		{
			tab_kolory[i][j][0] = ((float)(rand() % 10) + 1) / 10;
			tab_kolory[i][j][1] = ((float)(rand() % 10) + 1) / 10;
			tab_kolory[i][j][2] = ((float)(rand() % 10) + 1) / 10;
		}
	}
}

void Egg()
{
	if (model == 1)
	{
		glBegin(GL_POINTS);
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{

				glVertex3fv(tab[i][j]);
			}
		}
		glEnd();
	}
	else if (model == 2)
	{

		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				//Po x
				glBegin(GL_LINES);
				glVertex3fv(tab[i][j]);
				glVertex3fv(tab[i + 1][j]);
				glEnd();

				//Po y
				glBegin(GL_LINES);
				glVertex3fv(tab[i][j]);
				glVertex3fv(tab[i][j + 1]);
				glEnd();

				//*0
				//0*

				//0*
				//*0
				glBegin(GL_LINES);
				glVertex3fv(tab[i + 1][j]);
				glVertex3fv(tab[i][j + 1]);
				glEnd();
			}
		}
	}
	else if (model == 3)
	{

		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				/*
				**
				*0
				*/
				glBegin(GL_TRIANGLES);
				glColor3fv(tab_kolory[i][j]);
				glVertex3fv(tab[i][j]);
				glColor3fv(tab_kolory[i + 1][j]);
				glVertex3fv(tab[i + 1][j]);
				glColor3fv(tab_kolory[i][j + 1]);
				glVertex3fv(tab[i][j + 1]);
				glEnd();

				/*
				0*
				**
				*/
				glBegin(GL_TRIANGLES);
				glColor3fv(tab_kolory[i][j + 1]);
				glVertex3fv(tab[i][j + 1]);
				glColor3fv(tab_kolory[i + 1][j]);
				glVertex3fv(tab[i + 1][j]);
				glColor3fv(tab_kolory[i + 1][j + 1]);
				glVertex3fv(tab[i + 1][j + 1]);
				glEnd();

			}
		}
	}

}

void nalozSiatke()
{
	for (int x = 0; x < N + 1; x++)
	{
		for (int y = 0; y < N + 1; y++)
		{
			tab_param[x][y][0] = (1 / (float)N)*x;
			tab_param[x][y][1] = (1 / (float)N)*y;
		}
	}
}

void obliczWspolrzedne()
{
	for (int i = 0; i < N + 1; i++)
	{
		for (int j = 0; j < N + 1; j++)
		{
			tab[i][j][0] = (-90 * pow(tab_param[i][j][0], 5) + 225 * pow(tab_param[i][j][0], 4) - 270 * pow(tab_param[i][j][0], 3) + 180 * pow(tab_param[i][j][0], 2) - 45 * tab_param[i][j][0]) * cos(M_PI*tab_param[i][j][1]);

			// odejmujemy 5 by wyrownac jajko z ukladem wspolrzednych
			tab[i][j][1] = 160 * pow(tab_param[i][j][0], 4) - 320 * pow(tab_param[i][j][0], 3) + 160 * pow(tab_param[i][j][0], 2) - 5;
			tab[i][j][2] = (-90 * pow(tab_param[i][j][0], 5) + 225 * pow(tab_param[i][j][0], 4) - 270 * pow(tab_param[i][j][0], 3) + 180 * pow(tab_param[i][j][0], 2) - 45 * tab_param[i][j][0]) * sin(M_PI*tab_param[i][j][1]);
		}
	}
}

void Axes(void)
{
	point3  x_min = { -2.0, 0.0, 0.0 };
	point3  x_max = { 2.0, 0.0, 0.0 };
	// pocz�tek i koniec obrazu osi x

	point3  y_min = { 0.0, -2.0, 0.0 };
	point3  y_max = { 0.0, 2.0, 0.0 };
	// pocz�tek i koniec obrazu osi y

	point3  z_min = { 0.0, 0.0, -2.0 };
	point3  z_max = { 0.0, 0.0, 2.0 };
	//  pocz�tek i koniec obrazu osi y

	glColor3f(1.0f, 0.0f, 0.0f);  // kolor rysowania osi - czerwony
	glBegin(GL_LINES); // rysowanie osi x
	glVertex3fv(x_min);
	glVertex3fv(x_max);
	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f);  // kolor rysowania - zielony
	glBegin(GL_LINES);  // rysowanie osi y
	glVertex3fv(y_min);
	glVertex3fv(y_max);
	glEnd();

	glColor3f(0.0f, 0.0f, 1.0f);  // kolor rysowania - niebieski
	glBegin(GL_LINES); // rysowanie osi z
	glVertex3fv(z_min);
	glVertex3fv(z_max);
	glEnd();
}
/*************************************************************************************/
// Funkcja okre�laj�ca co ma by� rysowane (zawsze wywo�ywana gdy trzeba 
// przerysowa� scen�)

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz�cym

	glLoadIdentity();
	// Czyszczenie macierzy bie��cej
	if(status == 1) {	
		azymut += delta_x*pix2anglex;		
		elewacja += delta_y*pix2angley;		

		if(azymut >= 360.0)
			azymut = 0.0;
		if(elewacja >= 360.0)
			elewacja = 0.0;
	}
	else if(status == 2) {	
		R +=0.1* delta_y;	// zoom
		if(R <= 6.0)	
			R = 6.0; 
		if(R >= 25.0)	
			R = 25.0;
	}

	viewer[0] = R * cos(azymut) * cos(elewacja);
	viewer[1] = R * sin(elewacja);
	viewer[2] = R * sin(azymut) * cos(elewacja);

    gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, cos(elewacja), 0.0);
	Axes();
	Egg();
	glFlush();
	// Przekazanie polece� rysuj�cych do wykonania

	glutSwapBuffers();
	//
}

void keys(unsigned char key, int x, int y)
{
	if (key == 'p') model = 1;
	if (key == 'w') model = 2;
	if (key == 's') model = 3;

	RenderScene(); // przerysowanie obrazu sceny
}
/*************************************************************************************/
// Funkcja ustalaj�ca stan renderowania

void MyInit(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Kolor czyszc�cy (wype�nienia okna) ustawiono na czarny
}
/*************************************************************************************/
// Funkcja ma za zadanie utrzymanie sta�ych proporcji rysowanych 
// w przypadku zmiany rozmiar�w okna.
// Parametry vertical i horizontal (wysoko�� i szeroko�� okna) s� 
// przekazywane do funkcji za ka�dym razem gdy zmieni si� rozmiar okna.

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	pix2anglex = (360.0 / (float)horizontal)/10;
	pix2angley = (360.0 / (float)vertical)/10;
	GLfloat AspectRatio;
	// Deklaracja zmiennej AspectRatio  okre�laj�cej proporcj�
	// wymiar�w okna

	if (vertical == 0)  // Zabezpieczenie przed dzieleniem przez 0
		vertical = 1;

	glViewport(0, 0, horizontal, vertical);
	// Ustawienie wielko�ciokna okna widoku (viewport)
	// W tym przypadku od (0,0) do (horizontal, vertical) 

	glMatrixMode(GL_PROJECTION);
	// Prze��czenie macierzy bie��cej na macierz projekcji

	glLoadIdentity();
	// Czyszcznie macierzy bie��cej           

	gluPerspective(70.0, 1.0, 1.0, 30.0);
	// Ustawienie parametr�w dla rzutu perspektywicznego


	glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);

	glMatrixMode(GL_MODELVIEW);
	// Prze��czenie macierzy bie��cej na macierz widoku modelu                                    
	glLoadIdentity();
	// Czyszcenie macierzy bie��cej
}
/*************************************************************************************/
// G��wny punkt wej�cia programu. Program dzia�a w trybie konsoli

void main(void)
{
	srand((unsigned)time(NULL));
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(800, 600);

	glutCreateWindow("Jajko");

	glutDisplayFunc(RenderScene);
	// Okre�lenie, �e funkcja RenderScene b�dzie funkcj� zwrotn�
	// (callback function).  Bedzie ona wywo�ywana za ka�dym razem 
	// gdy zajdzie potrzba przeryswania okna

	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcj� zwrotn� odpowiedzialn�
	// zazmiany rozmiaru okna   

	glutMouseFunc(Mouse);
	// Ustala funkcj� zwrotn� odpowiedzialn� za badanie stanu myszy

	glutMotionFunc(Motion);
	// Ustala funkcj� zwrotn� odpowiedzialn� za badanie ruchu myszy

	glutKeyboardFunc(keys);


	nalozSiatke();
	obliczWspolrzedne();
	losujkolor();


	MyInit();
	// Funkcja MyInit() (zdeelewacjaniowana powy�ej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przyst�pieniem do renderowania 
	glEnable(GL_DEPTH_TEST);
	// W��czenie mechanizmu usuwania powierzchni niewidocznych

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT
}