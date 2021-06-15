/*
 *
 * rogprint.c
 * Tuesday, 4/18/1995.
 *
 */

#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <GnuFile.h>
#include <GnuStr.h>
#include <GnuMisc.h>
#include <GnuArg.h>


void PrintCodes (FILE *fp, PSZ pszCodes)
   {
   for (; *pszCodes; pszCodes++)
      {
      if (*pszCodes == '~')
         fputc ('\x1B', fp);
      else
         fputc (*pszCodes, fp);
      }
   }


void Usage (void)
   {
   printf ("\n");
   printf ("ROGPRINT     file printer  v1.0          %s\n", __DATE__);
   printf ("\n");
   printf ("USAGE: ROGPRINT {options} files ...\n");
   printf ("\n");
   printf ("WHERE: {options} are 0 or more of:\n");
   printf ("   /Portrait ...... Print the files in portrait mode.\n");
   printf ("   /Landscape ..... Print the files in landscape.\n");
   printf ("   /OutFile=fname . Send output files to this file here.\n");
   printf ("\n");
   printf ("   and files is the list of files to print.\n");
   printf ("\n");
   printf ("   Minimal param matching is supported (i.e. /P is sufficient for portrait).\n");
   printf ("   This program also appends \\r to all \\f 's.\n");
   printf ("   For further information see Roger!\n");
   exit (0);

   }


   
int main (int argc, char *argv[])
   {
   USHORT i, uPrints = 0;
   PSZ    pszOut, pszIn, psz;
   FILE   *fpOut, *fpIn;
   char   szBuff [1024];;

   if (ArgBuildBlk ("? *^Debug *^Portrait *^Landscape *^OutFile%"))
      Error ("%s", ArgGetErr ());

   if (ArgFillBlk (argv))
      Error ("%s", ArgGetErr ());

   if (argc < 2)
      Usage ();

   pszOut = (ArgIs ("OutFile") ? ArgGet ("OutFile", 0) : "PRN");

   if (!(fpOut = fopen (pszOut, "wt")))
      Error  ("Unable to open output file : %s", pszOut);


   for (i=0; i < ArgIs (NULL); i++)
      {
      pszIn = ArgGet (NULL, i);

      if (!(fpIn = fopen (pszIn, "rt")))
         {
         printf ("Unable to open input file : %s\n", pszIn);
         continue;
         }

      printf ("Printing File: %s", pszIn);

      if (ArgIs ("Portrait"))
         PrintCodes (fpOut, "~E~&l0o72F~=~(10U~(s0p16.67h8.5v0s0b0T");
      if (ArgIs ("Landscape"))
         PrintCodes (fpOut, "~E~&l1o58f6C~=~(10U~(s0p16.67h8.5v0s0b0T");

      while (FilReadLine (fpIn, szBuff, "", sizeof (szBuff)) != 0xFFFF)
         {
         for (psz=szBuff; *psz; psz++)
            {
            fputc (*psz, fpOut);
            if (*psz == '\f')
               fputc ('\r', fpOut);
            }
         fputc ('\n', fpOut);
         }
      PrintCodes (fpOut, "~E");
      printf ("\n");
      uPrints++;
      fclose (fpIn);
      }

   printf ("%d files printed", uPrints);
   return 0;
   }

