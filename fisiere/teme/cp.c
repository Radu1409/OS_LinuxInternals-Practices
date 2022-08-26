#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include "lab.h"

#define BUFF_SIZE 4096

int main(int argc, char* argv[])
{
    struct stat id_src;
    struct stat id_dest;
    struct stat dir;
    int dest_nou = 0;
    int director = 0;
    int status_src, status_dest;
    int srcFD;
    int destFD;
    int read_file;
    char *buff[BUFF_SIZE];
    int option, nFlag = 0;
    int vFlag = 0;
    int status = 0;

    while((option=getopt(argc,argv,"nv"))!=-1)
        switch(option)
        {
            case 'n':
                nFlag=1;
                break;
            case 'v':
                vFlag = 1;
		break;
            case '?':
                err_msg("Try 'cp --help' for more information.");
                exit(0);
                break;
        }

    char *destinatie = malloc(PATH_MAX);
    /* presupunem intial ca destinatia este un fisier si construim calea catre fisier in destinatie */
    snprintf(destinatie,PATH_MAX,"%s",argv[argc-1]);

    if(argc-optind == 0)
    {
        err_msg("missing file operand\nTry 'cp --help' for more information.");
        exit(1);
    }

    if(argc-optind == 1)
    {
        err_msg("missing destination file operand after '%s'\nTry 'cp --help' for more information.",argv[optind]);
        exit(1);
    }
    if(argc-optind > 2)
    {
        struct stat s;

	if((stat(argv[argc-1], &s)) == 0)
        {
            if(!S_ISDIR(s.st_mode))
            {
                err_msg("target '%s' is not a directory", argv[argc-1]);
                exit(1);
            }
        }
        else
        {
            err_msg("target '%s' is not a directory", argv[argc-1]);
	    exit(1);
        }
    }
    
    //daca nFlag este setat pe 1 se verifica daca fisierul exista
    //in caz afirmativ programul se opreste
    if(nFlag == 1)
        if(access(destinatie,F_OK)==0)
            exit(0);

   
    int cont = 0; 
    for (int i=optind; i < argc-1; i++)
    {
        for(int j=optind; j < i; j++)
        {
            struct stat source;
            struct stat curr;
            if (((stat(argv[j], &curr)) == 0) && ((stat(argv[i], &source)) == 0))
            {
                if((source.st_ino == curr.st_ino) && (source.st_dev == curr.st_dev))
                {
                    if (!S_ISDIR(curr.st_mode) && !S_ISDIR(source.st_mode))
                    {
                        err_msg("warning: source file '%s' specified more than once", argv[i]);
                        cont = 1;
                        continue;
                    }
                }	
            }
        }
        if (cont == 1)
        {
            cont = 0;
            continue;
        }
        struct stat s;	
        if((stat(argv[i], &s)) == 0)
        {
            if(S_ISDIR(s.st_mode))
            {
                err_msg("-r not specified; omitting directory '%s'", argv[i]);
                status = 1;
                continue;
            }
        }
        if((stat(argv[argc-1], &dir)) == 0)
        {
    	    // Verificare daca destinatia este un director
    	    if(S_ISDIR(dir.st_mode))
    	    {
                director = 1;
                /* daca destinatie este director avem doua situatii
                * 1. daca calea este relativa atunci se contruieste calea relativa la directorul curent "." cu numele
                * fisierului curent
                * 2. daca calea este absoluta se pastreaza calea si numele fisierului curent */ 
                if(argv[argc-1][0] != '/')
                    snprintf(destinatie,PATH_MAX,"%s/%s/%s",".",argv[argc-1],argv[i]);
                else
                {
                    int len = strlen(argv[optind]);
                    if(argv[optind][len-1] == '/')
                        argv[optind][len-1] = '\0';

                    char *ptr = strrchr(argv[i], '/');
                    snprintf(destinatie,PATH_MAX,"%s/%s",argv[argc-1],ptr+1);
                }
            }
        }
        if((status_src = stat(argv[i], &id_src)) == -1)
        {
            err_ret("cannot stat '%s'", argv[i]);
            status = 1;
            continue;
        }

        if(vFlag == 1)
            printf("'%s' -> '%s'\n",argv[optind],argv[argc-1]);

        if((srcFD = open(argv[i], O_RDONLY)) == -1)
        {
            err_ret("cannot open '%s' for reading", argv[i]);
            exit(1);
        }
    
        // Verificare existenta fisier
        // In cazul in care nu exista se va crea un fisier nou
        if(access(destinatie,F_OK) == -1)
        {
            // Crearea fisierului destinatie cu permisiunile fisierului sursa
            if((destFD = creat(destinatie, id_src.st_mode)) == -1)
            {
                if(director == 0)
                {
                    err_ret("cannot create regular file '%s'",destinatie);
                    exit(1);
                }
            }
            dest_nou = 1;
        }
        // Obtinerea statusului fisierului destinatie
        if((status_dest = stat(destinatie, &id_dest)) == -1)
        {
            err_ret("cannot stat '%s'",destinatie);
            exit(1);
        }

        // Verificare daca fisierele sunt identice
        // * st_ino pentru a verifica daca au acelasi serial number
        // * st_dev pentru a verifica daca se afla pe acelasi device
        if((id_src.st_ino  == id_dest.st_ino) && (id_src.st_dev == id_dest.st_dev))
        {
            if (director == 0)
            {
                err_msg("`%s` and `%s` are the same file",argv[optind],destinatie);
                exit(1);
            }  
        }
        if(dest_nou == 0)
        {
            // *O_TRUNC pentru a sterge continutul fisierului destinatie
            if((destFD = open(destinatie,O_WRONLY | O_TRUNC)) == -1)
            {
                err_ret("cannot create regular file '%s'", destinatie);
                exit(1);
            }
        }

        // Copierea datelor din fisierul sursa in fisierul destinatie
       
        while((read_file = read(srcFD,buff,BUFF_SIZE)) > 0)
        {
            if(write(destFD,buff,read_file) != read_file)
                err_sys("write error\n");
        }
        if(read_file == -1)
            err_sys("read error\n");  
        close(srcFD);
        close(destFD);	
    }
    free(destinatie);  
    exit(status);
}
