#include <stdio.h>

#define MAXN 1000
#define MAXW 1000
#define max(a,b) ((a) > (b) ? (a) : (b))

int w[MAXN + 1];
int v[MAXN + 1];
int dp[MAXW + 1];

int main() {
    int n, W;
    scanf("%d %d", &n, &W);

    for (int i = 1; i <= n; i++) scanf("%d", &w[i]);
    for (int i = 1; i <= n; i++) scanf("%d", &v[i]);

    // dp[j] = giá trị lớn nhất với sức chứa j (unbounded)
    for (int j = 0; j <= W; j++) dp[j] = 0;

    for (int i = 1; i <= n; i++) {
        for (int j = w[i]; j <= W; j++) {
            // Lưu ý: dùng lại dp[j - w[i]] (vẫn là hàng hiện tại)
            int take = v[i] + dp[j - w[i]]; 
            if (take > dp[j]) dp[j] = take;
        }
    }

    printf("Max value (unbounded): %d\n", dp[W]);

    return 0;
}

