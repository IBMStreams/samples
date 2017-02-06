#this script parses the MD file for each sample (which was on the  documentation page)
#and generates a Readme.md file for each sample based on the header comments in the
import os

def generate_readme_md():
    md_dir = "/Users/natashad/Documents/dev/git/ibmstreams/samples/Examples-for-beginners"
    #for each md_File
    github_list=  open(md_dir + "/Sample_list")
    md_file_list = github_list.readlines()
    github_list2=  open(md_dir + "/double_samples","w")

    for sample_name in md_file_list:
        sample_name=sample_name.strip()
        #get the sample name
        neu="/Users/natashad/Documents/dev/git/ibmstreams/samples/Examples-for-beginners/" + sample_name + "/README1.md"
        old="/Users/natashad/Documents/dev/git/ibmstreams/samples/Examples-for-beginners/" + sample_name + "/README.md"
        new_readme=  open(neu, "w")
        old_readme= open(old, "r")
        first_line = old_readme.readline()
        first_line = first_line.strip()
        first_line = first_line + " Scala \n"
        new_readme.write(first_line)
        skipNext =False
        for line in old_readme.readlines():
            if skipNext is False:
                new_readme.write(line)
            else:
                print line.strip()
                skipNext = False
            if (line == "---\n"):
                github_list2.write(sample_name)
                new_readme.write(first_line)
                skipNext = True

        new_readme.close()
        old_readme.close()
        os.remove(old)
        os.rename(neu, old)
    github_list2.close()
    github_list.close()

if __name__ == '__main__':
    generate_readme_md()
