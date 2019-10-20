#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

void combine_bins(char *newfn, char *iofn) {
   FILE *ifp;
   FILE *ofp;
   uint8_t addrhdr[2];
   uint8_t idata1[8192];
   int addr1;
   int idata1_len;
   uint8_t idata2[8192];
   int addr2;
   int idata2_len;
   int address;
   uint8_t odata[8192];
   int odata_len;
   
   ifp = fopen(newfn, "r");
   fread(addrhdr,1,2,ifp);
   addr1 = addrhdr[0] | (addrhdr[1] << 8);
   idata1_len = fread(idata1,1,8192,ifp);
   fclose(ifp);
   
   ifp = fopen(iofn, "r");
   fread(addrhdr,1,2,ifp);
   addr2 = addrhdr[0] | (addrhdr[1] << 8);
   idata2_len = fread(idata2,1,8192,ifp);
   fclose(ifp);
   
   if (addr1 < addr2) {
      address = addr1;
      memcpy(odata,idata1,idata1_len);
      memcpy(odata+addr2-addr1,idata2,idata2_len);
      odata_len = addr2-addr1+idata2_len;
   } else {
      address = addr2;
      memcpy(odata,idata2,idata2_len);
      memcpy(odata+addr1-addr2,idata1,idata1_len);
      odata_len = addr1-addr2+idata1_len;
   }
   
   addrhdr[0] = address & 0xFF;
   addrhdr[1] = (address & 0xFF00) >> 8;
   
   sprintf(&iofn[strlen(iofn)-8],"%s",newfn);
   
   ofp = fopen(iofn, "w");
   fwrite(addrhdr,1,2,ofp);
   fwrite(odata,1,odata_len,ofp);
   fclose(ofp);
}

void main(int argc, char **argv) {
   FILE *ofp;
   char *ifn;
   int ifn_len;
      
   int num_banks = 0;
   int bank;
   char bank_str[4] = "000";
   char bins[256][64];
   struct dirent *entry;
   DIR *dp;
   
   char *paramsfn = "bankparams.asm";
   
   if (argc >= 2) {
      paramsfn = argv[1];
   }
   
   ofp = fopen(paramsfn, "w");
   if (ofp == NULL) {
      printf("Error opening %s for writing\n", paramsfn);
      return;
   }
   
   for (bank=0; bank<256; bank++) {
      bins[bank][0] = '\0';
   }

   dp = opendir(".");
   while (entry = readdir(dp)) {
      ifn = entry->d_name;
      ifn_len = strlen(ifn);
      if (  (ifn_len > 9) 
            && !memcmp(&ifn[ifn_len-9],".B",2*sizeof(char))
            && !memcmp(&ifn[ifn_len-4],".BIN",4*sizeof(char)) ) {
         memcpy(bank_str,&ifn[ifn_len-7],3*sizeof(char));
         bank = atoi(bank_str);
         if (strlen(bins[bank])) {
            combine_bins(ifn,bins[bank]);            
         } else {
            strcpy(bins[bank],ifn);
         }
      }
   }   
   closedir(dp);
   
   bank = 0;
   while ((bank < 256) && (strlen(bins[bank]))) {
      fprintf(ofp,"b%d_filename: .asciiz \"%s\"\n", bank, bins[bank]);
      bank++;
   }
   fprintf(ofp,"end_filenames:\n");

   num_banks = bank;
   fprintf(ofp,"BANKS_TO_LOAD = %d\n", num_banks);
   fprintf(ofp,"bankparams:\n");
   
   for (bank = 0; bank < num_banks; bank++) {
      if (bank+1 == num_banks) {
         fprintf(ofp,".byte end_filenames-b%d_filename-1\n", bank);
      } else {
         fprintf(ofp,".byte b%d_filename-b%d_filename-1\n", bank+1, bank);
      }
      fprintf(ofp,".byte <b%d_filename\n",bank);
      fprintf(ofp,".byte >b%d_filename\n",bank);  
   }
   
   fclose(ofp);

}   

