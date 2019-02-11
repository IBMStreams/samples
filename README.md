# IBM Streams Sample Catalog

The [catalog](http://ibmstreams.github.io/samples/) is the main page for links to samples hosted in this repository.


**Changelog:**

* Feb 2019: Added search by URL with the `filter` query parameter, e.g. `http://127.0.0.1:4000/?filter=python` will only show samples with the `python` keyword.
* March 2017: Added support to display samples not hosted in this repository. (Add an entry to `extSamples.json`).


### How to add new links to the catalog
See [instructions here](https://github.com/IBMStreams/samples/wiki/Adding-a-sample-to-the-catalog-and-repo).
### If you wish to clone the repo and run the app locally then follow the commands below:
For any reason if you wish to run this code locally, you can follow the steps:

 Step 1) Install Jekyll
    ```
    sudo gem install jekyll bundler
    ```
If you want to read more on Jekyll, [click here](https://jekyllrb.com/)

Step 2) git clone
    ```
    git clone git clone https://github.com/IBMStreams/samples -b gh-pages
    ```

Step 3) run the application
    ```
    cd samples
    bundle exec jekyll serve
    ```

Website will be running locally on: http://127.0.0.1:4000/
