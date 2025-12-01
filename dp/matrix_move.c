#include <stdio.h>
#define INF 1000000000

int main() {
    int M, N;
    scanf("%d %d", &M, &N);

    int A[1005][1005];
    int dp[1005][1005];

    // Nhập ma trận
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            scanf("%d", &A[i][j]);

    // Khởi tạo dp
    dp[0][0] = A[0][0];

    // Hàng đầu tiên
    for (int j = 1; j < N; j++)
        dp[0][j] = dp[0][j - 1] + A[0][j];

    // Cột đầu tiên
    for (int i = 1; i < M; i++)
        dp[i][0] = dp[i - 1][0] + A[i][0];

    // Tính các ô còn lại
    for (int i = 1; i < M; i++) {
        for (int j = 1; j < N; j++) {
            int up   = dp[i - 1][j];
            int left = dp[i][j - 1];
            dp[i][j] = A[i][j] + (up < left ? up : left);
        }
    }

    printf("%d\n", dp[M - 1][N - 1]);
    return 0;
}

