#pragma once

#include <stdio.h>
#include <windows.h>
#include <commctrl.h>
#include <fstream>
#include <iostream>
#include <string>
#include <tchar.h>
#include <stack>

using namespace std;

# define MAX_STACK 1000000

struct instructionStruct {
	char  label[10];
	int instruction;
	char  val1[100];
	char  val2[100];
	char  val3[100];
};

struct labelPosition {
	char       label[10];
	int         position;
	int   beforePosition;
};

struct cpuStruct {
	int PC;
	int DS;
	int SS;
	int SP;
	int BP;
	bool end;
};

instructionStruct *returnInstruction(string);
int setOpcode(char *);
const char* getOpcode(int);
int WonJeongCode(int *, instructionStruct *, labelPosition *, int);
int excutionFuntion(int *, stack<int> *, instructionStruct *, labelPosition *, cpuStruct *, int, int, HWND *, HWND *, string *);
void systemFuntion(int *, labelPosition *, cpuStruct *, int);
void fileSave(string, int, HWND *, labelPosition *);