/* weeeee */

int c;
int c;
int *c; 				/* line 5: conflicting types for 'c' */
void pp;

int correctA(void);

int incorrectB(int x){
    int y;
    x = y;
    y = z; 				/* line 13: 'z' undeclared */
}

int incorrectB(int x){		/* line 16: redefinition of 'incorrectB' */
    x = y;				/* line 17: 'y' undeclared */
}

int correctA(void){
    char a;
    char a;				/* line 22: redeclaration of 'a' */
    int a;				/* line 23: redeclaration of 'a' */

    {
        char d;
        d = b;			/* line 27: 'b' undeclared */
    }

    a = b;				/* line 30: 'b' undeclared */
    b = a;
    b = d;				/* line 32: 'd' undeclared */
    d = c;

    correctA();
    incorrectC();			/* line 36: 'incorrectC' undeclared */
}

int correctA(void);

int incorrectD(int x);
char incorrectD(int x);		/* line 42: conflicting types for 'incorrectD' */

int incorrectE(int x);
int incorrectE(int *x);			/* line 45: conflicting types for 'incorrectE' */

int incorrectF(int x);
char incorrectF(int x);			/* line 48: conflicting types for 'incorrectF' */

int incorrectG(int x);
int *incorrectG(int x);			/* line 51: conflicting types for 'incorrectG' */