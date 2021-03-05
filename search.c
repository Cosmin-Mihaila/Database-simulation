#include "structures.h"

// Functie pentru a afisa o lista de celule de tp FLOAT
void SEARCH_FLOAT(int numberColumns,t_floatCell* cell)
{	
	int i,j;
	for(i=0;i<numberColumns;i++)
	{
		// Printam valuarea celulei
		printf("%f",(*cell).value);

		int cellValueLength=0;
		int cellValueCopy;

		// Calculam lungimea partii intregi a valorii
		cellValueCopy =(int)(*cell).value;

		while(cellValueCopy)
		{
			cellValueLength++;
			cellValueCopy /= 10;
		}

		// Oferim aliniament
		for(j=0;j<=MAX_COLUMN_NAME_LEN-cellValueLength-8;j++)
		{
			printf(" ");
		}

		cell = (*cell).next;
		
	}
	printf("\n");
}

// Functie pentru a afisa o lista de celule de tp INT
void SEARCH_INT(int numberColumns,t_intCell* cell)
{	
	int i,j;
	for(i=0;i<numberColumns;i++)
	{
		// Printam valuarea celulei
		printf("%d",(*cell).value);

		int cellValueLength=0;
		int cellValueCopy;

		// Calculam lungimea numarului
		cellValueCopy = (*cell).value;

		if(cellValueCopy == 0)
			cellValueLength = 1;

		while(cellValueCopy)
		{
			cellValueLength++;
			cellValueCopy /= 10;
		}

		// Oferim aliniament
		for(j=0;j<=MAX_COLUMN_NAME_LEN-cellValueLength;j++)
		{
			printf(" ");
		}
		cell = (*cell).next;
	}
	printf("\n");
}

// Functie pentru a afisa o lista de celule de tp STRING
void SEARCH_STRING(int numberColumns,t_stringCell* cell)
{	
	int i;
	int j;
	for(i=0;i<numberColumns;i++)
	{
		// Printam valuarea celulei si oferim aliniament
		printf("%s",(*cell).value);
		for(j=0;j<=MAX_COLUMN_NAME_LEN-strlen((*cell).value);j++)
		{
			printf(" ");
		}
		cell = (*cell).next;
	}
	printf("\n");
}

