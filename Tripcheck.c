#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <mysql/mysql.h>
#include <stdio.h>

//formating Strings sets---------------------------------------
#ifndef __STDC_ISO_10646__
#error "Oops, our wide chars are not Unicode codepoints, sorry!"
#endif
//--------------------------------------------------------------

void getLineInDatafile(char const *argv[]);
void manageFormatQuery(char* line);
void numConvert_ptBr_eng();
void CreatDatabase();
void CreatBasicTables();
MYSQL* ConectDatabase();
void DisconectDatabase(MYSQL *con);
void finish_with_error(MYSQL *con);
void startBasicQuery();
void SeeAllTables();

const int TRUE  = 0;
const int FALSE = 1;
const char* _DATABASE_ = "TesteDoSucesso";

int main(int argc, char const *argv[])
{
    
    //formating Strings sets
    setlocale(LC_ALL, "");

    // When Run for the first time
    // Use this to control the basic set up of Database
    unsigned int creatDatabase    = TRUE ; // 0 = TRUE | 1 = FALSE
    unsigned int creatBasicTables = TRUE ; // 0 = TRUE | 1 = FALSE
    unsigned int sacanFile        = TRUE ; // 0 = TRUE | 1 = FALSE
    unsigned int interativeMode   = FALSE  ; // 0 = TRUE | 1 = FALSE

    if (creatDatabase == TRUE)
    { 
        CreatDatabase();
    }

    if (creatBasicTables == TRUE)
    { 
        CreatBasicTables();
    }
    
    if (sacanFile == TRUE)
     {
       getLineInDatafile(argv);
     } 

    if (interativeMode == TRUE)
    {
        startBasicQuery();
    }

    return 0;
}

void startBasicQuery()
{
    char choice;
    system("clear");
    printf("- Ask one below:\n");
    printf("(1) Show me all my Tables\n");
    printf("(2) Who has receive more times the benefit?\n");
    printf("(3) Biggest Pay for UnB\n");
    printf("(4) Custom your Query\n");
    scanf("%c",&choice);
    switch(choice){
        case '1':
                SeeAllTables();
            break;
        case '2':
                // SELECT name_favorecido FROM Pagamento GROUP BY Month;
            break;
        case '3':
            break;
        case '4':
            break;
    }
}

void SeeAllTables()
{
    MYSQL_ROW row;
    MYSQL_RES *res;
    MYSQL* con = ConectDatabase();

    /* send SQL query */
    if (mysql_query(con, "show tables")) {
       fprintf(stderr, "%s\n", mysql_error(con));
    }

    res = mysql_use_result(con);

    /* output table name */
    printf("This are all Tables in %s database:\n\n",_DATABASE_);
    while ((row = mysql_fetch_row(res)) != NULL)
    printf("%s \n", row[0]);

    /* close connection */
    mysql_free_result(res);
    DisconectDatabase(con);
    }

MYSQL* ConectDatabase()
{
  MYSQL *con = mysql_init(NULL);
    if (con == NULL) 
    {
      fprintf(stderr, "%s\n", mysql_error(con));
    }

    if  (mysql_real_connect(con, "localhost", "root", "Mysql666!", 
        _DATABASE_, 0, NULL, 0) == NULL) 
    {
       printf("AMO/SOU\n");
        finish_with_error(con);
    }

    // printf("Connection Done.\n");
    return (con);
}

void DisconectDatabase(MYSQL *con){
    mysql_close(con);
}

void CreatDatabase()
{
  MYSQL *con = mysql_init(NULL);

    if (con == NULL) 
    {
      fprintf(stderr, "%s\n", mysql_error(con));
    }

    if  (mysql_real_connect(con, "localhost", "root", "Mysql666!", 
        NULL, 0, NULL, 0) == NULL) 
    {
        finish_with_error(con);
    }
    if (mysql_query(con, "CREATE DATABASE TesteDoSucesso;")) 
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
    }
}

