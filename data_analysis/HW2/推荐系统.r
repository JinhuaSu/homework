library(reshape)
library(plyr)
library(recommenderlab)
library(ggplot2)

#total <- read.table("/data/mobile/processed_data/ubuntu_cat_days.txt",fileEncoding = 'utf-8')
#head(total)
total_time <- read.table("/data/mobile/processed_data/total_time.txt",sep = ',',header = FALSE,fill = TRUE,fileEncoding = 'utf-8')
cluster_label <- read.csv("/data/mobile/processed_data/cluster_label.csv",fileEncoding = 'utf-8')
app_class <- read.csv("/data/mobile/raw_data/app_class.csv",fileEncoding = 'utf-8',header = FALSE)
names(app_class) <- c("appid","class")
names(total_time) <- c("uid","appid","total_time")
total_time1 <- total_time[-which(total_time$appid == '河南省郑州市二七区康复后街'),]
droplevels(total_time1$appid)
total_time11 <- total_time1[-which(total_time1$appid == '河南省郑州市二七区大学北路25号'),]
droplevels(total_time11$appid)

appid0 <- total_time11[c('appid')]
appid <- as.character(appid0[!duplicated(appid0$appid),])
app_num <- length(appid)
uid0 <- total_time11[c('uid')]
uid <- as.character(uid0[!duplicated(uid0$uid),])
user_num <- length(uid)

#A <- array(0,dim = c(user_num,app_num))
#for(i in (1:total_len)){
#  rloc <- which(uid == as.character(total_time1$uid[i]))
#  cloc <- which(appid == as.character(total_time1$appid[i]))
#  A[rloc,cloc] = total_time1$total_time[i]
#}

fre_app <- ddply(total_time11,.(appid),nrow)
fre_app1 <- fre_app[order(-fre_app$V1),]
fre_app2 <- fre_app1[which(fre_app1$V1 > 20),]
fre_app3 <- fre_app2$appid
length(fre_app3)
#total_time2 <- total_time11[(total_time11$appid %in% fre_app3),]
#length(total_time2$uid)

fre_user0 <- ddply(total_time11,.(uid),nrow)
fre_user1 <- fre_user0[order(-fre_user0$V1),]
fre_user2 <- fre_user1[which(fre_user1$V1 > 100 ),]
length(fre_user2$uid)#109个
fre_user <- fre_user2$uid
total_time22 <- total_time11[(total_time11$uid %in% fre_user & total_time11$appid %in% fre_app3),]
length(total_time22$uid)
#调用reshape包的cast()函数将数据转换为usersId为行，moviesId为列的矩阵，删除第一列数据序号列，这个时候的数据包含两种类型:cast_df，data.frame，其中cast_df是不能直接转换为matrix的，因此需要去掉这个类属性，只保留data.frame 也就是数据框
#注意转换后是一个610*9724的超大矩阵，导致数据源评分矩阵是一个非常稀疏、含有许多空缺值的矩阵但并不影响我们的后续操作。为了让数据成为Recommenderlab包能够处理的类型，转换为RealRatingMatrix。
head(total_time22)
#total_time3 <- total_time2[1:10000,]
#total_time4 <- cast(total_time2,uid~appid,value = "total_time")
total_time4 <- cast(total_time22,uid~appid,value = "total_time")
#total_time4
total_time4[1,1:100]

total_time44 <- cast(total_time22,uid~appid,value = "total_time")
users_label <- merge(total_time44,cluster_label,by = 'uid')[c('uid','cluster_label')]
users_0 <- which(users_label$cluster_label == 0)
users_1 <- which(users_label$cluster_label == 1) 
users_2 <- which(users_label$cluster_label == 2) 
#003A8891295BA3EA3A92E0F4B8F9CE42 第一个uid
total_time4 <- total_time4[,-1]
class(total_time4)
class(total_time4) <- "data.frame"
mydata <- as.matrix(total_time4)
#mydata
mydata <- as(mydata,"realRatingMatrix")
#print(mydata)

