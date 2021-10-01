//Assignment Title: Grocery Store Database
//Date: 16/12/2020

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

struct item {
	int id;
	char name[20];
	double cost;
	char category;
	struct item* next;
};

typedef struct item GroceryItem;

GroceryItem* readfile(int, char*, double, char);
GroceryItem* intoList(GroceryItem*, GroceryItem*);
void display_menu(void);
int add_item(GroceryItem **, int);
int delete_item(GroceryItem **, int);
int change_cost(GroceryItem **, int);
void search_item(GroceryItem **);
int PrintList(GroceryItem *, int);
int sort_item(GroceryItem *, int);
void swapping(GroceryItem *, GroceryItem *);
void writeIntoFile(GroceryItem *, char*);
int integervalidation(int, int);
double doublevalidation(int, double);


int main(void) {
	FILE *grocery;
	char filename[100];
	int id;
	char name[20];
	double cost;
	char category;
	int valid, choice, save_flag = 0;
	GroceryItem* startPtr = NULL;
	char selection;

	printf("\nWelcome to the grocery store!\n");
	printf("Please input the file you'd like to load into stock.\n");
	fgets(filename, 100, stdin);

	size_t len = strlen(filename);
	if (len > 0 && filename[len - 1] == '\n') {
		filename[--len] = '\0';
	}

	if ((grocery = fopen(filename, "r")) == NULL) {
		printf("File could not be opened.\n");
	}
	else {
		printf("%s has been properly loaded into the database!\n", filename);

		//read file 
		fscanf(grocery, "%d %s %lf %c", &id, name, &cost, &category);
		while (!feof(grocery))
		{
			GroceryItem* newPtr = readfile(id, name, cost, category);
			startPtr = intoList(startPtr, newPtr);
			fscanf(grocery, "%d %s %lf %c", &id, name, &cost, &category);
		}
		fclose(grocery);

		display_menu();
		valid = scanf("%d", &choice);
		if(valid != 1)
			choice = integervalidation(valid, choice);//validation

		while (choice != 6) {
			switch (choice) {
			case 1:
				save_flag = add_item(&startPtr, save_flag);
				break;
			case 2:
				save_flag = delete_item(&startPtr, save_flag);
				break;
			case 3:
				save_flag = change_cost(&startPtr, save_flag);
				break;
			case 4:
				search_item(&startPtr);
				break;
			case 5:
				save_flag = PrintList(startPtr, save_flag);
				break;
			default:
				printf("Wrong input, please enter 1 - 6.\n");
				break;
			}
			
			display_menu();

			valid = scanf("%d", &choice);
			if (valid != 1)
				choice = integervalidation(valid, choice);//validation
		}
		getchar();
		if (save_flag == 0) {
			printf("No changes made to the database. Thanks for shopping.\n");
		}
		else if (save_flag == 1) {
			printf("\nWould you like to save the changes made to the database?(Y/N)\n");
			scanf("%c", &selection);
			if (selection == 'Y' || selection == 'y') {
				writeIntoFile(startPtr, filename);
				printf("The file has been saved. Thanks for shopping.\n");
			}
			else {
				printf("The file has not been saved. Thanks for shopping.\n");
			}

		}
	}
	
	return 0;
}

GroceryItem* readfile(int id, char* name, double cost, char category) {
	
	GroceryItem* newPtr = malloc(sizeof(GroceryItem));

	newPtr->id = id;
	strcpy(newPtr->name, name);
	newPtr->cost = cost;
	newPtr->category = category;
	newPtr->next = NULL;

	return newPtr;
}

GroceryItem* intoList(GroceryItem *startPtr , GroceryItem* newPtr) {

	GroceryItem* currentPtr = startPtr;

	if (currentPtr == NULL){
		newPtr->next = currentPtr;
		return newPtr;
	}
	else{
		while (currentPtr->next != NULL){
			currentPtr = currentPtr->next;
		}
	}
	newPtr->next = currentPtr->next;
	currentPtr->next = newPtr;
	return startPtr;
}

