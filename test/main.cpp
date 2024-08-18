#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include "../src/lineards.h"

using namespace std;

void verificar(LINEAR_DS * lds, vector<int> & vec, FILE * output);
void debugfmt(FILE * out, void *v);
inline void test_lds(LINEAR_DS * lds, vector<int> & vec, FILE * output);
inline void test_it(LINEAR_DS * lds, vector<int> & vec, FILE * output);

int main() {

    //srand(time(0)); // inicializa o gerador de números aleatórios
    FILE * log = fopen("log.txt", "a");
    if (log == NULL) {
        cout << "ERRO: noa foi possivel abrir o arquivo de log." << endl;
        return 1;
    }
    int num_operations, type, interf;
    cout << "Quantas operações deseja executar? ";
    cin >> num_operations;

    cout << "Que tipo de estrutura de dados deseja testar ("
         << LDS_VECTOR << "-LDS_VECTOR, "
         << LDS_LINKED_LIST << "-LDS_LINKED_LIST)? ";
    cin >> type;

    LINEAR_DS *lds;
    switch(type) {
        case LDS_VECTOR:
        lds = lds_new_vector(10, sizeof(int)); // cria uma estrutura de dados da biblioteca
        break;
        case LDS_LINKED_LIST:
        lds = lds_new_list(sizeof(int)); // cria uma estrutura de dados da biblioteca
        break;
        default:
        cout << "Tipo invalido!" << endl;
        return 1;
    }

    cout << "Que tipo de interface deseja testar (1-LINEAR_DS, 2-LDS_ITERATOR)? ";
    cin >> interf;
    if (interf != 1 && interf != 2) {
        cout << "Interface invalida!" << endl;
        return 1;
    }

    lds_debug(lds, log, debugfmt);

    vector<int> vec; // cria um vetor da classe std::vector

    for (int i = 0; i < num_operations; i++) {
        fprintf(log,"[%d]: ", i);

        if (interf == 1) {
            test_lds(lds, vec, log);
        }
        else {
            test_it(lds, vec, log);
        }
    }

    lds_free(lds);

    fprintf(log, "\n\nOperacoes: %d\n", num_operations);
    fprintf(log, "Tipo de estrutura: %s\n", (type == LDS_VECTOR ? "LDS_VECTOR" : "LDS_LINKED_LIST"));
    fprintf(log, "Interface: %s\n", (interf == 1 ? "LINEAR_DS" : "LDS_ITERATOR"));

    fclose(log);

    return 0;
}

void test_lds(LINEAR_DS *lds, vector<int> & vec, FILE * output) {
    int option = rand() % 6 + 1; // sorteia uma opção entre 1 e 4

    switch (option) {
        case 6:
        case 1: { // adicionar elemento
            fprintf(output,"Adicionando elemento\n");
            int value = rand() % 100; // sorteia um valor entre 0 e 99
            int position = rand() % (lds_size(lds) + 1); // sorteia uma posição entre 0 e o tamanho atual da estrutura
            fprintf(output,"valor: %d posicao %d ... ", value, position);
            lds_insert(lds, position, &value);
            fprintf(output,"Inserido na estrutura... ");
            vec.insert(vec.begin() + position, value);
            fprintf(output,"Inserido no vector.\n");
            fprintf(output,"Adicionado elemento %d na posicao %d\n", value, position);
            verificar(lds, vec, output);
            break;
        }
        case 5:
        case 2: { // remover elemento
            fprintf(output,"Removendo elemento\n");
            if (lds_size(lds) == 0) break; // não há elementos para remover
            int position = rand() % lds_size(lds); // sorteia uma posição entre 0 e o tamanho atual da estrutura
            int removed_lds, removed_vec;
            lds_remove(lds, position, &removed_lds);
            removed_vec = vec[position];
            vec.erase(vec.begin() + position);
            fprintf(output,"Removido elemento %d na posicao %d\n", removed_lds, position);
            verificar(lds, vec, output);
            if (removed_lds!= removed_vec) {
                fprintf(output,"Erro: valores removidos diferentes!\n");
                exit(1);
            }
            break;
        }
        case 3: { // consultar elemento
            fprintf(output,"Consultando elemento\n");
            if (lds_size(lds) == 0) break; // não há elementos para consultar
            int position = rand() % lds_size(lds); // sorteia uma posição entre 0 e o tamanho atual da estrutura
            int value_lds, value_vec;
            lds_get(lds, position, &value_lds);
            value_vec = vec[position];
            fprintf(output,"Valor na posição %d: %d (lds) vs %d (vec)\n", position,value_lds, value_vec);
            verificar(lds, vec, output);
            if (value_lds!= value_vec) {
                fprintf(output,"Erro: valores diferentes!\n");
                exit(1);
            }
            break;
        }
        case 4: { // alterar valor de um elemento
            if (lds_size(lds) == 0) break; // não há elementos para alterar
            int position = rand() % lds_size(lds); // sorteia uma posição entre 0 e o tamanho atual da estrutura
            int new_value = rand() % 100; // sorteia um novo valor entre 0 e 99
            fprintf(output,"Alterado valor na posição %d para %d\n", position, new_value);
            vec[position] = new_value;
            lds_set(lds, position, &new_value);verificar(lds, vec, output);
            break;
        }
    }
}

