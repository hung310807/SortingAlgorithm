#include<iostream>
#include<math.h>
#include<algorithm>
using namespace std;

void printOut(int a[], int size){
    for(int i = 0; i < size; ++i){
        cout<<a[i]<<" ";
    }
    cout<<endl;
    return ;
}

void quickSort(int a[], int start, int end, int size){
    int pivot = a[start];
    int i = start + 1, j = end;
    while(i <= j){
        while(a[i] < pivot && i <= end){
            ++i;
        }
        while(a[j] > pivot && j >= start){
            --j;
        }
        if(i <= j){
            swap(a[i], a[j]);
            ++i;
            --j;
        }
    }
    swap(a[j], a[start]);//let pivot be the barrier
    printOut(a, size);
    if(j > start){
        quickSort(a, start, j, size);
    }
    if(end > i){
        quickSort(a, i, end, size);
    }
}

int main(){
    int a[] = {1000, 999, 453, 123, 54, 32, 23, 14, 0};
    quickSort(a, 0, sizeof(a) / 4 - 1, sizeof(a) / 4);
    printOut(a, sizeof(a) / 4);
    return 0;
}
