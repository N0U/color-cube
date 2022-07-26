#include <stdlib.h>
#include "cube.h"

// char для экономии места (аш в 4 раза!) TEC!!!
// Знак означает направление отрезка ленты
static const char EDGES_DATA[] = {
  // 1
  15,15,-15,-15,
  // 2
  14,-14,-14,14,
  // 3
  15,15,-15,15,
  // 4
  14,-14,14,14,
  // 5
  14,14,-14,14,
  // 6
  14,-14,14,14
};

static const char FACES_DATA[MAX_FACES][EDGES_PER_FACE] = {
  { 0, 1, 2, 3 },
  { 4, 5, 6, 7 },
  { 8, 9, 10, 11 },
  { 12, 13, 14, 15 },
  { 16, 17, 18, 19 },
  { 20, 21, 22, 23 }
};

struct edge cubeEdges[MAX_EDGES];
struct face cubeFaces[MAX_FACES];
struct cubeInfo cubeInfo;

void initEdges() {
  int start = 0;
  for(int i = 0; i < MAX_EDGES; i++) {
    struct edge *edge = &cubeEdges[i];
    int len = EDGES_DATA[i];
    edge->start = start;
    edge->end = start + abs(len);
    edge->length = abs(len);
    edge->reverse = (len < 0);
    start += abs(len);
  }
}

void initFaces() {
  for(int i = 0; i < MAX_FACES; i++) {
    struct face *face = &cubeFaces[i];
    int len = 0;
    for(int j = 0; j < EDGES_PER_FACE; j++) {
      const struct edge *edge = getEdge(FACES_DATA[i][j]);
      face->edges[j] = edge;
      len += edge->length;
    }
    face->length = len;
  }
}

void initCubeApi() {
  initEdges();
  initFaces();

  cubeInfo.length = cubeEdges[MAX_EDGES - 1].end + 1;
}

const struct cubeInfo* getCubeInfo() {
  return &cubeInfo;
}

const struct edge* getEdge(int edge) {
  return &cubeEdges[edge];
}

int getPixelOnEdge(const struct edge* edge, int offset) {
  char backward = (offset < 0);
  int i = abs(offset) - 1;
  if(i > edge->length) {
    return -1;
  }

  // С начала в конец
  if(edge->reverse == backward) {
    return edge->start + i;
  } else { // С конца в начало
    return edge->end - i - 1;
  }
}

const struct face* getFace(int face) {
  return &cubeFaces[face];
}

int getPixelOnFace(const struct face* face, int offset, int *edgePtr) {
  char backward = (offset < 0);
  int i = abs(offset) - 1;
  if(i > face->length) {
    return -1;
  }

  int edge;
  if(!backward) {
    edge = 0;
    while(face->edges[edge]->length < i) {
      i -= face->edges[edge]->length;
      edge++;
    }
    if(edgePtr != 0) {
      *edgePtr = edge;
    }
  } else {
    edge = EDGES_PER_FACE - 1;
    while(face->edges[edge]->length < i) {
      i -= face->edges[edge]->length;
      edge--;
    }
    if(edgePtr != 0) {
      *edgePtr = EDGES_PER_FACE - 1 - edge;
    }
  }

  return getPixelOnEdge(face->edges[edge], backward ? -i : i);
}
