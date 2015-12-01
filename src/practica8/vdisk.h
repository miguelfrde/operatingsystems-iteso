#define HEADS 48
#define SECTORS 17
#define CYLINDERS 160

int vdreadsector(int unidad, int superficie, int cilindro,
                 int sector, int numero_de_sectores, unsigned char *buffer);
int vdwritesector(int unidad, int superficie, int cilindro,
                  int sector, int numero_de_sectores, unsigned char *buffer);
