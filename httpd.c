#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <error.h>
#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>


enum request_type{
  GET,
  POST
};

enum http_status_code{
  Ok,
  Bad_Request,
  Unauthorized,
  Forbidden,
  Not_Found,
  Internal_Server_Error,
  Server_Unavailable
};


enum http_response_data_type{

  text,
  image

};


void error_die(char * str){
  perror(str);
  exit(-1);
}

int startup(unsigned short * port){

    struct sockaddr_in server_addr;

  int server_socket= socket(AF_INET, SOCK_STREAM, 0);
  if(server_socket<0){
    error_die("server_socket");
  }
  
  //配置server_addr
  // memset(server_addr,0,sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(*port);
  server_addr.sin_addr.s_addr =INADDR_ANY;


  //绑定套接字
  if(bind(server_socket,(struct sockaddr*)&server_addr,sizeof(server_addr))<0){
    error_die("bind");
  }
  
  //端口复用

   int opt = 1;
 if(setsockopt(server_socket, SOL_SOCKET,SO_REUSEADDR, (const void *) &opt, sizeof(opt))){
    perror("setsockopt");
    return -1;
}
  //监听队列
  if(listen(server_socket,5)<0){

    error_die("listen");
  }
    


  return server_socket;
}


//从客户端读取一行
int get_line(int c_socket,char* buff,int size){
  char c=0;
  int i=0;
  while(i<size-1&&c!='\n'){

    int n=read(c_socket,&c,1);
    if(n==-1){
      error_die("read");
    }
    if(c!='\n'){
      buff[i++]=c;
    }

  }
  if(buff[0]=='G'||buff[0]=='g'){
    return GET;
  }else if(buff[0]=='P'||buff[0]=='p'){
    return POST;
  }
}
///发送 http 头文件 
/// 

void header(int c_socket,int status,int d_type){
      char *statu_chars;
      char *data_type;
      switch (status)
      {
      case Ok:{
        statu_chars="20 OK\r\n";
        break;
      }

      case Not_Found:{
        statu_chars="404 Not_Found\r\n";
        break;
      }

      // default:
      //   break;
      }

      switch (d_type)
      {
      case text:{
        data_type="text/html";
        break;
      }
      case image:{
        data_type="image/html";
        break;
      }

      // default:
      //   break;
      }

      char one[100]="HTTP/1.0 ";
      char three[100]="Content-Type: ";
      strcat(one,statu_chars);
      // send(c_socket,"HTTP/1.0 200 OK\r\n",strlen("HTTP/1.0 200 OK\r\n"),0);
      send(c_socket,one,strlen(one),0);

      send(c_socket,"Server:ysHttp/0.1\r\n",strlen("Server:ysHttp/0.1\r\n"),0);
      send(c_socket,"Content-Type: text/html\n",strlen("Content-Type: text/html\n"),0);
      send(c_socket,"\r\n",strlen("\r\n"),0);

}


//send http body

int send_body(int c_socket,int d_type){
  int fd;
  if(d_type==text){
    fd=open("html/hello.html",O_RDWR);
  }

  char buff[1024];
  char c;
  while(1){
    ssize_t size=read(fd,&c,sizeof(c));
    if(size==0){
      
      break;
    }else if(size==-1){
      error_die("send body read");
    }
    if(c<)
    
    // send(c_socket,html1,strlen(html1),0);
    send(c_socket,buff,size,0); 
    memset(buff,0,sizeof(buff));   
  }

   
  close(fd);
  return 0;
}


int file_pre_work(){
          char get_path[100]="./get_request";
          char post_path[100]="./post_request";
          char pid_str[10];
          memset(pid_str,0,sizeof(pid_str));
        sprintf(pid_str,"%d",getpid());
        strcat(get_path,pid_str);
        int fd=open(get_path,O_RDWR|O_CREAT|O_APPEND,0664);
        //每次打开文件清零
        ftruncate(fd,0);
        lseek(fd,0,SEEK_SET);
        return fd;

}

//与客户端服务的线程服务函数
void *client_work(void *client_socket){

  char buffer[1024];

  

  // int fd_get=open(get_path,O_CREAT||O_RDWR);
  // memset(buffer,0,sizeof(buffer));

  int c_socket=*(int*)client_socket;
  //read line data
  int fd;
  int get_flog=0,post_flog=0;
  while(1){
      
      int numchars=get_line(c_socket,buffer,sizeof(buffer));
      if(numchars==GET||get_flog==1){
        
        if(get_flog==0){
          fd=file_pre_work();
          if(fd==-1){

            error_die("fd");
          }

        }
        get_flog=1;
        write(fd,buffer,strlen(buffer));

      }else if (numchars==POST||post_flog==1){
                if(post_flog==0)
        fd=file_pre_work();
        get_flog=1;
        write(fd,buffer,strlen(buffer));
      }
      
      // char *header="HTTP/1.0 200 OK\r\nServer:ysHttp/0.1\r\nContent-Type: text/plain\r\n";
      // write(c_socket,header,strlen(header));
      if(buffer[0]=='\r'){
          close(fd);
          get_flog=0;
          post_flog=0;
          break;
        
      }
      printf("client line data = %s\n",buffer);

      memset(buffer,0,sizeof(buffer));
  }

  // char *html1="<!DOCTYPE html> <html><head><meta charset=\"utf-8\"><title>菜鸟教程(runoob.com)</title></head><body><h1>我的第一个标题</h1><p>我的第一个段落。</p></body></html>";

  header(c_socket,Ok,text);


  send_body(c_socket,text);
  


  close(c_socket);

  while(1){
    // printf("pid=%d\n",getpid());
  };

}


int main (int argc, char *argv[])
{
  int s_socket;
  unsigned short port =8080;
  struct sockaddr_in server_addr;
  struct sockaddr_in clinet_addr;

  int client_socket_len=sizeof(clinet_addr);
  pthread_t pid;
  s_socket=startup(&port);



  printf("httpd start port = %d\n",port);
  
  while(1){
    int *client_socket=malloc(sizeof(int));
    *client_socket=accept(s_socket,(struct sockaddr*)&clinet_addr,&client_socket_len);

    if(*client_socket==-1){
      error_die("client_socket");
    }

    if(pthread_create(&pid,NULL,client_work,(void *)client_socket)!=0){
      error_die("pthread_create");
    }
    // while(1){
    //   printf("mian\n");
    // }
    // pthread_join(pid,NULL);
    pthread_detach(pid);
  
  }
  return 0;
}
