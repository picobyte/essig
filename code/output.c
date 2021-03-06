/**
* This file is part of essig.
*
* Authors:
*   Niek Andresen <2andrese@inf>
*   Robert Hartmann <2hartman@inf>
*   Damian Hofmann <2hofmann@inf>
*/
#define _GNU_SOURCE // for asprintf
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "essig.h"
#include "util.h"

static char *safe_strcat(char *dest, const char *src)
{
  dest = realloc(dest, strlen(dest) + strlen(src) + sizeof(char));
  CHECK_ALLOC(dest);
  strcat(dest, src);
  return dest;
}

/**
 * Returns a list of the molecule's atoms in the orca/molden format.
 */
char *molecule_format_atom_list(const Molecule *mol)
{
  char *result; /* result string */
  size_t i, j, atom_count;
  Atom *atom;

  atom_count = mol->atom_count;
  result = malloc((1 + atom_count * (3 * 14 + 1 * 2 + 1 * 1)) * sizeof *result);
  /* 1 end of string and 3 coordinates, 1 element symbol and 1 end of line per atom */
  result[0] = '\0';

  for(i = 0; i < atom_count; ++i)
  {
    atom = mol->atoms + i;
    result = safe_strcat(result, atom->element_symbol);
    for(j = 0; j < 3; ++j)
    {
      char *temp;
      asprintf(&temp, "   %*.5f", 11, atom->pos.x[j]);
      result = safe_strcat(result, temp);
      free(temp);
    }
    result = safe_strcat(result, "\n");
  }
  return result;
}

/**
 * Creates a file of the molecule molden can work with and calls Molden with it.
 */
void molden_output_molecule(const Molecule *mol)
{
   char *list;
   FILE *dat;
   
   dat = fopen("molden_input.xyz", "w+");
   assert(dat != NULL);
   list = molecule_format_atom_list(mol);
   fprintf(dat,"%zu\n\n", mol->atom_count);
   fprintf(dat, "%s", list);   
   
   free(list);
   fclose(dat);
}

