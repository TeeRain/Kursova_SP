#include "G.h"

StackT Stack, StackIf, forStack, startStack;
int inner_for_flag = 0;

void BeginASMFile(FILE* fout)
{
	fprintf(fout, ".586\n.model flat, stdcall\noption casemap :none\n");
	fprintf(fout, "include \\masm32\\include\\windows.inc\ninclude \\masm32\\include\\kernel32.inc\ninclude \\masm32\\include\\masm32.inc\ninclude \\masm32\\include\\user32.inc\ninclude \\masm32\\include\\msvcrt.inc\n");
	fprintf(fout, "includelib \\masm32\\lib\\kernel32.lib\nincludelib \\masm32\\lib\\masm32.lib\nincludelib \\masm32\\lib\\user32.lib\nincludelib \\masm32\\lib\\msvcrt.lib\n");
	fprintf(fout, "\n.DATA");
}

void BeginCodeSegm(FILE* fout)
{
	fprintf(fout, "\n.CODE\n");
	fprintf(fout, "start:\n");
	fprintf(fout, "invoke AllocConsole\n");
	fprintf(fout, "invoke GetStdHandle, STD_INPUT_HANDLE\n");
	fprintf(fout, "mov hConsoleInput, EAX\n");
	fprintf(fout, "invoke GetStdHandle, STD_OUTPUT_HANDLE\n");
	fprintf(fout, "mov hConsoleOutput, EAX\n");
}

void CheckPresent()			//Визначення присутності операторів put i get
{
	for (int i = 0; Data.TableLexems[i].type != LEOF; ++i)
	{
		if (Data.TableLexems[i].type == LInput) Data.IsPresentInput = true;
		if (Data.TableLexems[i].type == LOutput) Data.IsPresentOutput = true;
		if (Data.TableLexems[i].type == LAnd) Data.IsPresentAnd = true;
		if (Data.TableLexems[i].type == LOr) Data.IsPresentOr = true;
		if (Data.TableLexems[i].type == LNot || Data.TableLexems[i].type == LNe) Data.IsPresentNot = true;
		if (Data.TableLexems[i].type == LEg || Data.TableLexems[i].type == LNe) Data.IsPresentEqu = true;
		if (Data.TableLexems[i].type == LGe) Data.IsPresentGreate = true;
		if (Data.TableLexems[i].type == LLe) Data.IsPresentLess = true;
		if (Data.TableLexems[i].type == LDiv) Data.IsPresentDiv = true;
		if (Data.IsPresentInput &&
			Data.IsPresentOutput &&
			Data.IsPresentAnd &&
			Data.IsPresentOr &&
			Data.IsPresentNot &&
			Data.IsPresentEqu &&
			Data.IsPresentGreate &&
			Data.IsPresentLess &&
			Data.IsPresentDiv) break;
	}
}

void PrintData(FILE* f)				//Друк сегмету даних
{
	fputs("\n;-------------------------------------------------------------------------", f);
	fputs("\n;------DataOdUser---------------------------------------------------------", f);
	fputs("\n;-------------------------------------------------------------------------\n", f);
	int i;
	for (i = 0; i < Data.IdNum; ++i)
	{
		fprintf(f, "\t%s\tdd\t0%xh\n", Data.IdTable[i].name, Data.IdTable[i].value);
	}
	fputs("\n;-------------------------------------------------------------------------", f);
	fputs("\n;------AdditionData-------------------------------------------------------", f);
	fputs("\n;-------------------------------------------------------------------------\n", f);
	fputs("\thConsoleInput\tdd\t?\n", f);
	fputs("\thConsoleOutput\tdd\t?\n", f);
	fputs("\tErrorMessage\tdb\t\"Error: division by zero\", 0\n", f);
	fputs("\tInputBuf\tdb\t15 dup (?)\n", f);
	fputs("\tInMessage\tdb\t\"GET: \", 0\n", f);
	fputs("\tOutMessage\tdb\t\"PUT: \",\" %d\",0\n", f);
	fputs("\tResMessage\tdb\t20 dup (?)\n", f);
	fputs("\tNumberOfCharsRead\tdd\t?\n", f);
	fputs("\tNumberOfCharsWrite\tdd\t?\n", f);
	fputs("\tmsg1310\tdw\t13,10\n", f);
	fputs("\tbuf\tdd\t0\n", f);

	fputs("\tlb1\tdd\t?\n", f); //Змінні для обробки логічних операцій
	fputs("\tlb2\tdd\t?\n", f);
}

