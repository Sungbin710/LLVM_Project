#include <stdio.h>

//int add(int a, int b) __attribute__((section(".f3_section")))
int add(int a, int b)
{
        return a+b;
}

//int sub(int a, int b) __attribute__((section(".f2_section")))
int sub(int a, int b)
{
        return a-b;
}

//int main() __attribute__((section(".f1_section")))
int main()
{
        int a = 1;
        int b = 3;
        int res = 0;

        if(a == 1)
        {
                res = add(a,b);
        }
        else
        {
                res = sub(a,b);
        }


        return 0;
}

