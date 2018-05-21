#include "stdafx.h"
#include <iostream>
#include <fstream>  
#include <cstring>
#include <sstream> //型別轉換
#include <codecvt> //編碼轉換
#include <locale>  //地區編碼設定

using namespace std;

/* 用於轉換utf-8 -> utf-16，即string -> wstring */
wstring utf8_to_wstring(const string str){
	wstring_convert<codecvt_utf8<wchar_t>> convert;
	return convert.from_bytes(str);
}

/*
1. 手動模式可不輸入艦娘名稱,批量模式可使用V3的格式來不輸入
2. 增加了UTF-8格式的支持，但移除了ANSI格式支持
3. 移除了endl改為\n，目的為去掉fflush(stdout)加快執行速度，並避免錯誤
4. 程式碼全面改用 寬字元 進行開發
5. 編碼判斷新增UTF-32
6. 增加對不同編碼的windows系統的支持(消除亂碼)
*/

void outputMOD0(), outputMOD1(), Fileoutput(int, wstring, wstring), GetTextFileType(wstring);
int shipid = 0;
wstring shipswf, shipname = L"";

int wmain() {
	/* 
	1. locale("")：調用構造函數創建一個local，其中的空字串具有特殊含義：
	使用客戶環境中缺省的locale。
	例如在簡體中文系統上，會返回簡體中文的locale。
    2. locale::global(locale(""))：將“C++標準IO庫的全域locale”設為“客戶環境中缺省的locale”。
	注意它還會設置C標準庫的locale環境，造成與“setlocale(LC_ALL, "")”類似的效果。
    3. wcout.imbue(locale(""))：使wcout使用“客戶環境中缺省的locale”。
	*/
	locale::global(locale(""));
	wcout.imbue(locale(""));
	int MOD = 0;

	wcout << L"使用模式說明:\n";
	wcout << L"手動模式 0\n";
	wcout << L"批量模式 1\n";
	wcout << L"備註1 : 批量模式請準備好字典檔kanID_swfName_V5.txt\n";
	wcout << L"備註2 : 支援V3的字典檔格式，請準備好字典檔kanID_swfName_V3.txt\n";
	wcout << L"備註3 : 不支援V4的字典檔格式(ANSI)，請使用V5的(UTF-8)格式字典檔\n";
	wcout << L"***********************************************************\n";
	wcout << L"V5字典檔格式範例  V3字典檔格式範例\n";
	wcout << L"1                 1\n";
	wcout << L"睦月              snohitatusbk\n";
	wcout << L"snohitatusbk\n";
	wcout << L"***********************************************************\n";
	wcout << L"模式選擇(0|1)\n";
	wcin >> MOD;
	if (MOD == 0) outputMOD0();
	if (MOD == 1) outputMOD1();
	wcout << L"下載連結已產生完成\n";
	system("pause");
}

void outputMOD0() {
	wcout << L"輸入艦娘ID : \n";
	wcin >> shipid;
	wcout << L"輸入艦娘名稱(可不輸入) : \n";
	wcin.ignore();
	getline(wcin, shipname);
	wcout << L"輸入艦娘swf包名 : \n";
	wcin >> shipswf;
	Fileoutput(shipid, shipname, shipswf);
}

void outputMOD1() {
	int txtMOD = 4, m = 3;

	wcout << L"選擇字典檔格式\n";
	wcout << L"V3請輸入 : 3\n";
	wcout << L"V5請輸入 : 5\n";
	wcin >> txtMOD;
	wstring DictionaryFile;
	if (txtMOD == 3) { DictionaryFile = L"kanID_swfName_V3.txt"; m = 2; }
	if (txtMOD == 5) {
		DictionaryFile = L"kanID_swfName_V5.txt";
		GetTextFileType(DictionaryFile); //編碼判斷，若非UTF-8檔案則結束進程
	}
	ifstream idin;
	idin.open(DictionaryFile, ios::in);
	string line;
	stringstream ssline;
	int i = 0;
	if (idin) //存在DictionaryFile文件
	{
		idin.seekg(3,ios::cur); //把檔案讀取指標從目前位置向後移3個字元
		while (getline(idin, line)) { //line中不包括每行的換行符  
			if (i%m == 0) {
				/* string to int */
				ssline << line; //以串流運算子寫入ssline
				ssline >> shipid; //以串流運算子寫入shipid
				ssline.clear(); //清空stringstream空間
			}
			if (i%m == 1) {
				shipname = utf8_to_wstring(line);
				if (txtMOD == 3) { shipswf = utf8_to_wstring(line); Fileoutput(shipid, shipname, shipswf); }
			}
			else if (i%m == 2) {
				shipswf = utf8_to_wstring(line);
				Fileoutput(shipid, shipname, shipswf);
			}
			i++;
		}
	}
	else { //沒有該文件
		wcout << L"no such file\n";
		system("pause");
		exit(1);
	}
}

