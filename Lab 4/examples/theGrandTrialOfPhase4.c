/* Obligatory disclaimer that this check could be error prone, please upload your error file in the case of any unclear discrepancies */
/* BREAK CHECK */

/* RETURN CHECK - If this passes, isCompatibleWith() and promote() is most likely functional */
int intReturnCheck(void){
	int a;
	int b;
	char c;
	int *d;
	
	return a;
	return b;
	return c;
	return d;		/* invalid return type */
}
int voidReturnCheck(void){
	int a;
	int b;
	char c;
	int *d;
	
	return a;
	return b;
	return c;
	return d;		/* invalid return type */
}
char charReturnCheck(void){
	int a;
	int b;
	char c;
	int *d;
	
	return a;
	return b;
	return c;
	return d;		/* invalid return type */
}
int *pointerReturnCheck(void){
	int a;
	int b;
	char c;
	int *d;
	
	return a;		/* invalid return type */
	return b;		/* invalid return type */
	return c;		/* invalid return type */
	return d;
}

/* PREDICATE IN WHILE/FOR/IF - If this passes, isPredicate() is most likely functional */
int f(void){
}

int predicateInConditionals(void){
	int a;
	int b;
	char c;
	int *d;
	char e[10];
	
	while(a){}
	for(a = 0; b; a){}
	if(c){}
	while(d){}
	for(a = 0; d; a){}
	if(e){}
	while(e){}
	if(f){}			/* invalid type for test expression */
	for(a = 0; f; a){}	/* invalid type for test expression */
	while(f){}		/* invalid type for test expression */
}

/* ASSIGNMENT IS COMPATIBLE? */
char assignmentCompatibleCheck(void){
	int a;
	char b;
	int *c;
	char *d;
	int e[10];
	a = b;
	b = a;
	a = c;				/* invalid operands to binary = */
	c = e;
	d = c;				/* invalid operands to binary = */
	c = a;				/* invalid operands to binary = */
}

/* LVALUE TESTS WILL BE TESTED IN OTHER TESTS FOR INDIVIDUAL OPERATIONS */
/* OPERATION TESTS – the returns int check for a correct return type */
int logicalOperations(void){
	int a;
	char b;
	int *c;
	char *d;
	int e[10];

	return a && b;
	return c || d;
	return a && e;

	a && b = a;		/* lvalue required in expression */
	c || d = c;		/* lvalue required in expression */
}

int equalityOperations(void){
	int a;
	char b;
	int *c;
	char *d;
	int e[10];

	return a == b;
	return c != e;

	c == d;			/* invalid operands to binary == */
	a != e;			/* invalid operands to binary != */

	a == b = a;		/* lvalue required in expression */
	c != e = c;		/* lvalue required in expression */
}

int relationalOperations(void){
	int a;
	char b;
	int *c;
	char *d;
	int e[10];

	return a <= b;
	return c > e;

	c >= d;			/* invalid operands to binary >= */
	a < e;			/* invalid operands to binary < */

	a <= b = a;		/* lvalue required in expression */
	c >= e = c;		/* lvalue required in expression */
}

int additiveIntOperation(void){
	int a;
	char b;
	int *c;
	char *d;
	int e[10];

	return a + b;
	return c - e;
	return e - c;

	c - d;			/* invalid operands to binary - */
	c + e;			/* invalid operands to binary + */

	a + b = a;		/* lvalue required in expression */
	c - e = c;		/* lvalue required in expression */
}

char *additivePointerOperation(void){
	int a;
	char b;
	char *c;
	int *d;
	char e[10];

	return e - b;
	return d + a;		/* invalid return type */
	return b + d;		/* invalid return type */

	c + e;			/* invalid operands to binary + */
	a - e;			/* invalid operands to binary - */

	d + a = a;		/* lvalue required in expression */
	e - b = c;		/* lvalue required in expression */
}

char dereferenceOperation(void){
	char a;
	char *b;
	char **c;

	return *b;
	return *c;		/* invalid return type */
	return **c;

	*a;			/* invalid operand to unary * */
	
	*b = *b;
}

char **addressOperation(void){
	char a;
	char *b;
	char **c;

	return &b;
	return &c;		/* invalid return type */
	return &a;		/* invalid return type */
	
	&b = &b;		/* lvalue required in expression */
}

/* DOES NOT TEST int/INT DISTINCTION – PROMOTION OCCURS */
int multiplicativeIntOperation(void){
	int a;
	int b;
	
	return a * a;
	return a / b;
	return b * a;
	return b / b;
	return a % a;
	
	a % b;			/* invalid operands to binary % */
	a * a = b;		/* lvalue required in expression */
}

int someOtherPrefixOperation(void){
	int a;
	char b;
	int *c;
	
	return -a;
	return -1;
	return -b;
	return !a;
	return !!b;
	
	-c;			/* invalid operand to unary - */
	!a = 1;			/* lvalue required in expression */
	!!b = 0;		/* lvalue required in expression */

	return sizeof a;
	return sizeof b;

	sizeof a = 5;		/* lvalue required in expression */
}

int *castOperation(void){
	int a;
	char b;
	int *c;

	return (int *) a;
	return (int) b;	/* invalid return type */
	return (char) c;	/* invalid return type */
	return (int) a;		/* invalid return type */
	return (int *) c;
}

int *arrayStuff(void){
	int **a;
	int *b[10];
	int c[10];

	return a[10];
	return b[20];
	return c[30];		/* invalid return type */
}

int incrementStuff(void){
	int a;
	char b;
	int c;
	int *d;

	return a++;
	return b--;
	return c++;
	return d--;		/* invalid return type */

	5--;			/* lvalue required in expression */
	a++ = 3;		/* lvalue required in expression */
}

int primaryLvalues(void){
	10 = 1;				/* lvalue required in expression */
	"hello" = "wow";		/* lvalue required in expression */
	1.23 = 1.23;			/* lvalue required in expression */
	'a' = 'c';			/* lvalue required in expression */
	("hello again") = "goodbye";	/* lvalue required in expression */
}

int funA(int a, char b, int* c){
}
int funB(int a, char b, int* c){
}
char***** funC(void){
}

char***** someFunTests(void){
	int a;
	char b;
	int *c;

	return funC();

	funA(b, c, c);			/* invalid arguments to called function */
	funA(a, b, c, a);		/* invalid arguments to called function */
	funB(a, a);			/* invalid arguments to called function */
	funB(a, a, c, a);

	funB(a, b, c, a, a, a, c, a) = 3;	/* lvalue required in expression */
}


