#include <iostream>
#include <fstream>
#include <dirent.h>
#include <vector>
#include <iterator>
#include "md5.h"
#include <mpi.h>
#include <string>
#include <stdio.h>

std::string calcularHash (std::string nomeArq, std::string nomeDir);
void percorrerDir (std::string nomeDir, std::string *vetBinarios, int *i);


void mergeSort(std::string *ptr, int p, int r);
void merge(std::string *ptr, int p, int q, int r);



int main(int argc, char *argv[]){
  MPI::Init(argc, argv);   
  int p;
  int id;
  int ierr;
  std::string vetBinarios[10000];
  int var = 0;
  int *ptr = &var;
  std::string diretorio;
  //if (id == 0)
    //std::cin>>diretorio; 
  diretorio = "/mnt/e/Qt/Projects/ARQ2/MPI/source";  
  percorrerDir(diretorio, vetBinarios, ptr);

  mergeSort(vetBinarios, 0 , *ptr);

  std::string hashs[10000];  
   
 
//  Get the number of processes.
  p = MPI::COMM_WORLD.Get_size();
  

//  Get the individual process ID.
  id = MPI::COMM_WORLD.Get_rank();  

  
 
  for (int k = (*ptr)/p * id; k < (*ptr)/p; k++){
    if (k == (*ptr)/p * id)
      std::cout<<"processo " <<id <<" esta trabalhando...\n";
    if (vetBinarios[k] != ""){
      std::string s = vetBinarios[k];
      hashs[k] = MD5(s.c_str()).hexdigest();
      std::cout<<k <<"(" <<id <<") = " <<hashs[k]<<std::endl;     
    }
  }
  
  

  MPI::Finalize();


  
  
/*
  //gerar sequencialmente vetor com hashs percorrendo o vetor de strings dos binarios
  for (int k = 0, m = 0; k <= *ptr; k++, m++){
    if(vetBinarios[k] != ""){
      std::string s = vetBinarios[k];
      hashs[k] = MD5(s.c_str()).hexdigest();
      std::cout<<hashs[k]<<std::endl;     
    }
  }
*/
    
  return 0;
}













std::string calcularHash (std::string nomeArq, std::string nomeDir){
  std::string caminho = nomeDir+"/"+nomeArq;
  // abrir o arquivo:
  std::streampos fileSize;
  std::string binario;
  std::ifstream file;
  file.open(caminho);

  if(file.is_open()){
  // ver tamanho do arquivo:
    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    
    if(fileSize <= 0)
      std::cout<<"arquivo vazio"<<std::endl;
    

    // ler dados binarios para vetor:
    std::vector<unsigned char> fileData(fileSize);
    file.read((char*) &fileData[0], fileSize);
    //transformar binario do arquivo em string
    for (int i=0;i<fileData.size();i++){
    //cout<<fileData.at(i)<<endl;
      binario += fileData.at(i);
    }
    file.close();
  }

  else
    std::cout<<"impossivel abrir"<<std::endl;
  
  return binario;
}

void percorrerDir (std::string nomeDir, std::string *vetBinarios, int *i){
  //abrir diretorio
  DIR *dir;
  struct dirent *ent;

  if((dir = opendir (nomeDir.c_str())) != NULL){
    /* percorrer todos os arquivos dentro de um diretorio*/
    while ((ent = readdir (dir)) != NULL){
      //se for arquivo guardar binario na matriz se diretorio entrar nele
      if(ent->d_type == DT_DIR ){
        //verificar se é um diretório válido
        if(ent->d_name[0] != '.'){
          //concatenar caminho
          percorrerDir(nomeDir+ '/'+ent->d_name, vetBinarios, i);
        }
      }

      else{
        //pegar o binario do arquivo e armazenar na matriz
        std::string binario = calcularHash(ent->d_name,nomeDir);
        *(vetBinarios + *i) = binario;        
        (*i)++;
      }
    }
    closedir (dir);
  }

  else{
    /* impossivel abrir diretorio */
    perror ("");
  }
}





void mergeSort(std::string *ptr, int p, int r){
    if (p < r){
        int q = (p + r)/2;
        mergeSort(ptr, p, q);
        mergeSort(ptr, q + 1, r);
        merge(ptr, p, q, r);
    }
}

void merge(std::string *ptr, int p, int q, int r){
    int nL = q - p + 1;
    int nR = r - q;
    std::string *L = new std::string[nL];
    std::string *R = new std::string[nR];
    for (int i = 0; i < nL; i++)
        *(L + i) = *(ptr + p + i);
    for (int j = 0; j < nR; j++)
        *(R + j) = *(ptr + q + 1 + j);

    int i = 0;
    int j = 0;
    int k = p;

    while(i < nL && j < nR){
        if (*(L + i) <= *(R + j)){
            *(ptr + k) = *(L + i);
            i++;
        }

        else{
            *(ptr + k) = *(R + j);
            j++;
        }

        k++;
    }

    while (i < nL){
        *(ptr + k) = *(L + i);
        i++;
        k++;
    }

    while (j < nR){
        *(ptr + k) = *(R + j);
        j++;
        k++;
    }

    delete []L;
    delete []R;
}