#include <iostream>
#include <fstream>  
#include <cstring>
#include <sstream> 

using namespace std;

/*
1. ID�Mswf�W�٬ҥi�z�L��J�Ӵ���
2. �ϥΤFstringstream�ӦX�֤�r(string) 
*/

int main()
{
    int a[54]={604825,607300,613847,615318,624009,631856,635451,637218,640529,
               643036,652687,658008,662481,669598,675545,685034,687703,696444,
               702593,703894,711191,714166,720579,728970,738675,740918,743009,
               747240,750347,759846,764051,770064,773457,779858,786843,790526,
               799973,803260,808441,816028,825381,827516,832463,837868,843091,
               852548,858315,867580,875771,879698,882759,885564,888837,896168};
    int shipid=0,i=0;
    string shipswf;
    cout<<"��Jshipid"<<endl;
    cin>>shipid;
    cout<<"��Jshipswf�]�W"<<endl;
    cin>>shipswf;
    stringstream Tempname;
    ofstream outputfilenameout;
    Tempname.str("");
    Tempname<<shipid<<".txt";
    string outputfilename = Tempname.str();
    outputfilenameout.open(outputfilename.c_str(),ios::out);
    for(i=0;i<53;i++){
        outputfilenameout << "http://203.104.209.102/kcs/sound/kc"<<shipswf<<"/"<<(shipid+7)*17*(a[i+1]-a[i])%99173+100000<<".mp3" << endl; // ��X��txt��
    }
    system("pause");
}
