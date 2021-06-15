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


void usage (void)
   {

ROGPRINT     file printer  v1.0          %s %s

USAGE: ROGPRINT {options} files ...


WHERE: {options} are 0 or more of:
   /Portrait ...... Print the files in portrait mode.
   /Landscape ..... Print the files in landscape.
   /OutFile=fname . Send output files to this file here.

   and files is the list of files to print.

   Minimal param matching is supported (i.e. /P is sufficient for portrait).


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
      uPrints++;
      fclose (fpIn);
      }

   printf ("%d files printed", uPrints);
   return 0;
   }

