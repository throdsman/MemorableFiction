
#include "Data/unionFind.h"

int main()
{
    unionFind uf = unionFind(12, 2);

    // 0 is jpg
    uf.unite(0, 2);
    uf.unite(0, 3);
    uf.unite(0, 4);
    uf.unite(0, 5);
    uf.unite(0, 6);
    uf.unite(0, 7);
    // 1 is mp3
    uf.unite(1, 8);
    uf.unite(1, 9);
    uf.unite(1, 10);
    uf.unite(1, 11);

    uf.mostrarPadres();
    return 0;
}