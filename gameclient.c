#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include<stdbool.h>
#define PORT 8080
#define SA struct sockaddr
char matrix[3][3];
void initial(){
   for(int i=0;i<3;i++){
      for(int j=0;j<3;j++){
          matrix[i][j]='_';
      }
   }
}
bool is_valid(int a,int b){
   if(a<1||a>3||b<1||b>3) return false;
   else return true;

}
void read_from_socket(int sockfd){
     char string[1000];
     bzero(string,sizeof(string));
     read(sockfd,string,sizeof(string));
     printf("%s\n",string);

}
char* read_new(int sockfd){
    char *id;
    id=(char*)malloc(1000*sizeof(id));
    bzero(id,1000);
    read(sockfd,id,1000);
    return id;
}
int read1(int sockfd){
     char string[1000];
     bzero(string,sizeof(string));
     read(sockfd,string,sizeof(string));
     int a=string[0]-'0';
     return a;
}
void read_from_socket_mat(int sockfd){
     char string[1000];
     //string=(char*)malloc(1000*sizeof(char));
     bzero(string,sizeof(string));
     read(sockfd,string,sizeof(string));
     //printf("%s\n",string);
     for(int i=0;i<3;i++){
        printf("%c|%c|%c|\n",string[i*3+0],string[i*3+1],string[i*3+2]);
     }
     for(int i=0;i<3;i++){
         for(int j=0;j<3;j++){
             matrix[i][j]=string[i*3+j];
         }
     }
}

void write_to_server(int length,char *message,int sockfd){
    char buff[length];
    bzero(buff,length);
    strcpy(buff,message);
    write(sockfd,buff,length);
}

void game(int sockfd){
    initial();
    char id[1000];
	     bzero(id,sizeof(id));
	     read(sockfd,id,sizeof(id));
	if(id[0]=='1'){
	char string[1000],string2[1000];
	     read_from_socket(sockfd);
             read_from_socket(sockfd); 
	      for(int i=0;i<3;i++){
                 printf("__|__|__\n");
             }   
             for(;;){
                  
                  int a,b;
                   printf("Enter (ROW, COL) for placing your mark:");
                   for(;;){
                   scanf("%d %d",&a,&b);
                      if(!is_valid(a,b)){
                          printf("not valid please enter again:");
                      }
                      else if(matrix[a-1][b-1]=='_'){
                          break;
                      }
                      else{
                          printf("the cell is filled please enter again:");
                      }
                   }
                   char input[200] ={'0' + a,',','0' + b};
                   write_to_server(200,input,sockfd);
                   read_from_socket_mat(sockfd);
                   int r=read1(sockfd);
                   if(r==1){
                       printf("client_1 wins\n");
                       break;
                   }
                   if(r==2){
                       printf("client_2 wins\n");
                       break;
                   }
                   if(r==0){
                       printf("draw\n");
                       break;
                   }
                   read_from_socket_mat(sockfd);
                    int s=read1(sockfd);
                   if(s==1){
                       printf("client_1 wins\n");
                       break;
                   }
                   if(s==2){
                       printf("client_2 wins\n");
                       break;
                   }
                   if(s==0){
                       printf("draw\n");
                       break;
                   }
                   
             }   
	}
	if(id[0]=='2'){
	     // char *msg=read_new(sockfd);
	      //if(msg[0]=='1'){
	         //  printf("your partner got disconnected");
	      //}
	      read_from_socket(sockfd); 
	      for(int i=0;i<3;i++){
                 printf("__|__|__\n");
             }
                for(;;){
                   read_from_socket_mat(sockfd);
                   int r=read1(sockfd);
                   if(r==1){
                       printf("client_1 wins\n");
                       break;
                   }
                   if(r==2){
                       printf("client_2 wins\n");
                       break;
                   }
                   if(r==0){
                       printf("draw\n");
                       break;
                   }
                   int a,b;
                   printf("Enter (ROW, COL) for placing your mark:");
                   for(;;){
                   scanf("%d %d",&a,&b);
                      if(!is_valid(a,b)){
                          printf("not valid please enter again:");
                      }
                      else if(matrix[a-1][b-1]=='_'){
                          break;
                      }
                      else{
                          printf("the cell is filled please enter again:");
                      }
                   }
                   char input2[200] ={'0' + a,',','0' + b};
                   write_to_server(200,input2,sockfd);
                   read_from_socket_mat(sockfd);
                    int s=read1(sockfd);
                   if(s==1){
                       printf("client_1 wins\n");
                       break;
                   }
                   if(s==2){
                       printf("client_2 wins\n");
                       break;
                   }
                   if(s==0){
                       printf("draw\n");
                       break;
                   }
                
                }
	
	}

}
int main(){
        
	int myid ; 
	// recieve client from server
	// client = 0 
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

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
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("connection with the server failed...\n");
		exit(0);
	}
	
	for(;;){
	   game(sockfd);
	    char *t;
	    t=(char*)malloc(1000*sizeof(char));
	    printf("If you want to re-play enter y: ");
	    scanf("%s",t);
	    write_to_server(1000,t,sockfd);
	    char *p;
	    p=(char*)malloc(1000*sizeof(char));
	    bzero(p,1000);
	    read(sockfd,p,1000);
	    if(p[0]=='1'){
	       break;
	    }       
	}
	close(sockfd);
}

