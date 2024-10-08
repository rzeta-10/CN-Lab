#include<stdio.h>
#define max 10
int arr[max][max]={0};
int r,c,flag=1,flag1=1;
int hor[max]={0};
int r_hor[max]={0};
int ver[max]={0};
int r_ver[max]={0};
int main()
{
    scanf("%d %d",&r,&c);
    for(int i=0;i<r;i++)
    {
        for(int j=0;j<c;j++)
        {
            scanf("%d",&arr[i][j]);
        }
    }
    for(int j=0;j<c;j++)
    {
        for(int i=0;i<r;i++)
        {
            hor[j]^=arr[i][j];
        }
    }
    for(int i=0;i<r;i++)
    {
        for(int j=0;j<c;j++)
        {
            ver[i]^=arr[i][j];
        }
    }
    for(int i=0;i<r;i++)
    {
        for(int j=0;j<c;j++)
        {
            scanf("%d",&arr[i][j]);
        }
    }
    for(int j=0;j<c;j++)
    {
        for(int i=0;i<r;i++)
        {
            r_hor[j]^=arr[i][j];
        }
    }
    for(int i=0;i<r;i++)
    {
        for(int j=0;j<c;j++)
        {
            r_ver[i]^=arr[i][j];
        }
    }
    for(int i=0;i<c;i++)
    {
        if(hor[i]!=r_hor[i])
        {
            printf("lrc false\n");
            flag=0;
            break;
        }
    }
    if(flag==1)
    printf("lrc true\n");
    for(int i=0;i<r;i++)
    {
        if(ver[i]!=r_ver[i])
        {
            printf("vrc false");
            flag1=0;
            break;
        }
    }
    if(flag1==1)
    printf("vrc true");
    return 0;
}