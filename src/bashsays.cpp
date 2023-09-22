#include <sys/ioctl.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <string>

using namespace std;

unsigned short int caractere_largura = 5;
unsigned short int caractere_altura = 7;
unsigned short int caractere_espacamento = 2;
unsigned short int caractere_tamanho = caractere_largura + caractere_espacamento;
unsigned short int tempo = 5;
unsigned short int cor = 32;
unsigned short int cor_tempo = 0;
unsigned short int cor_borda = 31;
unsigned short int mensagens_quantidade = 1;

bool infinitamente = false;
bool paraDireita = false;
bool debug = false;

string simbolo = "\u2588";
string fundo = "\u2591";
string borda = "=";

//Conjuntos de letras
unsigned int caracteres[64][35] = {
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
    {0,0,0,0,0, 1,1,1,1,1, 1,1,0,0,1, 1,0,1,0,1, 1,0,0,1,1, 1,1,1,1,1, 0,0,0,0,0},
    //Número 1
    {0,0,0,0,0, 0,1,1,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,1,1,1,0, 0,0,0,0,0},
    //Número 2
    {0,0,0,0,0, 0,1,1,1,0, 1,0,0,0,1, 0,0,1,1,0, 0,1,0,0,0, 1,1,1,1,1, 0,0,0,0,0},
    //Número 3
    {0,0,0,0,0, 1,1,1,1,1, 0,0,0,1,1, 0,1,1,1,1, 0,0,0,1,1, 1,1,1,1,1, 0,0,0,0,0},
    //Número 4
    {0,0,0,0,0, 1,1,0,1,1, 1,1,0,1,1, 1,1,1,1,1, 0,0,0,1,1, 0,0,0,1,1, 0,0,0,0,0},
    //Número 5
    {0,0,0,0,0, 1,1,1,1,1, 1,0,0,0,0, 0,1,1,1,0, 0,0,0,0,1, 1,1,1,1,1, 0,0,0,0,0},
    //Número 6
    {0,0,0,0,0, 1,1,1,1,1, 1,1,0,0,0, 1,1,1,1,1, 1,1,0,1,1, 1,1,1,1,1, 0,0,0,0,0},
    //Número 7
    {0,0,0,0,0, 1,1,1,1,1, 0,0,0,0,1, 0,0,0,1,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,0,0,0},
    //Número 8
    {0,0,0,0,0, 1,1,1,1,1, 1,1,0,1,1, 1,1,1,1,1, 1,1,0,1,1, 1,1,1,1,1, 0,0,0,0,0},
    //Número 9
    {0,0,0,0,0, 1,1,1,1,1, 1,1,0,1,1, 1,1,1,1,1, 0,0,0,1,1, 1,1,1,1,1, 0,0,0,0,0},

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
    {0,0,0,0,0, 1,1,1,1,1, 1,0,0,0,1, 0,0,1,1,1, 0,0,0,0,0, 0,0,1,0,0, 0,0,0,0,0},
    //Símbolo @
    {0,0,0,0,0, 1,1,1,1,1, 1,0,0,0,1, 1,0,1,1,1, 1,0,1,0,1, 1,0,1,1,1, 0,0,0,0,0},

    //Letra A
    {0,0,0,0,0, 1,1,1,1,1, 1,1,0,1,1, 1,1,1,1,1, 1,1,0,1,1, 1,1,0,1,1, 0,0,0,0,0},
    //Letra B
    {0,0,0,0,0, 1,1,1,1,0, 1,0,0,1,0, 1,1,1,1,1, 1,0,0,0,1, 1,1,1,1,1, 0,0,0,0,0},
    //Letra C
    {0,0,0,0,0, 1,1,1,1,1, 1,1,0,0,0, 1,1,0,0,0, 1,1,0,0,0, 1,1,1,1,1, 0,0,0,0,0},
    //Letra D
    {0,0,0,0,0, 1,1,1,1,0, 1,1,0,0,1, 1,1,0,0,1, 1,1,0,0,1, 1,1,1,1,0, 0,0,0,0,0},
    //Letra E
    {0,0,0,0,0, 1,1,1,1,1, 1,1,0,0,0, 1,1,1,1,0, 1,1,0,0,0, 1,1,1,1,1, 0,0,0,0,0},
    //Letra F
    {0,0,0,0,0, 1,1,1,1,1, 1,1,0,0,0, 1,1,1,1,0, 1,1,0,0,0, 1,1,0,0,0, 0,0,0,0,0},
    //Letra G
    {0,0,0,0,0, 1,1,1,1,1, 1,1,0,0,0, 1,1,0,1,1, 1,1,0,0,1, 1,1,1,1,1, 0,0,0,0,0},
    //Letra H
    {0,0,0,0,0, 1,1,0,1,1, 1,1,0,1,1, 1,1,1,1,1, 1,1,0,1,1, 1,1,0,1,1, 0,0,0,0,0},
    //Letra I
    {0,0,0,0,0, 0,1,1,1,0, 0,0,0,0,0, 0,1,1,1,0, 0,1,1,1,0, 0,1,1,1,0, 0,0,0,0,0},
    //Letra J
    {0,0,0,0,0, 0,0,1,1,0, 0,0,0,0,0, 0,0,1,1,0, 1,0,1,1,0, 1,1,1,1,0, 0,0,0,0,0},
    //Letra K
    {0,0,0,0,0, 1,1,0,0,1, 1,1,0,1,0, 1,1,1,0,0, 1,1,0,1,0, 1,1,0,0,1, 0,0,0,0,0},
    //Letra L
    {0,0,0,0,0, 1,1,0,0,0, 1,1,0,0,0, 1,1,0,0,0, 1,1,0,0,0, 1,1,1,1,1, 0,0,0,0,0},
    //Letra M
    {0,0,0,0,0, 1,1,0,1,1, 1,0,1,0,1, 1,0,1,0,1, 1,0,0,0,1, 1,0,0,0,1, 0,0,0,0,0},
    //Letra N
    {0,0,0,0,0, 1,1,0,0,1, 1,0,1,0,1, 1,0,0,1,1, 1,0,0,0,1, 1,0,0,0,1, 0,0,0,0,0},
    //Letra O
    {0,0,0,0,0, 1,1,1,1,1, 1,1,0,1,1, 1,1,0,1,1, 1,1,0,1,1, 1,1,1,1,1, 0,0,0,0,0},
    //Letra P
    {0,0,0,0,0, 1,1,1,1,1, 1,1,0,1,1, 1,1,1,1,1, 1,1,0,0,0, 1,1,0,0,0, 0,0,0,0,0},
    //Letra Q
    {0,0,0,0,0, 1,1,1,1,1, 1,0,0,0,1, 1,0,0,0,1, 1,0,0,1,1, 1,1,1,1,1, 0,0,0,0,0},
    //Letra R
    {0,0,0,0,0, 1,1,1,1,1, 1,1,0,0,1, 1,1,1,1,0, 1,1,0,0,1, 1,1,0,0,1, 0,0,0,0,0},
    //Letra S
    {0,0,0,0,0, 1,1,1,1,1, 1,1,0,0,0, 1,1,1,1,1, 0,0,0,1,1, 1,1,1,1,1, 0,0,0,0,0},
    //Letra T
    {0,0,0,0,0, 1,1,1,1,1, 0,0,1,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,0,0,0},
    //Letra U
    {0,0,0,0,0, 1,1,0,1,1, 1,1,0,1,1, 1,1,0,1,1, 1,1,0,1,1, 1,1,1,1,1, 0,0,0,0,0},
    //Letra V
    {0,0,0,0,0, 1,0,0,0,1, 1,0,0,0,1, 1,1,0,1,1, 0,1,0,1,0, 0,0,1,0,0, 0,0,0,0,0},
    //Letra W
    {0,0,0,0,0, 1,0,0,0,1, 1,0,0,0,1, 1,0,1,0,1, 1,0,1,0,1, 1,1,0,1,1, 0,0,0,0,0},
    //Letra X
    {0,0,0,0,0, 1,0,0,0,1, 1,1,0,1,1, 0,1,1,1,0, 1,1,0,1,1, 1,0,0,0,1, 0,0,0,0,0},
    //Letra Y
    {0,0,0,0,0, 1,0,0,0,1, 0,1,0,1,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,0,0,0},
    //Letra Z
    {0,0,0,0,0, 1,1,1,1,1, 0,0,0,1,0, 0,0,1,0,0, 0,1,0,0,0, 1,1,1,1,1, 0,0,0,0,0},
    
    //Símbolo [
    {0,0,0,0,0, 0,1,1,1,0, 0,1,0,0,0, 0,1,0,0,0, 0,1,0,0,0, 0,1,1,1,0, 0,0,0,0,0},
    //Símbolo "\"
    {0,0,0,0,0, 0,1,0,0,0, 0,1,0,0,0, 0,0,1,0,0, 0,0,0,1,0, 0,0,0,1,0, 0,0,0,0,0},
    //Símbolo ]
    {0,0,0,0,0, 0,1,1,1,0, 0,0,0,1,0, 0,0,0,1,0, 0,0,0,1,0, 0,1,1,1,0, 0,0,0,0,0},
    //Símbolo ^
    {0,0,0,0,0, 0,0,1,0,0, 0,1,0,1,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0},
    //Símbolo _
    {0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,1,1,1,1, 0,0,0,0,0},
    //Símbolo `
    {0,0,0,0,0, 0,1,0,0,0, 0,0,1,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0}
};

