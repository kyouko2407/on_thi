#include <stdio.h>

#define MAX 100

long long memo[MAX];
    for (int i = 0; i < MAX; i++){
        memo[i] = -1;
    }
}

long long top_down (int n){
    if (n <= 1) return n;
    if (memo[n] != -1) return memo[n];
    memo[n] = top_down(n - 1) + top_down(n - 2);
    return memo[n];
}

long long bottom_up (int n){
    if (n < 2) return n;
    int f[100];
    f[0] = 0;
    f[1] = 1;
    for (int i = 2; i <= n; i++){
        f[i] = f[i - 1] + f[i - 2];
    }
    return f[n];
}

int main (){
    int n;
    scanf("%d", &n);
    printf("Phuong phap Bottom_Up: \n");
    printf("%lld", bottom_up(n));
    printf("\nPhuong phap Top_Down: \n");
    initmemo();
    printf("%lld", top_down(n));
    printf("\n");
    return 0;
}

