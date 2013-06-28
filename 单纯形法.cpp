#include<stdio.h>
#include<string.h>
#include<math.h>
#include<algorithm>
using namespace std;
const double eps=1e-9;
double ga[50][50],gc[50];
int gw[50+50],gpos[50+50];
int row,col;
int gans; // 1唯一最优解   2为无穷最优解   3为无穷大的解    4为无解 
//转轴操作换x行，y列的那个未知数 
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
    //先更新x行的系数 
    tmp=ga[x][y];
    ga[x][y]=-1;
    for (i=0;i<=col;i++) ga[x][i]=ga[x][i]/(-tmp);
    //将其他行y列替换 
    for (i=0;i<=row;i++){
        if (fabs(ga[i][y])>0&&i!=x){
            tmp=ga[i][y];
            ga[i][y]=0;
            for (j=0;j<=col;j++) ga[i][j]=ga[i][j]+ga[x][j]*tmp;
        }
    }
}

//删除x[row+col]，构造题目要求的约束矩阵
void build(){
    int i,j;
    int x,y;
    for (i=1;i<=row+col;i++) gpos[gw[i]]=i;
    if (gpos[row+col]<=row){   //如果x[row+col]是基变量则进行转轴操作
        i=gpos[row+col];
        for (j=1;j<=col;j++){
            if (fabs(ga[i][j])>eps){
                pivot(i,j);
            }
        }
        for (i=1;i<=row+col;i++) gpos[gw[i]]=i;
    }
    //将x[row+col]换到最后一列，删除
    x=gpos[row+col];y=row+col;
    swap(gw[x],gw[y]);
    x=x-row;
    y=y-row;
    for (i=0;i<=row;i++) swap(ga[i][x],ga[i][y]);
    for (i=1;i<=row+col;i++) gpos[gw[i]]=i;
    col--;
    //构造满足约束条件的矩阵
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
        //查找系数>=0的非基变量 
        j=0;
        for (i=1;i<=col;i++){
            if (ga[0][i]>eps){
                if (j==0||gw[row+i]<gw[row+j]) j=i;  //勃兰特规则：选下标最小的满足要求的
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
        //转轴操作得到更优解 
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
void danchun(){   //判断是否有解
    int i,j;
    for (i=1;i<=row+col;i++) gw[i]=i;         //初始矩阵下标 
    //构造辅助矩阵
    for (i=0;i<col;i++) ga[0][i]=0;
    ga[0][i]=-1;
    j=1;
    for (i=2;i<=row;i++){
        if (ga[i][0]<ga[j][0]) j=i;
    }
    //转轴操作，将人工变量作为基变量，则此矩阵满足所有ga[i][0]非负性 
    pivot(j,col);
    if (work(0)){
        //删除x[row+col]，构造题目要求的约束矩阵 
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
        work(1);    //矩阵本身就满足非负要求 
        ans=(int)ceil(ga[0][0]*m-eps);
        printf("Nasa can spend %d taka.\n",ans);
    }
    //while (1);
    return 0;
}