int kbhit(){
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch == 'q' || ch == 'Q'){
        ungetc(ch, stdin);
        return 1;
    }

  return 0;
}

bool imprimirCaractere(char u,unsigned int linha,unsigned int* colunas,unsigned int startpos){
    if (startpos < caractere_largura){
        for (unsigned int s=startpos;s<caractere_largura;s++){
            if (u == ' ')
                printf("%s",fundo.c_str());
            else{
                unsigned int caractere;
                if (u >= 'a' && u <= 'z')
                    u = toupper(u);
                if (u >= '!' && u < '{')
                    caractere = caracteres[u - '!'][s + (linha * caractere_largura)];
                else
                    caractere = caracteres['?' - '!'][s + (linha * caractere_largura)];

                if (caractere == 1)
                    printf("%s",simbolo.c_str());
                else
                    printf("%s",fundo.c_str());
            }
            *colunas = *colunas - 1;
            if (*colunas <= 0)
                return true;
        }
    }
    //Espaçamento entre caracteres
    if (*colunas > 0){
        for (unsigned int s=0;s<caractere_espacamento;s++){
            printf("%s",fundo.c_str());
            *colunas = *colunas - 1;
            if (*colunas <= 0)
                return true;
        }
    }
    return false;
}

void imprimirMensagem(string texto,unsigned int espaco,unsigned int limite){
    unsigned int colunas = 0;
    unsigned int mensagem_tamanho = texto.length();
    unsigned int tamanho = caractere_tamanho * mensagem_tamanho;

    //Para cada linha de cada caractere
    for (unsigned int y=0;y<caractere_altura;y++){
        colunas = limite;
        if (cor > 30)
            printf("\x1B[%dm",cor);

        //Espaçamento lateral
        if (espaco > tamanho){
            for (unsigned int s=0;s<espaco-tamanho;s++){
                printf("%s",fundo.c_str());
                colunas--;
            }
        }
        //Se mensagem sumir da tela, pular para próxima linha
        if (colunas > 0){
            //Caractere inicial, caso mensagem esteja sumindo
            unsigned int stcar = 0;
            if (espaco <= tamanho)
                stcar = mensagem_tamanho - (espaco/caractere_tamanho) - 1;

            //Para cada caractere
            for (unsigned int c=stcar;c<mensagem_tamanho;c++){
                //Posição inicial, caso mensagem esteja sumindo
                unsigned int stpos = 0;
                if ((espaco <= tamanho) && (c == stcar))
                    stpos = caractere_tamanho - (espaco % caractere_tamanho);
                
                //Para cada coluna em cada caractere
                if (imprimirCaractere(texto[c],y,&colunas,stpos))
                    break;
            }
        }
        //Espaçamento lateral
        if (espaco <= limite){
            for (unsigned int s=0;s<limite-espaco;s++){
                printf("%s",fundo.c_str());
                colunas--;
                if (colunas <= 0)
                    break;
            }
        }
        if (cor > 30)
            printf("\033[0m");
        printf("\n");
    }
}

