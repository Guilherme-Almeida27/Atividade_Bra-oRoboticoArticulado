// Criado por: Guilherme José Almeida Nunes e Anderson André Ferreira Gomes
// Disciplina: Computação Gráfica
// Descrição: Simulação de um braço robótico 2D utilizando OpenGL e GLUT
// Compilação: g++ -o main.exe main.cpp -I"./include" -L"./lib/x64" -lfreeglut -lopengl32 -lglu32 ou g++ -o main.exe main.cpp -I./include -L./lib/x64 -lfreeglut -lopengl32 -lglu32
// Executar: ./main.exe ou main.exe no Windows

#include <GL/glut.h>
#include <math.h>

// Constantes
#define MAX_ABERTURA_GARRA 45.0f
#define MIN_ABERTURA_GARRA 8.0f
#define PASSO_GARRA 2.0f
#define PASSO_ANGULO 5.0f
#define PASSO_BASE 10.0f
#define RAIO_JUNTA 10.0f
#define RAIO_JUNTA_FINAL 7.0f

// Variáveis de estado
float anguloBase = 0;
float anguloBraco = 0;
float anguloAntebraco = 0;
float aberturaGarra = 10;
float posX = 0;
int segmentoSelecionado = -1;  // 0 = base, 1 = braço, 2 = antebraço
float centroX = 0, centroY = 0;
float anguloInicialMouse = 0;
float anguloOriginal = 0;


// Função para desenhar um retângulo
void drawRect(float largura, float altura) {
    glBegin(GL_POLYGON);
        glVertex2f(-largura / 2, 0);
        glVertex2f(largura / 2, 0);
        glVertex2f(largura / 2, altura);
        glVertex2f(-largura / 2, altura);
    glEnd();
}
// Função para desenhar um círculo
void drawCircle(float radius) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float angle = i * M_PI / 180.0f;
        glVertex2f(radius * cos(angle), radius * sin(angle));
    }   
    glEnd();
}
// Função para desenhar a base do braço robótico
void desenhaBase(){
    glColor3f(0.3, 0.3, 0.3);
    drawRect(60, 20);
}

// Função para desenhar o segmento do braço robótico
void desenhaSegmento(float largura, float altura, float r, float g, float b) {
    glColor3f(r, g, b);
    drawRect(largura, altura);
}
// Função para desenhar a junta do braço robótico
void desenhaJunta(float radius, float r, float g, float b) {
    glColor3f(r, g, b);
    drawCircle(radius);
}

// Função para desenhar a garra do braço robótico
void desenhaGarra(float abertura) {
    // Garra esquerda
    glPushMatrix();
    glTranslatef(-10.0f, 0.0f, 0.0f);
    glRotatef(abertura, 0, 0, 1);
    glColor3f(0.2, 0.8, 0.2);
    drawRect(5, 30);
    glTranslatef(0.0f, 30.0f, 0.0f);
    desenhaJunta(2.5, 0.2, 0.8, 0.2);
    glRotatef(-45, 0, 0, 1);
    drawRect(5, 20);
    glPopMatrix();

    // Garra direita
    glPushMatrix();
    glTranslatef(10.0f, 0.0f, 0.0f);
    glRotatef(-abertura, 0, 0, 1);
    glColor3f(0.2, 0.8, 0.2);
    drawRect(5, 30);
    glTranslatef(0.0f, 30.0f, 0.0f);
    desenhaJunta(2.5, 0.2, 0.8, 0.2);
    glRotatef(45, 0, 0, 1);
    drawRect(5, 20);
    glPopMatrix();
}

void desenhaBraco() {
    glPushMatrix();

    // Translação da base
    glTranslatef(posX, 0.0f, 0.0f);

    // Base fixa
    desenhaBase();

    // Rotação da base
    glTranslatef(0.0f, 20.0f, 0.0f);
    glColor3f(0.8f, 0.2f, 0.2f);
    desenhaJunta(RAIO_JUNTA, 0.8f, 0.2f, 0.2f);
    glRotatef(anguloBase, 0, 0, 1);

    // Braço
    desenhaSegmento(20, 100, 0.8f, 0.2f, 0.2f);

    // Ponto de rotação do braço
    glTranslatef(0.0f, 100.0f, 0.0f);
    glRotatef(anguloBraco, 0, 0, 1);

    // Antebraço
    desenhaSegmento(15, 80, 0.2f, 0.2f, 0.8f);
    desenhaJunta(RAIO_JUNTA, 1.0f, 1.0f, 1.0f);

    // Ponto de rotação do antebraço
    glTranslatef(0.0f, 80.0f, 0.0f);
    glRotatef(anguloAntebraco, 0, 0, 1);
    desenhaJunta(RAIO_JUNTA_FINAL, 1.0f, 1.0f, 1.0f);

    // Garras
    desenhaGarra(aberturaGarra);
    
    glPopMatrix(); // Fim base
}

// Função para desenhar o texto na tela
void desenhaTexto(float x, float y, const char * texto) {
    glColor3f(1.0, 1.0, 1.0); // Cor do texto: branco
    glRasterPos2f(x, y);
    for (const char* p = texto; *p != '\0'; p++) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *p);
    }
}


// Função de exibição
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    desenhaBraco();
    glLoadIdentity(); // Reset para desenhar texto sem transformações
    desenhaTexto(-390, 450, "Controles: A/D Base | W/S Braço | E/Q Antebraço | O/C Garra | Z/X Move Base | ESC Sair");
    desenhaTexto(-390, 430, "Clique nas juntas com o botão esquerdo para rotacionar segmentos.");
    desenhaTexto(-390, 410, "Clique com o botão direito para resetar os ângulos e a posição do braço.");
    glutSwapBuffers();
}



