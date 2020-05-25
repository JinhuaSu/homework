#ex2
x2<-ts(file9$overshort)

#ex
X=c(2,2,1,1,5,4,4,3,1,2)
N <- 100
X_1 = c()
X_2 = c()
for( i in 1:N ){
  tmp <- runif()
  if tmp <= 0.3{
    
  }else if(tmp<=0.6){
    
  }else if(tmp<=){
    
  }else if()
}

#ex3

LSAT = c(576,635,558,578,666,580,555,661,651,605,653,575,545,572,594)
GPA = c(339,330,281,303,344,307,300,343,336,313,312,274,276,288,296)
data <- cbind(LSAT,GPA)
N <-1000
cor_hat <- NULL
for (i in 1:N){
  resample_idx <- sample(1:length(data[,1]),length(data[,1]),replace=T)
  resample <- data[resample_idx,]
  cor_hat[i] <- cor(resample[,1],resample[,2])
}
sqrt(var(cor_hat))

# ex4

x <- rnorm(10,mean=0,sd=sqrt(10))
N <-2000
resample_ratio <- 10
sigma_hat <- NULL
for(i in 1:N){
  resample_idx <- sample(1:length(x),length(x)*resample_ratio,replace=T)
  resample <- x[resample_idx]
  sigma_hat[i] <- var(resample) * length(x)*resample_ratio / (length(x) * resample_ratio-1)
}
mean(sigma_hat)