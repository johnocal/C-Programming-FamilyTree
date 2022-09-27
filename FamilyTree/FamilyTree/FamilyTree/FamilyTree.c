#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct member 
{
	char name[50];
	int age;
	char leftParentName[50];
	char rightParentName[50];
	struct member* leftParent;
	struct member* rightParent;
};

int addMemberToTree(struct member** head, char name[50],int age,char leftParentName[50],char rightParentName[50])
{
	if ((*head) == NULL)
		return 0;

	if (strcmp((*head)->leftParentName, name) == 0)
	{
		(*head)->leftParent = (struct member*)malloc(sizeof(struct member));

		strcpy((*head)->leftParent->name, name);
		(*head)->leftParent->age = age;
		strcpy((*head)->leftParent->leftParentName, leftParentName);
		strcpy((*head)->leftParent->rightParentName, rightParentName);
		(*head)->leftParent->leftParent = NULL;
		(*head)->leftParent->rightParent = NULL;

		return 1;
	}
	else if (strcmp((*head)->rightParentName, name) == 0)
	{
		(*head)->rightParent = (struct member*)malloc(sizeof(struct member));

		strcpy((*head)->rightParent->name, name);
		(*head)->rightParent->age = age;
		strcpy((*head)->rightParent->leftParentName, leftParentName);
		strcpy((*head)->rightParent->rightParentName, rightParentName);
		(*head)->rightParent->leftParent = NULL;
		(*head)->rightParent->rightParent = NULL;

		return 1;
	}
	else
	{
		if ((*head)->leftParent == NULL)
			return 0;
		struct member* temp = *head;
		*head = (*head)->leftParent;
		if (addMemberToTree(head, name, age, leftParentName, rightParentName) == 1)
			return 1;
		else
		{
			*head = temp;
			if ((*head)->rightParent == NULL)
				return 0;
			temp = *head;
			*head = (*head)->rightParent;
			if (addMemberToTree(head, name, age, leftParentName, rightParentName) == 1)
				return 1;
			else
				return 0;
		}
	}
}

void readDataFromFile(struct member** head)
{
	FILE* fp = fopen("familyinfo.txt", "r");

	char name[50], leftParentName[50], rightParentName[50];
	int age;

	while (fscanf(fp, "%s %d %s %s\n", name, &age, leftParentName, rightParentName) != EOF)
	{
		if (*head == NULL)
		{
			*head = (struct member*)malloc(sizeof(struct member));

			strcpy((*head)->name, name);
			(*head)->age = age;
			strcpy((*head)->leftParentName, leftParentName);
			strcpy((*head)->rightParentName, rightParentName);
			(*head)->leftParent = NULL;
			(*head)->rightParent = NULL;
		}
		else
		{
			struct member* temp = *head;
			addMemberToTree(head, name, age, leftParentName, rightParentName);
			*head = temp;
		}
	}
}

int addNewMember(struct member** head, char name[50], int age, char parentOf[50])
{
	if (*head == NULL)
		return 0;

	if (strcmp((*head)->name, parentOf) == 0)
	{
		if ((*head)->leftParent != NULL && (*head)->rightParent != NULL)
		{
			printf("Child already has 2 parents\n");
			return 0;
		}

		if ((*head)->leftParent == NULL)
		{
			strcpy((*head)->leftParentName, name);
			(*head)->leftParent = (struct member*)malloc(sizeof(struct member));

			(*head)->leftParent->age = age;
			strcpy((*head)->leftParent->name, name);
			strcpy((*head)->leftParent->leftParentName, "NONE");
			strcpy((*head)->leftParent->rightParentName, "NONE");
			(*head)->leftParent->leftParent = NULL;
			(*head)->leftParent->rightParent = NULL;

			return 1;
		}

		if ((*head)->rightParent == NULL)
		{
			strcpy((*head)->rightParentName, name);
			(*head)->rightParent = (struct member*)malloc(sizeof(struct member));

			(*head)->rightParent->age = age;
			strcpy((*head)->rightParent->name, name);
			strcpy((*head)->rightParent->leftParentName, "NONE");
			strcpy((*head)->rightParent->rightParentName, "NONE");
			(*head)->rightParent->leftParent = NULL;
			(*head)->rightParent->rightParent = NULL;

			return 1;
		}
	}

	if ((*head)->leftParent == NULL)
		return 0;

	struct member* temp = *head;
	*head = (*head)->leftParent;
	if (addNewMember(head, name, age, parentOf) == 1)
		return 1;
	else
	{
		*head = temp;
		if ((*head)->rightParent == NULL)
			return 0;

		temp = *head;
		*head = (*head)->rightParent;
		if (addNewMember(head, name, age, parentOf) == 1)
			return 1;
		else
		{
			head = temp;
			return 0;
		}
	}
}

