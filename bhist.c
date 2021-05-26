
/*
 * Datei         :  bhist.c
 * Erstellung    :  A.Schmiedl, 20-dez-2016, modifiziert 26-mai-2021
 * Funktion      :  Berechnung und Visualisierung eines Histogramms ueber alle
 *                  Byte-Werte einer Datei. Sehr langes Integerformat (8 Byte = 2^64)
 *                  wird f.d. Zaehler verwendet, um sehr grosse Dateien verarbeiten
 *                  zu koennen.
 * Motivation    :  Ueberpruefung der Formate grosser Datendateien auf stoerende Zeichen.
 * Uebersetzung  :  gcc -o bhist bhist.c
 * Verwendung    :  ./bhist zupruefendedatei histotab.dat
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define  BU_LEN  8192                    /* Zahl der Bearbeitungspufferelemente  */
static unsigned char  buff[BU_LEN];          /* Bearbeitungspuffer f. Signalteil */

short  fill_buf(FILE *fp);                /* Prototyp: Datenpuffer_Fuellfunktion */

int  main(argc,argv)
int  argc;
char *argv[];
{
   FILE  *infil, *outfil, *pltfil;
   unsigned long long ind;                              /* Zaehlt gelesene Bytes */
   short nr, tmp, block=0;
   static unsigned long long  hista[256];           /* Globales Histogramm-Array */
   char* plotscript="./bhist.plt";                  /* Name des GnuPlot-Skriptes */
   char bashcmd[50];                   /* bash-Befehl: Skript ausfuehrbar machen */

   if (argc != 3)  {
      printf(" ---> bhist.c \n");
      printf("      A.Schmiedl, Erlangen, 26-may-2021 \n");
      printf("      Create a byte value count table. \n");
      printf("      Requires 'gnuplot' for visualizing. \n");
      printf("      Usage: bhist inputfile resulttabletextfile \n");
      printf("      (Re)visualization with command %s \n",plotscript);
      return(-1);
   }

   printf(" ---> Byte count histogram with graphical visualization \n");
   printf(" ---> Input file is : %s ", argv[1]);
   if ((infil = fopen(argv[1],"rb")) == NULL)
   { printf ("\n ---> Unable to open input file %s for reading !! ",argv[1]);
      return(-2);
   }

   printf("\n ---> Output text file is : %s ", argv[2]);
   if ((outfil = fopen(argv[2],"w")) == NULL)
   { printf ("\n ---> Unable to open output text file %s for writing !! ",argv[2]);
      return(-3);
   }

   if ((pltfil = fopen(plotscript,"w")) == NULL)
   { printf ("\n ---> Unable to write plotscript file %s !! ",plotscript);
      return(-2);
   }

   for (tmp=0; tmp<256; tmp++)  hista[tmp]=0;                /* Histogramm loeschen  */

   ind = 0;                                                       /* Hauptschleife : */
   while ((nr=fill_buf(infil))!=0)
   {
      ind += (unsigned long long) nr;
      block++;
      for (tmp=0; tmp<nr; tmp++)  hista[buff[tmp]]++;
   }

   printf("\n ---> %d block(s) with %lld bytes have been processed in total ",block,ind);
   for (tmp=0; tmp<256; tmp++)
   {
                                                            /* Histogramm ausgeben : */
      fprintf(outfil,"\n     %02X %7d %10lld",tmp,tmp,hista[tmp]);
   }

   printf("\n ---> Results table has been written into text file %s ",argv[2]);
   printf("\n ---> Revisualization with GnuPlot script %s \n",plotscript);

   fprintf(pltfil,"#!/usr/bin/gnuplot\n");
   fprintf(pltfil,"set grid\n");
   fprintf(pltfil,"set style line 1 linecolor rgb '#0060ad' linewidth 2\n");
   fprintf(pltfil,"set xtics 0,8,255 scale 1.5 out\n");
   fprintf(pltfil,"set mxtics 8\n");
   fprintf(pltfil,"set xtics format \"%%02X\"\n");
//   fprintf(pltfil,"set yrange [0.1<*:];\n");
   fprintf(pltfil,"set yrange [0.5:*];\n");
   fprintf(pltfil,"set logscale y 2\n");
   fprintf(pltfil,"set ytics 1,2\n");
   fprintf(pltfil,"set format y \"2^{%%L}\"\n");     /* Darstellung von 2er-Potenzen */
   fprintf(pltfil,"set label 'Byte histogram of file %s' at graph 0.45, 0.93\n",argv[1]);
   fprintf(pltfil,"plot [-1:256] '%s' using 2:3 with impulses linestyle 1\n",argv[2]);
   fprintf(pltfil,"pause -1\n");

   fclose(pltfil);
   fclose(outfil);
   fclose(infil);
   strcpy(bashcmd,"chmod +x ");
   strcat(bashcmd,plotscript);
   strcat(bashcmd,"\n");
   system(bashcmd);                             /* GnuPlot-Skript ausfuehrbar machen */
   system(plotscript);                                  /* GnuPlot-Skript ausfuehren */
   return(0);
}

short fill_buf(fp)                                             /* BU_LEN Werte lesen */
FILE *fp;
{
   short tmp,nwerte,nn;
                                                          /* bei EOF wird nwerte=0 : */
   nwerte = fread((void *)buff,sizeof(char),BU_LEN,fp);
   if (nwerte<BU_LEN)                   /* falls zu wenig Werte, Rest mit 0 belegen: */
      for (tmp=nwerte; tmp<BU_LEN; tmp++)    buff[tmp] = 0;
   return(nwerte);
}
