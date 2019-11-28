#include <iostream>
#include <fstream>
#include <dirent.h>
#include <vector>
#include <iterator>
#include "md5.h"

using namespace std;

//vetor de strings dos binarios
string M[10];
int i=0;

string calcularHash (string nomeArq, string nomeDir) {
    string caminho = nomeDir+"/"+nomeArq;
        // abrir o arquivo:
        std::streampos fileSize;
        string binario;
        std::ifstream file;
        file.open(caminho);

        if ( file.is_open() ) {

            // ver tamanho do arquivo:
            file.seekg(0, std::ios::end);
            fileSize = file.tellg();
            file.seekg(0, std::ios::beg);
            if ( fileSize <= 0 ) {
                cout<<"arquivo vazio"<<endl;
            }

            // ler dados binarios para vetor:
            std::vector<unsigned char> fileData(fileSize);
            file.read((char*) &fileData[0], fileSize);

            //transformar binario do arquivo em string
            for (int i=0;i<fileData.size();i++) {
                //cout<<fileData.at(i)<<endl;
                binario += fileData.at(i);
            }
            file.close();
        }
        else {

            cout<<"impossivel abrir"<<endl;
        }

        return binario;
}

void percorrerDir (string nomeDir) {

    //abrir diretorio
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir (nomeDir.c_str())) != NULL) {
      /* percorrer todos os arquivos dentro de um diretorio*/
      while ((ent = readdir (dir)) != NULL) {

        //se for arquivo guardar binario na matriz se diretorio entrar nele
          if(ent->d_type == DT_DIR ) {

              //verificar se é um diretório válido
              if(ent->d_name[0] != '.'){

                  //concatenar caminho

                  percorrerDir(nomeDir+ '/'+ent->d_name);
              }

          } else {

              //pegar o binario do arquivo e armazenar na matriz
              string binario = calcularHash(ent->d_name,nomeDir);

              M[i] = binario;
              i++;
              if (i>=10) {
                  if (i>=10) cout<<"matriz cheia"<<endl;
              }

          }
      }
      closedir (dir);
    } else {
      /* impossivel abrir diretorio */
      perror ("");

    }
}

int main()
{

    int m=0;
    percorrerDir("teste");
    string hashs[10];

    //gerar vetor com hashs percorrendo o vetor de strings dos binarios
    for (int k=0;k<=i; k++) {

            if(M[k] != ""){
            string s = M[k];
            hashs[m] = MD5(s.c_str()).hexdigest();
            //cout<<hashs[m]<<endl;
            m++;

        }
    }

    return 0;
}
