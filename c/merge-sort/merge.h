/**
 * @brief Merge sort where aur array is allocated only once
 *
 * @param a
 * @param n
 */
void sort1(int *a, int n);

/**
 * @brief Merge sort. The same with sort1() except few branches
 *
 * @param a
 * @param n
 */
void sort2(int *a, int n);

/**
 * @brief Merge sort. The same with sort2() except allocation inside merge
 *
 * @param a
 * @param n
 */
void sort3(int *a, int n);

/**
 * @brief Merge sort. The same with sort1() using memcpy
 *
 * @param a
 * @param n
 */
void sort4(int *a, int n);
