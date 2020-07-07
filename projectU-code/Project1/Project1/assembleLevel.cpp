#include "Uwonjeong.h"

// 명령어 메모리의 Data부분에 대입 및 DS 계산
int WonJeongCode(int *WonJeong, instructionStruct *instructionLink, labelPosition *labelValue, int instructionLine){
	int  DataCount  =     0;
	int  tempLine   =     0;
	bool labelcheck = false;

	for (int i = 0; i < instructionLine; i++) {
		WonJeong[DataCount++] = (instructionLink + i)->instruction;

		// 라벨 position 다시 지정
		if (strcmp((labelValue + tempLine)->label, (instructionLink + i)->label) == 0) {
			(labelValue + tempLine)->beforePosition = (labelValue + tempLine)->position;
			(labelValue + tempLine)->position = DataCount - 1;
			tempLine++;
		}

		// val1 라벨 position으로 수정
		for (int k = 0; k < instructionLine; k++) {
			for (int j = 0; j < sizeof(sizeof(labelValue) / sizeof(labelPosition)); j++) {
				if (strcmp((instructionLink + k)->val1, (labelValue + j)->label) == 0) {
					_itoa_s((labelValue + j)->position, (instructionLink + k)->val1, 6, 10);
				}
			}
		}

		if (strlen((instructionLink + i)->val1) != 0) {
			if (strcmp((instructionLink + i)->val1, "read") == 0) WonJeong[DataCount++] = -3;
			else if (strcmp((instructionLink + i)->val1, "write") == 0) WonJeong[DataCount++] = -2;
			else if (strcmp((instructionLink + i)->val1, "lf") == 0) WonJeong[DataCount++] = -1;
			else WonJeong[DataCount++] = atoi((instructionLink + i)->val1);
			if (strlen((instructionLink + i)->val2) != 0) {
				WonJeong[DataCount++] = atoi((instructionLink + i)->val2);
				if (strlen((instructionLink + i)->val3) != 0) {
					WonJeong[DataCount++] = atoi((instructionLink + i)->val3);
				}
			}
		}
	}

	// 라벨 애프터로 바꿔주기
	for (int i = 0; i < DataCount; i++) {
		if (WonJeong[i] == 8 || WonJeong[i] == 34 || WonJeong[i] == 35 || WonJeong[i] == 36) {
			i++;
			for (int j = 0; j < sizeof(sizeof(labelValue) / sizeof(labelPosition)); j++) {
				if (WonJeong[i] == (labelValue + j)->beforePosition) {
					WonJeong[i] = (labelValue + j)->position;
					break;
				}
			}
		}
	}

	return DataCount;
}

int setOpcode(char *instructionName) {
	// Opcode 설정
	if      (!strcmp(instructionName,   "nop")) return  0;
	else if (!strcmp(instructionName,   "bgn")) return  1;
	else if (!strcmp(instructionName,   "sym")) return  2;
	else if (!strcmp(instructionName,   "end")) return  3;
	else if (!strcmp(instructionName,  "proc")) return  4;
	else if (!strcmp(instructionName,   "ret")) return  5;
	else if (!strcmp(instructionName,   "ldp")) return  6;
	else if (!strcmp(instructionName,  "push")) return  7;
	else if (!strcmp(instructionName,  "call")) return  8;
	else if (!strcmp(instructionName,   "lod")) return  9;
	else if (!strcmp(instructionName,   "lda")) return 10;
	else if (!strcmp(instructionName,   "ldc")) return 11;
	else if (!strcmp(instructionName,   "str")) return 12;
	else if (!strcmp(instructionName,   "ldi")) return 13;
	else if (!strcmp(instructionName,   "sti")) return 14;
	else if (!strcmp(instructionName,   "not")) return 15;
	else if (!strcmp(instructionName,   "neg")) return 16;
	else if (!strcmp(instructionName,   "inc")) return 17;
	else if (!strcmp(instructionName,   "dec")) return 18;
	else if (!strcmp(instructionName,   "dup")) return 19;
	else if (!strcmp(instructionName,   "add")) return 20;
	else if (!strcmp(instructionName,   "sub")) return 21;
	else if (!strcmp(instructionName,  "mult")) return 22;
	else if (!strcmp(instructionName,   "div")) return 23;
	else if (!strcmp(instructionName,   "mod")) return 24;
	else if (!strcmp(instructionName,    "gt")) return 25;
	else if (!strcmp(instructionName,    "lt")) return 26;
	else if (!strcmp(instructionName,    "ge")) return 27;
	else if (!strcmp(instructionName,    "le")) return 28;
	else if (!strcmp(instructionName,    "eq")) return 29;
	else if (!strcmp(instructionName,    "ne")) return 30;
	else if (!strcmp(instructionName,   "and")) return 31;
	else if (!strcmp(instructionName,    "or")) return 32;
	else if (!strcmp(instructionName,   "swp")) return 33;
	else if (!strcmp(instructionName,   "ujp")) return 34;
	else if (!strcmp(instructionName,   "tjp")) return 35;
	else if (!strcmp(instructionName,   "fjp")) return 36;
}