void CreatBasicTables()
{

  MYSQL *con = mysql_init(NULL);

    if (con == NULL) 
    {
      fprintf(stderr, "%s\n", mysql_error(con));
    }

    if  (mysql_real_connect(con, "localhost", "root", "Mysql666!", 
        _DATABASE_, 0, NULL, 0) == NULL) 
    {
        finish_with_error(con);
    }
    if (mysql_query(con, "CREATE TABLE IF NOT EXISTS `OrgSuperior` (`cod_org_sup` INTEGER NOT NULL, `name_org_sup` VARCHAR(300) NOT NULL, PRIMARY KEY(`cod_org_sup`))")) 
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
    }

    if (mysql_query(con, "CREATE TABLE IF NOT EXISTS `OrgSubord` (`cod_org_subord` INTEGER NOT NULL, `name_org_subord` VARCHAR(300) NOT NULL, `cod_org_sup` INTEGER NOT NULL, INDEX fk_codSup(`cod_org_sup`), FOREIGN KEY fk_codSup(`cod_org_sup`) REFERENCES OrgSuperior(`cod_org_sup`), PRIMARY KEY(`cod_org_subord`))")) 
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
    }

    if (mysql_query(con, "CREATE TABLE IF NOT EXISTS `UnddGestora` (`cod_unnd_gest` INTEGER NOT NULL, `name_unidade_gestora` VARCHAR(300) NOT NULL, `cod_org_subord` INTEGER NOT NULL, INDEX fk_codSub(`cod_org_subord`), FOREIGN KEY (`cod_org_subord`) REFERENCES OrgSubord(`cod_org_subord`), PRIMARY KEY(`cod_unnd_gest`))")) 
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
    }

    if (mysql_query(con, "CREATE TABLE IF NOT EXISTS `Funcao` (`cod_funcao` INTEGER NOT NULL, `name_funcao` VARCHAR(300) NOT NULL, `cod_unnd_gest` INTEGER NOT NULL, INDEX fk_codUnddGest(`cod_unnd_gest`), FOREIGN KEY (`cod_unnd_gest`) REFERENCES UnddGestora(`cod_unnd_gest`), PRIMARY KEY(`cod_funcao`) )")) 
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
    }

    if (mysql_query(con, "CREATE TABLE IF NOT EXISTS `Subfuncao` (`cod_subfuncao` INTEGER NOT NULL, `name_subfuncao` VARCHAR(300) NOT NULL, `cod_funcao` INTEGER NOT NULL, INDEX fk_codFuncao(`cod_funcao`), FOREIGN KEY (`cod_funcao`) REFERENCES Funcao(`cod_funcao`), PRIMARY KEY(`cod_subfuncao`) )")) 
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
    }

    if (mysql_query(con, "CREATE TABLE IF NOT EXISTS `Programa` (`cod_prog` INTEGER NOT NULL, `name_prog` VARCHAR(300) NOT NULL, `cod_subfuncao` INTEGER NOT NULL, INDEX fk_subfuncao(`cod_subfuncao`), FOREIGN KEY (`cod_subfuncao`) REFERENCES Subfuncao(`cod_subfuncao`), PRIMARY KEY(`cod_prog`) )")) 
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
    }

    if (mysql_query(con, "CREATE TABLE IF NOT EXISTS `Acao` (`cod_acao` INTEGER NOT NULL, `name_acao` VARCHAR(300) NOT NULL, `linguagem_cidada` VARCHAR(300), `cod_prog` INTEGER NOT NULL, INDEX fk_codProg(`cod_prog`), FOREIGN KEY (`cod_prog`) REFERENCES Programa(`cod_prog`), PRIMARY KEY(`cod_acao`))")) 
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
    }

    if (mysql_query(con, "CREATE TABLE IF NOT EXISTS `Favorecido` (`cpf` VARCHAR(16) NOT NULL, `name_favorecido` VARCHAR(300) NOT NULL, PRIMARY KEY(`cpf`,`name_favorecido`) )")) 
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
    }
    
    if (mysql_query(con, "CREATE TABLE IF NOT EXISTS `Pagamento` (`doc_pag` VARCHAR(12) NOT NULL, `gestao_pag` INTEGER NOT NULL, `data_pag` DATE NOT NULL, `valor_pag` INTEGER NOT NULL, `cpf` VARCHAR(16) NOT NULL, `name_favorecido` VARCHAR(300) NOT NULL, INDEX fk_cpf(`cpf`), INDEX fk_name_favorecido (`name_favorecido`), FOREIGN KEY(`cpf`,`name_favorecido`) REFERENCES Favorecido(`cpf`,`name_favorecido`), PRIMARY KEY (`cpf`,`name_favorecido`,`doc_pag`))")) 
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
    }

    if (mysql_query(con, "CREATE TABLE IF NOT EXISTS `Favorecido_Acao` (`cpf` VARCHAR(16) NOT NULL, `name_favorecido` VARCHAR(300) NOT NULL, `cod_acao` INTEGER NOT NULL, INDEX fk_name_favorecido (`name_favorecido`), INDEX fk_cod_acao (`cod_acao`), FOREIGN KEY(`cpf`,`name_favorecido`) REFERENCES Favorecido(`cpf`,`name_favorecido`), FOREIGN KEY(`cod_acao`) REFERENCES Acao(`cod_acao`), PRIMARY KEY(`cpf`, `name_favorecido`, `cod_acao`))")) 
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
    }
}


