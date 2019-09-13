# this makes sure the model  always returns a number around 5
# num_unique_users and total_users_counted must be mapped to Streams attributes using the streamAttributes and 
# rObjects parameters in the RScript operator 
predictedCount<-(score(num_unique_users, total_users_counted)  + runif(1,0.1, 0.6))
