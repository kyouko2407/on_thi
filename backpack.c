#include <stdio.h>

#define N 3
#define W 50

int max(int a, int b) {
    return (a > b) ? a : b;
}

int main(void) {
    int weight[N + 1] = {0, 10, 20, 30};
    int value[N + 1]  = {0, 60, 100, 120};

    int dp[N + 1][W + 1];

    for (int i = 0; i <= N; ++i) {
        for (int w = 0; w <= W; ++w) {
            if (i == 0 || w == 0) {
                dp[i][w] = 0;
            }
        }
    }

    for (int i = 1; i <= N; ++i) {
        for (int w = 1; w <= W; ++w) {
            if (weight[i] > w) {
                dp[i][w] = dp[i - 1][w];
            } else {
                dp[i][w] = max(dp[i - 1][w],
                               value[i] + dp[i - 1][w - weight[i]]);
            }
        }
    }

    printf("Gia tri toi da co the dat duoc: %d\n", dp[N][W]);

    int w = W;
    int chosen[N + 1] = {0};

    for (int i = N; i >= 1; --i) {
        if (dp[i][w] != dp[i - 1][w]) {
            chosen[i] = 1;
            w -= weight[i];
        }
    }

    printf("Cac mon duoc chon (theo chi so 1..N):\n");
    for (int i = 1; i <= N; ++i) {
        if (chosen[i]) {
            printf("Mon %d (w = %d, v = %d)\n", i, weight[i], value[i]);
        }
    }
    printf("\nBang DP:\n");
    int cols[] = {0, 10, 20, 30, 40, 50};
    int numCols = sizeof(cols) / sizeof(cols[0]);

    printf("i\\w");
    for (int c = 0; c < numCols; ++c) {
        printf("\t%d", cols[c]);
    }
    printf("\n");

    for (int i = 0; i <= N; ++i) {
        printf("%d", i);
        for (int c = 0; c < numCols; ++c) {
            int ww = cols[c];
            printf("\t%d", dp[i][ww]);
        }
        printf("\n");
    }

    return 0;
}
