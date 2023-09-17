#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <chrono>
#include <thread>
#include <string>
#include <vector>

using namespace std;

unsigned int caractere_largura = 5;
unsigned int caractere_altura = 7;
unsigned int caractere_espacamento = 2;
unsigned int caractere_tamanho = caractere_largura + caractere_espacamento;
unsigned int tempo = 50;
unsigned int cor = 32;
unsigned int cor_tempo = 0;
bool infinitamente = false;
bool debug = false;
string simbolo = "\u2588";

//Conjuntos de letras
vector<vector<unsigned int>> caracteres = {
    //Símbolo !
    {0,0,0,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,0,0,0, 0,0,1,0,0, 0,0,0,0,0},
    //Símbolo ""
    {0,1,0,1,0, 0,1,0,1,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0},
    //Símbolo #
    {0,0,0,0,0, 0,1,0,1,0, 1,1,1,1,1, 0,1,0,1,0, 1,1,1,1,1, 0,1,0,1,0, 0,0,0,0,0},
    //Símbolo $
    {0,0,1,0,0, 1,1,1,1,1, 1,0,1,0,0, 1,1,1,1,1, 0,0,1,0,1, 1,1,1,1,1, 0,0,1,0,0},
    //Símbolo %
    {0,0,0,0,0, 0,1,0,0,1, 0,0,0,1,0, 0,0,1,0,0, 0,1,0,0,0, 1,0,0,1,0, 0,0,0,0,0},
    //Símbolo &
    {0,0,0,0,0, 1,1,1,0,0, 1,0,0,1,0, 1,0,0,1,1, 1,0,0,1,0, 1,1,1,1,1, 0,0,0,0,0},
    //Símbolo '
    {0,0,1,0,0, 0,0,1,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0},
    //Símbolo (
    {0,0,0,0,0, 0,0,0,1,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,0,1,0, 0,0,0,0,0},
    //Símbolo )
    {0,0,0,0,0, 0,1,0,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,1,0,0,0, 0,0,0,0,0},
    //Símbolo *
    {0,0,0,0,0, 0,0,1,0,0, 0,1,1,1,0, 0,0,1,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0},
    //Símbolo +
    {0,0,0,0,0, 0,0,0,0,0, 0,0,1,0,0, 0,1,1,1,0, 0,0,1,0,0, 0,0,0,0,0, 0,0,0,0,0},
    //Símbolo ,
    {0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,1,1,0,0, 0,0,1,0,0},
    //Símbolo -
    {0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,1,1,1,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0},
    //Símbolo .
    {0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,1,0,0, 0,0,0,0,0},
    //Símbolo /
    {0,0,0,0,0, 0,0,0,1,0, 0,0,0,1,0, 0,0,1,0,0, 0,1,0,0,0, 0,1,0,0,0, 0,0,0,0,0},

    //Letra 0
    {0,0,0,0,0, 0,1,1,1,0, 1,1,0,0,1, 1,0,1,0,1, 1,0,0,1,1, 0,1,1,1,0, 0,0,0,0,0},
    //Número 1
    {0,0,0,0,0, 0,1,1,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,1,1,1,0, 0,0,0,0,0},
    //Número 2
    {0,0,0,0,0, 1,1,1,1,1, 0,0,0,0,1, 0,1,1,1,0, 1,0,0,0,0, 1,1,1,1,1, 0,0,0,0,0},
    //Número 3
    {0,0,0,0,0, 1,1,1,1,1, 0,0,0,0,1, 0,0,1,1,1, 0,0,0,0,1, 1,1,1,1,1, 0,0,0,0,0},
    //Número 4
    {0,0,0,0,0, 1,0,0,0,1, 1,0,0,0,1, 1,1,1,1,1, 0,0,0,0,1, 0,0,0,0,1, 0,0,0,0,0},
    //Número 5
    {0,0,0,0,0, 1,1,1,1,1, 1,0,0,0,0, 0,1,1,1,0, 0,0,0,0,1, 1,1,1,1,1, 0,0,0,0,0},
    //Número 6
    {0,0,0,0,0, 1,1,1,1,1, 1,0,0,0,0, 1,1,1,1,1, 1,0,0,0,1, 1,1,1,1,1, 0,0,0,0,0},
    //Número 7
    {0,0,0,0,0, 1,1,1,1,1, 0,0,0,0,1, 0,0,0,1,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,0,0,0},
    //Número 8
    {0,0,0,0,0, 1,1,1,1,1, 1,0,0,0,1, 1,1,1,1,1, 1,0,0,0,1, 1,1,1,1,1, 0,0,0,0,0},
    //Número 9
    {0,0,0,0,0, 1,1,1,1,1, 1,0,0,0,1, 1,1,1,1,1, 0,0,0,0,1, 1,1,1,1,1, 0,0,0,0,0},

    //Símbolo :
    {0,0,0,0,0, 0,0,0,0,0, 0,1,1,0,0, 0,0,0,0,0, 0,1,1,0,0, 0,0,0,0,0, 0,0,0,0,0},
    //Símbolo ;
    {0,0,0,0,0, 0,0,0,0,0, 0,1,1,0,0, 0,0,0,0,0, 0,1,1,0,0, 0,0,1,0,0, 0,0,0,0,0},
    //Símbolo <
    {0,0,0,0,0, 0,0,0,1,1, 0,0,1,0,0, 1,1,0,0,0, 0,0,1,0,0, 0,0,0,1,1, 0,0,0,0,0},
    //Símbolo =
    {0,0,0,0,0, 0,0,0,0,0, 1,1,1,1,1, 0,0,0,0,0, 1,1,1,1,1, 0,0,0,0,0, 0,0,0,0,0},
    //Símbolo >
    {0,0,0,0,0, 1,1,0,0,0, 0,0,1,0,0, 0,0,0,1,1, 0,0,1,0,0, 1,1,0,0,0, 0,0,0,0,0},
    //Símbolo ?
    {0,0,0,0,0, 0,1,1,1,0, 1,0,0,0,1, 0,0,1,1,0, 0,0,0,0,0, 0,0,1,0,0, 0,0,0,0,0},
    //Símbolo @
    {0,0,0,0,0, 1,1,1,1,1, 1,0,0,0,1, 1,0,1,1,1, 1,0,1,0,1, 1,0,1,1,1, 0,0,0,0,0},

    //Letra A
    {0,0,0,0,0, 1,1,1,1,1, 1,0,0,0,1, 1,1,1,1,1, 1,0,0,0,1, 1,0,0,0,1, 0,0,0,0,0},
    //Letra B
    {0,0,0,0,0, 1,1,1,1,0, 1,0,0,1,0, 1,1,1,1,1, 1,0,0,0,1, 1,1,1,1,1, 0,0,0,0,0},
    //Letra C
    {0,0,0,0,0, 1,1,1,1,1, 1,0,0,0,0, 1,0,0,0,0, 1,0,0,0,0, 1,1,1,1,1, 0,0,0,0,0},
    //Letra D
    {0,0,0,0,0, 1,1,1,1,0, 1,0,0,0,1, 1,0,0,0,1, 1,0,0,0,1, 1,1,1,1,0, 0,0,0,0,0},
    //Letra E
    {0,0,0,0,0, 1,1,1,1,1, 1,0,0,0,0, 1,1,1,1,0, 1,0,0,0,0, 1,1,1,1,1, 0,0,0,0,0},
    //Letra F
    {0,0,0,0,0, 1,1,1,1,1, 1,0,0,0,0, 1,1,1,1,0, 1,0,0,0,0, 1,0,0,0,0, 0,0,0,0,0},
    //Letra G
    {0,0,0,0,0, 1,1,1,1,1, 1,0,0,0,0, 1,0,1,1,1, 1,0,0,0,1, 1,1,1,1,1, 0,0,0,0,0},
    //Letra H
    {0,0,0,0,0, 1,0,0,0,1, 1,0,0,0,1, 1,1,1,1,1, 1,0,0,0,1, 1,0,0,0,1, 0,0,0,0,0},
    //Letra I
    {0,0,0,0,0, 0,0,1,0,0, 0,0,0,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,0,0,0},
    //Letra J
    {0,0,0,0,0, 0,0,1,0,0, 0,0,0,0,0, 0,0,1,0,0, 1,0,1,0,0, 1,1,1,0,0, 0,0,0,0,0},
    //Letra K
    {0,0,0,0,0, 1,0,0,0,1, 1,0,0,1,0, 1,1,1,0,0, 1,0,0,1,0, 1,0,0,0,1, 0,0,0,0,0},
    //Letra L
    {0,0,0,0,0, 1,0,0,0,0, 1,0,0,0,0, 1,0,0,0,0, 1,0,0,0,0, 1,1,1,1,1, 0,0,0,0,0},
    //Letra M
    {0,0,0,0,0, 1,1,0,1,1, 1,0,1,0,1, 1,0,1,0,1, 1,0,0,0,1, 1,0,0,0,1, 0,0,0,0,0},
    //Letra N
    {0,0,0,0,0, 1,1,0,0,1, 1,0,1,0,1, 1,0,0,1,1, 1,0,0,0,1, 1,0,0,0,1, 0,0,0,0,0},
    //Letra O
    {0,0,0,0,0, 1,1,1,1,1, 1,0,0,0,1, 1,0,0,0,1, 1,0,0,0,1, 1,1,1,1,1, 0,0,0,0,0},
    //Letra P
    {0,0,0,0,0, 1,1,1,1,1, 1,0,0,0,1, 1,1,1,1,1, 1,0,0,0,0, 1,0,0,0,0, 0,0,0,0,0},
    //Letra Q
    {0,0,0,0,0, 1,1,1,1,1, 1,0,0,0,1, 1,0,0,0,1, 1,0,0,1,1, 1,1,1,1,1, 0,0,0,0,0},
    //Letra R
    {0,0,0,0,0, 1,1,1,1,1, 1,0,0,0,1, 1,1,1,1,0, 1,0,0,0,1, 1,0,0,0,1, 0,0,0,0,0},
    //Letra S
    {0,0,0,0,0, 1,1,1,1,1, 1,0,0,0,0, 1,1,1,1,1, 0,0,0,0,1, 1,1,1,1,1, 0,0,0,0,0},
    //Letra T
    {0,0,0,0,0, 1,1,1,1,1, 0,0,1,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,0,0,0},
    //Letra U
    {0,0,0,0,0, 1,0,0,0,1, 1,0,0,0,1, 1,0,0,0,1, 1,0,0,0,1, 1,1,1,1,1, 0,0,0,0,0},
    //Letra V
    {0,0,0,0,0, 1,0,0,0,1, 1,0,0,0,1, 0,1,0,1,0, 0,1,0,1,0, 0,0,1,0,0, 0,0,0,0,0},
    //Letra W
    {0,0,0,0,0, 1,0,0,0,1, 1,0,0,0,1, 1,0,1,0,1, 1,0,1,0,1, 1,1,0,1,1, 0,0,0,0,0},
    //Letra X
    {0,0,0,0,0, 1,0,0,0,1, 0,1,0,1,0, 0,0,1,0,0, 0,1,0,1,0, 1,0,0,0,1, 0,0,0,0,0},
    //Letra Y
    {0,0,0,0,0, 1,0,0,0,1, 0,1,0,1,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,0,0,0},
    //Letra Z
    {0,0,0,0,0, 1,1,1,1,1, 0,0,0,1,0, 0,0,1,0,0, 0,1,0,0,0, 1,1,1,1,1, 0,0,0,0,0}
};