void Fileoutput(int shipid, wstring shipname, wstring shipswf) {
	int a[54] = { 604825,607300,613847,615318,624009,631856,635451,637218,640529,
		          643036,652687,658008,662481,669598,675545,685034,687703,696444,
		          702593,703894,711191,714166,720579,728970,738675,740918,743009,
		          747240,750347,759846,764051,770064,773457,779858,786843,790526,
		          799973,803260,808441,816028,825381,827516,832463,837868,843091,
		          852548,858315,867580,875771,879698,882759,885564,888837,896168 };
	wstringstream Tempname;
	wstring replace = L",";
	wstring replacefinsh = L"";
	wofstream outputfilenameout;
	
	Tempname << shipid; //以串流運算子寫入Tempname(因shipid為Int)
	wstring FolderName = Tempname.str();
	Tempname.clear();
	/* 確保產生','才做取代，否則錯誤 */
	if (shipid / 1000 >= 1) {
		size_t nPosid = FolderName.find(replace);
		FolderName.replace(nPosid, replace.length(), replacefinsh);
	}
	/* 若有艦娘名稱則ID加上名稱作為檔名 */
	if (shipname != L"") FolderName += L"_" + shipname;
	wstring outputfilename = FolderName + L".txt";
	outputfilenameout.open(outputfilename, ios::out);
	for (int i = 0; i<53; i++) {
		wstringstream mp3name;
		int mp3nameTemp = (shipid + 7) * 17 * (a[i + 1] - a[i]) % 99173 + 100000;
		mp3name.clear();
		mp3name << mp3nameTemp;
		wstring Newmp3name = mp3name.str();
		/* 找到wstring中的','並取代 */
		size_t nPosmp3 = Newmp3name.find(replace);
		Newmp3name.replace(nPosmp3, replace.length(), replacefinsh);

		outputfilenameout << "http://203.104.209.102/kcs/sound/kc" << shipswf << "/" << Newmp3name << ".mp3\n"; // 輸出到txt中
	}
	outputfilenameout.close(); //因檔案要交由win api控制，所以要關閉寫入狀態
	wcout << L"輸出" << outputfilename <<"\n";
	/* 使用到win api(utf-16)，所以要wchar_t* 形式傳回wstring內字串 */
	wstring CreateFolderTemp = L"MD " + FolderName;
	const wchar_t* CreateFolder = CreateFolderTemp.c_str();
	_wsystem(CreateFolder);
	wstring MoveFileTemp = L"Move " + outputfilename + L" " + FolderName + L"/" + outputfilename;
	const wchar_t* MoveFile = MoveFileTemp.c_str();
	_wsystem(MoveFile);
}

void GetTextFileType(wstring DictionaryFile) {
	wstring fileType = L"ANSI";
	int Error = 1;
	ifstream file;
	file.open(DictionaryFile, ios::in);
	/* 開始判斷文件格式 */
	if (file.good())
	{
		char szFlag[4] = { 0 };
		file.read(szFlag, sizeof(char) * 4);
		if ((unsigned char)szFlag[0] == 0xFF && (unsigned char)szFlag[1] == 0xFE) {
			fileType = L"UTF-16LE";
			wcout << L"偵測到字典檔格式為UTF-16LE，請將字典檔改為UTF-8格式\n";
		}
		if ((unsigned char)szFlag[0] == 0xFE && (unsigned char)szFlag[1] == 0xFF) {
			fileType = L"UTF-16BE";
			wcout << L"偵測到字典檔格式為UTF-16BE，請將字典檔改為UTF-8格式\n";
		}
		else if ((unsigned char)szFlag[0] == 0xEF && (unsigned char)szFlag[1] == 0xBB && (unsigned char)szFlag[2] == 0xBF) {
			fileType = L"UTF-8";
			Error = 0;
		}
		else if ((unsigned char)szFlag[0] == 0xFF && (unsigned char)szFlag[1] == 0xFE && (unsigned char)szFlag[2] == 0x00 && (unsigned char)szFlag[3] == 0x00) {
			fileType = L"UTF-32LE";
			wcout << L"偵測到字典檔格式為UTF-32LE，請將字典檔改為UTF-8格式\n";
		}
		else if ((unsigned char)szFlag[0] == 0x00 && (unsigned char)szFlag[1] == 0x00 && (unsigned char)szFlag[2] == 0xFE && (unsigned char)szFlag[3] == 0xFF) {
			fileType = L"UTF-32BE";
			wcout << L"偵測到字典檔格式為UTF-32BE，請將字典檔改為UTF-8格式\n";
		}
		else {
			wcout << L"偵測到字典檔格式為ANSI或其它地區格式，請將字典檔改為UTF-8格式\n";
		}
	}
	else { //沒有該文件  
		wcout << L"no such file\n";
	}
	file.close();
	//wcout << fileType << "\n";
	if (Error == 1) {
		system("pause");
		exit(1); //void副程式不能用return 
	}
}