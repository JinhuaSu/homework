#σ未知时µ的置信区间
set.seed(1)
m=1000 #循环次数
n=500 #样本量
a=0.05#置信水平
u=0#期望
v=1#标准差
inter=matrix(0,m,2)
prob=matrix(0,m,1)
for(i in 1:m)
{data=rnorm(n,u,v)
inter[i,]=c(mean(data)-qt(1-a/2,n-1)*sd(data)/sqrt(n),
            mean(data)+qt(1-a/2,n-1)*sd(data)/sqrt(n))
prob[i]=(u>inter[i,1])&(u<inter[i,2])
}
colMeans(inter)
mean(prob)

#总体方差的置信区间
set.seed(1)
m=1000 #循环次数
n=500 #样本量
a=0.05#置信水平
u=0#期望
v=1#标准差
inter=matrix(0,m,2)
prob=matrix(0,m,1)
for (i in 1:m)
{data=rnorm(n,u,v)
inter[i,]=c(sd(data)*sqrt(n-1)/sqrt(qchisq(1-a/2,n-1)),
            sd(data)*sqrt(n-1)/sqrt(qchisq(a/2,n-1)))
prob[i]=(v>inter[i,1])&(v<inter[i,2])
}
colMeans(inter)
mean(prob)

#总体比例置的置信区间
set.seed(1)
m=1000 #循环次数
n=500 #样本量
a=0.05#置信水平
p=0.2 #成功概率
inter=matrix(0,m,2)
prob=matrix(0,m,1)
for(i in 1:m)
{data=rbinom(n,1,0.2)#成功抽取1的概率为0.2
S=sqrt(mean(data)*(1-mean(data))/n)
inter[i,]=c(mean(data)-qnorm(1-a/2,mean=0,sd=1)*S,
            mean(data)+qnorm(1-a/2,mean=0,sd=1)*S)
prob[i]=(p>inter[i,1])&(p<inter[i,2])
}
colMeans(inter)
mean(prob)

#课堂练习
#(1)
n = 50
x = rnorm(n)
epsilon = rnorm(n)
y = 1+x+epsilon
beta1 = sum((x - mean(x))*(y-mean(y))) / sum((x - mean(x))*(x - mean(x)))
beta1
#(2)
sigma = 1
sigma_beta = sqrt(n*sigma / ( sum((x - mean(x))*(x - mean(x))) ))
sigma_beta
c(beta1-sigma_beta*1.96,beta1+sigma_beta*1.96)
#(3)
z = (beta1 - 0 )/ sigma_beta
z

#ex1