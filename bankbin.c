#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>

void main(int argc, char **argv) {
   char *destfnarg_ptr;
   char *destfn_ptr;
   char destfnbase[40];
   char destfn[50];
   int bank;
   FILE *ifp;
   FILE *ofp;
   
   int address;
   uint8_t idata[8192];
   int bytes_read;
   uint8_t odata[2];

   if (argc < 4) {
      printf("Usage: %s [source bin] [dest fn base] [start bank] [load address]\n", argv[0]);
      return;
   }

   ifp = fopen(argv[1], "r");
   if (ifp == NULL) {
      printf("Error opening %s for reading\n", argv[1]);
      return;
   }
   
   // convert destination filename base to uppercase
   destfnarg_ptr = argv[2];
   destfn_ptr = destfnbase;
   while (*destfnarg_ptr != '\0') {
      *destfn_ptr = toupper(*destfnarg_ptr);
      destfnarg_ptr++;
      destfn_ptr++;
   }
   
   bank = atoi(argv[3]);
   
   if (argc >= 5) {
      sscanf(argv[4],"%x",&address);
      if ((address < 0xA000) || (address > 0xBFE0)) {
         printf("Error: load address must be between A000 and BFE0\n");
         return;
      }
      if (address & 0x1F) {
         printf("Error: load address must be at 32-byte boundary (lowest 5 bits clear)\n");
         return;
      }
   } else {
      // set default load address to 0xA000
      address = 0xA000;
   }
   
   while (!feof(ifp)) {
      sprintf(destfn, "%s.B%03d.BIN", destfnbase, bank);
      
      ofp = fopen(destfn, "w");
      if (ofp == NULL) {
         printf("Error opening %s for writing\n", argv[2]);
         return;
      }
   
      odata[0] = (uint8_t) (address & 0x00FF);
      odata[1] = (uint8_t) ((address & 0xFF00) >> 8);
      fwrite(odata,1,2,ofp);
      
      bytes_read = fread(idata,1,8192,ifp);
      fwrite(idata,1,bytes_read,ofp);
      
      fclose(ofp);
      
      bank++;
      address = 0xA000;
   }
   
   fclose(ifp);
}   