void display_menu(void) {
	printf("\nPlease make one of the following selections:\n");
	printf("1. Add new item\n");
	printf("2. Delete item\n");
	printf("3. Change the cost of an item\n");
	printf("4. Search for item\n");
	printf("5. Display inventory details\n");
	printf("6. Quit\n");
}

int add_item(GroceryItem **startPtr, int save_flag) {
	int valid, d_valid;
	GroceryItem *currentPtr;
	GroceryItem *newPtr;

	newPtr = malloc(sizeof(GroceryItem));
	currentPtr = *startPtr;

	int id;
	char name[20], temp1[20], temp2[20];
	double cost;
	char category;
	printf("\nWhat is the ID number of the item to add?\n");
	valid = scanf("%d", &id);
	if(valid != 1)
		id = integervalidation(valid, id);
	getchar();
	printf("What is the name of the item to be added?\n");
	fgets(name, 20, stdin);
	size_t len = strlen(name);
	if (len > 0 && name[len - 1] == '\n') {
		name[--len] = '\0';
	}
	printf("What is the cost of the item to be added?\n");
	d_valid = scanf("%lf", &cost);
	if (d_valid != 1)
		cost = doublevalidation(d_valid, cost);//validation
	printf("What is the product category of the item to be added?\n");
	getchar();
	scanf("%c", &category);
	category = toupper(category);
	while (category != 'M' && category != 'P' && category != 'D' && category != 'C' && category != 'N') {
		printf("%c category is not existed!\n", category);
		printf("M: Meat, P: Produce, D: Dairy, C: Canned goods, N: nonfoods\n");
		printf("What is the product category of the item to be added?\n");
		getchar();
		scanf("%c", &category);
		category = toupper(category);//category is stored in capital letter
	}

	//check duplicate
	GroceryItem* checkPtr;
	if (newPtr != NULL) {
		newPtr->id = id;
		strcpy(newPtr->name, name);
		newPtr->cost = cost;
		newPtr->category = category;

		checkPtr = *startPtr;

		while (checkPtr != NULL) {
			//as long as both names are the same, regardless uppercase or lowercase
			for (int i = 0; i < 20; i++) {
				temp1[i] = toupper(checkPtr->name[i]);
				temp2[i] = toupper(newPtr->name[i]);
			}
			
			if (checkPtr->id == newPtr->id) {
				free(newPtr);
				printf("Sorry, an item with the ID number %d is already in the database.\n", id);
				if (save_flag == 1) {
					return 1;
				}
				else
					return 0;
			}
			else if (strcmp(temp1, temp2) == 0) {
				free(newPtr);
				printf("Sorry, an item with the name %s is already in the database.\n", name);
				if (save_flag == 1) {
					return 1;
				}
				else
					return 0;
			}
			else {
				checkPtr = checkPtr->next;
			}
		}
	}

	//add to the end of the list
	newPtr->next = NULL;//this will be the last

	if (*startPtr == NULL) {
		*startPtr = newPtr;
		printf("%s has been added into the database.\n", newPtr->name);
		return 1;
	}
	else {
		while (currentPtr->next != NULL)
			currentPtr = currentPtr->next;
		currentPtr->next = newPtr;
		printf("%s has been added into the database.\n", newPtr->name);
		return 1;
	}
}

