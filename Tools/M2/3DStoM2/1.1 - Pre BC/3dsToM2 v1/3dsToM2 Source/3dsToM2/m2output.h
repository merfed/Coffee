void fillLine(FILE *f);
void writeAnimationBlockData(AnimationBlock *block, FILE *f, int *dRange, int nRange, int *dTimestamps, int nTimestamps, void *dData, int nData, int dataSize);

void initFile(FILE *f,const char *name);
void writeModelData(FILE *f,ModelVertex *vertices,int nVertices, uint16	*triangles, int nTriangles, ModelGeoset	*submeshes, int nSubmeshes, ModelTexUnit *texUnits, int nTexUnit);
void writeMaterialData(FILE *f, std::vector<std::string> texNames, ModelTextureDef *textures, int nTextures, int nMaterials, float *colorData, short *colorTransData, short *transData, uint32 *renderFlags);
void rewriteHeader(FILE *f);