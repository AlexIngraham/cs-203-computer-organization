#include <stdio.h>
#include <stdlib.h>

int global;
int garray[4];

int test (int x) {

    int y;
    int tarray[4];

    printf("\n");
    printf("test %d param x : %p\n", x, &x);
    printf("test %d local y : %p\n", y, &y);
    printf("tarray 0 loc : %p\n", &tarray[0]);
    printf("tarray 1 loc : %p\n", &tarray[1]);
    printf("tarray 2 loc : %p\n", &tarray[2]);
    printf("tarray 3 loc : %p\n", &tarray[3]);

    if(x > 0)
        test(x - 1);

    return 0;
}


int test_1 (int x) {

    int y;

    printf("\n");
    printf("test1 %d param x : %p\n", x, &x);
    printf("test1 %d local y : %p\n", y, &y);

    if(x > 0)
        test_1(x - 1);

    return 0;
}

int test_2 (int x) {

    int y;
    int z;

    printf("\n");
    printf("test2 %d param x : %p\n", x, &x);
    printf("test2 %d local y : %p\n", y, &y);
    printf("test2 %d local z : %p\n", z, &z);

    if(x > 0)
        test_2(x - 1);

    return 0;
}
int main(int argc, char* argv[]) {

    int x;
    int y;
    int z;
    char *v1 = malloc( 8 );
    char *v2 = malloc( 8 );

    printf("argc loc : %p\n", &argc);
    printf("argv loc : %p\n", &argv);
    printf("argv ptr : %p\n", argv);

    printf("test location: %p\n", &test);
    printf("test_1 location: %p\n", &test_1);
    printf("test_2 location: %p\n", &test_2);
    printf("main location: %p\n", &main);
    printf("global val   : %p\n", &global);
    printf("garray 0 loc : %p\n", &garray[0]);
    printf("garray 1 loc : %p\n", &garray[1]);
    printf("garray 2 loc : %p\n", &garray[2]);
    printf("garray 3 loc : %p\n", &garray[3]);

    printf("local var x: %p\n", &x);
    printf("local var y: %p\n", &y);
    printf("local var z: %p\n", &z);
    printf("local var v1: %p\n", &v1);
    printf("local var v2: %p\n", &v2);

    printf("malloc v1 0 : %p\n", v1);
    printf("malloc v1 1 : %p\n", v1 + 1);
    printf("malloc v1 2 : %p\n", v1 + 2);
    printf("malloc v1 3 : %p\n", v1 + 3);
    printf("malloc v1 4 : %p\n", v1 + 4);
    printf("malloc v1 5 : %p\n", v1 + 5);
    printf("malloc v1 6 : %p\n", v1 + 6);
    printf("malloc v1 7 : %p\n", v1 + 7);

    printf("malloc v2 0 : %p\n", v2);
    printf("malloc v2 1 : %p\n", v2 + 1);
    printf("malloc v2 2 : %p\n", v2 + 2);
    printf("malloc v2 3 : %p\n", v2 + 3);
    printf("malloc v2 4 : %p\n", v2 + 4);
    printf("malloc v2 5 : %p\n", v2 + 5);
    printf("malloc v2 6 : %p\n", v2 + 6);
    printf("malloc v2 7 : %p\n", v2 + 7);

    test(4);
    test_1(4);
    test_2(4);

    return 0;
}
