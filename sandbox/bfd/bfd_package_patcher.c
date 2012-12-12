#include "bfd.h"
#include "stdio.h"

void printInfo(bfd *abfd, asection *sect, void *obj)
{
    printf("section name %s\n", sect->name);
}

void printSymbolTable(bfd *abfd) {
  long size;
  asymbol **symbol_table;
  long symbols;
  long i;

  if((size=bfd_get_symtab_upper_bound(abfd))<=0) {
    return 1;
  }

  symbol_table=xmalloc(size);
    if((symbols=bfd_canonicalize_symtab(abfd, symbol_table))<0) {
    return 1;
  }

  for(i=0; i<symbols; i++) {
    printf("symbol: %s\n", symbol_table[i]->name);
  }
}


int main(int argc, char* argv[])
{
    bfd_init();

    bfd* abfd = bfd_openr("liborg_package_a.so", "default");
  if (abfd == NULL)
  {
    printf("bfd_openr error\n");
    return 1;
  }

  bfd_boolean check = bfd_check_format(abfd, bfd_object);
  printf("checking format bfd_object: %d\n", check);

  bfd_map_over_sections(abfd, printInfo, NULL);


  asection* section = bfd_get_section_by_name(abfd, ".dynamic");
  if (section == NULL)
  {
      printf("bfd_get_section_by_name error\n");
      return 1;
  }

  bfd_byte* dynamicSection = NULL;
  bfd_boolean readSuccess = bfd_malloc_and_get_section(abfd, section, &dynamicSection);
  if (!readSuccess)
  {
      printf("Reading .dynamic section failed.\n");
      return 1;
  }

  printf("section read\n");

  printSymbolTable(abfd);

  return 0;
}
