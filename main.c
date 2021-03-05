// Pentru a nu avea warning-uri la functia strdup
#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif

#include "structures.h"


t_db INIT_DB (char* name)
{
	t_db dataBase;

	strcpy(dataBase.name, name);
	dataBase.tables = NULL;

	return dataBase;
}

t_column* allocColumn ()
{
	t_column* Column=NULL;

	Column = (t_column*)malloc(sizeof(t_column));
	
	if(Column == NULL)
	{
		printf("ERROR");
		exit(1);
	}

	Column->next = NULL;
	return Column;
}

void CLEAR(t_db* dataBase, char* tableName)
{	
	t_table* table = (*dataBase).tables;

	// Parcurgem tabelele din baza de date pana gasim tabelul cautat
	while(strcmp(table->name, tableName))
	{
		table = table->next;

		// Verificam daca am parcurs toate tabelele si nu am gasit
		// tabelul cautat
		if(table == NULL)
		{
			printf("Table \"%s\" not found in database.\n",tableName);
		}

	}

	//Verificam tipul celulelor tabelului si stergem lista de celule
	if(table->type == STRING)
	{
		t_stringLine* headLine=NULL;
		t_stringLine* saveLine=NULL;
		headLine =(t_stringLine*) table->lines;

		while(headLine!=NULL)
		{
			saveLine=headLine->next;
			deleteStringList(headLine->cells);
			free(headLine);
			headLine = saveLine;
		}
	}

	else if(table->type == INT)
	{	
		t_intLine* headLine=NULL;
		t_intLine* saveLine=NULL;
		headLine =(t_intLine*) table->lines;

		while(headLine!=NULL)
		{
			saveLine=headLine->next;
			deleteIntList(headLine->cells);
			free(headLine);
			headLine = saveLine;
		}
								
	}

	else if(table->type == FLOAT)
	{
		t_floatLine* headLine=NULL;
		t_floatLine* saveLine=NULL;
		headLine =(t_floatLine*) table->lines;

		while(headLine!=NULL)
		{
			saveLine=headLine->next;
			deleteFloatList(headLine->cells);
			free(headLine);
			headLine = saveLine;
		}
	}


	//table->lines = NULL;

}

void CREATE(t_db* dataBase,char* tableName,char* type, char* columnsNames)
{
	t_table* table = NULL;

	table = (t_table*) malloc(sizeof(t_table));
	table->lines = NULL;
	table->columns = NULL;
	table->next = NULL;

	if(table == NULL)
	{
		printf("ERROR");
		return;
	}

	// Daca baza de date este goala, adaugam tabelul ca fiin primul
	if((*dataBase).tables == NULL)
		(*dataBase).tables = table;
	else
	{	
		t_table* checkTable = (*dataBase).tables;
		while(checkTable!=NULL)
		{	
			// Verificam daca tabelul exista deja in baza de date
			if(strcmp(checkTable->name,tableName)==0)
			{
				printf("Table %s already exists.",tableName);
				free(table);
				return;
				
			}
			checkTable = checkTable->next;
			
		}

		// Parcurgem baza de date pana la ultimul tabel
		t_table* currentTable = (*dataBase).tables;
		while(currentTable->next != NULL)
		{
			currentTable = currentTable->next;
		}

		currentTable->next = table;
	}

	// Folosimt strdup pentru a putea folosii strtok
	// pe sirul primit ca parametru
	char* columnsNamesTok;
	columnsNamesTok = strdup(columnsNames);

	char* p;
	p=strtok(columnsNamesTok," ");

	t_column* Column=NULL;
	t_column* saveColumn=NULL;

	int found = 0;
	
	while(p!=NULL)
	{	
		Column = allocColumn();

		// In cazul in care este prima coloana
		if(found == 0)
		{
			table->columns = Column;
			found = 1;
		}

		// In cazul in care mai exista deja coloane
		else
		{
			saveColumn->next= Column;
		}
		strcpy(Column->name, p);
		saveColumn = Column;
		p=strtok(NULL," ");

	}
	
	// Adaugam numele tabelului
	strcpy(table->name, tableName);
	table->lines=NULL;

	// Adaugam tipul de date al tabelului
	if(strcmp(type,"INT")==0)
	{	
		table->type = INT;
	}
	else if(strcmp(type,"STRING")==0)
	{	
		table->type = STRING;
	}
	else if(strcmp(type,"FLOAT")==0)
	{
		table->type = FLOAT;
	}
	else
	{
		printf("Unknown data type: \n%s\n.\n",type);
		free(table);
		free(columnsNamesTok);
		return;
	}

	free(columnsNamesTok);

}

