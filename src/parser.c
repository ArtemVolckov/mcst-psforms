#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "psf.h"

Term *parse_term(const char*, int*, int);

/* Возращаемое значение = NULL при ошибке парсинга */
PSForm *parse_psf(const char *str) {
  PSForm *res = (PSForm*)calloc(1, sizeof(PSForm));
  int len = strlen(str), is_sum = -1;
  Term *new_term, *tail;

  for (int pos = 0; pos < len; ++pos) {
    char ch = str[pos];
    if (isspace(ch))
      continue;
    else if (ch == '+')
      if (is_sum == -1)
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
        // FIX: добавить до этого проверку чтоб не NULL
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
