#include <stdio.h>
#include <windows.h>	//da una posicion al cursor.
#include <conio.h>
#include <stdlib.h>
#include <list>

using namespace std;

//codigos ascii de las flechas
#define ARRIBA 72
#define IZQUIERDA 75
#define DERECHA 77
#define ABAJO 80

void gotoxy(int x, int y){
    HANDLE hCon; //Identificador
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	//estructura y un objeto de la estructura
	COORD dwPos;
	dwPos.X = x;
	dwPos.Y = y;
	SetConsoleCursorPosition(hCon, dwPos);  //se seetean las coordenadas
}

void OcultarCursor(){
    HANDLE hCon; //Identificador
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    //Objeto de estructura
    CONSOLE_CURSOR_INFO cci;
    cci.dwSize = 2;    //tamano del cursor
    cci.bVisible = FALSE;    //controla la visibilidad del cursor
    SetConsoleCursorInfo(hCon, &cci);
}

void pintar_limites(){
    for(int i=2; i<78 ; i++){
        gotoxy(i,3); printf("%c",205);   //linea superior
        gotoxy(i,33); printf("%c",205);   //linea inferior
    }
    for(int i=4; i<33;i++){
        gotoxy(2,i); printf("%c",186);   //linea izquierda
        gotoxy(77,i); printf("%c",186);
    }
    //esquinas
    gotoxy(2,3); printf("%c",201);
    gotoxy(2,33); printf("%c",200);
    gotoxy(77,3); printf("%c",187);
    gotoxy(77,33); printf("%c",188);
}

class NAVE{
    int x,y;    //coordenadas
    int corazones;
    int vidas;
public: //accesible
        //constructor
    NAVE(int _x, int _y, int _corazones, int _vidas): x(_x),y(_y),corazones(_corazones),vidas(_vidas){}
        //metodos
    int X(){ return x; } //retorna la posicion x de la nave
    int Y(){ return y; } //retorna la posicion y.
    int VID() { return vidas; }
    void COR(){ corazones--;}   //para que decrezcan los corazones
    void pintar();
    void borrar();
    void mover();
    void pintar_corazones();
    void morir();
};

void NAVE::pintar(){
    gotoxy(x,y); printf("  %c",30); //inicial
    gotoxy(x,y+1); printf(" %c%c%c",40,207,41);    //segunda linea del avion
    gotoxy(x,y+2); printf("%c%c %c%c",30,190,190,30);   //tercera linea
}

void NAVE::borrar(){
    gotoxy(x,y); printf("        ");
    gotoxy(x,y+1); printf("        ");
    gotoxy(x,y+2); printf("        ");
}

void NAVE::mover(){
    if(kbhit()){    //kbhit detecta si se presiono una tecla
        char tecla = getch();   //captura la tecla.
        borrar();
        if(tecla == IZQUIERDA && x>3) x--; //si se toca j se mueve a la izq
        if(tecla == DERECHA && x+6 < 77) x++; //si se toca l se mueve a la der
        if(tecla == ARRIBA && y>4) y--;   //sube. para subir se le resta
        if(tecla == ABAJO && y+3 < 33) y++;   //baja
        //gotoxy(x,y);printf("*");
        if(tecla == 'a')corazones--;
        pintar();
        pintar_corazones();
    }
}

void NAVE::pintar_corazones(){
    gotoxy(50,2); printf("VIDAS %d",vidas);
    gotoxy(64,2); printf("Salud");
    gotoxy(70,2); printf("       ");

    for(int i=0; i < corazones;i++){
        gotoxy(70+i,2); printf("%c",3);
    }
}

void NAVE::morir(){
    if(corazones == 0){
        borrar();
        gotoxy(x,y); printf("   **   ");
        gotoxy(x,y+1); printf("  ****  ");
        gotoxy(x,y+2); printf("   **   ");
        Sleep(500);

        borrar();
        gotoxy(x,y); printf(" * ** * ");
        gotoxy(x,y+1); printf("  ****  ");
        gotoxy(x,y+2); printf(" * ** * ");
        Sleep(500);
        borrar();

        vidas--;
        corazones = 3;
        pintar_corazones();
        pintar();
    }
}

class AST{
    int x,y;
public:
    AST(int _x, int _y):x(_x),y(_y){}
    void pintar();
    void mover();
    void choque(class NAVE &N);    //porque se le haran cambios se pasa por referencia '&'
    int X() { return x; }
    int Y() { return y; }
};

void AST::pintar(){
    gotoxy(x,y); printf("%c",245);
}

void AST::mover(){
    gotoxy(x,y); printf(" ");
    y++;
    if(y > 32){
        x = rand()%71 + 4;
        y = 4;
    }
    pintar();
}

void AST::choque(class NAVE &N){
    if( x >= N.X() && x < N.X()+6 && y >= N.Y() && y <= N.Y()+2)  //si elasteroide es igual a la nave
    {
        N.COR();
        N.borrar();
        N.pintar();
        N.pintar_corazones();

        x = rand()%71 + 4;
        y = 4;
    }
}

class BALA{
    int x,y;
public:
    BALA(int _x, int _y): x(_x), y(_y){}
    int X(){ return x; }
    int Y(){ return y; }
    void mover();
    bool fuera();
};

void BALA::mover(){
    gotoxy(x,y); printf(" ");
    y--;
    gotoxy(x,y); printf("*");
}

bool BALA::fuera(){
    if(y==4) return true;
    return false;
}

int main(){
    OcultarCursor();
    pintar_limites();
    NAVE N(37,30,3,3);    //creó el objeto nave CONSTRUCTOR
    N.pintar();
    N.pintar_corazones();

    list<AST*> A;
    list<AST*>::iterator itA;

    for(int i=0; i<5; i++){
        A.push_back(new AST(rand()%75 + 3, rand()%5 + 4));
    }

    list<BALA*> B;
    list<BALA*>::iterator it;

    bool game_over = false;
    int puntos = 0;
    while(!game_over){  //si es verdad
        gotoxy(4,2); printf("Puntos %d",puntos);
        if(kbhit()){
            char tecla =getch();
            if(tecla == 'a')
                B.push_back(new BALA(N.X()+2, N.Y()-1));
        }

        for(it = B.begin(); it != B.end(); it++) //si hay bala
        {
            (*it)->mover();
            if((*it)->fuera()){
                gotoxy((*it)->X(), (*it)->Y()); printf(" ");
                delete(*it);
                it = B.erase(it);
            }
        }

        for(itA = A.begin(); itA != A.end();itA++){
            (*itA)->mover();
            (*itA)->choque(N);
        }

        for(itA = A.begin(); itA != A.end(); itA++){
            for(it = B.begin(); it != B.end(); it++){
                if((*itA)->X() == (*it)->X() && ( (*itA)->Y() +1 == (*it)->Y() || (*itA)->Y() == (*it)->Y() )){
                    gotoxy((*it)-> X(), (*it)->Y()); printf(" ");
                    delete(*it);
                    it = B.erase(it);

                    A.push_back(new AST(rand()%75 + 3, 4));
                    gotoxy((*itA)-> X(), (*itA)->Y()); printf(" ");
                    delete(*itA);
                    itA = A.erase(itA);


                    puntos+=5;
                }
            }
        }

        if(N.VID() == 0) game_over = true;
        N.morir();
        N.mover();

        if(puntos == 100){
            game_over=true;
            gotoxy(36,18);
            printf("YOU WIN");
            Sleep(4000);
        }
        Sleep(30);
    }

    system("cls");
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t     GAME OVER\n\n\n\n\n\n\n\n\n\n\n\n");
    getch();
    return 0;
}
