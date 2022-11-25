#include "G.h"

bool IsOperation(TypeOfLex t)
{
	bool r;
	r = ((t == LAdd) || (t == LSub) || (t == LMul) || (t == LDiv) || (t == LMod) || (t == LNot) ||
		(t == LAnd) || (t == LOr) || (t == LEg) || (t == LNe) || (t == LLe) || (t == LGe));
	return r;
}
int IsExpression(int i, FILE* ef)
{
	int nom, error = 0;
	nom = i;
	if ((Data.TableLexems[nom].type != LLBraket) && (Data.TableLexems[nom].type != LIdentifier) && (Data.TableLexems[nom].type != LNumber))
	{
		fprintf(ef, "line %d: \tExpression must begin from identifier, number or '('!\n", Data.TableLexems[nom].line);
		error++;
	}
	for (; (Data.TableLexems[nom].type != LSeparator); nom++)
	{
		if (Data.TableLexems[nom].type == LRBraket)
			if (!(IsOperation(Data.TableLexems[nom + 1].type)) && (Data.TableLexems[nom + 1].type != LSeparator) && (Data.TableLexems[nom + 1].type != LRBraket))
			{
				fprintf(ef, "line %d: \tAfter ')' must be ')', operation or ';'!\n", Data.TableLexems[nom].line);
				error++;
			}
		if (Data.TableLexems[nom].type == LLBraket)
		{
			if ((Data.TableLexems[nom + 1].type != LIdentifier) && (Data.TableLexems[nom + 1].type != LLBraket) &&
				(Data.TableLexems[nom + 1].type != LNumber) && (Data.TableLexems[nom + 1].type != LNot))
			{
				fprintf(ef, "line %d: \tAfter '(' must be '(' or identifier!\n", Data.TableLexems[nom].line);
				error++;
			}
		}
		if (IsOperation(Data.TableLexems[nom].type) && Data.TableLexems[nom].type != LNot)
		{
			if ((Data.TableLexems[nom + 1].type != LIdentifier) && (Data.TableLexems[nom + 1].type != LLBraket) && (Data.TableLexems[nom + 1].type != LNumber) && (Data.TableLexems[nom + 1].type != LNot))
			{
				fprintf(ef, "line %d: \tAfter operation must be '(' or idetifier!\n", Data.TableLexems[nom].line);
				error++;
			}
		}
		if ((Data.TableLexems[nom].type == LIdentifier) || (Data.TableLexems[nom].type == LNumber))
		{
			if (!(IsOperation(Data.TableLexems[nom + 1].type)) && (Data.TableLexems[nom + 1].type != LRBraket) && (Data.TableLexems[nom + 1].type != LSeparator))
			{
				fprintf(ef, "line %d: \tAfter identifier must be ')' or ';' or operation!\n", Data.TableLexems[nom].line);
				error++;
			}
		}
	}
	return error;
}

int Balans(int nom, TypeOfLex ends, TypeOfLex ltStart, TypeOfLex ltFinish)
{
	StackT ss;
	int j = 0, i;
	i = nom;
	ss.Init(&ss.S);
	for (; Data.TableLexems[i].type != ends; i++)
	{
		if (Data.TableLexems[i].type == ltStart)
		{
			ss.Push(i, &ss.S);
		}
		if (Data.TableLexems[i].type == ltFinish)
		{
			if (ss.IsEmpty(&ss.S))
			{
				j = 1;	//  Too much ')';
				break;
			}
			else
			{
				ss.Pop(&ss.S);
			}
		}
	}
	if (!(ss.IsEmpty(&ss.S)))
	{
		j = 2;	// ')' expected;
	}
	return j;
}

