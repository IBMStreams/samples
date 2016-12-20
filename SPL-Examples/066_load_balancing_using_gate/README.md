As documented in the Streams Info Center for a ThreadedSplit, if the processing time of a tuple varies considerably depending on the tuple data, it may cause problems where a tuple with a long processing time may cause subsequent tuples to be backed up in the stream. This example shows how a Gate operator can be combined with the ThreadedSplit can be used to ensure load balancing.

