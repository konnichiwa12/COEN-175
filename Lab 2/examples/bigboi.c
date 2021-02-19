/* bigboi.c */

int **wow;
int woooo[100];

int theEmpty(void){}
char theOne(char *hmm){}
int *theThree(char *hmm1, int hmm2, int ***hmm3){}
int **theSome(int and, char then){}


int declaringStuff(void){
    char aaa;
    int *bbb;
    int *one, two, **three;
}

int likeEverythingElse(void){
    int i;
    char a, b, c, d, e, f, g, h, i, j, k, lll, m, n;
    i = 0;

    for(i = 0; i < 3; i = i + 2){
        int j;
        j = i + 2;
    }

    if(i != 3) i = 3;
    if(i == 3){
        i = 4;
    }else{
        i = 3;
    }


    function();
    function(a + b);
    function(c - d, a + b);

    /* the big one */
    i = a || b && (c == d != e) <= (f - 1) >= (g + 1) < h[a+b] > sizeof j - *k * &lll / -m % !n + sizeof(-negative);


    return i;
}