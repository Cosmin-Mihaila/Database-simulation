#include "structures.h"


// Functie pentru a sterge o lista cu celule de tip STRING
void deleteStringList(t_stringCell* cells)
{
	t_stringCell* head;
	t_stringCell* save;
	head = cells;

	while(head != NULL)
	{	
		// Salvam celula urmatoare pentru a o putea accesa
		// dupa eliberarea memoriei celulei curente
		save = head->next;

		// Eliberm memoria ocupata de sir
		free(head->value);

		free(head);
		head = save;
	}
}

// Functie pentru a sterge o lista de celule de tip INT
void deleteIntList(t_intCell* cells)
{	
	t_intCell* head=NULL;
	t_intCell* save=NULL;
	head = cells;

	while(head != NULL)
	{	
		// Salvam celula urmatoare pentru a o putea accesa
		// dupa eliberarea memoriei celulei curente
		save = head->next;
		free(head);
		head = save;
	}
	

}

// Functie pentru a sterge lista de coloane a unui tabel
void deleteColumns(t_column* columns)
{
	t_column* head;
	t_column* save;
	head = columns;

	while(head != NULL)
	{	
		// Salvam celula urmatoare pentru a o putea accesa
		// dupa eliberarea memoriei celulei curente
		save = head->next;
		free(head);
		head = save;
	}

}

// Functie pentru a sterge o lista de celule de tip FLOAT
void deleteFloatList(t_floatCell* cells)
{	
	t_floatCell* head;
	t_floatCell* save;
	head = cells;

	while(head != NULL)
	{	
		// Salvam celula urmatoare pentru a o putea accesa
		// dupa eliberarea memoriei celulei curente
		save = head->next;
		free(head);
		head = save;
	}
}

