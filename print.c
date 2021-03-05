#include "structures.h"

void PRINT_DB(t_db dataBase)
{	
	int numberColumns=0;
	int spaceLeft;
	int i;
	int j;

	printf("DATABASE: %s\n", dataBase.name);

	// Parcurgem fiecare tabel din baza de date
	while(dataBase.tables != NULL)
	{	
		numberColumns = 0;
		// Afisam numele bazei de date
		printf("\nTABLE: %s\n",dataBase.tables->name);

		t_column* Column;
		Column = dataBase.tables->columns;

		// Parcurgem fiecare coloana din baza de date
		while(Column != NULL)
		{	
			numberColumns++;
			printf("%s ",Column->name);
			
			spaceLeft = MAX_COLUMN_NAME_LEN - strlen(Column->name);

			// Pentru a alinia numele coloanelor
			for(i=0;i<spaceLeft;i++)
			{
				printf(" ");
			}

			Column = Column->next;
			
		}
		

		// Adaugam paddingul acela cu "-" de dupa numele coloanelor
		printf("\n");
		for(i=0;i<numberColumns;i++)
		{
			for(j=0;j<MAX_COLUMN_NAME_LEN;j++)
			{
				printf("-");
			}

			printf(" ");
		}

		// Trecem dupa padding
		printf("\n");

		// Verificam ce tip de date are tabelul
		if(dataBase.tables->type == STRING)
		{	
			t_stringLine* line;
			line = (t_stringLine*)dataBase.tables->lines;

			// Parcurgem fiecare linie a tabelului
			while(line != NULL)
			{	
				t_stringCell* cell;
				cell = (t_stringCell*) line->cells;

				for(i=0;i<numberColumns;i++)
				{
					// Printam valuarea celulei si oferim aliniament
					printf("%s",cell->value);
					for(j=0;j<=MAX_COLUMN_NAME_LEN-strlen(cell->value);j++)
					{
						printf(" ");
					}
					cell = cell->next;
				}
				printf("\n");

				line=line->next;

			}

		}

		else if(dataBase.tables->type == INT)
		{
			t_intLine* line;
			line = (t_intLine*)dataBase.tables->lines;

			// Parcurgem feicare linie a tabelului
			while(line != NULL)
			{	
				t_intCell* cell;
				cell = (t_intCell*) line->cells;

				// Parcurgem fiecare celula
				for(i=0;i<numberColumns;i++)
				{
					// Printam valuarea celulei 
					printf("%d",cell->value);

					int cellValueLength=0;
					int cellValueCopy;

					// Calculam lungimea numarului
					cellValueCopy = cell->value;
					if(cellValueCopy==0)
						cellValueLength++;
					while(cellValueCopy)
					{
						cellValueLength++;
						cellValueCopy /= 10;
					}

					// Oferim aliniament
					for(j=0;j<=(MAX_COLUMN_NAME_LEN-cellValueLength);j++)
					{
						printf(" ");
					}
					
					cell = cell->next;
				}
				printf("\n");

				line=line->next;
			}
		}

		else if(dataBase.tables->type == FLOAT)
		{
			t_floatLine* line;
			line = (t_floatLine*)dataBase.tables->lines;

			// Parcurgem feicare linie a tabelului
			while(line != NULL)
			{	
				t_floatCell* cell;
				cell = (t_floatCell*) line->cells;


				for(i=0;i<numberColumns;i++)
				{
					// Printam valuarea celulei
					printf("%.6f",cell->value);

					// Calculam lungimea celulei
					int cellValueLength=0;
					int cellValueCopy;

					cellValueCopy = (int)cell->value;
					while(cellValueCopy)
					{
						cellValueLength++;
						cellValueCopy /= 10;
					}

					// Oferim aliniament
					for(j=0;j<=(MAX_COLUMN_NAME_LEN-cellValueLength-8);j++)
					{
						printf(" ");
					}
					cell = cell->next;
				}
				printf("\n");

				line=line->next;
			}
		}
		
		// Trecem la tabelul urmator
		dataBase.tables = dataBase.tables->next;

	}
	printf("\n");
}


