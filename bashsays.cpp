#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <vector>

using namespace std;

unsigned int caractere_largura = 5;
unsigned int caractere_altura = 5;
unsigned int caractere_espacamento = 2;
unsigned int caractere_tamanho = caractere_largura + caractere_espacamento;

//Conjuntos de letras
vector<vector<unsigned int>> caracteres = {
    //Letra A
    {
        1,1,1,1,1,
        1,0,0,0,1,
        1,1,1,1,1,
        1,0,0,0,1,
        1,0,0,0,1
    },
    //Letra B
    {
        1,1,1,1,0,
        1,0,0,1,0,
        1,1,1,1,1,
        1,0,0,0,1,
        1,1,1,1,1
    },
    //Letra C
    {
        1,1,1,1,1,
        1,0,0,0,0,
        1,0,0,0,0,
        1,0,0,0,0,
        1,1,1,1,1
    },
    //Letra D
    {
        1,1,1,1,0,
        1,0,0,0,1,
        1,0,0,0,1,
        1,0,0,0,1,
        1,1,1,1,0
    },
    //Letra E
    {
        1,1,1,1,1,
        1,0,0,0,0,
        1,1,1,1,0,
        1,0,0,0,0,
        1,1,1,1,1
    },
    //Letra F
    {
        1,1,1,1,1,
        1,0,0,0,0,
        1,1,1,1,0,
        1,0,0,0,0,
        1,0,0,0,0
    },
    //Letra G
    {
        1,1,1,1,1,
        1,0,0,0,0,
        1,0,1,1,1,
        1,0,0,0,1,
        1,1,1,1,1
    },
    //Letra H
    {
        1,0,0,0,1,
        1,0,0,0,1,
        1,1,1,1,1,
        1,0,0,0,1,
        1,0,0,0,1
    },
    //Letra I
    {
        0,0,1,0,0,
        0,0,0,0,0,
        0,0,1,0,0,
        0,0,1,0,0,
        0,0,1,0,0
    },
    //Letra J
    {
        0,0,1,0,0,
        0,0,0,0,0,
        0,0,1,0,0,
        1,0,1,0,0,
        1,1,1,0,0
    },
    //Letra K
    {
        1,0,0,0,1,
        1,0,0,1,0,
        1,1,1,0,0,
        1,0,0,1,0,
        1,0,0,0,1
    },
    //Letra L
    {
        1,0,0,0,0,
        1,0,0,0,0,
        1,0,0,0,0,
        1,0,0,0,0,
        1,1,1,1,1
    },
    //Letra M
    {
        1,1,0,1,1,
        1,0,1,0,1,
        1,0,1,0,1,
        1,0,0,0,1,
        1,0,0,0,1
    },
    //Letra N
    {
        1,1,0,0,1,
        1,0,1,0,1,
        1,0,0,1,1,
        1,0,0,0,1,
        1,0,0,0,1
    },
    //Letra O
    {
        1,1,1,1,1,
        1,0,0,0,1,
        1,0,0,0,1,
        1,0,0,0,1,
        1,1,1,1,1
    },
    //Letra P
    {
        1,1,1,1,1,
        1,0,0,0,1,
        1,1,1,1,1,
        1,0,0,0,0,
        1,0,0,0,0
    },
    //Letra Q
    {
        1,1,1,1,1,
        1,0,0,0,1,
        1,0,0,0,1,
        1,0,0,1,1,
        1,1,1,1,1
    },
    //Letra R
    {
        1,1,1,1,1,
        1,0,0,0,1,
        1,1,1,1,0,
        1,0,0,0,1,
        1,0,0,0,1
    },
    //Letra S
    {
        1,1,1,1,1,
        1,0,0,0,0,
        1,1,1,1,1,
        0,0,0,0,1,
        1,1,1,1,1
    },
    //Letra T
    {
        1,1,1,1,1,
        0,0,1,0,0,
        0,0,1,0,0,
        0,0,1,0,0,
        0,0,1,0,0
    },
    //Letra U
    {
        1,0,0,0,1,
        1,0,0,0,1,
        1,0,0,0,1,
        1,0,0,0,1,
        1,1,1,1,1
    },
    //Letra V
    {
        1,0,0,0,1,
        1,0,0,0,1,
        0,1,0,1,0,
        0,1,0,1,0,
        0,0,1,0,0
    },
    //Letra W
    {
        1,0,0,0,1,
        1,0,0,0,1,
        1,0,1,0,1,
        1,0,1,0,1,
        1,1,0,1,1
    },
    //Letra X
    {
        1,0,0,0,1,
        0,1,0,1,0,
        0,0,1,0,0,
        0,1,0,1,0,
        1,0,0,0,1
    },
    //Letra Y
    {
        1,0,0,0,1,
        0,1,0,1,0,
        0,0,1,0,0,
        0,0,1,0,0,
        0,0,1,0,0
    },
    //Letra Z
    {
        1,1,1,1,1,
        0,0,0,1,0,
        0,0,1,0,0,
        0,1,0,0,0,
        1,1,1,1,1
    }
};