void DELETE(t_db* dataBase,char* tableName,char* columnName, char* op,
			char* val)
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

	int i;
	int numberColumns=0;
	int columnIndex=-1;

	// Verificam daca data de baze este goala
	// (nu are niciun tabel in ea)
	if(dataBase->tables == NULL)
		{
			printf("Table \"%s\" not found in database.\n",tableName);
			return;
		}
	
	// Salvam primul tabel din baza de date
	t_table* saveTable = dataBase->tables;

	// Parcurgem data de baze pana gasim tabela ceruta
	while(strcmp(dataBase->tables->name, tableName))
	{
		dataBase->tables = dataBase->tables->next;

		// Verificam daca am cautat toate tabelel
		// si nu am gasit tabela dorita
		if(dataBase->tables == NULL)
		{
			printf("Table \"%s\" not found in database.\n",tableName);
			dataBase->tables = saveTable;
			return;
		}
	}

	t_column* Column;
	Column = dataBase->tables->columns;

	// Parcurgem fiecare coloana din baza de date
	while(Column != NULL)
	{	
		numberColumns++;

		// Salvam index-ul coloanei cautate
		if(strcmp(Column->name, columnName) == 0)
			columnIndex=numberColumns;

		Column = Column->next;
		
		// Verificam daca am ajuns la finalul coloanelor tabelului
		// si nu am gasit coloana cautata. In if am pus columnIndex == -1
		// pentru cazul in care coloana cautata era ultima
		if((Column == NULL)&&(columnIndex == -1))
		{
			printf("Table \"%s\" does not contain column \"%s\".\n",
					tableName,columnName);

			dataBase->tables = saveTable;
			return;
		}
	}

	// Verificam daca tabelul cautat nu are nici-o linie adaugata
	if(dataBase->tables->lines == NULL)
	{	
		dataBase->tables = saveTable;
		return;
	}

	if(dataBase->tables->type == STRING)
	{
		t_stringLine* line;
		line = (t_stringLine*)dataBase->tables->lines;

		
		// Salvam linia precedenta
		t_stringLine* savePreLine;

		// Salvam linia curenta pentru a elimina memoria
		// dupa schimbarea legaturilor
		t_stringLine* saveFreeLine;


		while(line != NULL)
		{	
			t_stringCell* cell;
			cell = (t_stringCell*) line->cells;

			// Salvam prima celula a liniei
			t_stringCell* translationCell;
			translationCell = cell;

			for(i=0;i<columnIndex-1;i++)
			{
				translationCell = translationCell->next;
			}


			int check = strcmp(translationCell->value, val);
			switch (operation)
			{
				case 0 :
				{
					if(check < 0)
					{	
						// Stergem lista de celule
						deleteStringList(cell);
						
						// Luam cazul in care linia noastra este 
						// prima linie din tabel
						if(line ==(t_stringLine*)dataBase->tables->lines)
						{	
							
							dataBase->tables->lines = line->next;
							saveFreeLine = line;
							line = line->next;

							// Eliberam memoria din linia stearsa
							free(saveFreeLine);
							continue;
						}
						// Luam celelate cazuri
						else
						{
							savePreLine->next = line->next;
							saveFreeLine = line;
							line=line->next;

							// Eliberam memoria din linia stearsa;
							free(saveFreeLine);
							continue;
						}

					}
					break;
				}
				case 1 :
				{
					if(check > 0)
					{	
						// Stergem lista de celule
						deleteStringList(cell);
						
						// Luam cazul in care linia noastra este 
						// prima linie din tabel
						if(line ==(t_stringLine*)dataBase->tables->lines)
						{	
							
							dataBase->tables->lines = line->next;
							saveFreeLine = line;
							line = line->next;

							// Eliberam memoria din linia stearsa
							free(saveFreeLine);
							continue;
						}
						// Luam celelate cazuri
						else
						{
							savePreLine->next = line->next;
							saveFreeLine = line;
							line=line->next;

							// Eliberam memoria din linia stearsa;
							free(saveFreeLine);
							continue;
						}

					}
					break;
				}
				case 2 :
				{
					if(check <= 0)
					{	
						// Stergem lista de celule
						deleteStringList(cell);
						
						// Luam cazul in care linia noastra este 
						// prima linie din tabel
						if(line ==(t_stringLine*)dataBase->tables->lines)
						{	
							
							dataBase->tables->lines = line->next;
							saveFreeLine = line;
							line = line->next;

							// Eliberam memoria din linia stearsa
							free(saveFreeLine);
							continue;
						}
						// Luam celelate cazuri
						else
						{
							savePreLine->next = line->next;
							saveFreeLine = line;
							line=line->next;

							// Eliberam memoria din linia stearsa;
							free(saveFreeLine);
							continue;
						}

					}
					break;
				}
				case 3 :
				{
					if(check >= 0)
					{	
						// Stergem lista de celule
						deleteStringList(cell);
						
						// Luam cazul in care linia noastra este 
						// prima linie din tabel
						if(line ==(t_stringLine*)dataBase->tables->lines)
						{	
							
							dataBase->tables->lines = line->next;
							saveFreeLine = line;
							line = line->next;

							// Eliberam memoria din linia stearsa
							free(saveFreeLine);
							continue;
						}
						// Luam celelate cazuri
						else
						{
							savePreLine->next = line->next;
							saveFreeLine = line;
							line=line->next;

							// Eliberam memoria din linia stearsa;
							free(saveFreeLine);
							continue;
						}

					}
					break;
				}
				case 4 :
				{
					if(check == 0)
					{	
						// Stergem lista de celule
						deleteStringList(cell);
						
						// Luam cazul in care linia noastra este 
						// prima linie din tabel
						if(line ==(t_stringLine*)dataBase->tables->lines)
						{	
							
							dataBase->tables->lines = line->next;
							saveFreeLine = line;
							line = line->next;

							// Eliberam memoria din linia stearsa
							free(saveFreeLine);
							continue;
						}
						// Luam celelate cazuri
						else
						{
							savePreLine->next = line->next;
							saveFreeLine = line;
							line=line->next;

							// Eliberam memoria din linia stearsa;
							free(saveFreeLine);
							continue;
						}

					}
					break;
				}
				case 5 :
				{
					if(check != 0)
					{	
						// Stergem lista de celule
						deleteStringList(cell);
						
						// Luam cazul in care linia noastra este 
						// prima linie din tabel
						if(line ==(t_stringLine*)dataBase->tables->lines)
						{	
							
							dataBase->tables->lines = line->next;
							saveFreeLine = line;
							line = line->next;

							// Eliberam memoria din linia stearsa
							free(saveFreeLine);
							continue;
						}
						// Luam celelate cazuri
						else
						{
							savePreLine->next = line->next;
							saveFreeLine = line;
							line=line->next;

							// Eliberam memoria din linia stearsa;
							free(saveFreeLine);
							continue;
						}

					}
					break;
				}
			}
			
			
			savePreLine = line;
			line=line->next;
		}


	}

	
	else if(dataBase->tables->type == INT)
	{
		t_intLine* line;
		line = (t_intLine*)dataBase->tables->lines;

		t_intLine* saveFreeLine;
			t_intLine* savePreLine;


		while(line != NULL)
		{	
			t_intCell* cell;
			cell = (t_intCell*) line->cells;

			// Salvam prima celula a liniei
			t_intCell* translationCell;
			translationCell = cell;

			for(i=0;i<columnIndex-1;i++)
			{
				translationCell = translationCell->next;
			}

			// Facem din string int
			int intVal = atoi(val);
			int check = translationCell->value - intVal;
			switch (operation)
			{	
				case 0 :
					{
						if(check < 0)
						{	
							// Stergem lista de celule
							deleteIntList(cell);

							// Luam cazul in care linia noastra este 
						// prima linie din tabel
							if(line ==(t_intLine*)dataBase->tables->lines)
							{	
								
								dataBase->tables->lines = line->next;
								saveFreeLine = line;
								line = line->next;

								// Eliberam memoria din linia stearsa
								free(saveFreeLine);
								continue;
							}
							// Luam celelate cazuri
							else
							{
								savePreLine->next = line->next;
								saveFreeLine = line;
								line=line->next;

								// Eliberam memoria din linia stearsa;
								free(saveFreeLine);
								continue;
							}

						}

						break;
					}
				case 1 :
					{
						if(check > 0)
						{	
							// Stergem lista de celule
							deleteIntList(cell);

							// Luam cazul in care linia noastra este 
							// prima linie din tabel
							if(line ==(t_intLine*)dataBase->tables->lines)
							{	
								
								dataBase->tables->lines = line->next;
								saveFreeLine = line;
								line = line->next;

								// Eliberam memoria din linia stearsa
								free(saveFreeLine);
								continue;
							}
							// Luam celelate cazuri
							else
							{
								savePreLine->next = line->next;
								saveFreeLine = line;
								line=line->next;

								// Eliberam memoria din linia stearsa;
								free(saveFreeLine);
								continue;
							}

						}

						break;
					}
				case 2 :
					{
						if(check <= 0)
						{	
							// Stergem lista de celule
							deleteIntList(cell);

							// Luam cazul in care linia noastra este 
							// prima linie din tabel
							if(line ==(t_intLine*)dataBase->tables->lines)
							{	
								
								dataBase->tables->lines = line->next;
								saveFreeLine = line;
								line = line->next;

								// Eliberam memoria din linia stearsa
								free(saveFreeLine);
								continue;
							}
							// Luam celelate cazuri
							else
							{
								savePreLine->next = line->next;
								saveFreeLine = line;
								line=line->next;

								// Eliberam memoria din linia stearsa;
								free(saveFreeLine);
								continue;
							}

						}

						break;
					}
				case 3 :
					{
						if(check >= 0)
						{	
							// Stergem lista de celule
							deleteIntList(cell);

							// Luam cazul in care linia noastra este 
							// prima linie din tabel
							if(line ==(t_intLine*)dataBase->tables->lines)
							{	
								
								dataBase->tables->lines = line->next;
								saveFreeLine = line;
								line = line->next;

								// Eliberam memoria din linia stearsa
								free(saveFreeLine);
								continue;
							}
							// Luam celelate cazuri
							else
							{
								savePreLine->next = line->next;
								saveFreeLine = line;
								line=line->next;

								// Eliberam memoria din linia stearsa;
								free(saveFreeLine);
								continue;
							}

						}

						break;
					}
				case 4 :
					{
						if(check == 0)
						{	
							// Stergem lista de celule
							deleteIntList(cell);

							// Luam cazul in care linia noastra este 
							// prima linie din tabel
							if(line ==(t_intLine*)dataBase->tables->lines)
							{	
								
								dataBase->tables->lines = line->next;
								saveFreeLine = line;
								line = line->next;

								// Eliberam memoria din linia stearsa
								free(saveFreeLine);
								continue;
							}
							// Luam celelate cazuri
							else
							{
								savePreLine->next = line->next;
								saveFreeLine = line;
								line=line->next;

								// Eliberam memoria din linia stearsa;
								free(saveFreeLine);
								continue;
							}

						}

						break;
					}
				case 5 :
					{
						if(check != 0)
						{	
							// Stergem lista de celule
							deleteIntList(cell);

							// Luam cazul in care linia noastra este 
							// prima linie din tabel
							if(line ==(t_intLine*)dataBase->tables->lines)
							{	
								
								dataBase->tables->lines = line->next;
								saveFreeLine = line;
								line = line->next;

								// Eliberam memoria din linia stearsa
								free(saveFreeLine);
								continue;
							}
							// Luam celelate cazuri
							else
							{
								savePreLine->next = line->next;
								saveFreeLine = line;
								line=line->next;

								// Eliberam memoria din linia stearsa;
								free(saveFreeLine);
								continue;
							}

						}

						break;
					}
			}
			
			savePreLine = line;
			line=line->next;
		}
		
	}

	
	else if(dataBase->tables->type == FLOAT)
	{
		t_floatLine* line;
		line = (t_floatLine*)dataBase->tables->lines;


		t_floatLine* saveFreeLine;
		t_floatLine* savePreLine;

		while(line != NULL)
		{	
			// Salvam prima celula a liniei
			t_floatCell* cell;
			cell = (t_floatCell*) line->cells;

		
			t_floatCell* translationCell;
			translationCell = cell;

			// Parcurgem lista de ecelule pana la coloana cautata
			// pentru a verifica daca valoare corespunda
			// cautarii noastre
			for(i=0;i<columnIndex-1;i++)
			{
				translationCell = translationCell->next;
			}

			// Facem din string float
			float floatVal = atof(val);
			float check = translationCell->value - floatVal;
			float zero = 0;

			switch(operation)
			{
				case 0 :
					{
						if(check < zero)
						{	
							// Stergem lista de celule
							deleteFloatList(cell);

							// Luam cazul in care linia noastra este 
							// prima linie din tabel
							if(line ==(t_floatLine*)dataBase->tables->lines)
							{	
								
								dataBase->tables->lines = line->next;
								saveFreeLine = line;
								line = line->next;

								// Eliberam memoria din linia stearsa
								free(saveFreeLine);
								continue;
							}
							// Luam celelate cazuri
							else
							{
								savePreLine->next = line->next;
								saveFreeLine = line;
								line=line->next;

								// Eliberam memoria din linia stearsa;
								free(saveFreeLine);
								continue;
							}

						}
						break;
					}
				case 1 :
					{
						if(check > zero)
						{	
							// Stergem lista de celule
							deleteFloatList(cell);

							// Luam cazul in care linia noastra este 
							// prima linie din tabel
							if(line ==(t_floatLine*)dataBase->tables->lines)
							{	
								
								dataBase->tables->lines = line->next;
								saveFreeLine = line;
								line = line->next;

								// Eliberam memoria din linia stearsa
								free(saveFreeLine);
								continue;
							}
							// Luam celelate cazuri
							else
							{
								savePreLine->next = line->next;
								saveFreeLine = line;
								line=line->next;

								// Eliberam memoria din linia stearsa;
								free(saveFreeLine);
								continue;
							}

						}
						break;
					}
				case 2 :
					{
						if(check <= zero)
						{	
							// Stergem lista de celule
							deleteFloatList(cell);

							// Luam cazul in care linia noastra este 
							// prima linie din tabel
							if(line ==(t_floatLine*)dataBase->tables->lines)
							{	
								
								dataBase->tables->lines = line->next;
								saveFreeLine = line;
								line = line->next;

								// Eliberam memoria din linia stearsa
								free(saveFreeLine);
								continue;
							}
							// Luam celelate cazuri
							else
							{
								savePreLine->next = line->next;
								saveFreeLine = line;
								line=line->next;

								// Eliberam memoria din linia stearsa;
								free(saveFreeLine);
								continue;
							}

						}
						break;
					}
				case 3 :
					{
						if(check >= zero)
						{	
							// Stergem lista de celule
							deleteFloatList(cell);

							// Luam cazul in care linia noastra este 
							// prima linie din tabel
							if(line ==(t_floatLine*)dataBase->tables->lines)
							{	
								
								dataBase->tables->lines = line->next;
								saveFreeLine = line;
								line = line->next;

								// Eliberam memoria din linia stearsa
								free(saveFreeLine);
								continue;
							}
							// Luam celelate cazuri
							else
							{
								savePreLine->next = line->next;
								saveFreeLine = line;
								line=line->next;

								// Eliberam memoria din linia stearsa;
								free(saveFreeLine);
								continue;
							}

						}
						break;
					}
				case 4 :
					{
						if(check == zero)
							{	
								// Stergem lista de celule
								deleteFloatList(cell);

								// Luam cazul in care linia noastra este 
								// prima linie din tabel
								if(line ==(t_floatLine*)dataBase->tables->lines)
								{	
									
									dataBase->tables->lines = line->next;
									saveFreeLine = line;
									line = line->next;

									// Eliberam memoria din linia stearsa
									free(saveFreeLine);
									continue;
								}
								// Luam celelate cazuri
								else
								{
									savePreLine->next = line->next;
									saveFreeLine = line;
									line=line->next;

									// Eliberam memoria din linia stearsa;
									free(saveFreeLine);
									continue;
								}

							}
						break;
					}
				case 5 :
					{
						if(check != zero)
							{	
								// Stergem lista de celule
								deleteFloatList(cell);

								// Luam cazul in care linia noastra este 
								// prima linie din tabel
								if(line ==(t_floatLine*)dataBase->tables->lines)
								{	
									
									dataBase->tables->lines = line->next;
									saveFreeLine = line;
									line = line->next;

									// Eliberam memoria din linia stearsa
									free(saveFreeLine);
									continue;
								}
								// Luam celelate cazuri
								else
								{
									savePreLine->next = line->next;
									saveFreeLine = line;
									line=line->next;

									// Eliberam memoria din linia stearsa;
									free(saveFreeLine);
									continue;
								}

							}
						break;
					}
			}
			
			savePreLine = line;
			line=line->next;
		}
		
	}

	dataBase->tables = saveTable;
}

