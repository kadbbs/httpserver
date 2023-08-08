
#include <unistd.h>
#include <stdarg.h>
// int  my_log10(int x);

int my_pow(int x,int y){


    for(int i=1;i<y+1;i++){
        x=x*i;
    }
    return x;
}

int num_ch(char *buff,char *format,int num){

    //num为正数

    
    int i=1;
    int re;
    while(1){
        re=my_pow(10,i);
        if(re>num){
            break;
        }else if(my_pow(10,i)==num){
            i++;
            break;
        }

        i++;
    }

    //暂时只使用%d格式
    if(format[1]=='d'){

        for(int t=0;t<i;t++){

            buff[i-1-t]=num%my_pow(10,t+1)+48;
        }

    }
    return i;
}

int lprintf(const char *format,...) {

    
    va_list ap;
    va_start(ap, format);
    char c=0;
    int i=0;
    int flog=0;
    int val=0;
    char ch;
    char *chs;
    char buf[100];

    while(1){

        c=format[i++];

        if(c=='%'){
            flog=1;
            //printf("%d\n",flog);
            continue;
        }else if (c=='\0')
        {
            break;
        }

        if(flog==1){
            flog=0;
            switch (c)
            {
            case 'd':{ 
                val=va_arg(ap,int);
                //  int len=sprintf(buf,"%d",val);
                 int len=num_ch(buf,"%d",val);
                write(1,buf,len);
                break;

            } 
            case 'c':{

                    ch=va_arg(ap,int);
                    // int len_c=sprintf(buf,"%d",ch);
                    int len_c=num_ch(buf,"%d",ch);
                    write(1,&ch,len_c);
                    break;

            }
            case 's':{
                chs=va_arg(ap,char*);
                while(1){
                    if(*chs=='\0'){
                        break;
                    }
                    write(1,chs,1);
                    chs++;

                }
                break;

        }                                                       
              
        }
        }else{

            write(1,&c,1);
        }
    }
    va_end(ap);
    return 0;
}
int  main(int argc, char const *argv[])
{
    lprintf("Usage: %d\n",2);
    char *name="we";
    lprintf("name: %ssize=%d,%s\n",name,6,"op");
    lprintf("char=%c\n",'A');
    return 0;
}