void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  // return(1);        
}

void getLineInDatafile(char const *argv[])
{
    //Variables Set
    char    stringLine[1000];
    FILE*   file;
    int c = 0;
    int desespero;

    //To make sure it will stop
    for (desespero = 0; desespero < 1000; desespero++)
    {
        stringLine[desespero] = '\0';
    }


    file = fopen( argv[1] , "r");
    if (file) {
            
        //Jump The First Line
       // fscanf(file, "%[^\n]s", stringLine);
        do {
            fscanf(file, "%[^\n]s", stringLine);
            //printf("\nstringLine:: %s\n", stringLine);
            manageFormatQuery(stringLine);
            c = fgetc(file);
        }while(( c != EOF));
        fclose(file);
    }
}

void manageFormatQuery(char* line)
{
    int   notTab = '\t';
    int   i, linha, coluna;
    char  dataToSaveInDB[30][300];
    //Split by Tabs
   
    i = linha = coluna = 0;
    do{
        notTab = line[i];
        // printf("Letra: %c\n", line[i]);
        // getchar();
        if (notTab != '\t')
        {
            // printf("NotTAB\n");
            dataToSaveInDB[linha][coluna] = line[i];
            coluna++;
            i++;
        }
        else
        {
            // printf("TAB\n");
            dataToSaveInDB[linha][coluna] = '\0';
            i++;
            linha++;
            coluna = 0;
        }
    }while ((notTab != '\0'));

    //To Visualize whats going to be saved
    // int fim = 0;
    // int fim2 = 0;
    // printf("i = %i\n", i);
    // while (fim <= linha)
    // {
    //     while (dataToSaveInDB[fim][fim2] != '\0')
    //     {
    //         printf("%c", dataToSaveInDB[fim][fim2]);
    //         fim2++;
    //     }
    //     fim2 = 0;
    //     fim++;
    //     printf("\n");
    // }

    // Código Órgão Superior       <PK>
    // Nome Órgão Superior
        
    char dataColumns[1024];
    int k = 0;
    char* insert_Text = "INSERT INTO OrgSuperior (`cod_org_sup`, `name_org_sup`) " ;
    char* select_Text = "SELECT * FROM (SELECT '";
    char* end_query   = "') AS tmp WHERE NOT EXISTS ( SELECT cod_org_sup FROM OrgSuperior WHERE cod_org_sup = ";
    strcat(dataColumns,insert_Text);
    strcat(dataColumns,select_Text);
    strcat(dataColumns,dataToSaveInDB[0]);
    strcat(dataColumns,"'AS A1, '");
    strcat(dataColumns,dataToSaveInDB[1]);
    strcat(dataColumns,end_query);
    strcat(dataColumns,"A1");
    strcat(dataColumns, ") LIMIT 1;" );

    MYSQL* con = ConectDatabase();
    if (mysql_query(con, dataColumns))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        printf("Aqui1\n");
        mysql_close(con);
    }
    DisconectDatabase(con);
    
    // Código Órgão Subordinado    <PK>
    // Nome Órgão Subordinado
    // Código Órgão Superior       <PK><FK>
    
    for (k = 0; k < 500; ++k)
    {
        dataColumns[i] = '\0';
    }
   
    
    insert_Text = "INSERT INTO OrgSubord (`cod_org_subord`, `name_org_subord`, `cod_org_sup`) ";
    select_Text = "SELECT * FROM (SELECT '";
    end_query   = "AS C1') AS tmp WHERE NOT EXISTS ( SELECT cod_org_subord FROM OrgSubord WHERE cod_org_subord = ";
   
    // X       printf("0: %s\n", dataToSaveInDB[0]);
    //     printf("2: %s\n", dataToSaveInDB[2]);
    //     printf("3: %s\n", dataToSaveInDB[3]);

        strcpy(dataColumns,insert_Text);
        strcat(dataColumns,select_Text);
        strcat(dataColumns,dataToSaveInDB[2]);
        strcat(dataColumns,"' AS A1, '");        //This stops FK conflicts with PK when same number
        strcat(dataColumns,dataToSaveInDB[3]);
        strcat(dataColumns,"' AS B1, '");
        strcat(dataColumns,dataToSaveInDB[0]);
        strcat(dataColumns,end_query);
        strcat(dataColumns,"A1");
        strcat(dataColumns, ") LIMIT 1;" );

        con = ConectDatabase();
        if (mysql_query(con, dataColumns))
        {
            fprintf(stderr, "%s\n", mysql_error(con));
            mysql_close(con);
        }
        DisconectDatabase(con);

    // Código Unidade Gestora      <PK>
    // Nome Unidade Gestora
    // Código Órgão Subordinado    <PK><FK>

    for (k = 0; k < 500; ++k)
    {
        dataColumns[i] = '\0';
    }
   
    
    insert_Text = "INSERT INTO UnddGestora (cod_unnd_gest, name_unidade_gestora, cod_org_subord) ";
    select_Text = "SELECT * FROM (SELECT '";
    end_query   = "AS C1') AS tmp WHERE NOT EXISTS ( SELECT cod_unnd_gest FROM UnddGestora WHERE cod_unnd_gest = ";
   
        // printf("0: %s\n", dataToSaveInDB[0]);
        // printf("3: %s\n", dataToSaveInDB[2]);

        strcpy(dataColumns,insert_Text);
        strcat(dataColumns,select_Text);
        strcat(dataColumns,dataToSaveInDB[4]);
        strcat(dataColumns,"'AS A1, '");
        strcat(dataColumns,dataToSaveInDB[5]);
        strcat(dataColumns,"'AS B1, '");
        strcat(dataColumns,dataToSaveInDB[2]);
        strcat(dataColumns,end_query);
        strcat(dataColumns,"A1");
        strcat(dataColumns, ") LIMIT 1;" );

        con = ConectDatabase();
        if (mysql_query(con, dataColumns))
        {
            fprintf(stderr, "%s\n", mysql_error(con));
            mysql_close(con);
        }
        DisconectDatabase(con);

    // // Código Função               <PK>
    // // Nome Função
    // // Código Unidade Gestora      <PK><FK>
    for (k = 0; k < 500; ++k)
    {
        dataColumns[i] = '\0';
    }
   
    
    insert_Text = "INSERT INTO Funcao (cod_funcao, name_funcao, cod_unnd_gest) ";
    select_Text = "SELECT * FROM (SELECT '";
    end_query   = "AS C1') AS tmp WHERE NOT EXISTS ( SELECT cod_funcao FROM Funcao WHERE cod_funcao = ";
   
        // printf("0: %s\n", dataToSaveInDB[0]);
        // printf("3: %s\n", dataToSaveInDB[2]);

        strcpy(dataColumns,insert_Text);
        strcat(dataColumns,select_Text);
        strcat(dataColumns,dataToSaveInDB[6]);
        strcat(dataColumns,"'AS A1, '");
        strcat(dataColumns,dataToSaveInDB[7]);
        strcat(dataColumns,"'AS B1, '");
        strcat(dataColumns,dataToSaveInDB[4]);
        strcat(dataColumns,end_query);
        strcat(dataColumns,"A1");
        strcat(dataColumns, ") LIMIT 1;" );

        con = ConectDatabase();
        if (mysql_query(con, dataColumns))
        {
            fprintf(stderr, "%s\n", mysql_error(con));
            mysql_close(con);
        }
        DisconectDatabase(con);
    
    // // Código Subfunção            <PK>
    // // Nome Subfunção
    // // Código Função               <PK><FK>
    
    for (k = 0; k < 500; ++k)
    {
        dataColumns[i] = '\0';
    }
   
    insert_Text = "INSERT INTO Subfuncao (cod_subfuncao, name_subfuncao, cod_funcao) ";
    select_Text = "SELECT * FROM (SELECT '";
    end_query   = "'AS C1) AS tmp WHERE NOT EXISTS ( SELECT cod_subfuncao FROM Subfuncao WHERE cod_subfuncao = ";
   
        // printf("0: %s\n", dataToSaveInDB[0]);
        // printf("3: %s\n", dataToSaveInDB[2]);

        strcpy(dataColumns,insert_Text);
        strcat(dataColumns,select_Text);
        strcat(dataColumns,dataToSaveInDB[8]);
        strcat(dataColumns,"'AS A1, '");
        strcat(dataColumns,dataToSaveInDB[9]);
        strcat(dataColumns,"'AS B1, '");
        strcat(dataColumns,dataToSaveInDB[6]);
        strcat(dataColumns,end_query);
        strcat(dataColumns,"A1");
        strcat(dataColumns, ") LIMIT 1;" );

        con = ConectDatabase();
        if (mysql_query(con, dataColumns))
        {
            fprintf(stderr, "%s\n", mysql_error(con));
            mysql_close(con);
        }
        DisconectDatabase(con);
    


    
    // // Código Programa             <PK>
    // // Nome Programa
    // // Código Subfunção            <PK><FK>
    for (k = 0; k < 500; ++k)
    {
        dataColumns[i] = '\0';
    }
   
    insert_Text = "INSERT INTO Programa (cod_prog, name_prog, cod_subfuncao) ";
    select_Text = "SELECT * FROM (SELECT '";
    end_query   = "AS C1') AS tmp WHERE NOT EXISTS ( SELECT cod_prog FROM Programa WHERE cod_prog = ";
   
    // printf("0: %s\n", dataToSaveInDB[0]);
    // printf("3: %s\n", dataToSaveInDB[2]);

    strcpy(dataColumns,insert_Text);
    strcat(dataColumns,select_Text);
    strcat(dataColumns,dataToSaveInDB[10]);
    strcat(dataColumns,"'AS A1, '");
    strcat(dataColumns,dataToSaveInDB[11]);
    strcat(dataColumns,"'AS B1, '");
    strcat(dataColumns,dataToSaveInDB[8]);
    strcat(dataColumns,end_query);
    strcat(dataColumns,"A1");
    strcat(dataColumns, ") LIMIT 1;" );

    con = ConectDatabase();
    if (mysql_query(con, dataColumns))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
    }
    DisconectDatabase(con);

    //12 // Código Ação                 <PK>
    //13 // Nome Ação
    //14 // Linguagem Cidadã
    //9 // Código Programa             <PK><FK>

    for (k = 0; k < 500; ++k)
    {
        dataColumns[i] = '\0';
    }
   
    insert_Text = "INSERT INTO Acao (cod_acao, name_acao, linguagem_cidada, cod_prog) ";
    select_Text = "SELECT * FROM (SELECT '";
    end_query   = "AS D1') AS tmp WHERE NOT EXISTS ( SELECT cod_acao FROM Acao WHERE cod_acao = ";
  
    int w = 0;
    for(; w < strlen(dataToSaveInDB[14]); w++)
    {
        dataToSaveInDB[14][w] = tolower(dataToSaveInDB[14][w]);
    }
    w = 0;
    for(; w < strlen(dataToSaveInDB[13]); w++)
    {
         dataToSaveInDB[13][w] = tolower(dataToSaveInDB[13][w]);
    }
  if (strcmp(dataToSaveInDB[13],dataToSaveInDB[14])!= 0)
    {

        // printf("10: %s\n", dataToSaveInDB[10]);
        // printf("12: %s\n", dataToSaveInDB[12]);
        // printf("13: %s\n", dataToSaveInDB[13]);
        // printf("14: %s\n", dataToSaveInDB[14]);


        strcpy(dataColumns,insert_Text);
        strcat(dataColumns,select_Text);
        strcat(dataColumns,dataToSaveInDB[12]);
        strcat(dataColumns,"'AS A1, '");
        strcat(dataColumns,dataToSaveInDB[13]);
        strcat(dataColumns,"'AS B1, '");
        strcat(dataColumns,dataToSaveInDB[14]);
        strcat(dataColumns,"'AS C1, '");
        strcat(dataColumns,dataToSaveInDB[10]);
        strcat(dataColumns,end_query);
        strcat(dataColumns,"A1");
        strcat(dataColumns, ") LIMIT 1;" );

        con = ConectDatabase();
        if (mysql_query(con, dataColumns))
        {
            fprintf(stderr, "%s\n", mysql_error(con));
            mysql_close(con);
        }
        DisconectDatabase(con);
    }

    // // CPF Favorecido              <PK>
    // // Nome Favorecido             <PK>
    
    for (k = 0; k < 500; ++k)
    {
        dataColumns[i] = '\0';
    }
   
    insert_Text = "INSERT INTO Favorecido (cpf, name_favorecido) ";
    select_Text = "SELECT * FROM (SELECT '";
    end_query   = "'AS B1) AS tmp WHERE NOT EXISTS ( SELECT name_favorecido FROM Favorecido WHERE name_favorecido = ";
   
        // printf("15: %s\n", dataToSaveInDB[15]);
        // printf("16: %s\n", dataToSaveInDB[16]);

        strcpy(dataColumns,insert_Text);
        strcat(dataColumns,select_Text);
        strcat(dataColumns,dataToSaveInDB[15]);
        strcat(dataColumns,"'AS A1, '");
        strcat(dataColumns,dataToSaveInDB[16]);
        strcat(dataColumns,end_query);
        strcat(dataColumns,"A1");
        strcat(dataColumns, ") LIMIT 1;" );

        con = ConectDatabase();
        if (mysql_query(con, dataColumns))
        {
            fprintf(stderr, "%s\n", mysql_error(con));
            mysql_close(con);
        }
        DisconectDatabase(con);



    // // Documento Pagamento         <PK>
    // // Gestão Pagamento            
    // // Data Pagamento       
    // // Valor Pagamento
    // // CPF Favorecido              <PK><FK>
    // // Nome Favorecido             <PK><FK>

        for (k = 0; k < 500; ++k)
    {
        dataColumns[i] = '\0';
    }
   
    insert_Text = "INSERT INTO Pagamento (doc_pag, gestao_pag, data_pag, valor_pag, cpf, name_favorecido) ";
    select_Text = "SELECT * FROM (SELECT '";
    end_query   = "AS F1') AS tmp WHERE NOT EXISTS ( SELECT doc_pag FROM Pagamento WHERE doc_pag = ";

    printf("17: %s\n", dataToSaveInDB[17]);
    printf("18: %s\n", dataToSaveInDB[18]);
    printf("19: %s\n", dataToSaveInDB[19]);
    printf("20: %s\n", dataToSaveInDB[20]);
    printf("15: %s\n", dataToSaveInDB[15]);
    printf("16: %s\n", dataToSaveInDB[16]);

    strcpy(dataColumns,insert_Text);
    strcat(dataColumns,select_Text);
    strcat(dataColumns,dataToSaveInDB[17]);
    strcat(dataColumns,"'AS A1, '");
    strcat(dataColumns,dataToSaveInDB[18]);
    strcat(dataColumns,"'AS B1, '");
    strcat(dataColumns,dataToSaveInDB[19]);
    strcat(dataColumns,"'AS C1, '");
    strcat(dataColumns,dataToSaveInDB[20]);
    strcat(dataColumns,"'AS D1, '");
    strcat(dataColumns,dataToSaveInDB[15]);
    strcat(dataColumns,"'AS E1, '");
    strcat(dataColumns,dataToSaveInDB[16]);
    strcat(dataColumns,end_query);
    strcat(dataColumns,"A1");
    strcat(dataColumns, ") LIMIT 1;" );

    con = ConectDatabase();
    if (mysql_query(con, dataColumns))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
    }
    DisconectDatabase(con);
    //     dataToSaveInDB[17];
    //     dataToSaveInDB[18];
    //     dataToSaveInDB[19];
    numConvert_ptBr_eng(dataToSaveInDB);        
        // printf("Money: %s\n", dataToSaveInDB[20]);
    //     dataToSaveInDB[25];
    //     dataToSaveInDB[16];

}

void numConvert_ptBr_eng(char dataToSaveInDB[20][300])
{
    int letra   = 0;
    int i       = 0;
    do{
        letra = dataToSaveInDB[20][i];
        if (letra == ',')
        {
            dataToSaveInDB[20][i] = '.';
        }
            i++;
    }while (letra != '\0');
}
