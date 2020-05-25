library(ggplot2)

x <- c(2, 2, 1, 1, 5, 4, 4, 3, 1, 2)
# set.seed(1234)
rex <- sample(x, length(x)*1000, replace = T)

u <- seq(from = 0, to = 6, by = 0.01)
fn <- sapply(u, function(i){
  mean(x <= i)
})
refn <- sapply(u, function(i){
  mean(rex <= i)
})

dat <- data.frame(u, fn, refn)
ggplot(dat, aes(u)) +
  geom_line(aes(y = fn), col = 'steelblue') +
  geom_line(aes(y = refn), col = 'forestgreen') +
  theme_minimal(base_size = 12)

sample(x,)