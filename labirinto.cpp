// Daniel Medeiros Alvim - 22/08/2021
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <string>

using namespace std;

// Ler as linhas de um arquivo
vector<string> ler_linhas_arquivo(string nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    vector<string> linhas;
    string linha;
    
    while (getline(arquivo, linha)) {
        linhas.push_back(linha);
        cout << linha << endl;
    }

    return linhas;
}

// Mostra o labirinto 
void mostrar_labirinto(vector<vector<int>> labirinto, int tamanho) {
  for (int i = 0; i < tamanho; i++) {
    for (int j = 0; j < tamanho; j++) {
      if(labirinto[i][j] == -1) {
        cout << "#";
      } else if (labirinto[i][j] == 0) {
        cout << " ";
      } else {
        cout << labirinto[i][j];
      }
    }
    cout << endl;
  }
}

void mostrar_labirinto_final(vector<vector<int>> labirinto, int tamanho) {
  for (int i = 0; i < tamanho; i++) {
    for (int j = 0; j < tamanho; j++) {
      if(labirinto[i][j] == -1) {
        cout << "#";
      } else if (labirinto[i][j] == -2) {
        cout << "X";
      } else {
        cout << " ";
      }
    }
    cout << endl;
  }
}

vector<vector<int>> instanciar_labirinto(vector<string> linhas, int tamanho) {
  vector<vector<int>> labirinto;

  for (int i = 0; i < tamanho; i++) {
    vector<int> linha;
    for (int j = 0; j < tamanho; j++) {
      int representacao = linhas[i][j] == '#' ? -1 : 0;
      linha.push_back(representacao);
      cout << representacao;
    }
    cout << endl;
    labirinto.push_back(linha);
  }

  return labirinto;
}

/* 
Pseudocódigo utilizado:

  L[2, 2] <- 1
  insira a posição (2, 2) em uma fila F vazia
  enquanto F não estiver vazia faça
    remova uma posição (i, j) da fila F
    para cada posição (x, y) adjacente a (i, j) tal que L[x, y] = 0 faça
      L[x, y] <- L[i, j] + 1
      insira a posição (x, y) na fila F
*/
vector<vector<int>> anotar(vector<vector<int>> labirinto, int tamanho) {
  queue<pair<int, int>> fila;
  labirinto[1][1] = 1;
  fila.push(pair<int, int>(1, 1));

  while (!fila.empty()) {
    pair<int, int> posicao = fila.front(); // Ponto (i, j)
    fila.pop();

    // Posições adjacentes á posição (i, j) removida da fila F
    vector<pair<int, int>> adjacentes = {
      { posicao.first - 1, posicao.second },
      { posicao.first + 1, posicao.second },
      { posicao.first, posicao.second - 1 },
      { posicao.first, posicao.second + 1 }
    };

    for(int i = 0; i < adjacentes.size(); i++) {
      // Caso ela seja igual á zero, ou seja, um caminho possível:
      if(labirinto[adjacentes[i].first][adjacentes[i].second] == 0) {
        // Alterando no labirinto a posição (x, y) para ser igual a posição anterior + 1
        labirinto[adjacentes[i].first][adjacentes[i].second] = labirinto[posicao.first][posicao.second] + 1;
        fila.push(adjacentes[i]); // Adicionando a posição (x, y) na fila
      }
    }
  }

  return labirinto;
}

/* 
Pseudocódigo utilizado:

  se L[n-1, n-1] = 0 então não existe um caminho, pare a execução da função
  insira a posição (n-1, n-1) em uma pilha P vazia
  enquanto a posição (2, 2) não estiver no topo da pilha P faça
    seja (i, j) a posição existente no topo da pilha
    encontre uma posição (x, y) adjacente a (i, j) tal que L[x, y] = L[i, j] – 1
    insira a posição (x, y) na pilha P
    para cada posição (x, y) adjacente a (i, j) tal que L[x, y] = 0 faça
      L[x, y] <- L[i, j] + 1

  Não entendi o sentido da ultima parte onde a posição adjacente a (i, j) tal que L[x, y] = 0 recebe 1 já que nenhum ponto vai ter valor 0
*/
vector<vector<int>> extrair(vector<vector<int>> labirinto, int tamanho) {
  if(labirinto[tamanho - 2][tamanho - 2] == 0) {
    cout << "Não existe um caminho!";
    return labirinto;
  }

  stack<pair<int, int>> pilha;
  pilha.push(pair<int, int>(tamanho - 2, tamanho - 2));

  while(pilha.top() != pair<int, int>(1, 1)) {
    pair<int, int> posicao = pilha.top();

    vector<pair<int, int>> adjacentes = {
      { posicao.first - 1, posicao.second },
      { posicao.first + 1, posicao.second },
      { posicao.first, posicao.second - 1 },
      { posicao.first, posicao.second + 1 }
    };

    // Encontrando uma posição (x, y) adjacente a (i, j) tal que L[x, y] = L[i, j] – 1
    for(int i = 0; i < adjacentes.size(); i++) {
      if(labirinto[adjacentes[i].first][adjacentes[i].second] == labirinto[posicao.first][posicao.second] - 1) {
        pilha.push(adjacentes[i]);
      }
    }
  }

  // Marcando como -2 as coordenadas do menor caminho, para substitui-los pelo X na hora de imprimir o labirinto.
  while(!pilha.empty()) {
    pair<int, int> posicao = pilha.top();
    pilha.pop();

    for(int i = 0; i < tamanho; i++) {
      for(int j = 0; j < tamanho; j++) {
        if(i == posicao.first && j == posicao.second) 
          labirinto[i][j] = -2;
      }
    }
  }

  return labirinto;
}


int main() {
  string nomeArquivo;
  cout << "Digite o nome do arquivo: ";
  cin >> nomeArquivo;

  cout << endl << "Conteudo do arquivo " << nomeArquivo << ": " << endl;
  vector<string> linhas = ler_linhas_arquivo(nomeArquivo);

  // Obtendo o tamanho das linhas e colunas da matriz
  const int tamanho = linhas.size();

  // Usando vector pois meu compilador não permite declarar uma matriz usando variável como tamanho, o ideal seria int labirinto[tamanho][tamanho]
  cout << endl << "Representacao do labirinto: " << endl;
  vector<vector<int>> labirinto = instanciar_labirinto(linhas, tamanho);

  cout << endl << "Labirinto: " << endl;
  mostrar_labirinto(labirinto, tamanho);

  labirinto = anotar(labirinto, tamanho);
  cout << endl << "Anotacao dos passos: " << endl;
  mostrar_labirinto(labirinto, tamanho);

  if(labirinto[tamanho - 2][tamanho - 2] == -1) {
    cout << endl << "Nao e possivel achar um caminho pois o ponto de saida e uma parede.";
    return -1;
  }

  labirinto = extrair(labirinto, tamanho);
  cout << endl << "Melhor caminho: " << endl;
  mostrar_labirinto_final(labirinto, tamanho);

  return 0;
}