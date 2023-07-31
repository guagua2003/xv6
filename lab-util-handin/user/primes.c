#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX_NUM 34

void process_prime(int a[], int num) {
    if (num == 1) {
        printf("prime %d\n", a[0]);
        return;
    }

    int p[2];
    int i;
    int prime = a[0];
    int temp;

    printf("prime %d\n", prime);

    pipe(p);

    if (fork() == 0) {
        for (i = 0; i < num; i++)
            write(p[1], (char *)(&a[i]), sizeof(a[i]));
            
        exit(0);
    }

    close(p[1]);

    if (fork() == 0) {
        int count = 0;
        char buffer[sizeof(int)];

        while (read(p[0], buffer, sizeof(buffer)) != 0) {
            temp = *((int *)buffer);

            if (temp % prime != 0) {
                a[count] = temp;
                count++;
            }
        }

        process_prime(a, count);

        exit(0);
    }

    wait(0);
    wait(0);

    close(p[0]);
}

int main() {
    int a[MAX_NUM];
    
    for (int i = 0; i < MAX_NUM; i++)
        a[i] = i + 2;

    process_prime(a, MAX_NUM);

    exit(0);
}

