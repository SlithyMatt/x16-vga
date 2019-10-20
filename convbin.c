#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void main(int argc, char **argv) {
   FILE *ifp;
   FILE *ofp;
   
   int address;
   uint8_t idata;
   uint8_t odata[2];

   if (argc < 3) {
      printf("Usage: %s [source binary file] [converted binary file] [default load address]\n", argv[0]);
      return;
   }

   ifp = fopen(argv[1], "r");
   if (ifp == NULL) {
      printf("Error opening %s for reading\n", argv[1]);
      return;
   }
   ofp = fopen(argv[2], "w");
   if (ofp == NULL) {
      printf("Error opening %s for writing\n", argv[2]);
      return;
   }
   
   if (argc >= 4) {
      sscanf(argv[3],"%x",&address);   
   } else {
      // set default load address to 0x0000
      address = 0x0000;
   }
   
   odata[0] = (uint8_t) (address & 0x00FF);
   odata[1] = (uint8_t) ((address & 0xFF00) >> 8);
   fwrite(odata,1,2,ofp);
   
   while (!feof(ifp)) {
      if (fread(&idata,1,1,ifp) > 0) {
         fwrite(&idata,1,1,ofp);
      }
   }
   
   fclose(ifp);
   fclose(ofp);
}   

