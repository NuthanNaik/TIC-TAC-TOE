#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#define PORT 8080
#define SA struct sockaddr

void func(int connfd){
	char buff[1000];
	int n;
	// infinite loop for chat
	for (;;) {
		bzero(buff, 1000);

		// read the message from client and copy it in buffer
		read(connfd, buff, sizeof(buff));
		// print buffer which contains the client contents
		printf("From client: %s\t To client : ", buff);
		bzero(buff, 1000);
		n = 0;
		// copy server message in the buffer
		while ((buff[n++] = getchar()) != '\n')
			;

		// and send that buffer to client
		write(connfd, buff, sizeof(buff));

		// if msg contains "Exit" then server exit and chat ended.
		if (strncmp("exit", buff, 4) == 0) {
			printf("Server Exit...\n");
			break;
		}
	}
}
char mat[3][3];
void initialise(){
for(int i=0;i<3;i++){
  for(int j=0;j<3;j++){
       mat[i][j]='_';
   }
}
}
void update(int i,int j,char c){
    mat[i][j]=c;
}
char * convert(){
    char * string;
    string =(char*) malloc(sizeof(char)*100);
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
             string[i*3+j]=mat[i][j];
        }
    }
    return string;
}
void write_to_client(int length,char *message,int connfd){
    char buff[length];
    bzero(buff,length);
    strcpy(buff,message);
    //printf("hi");
    write(connfd,buff,length);
}
void read_from_client(int array[],int connfd){
     char string[1000];
     bzero(string,sizeof(string));
     read(connfd,string,sizeof(string));
     int arr[2];
     arr[0]=string[0]-'0'-1;
     arr[1]=string[2]-'0'-1;
    // printf("%s",string);
    // return arr;
}

