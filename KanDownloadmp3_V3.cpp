#include <iostream>
#include <fstream>  
#include <cstring>
#include <sstream> 

using namespace std;

/*
1. 可以選擇模式0及模式1，模式0繼承了V2版本的手動輸入來輸出；模式1為給字典檔，就能批量輸出 
2. 利用副程式進行模組化
3. 一樣透過sstream來做string的處理(例如string to int) 
*/ 

void outputMOD0(),outputMOD1(),Fileoutput(int ,string );
int shipid=0;
string shipswf;

int main(){
    int MOD=0;
    cout<<"使用模式說明:\n手動模式 0\n批量模式 1\n備註:批量模式請準備好字典檔kanID_swfName_V3.txt\n字典檔格式範例\n1\nsnohitatusbk\n模式選擇(0|1): "; 
    cin>>MOD;
    if(MOD == 0) outputMOD0();
    if(MOD == 1) outputMOD1();
    system("pause");
}

void outputMOD0(){
    cout<<"輸入shipid"<<endl;
    cin>>shipid;
    cout<<"輸入shipswf包名"<<endl;
    cin>>shipswf;
    Fileoutput(shipid,shipswf);
}

void outputMOD1(){
    ifstream idin("kanID_swfName_V3.txt");
    string line;
    stringstream ssline;
    int i=0;
    if(idin) // 有kanID_swfName_V3.txt文件  
    {  
        while (getline (idin, line)) // line中不包括每行的換行符  
        {
            if(i%2 == 0){
                /* string to int */ 
                ssline << line; //以串流運算子寫入ssline
                ssline >> shipid; //以串流運算子寫入shipid 
                ssline.clear(); //清空stringstream空間 
            }
            if(i%2 == 1){
                shipswf = line;
                Fileoutput(shipid,shipswf);
            }
            i++;
        }
    }
    else // 沒有該文件  
    {  
        cout <<"no such file" << endl;  
    }
}

void Fileoutput(int shipid,string shipswf){
    int a[54]={604825,607300,613847,615318,624009,631856,635451,637218,640529,
               643036,652687,658008,662481,669598,675545,685034,687703,696444,
               702593,703894,711191,714166,720579,728970,738675,740918,743009,
               747240,750347,759846,764051,770064,773457,779858,786843,790526,
               799973,803260,808441,816028,825381,827516,832463,837868,843091,
               852548,858315,867580,875771,879698,882759,885564,888837,896168};
    stringstream Tempname;
    ofstream outputfilenameout;
    Tempname.str("");
    Tempname<<shipid<<".txt"; //以串流運算子合併寫入Tempname
    string outputfilename = Tempname.str();
    outputfilenameout.open(outputfilename.c_str(),ios::out);
    for(int i=0;i<53;i++){
        outputfilenameout << "http://203.104.209.102/kcs/sound/kc"<<shipswf<<"/"<<(shipid+7)*17*(a[i+1]-a[i])%99173+100000<<".mp3" << endl; // 輸出到txt中
    }
}