void DELETE_DB(t_db* dataBase)
{
	t_table* head=NULL;
	t_table* save;

	head = (*dataBase).tables;

	// Parcurgem toate tabelel din baza de date
	while(head != NULL )
	{	
		// Salvam tabelul urmator pentru a-l pute accesa
		// dupa ce eliberam memoria din tabelul curent
		save = head->next;

		//Stergem lista de coloane din tabel
		deleteColumns(head->columns);

		// Verificam tipul tabelului si stergem fiecare linie si
		// lista de celule a acesteia
		if(head->type == STRING)
		{
			t_stringLine* headLine=NULL;
			t_stringLine* saveLine=NULL;
			headLine =(t_stringLine*) head->lines;

			while(headLine!=NULL)
			{
				saveLine=headLine->next;
				deleteStringList(headLine->cells);
				free(headLine);
				headLine = saveLine;
			}
		}
		else if(head->type == INT)
		{	
			t_intLine* headLine=NULL;
			t_intLine* saveLine=NULL;
			headLine =(t_intLine*) head->lines;

			while(headLine!=NULL)
			{
				saveLine=headLine->next;
				deleteIntList(headLine->cells);
				free(headLine);
				headLine = saveLine;
			}
			
			
		}
		else if(head->type == FLOAT)
		{
			t_floatLine* headLine=NULL;
			t_floatLine* saveLine=NULL;
			headLine =(t_floatLine*) head->lines;

			while(headLine!=NULL)
			{
				saveLine=headLine->next;
				deleteFloatList(headLine->cells);
				free(headLine);
				headLine = saveLine;
			}
		}

		free(head);
		head = save;
	}

	exit(0);

}