static size_t position = 0;
void test_it(LINEAR_DS * lds, vector<int> & vec, FILE * output) {
    LDS_ITERATOR * it = lds_iterator(lds);

    int option = rand() % 6 + 1; // sorteia uma opção entre 1 e 6

    switch (option) {
        case 1: { // adicionar elemento
            fprintf(output,"Adicionando elemento\n");
            int value = rand() % 100; // sorteia um valor entre 0 e 99
            fprintf(output,"valor: %d posicao: %d...\n",value,position);
            lds_it_add(it, &value);
            fprintf(output,"Inserido na estrutura... ");
            vec.insert(vec.begin() + position, value);
            fprintf(output,"Inserido no vector.");
            fprintf(output,"Adicionado elemento %d na posicao %d\n", value, position);
            verificar(lds, vec, output);
            break;
        }
        case 2: { // remover elemento
            fprintf(output,"Removendo elemento\n");
            int removed_lds, removed_vec;
            if ( lds_it_remove(it, &removed_lds) == LDS_SUCCESS) {
                removed_vec = vec[position];
                vec.erase(vec.begin() + position);
                fprintf(output,"Removido elemento %d na posicao %d\n", removed_lds, position);
                verificar(lds, vec, output);
                if (removed_lds!= removed_vec) {
                    fprintf(output,"Erro: valores removidos diferentes!\n");
                    exit(1);
                }
            }
            else if (position == lds_size(lds)) {
                fprintf(output,"Nao e possivel remover elemento quando o iterador esta apos o ultimo.\n");
                verificar(lds, vec, output);
            }
            else {
                fprintf(output,"ERRO: nao foi possivel remover o elemento da posicao %d/%d.\n", position, lds_size(lds));
                verificar(lds, vec, output);
            }
            break;
        }
        case 3: { // consultar elemento
            fprintf(output,"Consultando elemento\n");
            int value_lds, value_vec;
            if (lds_it_get(it, &value_lds) == LDS_SUCCESS) {
                value_vec = vec[position];
                fprintf(output,"Valor na posição %d: %d (lds) vs %d (vec)\n", position, value_lds, value_vec);
                verificar(lds, vec, output);
                if (value_lds!= value_vec) {
                    fprintf(output,"Erro: valores diferentes!\n");
                    exit(1);
                }
            }
            else if (position == lds_size(lds)) {
                fprintf(output,"Nao e possivel consultar elemento quando o iterador esta apos o ultimo.\n");
            }
            else {
                fprintf(output,"ERRO: nao foi possivel consultar o elemento da posicao %d/%d.\n", position, lds_size(lds));
            }
            verificar(lds, vec, output);
            break;
        }
        case 4: { // alterar valor de um elemento
            if (lds_size(lds) == 0) break; // não há elementos para alterar
            int new_value = rand() % 100; // sorteia um novo valor entre 0 e 99
            fprintf(output,"Alterando elemento na posicao %d para %d\n", position, new_value);
            if ( lds_it_set(it, &new_value) == LDS_SUCCESS) {
                vec[position] = new_value;
                fprintf(output,"Alterado valor na posição %d para %d\n", position, new_value);
            }
            else if (position == lds_size(lds)) {
                fprintf(output,"Nao e possivel alterar elemento quando o iterador esta apos o ultimo.\n");
            }
            else {
                fprintf(output,"ERRO: nao foi possivel alterar o elemento da posicao %d/%d.\n", position, lds_size(lds));
            }
            verificar(lds, vec, output);
            break;
        }
        case 5: { // Next
            fprintf(output,"Next\n");
            lds_return_t r = lds_it_next(it);
            if ( (r == LDS_SUCCESS) && (position + 1 <= lds_size(lds)) ) {
                position++;
                if (lds_it_position(it) != position) {
                    fprintf(output,"ERRO: nao conseguiu deslocar para a posicao %d. Posicao do iterador: %d.\n", position, lds_it_position(it));
                         exit(1);
                }
                fprintf(output,"Deslocado para a posição %d\n",position);
                fprintf(output,"Iterador na posição %d\n",lds_it_position(it));
            }
            else if ( (r == LDS_POS_ERR) && (position + 1 > lds_size(lds)) ) {
                fprintf(output,"Nao foi possivel deslocar, porque ja esta no fim.\n");
            }
            else {
                fprintf(output,"Falha no deslocamento.\n");
                fprintf(output,"Posicao: %d\n", position);
                fprintf(output,"Tamanho da estrutura: %d\n", lds_size(lds));
                exit(1);
            }
            verificar(lds, vec, output);
            break;
        }
        case 6: { // Go
            fprintf(output,"Go\n");
            size_t new_position = rand() % (lds_size(lds) + 1) * 1.1;
            fprintf(output,"Tentando deslocar iterador para posicao %d.\n", new_position);
            if (lds_it_go(it, new_position) == LDS_SUCCESS) {
                if (new_position > vec.size()) {
                    fprintf(output,"ERRO: deslocamento do iterador permitiu posicao invalida.\n");
                    fprintf(output,"Posicao anterior: %d\n", position);
                    fprintf(output,"Nova posicao: %d\n", new_position);
                    fprintf(output,"Tamanho do vetor: %d\n", vec.size());
                    fprintf(output,"Tanho da estrutura de dados: %d\n", lds_size(lds));
                    exit(1);
                }
                position = new_position;
            }
            else if (new_position > vec.size()) {
                fprintf(output,"Nao foi possivel deslocar.\n");
                fprintf(output,"Posicao atual: %d/%d\n", position, vec.size());
            }
            else {
                fprintf(output,"ERRO: Nao permitiu deslocar para a posicao %d/%d\n", new_position, vec.size());
            }
            break;
        }
    }

    if (lds_it_position(it) != position) {
        fprintf(output,"ERRO: posicao do iterador (%d) nao e' a esperada (%d). Tamanho: %d\n",
                lds_it_position(it), position, lds_size(lds));
        exit(1);
    }
}

void verificar(LINEAR_DS * lds, vector<int> & vec, FILE * output) {
    LDS_ITERATOR * it = lds_iterator(lds);
    size_t p = lds_it_position(it);

    lds_debug(lds, NULL, NULL);
    fprintf(output,"\nLINEAD_DS: {");
    for(int i = 0, tam=lds_size(lds); i<tam; i++) {
        int e;
        lds_get(lds, i, &e);
        fprintf(output," %3d", e);
    }
    fprintf(output,"}\n");

    fprintf(output,"\nvector<int>: {");
    for(int i = 0, tam=vec.size(); i<tam; i++) {
        fprintf(output," %3d", vec[i]);
    }
    fprintf(output,"}\n");

    for(int i = 0, tam=lds_size(lds); i<tam; i++) {
        int e;
        lds_get(lds, i, &e);
        if (e != vec[i]) {
            fprintf(output,"ERRO: elementos diferentes na posicao %d\n", i);
            exit(1);
        }
    }
    lds_debug(lds, output, debugfmt);
    lds_it_go(it, p);
}

void debugfmt(FILE * out, void *v) {
    fprintf(out, "%d ", *(int *)v);
}
