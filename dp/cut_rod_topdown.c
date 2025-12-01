#include <stdio.h>
#include <limits.h>

#define MAXN 1000
#define INF  1000000000

int price[MAXN + 1];
int dp_no_cost[MAXN + 1];
int dp_with_cost[MAXN + 1];

int max(int a, int b) { return (a > b) ? a : b; }

int main() {
    int n;
    int cost;  // phí mỗi lần cắt

    // Nhập độ dài thanh gỗ
    scanf("%d", &n);

    // Nhập bảng giá: price[i] = giá bán đoạn dài i
    for (int i = 1; i <= n; i++) {
        scanf("%d", &price[i]);
    }

    // Nhập phí mỗi lần cắt (ví dụ: 2)
    scanf("%d", &cost);

    // ---------- Phiên bản 1: KHÔNG TÍNH PHÍ CẮT ----------
    dp_no_cost[0] = 0;
    for (int len = 1; len <= n; len++) {
        int best = -INF;
        for (int first = 1; first <= len; first++) {
            int val = price[first] + dp_no_cost[len - first];
            if (val > best) best = val;
        }
        dp_no_cost[len] = best;
    }

    // ---------- Phiên bản 2: CÓ TÍNH PHÍ CẮT ----------
    // Mỗi LẦN cắt (tách một đoạn ra) bị trừ thêm 'cost'
    // Ý tưởng:
    //  - Không cắt: lấy nguyên price[len]
    //  - Cắt tại vị trí first: 
    //       price[first] + dp_with_cost[len - first] - cost
    dp_with_cost[0] = 0;
    for (int len = 1; len <= n; len++) {
        // Trường hợp không cắt nữa
        int best = price[len];
        // Trường hợp có cắt
        for (int first = 1; first < len; first++) {
            int val = price[first] + dp_with_cost[len - first] - cost;
            if (val > best) best = val;
        }
        dp_with_cost[len] = best;
    }

    // In kết quả
    printf("Max revenue (no cut cost): %d\n", dp_no_cost[n]);
    printf("Max revenue (with cut cost = %d): %d\n", cost, dp_with_cost[n]);

    return 0;
}