void PRINT_TABLE(t_db dataBase, char* tableName)
{
	int numberColumns = 0;
	int i;
	int j;
	int spaceLeft;

	t_table* saveTable = dataBase.tables;

	// Verificam daca baza de date este goala
	if(dataBase.tables == NULL)
		{
			printf("Table \"%s\" not found in database.\n",tableName);
			return;
			
		}

	// Cautam tabelul in baza de date
	while(strcmp(dataBase.tables->name, tableName))
	{
		dataBase.tables = dataBase.tables->next;
		if(dataBase.tables == NULL)
		{
			printf("Table \"%s\" not found in database.\n",tableName);
			return;
		}

	}
	printf("TABLE: %s\n",dataBase.tables->name);

	t_column* Column;
	Column = dataBase.tables->columns;

	// Parcurgem fiecare coloana din baza de date
	while(Column != NULL)
	{	
		numberColumns++;
		printf("%s ",Column->name);

		// Acest artificu este pentru a evita linii mai mari
		// de 80 de caractere
			
		spaceLeft = MAX_COLUMN_NAME_LEN - strlen(Column->name);

		// Pentru a alinia numele coloanelor
		for(i=0;i<spaceLeft;i++)
		{
			printf(" ");
		}

		Column = Column->next;
				
	}
		
	// Adaugam paddingul acela cu "-" de dupa numele coloanelor
	printf("\n");
	for(i=0;i<numberColumns;i++)
	{
		for(j=0;j<MAX_COLUMN_NAME_LEN;j++)
		{
			printf("-");
		}

		printf(" ");
	}

	// Trecem dupa padding
	printf("\n");

	if(dataBase.tables->type == STRING)
	{	
		
		t_stringLine* line;
		line = (t_stringLine*)dataBase.tables->lines;

		// Parcurgem feicare linie a tabelului
		while(line != NULL)
		{	
			t_stringCell* cell;
			cell = (t_stringCell*) line->cells;

			for(i=0;i<numberColumns;i++)
			{
				// Printam valuarea celulei si oferim aliniament
				printf("%s",cell->value);
				for(j=0;j<=MAX_COLUMN_NAME_LEN-strlen(cell->value);j++)
				{
					printf(" ");
				}
				cell = cell->next;
			}
			
			printf("\n");

			line=line->next;
		}
	}

	else if(dataBase.tables->type == INT)
	{
		t_intLine* line;
		line = (t_intLine*)dataBase.tables->lines;

		// Parcurgem feicare linie a tabelului
		while(line != NULL)
		{	
			t_intCell* cell = NULL;
			//cell->next = NULL;
			cell = (t_intCell*) line->cells;

			for(i=0;i<numberColumns;i++)
			{
				// Printam valuarea celulei si oferim aliniament
				printf("%d",cell->value);

				int cellValueLength=0;
				int cellValueCopy;

				// Calculam lungimea celulei
				cellValueCopy = cell->value;
				if(cellValueCopy==0)
					cellValueLength++;
				while(cellValueCopy)
				{
					cellValueLength++;
					cellValueCopy /= 10;
				}
				
				// Oferim aliniament
				for(j=0;j<=(MAX_COLUMN_NAME_LEN-cellValueLength);j++)
				{
					printf(" ");
				}
				

				cell = cell->next;
			}
			//if(line->next)
				printf("\n");

			line=line->next;
		}
	}

	else if(dataBase.tables->type == FLOAT)
	{
		t_floatLine* line;
		line = (t_floatLine*)dataBase.tables->lines;

		// Parcurgem feicare linie a tabelului
		while(line != NULL)
		{	
			t_floatCell* cell;
			cell = (t_floatCell*) line->cells;

			// Parcurgem fiecare coloana
			for(i=0;i<numberColumns;i++)
			{
				// Printam valuarea celulei si oferim aliniament
				printf("%f",cell->value);

				int cellValueLength=0;
				int cellValueCopy;

				// Calculam lungimea celulei
				cellValueCopy = (int)cell->value;
				while(cellValueCopy)
				{
					cellValueLength++;
					cellValueCopy /= 10;
				}

				// Oferim aliniament
				for(j=0;j<=(MAX_COLUMN_NAME_LEN-cellValueLength-8);j++)
				{
					printf(" ");
				}
				cell = cell->next;
			}
			
			printf("\n");

			line=line->next;
		}
	}

	dataBase.tables = saveTable;
	printf("\n");
}