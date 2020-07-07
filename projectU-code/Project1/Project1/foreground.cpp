#include "UwonJeong.h"

// API ����
LRESULT CALLBACK WndProc(HWND hWnd, UINT mesg, WPARAM wParam, LPARAM lParam) {
	static HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);

	// ��ư �ڵ�
	static HWND btnNextop, btnRun, btnLoaduco, btnSavelst, edRead, edWrite, edResult;

	// ����Ʈ �ڵ�
	static HWND lsUco, lsStack, lsMStack;
	static LVCOLUMN COL;
	static LVITEM lvI;

	// ��� ����
	static OPENFILENAME ofn;
	static TCHAR szFile[260] = { NULL };

	// ���� ����
	static string *pFileString, tempString;
	static labelPosition *labelValue = NULL;
	static instructionStruct *instructionLink = NULL;
	static int labelCount;
	static ifstream file;
	static ofstream output;
	static int intLabelPosition, instructionLine, cnt, bgnUcoIndex;
	static const char* constInstruction;
	static char* tempInttoString;
	static char pInstruction[10] = { NULL };

	// ��� ����
	static string tempResult;

	// ���� Level
	static int    excuteCount     = 0;
	static int    resultLineCount = 0;
	static int    forInstruction  = 0;
	static int    beforePC        = 0;
	static int    countPCLine     = 0;
	static char       editBoxText[10];
	static int			   forInt = 0;
	static int		   valueCount = 0;
	char				 stet[100000];

	// �޸� �� CPUSTRUCT
	static int    WonJeong[MAX_STACK];
	static stack<int>  cpuWonJeong, temp;
	static cpuStruct    pointerMemory;
	
	switch (mesg) {
	case WM_CREATE:
		// ����Ʈ���� �κ�
		lsUco    = CreateWindow(WC_LISTVIEW, "", LVS_REPORT | WS_CHILD | WS_VISIBLE | WS_BORDER,  20,  10, 420, 380, hWnd, (HMENU)(WM_USER + 5), hInstance, NULL);
		lsStack  = CreateWindow(WC_LISTVIEW, "", LVS_REPORT | WS_CHILD | WS_VISIBLE | WS_BORDER, 460,  90, 100, 300, hWnd, (HMENU)(WM_USER + 6), hInstance, NULL);
		lsMStack = CreateWindow(WC_LISTVIEW, "", LVS_REPORT | WS_CHILD | WS_VISIBLE | WS_BORDER, 460, 400, 100, 300, hWnd, (HMENU)(WM_USER + 7), hInstance, NULL);
		
		// ��ư���� �κ�
		btnNextop  = CreateWindow("BUTTON", "Next Op",  BS_FLAT | WS_CHILD | WS_VISIBLE | WS_DISABLED, 460,  10, 100, 30, hWnd, (HMENU)(WM_USER + 1), hInstance, NULL);
		btnRun     = CreateWindow("BUTTON", "Run",      BS_FLAT | WS_CHILD | WS_VISIBLE | WS_DISABLED, 460,  50, 100, 30, hWnd, (HMENU)(WM_USER + 2), hInstance, NULL);
		btnLoaduco = CreateWindow("BUTTON", "Load uco", BS_FLAT | WS_CHILD | WS_VISIBLE,               460, 710, 100, 30, hWnd, (HMENU)(WM_USER + 3), hInstance, NULL);
		btnSavelst = CreateWindow("BUTTON", "Save lst", BS_FLAT | WS_CHILD | WS_VISIBLE | WS_DISABLED, 460, 750, 100, 30, hWnd, (HMENU)(WM_USER + 4), hInstance, NULL);

		// �������� �κ�
		edRead   = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER                                                 , 380, 400,  60,  30, hWnd, (HMENU)(WM_USER +  8), hInstance, NULL);
		edResult = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | WS_BORDER | ES_MULTILINE,  20, 400, 350, 380, hWnd, (HMENU)(WM_USER +  9), hInstance, NULL);
		edWrite  = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | WS_BORDER | ES_MULTILINE, 380, 440,  60, 340, hWnd, (HMENU)(WM_USER + 10), hInstance, NULL);

		// ����Ʈ ���̺� �߰�
		ListView_SetExtendedListViewStyle(lsUco, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
		COL.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		COL.fmt  = LVCFMT_LEFT;

		COL.cx = 70;
		COL.pszText = (LPSTR)"��ȣ";
		COL.iSubItem = 0;
		ListView_InsertColumn(lsUco, 0, &COL);
		COL.cx = 70;
		COL.pszText = (LPSTR)"��";
		COL.iSubItem = 1;
		ListView_InsertColumn(lsUco, 1, &COL);
		COL.cx = 70;
		COL.pszText = (LPSTR)"��ɾ�";
		COL.iSubItem = 2;
		ListView_InsertColumn(lsUco, 2, &COL);
		COL.cx = 70;
		COL.pszText = (LPSTR)"��1";
		COL.iSubItem = 3;
		ListView_InsertColumn(lsUco, 3, &COL);
		COL.cx = 70;
		COL.pszText = (LPSTR)"��2";
		COL.iSubItem = 4;
		ListView_InsertColumn(lsUco, 4, &COL);
		COL.cx = 70;
		COL.pszText = (LPSTR)"��3";
		COL.iSubItem = 5;
		ListView_InsertColumn(lsUco, 5, &COL);

		COL.cx = 100;
		COL.pszText = (LPSTR)"����";
		COL.iSubItem = 0;
		ListView_InsertColumn(lsStack, 0, &COL);

		COL.cx = 100;
		COL.pszText = (LPSTR)"�޸𸮽���";
		COL.iSubItem = 0;
		ListView_InsertColumn(lsMStack, 0, &COL);

		// ������ �߰� �ʱ� ����
		lvI.mask = LVIF_TEXT;
		lvI.iSubItem = intLabelPosition;
		break;

	case WM_COMMAND:
		// uco �ε� �� assemble �ܰ� �۾�
		if (LOWORD(wParam) == WM_USER + 3) {
			ofn.lStructSize     =                           sizeof(ofn);
			ofn.hwndOwner       =                                  hWnd;
			ofn.lpstrFile       =                                szFile;
			ofn.nMaxFile        =                        sizeof(szFile);
			ofn.lpstrFilter     =                    _T("uco\0*.uco\0");
			ofn.nFilterIndex    =                                     1;
			ofn.lpstrFileTitle  =                                  NULL;
			ofn.nMaxFileTitle   =                                     0;
			ofn.lpstrInitialDir =                                  NULL;
			ofn.Flags           = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			if (GetOpenFileName(&ofn) != TRUE) break;

			EnableWindow(btnNextop,   TRUE);
			EnableWindow(btnRun,      TRUE);
			EnableWindow(btnLoaduco,  TRUE);
			EnableWindow(btnSavelst, FALSE);

			// �� �ʱ�ȭ
			bgnUcoIndex       =     0;
			labelCount        =     0;
			intLabelPosition  =     0;
			instructionLine   =     0;
			excuteCount       =     0;
			resultLineCount   =     0;
			beforePC          =     0;
			pointerMemory.PC  =     0;
			pointerMemory.DS  =     0;
			pointerMemory.SS  =     0;
			pointerMemory.SP  =     0;
			pointerMemory.BP  =     0;
			pointerMemory.end = false;
			SetWindowText(edResult, "");
			ListView_DeleteAllItems(lsUco);
			ListView_DeleteAllItems(lsStack);
			ListView_DeleteAllItems(lsMStack);

			while (!cpuWonJeong.empty()) cpuWonJeong.pop();

			// �� �� Ȯ��
			file = ifstream(ofn.lpstrFile);
			while (getline(file, tempString)) instructionLine++;
			file.close();

			// �� count ó�� �� ������ �߰�
			file            = ifstream(ofn.lpstrFile);
			pFileString     = new string[instructionLine];
			instructionLink = new instructionStruct[instructionLine];
			tempInttoString = new char[1000];

			while (getline(file, tempString)) {
				// ������ �߰�
				lvI.iItem = intLabelPosition + 1;
				_itoa_s(intLabelPosition + 1, tempInttoString, sizeof(tempInttoString), 10);
				lvI.pszText = (LPSTR)tempInttoString;
				ListView_InsertItem(lsUco, &lvI);

				// �� count ó��
				instructionLink[intLabelPosition] = *returnInstruction(tempString);
				pFileString[intLabelPosition]     =                     tempString;
				
				constInstruction = tempString.c_str();
				for (int i = 0; i < 10; i++) {
					pInstruction[i] = NULL;
					if (*(constInstruction + i) == ' ') break;
					pInstruction[i] = *(constInstruction + i);
				}

				if (pInstruction[0] != NULL) labelCount++;

				// ��
				ListView_SetItemText(lsUco, intLabelPosition, 1, (LPSTR)instructionLink[intLabelPosition].label);
				// ��ɾ�
				ListView_SetItemText(lsUco, intLabelPosition, 2, (LPSTR)getOpcode(instructionLink[intLabelPosition].instruction));
				// ��1
				ListView_SetItemText(lsUco, intLabelPosition, 3, (LPSTR)instructionLink[intLabelPosition].val1);
				// ��2
				ListView_SetItemText(lsUco, intLabelPosition, 4, (LPSTR)instructionLink[intLabelPosition].val2);
				// ��3
				ListView_SetItemText(lsUco, intLabelPosition, 5, (LPSTR)instructionLink[intLabelPosition].val3);

				// bgn��ɾ� ������ġ�� ���
				if (instructionLink[intLabelPosition].instruction == 1) bgnUcoIndex = intLabelPosition;

				intLabelPosition++;
			}
			file.close();

			// LabelPosition ����
			labelValue = new labelPosition[labelCount + 3];
			for (int i = 0, cnt = 0; i < instructionLine; i++) {
				constInstruction = pFileString[i].c_str();

				for (int j = 0; j < 10; j++) pInstruction[j] = NULL;

				for (int j = 0; j < 10; j++) {
					if (*(constInstruction + j) == ' ') break;
					pInstruction[j] = *(constInstruction + j);
				}

				if (pInstruction[0] != NULL) {
					strcpy_s((labelValue + cnt)->label, 10, pInstruction);
					(labelValue + cnt++)->position = i;
				}
			}

			for (int i = 0; i < 10; i++) pInstruction[i] = NULL;

			// Code ���� �� DS�� ����
			pointerMemory.DS = WonJeongCode(WonJeong, instructionLink, labelValue, instructionLine);

			// read write lf
			systemFuntion(WonJeong, labelValue, &pointerMemory, labelCount + 3);
		}

		// Savelst
		if (LOWORD(wParam) == WM_USER + 4) fileSave(tempResult, resultLineCount, &edWrite, labelValue);

		// Run
		if (LOWORD(wParam) == WM_USER + 2) {
			while (!pointerMemory.end) {
				// ������ �� �����ߴ��� Ȯ��
				if (pointerMemory.end) break;

				// read Ȯ��
				if (WonJeong[pointerMemory.PC + 1] == -3) {
					GetWindowText(edRead, stet, 100000);
					if (strcmp(stet, "") == 0) {
						MessageBox(hWnd, "���� �Է����ּ���.", "Error", MB_OK);
						break;
					}
				}

				countPCLine = pointerMemory.PC;
				GetWindowText(edRead, editBoxText, 10);
				
				pointerMemory.PC = excutionFuntion(WonJeong, &cpuWonJeong, instructionLink, labelValue, &pointerMemory, beforePC, atoi(editBoxText), &edWrite, &edResult, &tempResult);
				if (WonJeong[pointerMemory.PC + 1] == -3) SetWindowText(edRead, "");

				// �޸𸮽��� ������Ʈ
				ListView_DeleteAllItems(lsMStack);
				for (int i = pointerMemory.SP - 1; i >= 0; i--) {
					_itoa_s(WonJeong[(MAX_STACK - 1) - i], tempInttoString, 10, 10);
					lvI.pszText = (LPSTR)tempInttoString;
					ListView_InsertItem(lsMStack, &lvI);
				}

				// ���� ������Ʈ
				ListView_DeleteAllItems(lsStack);
				forInt = cpuWonJeong.size();
				for (int i = 0; i < forInt; i++) {
					_itoa_s(cpuWonJeong.top(), tempInttoString, 10, 10);
					lvI.pszText = (LPSTR)tempInttoString;
					ListView_InsertItem(lsStack, &lvI);
					temp.push(cpuWonJeong.top());
					cpuWonJeong.pop();
				}

				forInt = temp.size();
				for (int i = 0; i < forInt; i++) {
					cpuWonJeong.push(temp.top());
					temp.pop();
				}

				beforePC = pointerMemory.PC;
				resultLineCount++;

				if (pointerMemory.end) {
					EnableWindow(btnNextop, FALSE);
					EnableWindow(btnRun,    FALSE);
					EnableWindow(btnSavelst, TRUE);
				}
			}
		}
			
		// NextOp
		if (LOWORD(wParam) == WM_USER + 1) {
			// ������ �� �����ߴ��� Ȯ��
			if (pointerMemory.end) break;

			// read Ȯ��
			if (WonJeong[pointerMemory.PC + 1] == -3) {
				GetWindowText(edRead, stet, 100000);
				if (strcmp(stet, "") == 0) {
					MessageBox(hWnd, "���� �Է����ּ���.", "Error", MB_OK);
					break;
				}
			}

			countPCLine = pointerMemory.PC;
			GetWindowText(edRead, editBoxText, 10);
			pointerMemory.PC = excutionFuntion(WonJeong, &cpuWonJeong, instructionLink, labelValue, &pointerMemory, beforePC, atoi(editBoxText), &edWrite, &edResult, &tempResult);
			if (WonJeong[pointerMemory.PC + 1] == -3) SetWindowText(edRead, "");

			// �޸𸮽��� ������Ʈ
			ListView_DeleteAllItems(lsMStack);
			for (int i = pointerMemory.SP - 1; i >= 0; i--) {
				_itoa_s(WonJeong[(MAX_STACK - 1) - i], tempInttoString, 10, 10);
				lvI.pszText = (LPSTR)tempInttoString;
				ListView_InsertItem(lsMStack, &lvI);
			}

			// ���� ������Ʈ
			ListView_DeleteAllItems(lsStack);
			forInt = cpuWonJeong.size();
			for (int i = 0; i < forInt; i++) {
				_itoa_s(cpuWonJeong.top(), tempInttoString, 10, 10);
				lvI.pszText = (LPSTR)tempInttoString;
				ListView_InsertItem(lsStack, &lvI);
				temp.push(cpuWonJeong.top());
				cpuWonJeong.pop();
			}

			forInt = temp.size();
			for (int i = 0; i < forInt; i++) {
				cpuWonJeong.push(temp.top());
				temp.pop();
			}

			beforePC = pointerMemory.PC;
			resultLineCount++;

			if (pointerMemory.end) {
				EnableWindow(btnNextop, FALSE);
				EnableWindow(btnRun,    FALSE);
				EnableWindow(btnSavelst, TRUE);
			}
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return FALSE;
	}

	return DefWindowProc(hWnd, mesg, wParam, lParam);
}


// ���� ������ ����
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprevInstance, LPSTR lpszArg, int nCmdShow) {
	HWND hWnd;
	MSG msg;
	WNDCLASS WndClass;

	WndClass.style = NULL;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = "hwnd";
	if (!RegisterClass(&WndClass)) return NULL;

	hWnd = CreateWindow("hwnd", "��ǻ�Ͱ��а� 2�г� ������ 20160434", WS_CAPTION | WS_POPUPWINDOW, 400, 100, 600, 831, NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}