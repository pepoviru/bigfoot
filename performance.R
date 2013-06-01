library(ggplot2)
library(psych)

t15<-read.csv('test-rw-15mi.csv') ;
t60<-read.csv('test-rw-1hr.csv');
t<-rbind(t15,t60)
t$nelem<-(t$to-t$from+1);
t$pagesize<-as.factor((t$to-t$from+1)/60/1000);
p<-ggplot(t,aes(x=from, y=tuser,color=pagesize,group=pagesize))
p<-p + geom_point() + geom_line()
p<-p + labs(title = 'User execution time (seconds) using differen page sizes (minutes).')
p<-p + theme_bw()
p
