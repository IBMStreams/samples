# Bluemix Open Source Github.io page

The Bluemix.github.io page is where Bluemix sample links to be displayed.
This repo is fully managed by the Bluemix Developer Advocate team, making sure we are displaying links to showcase demos in which are fully working and clear to follow through.

### How to add new links to the Bluemix.github.io page
We have a process in which how new links can be added to the Bluemix GitHub.io page.
To add a new link follow these steps:

Step 1) Create a new issue and label it as "Potential-Links".
The issue descreption must have the following details:
- Live URL of the site
- GitHub repo
- DeveloperWorks blog post for that demo
- Good ReadMe follows our IBM Bluemix ReadMe standards.

Step 2) Once issue is created with new link, we will review the issue created, we will quickly check the source code demo, ReadMe and making sure the application is at the standards we want.

We want to show large applications that shows off our technologies, we don't want hello world or getting started stuff.



### If you wish to clone the repo and run the app locally then follow the commands below:
For any reason if you wish to run this code locally, you can follow the steps:

 Step 1) Install Jekyll
    ```
    sudo gem install jekyll bundler
    ```
If you want to read more on Jekyll, [click here](https://jekyllrb.com/)

Step 2) git clone
    ```
    git clone https://github.com/IBM-Bluemix/IBM-Bluemix.github.io
    ```

Step 3) run the application
    ```
    bundle exec jekyll serve
    ```

Website will be running locally on: http://127.0.0.1:4000/
