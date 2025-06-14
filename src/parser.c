#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "psf.h"

Term *parse_term(const char*, int*, int);

/* Возращаемое значение = NULL при ошибке парсинга */
PSForm *parse_psf(const char *str) {
  PSForm *res = (PSForm*)calloc(1, sizeof(PSForm));
  int len = strlen(str), is_sum = -2;
  Term *new_term, *tail;

  for (int pos = 0; pos < len; ++pos) {
    char ch = str[pos];
    if (isspace(ch))
      continue;
    else if (ch == '+')
      if (is_sum < 0)
        is_sum = 1;
    else if (ch == '-') 
      is_sum = !is_sum;
    else if (isalnum(ch)) {
      if (is_sum == -1) {
        free_psf(res);
        return NULL;
      }
      new_term = parse_term(str, &pos, len);
      if (!new_term) {
        free_psf(res);
        return NULL;
      }
      /* замена знака константы */
      if (!is_sum)
        new_term->factors->val *= -1;
      if (!res->terms) {
        res->terms = new_term;
        tail = new_term;
      }
      else {
        tail->next = new_term;
        tail = new_term; 
      }
      is_sum = -1;
    }
    else {
      free_psf(res);
      return NULL;
    }
  }
  return res;
}


void free_term(Term *t) {
  if (!t)
    return;
  Factor *f1 = t->factors, *f2;
  while (f1) {
    f2 = f1;
    f1 = f1->next;
    free(f2)
  }
  free(t);
}


Term *parse_term(const char *str, int *pos, int len) {
  int is_read = 0;
  long val = 0;
  char ch;

  /* попытка считать константный множитель */
  while (*pos < len) {
    ch = str[*pos];
    if (!isdigit(ch))
      break;
    val = val * 10 + (ch - '0');
    if (val > INT_MAX)
      return NULL;
    (*pos)++;
    is_read = 1;    
  }
  /* аналогично отсутствию константного множителя */
  if (!is_read)
    val = 1;
  is_read = -1;
  Term *res = (Term*)calloc(1, sizeof(Term));
  /* создание константного множителя (val - его значение) */
  Factor *new_factor = (Factor*)calloc(1, sizeof(Factor));
  new_factor->val = val;
  res->factors = new_factor;

  /* считывание переменных */
  for (; *pos < len; ++(*pos)) {
    ch = str[*pos];
    if (isspace(ch))
      continue;
    else if (ch == '*') {
      if (is_read == 1) {
        free_term(res);
        return NULL;
      }
      is_read = 1;
    }
    else if (ch == '-' || ch == '+') {
      if (is_read == 1) {
        free_term(res);
        return NULL;
      }
      (*pos)--;
      break;
    }
    else if (isalpha(ch)) {
      if (is_read == -1 || is_read = 0) {
        free_term(res);
        return NULL;
      }
      new_factor->next = (Factor*)calloc(1, sizeof(Factor));
      new_factor = new_factor->next;
      new_factor->val = ch;
      is_read = 0;     
    }
    else {
      free_term(res);
      return NULL;
    }
  }
  return res;
}