bool imprimirCaractere(char u,unsigned int linha,unsigned int* colunas,unsigned int startpos){
    if (startpos < caractere_largura){
        for (unsigned int s=startpos;s<caractere_largura;s++){
            char unicode = toupper(u);

            if (unicode == ' '){
                cout << " ";
            }else{
                unsigned int caractere = caracteres[unicode - 'A'][s + (linha * caractere_largura)];
                
                if (caractere == 1){
                    cout << "#";
                }else{
                    cout << " ";
                }
            }
            *colunas = *colunas - 1;
            if (*colunas <= 0){
                return true;
            }
        }
    }
    //Espaçamento entre caracteres
    if (*colunas > 0){
        for (unsigned int s=0;s<caractere_espacamento;s++){
            cout << " ";
            *colunas = *colunas - 1;
            if (*colunas <= 0){
                return true;
            }
        }
    }
    return false;
}

void imprimirMensagem(string texto,unsigned int espaco,unsigned int limite){
    unsigned int colunas = 0;
    unsigned int tamanho = caractere_tamanho * texto.length();

    //Para cada linha de cada caractere
    for (unsigned int y=0;y<caractere_altura;y++){
        colunas = limite;

        //Espaçamento lateral
        if (espaco > tamanho){
            for (unsigned int s=0;s<espaco-tamanho;s++){
                if (s % 4 == 0) {cout << ".";}
                if (s % 4 == 1) {cout << "_";}
                if (s % 4 == 2) {cout << "-";}
                if (s % 4 == 3) {cout << "=";}
                colunas--;
            }
        }
        //Se mensagem sumir da tela, pular para próxima linha
        if (colunas > 0){
            //Para cada caractere
            for (unsigned int c=0;c<texto.length();c++){
                //Posição inicial, caso mensagem esteja sumindo
                unsigned int stpos = 0;
                if (espaco <= tamanho){
                    stpos = (espaco-tamanho) % caractere_tamanho;
                }
                //Para cada coluna em cada caractere
                if (imprimirCaractere(texto[c],y,&colunas,stpos)) {
                    break;
                }
            }
        }
        cout << endl;
    }
}

int main(int argc, char* argv[]){
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    string texto_mensagem = "Hello";
    if (argc > 1){
        texto_mensagem = "";
        for (unsigned int i=1;i<argc;i++){
            texto_mensagem += string(argv[i]) + string(" ");
        }
    }

    unsigned int texto_tamanho = caractere_tamanho * texto_mensagem.length();
    unsigned int texto_x = w.ws_col + texto_tamanho;
    unsigned int sleep = 50;
    unsigned int espacamento_topo = (w.ws_row/2) - (caractere_altura/2);

    while (texto_x > 0){
        cout << "cols: " << texto_x << "; tamanho: " << (texto_tamanho) << "; limite: " << w.ws_col << endl;
        for (unsigned int s=0;s<espacamento_topo;s++)
            cout << endl;
        imprimirMensagem(texto_mensagem,texto_x,w.ws_col);
        texto_x -= 1;
        this_thread::sleep_for(chrono::milliseconds(sleep));
        system("clear");
    }

    return 0;
}