int delete_item(GroceryItem** startPtr, int save_flag) {

	int id, valid;
	GroceryItem* tempPtr = *startPtr, * previousPtr;
	GroceryItem* currentPtr = *startPtr;

	printf("\nWhat is the ID number of the product to delete?\n");
	valid = scanf("%d", &id);
	if (valid != 1)
		id = integervalidation(valid, id);//validation

	//delete first node
	if (tempPtr != NULL && tempPtr->id == id) {
		printf("%s has been deleted from the database.\n", tempPtr->name);
		while (currentPtr->next != NULL) {
			previousPtr = currentPtr;
			currentPtr = currentPtr->next;
		}
		previousPtr->next = NULL;
		*startPtr = currentPtr;
		currentPtr->next = tempPtr->next;
		return 1;
	}

	while (tempPtr != NULL && tempPtr->id != id) {
		previousPtr = tempPtr;
		tempPtr = tempPtr->next;
	}

	
	if (tempPtr == NULL) {
		printf("Sorry, there is no item in the database with that ID number.\n");
		if (save_flag == 1) {
			return 1;
		}
		else
			return 0;
	}
	//delete last node
	else if(tempPtr->next == NULL){
		previousPtr->next = tempPtr->next;
		printf("%s has been deleted from the database.\n", tempPtr->name);
		free(tempPtr);
		return 1;
	}
	//delete the node between first and last
	else {
		printf("%s has been deleted from the database.\n", tempPtr->name);
		while (currentPtr->next != NULL) {
			previousPtr = currentPtr;
			currentPtr = currentPtr->next;
		}
		previousPtr->next = NULL;
		tempPtr->id = currentPtr->id;
		strcpy(tempPtr->name, currentPtr->name);
		tempPtr->cost = currentPtr->cost;
		tempPtr->category = currentPtr->category;

		return 1;
	}
}

int change_cost(GroceryItem** startPtr, int save_flag) {
	int id, valid, d_valid;
	double cost;
	GroceryItem* currentPtr;
	currentPtr = *startPtr;

	printf("\nWhat is the ID number of the item in question?\n");
	valid = scanf("%d", &id);
	if (valid != 1)
		id = integervalidation(valid, id);//validation

	//searh the id 
	while (currentPtr != NULL) {
		if (currentPtr->id == id) {
			printf("What is the new price for %s?\n", currentPtr->name);
			d_valid = scanf("%lf", &cost);
			if (d_valid != 1)
				cost = doublevalidation(d_valid, cost);//validation
			currentPtr->cost = cost;
			printf("%s now costs %.2lf.\n", currentPtr->name, currentPtr->cost);
			return 1;
		}
		currentPtr = currentPtr->next;
	}

	printf("Sorry, there is no item in the database with that ID number.\n");
	if (save_flag == 1) {
		return 1;
	}
	else
		return 0;
}

void search_item(GroceryItem** startPtr) {
	char name[20];
	char temp1[20];
	char temp2[20];
	getchar();
	printf("\nWhich item would you like to search for?\n");
	fgets(name, 20, stdin);
	size_t len = strlen(name);
	if (len > 0 && name[len - 1] == '\n') {
		name[--len] = '\0';
	}
	
	//search name in linked list
	GroceryItem* currentPtr;
	currentPtr = *startPtr;
	
	while (currentPtr != NULL) {
		//neither uppercase nor lowercase wont affect the searching
		for (int i = 0; i < 20; i++) {
			temp1[i] = toupper(currentPtr->name[i]);
			temp2[i] = toupper(name[i]);
		}

		if (strcmp(temp1, temp2) == 0){
			printf("Here is the product information:\n");
			printf("%d %s %.2lf %c\n", currentPtr->id, currentPtr->name, currentPtr->cost, currentPtr->category);
			return;
		}
		currentPtr = currentPtr->next;
	}

	printf("Sorry, we do not have %s in the store.\n", name);
}

int PrintList(GroceryItem *currentPtr, int save_flag) {
	GroceryItem* startPtr = currentPtr;
	char ans;
	int flag;

	if (currentPtr == NULL) {
		printf("List is empty.\n");
		if (save_flag == 1) {
			return 1;
		}
		else
			return 0;
	}
	else {
		printf("\nHere is a listing of all the products in stock: \n");
		printf("%s %s\t%s %s\n","ID", "Product", "Price", "Category");

		while (currentPtr != NULL) {
			printf("%d %s\t%.2lf\t%c\n", currentPtr->id, currentPtr->name, currentPtr->cost, currentPtr->category);
			currentPtr = currentPtr->next;
		}
	}
	printf("Do you want to have the list sorted? (Y/N)\n");
	getchar();
	scanf("%c", &ans);
	if (ans == 'Y' || ans == 'y') {
		flag = sort_item(startPtr, save_flag);
		return flag;
	}
	else {
		printf("The list is not sorted.\n");
		if (save_flag == 1) {
			return 1;
		}
		else
			return 0;
	}
}