void PrintAND(FILE* f)
{
	fputs("\n;-------------------------------------------------------------------------", f);
	fputs("\n;------ProcedureAnd-------------------------------------------------------", f);
	fputs("\n;-------------------------------------------------------------------------\n", f);
	fputs("And_ PROC\n", f);
	fputs("\tpop ebx\n", f);
	fputs("\tpop lb1\n", f);
	fputs("\tpop lb2\n", f);

	fputs("\tmov eax,lb1\n", f);
	fputs("\tcmp eax,0\n", f);
	fputs("\tjnz and_t1\n", f);
	fputs("\tjz and_false\n", f);
	fputs("and_t1:\n", f);
	fputs("\tmov eax,lb2\n", f);
	fputs("\tcmp eax,0\n", f);
	fputs("\tjnz and_true\n", f);
	fputs("and_false:\n", f);
	fputs("\tpush 0\n", f);
	fputs("\tjmp and_fin\n", f);
	fputs("and_true:\n", f);
	fputs("\tpush 1\n", f);

	fputs("and_fin:\n\n", f);
	fputs("\tpush ebx\n", f);
	fputs("\tret\n", f);
	fputs("And_ ENDP\n", f);
	fputs("\n;-------------------------------------------------------------------------\n\n", f);
}
void PrintOR(FILE* f)
{
	fputs("\n;-------------------------------------------------------------------------", f);
	fputs("\n;------ProcedureOr--------------------------------------------------------", f);
	fputs("\n;-------------------------------------------------------------------------\n", f);
	fputs("Or_ PROC\n", f);
	fputs("\tpop ebx\n", f);
	fputs("\tpop lb1\n", f);
	fputs("\tpop lb2\n", f);

	fputs("\tmov eax,lb1\n", f);
	fputs("\tcmp eax,0\n", f);
	fputs("\tjnz or_true\n", f);
	fputs("\tmov eax,lb2\n", f);
	fputs("\tcmp eax,0\n", f);
	fputs("\tjnz or_true\n", f);
	fputs("\tpush 0\n", f);
	fputs("\tjmp or_fin\n", f);
	fputs("or_true:\n", f);
	fputs("\tpush 1\n", f);

	fputs("or_fin:\n\n", f);
	fputs("\tpush ebx\n", f);
	fputs("\tret\n", f);
	fputs("Or_ ENDP\n", f);
	fputs("\n;-------------------------------------------------------------------------\n\n", f);
}
void PrintNOT(FILE* f)
{
	fputs("\n;-------------------------------------------------------------------------", f);
	fputs("\n;------ProcedureNot-------------------------------------------------------", f);
	fputs("\n;-------------------------------------------------------------------------\n", f);
	fputs("Not_ PROC\n", f);
	fputs("\tpop ebx\n", f);
	fputs("\tpop lb1\n", f);

	fputs("\tmov eax,lb1\n", f);
	fputs("\tcmp eax,0\n", f);
	fputs("\tjne not_false\n", f);
	fputs("\tpush 1\n", f);
	fputs("\tjmp not_fin\n", f);
	fputs("not_false:\n", f);
	fputs("\tpush 0\n", f);

	fputs("not_fin:\n\n", f);
	fputs("\tpush ebx\n", f);
	fputs("\tret\n", f);
	fputs("Not_ ENDP\n", f);
	fputs("\n;-------------------------------------------------------------------------\n\n", f);
}
void PrintEQ(FILE* f)
{
	fputs("\n;-------------------------------------------------------------------------", f);
	fputs("\n;------ProcedureEq--------------------------------------------------------", f);
	fputs("\n;-------------------------------------------------------------------------\n", f);
	fputs("Eq_ PROC\n", f);
	fputs("\tpop ebx\n", f);
	fputs("\tpop lb2\n", f);
	fputs("\tpop lb1\n", f);

	fputs("\tmov eax,lb1\n", f);
	fputs("\tmov edx,lb2\n", f);
	fputs("\tcmp eax,edx\n", f);
	fputs("\tjne not_eq\n", f);
	fputs("\tpush 1\n", f);
	fputs("\tjmp eq_fin\n", f);
	fputs("not_eq:\n", f);
	fputs("\tpush 0\n", f);

	fputs("eq_fin:\n", f);
	fputs("\tpush ebx\n", f);
	fputs("\tret\n", f);
	fputs("Eq_ ENDP\n", f);
	fputs("\n;-------------------------------------------------------------------------\n\n", f);
}
void PrintGE(FILE* f)
{
	fputs("\n;-------------------------------------------------------------------------", f);
	fputs("\n;------ProcedureGreate----------------------------------------------------", f);
	fputs("\n;-------------------------------------------------------------------------\n", f);
	fputs("Greate_ PROC\n", f);
	fputs("\tpop ebx\n", f);
	fputs("\tpop lb2\n", f);
	fputs("\tpop lb1\n", f);
	fputs("\tmov eax, lb1\n", f);
	fputs("\tmov edx, lb2\n", f);

	fputs("\tcmp eax,edx\n", f);
	fputs("\tjle lov\n", f);
	fputs("\tpush 1\n", f);
	fputs("\tjmp gr_fin\n", f);
	fputs("lov:\n", f);
	fputs("\tpush 0\n", f);

	fputs("gr_fin:\n", f);
	fputs("\tpush ebx\n", f);
	fputs("\tret\n", f);
	fputs("Greate_ ENDP\n", f);
	fputs("\n;-------------------------------------------------------------------------\n\n", f);
}
void PrintLE(FILE* f)
{
	fputs("\n;-------------------------------------------------------------------------", f);
	fputs("\n;------ProcedureLess------------------------------------------------------", f);
	fputs("\n;-------------------------------------------------------------------------\n", f);
	fputs("Less_ PROC\n", f);
	fputs("\tpop ebx\n", f);
	fputs("\tpop lb2\n", f);
	fputs("\tpop lb1\n", f);
	fputs("\tmov eax, lb1\n", f);
	fputs("\tmov edx, lb2\n", f);
	fputs("\tcmp eax,edx\n", f);
	fputs("\tjge gr\n", f);
	fputs("\tpush 1\n", f);
	fputs("\tjmp less_fin\n", f);
	fputs("gr:\n", f);
	fputs("\tpush 0\n", f);

	fputs("less_fin:\n", f);
	fputs("\tpush ebx\n", f);
	fputs("\tret\n", f);
	fputs("Less_ ENDP\n", f);
	fputs("\n;-------------------------------------------------------------------------\n\n", f);
}
void PrintCheck(FILE* f)
{
	fputs("\n;-------------------------------------------------------------------------", f);
	fputs("\n;------ProcedureCheck-----------------------------------------------------", f);
	fputs("\n;-------------------------------------------------------------------------\n", f);
	fputs("Check_ PROC\n", f);
	fputs("\tpop edx\n", f);
	fputs("\tpop eax\n", f);
	fputs("\tcmp eax,0\n", f);
	fputs("\tjne end_check\n", f);
	fputs("invoke WriteConsoleA,hConsoleOutput,ADDR msg1310,SIZEOF msg1310,ADDR NumberOfCharsWrite,0\n", f);
	fputs("\tinvoke WriteConsoleA, hConsoleOutput, ADDR ErrorMessage, SIZEOF ErrorMessage, ADDR NumberOfCharsWrite, 0\n", f);
	fputs("\tjmp exit_label\n", f);

	fputs("end_check:\n", f);
	fputs("\tpush eax\n", f);
	fputs("\tpush edx\n", f);
	fputs("\tret\n", f);
	fputs("Check_ ENDP\n", f);
	fputs("\n;-------------------------------------------------------------------------\n\n", f);
}