void SEARCH(t_db dataBase, char* tableName, char* columnName, 
			char* op, char* val)
{	
	// Verificam ce relatie a fost introdusa 
	// si ii atribuim o valoarea
	int operation;

	if(strcmp(op, "<") == 0)
		operation =0;
	if(strcmp(op, ">") == 0)
		operation =1;
	if(strcmp(op, "<=") == 0)
		operation =2;
	if(strcmp(op, ">=") == 0)
		operation =3;
	if(strcmp(op, "==") == 0)
		operation =4;
	if(strcmp(op, "!=") == 0)
		operation =5;

	// Verificam daca data de baze este goala
	// (nu are niciun tabel in ea)
	if(dataBase.tables == NULL)
	{
		printf("Table \"%s\" not found in database.\n",tableName);
		return;
	}

	int i,j;
	int numberColumns=0;
	int columnIndex=-1;
	int spaceLeft;
	
	// Parcurgem data de baze pana gasim tabela ceruta
	while(strcmp(dataBase.tables->name, tableName))
	{
		dataBase.tables = dataBase.tables->next;

		// Verificam daca am parcurs toate tabelele
		// si nu am gasit-o pe cea cautata
		if(dataBase.tables == NULL)
		{
			printf("Table \"%s\" not found in database.\n",tableName);
			return;
		}

	}

	t_column* Column;
	Column = dataBase.tables->columns;

	t_column* saveColumn;
	saveColumn = Column;

	// Cautam coloana dorita in tabel
	while(strcmp(Column->name,columnName))
	{
		Column = Column->next;

		// Verificam daca am parcurs toate coloanele din tabel
		// si nu am gasit-o pe cea cautata
		if(Column == NULL)
		{
			printf("Table \"%s\" does not contain column \"%s\".\n",
					tableName,columnName);
			return;
		}
	}
	
	// Afisam numele bazei de date
	printf("TABLE: %s\n",dataBase.tables->name);

	Column = saveColumn;

	// Parcurgem fiecare coloana din baza de date
	while(Column != NULL)
	{	
		numberColumns++;
		printf("%s ",Column->name);

		// Salvam index-ul coloanei cautate
		if(strcmp(Column->name, columnName) == 0)
			columnIndex=numberColumns;

		spaceLeft = MAX_COLUMN_NAME_LEN - strlen(Column->name);

		// Pentru a alinia numele coloanelor
		for(i=0;i<spaceLeft;i++)
		{
			printf(" ");
		}

		Column = Column->next;
	}

	// Adaugam padding-ul
	printf("\n");
	
	for(i=0;i<numberColumns;i++)
	{
		for(j=0;j<MAX_COLUMN_NAME_LEN;j++)
		{
			printf("-");
		}

		printf(" ");
	}

	printf("\n");
	
	// Verificam daca tabelul nu are nici-o linie adaugata
	if(dataBase.tables->lines == NULL)
	{	
		printf("\n");
		return;
	}

	if(dataBase.tables->type == STRING)
	{
		t_stringLine* line;
		line = (t_stringLine*)dataBase.tables->lines;

		t_stringLine* saveLine;
		saveLine=(t_stringLine*)dataBase.tables->lines;

		t_stringCell* saveCell;
		saveCell = (t_stringCell*) line->cells;


		while(line != NULL)
		{	
			t_stringCell* cell;
			cell = (t_stringCell*) line->cells;

			
			t_stringCell* translationCell;
			translationCell = cell;

			// Mutam celula pana la coloana cautata pentru
			// a verifica daca celula corespunde valorii cautate
			for(i=0;i<columnIndex-1;i++)
			{
				translationCell = translationCell->next;
			}

			int check = strcmp(translationCell->value, val);

			// Afisam linia daca valoare celulei este cea cautata
			switch (operation)
			{
				case 0 :
					{
						if(check < 0)
						{	
							SEARCH_STRING(numberColumns,cell);
						}
						break;
					}
				case 1 :
					{
						if(check > 0)
						{	
							SEARCH_STRING(numberColumns,cell);
						}
						break;
					}
				case 2 :
					{
						if(check <= 0)
						{	
							SEARCH_STRING(numberColumns,cell);
						}
						break;
					}
				case 3 :
					{
						if(check >= 0)
						{	
							SEARCH_STRING(numberColumns,cell);
						}
						break;
					}
				case 4 :
					{
						if(check == 0)
						{	
							SEARCH_STRING(numberColumns,cell);
						}
						break;
					}
				case 5 :
					{
						if(check != 0)
						{	
							SEARCH_STRING(numberColumns,cell);
						}
						break;
					}

			}
			
			line=line->next;
		}

		saveLine->cells = saveCell;
		dataBase.tables->lines = saveLine;

	}

	else if(dataBase.tables->type == INT)
	{
		t_intLine* line;
		line = (t_intLine*)dataBase.tables->lines;

		t_intLine* saveLine;
		saveLine=(t_intLine*)dataBase.tables->lines;

		t_intCell* saveCell;
		saveCell = (t_intCell*) line->cells;


		while(line != NULL)
		{	
			t_intCell* cell;
			cell = (t_intCell*) line->cells;

			// Mutam celula pana la coloana cautata pentru
			// a verifica daca celula corespunde valorii cautate
			t_intCell* translationCell;
			translationCell = cell;

			for(i=0;i<columnIndex-1;i++)
			{
				translationCell = translationCell->next;
			}

			// Facem din string int
			int intVal = atoi(val);
			int check = translationCell->value - intVal;

			// Afisam linia daca valoarea celulei corespunde
			// valorii cautate
			switch(operation)
			{
				case 0 :
					{
						if(check < 0)
						{	
							SEARCH_INT(numberColumns,cell);
						}
						break;
					}
				case 1 :
					{
						if(check > 0)
						{	
							SEARCH_INT(numberColumns,cell);
						}
						break;
					}
				case 2 :
					{
						if(check <= 0)
						{	
							
							SEARCH_INT(numberColumns,cell);
							
						}
						break;
					}
				case 3 :
					{
						if(check >= 0)
						{	
							
							SEARCH_INT(numberColumns,cell);
						}
						break;
					}
				case 4 :
					{
						if(check == 0)
						{	
							SEARCH_INT(numberColumns,cell);
						}
						break;
					}
				case 5 :
					{
						if(check != 0)
						{	
							SEARCH_INT(numberColumns,cell);
						}
						break;
					}

			}
			
			line=line->next;
		}

		saveLine->cells = saveCell;
		dataBase.tables->lines = saveLine;

	}

	else if(dataBase.tables->type == FLOAT)
	{
		t_floatLine* line;
		line = (t_floatLine*)dataBase.tables->lines;

		t_floatLine* saveLine;
		saveLine=(t_floatLine*)dataBase.tables->lines;

		t_floatCell* saveCell;
		saveCell = (t_floatCell*) line->cells;

		if(saveLine == NULL)
			return;

		while(line != NULL)
		{	
			// Salvam prima celula a liniei
			t_floatCell* cell;
			cell = (t_floatCell*) line->cells;

			// Mutam celula pana la coloana cautata pentru
			// a verifica daca celula corespunde valorii cautate
			t_floatCell* translationCell;
			translationCell = cell;

			for(i=0;i<columnIndex-1;i++)
			{
				translationCell = translationCell->next;
			}

			// Facem din string int
			float intVal = atof(val);
			float check =translationCell->value - intVal;
			float zero = 0;

			// Afisam linia daca valoarea celulei
			// corespunde valorii cautate
			switch(operation)
			{
				case 0 :
					{
						if(check < zero)
						{	
							SEARCH_FLOAT(numberColumns,cell);
						}
						break;
					}
				case 1 :
					{
						if(check > zero)
						{	
							SEARCH_FLOAT(numberColumns,cell);
						}
						break;
					}
				case 2 :
					{
						if(check <= zero)
						{	
							SEARCH_FLOAT(numberColumns,cell);
						}
						break;
					}
				case 3 :
					{
						if(check >= zero)
						{	
							SEARCH_FLOAT(numberColumns,cell);
						}
						break;
					}
				case 4 :
					{
						if(check == zero)
						{	
							SEARCH_FLOAT(numberColumns,cell);
						}
						break;
					}
				case 5 :
					{
						if(check != zero)
						{	
							SEARCH_FLOAT(numberColumns,cell);
						}
						break;
					}
			}
			
			line=line->next;
		}

		saveLine->cells = saveCell;
		dataBase.tables->lines = saveLine;

	}
	printf("\n");

}