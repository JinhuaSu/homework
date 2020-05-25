library(plyr)
library(ggplot2)
##批量读入txt文件，并将文本放入同一个数据框
reviewpath <- "/home/opendata/mobile/raw_data"
completepath <- list.files(reviewpath, pattern = "*.txt$", full.names = TRUE)

######批量读入文本
read.txt <- function(x) {
  one_day <- read.table(x,sep = ',',header = FALSE)                   #每行读取
  return(one_day)     #没有return则返回最后一个函数对象
}
review <- lapply(completepath, read.txt)
#如果程序警告，这里可能是部分文件最后一行没有换行导致，不用担心。

######list转数据框
docname <- list.files(reviewpath, pattern = "*.txt$")
reviewdf <- as.data.frame(rbind(docname, unlist(review)), 
                          stringsAsFactors = F) 

day01 = read.table("/home/opendata/mobile/raw_data/day01.txt",sep = ',',header = FALSE)
day02 = read.table("/home/opendata/mobile/raw_data/day02.txt",sep = ',',header = FALSE)
day03 = read.table("/home/opendata/mobile/raw_data/day03.txt",sep = ',',header = FALSE)
day04 = read.table("/home/opendata/mobile/raw_data/day04.txt",sep = ',',header = FALSE)
day05 = read.table("/home/opendata/mobile/raw_data/day05.txt",sep = ',',header = FALSE)
day06 = read.table("/home/opendata/mobile/raw_data/day06.txt",sep = ',',header = FALSE)
day07 = read.table("/home/opendata/mobile/raw_data/day07.txt",sep = ',',header = FALSE)

app_class = read.csv("/home/opendata/mobile/raw_data/app_class.csv",header = FALSE)

names(app_class) <- c("appid","appclass")
names(day01) <- c('uid','appid','app_type','start_day','start_time','end_day','end_time','duration','up_flow','down_flow')
names(day02) <- c('uid','appid','app_type','start_day','start_time','end_day','end_time','duration','up_flow','down_flow')
names(day03) <- c('uid','appid','app_type','start_day','start_time','end_day','end_time','duration','up_flow','down_flow')
names(day04) <- c('uid','appid','app_type','start_day','start_time','end_day','end_time','duration','up_flow','down_flow')
names(day05) <- c('uid','appid','app_type','start_day','start_time','end_day','end_time','duration','up_flow','down_flow')
names(day06) <- c('uid','appid','app_type','start_day','start_time','end_day','end_time','duration','up_flow','down_flow')
names(day07) <- c('uid','appid','app_type','start_day','start_time','end_day','end_time','duration','up_flow','down_flow')

day01$day <- 1
day02$day <- 2
day03$day <- 3
day04$day <- 4
day05$day <- 5
day06$day <- 6
day07$day <- 7

day12 <- rbind(day01,day02)
day34 <- rbind(day03,day04)
day56 <- rbind(day05,day06)
day1234 <- rbind(day12,day34)
day567 <- rbind(day56,day07)
days7 <- rbind(day1234,day567)
#days7 <- data.frame(Name =c('uid','appid','app_type','start_day','start_time','end_day','end_time','duration','up_flow','down_flow','day'))
days7 = merge(days7,app_class,by = "appid")

#1.空缺天数频数
#每人每天涉及到的个数
uid_day_apps <- ddply(days7,.(uid,day),nrow)
#每人检测的天数
day_null <- ddply(uid_day_apps,.(uid),nrow)
day_null$V1 <- 7 - day_null$V1
#print(length(day_null$uid))
#统计频数
null_days <- as.numeric(table(day_null$V1))
label <- c(22244,5873,4594,3667 ,3006, 2699, 2170)
#作出频数直方图
ggplot(day_null,aes(x = V1,y = ..count..)) +
  geom_bar()+
  labs(x = "days",y = "count")
