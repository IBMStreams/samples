This directory contains sample codes to demonstrate the usage of non-blocking
checkpointing in consistent regions. There is a related blog post on 
Streamsdev (https://developer.ibm.com/streamsdev/).

## Included files: 
The top directory contains the following sub-directories and files:

* `MyToolkit/`: A toolkit named 'MyToolkit'.
 
* `MyToolkit/impl/include/NBUnorderedMap.h`: The `NBUnorderedMap` C++ class 
 template which implements an unordered map with copy-on-write and non-blocking 
 checkpointing capabilities.

* `MyToolkit/CRSource/`: `CRSource` operator which generates input tuples and 
 serves as the start of a Consistent Region.

* `MyToolkit/NBOp/`: `NBOp` operator which uses a `NBUnorderedMap` as its state 
 and implements `SPL::StateHandler` interface to do non-blocking checkpointing. The operator
 can checkpoints its state (the `NBUnorderedMap`) while processing new tuples 
 and updating the state. 

* `test/test.splmm`: The SPL source file.

* `Makefile`: The Makefile to compile the code.

* `data/`: data directory.

* `data/input.50K`: A file containing 50,000 input tuples emulating quotes in stock exchange. 
 `CRSource` operator loads them in memory and repetitively submits them downstream.

* `data/delay.50K`: A file containing lines in the form of "numTuples|delayInNanoSeconds".
 For example, "50000|27000" means for the 50000 tuples, the delay between
 submitting two tuples is 27000 nano-seconds. It can contain multiple lines, and 
 each line configures the inter-tuple delay for a range of tuples; this way user
 can simulate dynamically varying input rates.

Among the files, test/test.splmm is the SPL source file and it defines the test
application: A `CRSource` operator connected to a `NBOp` operator and the two forms
a Consistent Region.

The `NBOp` operator is defined in MyToolkit/NBOp/. It is a C++ primitive operator.
Its state is a map from stock tip to per-instrument state (which is 25 floats). The
operator can be configured via the "useNonBlockingCheckpoint" parameter in SPL 
source to generate codes that do either non-blocking or blocking checkpointing. 
For non-blocking checkpointing, the map is of type `NBUnorderedMap`; otherwise the
map is of type `std::tr1::unordered_map`. Please take a look at NBOp_h.cgt and 
NBOp_cpp.cgt as examples of implementing StateHandler callbacks for non-blocking
and blocking checkpointing. 

In addition, `NBOp` has an optional parameter called "checkpointDelay" which 
specifies the number of seconds to sleep in `checkpoint()` callback. This can be
set to introduce arbitrary delay in `checkpoint()`. When non-blocking checkpointing
is used, the delay in `checkpoint()` should not affect when tuple processing is 
resumed.

The NBUnorderedMap is a unordered map container which internally does 
copy-on-write to support updating and checkpointing concurrently. It is defined
in MyToolkit/impl/include/NBUnorderedMap.h.

## How to compile:
To compile, please run make from the top directory. For example:

 `make consistent=yes period=10.0 tupExpected=1000000000`

There are three paramters to specify:
 - consistent: whether to enable consistent region
 - period: checkpointing period of consistent region (in seconds)
 - tupExpected : total number of tuples to generate by CRSource

After compilation, the file 'output/test.Main.sab' should be generated for job
submission.

## How to run:
To run the application, please first create, configure and start Streams domain 
and instance, then submit the job via streamtool submitjob command.

Step 1: create your Streams domain and start the domain. For example:

```
 export STREAMS_DOMAIN_ID=MyDomainName

 streamtool mkdomain --property jmx.port=0 --property sws.port=0

 streamtool genkey

 streamtool startdomain
```

Step 2: create your Streams instance. For example:

```
 export STREAMS_INSTANCE_ID=MyInstanceName

 streamtool mkinstance
```

Step 3: to support consistent region and checkpointing, the instance must be 
configured with a checkpoint backend store. In Streams V4.2, there are three 
options: file system, Redis key-value store, or IBM Hyperstate Accelerator 
data store.

For example, to configure the instance to use file system as the checkpoint 
backend store, run the following streamtool commands before starting instance:

```
 CHECKPOINT_ROOT_DIR=/absolute/path/to/directory/for/storing/checkpoints/

 mkdir -p $CHECKPOINT_ROOT_DIR

 streamtool setproperty instance.checkpointRepository=fileSystem
 
 streamtool setproperty instance.checkpointRepositoryConfiguration="{\"Dir\":\"$CHECKPOINT_ROOT_DIR\"}"
```
To configure the instance to use Redis or IBM Hyperstate Accelerator, please 
follow the instructions in Streams document. Here are related references:

[Streams V4.2 documentation](https://www.ibm.com/support/knowledgecenter/SSCRJU_4.2.0/com.ibm.streams.cfg.doc/doc/ibminfospherestreams-configuring-checkpoint-data-store.html)

[How to install and configure Redis](https://developer.ibm.com/streamsdev/docs/install-configure-redis/)

[How to setup Redis replication with InfoSphere Streams](https://developer.ibm.com/streamsdev/docs/setup-redis-replication-infosphere-streams-4-0/)

You can also run streamtool setproperty to change the checkpoint backend 
store type and/or configuration after instance is started, but please note 
that you must restart the instance to make the new settings take effect.

Step 4: start the instance.

 `streamtool startinstance`

Step 5: submit job.

 `streamtool submitjob output/test.Main.sab`

You may turn on debug-level tracing at submission time:

 `streamtool submitjob output/test.Main.sab -C tracing=debug`

In test/test.splmm code, we set `NBOp` operator to sleep for 10 seconds in its 
`checkpoint()` function. From the traces of start operator (`CRSource`), we can see that tuple 
processing is resumed while `NBOp`'s checkpointing is ongoing in the background:

```
# CRSource operator resumes tuple flow
27 Sep 2016 23:27:28.454 [19346] DEBUG #splapptrc,J[1],P[3],Source,consistentRegion_0 M[ConsistentRegionEventHandler.cpp:executeResumeSequence:275]  - Submitting ResumeMarker to output ports.

# NBOp finishes checkpointing 10 seoncds later
27 Sep 2016 23:27:38.598 [19312] INFO #splapptrc,J[1],P[4],MyOp,spl_ckpt M[CheckpointContextImpl.cpp:createCheckpoint:353]  - Operator checkpointing is complete: SequenceID = 2, Size = 5550004 Bytes (non-incremental checkpoint data: 5550004 Bytes, incremental checkpoint data: 0 Bytes), Time = 10.1592 Seconds

# CRSource operator gets notified via regionCheckpointed() callback
27 Sep 2016 23:27:38.622 [19346] INFO #splapptrc,J[1],P[3],Source,spl_operator M[Source.cpp:regionCheckpointed:259]  - The Consistent Region has been checkpointed. Checkpoint sequence ID = 2
```