void PrintInput(FILE* f)
{
	fputs("\n;-------------------------------------------------------------------------", f);
	fputs("\n;------ProcedureInput-----------------------------------------------------", f);
	fputs("\n;-------------------------------------------------------------------------\n", f);
	fputs("Input PROC\n", f);
	fputs("invoke WriteConsoleA,hConsoleOutput,ADDR msg1310,SIZEOF msg1310,ADDR NumberOfCharsWrite,0\n", f);
	fputs("invoke WriteConsoleA,hConsoleOutput,ADDR InMessage,SIZEOF InMessage,ADDR NumberOfCharsWrite,0\n", f);
	fputs("invoke ReadConsoleA,hConsoleInput,ADDR InputBuf,11,ADDR NumberOfCharsRead,0\n", f);
	fputs("invoke  crt_atoi, addr InputBuf\n", f);
	fputs("mov dword ptr buf, eax\n", f);
	fputs("ret\n", f);
	fputs("Input ENDP\n", f);
	fputs("\n;-------------------------------------------------------------------------\n\n", f);


}
void PrintOutput(FILE* f)
{
	fputs("\n;-------------------------------------------------------------------------", f);
	fputs("\n;------ProcedureOutput----------------------------------------------------", f);
	fputs("\n;-------------------------------------------------------------------------\n", f);
	fputs("Output PROC\n", f);
	fputs("invoke WriteConsoleA,hConsoleOutput,ADDR msg1310,SIZEOF msg1310,ADDR NumberOfCharsWrite,0\n", f);
	fputs("invoke wsprintf, addr ResMessage, addr OutMessage, dword ptr buf\n", f);
	fputs("invoke WriteConsoleA,hConsoleOutput,ADDR ResMessage,eax,ADDR NumberOfCharsWrite,0\n", f);
	fputs("ret\n", f);
	fputs("Output ENDP\n", f);
	fputs("\n;-------------------------------------------------------------------------\n\n", f);


}



