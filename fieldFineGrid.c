#include <stdio.h>
#include "magnetic_field.h"
#include "vector.h"
#include "interp2.h"

#define SIZE_X 514
#define SIZE_Z 1026

int main(int argc, char *argv[]) {

  printf("JAAA");
  vector *point = vnew(2);
  vector *field_in_point = vnew(2);

  double B_x[SIZE_X][SIZE_Z];
  double B_z[SIZE_X][SIZE_Z];

  FILE *fp;
  fp = fopen("Output_B_x", "w");

  

  magnetic_field *B = magnetic_field_load("iter2d.bkg");
  
  int i;
  int k=2;
  printf("%d",k);
  double *grid_x = interp2_create_grid(SIZE_X, 3.5, 8.9);
  double *grid_z = interp2_create_grid(SIZE_Z, -5.5, 5.5);
  
  for (i=0; i < SIZE_Z; i++) {
    point->val[1] = grid_x[i];
    printf("%d",i);
    
    for (k=0; k < SIZE_X; k++) {
      point->val[0] = grid_z[k];
      
      /* Get field value in point */
      field_in_point = magnetic_field_get(B, point);
      /* store x-value in B_x array */
      B_x[i][k] = field_in_point->val[0];
      /* print to file */
      // fprintf(fp, "%f \t",B_x[i][k]);
    }
    fprintf(fp, "\n");
}
   fclose(fp);

  return 0;
}