#调用Recommenderlab的UBCF（基于用户的协同过滤算法）模型
#补个知识点：协同过滤主要分为两个步骤，首先依据目标用户的已知电影评分找到与目标用户观影风格相似的用户群，然后计算该用户群对其他电影的评分，并作为目标用户的预测评分，协同过滤算法的具体细节请参考文献。

mydata.model <- Recommender( mydata, method = "UBCF",parameter = list(method = "pearson"))
#,parameter = list(method = "pearson",nn = 30)
#数据处理完毕，接来下是进行预测评分（看过的电影不会再预测，显示为NA）
mydata.predict <- predict(mydata.model,mydata[1], type="ratings")
time_predict <- as(mydata.predict,"matrix")

#为用户推荐20个预测评分高的应用
mydata.predict2 <- predict( mydata.model,  mydata[1], n=20) #n指推荐电影数量
app_predict <- as(mydata.predict2,"list")
#[1] "18000" "8165"  "10857" "13251" "17442" "23547" "18597" "15629" "2548"  "7903"  "7280"  "776"   "12030"
#[14] "978"   "6957"  "6472"  "16479" "15311" "2344"  "1099" 
#pearson
# [1] "4"     "8165"  "10857" "13251" "23547" "18597" "19171" "15629" "7903"  "776"   "978"   "7280" 
#[13] "12030" "6472"  "6957"  "15311" "16479" "2344"  "1099"  "22375"
#"18000"    "8165"     "10857"    "13251"    "17442"    "23547"    "18597"   "15629"  "2548" 
#10536.397     9914.539  8805.537

mydata.model2 <- Recommender( mydata, method = "IBCF",parameter = list(method = "pearson"))
#,parameter = list(method = "pearson",nn = 30)
mydata.predict21 <- predict(mydata.model2,mydata[1], type="ratings")
time_predict2 <- as(mydata.predict21,"matrix")

mydata.predict22 <- predict( mydata.model2,  mydata[1], n=20) #n指推荐电影数量
app_predict2 <- as(mydata.predict22,"list")

# [1] "22034" "1719"  "1611"  "1094"  "12228" "3560"  "13318" "19777" "2627"  "13992" "21006" "16479"
#[13] "20833" "607"   "19213" "18952" "18504" "16179" "15761" "21392"


#method，评估方法（默认值split）
#train，划分为训练集的数据比例（method为split时，默认值为0.9）
#k运行评估的折数或倍数（method为split时，默认值为NULL）
#given表示用来进行模型评价的items的数量（默认值为3）
#goodRating 表示预测成功的最小评分（默认值为NA），realRatingMatrix时goodRating为必须的参数
model_eval <- evaluationScheme(mydata, method = "split",
                               train = 0.9, given = 20, goodRating = 5)
model_eval
model_ubcf <- Recommender(getData(model_eval, "train"), method = "UBCF",parameter = list(method = "pearson"))
model_ibcf <- Recommender(getData(model_eval, "train"), method = "IBCF",parameter = list(method = "pearson"))

predict_ubcf <- predict(model_ubcf, getData(model_eval, "known"), type = "ratings")
predict_ibcf <- predict(model_ibcf, getData(model_eval, "known"), type = "ratings")

error <- rbind(
  calcPredictionAccuracy(predict_ubcf, getData(model_eval, "unknown")),
  calcPredictionAccuracy(predict_ibcf, getData(model_eval, "unknown")))
#  calcPredictionError(predict_ubcf, getData(model_eval, "unknown")),
#calcPredictionError(predict_ibcf, getData(model_eval, "unknown")))
rownames(error) <- c("UBCF", "IBCF")
error
#MSE （Mean Squared Error）叫做均方误差
#RMSE（Root Mean Squard Error）均方根误差
#MAE(平均绝对误差)
# error
#RMSE        MSE       MAE
#UBCF 37704.98 1421665648 10309.418
#IBCF 15493.56  240050533  5866.105

#各类推荐的app分布UBCF
recommend_ubcf <- function(x){
  pre_ubcf <- predict( mydata.model, mydata[x,], n=20)
  pre_app <- as(pre_ubcf,"list")
  return(pre_app)
}

