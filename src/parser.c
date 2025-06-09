#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "psf.h"

Term *parse_term(const char*, int*, int);

PSForm parse_psf(const char *str) {
  int len = strlen(str), is_sum = 1;
  PSForm res = {NULL};
  Term *term, *last;
  char ch;
  for (int pos = 0; pos < len; ++pos) {
    ch = str[pos];
    if (ch == ' ' || ch == '+')
      continue;
    if (ch == '-') 
      is_sum = !is_sum;
    else {
      term = parse_term(str, &pos, len);
      /* замена знака константы */
      if (!is_sum) {
        if (term && term->factors)
          term->factors->val *= -1;
        is_sum = 1;
      }
      if (!res.terms) {
        res.terms = term;
        last = term;
      }
      else {
        last->next = term;
        last = term; 
      }
    }
  }
  return res;
}

Term *parse_term(const char *str, int *pos, int len) {
  Term *res = (Term*)malloc(sizeof(Term));
  res->next = NULL;
  Factor *factor;
  int val = 0, is_read = 0;
  char ch;
  /* попытка считать константный множитель */
  while (*pos < len) {
    ch = str[*pos];
    if (!isdigit(ch))
      break;
    val = val * 10 + (ch - '0');
    (*pos)++;
    is_read = 1;    
  }
  /* аналогично отсутствию константного множителя */
  if (!is_read)
    val = 1;
  /* создание константного множителя (val - его значение) */
  factor = (Factor*)malloc(sizeof(Factor));
  res->factors = factor;
  factor->next = NULL;
  factor->val = val;
  while (*pos < len) {
    ch = str[*pos];
    if (ch == '-' || ch == '+')
      break;
    if (ch != ' ' && ch != '*') {
      factor->next = (Factor*)malloc(sizeof(Factor));
      factor = factor->next;
      factor->next = NULL;
      factor->val = ch;      
    }
    (*pos)++;
  }
  (*pos)--;
  return res;
}
