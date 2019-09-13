
##Whenever this script is run, it will look for a model named /tmp/model_latest.rds

if (file.exists("/tmp/model_latest.rds")){
# if that model exists
	if (file.info("/tmp/model_latest.rds")$size > 0 ) {
		# and the file is not empty, reload the model
		model2 <- readRDS("/tmp/model_latest.rds")
		# re-define the scoring function
		score <- function(unique, total){
			row<-list(count_unique=unique,count=total)
			x<-predict(model2, row)
			if (x == 0) { return(unique)}
			return (x)
		}
	}
} else {

	log("Running reloaded model failed, no model" )

}
