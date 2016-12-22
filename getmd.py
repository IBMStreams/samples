import os

def generate_readme_md():
    md_dir = "/Users/natashad/Documents/dev/git/streamsx.documentation-gh-pages/samples/spl-for-beginner"
    #for each md_File
    github_list=  open(md_dir + "/file_list")
    md_file_list = github_list.readlines()
    #>>> files = [f.strip() for f in L if f.startswith("028")]
    with open("/Users/natashad/Documents/dev/git/samples/SPL-Examples/Sample_List") as sample_list_file :
        sample_list = sample_list_file.readlines()
        for sample_name in sample_list:
            sample_name=sample_name.strip()
            sample_source_files = [spl_file.strip() for spl_file in md_file_list if spl_file.startswith(sample_name)]
            with open("/Users/natashad/Documents/dev/git/samples/SPL-Examples/" + sample_name + "/README.md", "w") as readme:
                skip_line_break = True #some samples have multiple spl files, show them all in the readme with a line_Break
                for md_file in sample_source_files:#get the list of md files for each sample
                    if len(sample_source_files) > 1:
                        if skip_line_break is False:
                            readme.write("\n---\n")
                        if skip_line_break is True:
                            skip_line_break = False
                    found_delimiter = False
                    #print "for " + sample_name + ", opening " + md_dir + "/" + md_file
                    with open(md_dir + "/" + md_file) as source:
                        for line in source.readlines():
                            if found_delimiter is True:
                                readme.write(line)
                            if line.startswith("~~~~"):
                                if (found_delimiter is False): readme.write(line)
                                found_delimiter = not(found_delimiter)

    github_list.close()

if __name__ == '__main__':
    generate_readme_md()