int sort_item(GroceryItem *startPtr, int save_flag) {
	int flag, valid;
	int choice;
	printf("\nChoose sorted way:\n");
	printf("1. Ascending by id\n");
	printf("2. Descending by id\n");
	printf("3. Ascending by name\n");
	printf("4. Descending by name\n");
	printf("5. Ascending by cost\n");
	printf("6. Descending by cost\n");
	printf("7. Ascending by category\n");
	printf("8. Descending by category\n");
	valid = scanf("%d", &choice);
	if (valid != 1)
		choice = integervalidation(valid, choice);//check validation

	if (choice == 1) {
	
	GroceryItem* currentPtr = startPtr;
	GroceryItem* newPtr = NULL;
	while (currentPtr != NULL) {
		newPtr = currentPtr->next;
		while (newPtr != NULL) {
			if (currentPtr->id > newPtr->id) {
				swapping(currentPtr, newPtr);
			}
			newPtr = newPtr->next;
		}
		currentPtr = currentPtr->next;
	}
		flag = 1;
	}
	else if (choice == 2) {
		GroceryItem* currentPtr = startPtr;
		GroceryItem* newPtr = NULL;
		while (currentPtr != NULL) {
			newPtr = currentPtr->next;
			while (newPtr != NULL) {
				if (currentPtr->id < newPtr->id) {
					swapping(currentPtr, newPtr);
				}
				newPtr = newPtr->next;
			}
			currentPtr = currentPtr->next;
		}
		flag = 1;
	}
	else if (choice == 3) {
		int i = 0;
		GroceryItem* currentPtr = startPtr;
		GroceryItem* newPtr = NULL;
		while (currentPtr != NULL) {
			newPtr = currentPtr->next;
			while (newPtr != NULL) {
				//if first letters equal, then compare second one and so on
				while (currentPtr->name[i] == newPtr->name[i]) {
					i++;
				}
				if (currentPtr->name[i] > newPtr->name[i]) {
						swapping(currentPtr, newPtr);
				}
				
				newPtr = newPtr->next;
				i = 0;
			}
			currentPtr = currentPtr->next;
		}
		flag = 1;
	}
	else if (choice == 4) {
		int i = 0;
		GroceryItem* currentPtr = startPtr;
		GroceryItem* newPtr = NULL;
		while (currentPtr != NULL) {
			newPtr = currentPtr->next;
			while (newPtr != NULL) {
				//if first letters equal, then compare second one and so on
				while (currentPtr->name[i] == newPtr->name[i]) {
					i++;
				}
				if (currentPtr->name[i] < newPtr->name[i]) {
					swapping(currentPtr, newPtr);
				}

				newPtr = newPtr->next;
				i = 0;
			}
			currentPtr = currentPtr->next;
		}
		flag = 1;
	}
	else if (choice == 5) {
		GroceryItem* currentPtr = startPtr;
		GroceryItem* newPtr = NULL;
		while (currentPtr != NULL) {
			newPtr = currentPtr->next;
			while (newPtr != NULL) {
				if (currentPtr->cost > newPtr->cost) {
					swapping(currentPtr, newPtr);
				}
				newPtr = newPtr->next;
			}
			currentPtr = currentPtr->next;
		}
		flag = 1;
	}
	else if (choice == 6) {
		GroceryItem* currentPtr = startPtr;
		GroceryItem* newPtr = NULL;
		while (currentPtr != NULL) {
			newPtr = currentPtr->next;
			while (newPtr != NULL) {
				if (currentPtr->cost < newPtr->cost) {
					swapping(currentPtr, newPtr);
				}
				newPtr = newPtr->next;
			}
			currentPtr = currentPtr->next;
		}
		flag = 1;
	}
	else if (choice == 7) {
		GroceryItem* currentPtr = startPtr;
		GroceryItem* newPtr = NULL;
		while (currentPtr != NULL) {
			newPtr = currentPtr->next;
			while (newPtr != NULL) {
				if (currentPtr->category > newPtr->category) {
					swapping(currentPtr, newPtr);
				}
				newPtr = newPtr->next;
			}
			currentPtr = currentPtr->next;
		}
		flag = 1;
	}
	else if (choice == 8) {
	GroceryItem* currentPtr = startPtr;
	GroceryItem* newPtr = NULL;
	while (currentPtr != NULL) {
		newPtr = currentPtr->next;
		while (newPtr != NULL) {
			if (currentPtr->category < newPtr->category) {
				swapping(currentPtr, newPtr);
			}
			newPtr = newPtr->next;
		}
		currentPtr = currentPtr->next;
	}
		flag = 1;
	}
	else {
		printf("Invalid input.\nThe list is not sorted.\n");
		flag = 0;
	}
	GroceryItem * currentPtr = startPtr;
	if (currentPtr == NULL) {
		printf("List is empty.\n");
	}
	else if(flag == 1){
		printf("\nHere is the sorted list: \n");
		printf("%s %s\t%s %s\n", "ID", "Product", "Price", "Category");

		while (currentPtr != NULL) {
			printf("%d %s\t%.2lf\t%c\n", currentPtr->id, currentPtr->name, currentPtr->cost, currentPtr->category);
			currentPtr = currentPtr->next;
		}
	}
	if (flag == 0) {
		if (save_flag == 1) {
			return 1;
		}
		else
			return 0;
	}
	else
		return flag;
}