void PrintEnding(FILE* f)
{
	fputs("exit_label:\n", f);
	fputs("invoke WriteConsoleA,hConsoleOutput,ADDR msg1310,SIZEOF msg1310,ADDR NumberOfCharsWrite,0\n", f);
	fputs("invoke ReadConsoleA,hConsoleInput,ADDR InputBuf,1,ADDR NumberOfCharsRead,0\n", f);

	fputs("invoke ExitProcess, 0\n", f);
	if (Data.IsPresentInput) PrintInput(f);
	if (Data.IsPresentOutput) PrintOutput(f);
	if (Data.IsPresentAnd) PrintAND(f);
	if (Data.IsPresentOr) PrintOR(f);
	if (Data.IsPresentNot) PrintNOT(f);
	if (Data.IsPresentEqu) PrintEQ(f);
	if (Data.IsPresentGreate) PrintGE(f);
	if (Data.IsPresentLess) PrintLE(f);
	if (Data.IsPresentDiv) PrintCheck(f);
	fputs("end start\n", f);
}

bool Prioritet(TypeOfLex t, StackType s)
{
	bool r;
	r = (
		((t == LDiv) && (Data.TableLexems[s.st[s.top]].type == LAdd)) ||
		((t == LDiv) && (Data.TableLexems[s.st[s.top]].type == LSub)) ||
		((t == LDiv) && (Data.TableLexems[s.st[s.top]].type == LOr)) ||
		((t == LDiv) && (Data.TableLexems[s.st[s.top]].type == LAnd)) ||
		((t == LDiv) && (Data.TableLexems[s.st[s.top]].type == LEg)) ||
		((t == LDiv) && (Data.TableLexems[s.st[s.top]].type == LNe)) ||
		((t == LDiv) && (Data.TableLexems[s.st[s.top]].type == LLe)) ||
		((t == LDiv) && (Data.TableLexems[s.st[s.top]].type == LGe)) ||

		((t == LMul) && (Data.TableLexems[s.st[s.top]].type == LAdd)) ||
		((t == LMul) && (Data.TableLexems[s.st[s.top]].type == LSub)) ||
		((t == LMul) && (Data.TableLexems[s.st[s.top]].type == LOr)) ||
		((t == LMul) && (Data.TableLexems[s.st[s.top]].type == LAnd)) ||
		((t == LMul) && (Data.TableLexems[s.st[s.top]].type == LEg)) ||
		((t == LMul) && (Data.TableLexems[s.st[s.top]].type == LNe)) ||
		((t == LMul) && (Data.TableLexems[s.st[s.top]].type == LLe)) ||
		((t == LMul) && (Data.TableLexems[s.st[s.top]].type == LGe)) ||

		((t == LMod) && (Data.TableLexems[s.st[s.top]].type == LAdd)) ||
		((t == LMod) && (Data.TableLexems[s.st[s.top]].type == LSub)) ||
		((t == LMod) && (Data.TableLexems[s.st[s.top]].type == LOr)) ||
		((t == LMod) && (Data.TableLexems[s.st[s.top]].type == LAnd)) ||
		((t == LMod) && (Data.TableLexems[s.st[s.top]].type == LEg)) ||
		((t == LMod) && (Data.TableLexems[s.st[s.top]].type == LNe)) ||
		((t == LMod) && (Data.TableLexems[s.st[s.top]].type == LLe)) ||
		((t == LMod) && (Data.TableLexems[s.st[s.top]].type == LGe)) ||

		((t == LNot) && (Data.TableLexems[s.st[s.top]].type == LAdd)) ||
		((t == LNot) && (Data.TableLexems[s.st[s.top]].type == LSub)) ||
		((t == LNot) && (Data.TableLexems[s.st[s.top]].type == LDiv)) ||
		((t == LNot) && (Data.TableLexems[s.st[s.top]].type == LMul)) ||
		((t == LNot) && (Data.TableLexems[s.st[s.top]].type == LMod)) ||
		((t == LNot) && (Data.TableLexems[s.st[s.top]].type == LOr)) ||
		((t == LNot) && (Data.TableLexems[s.st[s.top]].type == LAnd)) ||
		((t == LNot) && (Data.TableLexems[s.st[s.top]].type == LEg)) ||
		((t == LNot) && (Data.TableLexems[s.st[s.top]].type == LNe)) ||
		((t == LNot) && (Data.TableLexems[s.st[s.top]].type == LLe)) ||
		((t == LNot) && (Data.TableLexems[s.st[s.top]].type == LGe))
		);
	return r;
}