void DELETE_TABLE(t_db* dataBase, char* tableName)
{	
	t_table* table =(*dataBase).tables;

	// Verificam daca data de baze este goala
	if((*dataBase).tables==NULL)
	{
		printf("Table \"%s\" not found in database.\n",tableName);
		return;
	}

	int found = 0;

	// Cazul in care tabelul cautat este primul din baza de date
	if(strcmp(table->name,tableName) == 0)
	{	
		found = 1;

		// Stergem lista de coloane
		deleteColumns(table->columns);

		// Verificam tipul tabelului si stergem fiecare linie si
		// lista de celule a acesteia
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

		(*dataBase).tables = table->next;
		free(table);

	}

	// Cazul in are tabelul NU este primul din baza de date
	else
	{	t_table* saveTable;

		while(table != NULL)
		{
			if(strcmp(table->name,tableName) == 0)
			{	
				found = 1;

				// Stergem lista de coloane
				deleteColumns(table->columns);

				// Verificam tipul tabelului si stergem fiecare linie si
				// lista de celule a acesteia
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

				saveTable->next = table->next;
				free(table);
				break;
			}

			saveTable = table;
			table = table->next;

		}
	}

	// Daca tabelul cautat nu a fost gasit in baza de date
	if(!found)
	{
		printf("Table \"%s\" not found in database.\n",tableName);
	}
}