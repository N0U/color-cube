#ifdef __cplusplus
extern "C" {
#endif

// Include guard
#ifndef CUBE_H
#define CUBE_H

#define MAX_EDGES 24
#define MAX_FACES 6
#define EDGES_PER_FACE 4

struct edge {
  unsigned short start;
  unsigned short end;
  unsigned char length;
  char reverse;
};

struct face {
  const struct edge *edges[EDGES_PER_FACE];
  unsigned short length;
};

struct cubeInfo {
    unsigned short length;
};

void initCubeApi();
const struct cubeInfo* getCubeInfo();

const struct edge* getEdge(int edge);
int getPixelOnEdge(const struct edge* edge, int offset);

const struct face* getFace(int face);
int getPixelOnFace(const struct face* face, int offset, int *edgePtr);

#endif /* CUBE_H */

#ifdef __cplusplus
}
#endif
