#include "A.h"

//Повертає кількість лексем
DataType Data;
bool name = 0;
int AnalisisLexems(FILE* FIn)
{
	Lexema* TempLexema;
	int i = 0;		//лічильник лексем
	char type[50];
	strcpy(type, "");

	do
	{
		TempLexema = GetNextLexem(FIn, i);
		strncpy(Data.TableLexems[i].name, TempLexema->name, 49);
		Data.TableLexems[i].value = TempLexema->value;
		Data.TableLexems[i].type = TempLexema->type;
		Data.TableLexems[i].line = TempLexema->line;
		i++;
	} while (TempLexema->type != LEOF);
	return i;
}

Lexema* GetNextLexem(FILE* f, int ii)
{
	char ch, buf[50];
	bool IsComment = false;
	Lexema* res;
	static int line = 1;
	res = (Lexema*)calloc(1, sizeof(Lexema));
	for (;;)
	{
		ch = getc(f);
		if (ch == '#') //шукаємо перший символ відкриття коментаря
		{
			char c;
			c = getc(f);
			if (c == '*') //шукаємо другий символ відкриття коментаря 
			{
				strncpy(res->name, "Comentar", 9);
				res->type = LComentar;
				res->value = 0;
				res->line = line;
				for (;;) {
					c = getc(f);
					if (c == '*') //шукаємо перший символ закриття коментаря 
					{
						c = getc(f);
						if (c == '\n')
						{
							line++;
						}
						if (c == EOF)
						{
							strncpy(res->name, "EOF", 4);
							res->type = LEOF;
							res->value = 0;
							res->line = line;
						}
						if (c == '#') //шукаємо другий символ закриття коментаря 
						{
							break;
						}
					}
					if (c == '\n')
					{
						line++;
					}
					if (c == EOF)
					{
						strncpy(res->name, "EOF", 4);
						res->type = LEOF;
						res->value = 0;
						res->line = line;
					}
				}
			}
			else
			{
				ungetc(c, f);
			}
		}
		else if (ch == '\n') line++;
		else if (ch == EOF)
		{
			strncpy(res->name, "EOF", 4); //кінець файлу
			res->type = LEOF;
			res->value = 0;
			res->line = line;
			break;
		}
		else if (ch == '(') //відриваюча дужка
		{
			strncpy(res->name, "(", 2);
			res->type = LLBraket;
			res->value = 0;
			res->line = line;
			break;
		}
		else if (ch == ')') //закриваюча дужка
		{
			strncpy(res->name, ")", 2);
			res->type = LRBraket;
			res->value = 0;
			res->line = line;
			break;
		}
		else if (ch == ';') //двокрапка
		{
			strncpy(res->name, ";", 2);
			res->type = LSeparator;
			res->value = 0;
			res->line = line;
			break;
		}
		else if (ch == ',') //кома
		{
			strncpy(res->name, ",", 2);
			res->type = LComma;
			res->value = 0;
			res->line = line;
			break;
		}

		
		else if (ch == '+')
		{
			strncpy(res->name, "+", 2); //додавання
			res->type = LAdd; //тут +
			res->value = 0;
			res->line = line;
			break;
		}
		else if (ch == '-')
		{
			char c = getc(f);
			if (isdigit(c)) //відємне число
			{
				int i = 2;
				buf[0] = ch;
				buf[1] = c;
				for (; isdigit(ch = getc(f)); ++i)
				{
					buf[i] = ch;
				}
				ungetc(ch, f);
				buf[i] = '\0';
				strncpy(res->name, buf, i);
				res->type = LNumber;
				res->value = atoi(buf);
				res->line = line;
				break;
			}
			else
			{
				strncpy(res->name, "-", 2); //віднімання
				res->type = LSub;
				res->value = 0;
				res->line = line;
				break;
			}
		}
		else if (ch == '!')
		{
			char c = getc(f);
			if (c == '!')
			{
				strncpy(res->name, "!!", 3); //логіяне НЕ
				res->type = LNot;
				res->value = 0;
				res->line = line;
				ungetc(c, f);
				break;
			}
			else if (c == '=')
			{
				strncpy(res->name, "!=", 3); //логіяне НЕ
				res->type = LNe;
				res->value = 0;
				res->line = line;
				ungetc(c, f);
				break;
			}
			else
			{
				strncpy(res->name, "!", 2);
				res->type = LUnknown;
				res->value = 0;
				res->line = line;
				break;
			}
		}
		else if (ch == '&')
		{
			char c = getc(f);
			if (c == '&')
			{
				strncpy(res->name, "&&", 3); //логічне І
				res->type = LAnd;
				res->value = 0;
				res->line = line;
				break;
			}
			else
			{
				strncpy(res->name, "&", 2);
				res->type = LUnknown;
				res->value = 0;
				res->line = line;
				break;
			}
		}
		else if (ch == '|')
		{
			char c = getc(f);
			if (c == '|')
			{
				strncpy(res->name, "||", 3); //логічне АБО
				res->type = LOr;
				res->value = 0;
				res->line = line;
				break;
			}
			else
			{
				strncpy(res->name, "|", 2);
				res->type = LUnknown;
				res->value = 0;
				res->line = line;
				break;
			}
		}
		else if (ch == '=')
		{
			char c = getc(f);
			if (c == '=')
			{
				strncpy(res->name, "==", 3);
				res->type = LEg;
				res->value = 0;
				res->line = line;
				break;
			}
			else
			{
				strncpy(res->name, "=", 2);
				res->type = LUnknown;
				res->value = 0;
				res->line = line;
				break;
			}
		}
		else if (ch == '>')
		{
			char c = getc(f);
			if (c == '>')
			{
				strncpy(res->name, ">>", 3);
				res->type = LNewValue;
				res->value = 0;
				res->line = line;
				break;
			}
			else if (c == '=')
			{
				strncpy(res->name, ">=", 3);
				res->type = LGe;
				res->value = 0;
				res->line = line;
				break;
			}
			else
			{
				strncpy(res->name, ">", 2);
				res->type = LUnknown;
				res->value = 0;
				res->line = line;
				break;
			}
		}
		else if (ch == '<')
		{
			char c = getc(f);
			if (c == '=')
			{
				strncpy(res->name, "<=", 3); 
				res->type = LLe;
				res->value = 0;
				res->line = line;
				break;
			}
			else
			{
				strncpy(res->name, "<", 2);
				res->type = LUnknown;
				res->value = 0;
				res->line = line;
				break;
			}
		}

		else if (isalpha(ch) || (ch == '_')) //Зарезервоване слово або ідентифікатор
		{
			int i = 0;
			buf[0] = ch;
			for (i = 1;; ++i)
			{
				ch = getc(f);
				if ((isdigit(ch) != 0) || (isalpha(ch) != 0) || (ch == '_')) buf[i] = ch;
				else break;
			}
			int j;
			for (j = 0; j < 1; j++)
			{
				if ((buf[j] == '_')) { break; } //назва змінної починається з _
			}
			for (j = 1; j < i; j++)
			{
				if (!islower(buf[j])) { break; } //другий символ (буква) у верхньому регістрів
			}
			ungetc(ch, f);
			buf[i] = '\0';
			strncpy(res->name, buf, i);

			//Співставлення ідентифікаторів та зарезервованих слів
			if (strcmp(buf, "Startprogram") == 0) //ім'я програми
			{
				res->type = LProgram;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "startblock")) == 0) //початок прогарми
			{
				res->type = LBody;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "variable")) == 0) //ключове слово
			{
				res->type = LVar;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "Int_2")) == 0) //тип даних
			{
				res->type = LVarType;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "endblock")) == 0) //кінець чогось(циклу чи програми)
			{
				res->type = LEnd;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "Scan")) == 0) //ввів
			{
				res->type = LInput;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "Print")) == 0) //вивід
			{
				res->type = LOutput;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "For")) == 0) //входження в цикл Do-While
			{
				res->type = LFor;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "Mul")) == 0) //ділення
			{
				res->type = LMul;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "Div")) == 0) //ділення
			{
				res->type = LDiv;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "If")) == 0)
			{
				res->type = LIf;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "Else")) == 0)
			{
				res->type = LElse;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "StartIf")) == 0)
			{
				res->type = LBodyIf;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "EndIf")) == 0)
			{
				res->type = LEndIf;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "Mod")) == 0)
			{
				res->type = LMod;
				res->line = line;
				break;
			}
			else if ((j == i) && ((i <= 5) || (ii == 1)) && (buf[0] == '_')) //назва змінних (ідентифікаторів)
			{
				strncpy(res->name, buf, i);
				res->type = LIdentifier;
				res->value = 1;
				res->line = line;
				break;
			}
			else
			{
				strncpy(res->name, buf, i);
				res->type = LUnknown;
				res->value = 0;
				res->line = line;
				break;
			}
			res->value = 0;
			res->line = line;
		}
		else if (isdigit(ch)) //додатнє число
		{
			int i = 1;
			buf[0] = ch;
			for (; isdigit(ch = getc(f)); ++i)
			{
				buf[i] = ch;
			}
			ungetc(ch, f);
			buf[i] = '\0';
			strncpy(res->name, buf, i);
			res->type = LNumber;
			res->value = atoi(buf);
			res->line = line;
			break;
		}
		else if ((ch != '\n') && (ch != '\t') && (ch != ' '))
		{
			char bufer[50];
			char c;
			int i;
			bufer[0] = ch;
			for (i = 1;; i++)
			{
				c = fgetc(f);
				if ((c == '\n') || (c == '\t') || (c == ' ') || (c == ';'))
				{
					if (c == '\n')
					{
						line++;
						break;
					}
					else break;
				}
				bufer[i] = c;
			}
			ungetc(c, f);
			bufer[i] = '\0';
			strncpy(res->name, bufer, i);
			res->type = LUnknown;
			res->value = 0;
			res->line = line;
			break;
		}
	}
	return res;
}

