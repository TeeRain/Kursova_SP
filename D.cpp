#pragma once
#include "D.h"

enum TypeOfLex
{
	LProgram,		// Startprogram
	LProgramName,	// ProgramName
	LBody,			// startblock
	LVar,			// variable
	LVarType,		// Int_2
	LIdentifier,	// NameVar
	LEnd,			// endblock

	LInput,			// Scan
	LOutput,		// Print

	LLabel,			// Label

	LFor,			// For

	LIf,			//If
	LBodyIf,		//BodyIf
	LElse,			//Else
	LEndIf,			//EndIf


	LNewValue,		// >>
	LAdd,			// +
	LSub,			// -
	LMul,			// Mul
	LDiv,			// Div
	LMod,			// Mod
	LEg,			// ==
	LNe,			// !=
	LLe,			// <=
	LGe,			// >=
	LNot,			// !!
	LAnd,			// &&
	LOr,			// ||

	LLBraket,		// (
	LRBraket,		// )
	LNumber,		// number
	LSeparator,		// ;
	LComma,			// ,
	LEOF,			// EndFile
	LComentar,
	LUnknown
};

typedef struct Lexem
{
	char name[50];
	int value;
	int line;
	TypeOfLex type;
}Lexema;

typedef struct ID
{
	char name[50];
	int value;
}Identificator;


typedef struct L
{
	Lexema TableLexems[MAX_LEXEMS];			//Таблиця лексем
	int LexNum;								//

	Identificator IdTable[MAX_IDENT];		//Таблиця ідентифікаторів
	int IdNum;								//

	bool IsPresentInput,					//Ознаки присутності операторів
		IsPresentOutput,
		IsPresentAnd,
		IsPresentOr,
		IsPresentNot,
		IsPresentEqu,
		IsPresentGreate,
		IsPresentLess,
		IsPresentDiv;

	int numberErrors;

	std::string InputFileName;
	/*std::string*/ char OutputFileName[50];

	int bufExprPostfixForm[MAX_BUF_SIZE];	//Буфер для виразу в постфіксній формі
}DataType;

typedef struct Stacks
{
	int st[STACK_SIZE];
	int top;
}StackType;

typedef class stack
{
public:
	StackType S;
	void Init(StackType* s)
	{
		s->top = -1;
	}

	void Push(int i, StackType* s)
	{
		if (IsFull(s))
		{
			puts("Stack error (is full)");
			exit(0);
		}
		else
		{
			++s->top;
			s->st[s->top] = i;
		}
	}
	int Pop(StackType* s)
	{
		int i;
		if (IsEmpty(s))
		{
			puts("Stack error (is empty)");
			exit(0);
		}
		else
		{
			i = s->st[s->top];
			--s->top;
		}
		return i;
	}
	bool IsEmpty(StackType* s)
	{
		if (s->top == -1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	bool IsFull(StackType* s)
	{
		if (s->top == 199)return true;
		else return false;
	}
	void prints(StackType s)
	{
		int i = 0;
		for (; i < 10; ++i)
		{
			printf("%d_", s.st[i]);
		}
	}
}StackT;

extern FILE* ErrorF;

extern DataType Data;