#include "Uwonjeong.h"
// 파일 저장
void fileSave(string result,int resultLineCount, HWND *edWrite, labelPosition *labelValue) {
	HMODULE hModule = GetModuleHandleW(NULL);
	WCHAR path[MAX_PATH];
	char stet[100000];
	GetModuleFileNameW(hModule, path, MAX_PATH);

	wstring pathString = wstring(path);

	ofstream output(pathString.substr(0, pathString.find_last_of(L"\\/")) + L"\\output.lst");
	output << result;
	output << "\r\n\r\n\r\n\r\n";
	output << "------------------------------\n";
	GetWindowText(*edWrite, stet, 100000);
	output << string("result = ") + stet;
	output << "\n";
	output << "------------------------------\n";
	output << "line = " + to_string(resultLineCount);
	output << "\n";
	output << "------------------------------\n";
	for (int j = 0, cnt = 0; j < sizeof(sizeof(labelValue) / sizeof(labelPosition)); j++) {
		if ((labelValue + j)->beforePosition <= 0) continue;
		cnt++;
		output << "label " + to_string(cnt) + "번째 라벨 위치 : " + to_string((labelValue + j)->position);
		output << "\n";
	}
	output << "------------------------------\n";
	output.close();
}


// 시스템 함수 처리
void systemFuntion(int *WonJeong, labelPosition *labelValue, cpuStruct *pointerMemory, int labelCount) {
	// read
	strcpy_s((labelValue + labelCount - 2)->label, 10, "read");
	(labelValue + labelCount - 2)->beforePosition = -3;
	(labelValue + labelCount - 2)->position = -3;

	// write
	strcpy_s((labelValue + labelCount - 1)->label, 10, "write");
	(labelValue + labelCount - 1)->beforePosition = -2;
	(labelValue + labelCount - 1)->position = -2;

	// lf
	strcpy_s((labelValue + labelCount)->label, 10, "lf");
	(labelValue + labelCount)->beforePosition = -1;
	(labelValue + labelCount)->position = -1;

	pointerMemory->DS += 3;
	pointerMemory->SS += 3;
}

