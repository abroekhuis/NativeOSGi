#include "bfd.h"
#include "stdio.h"

void printInfo(bfd *abfd, asection *sect, void *obj)
{
    printf("section name %s\n", sect->name);
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

  return 0;
}
