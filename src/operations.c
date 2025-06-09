#include <stdlib.h>
#include "psf.h"

Factor *copy_factors(Factor *f) {
  Factor *head = NULL, *f1 = NULL, *f2;

  if (!f)
    return NULL;
  while (f) {
    f2 = (Factor*) malloc(sizeof(Factor));
    f2->val = f->val;
    f2->next = NULL;
    
    if (!f1)
      head = f2;
    else 
      f1->next = f2;
    f1 = f2;
    f = f->next;
  }
  return head;
}

Term *copy_terms(Term *t) {
  Term *head = NULL, *t1 = NULL, *t2;

  if (!t)
    return NULL;
  while (t) {
    t2 = (Term*) malloc(sizeof(Term));
    t2->factors = copy_factors(t->factors);
    t2->next = NULL;
    
    if (!t1)
      head = t2;
    else 
      t1->next = t2;
    t1 = t2;
    t = t->next;
  }
  return head;
}

PSForm add_psf(PSForm *a, PSForm *b) {
  PSForm form = {NULL};
  form.terms = copy_terms(a->terms);
  Term *t;
  
  if (!form.terms)
    form.terms = copy_terms(b->terms);
  else {
    t = form.terms;
    
    while (t->next)
      t = t->next;
    t->next = copy_terms(b->terms);
  }
  return form;
}

PSForm sub_psf(PSForm *a, PSForm *b) {
  PSForm inv_form = {NULL}, form;
  inv_form.terms = copy_terms(b->terms);
  Term *t = inv_form.terms;

  /* инвертирование знака для каждого слагаемого из вычитаемой PS-формы */
  while (t) {
    if (t->factors)
      t->factors->val *= -1;
    t = t->next;
  }
  form = add_psf(a, &inv_form);
  free_psf(&inv_form);
  return form; 
}

PSForm mul_psf(PSForm *a, PSForm *b) {
  PSForm form = {NULL};
  Term *t1 = NULL, *t2, *t3;
  Factor *f1, *f2;
  t3 = b->terms;
 
  /* для каждого слагаемого из PS-формы множителя */ 
  while (t3) {
    t2 = copy_terms(a->terms);

    if (!t1)
      form.terms = t2;
    else
      t1->next = t2;
    /* для всей PS-формы множимого */
    while (t2) {
      t1 = t2;
      f1 = t1->factors;
      f2 = t3->factors;

      if (f1 && f2)
        f1->val *= f2->val;
      while (f1->next)
        f1 = f1->next;
      f2 = f2->next;

      /* добавление множителей (переменных) в слагаемое PS-формы множимого */
      while (f2) {
        f1->next = (Factor*) malloc(sizeof(Factor));
        f1 = f1->next;
        f1->next = NULL;
        f1->val = f2->val;
        f2 = f2->next;
      }
      t2 = t2->next;
    }
    t3 = t3->next;
  } 
  return form;
}

PSForm div_psf(PSForm *a, PSForm *b, int *err) {
  *err = 0;
  PSForm form;
  form.terms = copy_terms(a->terms);

  /* более одного слагаемого в PS-форме делителя */
  if (!a || !b || !b->terms || b->terms->next) {
    *err = 1;
    return form;
  }
  Term *t1 = form.terms;
  Factor *f1, *f2, *ff1;

  /* для каждого слагаемого в PS-форме делимого */
  while (t1) {
    f2 = b->terms->factors;
    f1 = t1->factors;

    /* проверка на деление нацело констант */
    if (f2->val != 0 && f1->val % f2->val == 0) 
      f1->val /= f2->val;
    else {
      *err = 1;
      return form;
    }
    ff1 = f1;
    f1 = f1->next;
    f2 = f2->next;

    /* удаление множителей (переменных) */
    while (f1) {
      /* достигнут конец слагаемого из делителя */
      if (!f2)
        break;
      if (f1->val == f2->val) {
        ff1->next = f1->next;
        free(f1);
        f1 = t1->factors;
        f2 = f2->next; 
      }
      else {
        ff1 = f1;
        f1 = f1->next;
      }
    }
    if (f2) {
      *err = 1;
      return form;
    }
    t1 = t1->next;
  }
  return form;
}

int compare_terms(Term *t1, Term *t2) {
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
  /* сравнение констант */
  if (f1->val != f2->val)
    return 1;

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

int compare_psf(PSForm *a, PSForm *b) {
  if (!a && !b)
    return 0;
  Term *t1 = a->terms, *t2 = b->terms;
  int count1 = 0, count2 = 0; 

  /* проверка на одинаковое число слагаемых */
  while (t1) {
    count1++;
    t1 = t1->next;
  }
  while (t2) {
    count2++;
    t2 = t2->next;
  }
  if (count1 != count2)
    return 1;

  t1 = a->terms;
 
  /* поиск слагаемого из 1-ой формы во 2-ой */
  while (t1) {
    t2 = b->terms;

    while (t2) {
      if (!compare_terms(t1, t2))
        break;
      t2 = t2->next;   
    }
    if (!t2)
      return 1;
    t1 = t1->next;
  }
  return 0;  
}
