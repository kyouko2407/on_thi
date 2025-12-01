#include <stdio.h>
#include <limits.h>

int cut_rod(int price[], int n, int first_cut[]) {
    int dp[n + 1];
    dp[0] = 0;              // thanh dài 0 thì doanh thu = 0
    first_cut[0] = 0;

    for (int i = 1; i <= n; ++i) {
        int best = INT_MIN;
        int best_cut = 0;

        // thử cắt mảnh đầu dài j (1..i)
        for (int j = 1; j <= i; ++j) {
            int val = price[j] + dp[i - j];
            if (val > best) {
                best = val;
                best_cut = j;
            }
        }

        dp[i] = best;
        first_cut[i] = best_cut;  // lưu lại độ dài mảnh đầu tối ưu
    }

    return dp[n];
}

void print_solution(int n, int first_cut[]) {
    printf("Cach cat toi uu (cac doan): ");
    while (n > 0) {
        printf("%d ", first_cut[n]);
        n -= first_cut[n];
    }
    printf("\n");
}

int main(void) {
    // price[i] = gia doan dai i (i = 1..10)
    // Đặt price[0] = 0 cho tiện chỉ số
    int price[] = {0, 1, 5, 8, 9, 10, 17, 17, 20, 24, 30};
    int n = 17;                             // chiều dài thanh
    int first_cut[n + 1];

    int best = cut_rod(price, n, first_cut);

    printf("Doanh thu toi da cho thanh dai %d: %d\n", n, best);
    print_solution(n, first_cut);

    return 0;
}