char* read_new(int connfd){
    char *id;
    id=(char*)malloc(1000*sizeof(char));
    bzero(id,1000);
    read(connfd,id,1000);
    return id;
}
int who_wins(){
    for(int i=0;i<3;i++){
        if(mat[i][0]==mat[i][1]&&mat[i][1]==mat[i][2]&&mat[i][0]=='O') {
              return 1;
        }
    }
     for(int i=0;i<3;i++){
        if(mat[i][0]==mat[i][1]&&mat[i][1]==mat[i][2]&&mat[i][0]=='X') {
              return 2;
        }
    }
     for(int i=0;i<3;i++){
        if(mat[0][i]==mat[1][i]&&mat[1][i]==mat[2][i]&&mat[0][i]=='O') {
              return 1;
        }
    }
    for(int i=0;i<3;i++){
        if(mat[0][i]==mat[1][i]&&mat[1][i]==mat[2][i]&&mat[0][i]=='X') {
              return 2;
        }
    }
    if(mat[0][0]==mat[1][1]&&mat[1][1]==mat[2][2]&&mat[0][0]=='O'){
         return 1;
    }
    if(mat[0][0]==mat[1][1]&&mat[1][1]==mat[2][2]&&mat[0][0]=='X'){
         return 2;
    }
    if(mat[0][2]==mat[1][1]&&mat[1][1]==mat[2][0]&&mat[1][1]=='O'){
            return 1;
    }
    if(mat[0][2]==mat[1][1]&&mat[1][1]==mat[2][0]&&mat[1][1]=='X'){
            return 2;
    }
    for(int i=0;i<3;i++){
       for(int j=0;j<3;j++){
           if(mat[i][j]=='_') return -1;
       }
    }
    return 0;
}
void playgame(int connfd1,int connfd2,FILE* fp){
   for(;;){
       int arr[2],arr2[2];
       //read_from_client(arr,connfd1);
       char string[1000];
	bzero(string,sizeof(string));
	read(connfd1,string,sizeof(string));
	    
	
	     arr[0]=string[0]-'0'-1;
	     arr[1]=string[2]-'0'-1;
	     fprintf(fp,"Move by client1: [%d,%d]\n ",arr[0],arr[1]);
	     
       update(arr[0],arr[1],'O');
       char *update_1=convert(); 
       write_to_client(1000,update_1,connfd1);
       write_to_client(1000,update_1,connfd2);
       int r=who_wins();
       char msg_1[1000];
       sprintf(msg_1,"%d",r);
       write_to_client(1000,msg_1,connfd1);
       write_to_client(1000,msg_1,connfd2);
       if(r==1) {
           printf("client_1 wins\n");
           fprintf(fp,"client_1 wins\n");
           break;
       }
       if(r==2){
           printf("client_2 wins\n");
            fprintf(fp,"client_2 wins\n");
          break;
       }
       if(r==0){
           printf("draw\n");
            fprintf(fp,"draw\n");
          break;
       }
       //read_from_client(arr2,connfd2);
       char string2[1000];
	bzero(string2,sizeof(string2));
	read(connfd2,string2,sizeof(string2));
	
	     arr2[0]=string2[0]-'0'-1;
	     arr2[1]=string2[2]-'0'-1;
	     
       fprintf(fp,"Move by client2: [%d,%d]\n ",arr2[0],arr2[1]);
       update(arr2[0],arr2[1],'X');
       char *update_2 =convert();
       write_to_client(1000,update_2,connfd1);
       write_to_client(1000,update_2,connfd2);
       int s=who_wins();
       char msg_2[1000];
        sprintf(msg_1,"%d",s);
       write_to_client(1000,msg_2,connfd1);
       write_to_client(1000,msg_2,connfd2);
       if(s==1) {
           printf("client_1 wins\n");
           fprintf(fp,"client_1 wins\n");
           break;
       }
       if(s==2){
           printf("client_2 wins\n");
           fprintf(fp,"client_2 wins\n");
          break;
       }
       if(s==0){
          printf("draw\n");
          fprintf(fp,"draw\n");
          break;
       }
   }

}
int main(){
	int sockfd,len;
	//int client_id=1;
	int connfd1,connfd2;
	struct sockaddr_in servaddr,client_1,client_2;
	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		printf("socket bind failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully binded..\n");

	// Now server is ready to listen and verification
	if ((listen(sockfd, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	}
	printf("Game server started. Waiting for players.\n");
	//else
		//printf("Server listening..\n");
	int length_1,length_2;
	length_1 = sizeof(client_1);
	length_2 = sizeof(client_2);

	// Accept the data packet from client and verification
	//bool win =false;
	//for(;;){
	   // char msg[1000] =" Enter (ROW, COL) for placing your mark:";
	   // write_to_client(1000,msg,connfd1);
           	
	
	
	
	//}
	
	connfd1 = accept(sockfd, (SA*)&client_1, &length_1);
	if (connfd1< 0) {
		printf("server accept failed...\n");
		exit(0);
	}
	   
	   //char s[1000]=convert_mat_to_string(mat);
	   //write_to_client(1000,s,connfd1);
	   
	   
	   
	
	connfd2 = accept(sockfd, (SA*)&client_2, &length_2);
	if (connfd2< 0) {
		printf("server accept failed...\n");
		exit(0);
	}
	
	
	FILE *fp ;
fp=fopen("game1.log","w");
if(fp<0){
	   printf("cannot open");
	   exit(1);
	}
	for(;;){
	   initialise();
	   char id1[1000] ="1";
	   write_to_client(1000,id1,connfd1);
	   char message[1000]="Connected to the game server. Your player ID is 1.\nWaiting for a partner to join . . .\n";
	   write_to_client(1000,message,connfd1);
	   char id2[1000] ="2";
	   write_to_client(1000,id2,connfd2);
	   char mess[1000]="Connected to the game server. Your player ID is 2.\nYour partner’s ID is 1. Your symbol is ‘X’\nStarting the game";
	   write_to_client(1000,mess,connfd2);
	   //char s[1000]=convert_mat_to_string(mat);
	  // write_to_client(1000,s,connfd2);
	  char message2[1000] ="Your partner's ID is 2. Your symbol is ‘O’.\nStarting the game …";
	  write_to_client(1000,message2,connfd1);
	  fprintf(fp,"Game begins\n");
	  time_t st,ed;
	  double t;
	  time(&st);
	  playgame(connfd1,connfd2,fp);
	  time(&ed);
	  t=difftime(ed,st);
	  fprintf(fp,"execution of game is: %f milli secs.\n",t);
	  fprintf(fp,"Game ends\n");
                char *s1;
                s1=(char*)malloc(1000*sizeof(char));
                 bzero(s1,1000);
                read(connfd1,s1,1000);
	        char *s;
                s=(char*)malloc(1000*sizeof(char));
	        bzero(s,1000);
	       char *s2;
                s2=(char*)malloc(1000*sizeof(char));
	        bzero(s2,1000);
	       read(connfd2,s2,1000);
	        s[0]='0';
	       if(s1[0]=='n'||s2[0]=='n'){
	           s[0]='1';
	       }
	       write(connfd1,s,1000);
	       write(connfd2,s,1000);
	       if(s1[0]=='n'||s2[0]=='n'){
	           break;
	       }
	}	

	// Function for chatting between client and server
	

	// After chatting close the socket
	close(sockfd);
}
