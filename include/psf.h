#ifndef PSF_H
#define PSF_H

/* Переменная в слагаемом умещается в тип char */
#define MAX_ASCII 256

typedef struct Factor Factor;
typedef struct Term Term;

struct Factor {
  int val; /* Значение: для констант - число, для переменных - ASCII-код символа */
  Factor *next;
}; 

struct Term {
  Factor *factors; /* Список множителей (первый - константа; остальные - переменные) */
  Term *next;
};

typedef struct {
  Term *terms; /* Список слагаемых PS-формы */
} PSForm;

/* Парсинг строки в PS-форму */
PSForm parse_psf(const char *);

void print_psf(PSForm *);
void free_psf(PSForm *);

/* Операции над PS-формами */

/* result = a + b */
PSForm add_psf(PSForm *a, PSForm *b);
/* result = a - b */
PSForm sub_psf(PSForm *a, PSForm *b);
/* result = a * b */
PSForm mul_psf(PSForm *a, PSForm *b);
/* result = a / b */
PSForm div_psf(PSForm *a, PSForm *b, int *err);
/* result = (a == b) */
int compare_psf(PSForm *a, PSForm *b);

/* приведение PS-формы к каноническому виду */
void simplify_psf(PSForm *);

#endif