bool imprimirCaractere(char u,unsigned int linha,unsigned int* colunas,unsigned int startpos){
    if (startpos < caractere_largura){
        for (unsigned int s=startpos;s<caractere_largura;s++){
            char unicode = toupper(u);

            if (unicode == ' ')
                cout << " ";
            else{
                unsigned int caractere;
                if (unicode >= '!')
                    caractere = caracteres[unicode - '!'][s + (linha * caractere_largura)];
                //else
                //    caractere = caracteres[unicode - '0' + 26][s + (linha * caractere_largura)];
                
                if (caractere == 1)
                    cout << simbolo;
                else
                    cout << " ";
            }
            *colunas = *colunas - 1;
            if (*colunas <= 0)
                return true;
        }
    }
    //Espaçamento entre caracteres
    if (*colunas > 0){
        for (unsigned int s=0;s<caractere_espacamento;s++){
            cout << " ";
            *colunas = *colunas - 1;
            if (*colunas <= 0)
                return true;
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
        if (cor > 30)
            cout << "\x1B[" << cor << "m";

        //Espaçamento lateral
        if (espaco > tamanho){
            for (unsigned int s=0;s<espaco-tamanho;s++){
                cout << " ";
                colunas--;
            }
        }
        //Se mensagem sumir da tela, pular para próxima linha
        if (colunas > 0){
            //Caractere inicial, caso mensagem esteja sumindo
            unsigned int stcar = 0;
            if (espaco <= tamanho)
                stcar = texto.length() - (espaco/caractere_tamanho) - 1;

            //Para cada caractere
            for (unsigned int c=stcar;c<texto.length();c++){
                //Posição inicial, caso mensagem esteja sumindo
                unsigned int stpos = 0;
                if (espaco <= tamanho && c == stcar)
                    stpos = caractere_tamanho - (espaco % caractere_tamanho);
                
                //Para cada coluna em cada caractere
                if (imprimirCaractere(texto[c],y,&colunas,stpos))
                    break;
            }
        }
        if (cor > 30)
            cout << "\033[0m";
        cout << endl;
    }
}

int main(int argc, char* argv[]){
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    //Exibir ajuda
    if ((argc == 1) || (argc == 2 && argv[1][0] == '-')){
        cout << "Bashsays v1.0 by Ewerton Bramos" << endl;
        cout << "   bashsays [-args] [args value] \"message\"\n" << endl;
        cout << "   -i: run the message forever." << endl;
        cout << "   -s: run the message at a slower pace. Can be repeated up to 5 times." << endl;
        cout << "   -f: run the message at a faster pace. Can be repeated up to 5 times." << endl;
        cout << "   -c [1-16]: change default/first text color." << endl;
        cout << "      1: black         9: bright black" << endl;
        cout << "      2: red          10: bright red" << endl;
        cout << "      3: green        11: bright green" << endl;
        cout << "      4: yellow       12: bright yellow" << endl;
        cout << "      5: blue         13: bright blue" << endl;
        cout << "      6: magenta      14: bright magenta" << endl;
        cout << "      7: cyan         15: bright cyan" << endl;
        cout << "      8: white        16: bright white" << endl;
        cout << "   -t [0|1+]: color blinking timeout." << endl;
        cout << "   -p: change message symbols.\n" << endl;
        cout << "   Exit the program with CTRL^C or Q.\n" << endl;
        return 0;
    }
    
    //Obter parâmetros opicionais
    unsigned int argst = 1;

    if (argv[1][0] == '-'){
        for (unsigned int p=1;p<strlen(argv[1]);p++){
            switch (argv[1][p]){
                case 'i':
                    infinitamente = true;
                    break;
                case 's':
                    tempo += 10;
                    if (tempo > 100)
                        tempo = 100;
                    break;
                case 'f':
                    tempo -= 10;
                    if (tempo < 5)
                        tempo = 5;
                    break;
                case 'c':
                    argst++;
                    cor = 30 + (*argv[argst] - '0');
                    if (cor > 38)
                        cor += 60;
                    if (cor > 98);
                        cor = 30;
                    break;
                case 't':
                    argst++;
                    cor_tempo = *argv[argst] - '0';
                    break;
                case 'p':
                    argst++;
                    simbolo = argv[argst][0];
                    break;
                case 'd':
                    debug = true;
                    break;
            }
        }
        argst++;
    }
    //Se tiver argumentos com valores sem texto a exibir, finalizar
    if (argst >= argc){
        cout << "Display message is missing." << endl;
        return 0;
    }

    //Exibir mensagem
    string texto_mensagem = "";
    for (unsigned int i=argst;i<argc;i++)
        texto_mensagem += string(argv[i]) + string(" ");

    //Obter mensagem a partir de um comando pipe
    /*vector<char> someData(100, '-');
    cin.read(&someData.front(), someData.size());
    someData.back() = '\0';
    cout << "DADOS: " << someData.data()[0] << endl;*/

    unsigned int texto_tamanho = caractere_tamanho * texto_mensagem.length();
    unsigned int texto_x = w.ws_col + texto_tamanho;
    unsigned int espacamento_topo = (w.ws_row/2) - (caractere_altura/2);
    unsigned int cor_espera = cor_tempo;
    
    cout << "\x1B[2J\x1B[H";
    
    while (texto_x > 0){
        if (debug){
            cout << "parameters: " << (strlen(argv[1]) - 1) << "; sleep: " << tempo;
            cout << "; color: " << cor << "; color timeout: " << cor_tempo;
            cout << "; window width: " << w.ws_col << "; text size: " << texto_tamanho << endl;
        }

        //Espaçamento superior
        for (unsigned int s=0;s<espacamento_topo;s++)
            cout << endl;
        
        //Escrever mensagem e alterar posição
        imprimirMensagem(texto_mensagem,texto_x,w.ws_col);
        texto_x -= 1;
        this_thread::sleep_for(chrono::milliseconds(tempo));
        cout << "\x1B[2J\x1B[H"; //Limpar terminal

        //Alterar cor e tempo de cor
        if (cor_tempo > 0){
            cor_espera--;
            if (cor_espera <= 0){
                cor_espera = cor_tempo;
                cor++;
                if (cor == 38)
                    cor = 90;
                if (cor == 98)
                    cor = 30;
            }
        }

        //Repetir caso esteja em modo infinito
        if (infinitamente && texto_x <= 0)
            texto_x = w.ws_col + texto_tamanho;
    }

    return 0;
}