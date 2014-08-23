#include <winsock2.h>
#include <iostream>
#include <vector>
#include <string>

#define IPADDRESS "127.0.0.1"
#define PORT 60070

using namespace std;

SOCKET m_socket;
//feitas por nos


string toString(char *ans){
	string str="";
	int i=0;
	while(ans[i]!=NULL){
		str+=ans[i];
		i++;
	}
	return str;
}
vector<vector<int> > toVecVector(string answer){
	int i=0;
	int numberOfPar=0;
	vector<vector<int> > result;

	if(answer[i]=='['){
		i++;
		numberOfPar++;
	}
	else
		return result;

	vector<int> line;
	while(i<answer.size()){
		if(numberOfPar==0)
			break;
		if(answer[i]=='['){
			numberOfPar++;
		}
		else if(answer[i]==']'){
			if(numberOfPar>1) {
			result.push_back(line);
			vector<int> clean;
			line=clean;
			}
			numberOfPar--;
			
		}
		else if(answer[i]==','){
		}
		else{
			int num = answer[i]-48; 
			line.push_back(num);
		}
		i++;
	}
	return result;
}
string toList(vector<vector<int> > _tab){
	string list="[";
	int j=0,k=0;
	cout<<"List:"<<endl;
	for(j=0;j<_tab.size();j++){
		list+="[";
		for(k=0;k<_tab[j].size();k++){
			char n=_tab[j][k]+48;
			list+=n;
			if(k!=_tab[j].size()-1)
				list+=",";
		}
		if(j!=_tab.size()-1)
			list+="],";
		else
			list+="]";

	}
	list+="]";
	return list;
}

//ja estavam
bool socketConnect() {// Initialize Winsock.
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != NO_ERROR)
		printf("Client: Error at WSAStartup().\n");
    else
        printf("Client: WSAStartup() is OK.\n");

	// Create a socket.
    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_socket == INVALID_SOCKET) {
        printf("Client: socket() - Error at socket(): %ld\n", WSAGetLastError());
        WSACleanup();
        return false;
    }
	else
       printf("Client: socket() is OK.\n");

    // Connect to a server.
    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    // Just test using the localhost, you can try other IP address
    clientService.sin_addr.s_addr = inet_addr("127.0.0.1");
    clientService.sin_port = htons(60070);

    if (connect(m_socket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
        printf("Client: connect() - Failed to connect.\n");
        WSACleanup();
        return false;
    }
    else {
       printf("Client: connect() is OK.\n");
       printf("Client: Can start sending and receiving data...\n");
    }

    // Send and receive data.
	printf("Connected\n");
	return true;
}

void envia(char *s, int len) {
	int bytesSent = send(m_socket, s, len, 0);
	if(bytesSent == SOCKET_ERROR)
		printf("Client: send() error %ld.\n", WSAGetLastError());
}
void recebe(char *ans) {
	int bytesRecv = SOCKET_ERROR;
	int pos = 0;
	while (true) {
		recv(m_socket, &ans[pos], 1, 0);
		if (ans[pos] == '\n')
			break;
		pos++;
	}
	ans[pos] = 0;
	int i=0;
	string answer=toString(ans);
	vector<vector<int> >new_tab ;
	if(answer.size()>3){
		new_tab = toVecVector(answer);
		int j=0,k=0;
		cout<<"Vector:"<<endl;
		for(j=0;j<new_tab.size();j++){
			cout<<"nova"<<endl;
			for(k=0;k<new_tab[j].size();k++){
				cout<<new_tab[j][k]<<"-";
			}
			cout<<"fim"<<endl;
			cout<<endl;
		}
	string answer2=toList(new_tab);
	cout << "Mylist answered: " << answer2 << endl;
	cout << "prolog answered: " << answer << endl;
	}
	else cout << "prolog answered: " << answer << endl;
}

void quit() {
	cout << "Asking prolog to quit" << endl;
	char buff[] = "quit.\n";
	envia(buff, 6);
	system("pause");
}

int main() {
	socketConnect();
	string s="";
	char s1[254];
	while(s1!="quit.")
	{
		cin>>s1;
		strcat(s1,"\n");
		envia(s1, strlen(s1));
		char ans[128];
		recebe(ans);
	}
	quit();
	getchar();
	return 0;
}

