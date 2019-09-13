wd <- getwd()

thisDir <- dirname(parent.frame(2)$ofile) 

setwd(thisDir)
model <- readRDS("./predictCount_v1.rds")
		
## convenience function for scoring
score <- function(unique, total){
    row<-list(count_unique=unique,count=total)
	return(predict(model,row))
}
	