#include <stdio.h>
#include <limits.h>

#define MAXN 15
#define INF  1000000000

int n;
int cost[MAXN][MAXN];
int dp[1 << MAXN][MAXN];
int parent[1 << MAXN][MAXN];

int main() {
    scanf("%d", &n);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            scanf("%d", &cost[i][j]);

    int FULL = (1 << n) - 1;

    // Khởi tạo dp
    for (int mask = 0; mask <= FULL; mask++) {
        for (int i = 0; i < n; i++) {
            dp[mask][i] = INF;
            parent[mask][i] = -1;
        }
    }

    // Bắt đầu từ thành phố 0, mask chỉ chứa bit 0
    dp[1][0] = 0;

    // DP qua mọi mask và đỉnh kết thúc u
    for (int mask = 1; mask <= FULL; mask++) {
        if (!(mask & 1)) continue; // luôn phải chứa thành phố 0

        for (int u = 0; u < n; u++) {
            if (!(mask & (1 << u))) continue;
            if (dp[mask][u] == INF) continue;

            // Thử đi từ u sang v chưa nằm trong mask
            for (int v = 0; v < n; v++) {
                if (mask & (1 << v)) continue; // v đã được thăm

                int nmask = mask | (1 << v);
                int val = dp[mask][u] + cost[u][v];
                if (val < dp[nmask][v]) {
                    dp[nmask][v] = val;
                    parent[nmask][v] = u;
                }
            }
        }
    }

    // Kết thúc: từ FULL mask, quay về 0
    int ans = INF;
    int last = -1;
    for (int u = 0; u < n; u++) {
        if (dp[FULL][u] == INF) continue;
        int tour_cost = dp[FULL][u] + cost[u][0];
        if (tour_cost < ans) {
            ans = tour_cost;
            last = u;
        }
    }

    if (ans == INF) {
        printf("Khong co chu trinh Hamilton hop le.\n");
        return 0;
    }

    printf("Min tour cost: %d\n", ans);

    // Truy vết đường đi
    int path[MAXN + 1];
    int mask = FULL;
    int cur = last;
    int idx = n - 1;

    while (cur != 0) {
        path[idx--] = cur;
        int prev = parent[mask][cur];
        mask ^= (1 << cur);
        cur = prev;
    }
    path[0] = 0;
    path[n] = 0; // quay về 0

    printf("Tour: ");
    for (int i = 0; i <= n; i++) {
        printf("%d ", path[i]);
    }
    printf("\n");

    return 0;
}

