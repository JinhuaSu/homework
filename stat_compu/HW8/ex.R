n=100
hat_mu1<-NULL
hat_mu2<-NULL
for (i in 1:1000){
  data=runif(n,0,1)
  X=data^2
  hat_mu1[i]=mean(X)
  Y=(1-data)^2
  hat_mu2[i]=0.5*mean(X+Y)
}
c(mean(hat_mu1),mean(hat_mu2),var(hat_mu1),var(hat_mu2))

library(MASS)
n=100
hat_mu1<-NULL
hat_mu2<-NULL
Mu=c(5,5)
Sigma=matrix(c(2,0.8,0.8,2),ncol=2,nrow=2,byrow=T)
for (i in 1:1000){
  Data=mvrnorm(n,Mu,Sigma)
  X=Data[,1]
  hat_mu1[i]=mean(X)
  Y=Data[,2]
  hat_mu2[i]=0.5*mean(X+Y);
}
c(mean(hat_mu1),mean(hat_mu2),var(hat_mu1),var(hat_mu2))

n=20
hat_mu1<-NULL
hat_mu2<-NULL
for (i in 1:1000){
  U=runif(n,0,1)
  X=exp(U)
  hat_mu1[i]=mean(X)
  Y=exp(1-U)
  hat_mu2[i]=0.5*mean(X+Y);
}
c(mean(hat_mu1),mean(hat_mu2),var(hat_mu1),var(hat_mu2))