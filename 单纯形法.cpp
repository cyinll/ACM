#include<stdio.h>
#include<string.h>
#include<math.h>
#include<algorithm>
using namespace std;
const double eps=1e-9;
double ga[50][50],gc[50];
int gw[50+50],gpos[50+50];
int row,col;
int gans; // 1Ψһ���Ž�   2Ϊ�������Ž�   3Ϊ�����Ľ�    4Ϊ�޽� 
//ת�������x�У�y�е��Ǹ�δ֪�� 
void output(){
    int i,j;
    for (i=0;i<=row;i++){
        for (j=0;j<=col;j++) printf("%.2lf ",ga[i][j]);
        printf("\n");
    }
}
void pivot(int x,int y){
    int i,j;
    double tmp;
    swap(gw[x],gw[row+y]);
    //�ȸ���x�е�ϵ�� 
    tmp=ga[x][y];
    ga[x][y]=-1;
    for (i=0;i<=col;i++) ga[x][i]=ga[x][i]/(-tmp);
    //��������y���滻 
    for (i=0;i<=row;i++){
        if (fabs(ga[i][y])>0&&i!=x){
            tmp=ga[i][y];
            ga[i][y]=0;
            for (j=0;j<=col;j++) ga[i][j]=ga[i][j]+ga[x][j]*tmp;
        }
    }
}

//ɾ��x[row+col]��������ĿҪ���Լ������
void build(){
    int i,j;
    int x,y;
    for (i=1;i<=row+col;i++) gpos[gw[i]]=i;
    if (gpos[row+col]<=row){   //���x[row+col]�ǻ����������ת�����
        i=gpos[row+col];
        for (j=1;j<=col;j++){
            if (fabs(ga[i][j])>eps){
                pivot(i,j);
            }
        }
        for (i=1;i<=row+col;i++) gpos[gw[i]]=i;
    }
    //��x[row+col]�������һ�У�ɾ��
    x=gpos[row+col];y=row+col;
    swap(gw[x],gw[y]);
    x=x-row;
    y=y-row;
    for (i=0;i<=row;i++) swap(ga[i][x],ga[i][y]);
    for (i=1;i<=row+col;i++) gpos[gw[i]]=i;
    col--;
    //��������Լ�������ľ���
    for (i=0;i<=col;i++) ga[0][i]=0; 
    ga[0][0]=gc[0];
    for (i=1;i<=col;i++){
        x=gpos[i+row];
        if (x>row){
            x=x-row;
            ga[0][x]=ga[0][x]+gc[i];
        }
        else{
            for (j=0;j<=col;j++)
                ga[0][j]=ga[0][j]+gc[i]*ga[x][j];
        }
    }
}        
int work(int sta){
    int i,j,k;
    double zx,tmp;
    while (1){
        //����ϵ��>=0�ķǻ����� 
        j=0;
        for (i=1;i<=col;i++){
            if (ga[0][i]>eps){
                if (j==0||gw[row+i]<gw[row+j]) j=i;  //�����ع���ѡ�±���С������Ҫ���
            }
        }
        if (j==0) break;
        zx=0;k=0;
        for (i=1;i<=row;i++){
            if (ga[i][j]<0){
                tmp=ga[i][0]/(-ga[i][j]);
                if (k==0||tmp<zx){zx=tmp;k=i;}
            }
        }
        if (k==0){
            if (sta==0) return 0;
            gans=3;
            return 0;
        }
        //ת������õ����Ž� 
        pivot(k,j);
    }
    if (sta==0){
        if (fabs(ga[0][0])<eps) return 1;
        return 0;
    }
    for (i=1;i<=row;i++){
        if (fabs(ga[i][0])<eps){
            gans=2;
            return 1;
        }
    }
    gans=1;
    return 1;
}
void danchun(){   //�ж��Ƿ��н�
    int i,j;
    for (i=1;i<=row+col;i++) gw[i]=i;         //��ʼ�����±� 
    //���츨������
    for (i=0;i<col;i++) ga[0][i]=0;
    ga[0][i]=-1;
    j=1;
    for (i=2;i<=row;i++){
        if (ga[i][0]<ga[j][0]) j=i;
    }
    //ת����������˹�������Ϊ����������˾�����������ga[i][0]�Ǹ��� 
    pivot(j,col);
    if (work(0)){
        //ɾ��x[row+col]��������ĿҪ���Լ������ 
        build();
        work(1);
    }else{
        gans=4;
    }
}
int n,m;
double a[50];
int ans;
int main(){
    //freopen("1.in","r",stdin);
    int i,j;
    while (scanf("%d%d",&n,&m)!=EOF){
        memset(ga,0,sizeof(ga));
        for (i=1;i<=n;i++) scanf("%lf",&gc[i]);
        for (i=1;i<=n;i++) ga[0][i]=gc[i];
        row=m;col=n;
        for (i=1;i<=m;i++){
            for (j=0;j<=n;j++) scanf("%lf",&a[j]);
            for (j=1;j<=n;j++) ga[i][j]=-a[j-1];
            ga[i][0]=a[j-1];
        }
        work(1);    //�����������Ǹ�Ҫ�� 
        ans=(int)ceil(ga[0][0]*m-eps);
        printf("Nasa can spend %d taka.\n",ans);
    }
    //while (1);
    return 0;
}
