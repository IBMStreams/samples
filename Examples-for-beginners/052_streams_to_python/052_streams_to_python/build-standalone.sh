rm -rf ./output/python.wrapper.example.streams_to_python/Standalone
sc -a -T -M python.wrapper.example::streams_to_python --data-directory=data --output-directory=./output/python.wrapper.example.streams_to_python/Standalone -x -I$(python -c 'import distutils.sysconfig as sc; print sc.get_python_inc()')