int excutionFuntion(int *WonJeong, stack<int> *cpuWonJeong, instructionStruct *instructionLink, labelPosition *labelValue, cpuStruct *pointerMemory, int beforePC, int number, HWND *edWrite, HWND *edResult, string *tempResult) {
	// 현재 PC에 대한 명령어 Line찾기
	int i, a, b;
	static int stackBlock, ldpCount, pushCount;
	static string tempMessage;
	for (i = 0; i; i++) if (instructionLink[i].instruction == WonJeong[pointerMemory->PC]) break;

	switch (WonJeong[pointerMemory->PC])
	{
	// nop - 완료 11
	case 0:
		*tempResult += to_string(WonJeong[pointerMemory->PC]);
		*tempResult += "\r\n";
		SetWindowText(*edResult, (tempResult)->c_str());
		return pointerMemory->PC + 1;

	// bgn - 완료 11
	case 1:
		stackBlock = 1;
		ldpCount = 0;
		pushCount = 0;
		pointerMemory->SS = pointerMemory->DS + WonJeong[pointerMemory->PC + 1];
		tempMessage = "";
		*tempResult = "";
		SetWindowText(*edWrite, tempMessage.c_str());
		*tempResult += getOpcode(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC + 1]);
		*tempResult += "\r\n";
		SetWindowText(*edResult, (tempResult)->c_str());
		return pointerMemory->PC + 2;

	// sym - 완료 11
	case 2:
		*tempResult += getOpcode(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC + 1]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC + 2]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC + 3]);
		*tempResult += "\r\n";
		SetWindowText(*edResult, (tempResult)->c_str());
		return pointerMemory->PC + 4;

	// end - 완료 11
	case 3:
		pointerMemory->end = true;
		*tempResult += getOpcode(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC]);
		*tempResult += "\r\n";
		SetWindowText(*edResult, (tempResult)->c_str());
		return pointerMemory->PC;

	// proc - 완료 11
	case 4:
		ldpCount = 0;
		stackBlock++;
		pointerMemory->SP = pointerMemory->BP + WonJeong[pointerMemory->PC + 1] + 2;
		for (i = 2 + pushCount + pointerMemory->BP; i < pointerMemory->BP + WonJeong[pointerMemory->PC + 1] + 2; i++) {
			WonJeong[(MAX_STACK - 1) - i] = -4;
		}
		pushCount = 0;
		*tempResult += getOpcode(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC + 1]);
		*tempResult += "\r\n";
		SetWindowText(*edResult, (tempResult)->c_str());
		return pointerMemory->PC + 2;

	// ret - 완료
	case 5:
		//
		for (i = pointerMemory->BP + 2; i < pointerMemory->SP; i++) WonJeong[(MAX_STACK - 1) - i] = -4;
		pointerMemory->SP = pointerMemory->BP;
		i = WonJeong[(MAX_STACK - 1) - (pointerMemory->BP + 1)];
		pointerMemory->BP = WonJeong[(MAX_STACK - 1) - pointerMemory->BP];
		*tempResult += getOpcode(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC]);
		*tempResult += "\r\n";
		SetWindowText(*edResult, (tempResult)->c_str());
		return i;

	// ldp - 완료
	case 6:
		ldpCount++;
		pointerMemory->SP = pointerMemory->SP + 2;
		for (i = 1; i < 3; i++) if(WonJeong[(MAX_STACK - 1) - (pointerMemory->SP - i)] == NULL) WonJeong[(MAX_STACK - 1) - (pointerMemory->SP - i)] = -4;
		*tempResult += getOpcode(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC]);
		*tempResult += "\r\n";
		SetWindowText(*edResult, (tempResult)->c_str());
		return pointerMemory->PC + 1;

	// push  - 완료 11
	case 7:
		a = pointerMemory->BP;
		b = 0;

		if (ldpCount != 0) {
			WonJeong[(MAX_STACK - 1) - (pointerMemory->SP)++] = cpuWonJeong->top();
			pushCount++;
		}

		else {
			while (WonJeong[(MAX_STACK - 1) - (a + b)] != -4) b++;
			WonJeong[(MAX_STACK - 1) - (a + b)] = cpuWonJeong->top();
		}

		cpuWonJeong->pop();

		*tempResult += getOpcode(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC]);
		*tempResult += "\r\n";
		SetWindowText(*edResult, (tempResult)->c_str());
		return pointerMemory->PC + 1;
		
	// call - 완료 1
	case 8:
		ldpCount--;
		if (WonJeong[pointerMemory->PC + 1] < 0) {
			pushCount = 0;
			switch (WonJeong[pointerMemory->PC + 1]) {
			// lf
			case -1:
				tempMessage += "\r\n";
				SetWindowText(*edWrite, tempMessage.c_str());
				break;

			// write
			case -2:
				tempMessage.append(to_string(WonJeong[(MAX_STACK - 1) - (pointerMemory->SP - 1)]));
				tempMessage.append(" ");
				SetWindowText(*edWrite, tempMessage.c_str());
				break;

			// read
			case -3:
				WonJeong[(MAX_STACK - 1) - WonJeong[(MAX_STACK - 1) - (pointerMemory->SP - 1)]] = number;
				break;
			}
			pointerMemory->SP = pointerMemory->SP - 2;
			pointerMemory->SP--;
			*tempResult += getOpcode(WonJeong[pointerMemory->PC]);
			*tempResult += " ";
			*tempResult += to_string(WonJeong[pointerMemory->PC]);
			*tempResult += " ";
			*tempResult += to_string(WonJeong[pointerMemory->PC + 1]);
			*tempResult += "\r\n";
			SetWindowText(*edResult, (tempResult)->c_str());
			return pointerMemory->PC + 2;
		}
		pointerMemory->SP -= pushCount;

		if (stackBlock == 1) WonJeong[(MAX_STACK - 1)] = -1;
		else WonJeong[(MAX_STACK - 1) - (pointerMemory->SP - 2)] = pointerMemory->BP;

		i = 0;
		while ((labelValue + i)->position != WonJeong[pointerMemory->PC + 1]) i++;

		WonJeong[(MAX_STACK - 1) - (pointerMemory->SP - 1)] = pointerMemory->PC + 2;

		pointerMemory->BP = pointerMemory->SP - 2;

		*tempResult += getOpcode(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC + 1]);
		*tempResult += "\r\n";
		SetWindowText(*edResult, (tempResult)->c_str());
		return (labelValue + i)->position;

	// lod - 완료 11
	case 9:
		if (WonJeong[pointerMemory->PC + 1] == 1) cpuWonJeong->push(WonJeong[pointerMemory->DS + WonJeong[pointerMemory->PC + 2]]);
		else cpuWonJeong->push(WonJeong[(MAX_STACK - 1) - (2 + pointerMemory->BP + WonJeong[pointerMemory->PC + 2])]);
		*tempResult += getOpcode(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC + 1]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC + 2]);
		*tempResult += "\r\n";
		SetWindowText(*edResult, (tempResult)->c_str());
		return pointerMemory->PC + 3;

	// lda - 완료 11
	case 10:
		cpuWonJeong->push(2 + pointerMemory->BP + WonJeong[pointerMemory->PC + 2]);
		*tempResult += getOpcode(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC + 1]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC + 2]);
		*tempResult += "\r\n";
		SetWindowText(*edResult, (tempResult)->c_str());
		return pointerMemory->PC + 3;

	// ldc - 완료 11
	case 11:
		i = WonJeong[pointerMemory->PC + 1];
		cpuWonJeong->push(i);
		*tempResult += getOpcode(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC + 1]);
		*tempResult += "\r\n";
		SetWindowText(*edResult, (tempResult)->c_str());
		return pointerMemory->PC + 2;

	// str - 완료 11
	case 12:
		i = cpuWonJeong->top();
		if (WonJeong[pointerMemory->PC + 1] == 1) WonJeong[pointerMemory->DS + WonJeong[pointerMemory->PC + 2]] = i;
		else WonJeong[(MAX_STACK - 1) - (2 + pointerMemory->BP + WonJeong[pointerMemory->PC + 2])] = cpuWonJeong->top();
		cpuWonJeong->pop();
		*tempResult += getOpcode(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC + 1]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC + 2]);
		*tempResult += "\r\n";
		SetWindowText(*edResult, (tempResult)->c_str());
		return pointerMemory->PC + 3;

	// ldi - 완료
	case 13:
		i = WonJeong[(MAX_STACK - 1) - cpuWonJeong->top()];
		cpuWonJeong->pop();
		cpuWonJeong->push(i);
		*tempResult += getOpcode(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC]);
		*tempResult += "\r\n";
		SetWindowText(*edResult, (tempResult)->c_str());
		return pointerMemory->PC + 1;

	// sti - 완료 11
	case 14:
		// 값
		a = cpuWonJeong->top();
		cpuWonJeong->pop();

		// 번지
		b = cpuWonJeong->top();
		cpuWonJeong->pop();

		WonJeong[(MAX_STACK - 1) - b] = a;
		*tempResult += getOpcode(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC]);
		*tempResult += "\r\n";
		SetWindowText(*edResult, (tempResult)->c_str());
		return pointerMemory->PC + 1;

	// not - 완료 11
	case 15:
		a = cpuWonJeong->top();
		cpuWonJeong->pop();
		cpuWonJeong->push(!a);
		*tempResult += getOpcode(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC]);
		*tempResult += "\r\n";
		SetWindowText(*edResult, (tempResult)->c_str());
		return pointerMemory->PC + 1;

	// neg - 완료 11
	case 16:
		a = cpuWonJeong->top();
		cpuWonJeong->pop();
		cpuWonJeong->push((-1) * a);
		*tempResult += getOpcode(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC]);
		*tempResult += "\r\n";
		SetWindowText(*edResult, (tempResult)->c_str());
		return pointerMemory->PC + 1;

	// inc - 완료 11
	case 17:
		a = cpuWonJeong->top();
		cpuWonJeong->pop();
		cpuWonJeong->push(a + 1);
		*tempResult += getOpcode(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC]);
		*tempResult += "\r\n";
		SetWindowText(*edResult, (tempResult)->c_str());
		return pointerMemory->PC + 1;

	// dec - 완료 11
	case 18:
		a = cpuWonJeong->top();
		cpuWonJeong->pop();
		cpuWonJeong->push(a - 1);
		*tempResult += getOpcode(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC]);
		*tempResult += "\r\n";
		SetWindowText(*edResult, (tempResult)->c_str());
		return pointerMemory->PC + 1;

	// dup - 완료 11
	case 19:
		a = cpuWonJeong->top();
		cpuWonJeong->push(a);
		*tempResult += getOpcode(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC]);
		*tempResult += "\r\n";
		SetWindowText(*edResult, (tempResult)->c_str());
		return pointerMemory->PC + 1;

	// add - 완료 11
	case 20:
		b = cpuWonJeong->top();
		cpuWonJeong->pop();
		a = cpuWonJeong->top();
		cpuWonJeong->pop();
		cpuWonJeong->push(a + b);
		*tempResult += getOpcode(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC]);
		*tempResult += "\r\n";
		SetWindowText(*edResult, (tempResult)->c_str());
		return pointerMemory->PC + 1;

	// sub - 완료 11
	case 21:
		b = cpuWonJeong->top();
		cpuWonJeong->pop();
		a = cpuWonJeong->top();
		cpuWonJeong->pop();
		cpuWonJeong->push(a - b);
		*tempResult += getOpcode(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC]);
		*tempResult += "\r\n";
		SetWindowText(*edResult, (tempResult)->c_str());
		return pointerMemory->PC + 1;

	// mult - 완료 11
	case 22:
		b = cpuWonJeong->top();
		cpuWonJeong->pop();
		a = cpuWonJeong->top();
		cpuWonJeong->pop();
		cpuWonJeong->push(a * b);
		*tempResult += getOpcode(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC]);
		*tempResult += "\r\n";
		SetWindowText(*edResult, (tempResult)->c_str());
		return pointerMemory->PC + 1;

	// div - 완료 11
	case 23:
		b = cpuWonJeong->top();
		cpuWonJeong->pop();
		a = cpuWonJeong->top();
		cpuWonJeong->pop();
		cpuWonJeong->push(a / b);
		*tempResult += getOpcode(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC]);
		*tempResult += "\r\n";
		SetWindowText(*edResult, (tempResult)->c_str());
		return pointerMemory->PC + 1;

	// mod - 완료 11
	case 24:
		b = cpuWonJeong->top();
		cpuWonJeong->pop();
		a = cpuWonJeong->top();
		cpuWonJeong->pop();
		cpuWonJeong->push(a % b);
		*tempResult += getOpcode(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC]);
		*tempResult += "\r\n";
		SetWindowText(*edResult, (tempResult)->c_str());
		return pointerMemory->PC + 1;

	// gt - 완료 11
	case 25:
		b = cpuWonJeong->top();
		cpuWonJeong->pop();
		a = cpuWonJeong->top();
		cpuWonJeong->pop();
		cpuWonJeong->push(a > b);
		*tempResult += getOpcode(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC]);
		*tempResult += "\r\n";
		SetWindowText(*edResult, (tempResult)->c_str());
		return pointerMemory->PC + 1;

	// lt - 완료 11
	case 26:
		b = cpuWonJeong->top();
		cpuWonJeong->pop();
		a = cpuWonJeong->top();
		cpuWonJeong->pop();
		cpuWonJeong->push(a < b);
		*tempResult += getOpcode(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC]);
		*tempResult += "\r\n";
		SetWindowText(*edResult, (tempResult)->c_str());
		return pointerMemory->PC + 1;

	// ge - 완료 11
	case 27:
		b = cpuWonJeong->top();
		cpuWonJeong->pop();
		a = cpuWonJeong->top();
		cpuWonJeong->pop();
		cpuWonJeong->push(a >= b);
		*tempResult += getOpcode(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC]);
		*tempResult += "\r\n";
		SetWindowText(*edResult, (tempResult)->c_str());
		return pointerMemory->PC + 1;

	// le - 완료 11
	case 28:
		b = cpuWonJeong->top();
		cpuWonJeong->pop();
		a = cpuWonJeong->top();
		cpuWonJeong->pop();
		cpuWonJeong->push(a <= b);
		*tempResult += getOpcode(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC]);
		*tempResult += "\r\n";
		SetWindowText(*edResult, (tempResult)->c_str());
		return pointerMemory->PC + 1;

	// eq - 완료 11
	case 29:
		b = cpuWonJeong->top();
		cpuWonJeong->pop();
		a = cpuWonJeong->top();
		cpuWonJeong->pop();
		cpuWonJeong->push(a == b);
		*tempResult += getOpcode(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC]);
		*tempResult += "\r\n";
		SetWindowText(*edResult, (tempResult)->c_str());
		return pointerMemory->PC + 1;

	// ne - 완료 11
	case 30:
		b = cpuWonJeong->top();
		cpuWonJeong->pop();
		a = cpuWonJeong->top();
		cpuWonJeong->pop();
		cpuWonJeong->push(a != b);
		*tempResult += getOpcode(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC]);
		*tempResult += "\r\n";
		SetWindowText(*edResult, (tempResult)->c_str());
		return pointerMemory->PC + 1;

	// and - 완료 11
	case 31:
		b = cpuWonJeong->top();
		cpuWonJeong->pop();
		a = cpuWonJeong->top();
		cpuWonJeong->pop();
		cpuWonJeong->push(a && b);
		*tempResult += getOpcode(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC]);
		*tempResult += "\r\n";
		SetWindowText(*edResult, (tempResult)->c_str());
		return pointerMemory->PC + 1;

	// or - 완료 11
	case 32:
		b = cpuWonJeong->top();
		cpuWonJeong->pop();
		a = cpuWonJeong->top();
		cpuWonJeong->pop();
		cpuWonJeong->push(a || b);
		*tempResult += getOpcode(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC]);
		*tempResult += "\r\n";
		SetWindowText(*edResult, (tempResult)->c_str());
		return pointerMemory->PC + 1;

	// swp - 완료 11
	case 33:
		b = cpuWonJeong->top();
		cpuWonJeong->pop();
		a = cpuWonJeong->top();
		cpuWonJeong->pop();
		cpuWonJeong->push(b);
		cpuWonJeong->push(a);
		*tempResult += getOpcode(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC]);
		*tempResult += "\r\n";
		SetWindowText(*edResult, (tempResult)->c_str());
		return pointerMemory->PC + 1;

	// ujp - 완료
	case 34:
		for (i = 0;; i++) if (WonJeong[pointerMemory->PC + 1] == (labelValue + i)->position) break;
		*tempResult += getOpcode(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC]);
		*tempResult += " ";
		*tempResult += to_string(WonJeong[pointerMemory->PC + 1]);
		*tempResult += "\r\n";
		SetWindowText(*edResult, (tempResult)->c_str());
		return (labelValue + i)->position;

	// tjp - 완료
	case 35:
		for (i = 0;; i++) if (WonJeong[pointerMemory->PC + 1] == (labelValue + i)->position) break;
		if (cpuWonJeong->top()) {
			cpuWonJeong->pop();
			*tempResult += getOpcode(WonJeong[pointerMemory->PC]);
			*tempResult += " ";
			*tempResult += to_string(WonJeong[pointerMemory->PC]);
			*tempResult += " ";
			*tempResult += to_string(WonJeong[pointerMemory->PC + 1]);
			*tempResult += "\r\n";
			SetWindowText(*edResult, (tempResult)->c_str());
			return (labelValue + i)->position;
		}
		else {
			cpuWonJeong->pop();
			*tempResult += getOpcode(WonJeong[pointerMemory->PC]);
			*tempResult += " ";
			*tempResult += to_string(WonJeong[pointerMemory->PC]);
			*tempResult += " ";
			*tempResult += to_string(WonJeong[pointerMemory->PC + 1]);
			*tempResult += "\r\n";
			SetWindowText(*edResult, (tempResult)->c_str());
			return pointerMemory->PC + 2;
		}

	// fjp - 완료
	case 36:
 		for (i = 0;; i++) if (WonJeong[pointerMemory->PC + 1] == (labelValue + i)->position) break;
 		if (!cpuWonJeong->top()) {
 			cpuWonJeong->pop();
			*tempResult += getOpcode(WonJeong[pointerMemory->PC]);
			*tempResult += " ";
			*tempResult += to_string(WonJeong[pointerMemory->PC]);
			*tempResult += " ";
			*tempResult += to_string(WonJeong[pointerMemory->PC + 1]);
			*tempResult += "\r\n";
			SetWindowText(*edResult, (tempResult)->c_str());
			return (labelValue + i)->position;
		}
		else {
			cpuWonJeong->pop();
			*tempResult += getOpcode(WonJeong[pointerMemory->PC]);
			*tempResult += " ";
			*tempResult += to_string(WonJeong[pointerMemory->PC]);
			*tempResult += " ";
			*tempResult += to_string(WonJeong[pointerMemory->PC + 1]);
			*tempResult += "\r\n";
			SetWindowText(*edResult, (tempResult)->c_str());
			return pointerMemory->PC + 2;
		}

	default:
		return -1;
	}	
}
