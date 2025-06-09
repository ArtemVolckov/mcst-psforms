#include <stdio.h>
#include <stdlib.h>
#include "psf.h"

void free_psf(PSForm *form) {
  Factor *f1, *f2;
  Term *t1, *t2;
  if (!form || !form->terms)
    return;
  t1 = form->terms;
  while (t1) {
    f1 = t1->factors;
    while (f1) {
      f2 = f1;
      f1 = f1->next;
      free(f2);
    }
    t2 = t1;
    t1 = t1->next;
    free(t2);
  }
  form->terms = NULL;   
}

void print_psf(PSForm *form) {
  Factor *f;
  Term *t;
  if (!form)
    return;
  if (!form->terms) {
    putchar('\n');
    return;
  }
  t = form->terms;
  while (t) {
    f = t->factors;
    /* вывод константы */
    if (f) {
      if (f->val != 1 || !f->next) {
        printf("%d", f->val);
        if (f->next)
          putchar('*');
      }
    }
    f = f->next;
    /* вывод переменных */
    while (f) {
      putchar(f->val);
      if (f->next)
        putchar('*');
      f = f->next;
    }
    if (t->next)
      printf(" + ");
    t = t->next;
  }
  putchar('\n');
}

int compare_terms_without_const(Term *t1, Term *t2) {
  if (!t1 && !t2)
    return 0;
  Factor *f1 = t1->factors, *f2 = t2->factors;
  int count1 = 0, count2 = 0;
  /* проверка на одинаковое число множителей */ 
  while (f1) {
    count1++;
    f1 = f1->next;
  }
  while (f2) {
    count2++;
    f2 = f2->next;
  }
  if (count1 != count2)
    return 1;
  f1 = t1->factors, f2 = t2->factors;
  if (!f1)
    return 0;
  f1 = f1->next;
  f2 = f2->next; 
  /* сбор кол-ва раз, которое встречается каждая переменная в слагаемом */ 
  int *t1_nval[MAX_ASCII] = {0};
  int *t2_nval[MAX_ASCII] = {0};
  while (f1) {
    t1_nval[f1->val]++;
    f1 = f1->next;
  }
  while (f2) {
    t2_nval[f2->val]++;
    f2 = f2->next;
  }
  /* проверка на совпадения каждой переменной */
  for (int i = 0; i < MAX_ASCII; ++i)
    if (t1_nval[i] != t2_nval[i])
      return 1;
  return 0;
}

void simplify_psf(PSForm *form) {
  if (!form || !form->terms)
    return;
  Term *t1 = form->terms, *t2, *tmp_t2;
  Factor *f, *tmp_f;
  while (t1) {
    t2 = t1->next;
    tmp_t2 = t1;
    while (t2) {
      /* найдено слагаемое, которое можно объединить */
      if (!compare_terms_without_const(t1, t2)) {
        if (t1->factors) 
          t1->factors->val += t2->factors->val;
        tmp_t2->next = t2->next;
        f = t2->factors;
        while (f) {
          tmp_f = f->next;
          free(f);
          f = tmp_f; 
        } 
        free(t2);
        t2 = tmp_t2->next;
        continue;
      }
      tmp_t2 = t2; 
      t2 = t2->next;
    }
    t1 = t1->next;
  }
  t1 = form->terms;
  t2 = NULL;
  /* удаляем слагаемые с константой 0 */
  while (t1) {
    if (t1->factors) {
      if (t1->factors->val == 0) {
        /* удаление первого слагаемого */
        if (!t2) {
          form->terms = t1->next;
          f = t1->factors;
          while (f) {
            tmp_f = f->next;
            free(f);
            f = tmp_f;
          }
          free(t1);
          t1 = form->terms;
          continue;
        }
        /* удаление промежуточного или последнего слагаемого */
        else {
          t2->next = t1->next;
          f = t1->factors;
          while (f) {
            tmp_f = f->next;
            free(f);
            f = tmp_f; 
          } 
          free(t1);
          t1 = t2->next;
          continue;
        }
      }
    }
    t2 = t1;
    t1 = t1->next;
  } 
}