pre_0_ubcf <- lapply(users_0, recommend_ubcf)
pre_1_ubcf <- lapply(users_1, recommend_ubcf)
pre_2_ubcf <- lapply(users_2, recommend_ubcf)
pre_0_sum_u <- table(as.factor(as.vector(unlist(pre_0_ubcf))))
pre_1_sum_u <- table(as.factor(as.vector(unlist(pre_1_ubcf))))
pre_2_sum_u <- table(as.factor(as.vector(unlist(pre_2_ubcf))))

#各类推荐的app分布IBCF
recommend_ibcf <- function(x){
  pre_ibcf <- predict( mydata.model2, mydata[x,], n=20) 
  pre_app <- as(pre_ibcf,"list")
  return(pre_app)
}
length(users_0)
length(users_1)
length(users_2)
pre_0_ibcf <- lapply(users_0, recommend_ibcf)
pre_1_ibcf <- lapply(users_1, recommend_ibcf)
pre_2_ibcf <- lapply(users_2, recommend_ibcf)
pre_0_sum_i <- table(as.factor(as.vector(unlist(pre_0_ibcf))))
pre_1_sum_i <- table(as.factor(as.vector(unlist(pre_1_ibcf))))
pre_2_sum_i <- table(as.factor(as.vector(unlist(pre_2_ibcf))))

png(file="类型0推荐i.png",width = 1200,height = 400)
barplot(pre_0_sum_i)
dev.off()

png(file="类型1推荐i.png",width = 1200,height = 400)
barplot(pre_1_sum_i)
dev.off()

png(file="类型2推荐i.png",width = 1200,height = 400)
barplot(pre_2_sum_i)
dev.off()
#各类推荐的app类型分布
#推荐id分布
pre_0_sum_i_df <- as.data.frame(pre_0_sum_i)
#合并生成带有类型标记的appid分布
names(pre_0_sum_i_df) <- c('appid','Freq')
pre_0_sum_i_id_class <- merge(pre_0_sum_i_df,app_class,by = 'appid')
#总结每类app推荐个数
pre_0_sum_i_class <- ddply(pre_0_sum_i_id_class,.(class),summarize,sum(Freq))
names(pre_0_sum_i_class) <- c('class','num')
#作图
png(file="人群0推荐app类型i.png",width = 1200,height = 400)
ggplot(pre_0_sum_i_class,aes(x = class,y = num)) +
  geom_bar(stat="identity")+
  labs(x = "class",y = "numbers")
#barplot($,names.arg = pre_0_sum_u_class$)
dev.off()

#1人群
#推荐id分布
pre_1_sum_i_df <- as.data.frame(pre_1_sum_i)
#合并生成带有类型标记的appid分布
names(pre_1_sum_i_df) <- c('appid','Freq')
pre_1_sum_i_id_class <- merge(pre_1_sum_i_df,app_class,by = 'appid')
#总结每类app推荐个数
pre_1_sum_i_class <- ddply(pre_1_sum_i_id_class,.(class),summarize,sum(Freq))
names(pre_1_sum_i_class) <- c('class','num')
#作图
png(file="人群1推荐app类型i.png",width = 1200,height = 400)
ggplot(pre_1_sum_i_class,aes(x = class,y = num)) +
  geom_bar(stat="identity")+
  labs(x = "class",y = "numbers")
#barplot($,names.arg = pre_0_sum_u_class$)
dev.off()

#各类推荐的app类型分布
#推荐id分布
pre_2_sum_i_df <- as.data.frame(pre_2_sum_i)
#合并生成带有类型标记的appid分布
names(pre_2_sum_i_df) <- c('appid','Freq')
pre_2_sum_i_id_class <- merge(pre_2_sum_i_df,app_class,by = 'appid')
#总结每类app推荐个数
pre_2_sum_i_class <- ddply(pre_2_sum_i_id_class,.(class),summarize,sum(Freq))
names(pre_2_sum_i_class) <- c('class','num')
#作图
png(file="人群2推荐app类型i.png",width = 1200,height = 400)
ggplot(pre_2_sum_i_class,aes(x = class,y = num)) +
  geom_bar(stat="identity")+
  labs(x = "class",y = "numbers")
dev.off()