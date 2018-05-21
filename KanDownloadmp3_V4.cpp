#include <iostream>
#include <fstream>  
#include <cstring>
#include <sstream> 

using namespace std;

/*
1. 增加了艦娘名稱的輸入(手動模式可不輸入)(批量模式可使用V3的格式來不輸入)
2. 因為檔案讀取非ANSI字元會產生亂碼，故加入檔案格式判斷 
*/ 

void outputMOD0(),outputMOD1(),GetTextFileType(string ),Fileoutput(int ,string ,string );
int shipid = 0;
string shipswf,shipname = "";

int main(){
    int MOD = 0;
    
    cout<<"使用模式說明:"<<endl;
    cout<<"手動模式 0"<<endl;
    cout<<"批量模式 1"<<endl;
    cout<<"備註1 : 批量模式請準備好字典檔kanID_swfName_V4.txt"<<endl;
    cout<<"備註2 : 支援V3的字典檔格式，請準備好字典檔kanID_swfName_V3.txt"<<endl;
    cout<<"***********************************************************"<<endl;
    cout<<"V4字典檔格式範例  V3字典檔格式範例"<<endl;
    cout<<"1                 1"<<endl;
    cout<<"睦月              snohitatusbk"<<endl;
    cout<<"snohitatusbk"<<endl;
    cout<<"***********************************************************"<<endl;
    cout<<"模式選擇(0|1)"<<endl; 
    cin>>MOD;
    if(MOD == 0) outputMOD0();
    if(MOD == 1) outputMOD1();
    cout<<"下載連結已產生完成"<<endl; 
    system("pause");
}

void outputMOD0(){
    cout<<"輸入艦娘ID : "<<endl;
    cin>>shipid;
    cout<<"輸入艦娘名稱(可不輸入) : "<<endl;
    cin.ignore();
    getline(cin,shipname);
    cout<<"輸入艦娘swf包名 : "<<endl;
    cin>>shipswf;
    Fileoutput(shipid,shipname,shipswf);
}

void outputMOD1(){
    int txtMOD = 4,m = 3;
    
    cout<<"選擇字典檔格式"<<endl;
    cout<<"V3請輸入 : 3"<<endl;
    cout<<"V4請輸入 : 4"<<endl;
    cout<<"備註 : V4限用ANSI格式字典檔"<<endl;
    cin>>txtMOD;
    string DictionaryFile = "";
    if(txtMOD == 3){ DictionaryFile = "kanID_swfName_V3.txt"; m = 2; }
    if(txtMOD == 4){ DictionaryFile = "kanID_swfName_V4.txt"; GetTextFileType(DictionaryFile); }
    ifstream idin;
    idin.open(DictionaryFile.c_str(),ios::in);
    string line;
    stringstream ssline;
    int i=0;
    if(idin) //存在DictionaryFile文件
    { 
        while (getline (idin, line)){ // line中不包括每行的換行符  
            if(i%m == 0){
                /* string to int */ 
                ssline << line; //以串流運算子寫入ssline
                ssline >> shipid; //以串流運算子寫入shipid 
                ssline.clear(); //清空stringstream空間 
            }
            if(i%m == 1){
                shipname = line;
                if(txtMOD == 3){ shipswf = line; Fileoutput(shipid,shipname,shipswf); }
            }
            else if(i%m == 2){
                shipswf = line;
                Fileoutput(shipid,shipname,shipswf);
            }
            i++;
        }
    }
    else{ //沒有該文件
        cout<<"no such file"<<endl; 
    }
}

void GetTextFileType(string DictionaryFile){
    string fileType = "ANSI";
    int Error = 0;
    ifstream file;
    file.open(DictionaryFile.c_str(), ios::in);
    /* 開始判斷文件格式 */ 
    if (file.good())
    {
        char szFlag[3] = {0};
        file.read(szFlag, sizeof(char) * 3);
        if ((unsigned char)szFlag[0] == 0xFF && (unsigned char)szFlag[1] == 0xFE){
            fileType = "UTF-16";
            cout <<"偵測到字典檔格式為UTF-16，請將字典檔改為ANSI格式"<<endl;
            Error = 1;
        }
        else if ((unsigned char)szFlag[0] == 0xEF && (unsigned char)szFlag[1] == 0xBB && (unsigned char)szFlag[2] == 0xBF){
            fileType = "UTF-8";
            cout <<"偵測到字典檔格式為UTF-8，請將字典檔改為ANSI格式"<<endl;
            Error = 1;
        }
    }
    else{ //沒有該文件  
        cout<<"no such file"<<endl;
        Error = 1;
    }
    file.close();
    if(Error == 1){
        system("pause");
        exit(1); //void副程式不能用return 
    }
}

void Fileoutput(int shipid,string shipname,string shipswf){
    int a[54]={604825,607300,613847,615318,624009,631856,635451,637218,640529,
               643036,652687,658008,662481,669598,675545,685034,687703,696444,
               702593,703894,711191,714166,720579,728970,738675,740918,743009,
               747240,750347,759846,764051,770064,773457,779858,786843,790526,
               799973,803260,808441,816028,825381,827516,832463,837868,843091,
               852548,858315,867580,875771,879698,882759,885564,888837,896168};
    stringstream Tempname;
    ofstream outputfilenameout;
    Tempname.str("");
    if(shipname != "") Tempname<<shipid<<"_"<<shipname; //以串流運算子合併寫入Tempname
    else Tempname<<shipid; //以串流運算子寫入Tempname
    string FolderName = Tempname.str();
    Tempname<<".txt"; //以串流運算子寫入Tempname
    string outputfilename = Tempname.str();
    outputfilenameout.open(outputfilename.c_str(),ios::out);
    for(int i=0;i<53;i++){
        outputfilenameout << "http://203.104.209.102/kcs/sound/kc"<<shipswf<<"/"<<(shipid+7)*17*(a[i+1]-a[i])%99173+100000<<".mp3" << endl; // 輸出到txt中
    }
    outputfilenameout.close(); //因檔案要交由win api控制，所以要關閉寫入狀態
    cout<<"輸出"<<outputfilename<<".txt"<<endl;
    /* 使用到win api，所以要char* 形式傳回string內字串 */
    string CreateFolder = "MD " + FolderName;
    system(CreateFolder.c_str());
    string MoveFile = "Move " + Tempname.str() + " " + FolderName +"/"+ Tempname.str();
    system(MoveFile.c_str());
}
