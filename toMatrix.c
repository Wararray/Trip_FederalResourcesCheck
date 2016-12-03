#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>

#ifndef __STDC_ISO_10646__
#error "Oops, our wide chars are not Unicode codepoints, sorry!"
#endif

int main()
{
    setlocale(LC_ALL, "");
    char matrix[330][50][500];
    // char word[10] = "amor";
    char str[999];
    FILE * file;
    int i = 0;
    int j = 0; 
    int c = 0;
    int COLUNAS = 0;
    int size = 0;
    char arquivo[100] = "201101_Diarias.csv";

    // for (i = 0; i < 0xffff; i++) {
    //             printf("%x - %lc\n", i, i);
    //     }
    //     getchar();

    // matrix[0][0] = word[0];
    // matrix[0][1] = word[1];
    // matrix[0][2] = word[2];
    // matrix[0][3] = word[3];
    // printf("Palavra: %s\n", word);
    // printf("Matrix: %s",matrix[i]);


 // while (i <= 10){
 //        printf("%c",matrix[0][i]);
 //        i++;
 //    }

    file = fopen( arquivo , "r");
    if (file) {
            
        //Jump The First Line
        fscanf(file, "%[\n]s", str);
        do {
            
            fscanf(file, "%[^\t^\n]s", str);
            printf("str:: %s\n",str);
            c = fgetc(file);
            // getchar();
            if ( (c == '\n') || (c == '\r') || (c == '\0')) 
            {
                // printf("GOTCHA!!\n");
                printf("Vou copiar: %s\n",str);
                printf("Tamanho: %d\n", (int)strlen(str));
                while (size < strlen(str)){
                    // printf("V: %c\n",str[size]);
                    matrix[i][j][size] = str[size];
                    printf("M[%d][%d][%d]: %c\n",i,j,size,matrix[i][j][size]);
                    size++;
                }
                matrix[i][j][size] = '\n';
                printf("%s\n", matrix[i][j]);
                printf("---------------BOTEI FIM DE LINHA \n" );
                // getchar();
                size = 0;
                i++;
                COLUNAS = j;
                j = 0;
                
            }
            
            else 
            {
                printf("Vou copiar: %s\n",str);
                printf("Tamanho: %d\n", (int)strlen(str));
                while (size < strlen(str))
                {
                    // printf("V: %c\n",str[size]);
                    matrix[i][j][size] = str[size];
                    // printf("M[%d][%d][%d]: %c\n",i,j,size,matrix[i][j][size]);
                    printf("%x - %lc\n", matrix[i][j][size], matrix[i][j][size]);
                    size++;
                }
                matrix[i][j][size] = '\0';
                printf("%s\n", matrix[i][j]);
                printf("BOTEI FIM DE PALAVRA \n");
                // getchar();
                size = 0;
                j++;
            // getchar();
            }
        }while((c != EOF));
        fclose(file);
    }
    int fim = 0;
    int fim2 = 0;
    int fim3 = 0; 
    COLUNAS = 21;
    while (fim < i)
    {
        while (fim2 < COLUNAS)
        {
            // while(matrix[fim][fim2][fim3] != '\0')
            // { 
            //     // printf("aqui\n");
            //     printf("matrix[%d][%d][%d] = %c", fim, fim2, fim3, matrix[fim][fim2][fim3]);
            //     printf("\n");
            //     fim3++;
            // }
            // printf("\n");
            // fim3 = 0; 
            printf("%s\n", matrix[fim][fim2]);
            
            fim2++;
        }
        fim2 = 0;
        fim++;
        printf("\n");
    }

    return 0;
}





// # import numpy as np
// # arquivo = open('une1.cvs', 'r')
// # texto = arquivo.readlines()
// # for x in texto:
// #     x.split('\t')
// #     b = np.array(x)
// # arquivo.close()

// # print b


// # # print matrix[1][1]