#!/bin/sh
# =======================
# This script can be used to build the complete dynamic_scaleout SPL project from 
# an xterm window. Specifically for this project, we will use the Streams Studio only 
# to edit the files and view the running application topology. Building, starting,
# and stopping the application will be done in the xterm window using shell scripts.
# =======================
s=0	# Number of options to shift out
while getopts "b:c:w:h" options; do
    case $options in
    b) buildType=$OPTARG
       let s=s+2
       ;;
    c) cleanBeforeBuild=$OPTARG
       let s=s+2
       ;;
    w) dynamic_scaleout_project_workspace_dir=$OPTARG
       let s=s+2
       ;;
    h | * ) echo "
Command line arguments
  -b STRING     buildType                      (lib-only or spl-only or all)
  -c STRING     cleanBeforeBuild               (yes or no)
  -w STRING     dynamicScaleoutWorkspaceDir    (Your dynamic scaleout project workspace directory)

  e-g:
  -b lib-only (or) spl-only (or) all
  -c yes (or) -c no
  -w $HOME/SPL-Examples-For-Beginners
"
        exit 1
        ;;
    esac
done
shift $s

# Validate the build type entered by the user.
if [ "$buildType" != "lib-only" ] && [ "$buildType" != "spl-only" ] && [ "$buildType" != "all" ];
then
   echo "Missing or wrong build type via the -b option."
   echo "Either 'lib-only' or 'spl-only' or 'all' must be specified as a build type."
   echo ""
   echo "Get help using -h option."
   exit 1
fi

# Validate the clean before build option entered by the user.
if [ "$cleanBeforeBuild" != "yes" ] && [ "$cleanBeforeBuild" != "no" ];
then
   echo "Missing or wrong clean before build value via the -c option."
   echo "Either 'yes' or 'no' must be specified as a value for clean before build."
   echo ""
   echo "Get help using -h option."
   exit 1
fi

# Validate the project root directory entered by the user.
if [ "$dynamic_scaleout_project_workspace_dir" == "" ];
then
   echo "Missing or wrong dynamic ScaleOut project workspace dir via the -w option."
   echo "Dynamic ScaleOut project workspace directory must be specified."
   echo ""
   echo "Get help using -h option."
   exit 1
fi

if [ "$buildType" == "lib-only" ] || [ "$buildType" == "all" ];
then
# We simplified this example by not requiring to build a .so file. Please ignore this.
#   cd $dynamic_scaleout_project_workspace_dir/AmGenLib
#   ./mk
echo
fi


if [ "$buildType" == "spl-only" ] || [ "$buildType" == "all" ];
then
   cd $dynamic_scaleout_project_workspace_dir/059_dynamic_scaleout_of_streams_application

   if [ "$cleanBeforeBuild" == "yes" ];
   then
      rm -rf ./output
   fi

   ./mk
fi

date
