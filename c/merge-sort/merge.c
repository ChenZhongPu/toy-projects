#include "merge.h"
#include <stdlib.h>
#include <string.h>

void merge1(int *a, int left, int mid, int right, int *aux) {
  for (int i = left; i <= right; i++) {
    aux[i] = a[i];
  }
  int i = left, j = mid + 1;
  for (int k = left; k <= right; k++) {
    if (i > mid) {
      a[k] = aux[j++];
    } else if (j > right) {
      a[k] = aux[i++];
    } else if (aux[j] < aux[i]) {
      a[k] = aux[j++];
    } else {
      a[k] = aux[i++];
    }
  }
}

void merge_sort1(int *a, int left, int right, int *aux) {
  if (right <= left) {
    return;
  }
  int mid = left + (right - left) / 2;
  merge_sort1(a, left, mid, aux);
  merge_sort1(a, mid + 1, right, aux);
  merge1(a, left, mid, right, aux);
}

void sort1(int *a, int len) {
  int *aux = malloc(len * sizeof(int));
  merge_sort1(a, 0, len - 1, aux);
}

void merge2(int *a, int left, int mid, int right, int *aux) {
  for (int i = left; i <= right; i++) {
    aux[i] = a[i];
  }
  int i = left, j = mid + 1, k = left;
  while (i <= mid && j <= right) {
    if (aux[j] < aux[i]) {
      a[k++] = aux[j++];
    } else {
      a[k++] = aux[i++];
    }
  }
  while (i <= mid) {
    a[k++] = aux[i++];
  }
  while (j <= right) {
    a[k++] = aux[j++];
  }
}

void merge_sort2(int *a, int left, int right, int *aux) {
  if (right <= left) {
    return;
  }
  int mid = left + (right - left) / 2;
  merge_sort2(a, left, mid, aux);
  merge_sort2(a, mid + 1, right, aux);
  merge2(a, left, mid, right, aux);
}

void sort2(int *a, int len) {
  int *aux = malloc(len * sizeof(int));
  merge_sort2(a, 0, len - 1, aux);
}

void merge3(int *a, int lo, int mid, int hi) {
  int n1 = mid - lo + 1;
  int n2 = hi - mid;
  int *left = malloc(n1 * sizeof(int));
  int *right = malloc(n2 * sizeof(int));

  for (int i = 0; i < n1; i++) {
    left[i] = a[lo + i];
  }
  for (int i = 0; i < n2; i++) {
    right[i] = a[mid + 1 + i];
  }

  int i = 0, j = 0, k = lo;
  while (i < n1 && j < n2) {
    if (left[i] < right[j]) {
      a[k++] = left[i++];
    } else {
      a[k++] = right[j++];
    }
  }

  while (i < n1) {
    a[k++] = left[i++];
  }
  while (j < n2) {
    a[k++] = right[j++];
  }
  free(left);
  free(right);
}

void merge_sort3(int *a, int lo, int hi) {
  if (lo >= hi) {
    return;
  }
  int mid = lo + (hi - lo) / 2;
  merge_sort3(a, lo, mid);
  merge_sort3(a, mid + 1, hi);
  merge3(a, lo, mid, hi);
}

void sort3(int *a, int n) { merge_sort3(a, 0, n - 1); }

void merge4(int *a, int left, int mid, int right, int *aux) {
  memcpy(aux + left, a + left, (right - left + 1) * sizeof(int));

  int i = left, j = mid + 1;
  for (int k = left; k <= right; k++) {
    if (i > mid) {
      a[k] = aux[j++];
    } else if (j > right) {
      a[k] = aux[i++];
    } else if (aux[j] < aux[i]) {
      a[k] = aux[j++];
    } else {
      a[k] = aux[i++];
    }
  }
}

void merge_sort4(int *a, int left, int right, int *aux) {
  if (right <= left) {
    return;
  }
  int mid = left + (right - left) / 2;
  merge_sort4(a, left, mid, aux);
  merge_sort4(a, mid + 1, right, aux);
  merge4(a, left, mid, right, aux);
}

void sort4(int *a, int len) {
  int *aux = malloc(len * sizeof(int));
  merge_sort4(a, 0, len - 1, aux);
}