#2.各类app使用总时长（取对数）
dura_sum <- ddply(days7,.(appclass),summarize,sum(duration))
names(dura_sum) <- c("appclass","duration_sum")
dura_sum$duration_sum <- log(dura_sum$duration_sum)
plot(x = dura_sum$appclass,y = dura_sum$duration_sum,type = 'p')

#3.每人app使用总时长(分钟)
person_sum <- ddply(days7,.(uid),summarize,sum(duration))
names(person_sum) <- c("uid","duration_sum")
#days_person <- ddply(apps_day,.(uid),nrow)
#5先行
person_sum <- merge(person_sum,days_person,by = "uid")
person_sum$duration_sum <- person_sum$duration_sum / (60 * person_sum$V1)
person_duration <-person_sum[order(-person_sum$duration_sum),]
person_duration

summary(person_sum$duration_sum)
ggplot(person_sum,aes(x = duration_sum))+
  geom_histogram() + xlim(0,1000)

#4.每类app的个数

app <- days7[c('appid','appclass')]
app <- app[!duplicated(app$appid),]
plot(app$appclass)

#5
#每人每天同一个app记录个数
app_day <- ddply(days7,.(uid,appid,day),nrow)
#每人每天的所用app个数
apps_day <- ddply(app_day,.(uid,day),nrow)
apps_day <- rename(apps_day,c(V1 = 'count'))
#每人每天所用app数七天总和
apps_everyday <- ddply(apps_day,.(uid),summarize,sum(count))
apps_everyday <- rename(apps_everyday,c(..1 = 'count'))
#每人检测的天数
days_person <- ddply(apps_day,.(uid),nrow)
#画出每人每天平均所用app个数
hist(apps_everyday$count / days_person$V1,ann = FALSE)
title(main = '',xlab = 'app numbers',ylab = 'frequency')

#6
#类型
summary(days7$app_type)
days7$app_type[which(days7$app_type == '用户')] <- 'usr'
days7$app_type[which(days7$app_type == '预装')] <- 'sys'
days7$app_type <- droplevels(days7$app_type)
apptype_num <- days7[c('appid','app_type')]
apptype_num <- apptype_num[!duplicated(apptype_num$appid),]
print(length(apptype_num$appid))
table(apptype_num$app_type)
type_num <- c(127,3567)
labels <- c('sys','usr')
piepercent<- paste(round(100*type_num/sum(type_num), 2), "%",labels)
pie(type_num,labels = piepercent)

#7
#各类app流量
app_duration <- ddply(days7,.(appclass),summarize,sum(duration))
app_upflow <- ddply(days7,.(appclass),summarize,sum(up_flow))
app_down_flow <- ddply(days7,.(appclass),summarize,sum(down_flow))
avg_upflow <- app_upflow$..1/app_duration$..1
avg_downflow <- app_down_flow$..1 / app_duration$..1
avg_upflow1 <- data.frame(app_duration$appclass,avg_upflow,'up')
names(avg_upflow1) <- c("appclass",'flow','up_down')
avg_downflow1 <- data.frame(app_duration$appclass,avg_downflow,'down')
names(avg_downflow1) <- c("appclass",'flow','up_down')
avg_flow <- rbind(avg_upflow1,avg_downflow1)

ggplot(avg_flow,aes(x = appclass,y = flow,colour = up_down))+
  geom_point()

print(length(day01$uid))
print(length(day02$uid))
print(length(day03$uid))
print(length(day04$uid))
print(length(day05$uid))
print(length(day06$uid))
print(length(day07$uid))
summary(day01)
summary(day02)
summary(day03)
summary(day04)
summary(day05)
summary(day06)
summary(day07)
#print(day01)
#print(days7.groupby('uid').count())
#for i in day_name:
#   grouped_uid = i.groupby('uid').count()
#  print(grouped_uid)
#uid_dura = pd.DataFrame(grouped_uid['duration'].sum())
#  print(uid_dura)
#uid_null = uid_dura.index[uid_dura['duration'] == 0]
#print(uid_null)