void imprimirBorda(int tamanho){
    printf("\x1B[%dm",cor_borda);
    for (unsigned int i=0;i<tamanho;i++)
        printf("%s",borda.c_str());
    printf("\033[0m\n");
}

int main(int argc, char* argv[]){
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    //Exibir ajuda
    if ((argc == 1) || (argc == 2 && argv[1][0] == '-')){
        printf("Bashsays v1.0 by Ewerton Bramos\n");
        printf("   bashsays [-args] [args value] \"message\"\n\n");
        printf("   -i: run the message forever.\n");
        printf("   -r: run the message from left to right.\n");
        printf("   -s: run the message at a slower pace. Can be repeated up to 5 times.\n");
        printf("   -f: run the message at a faster pace. Can be repeated up to 5 times.\n");
        printf("   -q: show the message multiple times at the screen. Can be repeated up to 2 times.\n");
        printf("   -c [1-16]: change default/first text color.\n");
        printf("   -b [1-16]: change border color.\n");
        printf("      1: black         9: bright black\n");
        printf("      2: red          10: bright red\n");
        printf("      3: green        11: bright green\n");
        printf("      4: yellow       12: bright yellow\n");
        printf("      5: blue         13: bright blue\n");
        printf("      6: magenta      14: bright magenta\n");
        printf("      7: cyan         15: bright cyan\n");
        printf("      8: white        16: bright white\n");
        printf("   -t [0|1+]: color blinking timeout.\n");
        printf("   -p [text symbol] [bg symbol] [border symbol]: change message symbols.\n\n");
        printf("   Exit the program with CTRL^C or Q.\n\n");
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
                case 'r':
                    paraDireita = true;
                    break;
                case 'q':
                    mensagens_quantidade++;
                    break;
                case 's':
                    tempo++;
                    if (tempo > 10)
                        tempo = 10;
                    break;
                case 'f':
                    tempo--;
                    if (tempo < 1)
                        tempo = 1;
                    break;
                case 'c':
                    argst++;
                    cor = *argv[argst] - '0';
                    if (cor > 8)
                        cor += 81;
                    else
                        cor += 29;
                    break;
                case 'b':
                    argst++;
                    cor_borda = *argv[argst] - '0';
                    if (cor_borda > 8)
                        cor_borda += 81;
                    else
                        cor_borda += 29;
                    break;
                case 't':
                    argst++;
                    cor_tempo = *argv[argst] - '0';
                    break;
                case 'p':
                    argst++;
                    simbolo = argv[argst][0];
                    if (strlen(argv[argst]) > 1)
                        fundo = argv[argst][1];
                    if (strlen(argv[argst]) > 2)
                        borda = argv[argst][2];
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
        printf("Display message is missing.\n");
        return 0;
    }

    //Exibir mensagem
    string texto_mensagem = " ";
    for (unsigned int i=argst;i<argc;i++)
        texto_mensagem += string(argv[i]) + string(" ");
    
    //Obter mensagem a partir de um comando pipe
    /*vector<char> someData(100, '-');
    cin.read(&someData.front(), someData.size());
    someData.back() = '\0';
    printf("DADOS: " << someData.data()[0] << endl;*/

    unsigned int texto_tamanho = caractere_tamanho * texto_mensagem.length();
    unsigned int texto_x = w.ws_col + texto_tamanho;
    unsigned int espacamento_topo = (w.ws_row/2) - (caractere_altura/2);
    unsigned int cor_espera = cor_tempo;

    if (mensagens_quantidade > 3)
        mensagens_quantidade = 3;
    for (unsigned int i=1;i<mensagens_quantidade;i++)
        espacamento_topo -= (caractere_altura/2);

    if (borda != "")
        espacamento_topo -= 2;
    if (paraDireita)
        texto_x = 1;
    
    printf("\x1B[2J\x1B[H");
    
    while (texto_x > 0){
        if (debug){
            printf("parameters: %d; sleep: %d",(strlen(argv[1]) - 1),tempo);
            printf("; color: %d; color timeout: %d",cor,cor_tempo);
            printf("; window width: %d; text size: %d\n",w.ws_col,texto_tamanho);
        }

        //Espaçamento superior
        for (unsigned int s=0;s<espacamento_topo;s++)
            printf("\n");
        
        //Bordas e mensagem
        for (unsigned int i=0;i<mensagens_quantidade;i++){
            if (borda != "")
                imprimirBorda(w.ws_col); 
            
            unsigned int esp;
            if (i == 1)
                esp = (w.ws_col + texto_tamanho) - texto_x;
            else
                esp = texto_x;
            imprimirMensagem(texto_mensagem,esp,w.ws_col);
        }
        if (borda != "")
            imprimirBorda(w.ws_col);

        //Alterar posição
        usleep(tempo * 10000);
        if (paraDireita){
            texto_x++;
            if (texto_x > w.ws_col + texto_tamanho)
                texto_x = 0;
        }else
            texto_x--;

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
        if (texto_x <= 0 && infinitamente){
            if (paraDireita)
                texto_x = 1;
            else
                texto_x = w.ws_col + texto_tamanho;
        }
        printf("\x1B[2J\x1B[H"); //Limpar terminal

        //Limpar o programa
        if (kbhit())
            texto_x = 0;
    }

    return 0;
}