int ConvertToPostfixForm(int i) //Формує в масиві послідовність номерів лексем яка відповідає постфіксній формі
{
	Stack.Init(&Stack.S);
	int n, z;
	n = 0;
	for (; ((Data.TableLexems[i + n].type != LSeparator) && (Data.TableLexems[i + n].type != LBodyIf)); ++n);		//Встановлення коректності та довжини вхідного масиву
	int k;
	k = i + n;
	for (z = 0; i < k; ++i)
	{
		TypeOfLex in;
		in = Data.TableLexems[i].type;

		if ((in == LIdentifier) || (in == LNumber))
		{
			Data.bufExprPostfixForm[z] = i;
			++z;
		}
		else if (IsOperation(in))
		{
			if (Stack.IsEmpty(&Stack.S) || Prioritet(in, Stack.S))
			{
				Stack.Push(i, &Stack.S);
			}
			else
			{
				if (Data.TableLexems[Stack.S.st[Stack.S.top]].type != LLBraket)
				{
					do
					{
						Data.bufExprPostfixForm[z] = Stack.Pop(&Stack.S);
						++z;
					} while ((!(Prioritet(in, Stack.S))) && (!(Stack.IsEmpty(&Stack.S))) && (Data.TableLexems[Stack.S.st[Stack.S.top]].type != LLBraket));
				}
				Stack.Push(i, &Stack.S);
			}
		}
		if (in == LLBraket)
		{
			Stack.Push(i, &Stack.S);
			Data.bufExprPostfixForm[z] = i;
			++z;
		}
		if (in == LRBraket)
		{
			for (; (Data.TableLexems[Stack.S.st[Stack.S.top]].type != LLBraket);)
			{
				Data.bufExprPostfixForm[z] = Stack.Pop(&Stack.S);
				++z;
			}
			Stack.Pop(&Stack.S);
			Data.bufExprPostfixForm[z] = i;
			++z;
		}
	}
	for (; !(Stack.IsEmpty(&Stack.S));)
	{
		Data.bufExprPostfixForm[z] = Stack.Pop(&Stack.S);
		++z;
	}
	Data.bufExprPostfixForm[z] = 3000;
	z++;
	return k;
}

