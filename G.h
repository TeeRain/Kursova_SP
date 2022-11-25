#pragma once
#include "D.cpp"
#include "E.h"
void BeginASMFile(FILE* );
void BeginCodeSegm(FILE* );

void CheckPresent();

void PrintData(FILE* );

void PrintAND(FILE* );
void PrintOR(FILE* );
void PrintNOT(FILE* );
void PrintEQ(FILE* );
void PrintGE(FILE* );
void PrintLE(FILE* );
void PrintCheck(FILE* );

void PrintInput(FILE* );
void PrintOutput(FILE* );


void PrintEnding(FILE* );

bool Prioritet(TypeOfLex , StackType );

int ConvertToPostfixForm(int );

void GenASMCode(const char* , FILE* );

void PrintCode(FILE* );

void GenerateCode(FILE* );
