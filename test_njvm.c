#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[]){
  printf("Start tests\n\n");  

  system("./nja asm/test/halt_test.asm bin/test/halt_test.bin");
  system("./nja asm/test/add_test.asm bin/test/add_test.bin");
  system("./nja asm/test/sub_test.asm bin/test/sub_test.bin");
  system("./nja asm/test/mul_test.asm bin/test/mul_test.bin");
  system("./nja asm/test/div_test.asm bin/test/div_test.bin");
  system("./nja asm/test/mod_test.asm bin/test/mod_test.bin");
  system("./nja asm/test/rdint_wrint_test.asm bin/test/rdint_wrint_test.bin");
  system("./nja asm/test/asf_test.asm bin/test/asf_test.bin");
  system("./nja asm/test/eq_test.asm bin/test/eq_test.bin");
  system("./nja asm/test/ne_test.asm bin/test/ne_test.bin");
  system("./nja asm/test/lt_test.asm bin/test/lt_test.bin");
  system("./nja asm/test/le_test.asm bin/test/le_test.bin");
  system("./nja asm/test/gt_test.asm bin/test/gt_test.bin");
  system("./nja asm/test/ge_test.asm bin/test/ge_test.bin");
  system("./nja asm/test/jmp_test.asm bin/test/jmp_test.bin");
  system("./nja asm/test/brf_test.asm bin/test/brf_test.bin");
  system("./nja asm/test/brt_test.asm bin/test/brt_test.bin");

  system("./njvm bin/test/halt_test.bin");
  system("./njvm bin/test/add_test.bin");
  system("./njvm bin/test/sub_test.bin");
  system("./njvm bin/test/mul_test.bin");
  system("./njvm bin/test/div_test.bin");
  system("./njvm bin/test/mod_test.bin");

  system("./njvm rdint_wrint_test.bin");
  system("./njvm asf_test.bin");

  system("./njvm eq_test.bin");
  system("./njvm ne_test.bin");
  system("./njvm lt_test.bin");
  system("./njvm le_test.bin");
  system("./njvm gt_test.bin");
  system("./njvm ge_test.bin");

  system("./njvm jmp_test.bin");
  system("./njvm brf_test.bin");
  system("./njvm brt_test.bin");

  printf("\n\nStopped tests\n");

  return 0;
}
