#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

static void search_dir(char* path, char* target)
{
    int fd = open(path,0);
    struct dirent dt;
    while(read(fd,&dt,sizeof(dt)) == sizeof(dt))
    {
#if 0
        printf("circl...\n");
#endif
        // inode号为0代表文件无效
        if(dt.inum == 0)
            continue;

        if(!strcmp(dt.name,".") || !strcmp(dt.name,".."))
            continue;

        char buf[64] = {0};
        memcpy(buf,path,strlen(path));
        char* p = buf;
        p = p + strlen(path);
        *p++ = '/';
        memcpy(p,dt.name,strlen(dt.name));

        struct stat st;
        stat(buf,&st);
        if(st.type == T_FILE)
        {
            if(!strcmp(dt.name,target))
                printf("%s\n",buf);
        }
        else if(st.type == T_DIR)
        {
            search_dir(buf,target);
        }
    }

    close(fd);
}

int main(int argc,char* argv[])
{
    if(3 != argc)
    {
        fprintf(2,"Invalid parameters\n");
        exit(1);
    }

    // 判断第一个参数是否是目录类型
    char* path = argv[1];
    struct stat st;
    stat(path, &st);
    if(st.type != T_DIR)
    {
        fprintf(2,"param one is not a directory\n");
        exit(1);
    }

    search_dir(path,argv[2]);

    exit(0);
}