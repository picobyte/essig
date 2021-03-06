/**
 * This file is part of essig.
 *
 * Authors:
 *  Florian Jochheim<florian.jochheim@gmx.de>
 *  Jan Fabian Schmid <2schmid@inf>
 */
#include "essig.h"
#include "vector.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

//void build_rights(Bond *bonds, size_t start_bond, size_t start_atom, size_t atom_count, size_t bond_count)
//{
  //size_t i, j, first, second, right_count = 0;
  //bool *bools, changed = true;
  //bools = calloc(atom_count, sizeof(*bools));
  //assert(bools != NULL);
  //bools[start_atom] = true;
//
  //while (changed == true)
  //{
    //changed = false;
    ///* In every run, every atom that is reachable from a currently selected atom
     //* is now selected */
    //for (i = 0; i < bond_count; i++)
    //{
      //first = bonds[i].first;
      //second = bonds[i].second;
      //if ( i == start_bond)
        //continue;
      //else if(bools[first] && !bools[second])
      //{
        //changed = true;
        //bools[second] = true;
        //right_count++;
      //}
      //else if(!bools[first] && bools[second])
      //{
        //changed = true;
        //bools[first] = true;
        //right_count++;
      //}
    //}
  //}
  ///*every atom that has been reached is saved in rights array of the bond
   //* we are looking at*/
  //bonds[start_bond].right = malloc(sizeof(size_t)*right_count);
  //for(i=0, j= 0; i<atom_count;i++)
  //{
    //if(bools[i] && i != start_atom)
    //{
      //bonds[start_bond].right[j] = i;
      //j++;
    //}
  //}
  //bonds[start_bond].right_count = right_count;
  //free(bools);
//}

static size_t digit_count(const size_t n)
{
  if (n < 10) return 1;
  return 1 + digit_count(n/10);
}

void molecule_read_from_file(Molecule *mol, FILE *fp)
{
  Atom *atoms;
  Bond *bonds;
  size_t *right;
  char *buffer, *token;
  int charge;
  Vector pos;
  size_t atom_count= 0, bonds_count = 0, buffer_size, i, first, second;

  atoms = malloc(sizeof(*atoms));
  bonds = malloc(sizeof(*bonds));
  fseek(fp, 0, SEEK_END);
  buffer_size = ftell(fp)+1;
  fseek(fp, 0, SEEK_SET);
  buffer = malloc(sizeof(*buffer)*buffer_size);
  fread(buffer, sizeof(char), buffer_size-1,fp);

  buffer[buffer_size-1] = '\0';

  token = strtok(buffer, "\n");
  assert(strcmp(token, "> Atoms") == 0);

  token = strtok(NULL, "\n");

  //Atome einlesen
  while(strcmp(token, "> Bonds") != 0)
  {
    atom_count++;
    if(atom_count != 1)
      atoms = realloc(atoms, sizeof(Atom)*atom_count);

    for(i=0;token[i] != '\t'; i++)
    {
      atoms[atom_count-1].element_symbol[i] = token[i];
    }
    atoms[atom_count-1].element_symbol[i] = '\0';

    if(3 != sscanf(&token[i+1],
                    "%lf\t%lf\t%lf",&(pos.x[0]),&(pos.x[1]), &(pos.x[2])))
    {
      fprintf(stderr, "Koordinates in wrong format for %zu\n", atom_count-1);
      exit(EXIT_FAILURE);
    }

    token = strtok(NULL, "\n");
    atoms[atom_count-1].pos = pos;

  }

  while(strncmp(token, "> Charge", 8))
  {
    token = strtok(NULL, "\t");

    bonds_count++;
    if(bonds_count != 1)
      bonds = realloc(bonds, sizeof(*bonds)*bonds_count);
    bonds[bonds_count-1].rotatable = true;

    if(2 != sscanf(token,"%zu-%zu", &first, &second))
    {
      fprintf(stderr, "Error reading file. Bonds not in correct format\n");
      exit(EXIT_FAILURE);
    }
    bonds[bonds_count-1].first = first-1;
    bonds[bonds_count-1].second = second-1;

    token = strtok(NULL, "\n");
    bonds[bonds_count-1].right = malloc(sizeof(*right));
    i = 0;
    while(*token != '\0')
    {
      if (*token != ' ')
      {
        assert(sscanf(token, "%zu", bonds[bonds_count-1].right+i) == 1);
        token += digit_count(bonds[bonds_count-1].right[i]);
        bonds[bonds_count-1].right[i]--;
      }
      else
      {
        i++;
        bonds[bonds_count-1].right
                    = realloc(bonds[bonds_count-1].right, sizeof(*right)*(i+1));
        token++;
      }
    }
    token++;
    bonds[bonds_count-1].right_count = i+1;
  }
  while(*token != '\n') token++;
  token++;
  if(1 != sscanf(token, "%d", &charge))
  {
    fprintf(stderr,"Charge missing or wrong format\n");
    exit(EXIT_FAILURE);
  }

  mol->atoms = atoms;
  mol->bonds = bonds;
  mol->atom_count = atom_count;
  mol->bond_count = bonds_count;
  mol->charge = charge;
  free(buffer);
}
/*
 * Debug Funktionen :
 *
void print_rights(Molecule *mol)
{
  size_t i,j;
  printf("bond count: %lu\n", mol->bond_count);
  for(i = 0; i < mol->bond_count; i++)
  {
    printf("%lu -th bond: first: %lu, second: %lu rotate is %s\n", i+1, mol->bonds[i].first+1, mol->bonds[i].second+1,mol->bonds[i].rotate ? "true":"false");
    for(j = 0; j < mol->bonds[i].right_count;j++)
    {
      printf("In %lu -th bond :\nfirst is: %lu\n second is: %lu\n %lu -th right is: %lu\n", i+1,mol->bonds[i].first+1,mol->bonds[i].second+1,j+1, mol->bonds[i].right[j]+1);
    }
  }

}
int main(int argc, char *argv[])
{
  Molecule *mol = malloc(sizeof(Molecule));
  assert(argc == 2);
  char *filename = argv[1];
  FILE *file = fopen(filename, "r");
  assert(file != NULL);
  molecule_read_from_file(mol, file);
  fclose(file);
  printf(molecule_format_atom_list(mol));
  printf("charge: %d\n", mol->charge);
  print_rights(mol);
  free(mol->atoms);
  free(mol->bonds);
  free(mol);
  return EXIT_SUCCESS;
}
*/
