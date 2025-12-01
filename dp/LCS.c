#include <stdio.h>
#include <string.h>

#define MAXN 1000

char X[MAXN + 1];
char Y[MAXN + 1];
int  dp[MAXN + 1][MAXN + 1];

int main() {
    // Nhập 2 chuỗi (không có dấu cách)
    scanf("%s", X);
    scanf("%s", Y);

    int m = strlen(X);
    int n = strlen(Y);

    // Khởi tạo hàng 0 và cột 0 = 0
    for (int i = 0; i <= m; i++) dp[i][0] = 0;
    for (int j = 0; j <= n; j++) dp[0][j] = 0;

    // Tính bảng dp
    // dp[i][j] = LCS length của X[0..i-1] và Y[0..j-1]
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (X[i - 1] == Y[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = (dp[i - 1][j] > dp[i][j - 1])
                           ? dp[i - 1][j]
                           : dp[i][j - 1];
            }
        }
    }

    int lcs_len = dp[m][n];
    printf("LCS length: %d\n", lcs_len);

    // Truy vết để lấy 1 chuỗi LCS
    char lcs[MAXN + 1];
    lcs[lcs_len] = '\0';  // kết thúc chuỗi

    int i = m, j = n, k = lcs_len - 1;
    while (i > 0 && j > 0) {
        if (X[i - 1] == Y[j - 1]) {
            lcs[k--] = X[i - 1];
            i--;
            j--;
        } else if (dp[i - 1][j] >= dp[i][j - 1]) {
            i--;
        } else {
            j--;
        }
    }

    printf("LCS string: %s\n", lcs);

    return 0;
}

