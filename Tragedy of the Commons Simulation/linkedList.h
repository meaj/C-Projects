struct node
{
	int ident;
	int hapy;
	int type;
	int lBhv;
	struct node *next;
	
};
typedef struct node node;

int length(node *head);

node *insertEnd(node *head, int type);

void printlist(node *head);

node *deleteHead(node *head);