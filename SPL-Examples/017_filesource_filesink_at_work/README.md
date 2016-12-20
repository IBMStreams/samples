We have used the FileSource and the FileSink operators in other examples before. However, this example shows off the following intriguing features that will become handy in a lot of practical situations.

a) Automatic deletion of a file after the FileSource finishes reading all the records.
b) Flushing the sink file on demand after writing a certain number of tuples.
c) Ability of the FileSource to move the file once it reads all the content in that file.
d) Creating a fresh and new output sink file after writing a certain number of tuples.
e) Ability of the FileSource to keep reading from a hot file as new CSV records get written to the end of that file.