void swapping(GroceryItem* currentPtr, GroceryItem* newPtr) {
	int tempid;
	char tempname[20];
	double tempcost;
	char tempcategory;

	tempid = currentPtr->id;
	currentPtr->id = newPtr->id;
	newPtr->id = tempid;
	strcpy(tempname, currentPtr->name);
	strcpy(currentPtr->name, newPtr->name);
	strcpy(newPtr->name, tempname);
	tempcost = currentPtr->cost;
	currentPtr->cost = newPtr->cost;
	newPtr->cost = tempcost;
	tempcategory = currentPtr->category;
	currentPtr->category = newPtr->category;
	newPtr->category = tempcategory;
}

void writeIntoFile(GroceryItem* startPtr, char *readfilename) {
	FILE* gFile;
	char filename[100];

	printf("What file would you like to save the new database?\n");
	scanf("%s", filename);

	while ((strcmp(filename, readfilename)) == 0) {
		printf("The filename cannot be same as the original data file.\n");
		printf("What file would you like to save the new database?\n");
		scanf("%s", filename);
	}

	gFile = fopen(filename, "w");
	if (gFile == NULL) {
		printf("File could not be opened.\n");
	}
	else{
		GroceryItem* currentPtr = startPtr;
		while (currentPtr != NULL) {
			fprintf(gFile, "%d %s %.2lf %c\n", currentPtr->id, currentPtr->name, currentPtr->cost, currentPtr->category);
			currentPtr = currentPtr->next;
		}
	}
}

int integervalidation(int valid, int input) {
	int temp;
	while (valid != 1) {
		while ((temp = getchar()) != EOF && temp != '\n');
		printf("Invalid input! Only integer is allowed.\n");
		printf("Enter again: ");
		valid = scanf("%d", &input);
	}

	return input;
}

double doublevalidation(int d_valid, double input) {
	int temp;
	while (d_valid != 1) {
		while ((temp = getchar()) != EOF && temp != '\n');
		printf("Invalid input! Only number is allowed.\n");
		printf("Enter again: ");
		d_valid = scanf("%lf", &input);
	}

	return input;
}