void PrintLexemsInFile()
{
	FILE* outTokensFiles;
	char type[MAX_LENGTH_TYPES];
	int i;
	outTokensFiles = fopen("lexems.txt", "w"); //відкриваємо файл з лексемами для запису
	//записумо в файл лексеми і їх аналіз
	fputs("Table of tokens!\n\n", outTokensFiles);
	fprintf(outTokensFiles, "--------------------------------------------------------------------\n");
	fprintf(outTokensFiles, "|%5s \t%s \t%12s %20s %10s \n", "Line", "№", "Name", "Type", "Value|");
	fprintf(outTokensFiles, "--------------------------------------------------------------------\n");
	for (i = 0; i < Data.LexNum; ++i)
	{
		switch (Data.TableLexems[i].type)
		{
		case LProgram: strncpy(type, "Program", 8); break;
		case LProgramName: strncpy(type, "ProgramName", 12); break;
		case LBody: strncpy(type, "Body", 5); break;
		case LVar: strncpy(type, "Var", 5); break;
		case LVarType: strncpy(type, "Int_2", 6); break;
		case LIdentifier: strncpy(type, "Identifier", 11); break;
		case LEnd: strncpy(type, "End", 4); break;
		case LInput: strncpy(type, "Input", 4); break;
		case LOutput: strncpy(type, "Ouput", 4); break;
		case LLabel: strncpy(type, "Label", 6); break;
		case LFor: strncpy(type, "For", 3); break;
		case LIf: strncpy(type, "If", 3); break;
		case LBodyIf: strncpy(type, "BodyIf", 7); break;
		case LElse: strncpy(type, "Else", 5); break;
			//case LBodyElse: strncpy(type, "BodyElse", 7); break;
			//case LEndElse: strncpy(type, "EndElse", 6); break;
		case LEndIf: strncpy(type, "EndIf", 6); break;
		case LNewValue: strncpy(type, "NewValue", 9); break;
		case LAdd: strncpy(type, "Add", 4); break;
		case LSub: strncpy(type, "Sub", 4); break;
		case LMul: strncpy(type, "Mul", 4); break;
		case LDiv: strncpy(type, "Div", 4); break;
		case LMod: strncpy(type, "Mod", 4); break;
		case LEg: strncpy(type, "Equ", 4); break;
		case LNe: strncpy(type, "NotEqu", 7); break;
		case LLe: strncpy(type, "Less", 5); break;
		case LGe: strncpy(type, "Greate", 7); break;
		case LNot: strncpy(type, "Not", 4); break;
		case LAnd: strncpy(type, "And", 4); break;
		case LOr: strncpy(type, "Or", 3); break;
		case LLBraket: strncpy(type, "LeftBraket", 11); break;
		case LRBraket: strncpy(type, "RightBraket", 12); break;
		case LNumber: strncpy(type, "Number", 7); break;
		case LSeparator: strncpy(type, "Separator", 10); break;
		case LComma: strncpy(type, "Comma", 6); break;
		case LEOF: strncpy(type, "EndOfFile", 10); break;
		case LComentar: strncpy(type, "Comentar", 9); break;
		case LUnknown: strncpy(type, "Unknown", 8); break;
		}
		fprintf(outTokensFiles, "%5d \t%d \t%12s %20s %10d \n", Data.TableLexems[i].line, i + 1, Data.TableLexems[i].name, type, Data.TableLexems[i].value);
	}
	fclose(outTokensFiles);
}