This example shows how multiple SPL applications can be grouped together so that they can be started, monitored, and stopped together. There is no code that needs to be written to accomplish this grouping.

One can simply create an SPL application set project inside the Streams Studio and then right-click on this project to get a dialog box displaying all possible applications in your workspace eligible to be grouped. You can now select the SPL projects that need to be grouped together. It is important to note that only those applications with Distributed active build configuration are eligible for the application set grouping.

In this example, you will notice that the 019_import_export_at_work and the 030_spl_config_at_work projects are added to the application set. You can now right click on the 039_application_set project and build/start/stop all of its member applications together.

