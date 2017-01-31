==========================================================================
First created on: Nov/28/2011
Last modified on: Mar/29/2015

Note:
Helper scripts used in running this application use an older technique of modifying the ADL files just before launching this
demo application. Starting in Streams 4.x, SAB files are the preferred way over the ADL files. In addition, ADL files
will be completely removed in future versions of Streams (probably in 2016). Since this example requires manipulating
the ADL files before the application launch, this is NOT GOING TO WORK IN STREAMS 4.x. This example can't be used
in Streams 4.x until it is modified as needed. This example will be updated to work with Streams 4.x at a later time in 2015.

This project directory (059_dynamic_scaleout_XXXX) contains files that exercise price
calculation logic within the IBM InfoSphere Streams execution environment.
This directory also features various helper bash scripts that allow us to
start and stop the application with rich sets of options to collect 
different kinds of performance numbers.
(In this particular example application, we removed the third party code to respect their
licensing policies. We simply stubbed out their price calculation library logic
with our own dummy code that spins the CPU for 35 msec on every request to
calculate an option price.)

There is a also a cool feature built into the application that enables us to
use couple of bash scripts and demonstrate dynamic scale-up and scale-down
(elastic) features. (see a specific section below that explains various steps to
demonstrate adding new pricer engines and stopping currently active pricer engines in
groups).
==========================================================================
This example contains a single SPL project.

059_dynamic_scaleout_XXXXX
It is an SPL project that contains the logic to scale the price calculation code by
encapsulating that code inside a Streams C++ primitive operator and by launching the 
SPL composite as multiple instances in parallel across multiple servers. One of the
techniques we will use to achieve dynamic scaling is via the SPL dynamic import API.
This trick is employed in the com.ibm.streams.pricing.Pricer SPL composite.

For this particular example application, Streams Studio is used only for
editing the project files and visualizing the running application using
the Streams instance graph. That is the reason why you will not see a
launch configuration for this project when you import this project into the 
Streams Studio. You need not create a launch configuration. Because,
this application is built, run, and stopped using bash script files.

This example is slightly an advanced one. Hence, the right thing to do is
to go through the code thoroughly and run the demo of this example by yourself to
understand how it works. There is quite a bit of business logic in this 
example. If all of that is not applicable to you, right thing to do is to
focus on the dynamic scaling part. The general pattern used here for dynamic scaling is
useful. If you don't like the approach taken by us to provide the
dynamic scaling in Streams, please ignore this example. This is just an example
to help the SPL developers. There may be other approaches better than this.

PLEASE READ THIS FILE COMPLETELY BEFORE TRYING TO RUN THIS EXAMPLE.
==========================================================================
Following bash scripts can be used to build and run the Streams application:

a) To build all the application components (SPL projects and C++ .so library projects):
./build_dynamic_scaleout_app.sh -b all -c yes -w $HOME/Examples-for-beginners-For-Beginners

Various options available in the build_dynamic_scaleout_app.sh script:
./build_dynamic_scaleout_app.sh -h

Command line arguments
  -b STRING     buildType                      (lib-only or spl-only or all)
  -c STRING     cleanBeforeBuild               (yes or no)
  -w STRING     dynamicScaleoutWorkspaceDir    (Your dynamic scaleout project workspace directory)
      
  e-g:
  -b lib-only (or) spl-only (or) all
  -c yes (or) -c no
  -w $HOME/Examples-for-beginners-For-Beginners

b) To start the application with one or more pricing engines on one or more machines:
   (Ensure that $HOSTNAME, YOUR_STREAMS_DOMAIN_NAME and YOUR_STREAMS_INSTANCE_NAME are set to the correct values.)

./start_dynamic_scaleout_app.sh -d <YOUR_STREAMS_DOMAIN_NAME> -i <YOUR_STREAMS_INSTANCE_NAME> -s $PWD -g 180 -e 8 -n $HOSTNAME -m $HOSTNAME -p 2000000 -b 58.00 -a 0.0 -d 7    

# If you have more servers to test with, you can also specify more servers in the previous command:
  -n myServer -n yourServer -n ourServer -n herServer>

Various options available in the start_dynamic_scaleout_app.sh script:
./start_dynamic_scaleout_app.sh -h

Command line arguments
  -d STRING       streamsDomainName     (big_space) 
  -i STRING       streamsInstanceName   (silver_stream)
  -s STRING       dynamicScaleOutSplProjectDir   (Your dynamic_scaleout SPL project directory)
  -g INTEGER      grandTotalOfPricers   (200)  (# of pricers that will likely be started during this application run)
  -e INTEGER      pricerEnginesPerNode  (10)   (# of engines per node to be started at the time of application start-up)
  -n STRING       nodesForPricerEngines (sacramento concord hudson potomac)
  -m STRING       nodeForTheMainApp (delaware)
  -p INTEGER      totalSpotPrices (1000)
  -d INTEGER      sourceOperatorInitDelay (10)
  -b FLOAT        beginningSpotPrice (58.00)
  -a FLOAT        additionToCurrentSpotPrice (0.05)
                  
  e-g:
  -d big_space
  -i silver_stream
  -s $PWD
  -g 200
  -e 10
  -n sacramento -n concord -n hudson -n potomac
  -m delaware
  -p 1000
  -d 10
  -b 58.00
  -a 0.05

c) To stop the running application:
./stop_dynamic_scaleout_app.sh -d <YOUR_STREAMS_DOMAIN_NAME> -i <YOUR_STREAMS_INSTANCE_NAME>

Various options available in the stop_dynamic_scaleout_app.sh script:
./stop_dynamic_scaleout_app.sh -h

Command line arguments
  -d STRING     streamsDomainName   (big_space) 
  -i STRING     streamsInstanceName (silver_stream)
  
  e-g:
  -d big_space
  -i silver_stream
  
d) To stop the running Streams instance:
./stop_streams_instance.sh -d <YOUR_STREAMS_DOMAIN_NAME> -i <YOUR_STREAMS_INSTANCE_NAME>

Various options available in the stop_streams_instance.sh script:
./stop_streams_instance.sh -h

Command line arguments
  -d STRING     streamsDomainName   (big_space) 
  -i STRING     streamsInstanceName (silver_stream)
  
  e-g:
  -d big_space
  -i silver_stream  
  
==========================================================================
dynamic_scaleout application also has built-in elastic capabilities. 

i.e. it provides different options to dynamically bring-up more pricer 
engines on-demand or bring-down those pricer engines that are no longer
needed. Following are the two additional bash scripts that will help us in
exercising the elastic features (dynamically scale-up new pricer engines or
dynamically scale-down currently active pricer engines).

1) ./start_new_pricers.sh 
2) ./stop_active_pricers.sh 
   (You must stop the currently active pricers in the reverse group order in which they were started.)
   (If you try to stop in any other way, it will produce unexpected results.)
   (You have to live with this limitation for now. Following examples illustrate how you can bring down a group of pricers together.)


What is an easy way to demonstrate this application?
---------------------------------------
You can demonstrate those features to dynamically scale-up and scale-down
pricer engines by modifying and running the bash script described below. As you run that script,
it is highly recommended that you monitor the application topology using
the Streams Instance Graph facility available in the Streams Studio eclipse tool.
(i.e. In the "Streams Explorer" view, right click on your running instance name and
select "Show Instance Graph" to bring up the Streams Instance Graph tool.)

In an xterm window, run an interactive shell script that conveniently wraps these commands for you.
./demo_dynamic_scaleout_app.sh -d <YOUR_STREAMS_DOMAIN_NAME> -i <YOUR_STREAMS_INSTANCE_NAME> -s $PWD
==========================================================================