void GenASMCode(const char* str, FILE* f)
{
	int n;
	for (n = 0; Data.bufExprPostfixForm[n] != 3000; ++n)
	{
		Stack.Init(&Stack.S);

		if ((!IsOperation(Data.TableLexems[Data.bufExprPostfixForm[n]].type)) && (Data.TableLexems[Data.bufExprPostfixForm[n]].type != LNot) && (Data.TableLexems[Data.bufExprPostfixForm[n - 1]].type != LNot))
		{
			if (Data.TableLexems[Data.bufExprPostfixForm[n]].type == LIdentifier)
			{
				fprintf(f, "\tpush %s\n", Data.TableLexems[Data.bufExprPostfixForm[n]].name);
			}
			else if (Data.TableLexems[Data.bufExprPostfixForm[n]].type == LNumber)
			{
				char buf[9];
				sprintf(&buf[0], "%d", abs(Data.TableLexems[Data.bufExprPostfixForm[n]].value));
				buf[8] = '\0';
				fprintf(f, "\tmov dword ptr buf, %s\n", buf);
				fputs("\tpush buf\n", f);
				if (Data.TableLexems[Data.bufExprPostfixForm[n]].value < 0)
				{
					fputs("\tpop buf\n", f);
					fputs("\tneg buf\n", f);
					fputs("\tpush buf\n", f);
				}
			}
			else if ((Data.TableLexems[Data.bufExprPostfixForm[n]].type == LLBraket) || (Data.TableLexems[Data.bufExprPostfixForm[n]].type == LRBraket))
			{
				continue;
			}
		}
		else
		{
			switch (Data.TableLexems[Data.bufExprPostfixForm[n]].type)
			{
			case LAdd: fputs("\tpop edx\n", f);
				fputs("\tpop eax\n", f);
				fputs("\tadd eax,edx\n", f);
				fputs("\tpush eax\n", f);
				break;
			case LSub: fputs("\tpop edx\n", f);
				fputs("\tpop eax\n", f);
				fputs("\tsub eax,edx\n", f);
				fputs("\tpush eax\n", f);
				break;
			case LDiv: fputs("\tcall Check_\n", f);
				fputs("\tpop ebx\n", f);
				fputs("\tpop eax\n", f);
				fputs("\tcdq\n", f);
				fputs("\tidiv ebx\n", f);
				fputs("\tpush eax\n", f);
				break;
			case LMod: fputs("\tpop ebx\n", f);
				fputs("\tpop eax\n", f);
				fputs("\tcdq\n", f);
				fputs("\tidiv ebx\n", f);
				fputs("\tpush edx\n", f);
				break;
			case LMul: fputs("\tpop edx\n", f);
				fputs("\tpop eax\n", f);
				fputs("\timul edx\n", f);
				fputs("\tpush eax\n", f);
				break;
			case LAnd: fputs("\tcall And_\n", f);
				break;
			case LOr: fputs("\tcall Or_\n", f);
				break;
			case LNot: fputs("\tcall Not_\n", f);
				break;
			case LEg: fputs("\tcall Eq_\n", f);
				break;
			case LNe:
				fputs("\tcall Eq_\n", f);
				fputs("\tcall Not_\n", f);
				break;
			case LLe: fputs("\tcall Less_\n", f); break;
			case LGe: fputs("\tcall Greate_\n", f); break;
			}
		}
	}
	if (inner_for_flag == 0) fprintf(f, "\tpop %s\n", str);
	else fprintf(f, "\tpop buf\n");
}

