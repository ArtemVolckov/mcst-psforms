#include <stdio.h>
#include <string.h>
#include <time.h>
#include "psf.h"

int main() {
  char str1[10000], str2[10000];
  PSForm a, b, res;
  int err = 0;
  char op;

  /* таймирование проводилось при помощи утилиты time */
  /* clock_t start = clock(); */

  /* для выхода из цикла необходимо подать EOF */
  for (;;) {
    if (scanf("%c", &op) == EOF) /* чтение операции */
      break;
    while (getchar() != '\n') {}
    fgets(str1, sizeof(str1), stdin); /* чтение 1-ой PS-формы */
    fgets(str2, sizeof(str2), stdin); /* чтение 2-ой PS-формы */
    str1[strcspn(str1, "\n")] = '\0';
    str2[strcspn(str2, "\n")] = '\0';
    a = parse_psf(str1), b = parse_psf(str2);
    switch (op) {
      case '+': res = add_psf(&a, &b); break;
      case '-': res = sub_psf(&a, &b); break;
      case '*': res = mul_psf(&a, &b); break;
      case '/': 
        res = div_psf(&a, &b, &err);
        if (err == 1) {
          fprintf(stderr, "error\n");
          free_psf(&a); free_psf(&b); free_psf(&res);
          continue;
        }
        break;
      case '=': 
        printf(compare_psf(&a, &b) ? "not equal" : "equal");
        break;
      default:
        fprintf(stderr, "error\n");
        free_psf(&a); free_psf(&b); free_psf(&res);
        return 1;
    }
    simplify_psf(&res); 
    print_psf(&res);
    free_psf(&res); free_psf(&a); free_psf(&b);
  }

  /* clock_t end = clock();
   * double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
   * fprintf(stderr, "%.3f seconds\n", elapsed); */

  return 0;
}