void teclado(unsigned char tecla, int x, int y) {
    switch (tecla) {
        case 'a': anguloBase += PASSO_ANGULO; 
            if(anguloBase > 90)
                anguloBase = 90;
            break;    
        case 'd': anguloBase -= PASSO_ANGULO;
            if(anguloBase < -90)
                anguloBase = -90;
            break;  
        case 'w': anguloBraco += PASSO_ANGULO; 
            if(anguloBraco > 150)
                anguloBraco = 150;
            break;
        case 's': anguloBraco -= PASSO_ANGULO;
            if(anguloBraco < -150)
                anguloBraco = -150;
            break;
        case 'e': anguloAntebraco += PASSO_ANGULO; 
            if(anguloAntebraco > 120)
                anguloAntebraco = 120;
            break;
        case 'q': anguloAntebraco -= PASSO_ANGULO; 
            if(anguloAntebraco < -120)
                anguloAntebraco = -120;
            break;
        case 'o': aberturaGarra += PASSO_GARRA; 
            if (aberturaGarra > MAX_ABERTURA_GARRA) 
                aberturaGarra = MAX_ABERTURA_GARRA; 
            break;
        case 'c': aberturaGarra -= PASSO_GARRA; 
            if (aberturaGarra < MIN_ABERTURA_GARRA) 
                aberturaGarra = MIN_ABERTURA_GARRA; 
            break;
        case 'z': posX -= PASSO_BASE; break;
        case 'x': posX += PASSO_BASE; break;
        case 27: exit(0); // ESC
    }
    glutPostRedisplay();
}

// Função para converter coordenadas de tela para coordenadas do mundo
float screenToWorldX(int x) {
    return (x / 800.0f) * 800.0f - 400.0f;
}
// Função para converter coordenadas de tela para coordenadas do mundo
float screenToWorldY(int y) {
    return 500.0f - ((y / 600.0f) * 600.0f);
}


// Função para gerenciar eventos do mouse
void mouse(int button, int state, int x, int y) {
   
    if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
        float mx = screenToWorldX(x);
        float my = screenToWorldY(y);

        // Junta da base
        float bx = posX;
        float by = 20.0f;

        // Junta do braço
        float radBase = -anguloBase * M_PI / 180.0f;
        float brx = bx + sin(radBase) * 100.0f;
        float bry = by + cos(radBase) * 100.0f;

        // Junta do antebraço
        float radBraco = -anguloBraco * M_PI / 180.0f;
        float totalAngle = radBase + radBraco;
        float arx = brx + sin(totalAngle) * 80.0f;
        float ary = bry + cos(totalAngle) * 80.0f;

        // Distâncias para detectar o segmento mais próximo
        float distBase = hypotf(mx - bx, my - by);
        float distBraco = hypotf(mx - brx, my - bry);
        float distAnte = hypotf(mx - arx, my - ary);

        float minDist = 35.0f; // Distância mínima para considerar o clique em um segmento
        // Detectar região aproximada do segmento
        if (distBase < minDist) segmentoSelecionado = 0; // Base
        else if (distBraco < minDist) segmentoSelecionado = 1; // Braço
        else if (distAnte < minDist) segmentoSelecionado = 2; // Antebraço
        else segmentoSelecionado = -1;

        // Define centro da rotação baseado no segmento
        if (segmentoSelecionado == 0) {
            centroX = bx;
            centroY = by;
            anguloOriginal = anguloBase;
        } else if (segmentoSelecionado == 1) {
            centroX = brx;
            centroY = bry;
            anguloOriginal = anguloBraco;
        } else if (segmentoSelecionado == 2) {
            centroX = arx;
            centroY = arx;
            anguloOriginal = anguloAntebraco;
        }

        anguloInicialMouse = atan2f(my - centroY, mx - centroX) * 180.0f / M_PI;

    } else if (state == GLUT_UP) {
        segmentoSelecionado = -1;
    }

    if(state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON) {
        anguloBase = anguloBraco = anguloAntebraco = 0;
        posX = 0;
        aberturaGarra = 10;
        segmentoSelecionado = -1;
    }
}


// Função para gerenciar movimento do mouse
void motion(int x, int y) {
    if (segmentoSelecionado == -1) return;

    float mx = screenToWorldX(x);
    float my = screenToWorldY(y);
    float anguloAtualMouse = atan2f(my - centroY, mx - centroX) * 180.0f / M_PI;
    float delta = anguloAtualMouse - anguloInicialMouse;
    
    if (delta > 180) delta -= 360;
    if (delta < -180) delta += 360;

    // Aplica rotação ao segmento com limites
    if (segmentoSelecionado == 0) {
        anguloBase = anguloOriginal + delta;
        if (anguloBase > 90) anguloBase = 90;
        if (anguloBase < -90) anguloBase = -90;
    } else if (segmentoSelecionado == 1) {
        anguloBraco = anguloOriginal + delta;
        if (anguloBraco > 150) anguloBraco = 150;
        if (anguloBraco < -150) anguloBraco = -150;
    } else if (segmentoSelecionado == 2) {
        anguloAntebraco = anguloOriginal + delta;
        if (anguloAntebraco > 120) anguloAntebraco = 120;
        if (anguloAntebraco < -120) anguloAntebraco = -120;
    }

    glutPostRedisplay();
}



// Função de inicialização
void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-400, 400, -100, 500);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Função principal
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Braço Robótico 2D");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(teclado);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutMainLoop();
    return 0;
}