void ADD(t_db* dataBase,char* tableName,char* valsNames)
{
	// Pentru a putea folosii strtok
	char* valsNameTok;
	valsNameTok = strdup(valsNames);

	// Verificam daca baza de date nu are niciun tabel
	if((*dataBase).tables == NULL)
		{
			printf("Table \"%s\" not found in database.\n",tableName);
			free(valsNameTok);
			return;
		}

	char* p;

	t_table* table = (*dataBase).tables;

	// Cautam in baza de date tabelul unde vrem sa adaugam
	while(strcmp(table->name, tableName))
	{
		table = table->next;

		// Verificam daca s-au parcurs toate tabelele din baza de date
		if(table == NULL)
		{
			printf("Table \"%s\" not found in database.\n",tableName);
			free(valsNameTok);
			return;
		}

	}


	// Verificam tipul celulelor
	if(table->type== STRING)
	{	

		// Alocam memorie pentru linie
		t_stringLine* newLine = NULL;
		newLine = (t_stringLine*) malloc(sizeof(t_stringLine));
		newLine->cells=NULL;
		newLine->next=NULL;

		if(newLine == NULL)
		{
			printf("ERROR");
			exit(1);
		}

		t_stringLine*	currentLine;

		if(table->lines == NULL)
			table->lines = (t_stringLine*) newLine;
		else
		{	currentLine = table->lines;
			while(currentLine->next != NULL)
			{	

				currentLine = currentLine->next;
			}
			
			currentLine->next = newLine;
		}

		p=strtok(valsNameTok," ");
		// Luam primul String primit ca parametru
		

		// Alocam memorie pentru celula si numele acesteia
		t_stringCell* cell=NULL;
		cell = (t_stringCell*) malloc (sizeof(t_stringCell));
		cell->value = (char*) malloc (sizeof(char)*MAX_COLUMN_NAME_LEN);
		cell->next = NULL;

		if(cell == NULL)
		{
			printf("ERROR");
			exit(1);
		}
		if(cell->value == NULL)
		{
			printf("ERROR");
			exit(1);
		}

		newLine->cells = cell;

		// Salvam undeva celulal dinainte
		t_stringCell* saveCell;
		saveCell = cell;


		strcpy(cell->value, p);
		p=strtok(NULL," ");
		// Adaugam celulele in continuare
		while(p!=NULL)
		{	
			cell=NULL;
			cell = malloc(sizeof(t_stringCell));
			cell->value = (char*) malloc (sizeof(char)*MAX_COLUMN_NAME_LEN);
			cell->next=NULL;

			if(cell == NULL)
			{
				printf("ERROR");
				exit(1);
			}
			if(cell->value == NULL)
			{
				printf("ERROR");
				exit(1);
			}
			
			strcpy(cell->value, p);

			saveCell->next = cell;
			saveCell = cell;

			p=strtok(NULL," ");
			
			
		}

	}

	else if(table->type == INT)
	{
		int cellVal;

		// Alocam memorie pentru linie
		t_intLine* newLine = NULL;
		newLine = (t_intLine*) malloc(sizeof(t_intLine));
		newLine->cells = NULL;
		newLine->next = NULL;
		if(newLine == NULL)
		{
			printf("ERROR");
			exit(1);
		}

		t_intLine*	currentLine;

		//saveNewLine = newLine;
		if(table->lines == NULL)
			table->lines = (t_intLine*) newLine;
		else
		{	currentLine = table->lines;
			while(currentLine->next != NULL)
			{	

				currentLine = currentLine->next;
			}
			
			currentLine->next = newLine;
		}

		p=strtok(valsNameTok," ");
		cellVal = atoi(p);

		// Alocam memorie pentru celula
		t_intCell* cell = NULL;
		cell = (t_intCell*) malloc (sizeof(t_intCell));
		cell->next=NULL;

		if(cell == NULL)
		{
			printf("ERROR");
			exit(1);
		}

		newLine->cells = cell;

		// Salvam celula dinainte
		t_intCell* saveCell;
		saveCell = cell;

		cell->value = cellVal;
		
		p=strtok(NULL," ");
		// Adaugam celulele in continuare
		while(p!=NULL)
		{	
			cell = NULL;
			cell = (t_intCell*)malloc(sizeof(t_intCell));
			cell->next = NULL;

			if(cell == NULL)
			{
				printf("ERROR");
				exit(1);
			}

			cell->value = atoi(p);
			
			saveCell->next = cell;
			saveCell = cell;
			p=strtok(NULL," ");
			
		}

	}

	else if(table->type == FLOAT)
	{
		

		// Alocam memorie pentru linie
		t_floatLine* newLine = NULL;
		newLine = (t_floatLine*) malloc(sizeof(t_floatLine));
		newLine->cells=NULL;
		newLine->next=NULL;

		if(newLine == NULL)
		{
			printf("ERROR");
			exit(1);
		}

		t_floatLine*	currentLine;

		if(table->lines == NULL)
			table->lines = (t_floatLine*) newLine;
		else
		{	currentLine = table->lines;
			while(currentLine->next != NULL)
			{	

				currentLine = currentLine->next;
			}
			
			currentLine->next = newLine;
		}

		p=strtok(valsNameTok," ");
		
		// Alocam memorie pentru celula si numele acesteia
		t_floatCell* cell = NULL;
		cell = (t_floatCell*) malloc (sizeof(t_floatCell));
		cell->next = NULL;

		if(cell == NULL)
		{
			printf("ERROR");
			exit(1);
		}

		newLine->cells = cell;

		// Salvam celula dinainte
		t_floatCell* saveCell;
		saveCell = cell;

		cell->value = atof(p);
		p=strtok(NULL," ");
		
		// Adaugam celulele in continuare
		while(p!=NULL)
		{	
			cell=NULL;
			cell = malloc(sizeof(t_floatCell));
			cell->next = NULL;

			if(cell == NULL)
			{
				printf("ERROR");
				exit(1);
			}
			
			cell->value = atof(p);
			
			saveCell->next = cell;
			saveCell = cell;
			p=strtok(NULL," ");
			
		}
	}

	free(valsNameTok);
}
int main()
{
	t_db dataBase;
	
	char* c_0 = malloc(100);
	if(c_0 == NULL)
	{
		printf("ERROR");
		exit(0);
	}

	char* dataBaseName = malloc(MAX_DB_NAME_LEN);
	if(dataBaseName == NULL)
	{
		printf("ERROR");
		exit(0);
	}

	char* tableName = (char*)malloc(MAX_TABLE_NAME_LEN);
	if(tableName == NULL)
	{
		printf("ERROR");
		exit(0);
	}

	char* type = (char*)malloc(7);
	if(type == NULL)
	{
		printf("ERROR");
		exit(0);
	}

	char* values = (char*)malloc(MAX_CMD_LEN);
	if(values == NULL)
	{
		printf("ERROR");
		exit(0);
	}

	char* operation = (char*)malloc(3);
	if(operation == NULL)
	{
		printf("ERROR");
		exit(0);
	}

	char* searchValue = (char*)malloc(MAX_TABLE_NAME_LEN);
	if(searchValue == NULL)
	{
		printf("ERROR");
		exit(0);
	}

	char* columnName = (char*)malloc(MAX_TABLE_NAME_LEN);
	if(columnName == NULL)
	{
		printf("ERROR");
		exit(0);
	}

	char* deleteCommand = (char*)malloc(MAX_CMD_LEN);
	if(deleteCommand == NULL)
	{
		printf("ERROR");
		exit(0);
	}

	while(1)
	{
		scanf("%s",c_0);

		if(strcmp(c_0,"INIT_DB") == 0)
		{
			scanf("%s",dataBaseName);
			dataBase = 	INIT_DB(dataBaseName);
			
		}

		else if(strcmp(c_0,"PRINT_DB") == 0)
		{
			PRINT_DB(dataBase);
		}

		else if(strcmp(c_0,"DELETE_DB")==0)
		{	
			// Eliberam toata memoria alocata in main
			free(c_0);
			free(dataBaseName);
			free(tableName);
			free(type);
			free(values);
			free(operation);
			free(searchValue);
			free(columnName);
			free(deleteCommand);

			DELETE_DB(&dataBase);
		}

		else if(strcmp(c_0,"CREATE")==0)
		{
			scanf("%s %s %[^\n]",tableName,type,values);
			CREATE(&dataBase,tableName,type,values);
		}
		else if (strcmp(c_0,"DELETE")==0)
		{
			scanf("%[^\n]",deleteCommand);

			char* p;
			p=strtok(deleteCommand," ");
			strcpy(tableName,p);
			p=strtok(NULL," ");

			// Verificam daca al doilea cuvant este NULL sau nu pentru
			// a sti daca apelam functia DELETE sau DELETE_TABLE
			if(p != NULL)
			{
				strcpy(columnName,p);
				p=strtok(NULL," ");
				strcpy(operation,p);
				p=strtok(NULL," ");
				strcpy(searchValue,p);
				DELETE(&dataBase,tableName,columnName,operation,searchValue);
			}
			else
			{
				DELETE_TABLE(&dataBase,tableName);
			}

		}
		else if(strcmp(c_0,"CLEAR")==0)
		{
			scanf("%s",tableName);
			CLEAR(&dataBase,tableName);
		}
		else if(strcmp(c_0,"ADD")==0)
		{
			scanf("%s %[^\n]",tableName,values);
			ADD(&dataBase,tableName,values);
		}
		else if(strcmp(c_0,"PRINT")==0)
		{
			scanf("%s",tableName);
			PRINT_TABLE(dataBase,tableName);
		}
		else if(strcmp(c_0,"SEARCH")==0)
		{
			scanf("%s %s %s %s",tableName,columnName,operation,searchValue);
			SEARCH(dataBase,tableName,columnName,operation,searchValue);
		}

		else
		{
			printf("Unknown command: \"%s\".",c_0);
		}
	}
}