#include <iostream>
#include <fstream>  
#include <cstring>
#include <sstream> 

using namespace std;

/*
1. �i�H��ܼҦ�0�μҦ�1�A�Ҧ�0�~�ӤFV2��������ʿ�J�ӿ�X�F�Ҧ�1�����r���ɡA�N���q��X 
2. �Q�ΰƵ{���i��Ҳդ�
3. �@�˳z�Lsstream�Ӱ�string���B�z(�Ҧpstring to int) 
*/ 

void outputMOD0(),outputMOD1(),Fileoutput(int ,string );
int shipid=0;
string shipswf;

int main(){
    int MOD=0;
    cout<<"�ϥμҦ�����:\n��ʼҦ� 0\n��q�Ҧ� 1\n�Ƶ�:��q�Ҧ��зǳƦn�r����kanID_swfName_V3.txt\n�r���ɮ榡�d��\n1\nsnohitatusbk\n�Ҧ����(0|1): "; 
    cin>>MOD;
    if(MOD == 0) outputMOD0();
    if(MOD == 1) outputMOD1();
    system("pause");
}

void outputMOD0(){
    cout<<"��Jshipid"<<endl;
    cin>>shipid;
    cout<<"��Jshipswf�]�W"<<endl;
    cin>>shipswf;
    Fileoutput(shipid,shipswf);
}

void outputMOD1(){
    ifstream idin("kanID_swfName_V3.txt");
    string line;
    stringstream ssline;
    int i=0;
    if(idin) // ��kanID_swfName_V3.txt���  
    {  
        while (getline (idin, line)) // line�����]�A�C�檺�����  
        {
            if(i%2 == 0){
                /* string to int */ 
                ssline << line; //�H��y�B��l�g�Jssline
                ssline >> shipid; //�H��y�B��l�g�Jshipid 
                ssline.clear(); //�M��stringstream�Ŷ� 
            }
            if(i%2 == 1){
                shipswf = line;
                Fileoutput(shipid,shipswf);
            }
            i++;
        }
    }
    else // �S���Ӥ��  
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
    Tempname<<shipid<<".txt"; //�H��y�B��l�X�ּg�JTempname
    string outputfilename = Tempname.str();
    outputfilenameout.open(outputfilename.c_str(),ios::out);
    for(int i=0;i<53;i++){
        outputfilenameout << "http://203.104.209.102/kcs/sound/kc"<<shipswf<<"/"<<(shipid+7)*17*(a[i+1]-a[i])%99173+100000<<".mp3" << endl; // ��X��txt��
    }
}
