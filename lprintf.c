#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>

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
                int len=sprintf(buf,"%d\n",val);
                write(1,buf,len);
                break;

            } 
            case 'c':{

                    ch=va_arg(ap,int);
                    int len_c=sprintf(buf,"%d\n",ch);

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
    lprintf("name: %s\n",name);
    return 0;
}