void PrintCode(FILE* f)
{
	int lab = 0;
	int ifLabelIndex = 0;
	Lexema l;		//Поточна лексема
	int i = 0;
	StackIf.Init(&StackIf.S);
	forStack.Init(&forStack.S);

	do			//Пошук початку коду
	{
		++i;
	} while (Data.TableLexems[i].type != LVarType);
	++i;
	if (Data.TableLexems[i].type == LVarType)
	{
		do
		{
			i++;
		} while (Data.TableLexems[i].type != LSeparator);
		i++;
	}

	for (;; ++i)
	{
		l.type = Data.TableLexems[i].type;
		strncpy(l.name, Data.TableLexems[i].name, 50);
		l.value = Data.TableLexems[i].value;

		if (l.type == LEOF)
		{
			break;
		}



		if (l.type == LIf)
		{
			ifLabelIndex++;
			StackIf.Push(ifLabelIndex, &StackIf.S);
			i = ConvertToPostfixForm(i + 1);
			GenASMCode("buf", f);
			fputs("\tcmp word ptr buf, 0\n", f);
			//fprintf(f, "\tje ifLabel%d\n", ifLabelIndex);
			for (size_t j = i + 1; Data.TableLexems[j].type != LEOF; j++)
			{
				if (Data.TableLexems[j].type == LElse)
				{
					fprintf(f, "\tje ifLabel%d\n", ifLabelIndex);
					break;
				}
				else if (Data.TableLexems[j].type == LEndIf)
				{
					fprintf(f, "\tje endIf%d\n", ifLabelIndex);
					break;
				}
			}
		}
		if (l.type == LElse)
		{
			fprintf(f, "\tjmp endIf%d\n", ifLabelIndex);
			fprintf(f, "ifLabel%d:\n", ifLabelIndex);
		}
		if (l.type == LEndIf)
		{
			fprintf(f, "endIf%d:\n", ifLabelIndex);
		}


		if (l.type == LLabel)
		{
			fprintf(f, "%s\n", Data.TableLexems[i].name);
		}
		if (l.type == LOutput)
		{
			i = ConvertToPostfixForm(i + 1);
			GenASMCode("buf", f);
			fputs("\tcall Output\n", f);
		}
		if (l.type == LInput)
		{
			fputs("\tcall Input\n", f);
			fprintf(f, "\tpush buf\n");
			fprintf(f, "\tpop %s\n", Data.TableLexems[i + 2].name);
			i += 4;
		}
		if (l.type == LBody)
		{

			startStack.Push(++lab, &startStack.S);
			if ((Data.TableLexems[i - 4].type == LAdd) || (Data.TableLexems[i - 4].type == LSub)) startStack.Push(1, &startStack.S);
			//if (tokensTable[i - 1].type == ltEndGroup) startStack.push(2, &startStack.S);
			else startStack.Push(2, &startStack.S);
		}
		if ((l.type == LEnd) && (Data.TableLexems[i + 1].type != LEOF))
		{
			if (startStack.S.top >= 0)
			{
				int temp1, temp2;
				temp2 = forStack.Pop(&forStack.S);
				///////////////////////////////////
				temp2 = forStack.Pop(&forStack.S);
				/////////////////////////////////
				temp1 = startStack.Pop(&startStack.S);
				fprintf(f, "\tmov ax, word ptr %s\n", Data.TableLexems[temp2].name);
				if (temp1 == 2) fprintf(f, "\tdec ax\n");
				else fprintf(f, "\tinc ax\n");

				fprintf(f, "\tmov word ptr %s, ax\n", Data.TableLexems[temp2].name);
				temp1 = startStack.Pop(&startStack.S);
				fprintf(f, "\tjmp forStart%d\n", temp1);
				fprintf(f, "forFinish%d:\n", temp1);
			}
			else
			{
				startStack.Pop(&startStack.S);
				startStack.Pop(&startStack.S);
			}
		}

		if (l.type == LFor)
		{
			forStack.Push(i + 2, &forStack.S);
			int bufi;
			int temp = i + 2;
			i += 3;
			bufi = i;
			i = ConvertToPostfixForm(i + 1);//Генерація постфіксного виразу			
			if (i < 0)
			{
				i = -i;
				puts("IE\n");
				continue;
			}
			//Генерація асемблерного коду з постфіксного виразу
			GenASMCode(Data.TableLexems[bufi - 1].name, f);
			i++;

			fprintf(f, "forStart%d:\n", lab + 1);



			forStack.Push(i, &forStack.S);

			int operand_index = i;
			int operation_index = ++i;
			bufi = i;

			i = ConvertToPostfixForm(i + 1);//Генерація постфіксного виразу			
			inner_for_flag = 1;
			GenASMCode(Data.TableLexems[bufi - 1].name, f);
			inner_for_flag = 0;

			fprintf(f, "\tpush %s\n", Data.TableLexems[operand_index].name);

			fprintf(f, "\tpush buf\n");
			if (Data.TableLexems[operation_index].type == LGe)	fputs("\tcall Greate_\n", f);
			else if (Data.TableLexems[operation_index].type == LLe)	fputs("\tcall Less_\n", f);
			else if (Data.TableLexems[operation_index].type == LEg)	fputs("\tcall Eq_\n", f);
			else if (Data.TableLexems[operation_index].type == LNe) fputs("\tcall Not_\n", f);

			fputs("\tpop buf\n", f);
			fputs("\tmov ax,word ptr buf\n", f);
			fputs("\tcmp ax,0\n", f);
			fprintf(f, "\tjz forFinish%d\n", lab + 1);
			i = i + 7;
		}
		if ((l.type == LNewValue) && (Data.TableLexems[i - 3].type != LFor))
		{
			int bufi;
			bufi = i;
			i = ConvertToPostfixForm(i + 1);//Генерація постфіксного виразу			
			if (i < 0)
			{
				i = -i;
				puts("IE\n");
				continue;
			}
			//Генерація асемблерного коду з постфіксного виразу
			GenASMCode(Data.TableLexems[bufi - 1].name, f);
		}
	}
}

void GenerateCode(FILE* f)
{
	BeginASMFile(f);
	CheckPresent();
	PrintData(f);
	BeginCodeSegm(f);

	PrintCode(f);

	PrintEnding(f);
}