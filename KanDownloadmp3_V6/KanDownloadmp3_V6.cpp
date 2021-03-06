#include "stdafx.h"
#include "urlmon.h"
#include <iostream>
#include <fstream>  
#include <string>
#include <codecvt> //編碼轉換
#include <locale> //地區編碼

#pragma comment(lib,"urlmon.lib")

using namespace std;

/* 用於轉換utf-8 -> utf-16，即string -> wstring */
wstring utf8_to_wstring(const string str){
	wstring_convert<codecvt_utf8<wchar_t>> convert;
	return convert.from_bytes(str);
}

/*
1. 手動模式可不輸入艦娘名稱,批量模式可使用V3的格式來不輸入
2. 增加了UTF-8格式的支持，但移除了ANSI格式支持
3. 程式碼全面使用 寬字元 進行開發
4. 對不同編碼的windows系統的部分顯示支持(消除亂碼)
5. 輸入檔案編碼判斷(確保為UTF-8格式)
6. 新增輸出之失敗報告為UTF-8格式
7. 新增直接對計算出之mp3檔案連結直接下載功能
*/

void outputMOD1(), outputMOD2(), Fileoutput(int, wstring, wstring), GetTextFileType(wstring);
int shipid = 0, MOD = 0;
wstring shipswf = L"", shipname = L"", DictionaryFile = L"";
wfstream errlog;
fstream idin;

int wmain() {
	locale::global(locale(""));
	wcin.imbue(locale(""));
	wcout.imbue(locale(""));
	errlog.imbue(locale(locale(), new codecvt_utf8_utf16<wchar_t>)); //write file as UTF-8
	int MOD = 0;

	wcout << L"使用模式說明:\n";
	wcout << L"批量模式 1\n";
	wcout << L"手動模式 2\n";
	wcout << L"備註1 : 批量模式請準備好字典檔\n";
	wcout << L"備註2 : 請使用(UTF-8)格式字典檔\n";
	wcout << L"***********************************************************\n";
	wcout << L"類型A字典檔格式範例  類型B字典檔格式範例\n";
	wcout << L"1                    1\n";
	wcout << L"睦月                 snohitatusbk\n";
	wcout << L"snohitatusbk\n";
	wcout << L"***********************************************************\n";
	wcout << L"模式選擇(1|2)\n";
	while (MOD != 1 && MOD != 2) {
		wcin >> MOD;
	}
	errlog.open(L"DownLoadFailed.txt",ios::out|ios::binary);
	/* 創建資料夾 */
	_wsystem(L"MD 艦隊Collection語音");
	if (MOD == 1) outputMOD1();
	if (MOD == 2) outputMOD2();
	errlog.close();
	idin.close();
	wcout << L"所有檔案已下載完成\n";
	_wsystem(L"pause");
}

void outputMOD1() {
	int txtMOD = 0, m = 3, i = 0;;
	string line;

	wcout << L"選擇字典檔格式\n";
	wcout << L"類型A請輸入 : 1\n";
	wcout << L"類型B請輸入 : 2\n";
	while (txtMOD != 1 && txtMOD != 2) {
		wcin >> txtMOD;
	}
	wcout << L"輸入字典檔路徑(可直接拖曳至視窗) :\n";
	while (DictionaryFile == L"") {
		wcin >> DictionaryFile;
	}
	GetTextFileType(DictionaryFile); //編碼判斷，若非UTF-8檔案則結束進程
	if (txtMOD == 2) m = 2;
	idin.open(DictionaryFile, ios::in);
	if (idin) //存在DictionaryFile文件
	{
		idin.seekg(3, ios::cur); //把檔案讀取指標從目前位置向後移3個字元
		while (getline(idin, line)) { //line中不包括每行的換行符  
			if (i%m == 0) {
				shipid = stoi(line); //string to int
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
		_wsystem(L"pause");
		exit(1);
	}
}

void outputMOD2() {
	wcout << L"輸入艦娘ID : \n";
	wcin >> shipid;
	wcout << L"輸入艦娘名稱(可不輸入) : \n";
	wcin.ignore();
	getline(wcin, shipname);
	wcout << L"輸入艦娘swf包名 : \n";
	wcin >> shipswf;
	Fileoutput(shipid, shipname, shipswf);
}

void Fileoutput(int shipid, wstring shipname, wstring shipswf) {
	int a[54] = { 604825,607300,613847,615318,624009,631856,635451,637218,640529,
		          643036,652687,658008,662481,669598,675545,685034,687703,696444,
		          702593,703894,711191,714166,720579,728970,738675,740918,743009,
		          747240,750347,759846,764051,770064,773457,779858,786843,790526,
		          799973,803260,808441,816028,825381,827516,832463,837868,843091,
		          852548,858315,867580,875771,879698,882759,885564,888837,896168 };
	wstring FolderName = L"艦隊Collection語音\\" + to_wstring(shipid);
	/* 若有艦娘名稱則ID加上名稱作為檔名 */
	if (shipname != L"") FolderName += L"_" + shipname;
	wstring outputfilename = FolderName + L".txt";
	/* 創建資料夾 */
	_wsystem((L"MD " + FolderName).c_str());
	int list = 0; //重置失敗計數
	for (int i = 0; i<53; i++) {
		wstring mp3name = to_wstring((shipid + 7) * 17 * (a[i + 1] - a[i]) % 99173 + 100000);
		wstring mp3url = L"http://203.104.209.102/kcs/sound/kc"+ shipswf + L"/" + mp3name + L".mp3";
		wstring mp3filefinsh = FolderName + L"/" + mp3name + L".mp3";
		/* 開始下載檔案 */
		HRESULT result = URLDownloadToFileW(NULL,mp3url.c_str(), mp3filefinsh.c_str(),0,NULL);
		if (result == S_OK) { wcout << mp3name << L".mp3" << L"檔案下載成功\n"; }
		else {
			/* 並非每個艦娘都有報時等語音，故可能算出無效連結 */
			wcout << mp3name << L".mp3" << L"檔案下載失敗\n";
			if (list == 0) errlog << L"\r\n<<" << shipid << "_" << shipname << L">>\r\n";
			errlog << L"http://203.104.209.102/kcs/sound/kc" << shipswf << L"/" << mp3name << L".mp3\r\n"; // 輸出到txt中
			list++;
		}
	}
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
		_wsystem(L"pause");
		exit(1); //void副程式不能用return 
	}
}