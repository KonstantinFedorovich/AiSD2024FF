#include <iostream>
#include "graph.h"     // Заголовочный файл для работы с графом
#include "set_union.h" // Заголовочный файл для работы с объединением множеств (set union)

struct edge_pair {
    int x, y;      // Вершины, которые соединяет ребро
    int weight;    // Вес ребра
};

// Преобразует граф в массив ребер для последующей обработки
void to_edge_array(graph *g, edge_pair e[]) {
    int m = 0;     // Индекс для массива ребер
    edgenode *p;   // Указатель на текущее ребро

    // Проходит по всем вершинам графа
    for (int i = 1; i <= g->nvertices; i++) {
        p = g->edges[i];   // Получает список смежных вершин для текущей вершины i
        while (p != NULL) {   // Проходит по всем ребрам для вершины i
            if (p->y > i) {   // Чтобы избежать дублирования ребер, обрабатываются только ребра, где вторая вершина больше первой
                e[m].x = i;          // Первая вершина ребра
                e[m].y = p->y;       // Вторая вершина ребра
                e[m].weight = p->weight; // Вес ребра
                m = m + 1;           // Переход к следующему ребру
            }
            p = p->next;  // Переход к следующему ребру в списке смежных вершин
        }
    }
}

// Функция для сравнения весов ребер (используется в qsort)
int weight_compare(const void *x, const void *y) {
    edge_pair *_x = (edge_pair*)x;   // Преобразование указателя на edge_pair для первого аргумента
    edge_pair *_y = (edge_pair*)y;   // Преобразование указателя на edge_pair для второго аргумента

    // Возвращает -1, если вес первого ребра меньше второго
    if (_x->weight < _y->weight) {
        return(-1);
    }

    // Возвращает 1, если вес первого ребра больше второго
    if (_x->weight > _y->weight) {
        return(1);
    }

    // Возвращает 0, если веса ребер равны
    return(0);
}

// Реализация алгоритма Краскала для нахождения MST (минимального остовного дерева)
void kruskal(graph *g) {
    set_union s;   // Структура для представления объединения множеств (для отслеживания связных компонент)
    edge_pair e[MAXV+1];  // Массив ребер, максимальная длина которого равна количеству вершин + 1

    // Инициализация структуры объединения множеств для вершин графа
    set_union_init(&s, g->nvertices);

    std::cout << "initialized set union\n";

    // Преобразование графа в массив ребер
    to_edge_array(g, e);

    // Сортировка массива ребер по возрастанию весов с использованием функции qsort
    qsort(&e, g->nedges, sizeof(edge_pair), &weight_compare);

    // Проход по отсортированному массиву ребер
    for (int i = 0; i < (g->nedges); i++) {
        print_set_union(&s);  // Печатает текущее состояние объединения множеств

        // Проверка, находятся ли вершины ребра в одной компоненте
        if (!same_component(&s, e[i].x, e[i].y)) {
            // Если не в одной компоненте, добавляем ребро в минимальное остовное дерево
            std::cout << "edge (" << e[i].x << ' ' <<  e[i].y << ") of weight " << e[i].weight << " in MST\n";

            // Объединяем компоненты, содержащие эти вершины
            union_sets(&s, e[i].x, e[i].y);
        }
    }
}

// Точка входа программы
int main(void) {
    graph g;   // Структура графа

    // Чтение графа из ввода, второй аргумент false указывает на неориентированный граф
    read_graph(&g, false);

    // Печать графа (список смежности)
    print_graph(&g);

    // Запуск алгоритма Краскала для нахождения минимального остовного дерева
    kruskal(&g);

    // Вывод по завершению алгоритма
    printf("Out of Kruskal\n");

    return 0;   // Завершение программы
}
