#include<stdio.h>
int main()
{
        int n;
        scanf("%d",&n);
        int i = 0, j = 0,k,a[10];
        while(n > 0)
        {
                a[i++] = n % 10;
                n /= 10;
        }
        k = i - 1;
        while(a[j] == a[k] && j <= i - 1)
        {
                j++;
                k--;
        }
        if(j > i - 1){
                printf("Y");
        }else{
                printf("N");
        }
        return 0;    
}