void printFamilyTree(struct member* head)
{
	if (head == NULL)
		return;

	printf("Name: %s\nAge: %d\nLeft Parent: %s\nRight Parent: %s\n\n", head->name,head->age, head->leftParentName, head->rightParentName);
	
	if ((head)->leftParent == NULL)
		return;
	struct member* temp = head;
	head = head->leftParent;
	printFamilyTree(head);

	head = temp;
	if ((head)->rightParent == NULL)
		return;

	temp = head;
	head = head->rightParent;
	printFamilyTree(head);
	head = temp;
}

void findOldest(struct member* head, struct member** oldest)
{
	if (head == NULL)
		return;

	if (head->age > (*oldest)->age)
		*oldest = head;

	if ((head)->leftParent == NULL)
		return;
	struct member* temp = head;
	head = head->leftParent;
	findOldest(head,oldest);

	head = temp;
	if ((head)->rightParent == NULL)
		return;

	temp = head;
	head = head->rightParent;
	findOldest(head,oldest);
	head = temp;
}

int removeMember(struct member** head, char name[50])
{
	if (*head == NULL)
		return 0;

	if ((*head)->leftParent == NULL)
		return 0;

	if (strcmp((*head)->leftParent->name, name) == 0)
	{
		(*head)->leftParent = NULL;
		//strcpy((*head)->leftParentName, "NONE");
		return 1;
	}

	if ((*head)->rightParent == NULL)
		return 0;

	if (strcmp((*head)->rightParent->name, name) == 0)
	{
		(*head)->rightParent = NULL;
		//strcpy((*head)->rightParentName, "NONE");
		return 1;
	}

	struct member* temp = *head;
	*head = (*head)->leftParent;
	if (removeMember(head, name) == 1)
		return 1;
	else
	{
		*head = temp;
		if ((*head)->rightParent == NULL)
			return 0;

		temp = *head;
		*head = (*head)->rightParent;
		if (removeMember(head, name) == 1)
			return 1;
		else
		{
			head = temp;
			return 0;
		}
	}
}

void printTreeLeaves(struct member* head)
{
	if (head == NULL)
		return;

	if(head->leftParent==NULL && head->rightParent==NULL)
		printf("Name: %s\nAge: %d\nLeft Parent: %s\nRight Parent: %s\n\n", head->name, head->age, head->leftParentName, head->rightParentName);

	if ((head)->leftParent == NULL)
		return;
	struct member* temp = head;
	head = head->leftParent;
	printTreeLeaves(head);

	head = temp;
	if ((head)->rightParent == NULL)
		return;

	temp = head;
	head = head->rightParent;
	printTreeLeaves(head);
	head = temp;
}

void menu(struct member** head)
{
	int choice;
	while (1)
	{
		printf("1: Add a family member.\n");
		printf("2: Print the tree.\n");
		printf("3: Find the oldest.\n");
		printf("4: Remove a person.\n");
		printf("5: Print the tree leaves.\n");
		printf("6: Exit.\n");

		scanf("%d", &choice);

		if (choice == 1)
		{
			char name[50], parentOf[50];
			int age;

			printf("Enter name of member: ");
			scanf("%s", name);

			printf("Enter age: ");
			scanf("%d", &age);

			printf("Enter child name: ");
			scanf("%s", parentOf);

			struct member* temp = *head;
			if (addNewMember(head, name, age, parentOf) == 1)
				printf("\nMember added Successfully.\n\n");
			else
				printf("\nChild does not exist. Member not added.\n\n");
			*head = temp;
		}
		else if (choice == 2)
		{
			printFamilyTree(*head);
		}
		else if (choice == 3)
		{
			struct member* oldest = *head;
			findOldest(*head,&oldest);

			printf("Oldest Member\n");
			printf("Name: %s\nAge: %d\n\n", oldest->name, oldest->age);
		}
		else if (choice == 4)
		{
			char name[50];
			printf("Enter person name to remove: ");
			scanf("%s", name);

			struct member* temp = *head;
			if (removeMember(head, name) == 1)
				printf("\nMember removed Successfully.\n\n");
			else
				printf("\nNo such member exist.\n\n");
			*head = temp;
		}
		else if (choice == 5)
		{
			printTreeLeaves(*head);
		}
		else if (choice == 6)
			break;
		else
			printf("\nEnter a valid choice\n\n");

	}
}

int main()
{
	struct member* head=NULL;

	readDataFromFile(&head);

	menu(&head);
}