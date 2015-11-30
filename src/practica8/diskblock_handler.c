// ******************************************************************************
// Para el mapa de bits del área de de datos
// ******************************************************************************
// Recibe como argumento el número de bloque que quiero comprobar
// si está libre o no, si está libre regresa 1, si no, regresa 0

int isblockfree(int block)
{
  int offset=block/8;
  int shift=block%8;
  int result;
  int i;

  //El sector de boot de la partición es necesario tenerlo
  // en memoria porque ahí están los parámteros necesarios
  // para saber en que sector lógico empieza el mapa de bits
  if(!secboot_en_memoria)
  {
    result=vdreadsector(0,0,0,2,1,(char *) &secboot);
    secboot_en_memoria=1;
  }

  mapa_bits_bloques= secboot.sec_res;

  // Si el mapa de bits de la partición no está en 
  // memoria, es necesario cargarlo
  if(!blocksmap_en_memoria)
  {
    for(i=0;i<secboot.sec_mapa_bits_bloques;i++)
      result=vdreadseclog(mapa_bits_bloques+i,blocksmap+i*512);
    blocksmap_en_memoria=1;
  }

  if(blocksmap[offset] & (1<<shift))
    return(0);  // El bloque no está libre
  else
    return(1);  // El bloque está disponible
}

// Buscar el siguiente bloque libre en el mapa de bits
int nextfreeblock()
{
  int i,j;
  int result;

  if(!secboot_en_memoria)
  {
    result=vdreadsector(0,0,0,2,1,(char *) &secboot);
    secboot_en_memoria=1;
  }

  mapa_bits_bloques = secboot.sec_res;

  if(!blocksmap_en_memoria)
  {
    for(i=0;i<secboot.sec_mapa_bits_bloques;i++)
      result=vdreadseclog(mapa_bits_bloques+i,blocksmap+i*512);
    blocksmap_en_memoria=1;
  } 



  // Recorrer cada uno de los bytes del mapa de bits buscando
  // un byte que no esté en puros 1s, es decir, diferente a 
  // 0xFF
  i=0;
  while(blocksmap[i]==0xFF && i<secboot.sec_mapa_bits_bloques*512)
    i++;

  // Si encontramos un byte del mapa de bits donde hay un bit
  // en 0, esto significa que hay un bloque libre
  if(i<secboot.sec_mapa_bits_bloques*512)
  {
    j=0;
    while(blocksmap[i] & (1<<j) && j<8)
      j++;

    return(i*8+j);
  }
  else
    return(-1); // Ya no hay bloques disponibles en la
          // partición
    
}

// Asignar un bloque, poner en el mapa de bits que ese bloque ya
// se está usando, es decir, en 1
int assignblock(int block)
{
  int offset=block/8;
  int shift=block%8;
  int result;
  int i;
  int sector;

  if(!secboot_en_memoria)
  {
    result=vdreadsector(0,0,0,2,1,(char *) &secboot);
    secboot_en_memoria=1;
  }

  mapa_bits_bloques= secboot.sec_res;

  if(!blocksmap_en_memoria)
  {
    for(i=0;i<secboot.sec_mapa_bits_bloques;i++)
      result=vdreadseclog(mapa_bits_bloques+i,blocksmap+i*512);
    blocksmap_en_memoria=1;
  } 

  // Del byte que corresponde al bloque, poner en 1 el bit
  blocksmap[offset]|=(1<<shift);

  // Actualizar el mapa de bits
  sector=(offset/512);
  vdwriteseclog(mapa_bits_bloques+sector,blocksmap+sector*512);
  return(1);
}


// Designar un bloque, poner en el mapa de bits que ese bloque ya
// no se está usando, es decir, en 0
int unassignblock(int block)
{
  int offset=block/8;
  int shift=block%8;
  int result;
  char mask;
  int sector;
  int i;

  if(!secboot_en_memoria)
  {
    result=vdreadsector(0,0,0,2,1,(char *) &secboot);
    secboot_en_memoria=1;
  }

  mapa_bits_bloques= secboot.sec_res;

  if(!blocksmap_en_memoria)
  {
    for(i=0;i<secboot.sec_mapa_bits_bloques;i++)
      result=vdreadseclog(mapa_bits_bloques+i,blocksmap+i*512);
    blocksmap_en_memoria=1;
  }

  blocksmap[offset]&=(char) ~(1<<shift);

  sector=(offset/512);
  vdwriteseclog(mapa_bits_bloques+sector,blocksmap+sector*512);
  return(1);
}

// **********************************************************************************
// Lectura y escritura de bloques
// **********************************************************************************

int writeblock(int block,char *buffer)
{
  int result;
  int i;
  if(!secboot_en_memoria)
  {
    // Leer el sector lógico 0 de la partición
    result=vdreadsector(0,0,0,2,1,(char *) &secboot);
    secboot_en_memoria=1;
  }

  // Inicio area de datos = sectores reservados +
  //              sectores mapa de bits + 
  //              sectores area nodos i (dr)
  inicio_area_datos=secboot.sec_res+secboot.sec_mapa_bits_bloques+secboot.sec_tabla_nodos_i;

  // Escribir todos los sectores que corresponden al 
  // bloque
  for(i=0;i<secboot.sec_x_bloque;i++)
    vdwriteseclog(inicio_area_datos+(block-1)*secboot.sec_x_bloque+i,buffer+512*i);
  return(1);  
}

int readblock(int block,char *buffer)
{
  int result;
  int i;

  if(!secboot_en_memoria)
  {
    result=vdreadsector(0,0,0,2,1,(char *) &secboot);
    secboot_en_memoria=1;
  }
  inicio_area_datos=secboot.sec_res+ secboot.sec_mapa_bits_bloques+secboot.sec_tabla_nodos_i;

  for(i=0;i<secboot.sec_x_bloque;i++)
    vdreadseclog(inicio_area_datos+(block-1)*secboot.sec_x_bloque+i,buffer+512*i);
  return(1);  
}

 