const char* getOpcode(int opCode) {
	// Opcode 설정
	if (opCode == 0)  return   "nop";
	if (opCode == 1)  return   "bgn";
	if (opCode == 2)  return   "sym";
	if (opCode == 3)  return   "end";
	if (opCode == 4)  return  "proc";
	if (opCode == 5)  return   "ret";
	if (opCode == 6)  return   "ldp";
	if (opCode == 7)  return  "push";
	if (opCode == 8)  return  "call";
	if (opCode == 9)  return   "lod";
	if (opCode == 10) return   "lda";
	if (opCode == 11) return   "ldc";
	if (opCode == 12) return   "str";
	if (opCode == 13) return   "ldi";
	if (opCode == 14) return   "sti";
	if (opCode == 15) return   "not";
	if (opCode == 16) return   "neg";
	if (opCode == 17) return   "inc";
	if (opCode == 18) return   "dec";
	if (opCode == 19) return   "dup";
	if (opCode == 20) return   "add";
	if (opCode == 21) return   "sub";
	if (opCode == 22) return  "mult";
	if (opCode == 23) return   "div";
	if (opCode == 24) return   "mod";
	if (opCode == 25) return    "gt";
	if (opCode == 26) return    "lt";
	if (opCode == 27) return    "ge";
	if (opCode == 28) return    "le";
	if (opCode == 29) return    "eq";
	if (opCode == 30) return    "ne";
	if (opCode == 31) return   "and";
	if (opCode == 32) return    "or";
	if (opCode == 33) return   "swp";
	if (opCode == 34) return   "ujp";
	if (opCode == 35) return   "tjp";
	if (opCode == 36) return   "fjp";
}

instructionStruct *returnInstruction(string instructionData) {
	// 데이터 가공
	instructionStruct *nowLine = new instructionStruct[1];

	const char* cInstruction =                               instructionData.c_str();
	int cInstrucitonlength   = instructionData.length(), instructionLengthFecth = 11;
	bool endCheck            =												   false;

	// 받아오기
	char labelName[10] = { 0 }, instructionName[100] = { 0 }, instructionValue1[100] = { 0 }, instructionValue2[100] = { 0 }, instructionValue3[100] = { 0 };

	// 라벨
	for (int i = 0; i < 10; i++) {
		if (*(cInstruction + i) == ' ') break;
		if (*(cInstruction + i) == '%') {
			endCheck = true;
			break; 
		}
		labelName[i] = *(cInstruction + i);
	}

	// 명령어
	for (int i = 0; i < 100, endCheck == false, instructionLengthFecth < cInstrucitonlength; i++, instructionLengthFecth++) {
		if (*(cInstruction + instructionLengthFecth) == ' ') break;
		if (*(cInstruction + instructionLengthFecth) == '%') {
			endCheck = true;
			break;
		}
		instructionName[i] = *(cInstruction + instructionLengthFecth);
	}
	instructionLengthFecth++;

	// 값1
	for (int i = 0; i < 100, endCheck == false, instructionLengthFecth < cInstrucitonlength; i++, instructionLengthFecth++) {
		if (*(cInstruction + instructionLengthFecth) == ' ') break;
		if (*(cInstruction + instructionLengthFecth) == '%') {
			endCheck = true;
			break;
		}
		instructionValue1[i] = *(cInstruction + instructionLengthFecth);
	}
	instructionLengthFecth++;

	// 값2
	for (int i = 0; i < 100, endCheck == false, instructionLengthFecth < cInstrucitonlength; i++, instructionLengthFecth++) {
		if (*(cInstruction + instructionLengthFecth) == ' ') break;
		if (*(cInstruction + instructionLengthFecth) == '%') {
			endCheck = true;
			break;
		}
		instructionValue2[i] = *(cInstruction + instructionLengthFecth);
	}
	instructionLengthFecth++;

	// 값3
	for (int i = 0; i < 100, endCheck == false, instructionLengthFecth < cInstrucitonlength; i++, instructionLengthFecth++) {
		if (*(cInstruction + instructionLengthFecth) == ' ') break;
		if (*(cInstruction + instructionLengthFecth) == '%') {
			endCheck = true;
			break;
		}
		instructionValue3[i] = *(cInstruction + instructionLengthFecth);
	}
	instructionLengthFecth++;

	// nowLine에 대입
	if (labelName != NULL)                 for (int i = 0; i < sizeof(labelName); i++)    nowLine->label[i] =               labelName[i];

	// 옵코드로 저장
	if (instructionName != NULL)                                                       nowLine->instruction = setOpcode(instructionName);

	if (instructionValue1 != NULL) for (int i = 0; i < sizeof(instructionValue1); i++)     nowLine->val1[i] =       instructionValue1[i];
	if (instructionValue2 != NULL) for (int i = 0; i < sizeof(instructionValue2); i++)     nowLine->val2[i] =       instructionValue2[i];
	if (instructionValue3 != NULL) for (int i = 0; i < sizeof(instructionValue3); i++)     nowLine->val3[i] =       instructionValue3[i];

	return nowLine;
}