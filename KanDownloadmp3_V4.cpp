#include <iostream>
#include <fstream>  
#include <cstring>
#include <sstream> 

using namespace std;

/*
1. �W�[�Fĥ�Q�W�٪���J(��ʼҦ��i����J)(��q�Ҧ��i�ϥ�V3���榡�Ӥ���J)
2. �]���ɮ�Ū���DANSI�r���|���ͶýX�A�G�[�J�ɮ׮榡�P�_ 
*/ 

void outputMOD0(),outputMOD1(),GetTextFileType(string ),Fileoutput(int ,string ,string );
int shipid = 0;
string shipswf,shipname = "";

int main(){
    int MOD = 0;
    
    cout<<"�ϥμҦ�����:"<<endl;
    cout<<"��ʼҦ� 0"<<endl;
    cout<<"��q�Ҧ� 1"<<endl;
    cout<<"�Ƶ�1 : ��q�Ҧ��зǳƦn�r����kanID_swfName_V4.txt"<<endl;
    cout<<"�Ƶ�2 : �䴩V3���r���ɮ榡�A�зǳƦn�r����kanID_swfName_V3.txt"<<endl;
    cout<<"***********************************************************"<<endl;
    cout<<"V4�r���ɮ榡�d��  V3�r���ɮ榡�d��"<<endl;
    cout<<"1                 1"<<endl;
    cout<<"����              snohitatusbk"<<endl;
    cout<<"snohitatusbk"<<endl;
    cout<<"***********************************************************"<<endl;
    cout<<"�Ҧ����(0|1)"<<endl; 
    cin>>MOD;
    if(MOD == 0) outputMOD0();
    if(MOD == 1) outputMOD1();
    cout<<"�U���s���w���ͧ���"<<endl; 
    system("pause");
}

void outputMOD0(){
    cout<<"��Jĥ�QID : "<<endl;
    cin>>shipid;
    cout<<"��Jĥ�Q�W��(�i����J) : "<<endl;
    cin.ignore();
    getline(cin,shipname);
    cout<<"��Jĥ�Qswf�]�W : "<<endl;
    cin>>shipswf;
    Fileoutput(shipid,shipname,shipswf);
}

void outputMOD1(){
    int txtMOD = 4,m = 3;
    
    cout<<"��ܦr���ɮ榡"<<endl;
    cout<<"V3�п�J : 3"<<endl;
    cout<<"V4�п�J : 4"<<endl;
    cout<<"�Ƶ� : V4����ANSI�榡�r����"<<endl;
    cin>>txtMOD;
    string DictionaryFile = "";
    if(txtMOD == 3){ DictionaryFile = "kanID_swfName_V3.txt"; m = 2; }
    if(txtMOD == 4){ DictionaryFile = "kanID_swfName_V4.txt"; GetTextFileType(DictionaryFile); }
    ifstream idin;
    idin.open(DictionaryFile.c_str(),ios::in);
    string line;
    stringstream ssline;
    int i=0;
    if(idin) //�s�bDictionaryFile���
    { 
        while (getline (idin, line)){ // line�����]�A�C�檺�����  
            if(i%m == 0){
                /* string to int */ 
                ssline << line; //�H��y�B��l�g�Jssline
                ssline >> shipid; //�H��y�B��l�g�Jshipid 
                ssline.clear(); //�M��stringstream�Ŷ� 
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
    else{ //�S���Ӥ��
        cout<<"no such file"<<endl; 
    }
}

void GetTextFileType(string DictionaryFile){
    string fileType = "ANSI";
    int Error = 0;
    ifstream file;
    file.open(DictionaryFile.c_str(), ios::in);
    /* �}�l�P�_���榡 */ 
    if (file.good())
    {
        char szFlag[3] = {0};
        file.read(szFlag, sizeof(char) * 3);
        if ((unsigned char)szFlag[0] == 0xFF && (unsigned char)szFlag[1] == 0xFE){
            fileType = "UTF-16";
            cout <<"������r���ɮ榡��UTF-16�A�бN�r���ɧאּANSI�榡"<<endl;
            Error = 1;
        }
        else if ((unsigned char)szFlag[0] == 0xEF && (unsigned char)szFlag[1] == 0xBB && (unsigned char)szFlag[2] == 0xBF){
            fileType = "UTF-8";
            cout <<"������r���ɮ榡��UTF-8�A�бN�r���ɧאּANSI�榡"<<endl;
            Error = 1;
        }
    }
    else{ //�S���Ӥ��  
        cout<<"no such file"<<endl;
        Error = 1;
    }
    file.close();
    if(Error == 1){
        system("pause");
        exit(1); //void�Ƶ{�������return 
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
    if(shipname != "") Tempname<<shipid<<"_"<<shipname; //�H��y�B��l�X�ּg�JTempname
    else Tempname<<shipid; //�H��y�B��l�g�JTempname
    string FolderName = Tempname.str();
    Tempname<<".txt"; //�H��y�B��l�g�JTempname
    string outputfilename = Tempname.str();
    outputfilenameout.open(outputfilename.c_str(),ios::out);
    for(int i=0;i<53;i++){
        outputfilenameout << "http://203.104.209.102/kcs/sound/kc"<<shipswf<<"/"<<(shipid+7)*17*(a[i+1]-a[i])%99173+100000<<".mp3" << endl; // ��X��txt��
    }
    outputfilenameout.close(); //�]�ɮ׭n���win api����A�ҥH�n�����g�J���A
    cout<<"��X"<<outputfilename<<".txt"<<endl;
    /* �ϥΨ�win api�A�ҥH�nchar* �Φ��Ǧ^string���r�� */
    string CreateFolder = "MD " + FolderName;
    system(CreateFolder.c_str());
    string MoveFile = "Move " + Tempname.str() + " " + FolderName +"/"+ Tempname.str();
    system(MoveFile.c_str());
}