int ErrorChecking()
{
	int ifNumb = 0;
	int label = 0;
	int i = 0, j = 1, temp = 0, ValNum = 0;
	FILE* ef;
	int while_num = 0, STARTBLOK_ENDBLOK_num = 0;//, r1, r2;
	int Err_num = 0;

	ef = fopen("errors.txt", "w");
	fputs("List of bugs in the program:\n\n\n", ef);

	//перевірка чи першим словом у програмі є program
	if (Data.TableLexems[0].type != LProgram)
	{
		Err_num++;
		fprintf(ef, "%d - line %d: \t'Startprogram' expected! (program must begin from the keyword 'Startprogram')\n", Err_num, Data.TableLexems[0].line);
	}
	

	//перевірка, чи другим словом в програмі є ім'я програми
	if (Data.TableLexems[1].type != LBody)
	{
		Err_num++;
		fprintf(ef, "%d - line %d: \t'startblock' expected!\n", Err_num, Data.TableLexems[1].line);
	}

	if (Data.TableLexems[Data.LexNum - 2].type != LEnd)
	{
		Err_num++;
		fprintf(ef, "line %d: \t'endblock' expected!\n", Data.TableLexems[1].line);
	}
	if (Data.TableLexems[Data.LexNum - 1].type != LEOF)
	{
		Err_num++;
		fprintf(ef, "line %d: \tEnd of file expected!\n", Data.TableLexems[0].line);
	}
	if (Data.TableLexems[2].type != LVar)
	{
		Err_num++;
		fprintf(ef, "line %d: \t'variable' expected!\n", Data.TableLexems[i].line);
	}

	//перевірка, чи  другим словом в програмі є Int_2
	if (Data.TableLexems[3].type != LVarType)
	{
		Err_num++;
		fprintf(ef, "%d line %d: \t'Int_2' expected!\n", Err_num, Data.TableLexems[3].line);
	}
	if (Data.TableLexems[3].type == LVarType)
	{
		i = 4;
		if (Data.TableLexems[i].type != LIdentifier)  	//перевірка, чи після Int_2 йдуть ідентифікатори
		{
			Err_num++;
			fprintf(ef, "%d - line %d: \tIdentifier expected!\n", Err_num, Data.TableLexems[i].line);
		}
		else
		{
			do
			{
				if ((Data.TableLexems[i].type == LIdentifier) && (Data.TableLexems[i + 1].type == LComma))
				{
					strcpy(Data.IdTable[Data.IdNum].name, Data.TableLexems[i].name);
					for (int i = 0; i < Data.IdNum; i++)
					{
						if (strcmp(Data.IdTable[i].name, Data.IdTable[Data.IdNum].name) == 0)
						{
							Err_num++;
							fprintf(ef, "%d - line %d: \tThe identifier must be specified only once!\n", Err_num, Data.TableLexems[Data.IdNum].line);
						}
					}

					Data.IdNum++;
					i = i + 2;
				}
			} while ((Data.TableLexems[i].type == LIdentifier) && (Data.TableLexems[i + 1].type == LComma));
			if ((Data.TableLexems[i].type == LIdentifier) && (Data.TableLexems[i + 1].type == LSeparator))
			{
				strcpy(Data.IdTable[Data.IdNum].name, Data.TableLexems[i].name);
				for (int i = 0; i < Data.IdNum; i++)
				{
					if (strcmp(Data.IdTable[i].name, Data.IdTable[Data.IdNum].name) == 0)
					{
						Err_num++;
						fprintf(ef, "%d - line %d: \tThe identifier must be specified only once!\n", Err_num, Data.TableLexems[Data.IdNum].line);
					}
				}
				Data.IdNum++;
				i = i + 2;
				goto label1;
			}
			if (Data.TableLexems[i].type != LSeparator)
			{
				if (Data.TableLexems[i].type == LComma)
				{
					Err_num++;
					fprintf(ef, "line %d: \tToo much commas!\n", Data.TableLexems[i].line);
				}
				else
				{
					Err_num++;
					fprintf(ef, "line %d: \t';' expected!\n", Data.TableLexems[i].line);
				}
			}
			else
			{
				Err_num++;
				fprintf(ef, "line %d: \tToo much commas or identifier expected!\n", Data.TableLexems[i].line);
			}
		}
		i++;
	label1:;
	}

	switch (Balans(i, LEOF, LIf, LBodyIf))
	{
	case 1:
		Err_num++;
		fprintf(ef, "line %d: \tToo much 'If {'!\n", Data.TableLexems[j].line);
		break;
		/*case 2:
			Err_num++;
			fprintf(ef, "line %d: \t'If {' expected!\n", Data.TableLexems[j].line);
			break;*/
	default:
		break;
	}
	switch (Balans(i, LEOF, LIf, LEndIf))
	{
	case 1:
		Err_num++;
		fprintf(ef, "line %d: \tToo much 'If }'!\n", Data.TableLexems[j].line);
		break;
	case 2:
		Err_num++;
		fprintf(ef, "line %d: \t'If }' expected!\n", Data.TableLexems[j].line);
		break;
	default:
		break;
	}

	for (j = 0; j < Data.LexNum; j++)
	{
		int s = 0, e = 0;
		if (Data.TableLexems[j].type == LBody)
		{
			s++;
		}
		else if (Data.TableLexems[j].type == LEnd)
		{
			e++;
		}
		if (s > 1)
		{
			Err_num++;
			fprintf(ef, "line %d: \tToo much 'startblock'!\n", Data.TableLexems[j].line);
		}
		if (e > 1)
		{
			Err_num++;
			fprintf(ef, "line %d: \tToo much 'endblock'!\n", Data.TableLexems[j].line);
		}
	}

	for (j = 0;; j++)
	{
		if (Data.TableLexems[j].type == LUnknown)        //Пошук невідомих слів(не ідентифікаторів)
		{
			Err_num++;
			fprintf(ef, "line %d: \tUnknown identifier!\t-->\t%s\n", Data.TableLexems[j].line, Data.TableLexems[j].name);
		}
		if ((Data.TableLexems[j].type == LIdentifier) && (j > i))
		{
			bool available = 0;
			for (int i = 0; i < Data.IdNum; i++)
			{
				if (strcmp(Data.IdTable[i].name, Data.TableLexems[j].name) == 0)
				{
					available = 1;
				}
			}
			if (!available)
			{
				Err_num++;
				fprintf(ef, "line %d: \tUnknown identifier!\t-->\t%s\n", Data.TableLexems[j].line, Data.TableLexems[j].name);
			}
		}
		if (Data.TableLexems[j].type == LNewValue)
		{
			int buf;
			if (Data.TableLexems[j - 1].type == LIdentifier)
			{
				buf = IsExpression((j + 1), ef);
			}
			else
			{
				buf = 1;
			}
			Err_num = Err_num + buf;
		}
		if (Data.TableLexems[j].type == LOutput)
		{
			int buf, brak;
			if (Data.TableLexems[j + 1].type != LLBraket)
			{
				Err_num++;
				fprintf(ef, "line %d: \t'(' expected!\n", Data.TableLexems[j + 1].line);
			}
			buf = IsExpression((j + 1), ef);
			Err_num = Err_num + buf;
			brak = Balans(j, LSeparator, LLBraket, LRBraket);
			if (brak == 1)
			{
				Err_num++;
				fprintf(ef, "line %d: \tToo much ')'!\n", Data.TableLexems[j].line);
			}
			if (brak == 2)
			{
				Err_num++;
				fprintf(ef, "line %d: \t')' expected!\n", Data.TableLexems[j].line);
			}
		}
		if (Data.TableLexems[j].type == LInput)
		{
			if (Data.TableLexems[j + 1].type != LLBraket)
			{
				Err_num++;
				fprintf(ef, "line %d: \t'(' expected!\n", Data.TableLexems[j + 1].line);
			}
			if (Data.TableLexems[j + 2].type != LIdentifier)
			{
				Err_num++;
				fprintf(ef, "line %d: \tIdentifier expected!\n", Data.TableLexems[j + 2].line);
			}
			if (Data.TableLexems[j + 3].type != LRBraket)
			{
				Err_num++;
				fprintf(ef, "line %d: \t')' expected!\n", Data.TableLexems[j + 3].line);
			}
			if (Data.TableLexems[j + 4].type != LSeparator)
			{
				Err_num++;
				fprintf(ef, "line %d: \t';' expected!\n", Data.TableLexems[j + 4].line);
			}

		}
		if (Data.TableLexems[j].type == LFor)				//перевірка for
		{
			if (Data.TableLexems[j + 1].type != LLBraket)
			{
				Err_num++;
				fprintf(ef, "line %d: \t'(' expected after 'For'!\n", Data.TableLexems[j + 1].line);
			}
			else if (Data.TableLexems[j + 2].type != LIdentifier)
			{
				Err_num++;
				fprintf(ef, "line %d: \tIdentifier expected after 'For'!\n", Data.TableLexems[j + 1].line);
			}
			else if (Data.TableLexems[j + 3].type != LNewValue)
			{
				Err_num++;
				fprintf(ef, "line %d: \t'->' expected after identifier!\n", Data.TableLexems[j + 2].line);
			}
			else if (IsExpression(j + 4, ef) != 0)
			{
				Err_num++;
				fprintf(ef, "line %d: \tIdentifier or number expected after '->'!\n", Data.TableLexems[j + 3].line);
			}
			while ((Data.TableLexems[j].type != LSeparator) && (Data.TableLexems[j].type != LEOF)) j++;

			if (Data.TableLexems[j + 1].type != LIdentifier)
			{
				Err_num++;
				fprintf(ef, "line %d: \tIdentifier expected after 'For'!\n", Data.TableLexems[j + 1].line);
			}
			else if ((Data.TableLexems[j + 2].type != LLe) && (Data.TableLexems[j + 2].type != LGe))
			{
				Err_num++;
				fprintf(ef, "line %d: \t'Le' or 'Ge' expected after identifier!\n", Data.TableLexems[j + 2].line);
			}
			else if (IsExpression(j + 3, ef) != 0)
			{
				Err_num++;
				fprintf(ef, "line %d: \tIdentifier or number expected after '->'!\n", Data.TableLexems[j + 5].line);
			}
			j++;
			while ((Data.TableLexems[j].type != LSeparator) && (Data.TableLexems[j].type != LEOF)) j++;
			if (Data.TableLexems[j + 1].type != LIdentifier)
			{
				Err_num++;
				fprintf(ef, "line %d: \t Identifier expected!\n", Data.TableLexems[j + 3].line);
			}
			else if (Data.TableLexems[j + 2].type != LNewValue)
			{
				Err_num++;
				fprintf(ef, "line %d: \t '->' expected after identifier!\n", Data.TableLexems[j + 3].line);
			}
			else if (IsExpression(j + 3, ef) != 0)
			{
				Err_num++;
				fprintf(ef, "line %d: \t number or identifier expected!\n", Data.TableLexems[j + 3].line);
			}
			j++;
			while ((Data.TableLexems[j].type != LSeparator) && (Data.TableLexems[j].type != LEOF)) j++;
			if (Data.TableLexems[j + 2].type != LBody)
			{
				Err_num++;
				fprintf(ef, "line %d: \t'Start' expected!\n", Data.TableLexems[j + 6].line);
			}
		}
		if (Data.TableLexems[j].type == LEOF) break;
	}
	if (Err_num == 0) fprintf(ef, "No errors found.");
	return Err_num;
}
