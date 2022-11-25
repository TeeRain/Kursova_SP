#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"
#include "malloc.h"
#include "string.h"
#include "stdlib.h"
#include "ctype.h"
#include <string>

#define MAX_LEXEMS 1000
#define MAX_IDENT 100
#define MAX_BUF_SIZE 100
#define STACK_SIZE 200
#define MAX_LENGTH_TYPES 20
#define LEX_SIZE 28

extern enum TypeOfLex;

typedef struct Lexem;

typedef struct ID;

typedef struct L;

typedef struct Stacks;

typedef class stack;
 